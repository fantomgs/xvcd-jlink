#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "JLinkARM.h"
#include "JlinkUtil.h"

static void ErrorOut(const char *str)
{
  printf("\n***** Error: %s\n",str);
}

static void WarnOut(const char *str)
{
  printf("\nWARNING: %s\n",str);
}

static void InfoOut(const char *str)
{
  printf("Info: %s\n",str);
}

static void LogOut(const char *str)
{
  puts(str);
}

static void ShowDllInfo()
{
  int dllver = JLINKARM_GetDLLVersion();
  printf("DLL version: %d.%d.%d\n",dllver/10000,(dllver%10000)/100,dllver%100);
}

static void ShowHwInfo()
{
  char fwstr[0x100];
  JLINKARM_GetFirmwareString(fwstr,0x100);
  if (fwstr[0]==0)
  {
    puts("Can't get firmware info !");
    return;
  }
  printf("Firmware: %s\n",fwstr);
  unsigned hwver = JLINKARM_GetHardwareVersion();
  if (hwver==0)
     return;
  printf("HW version: %d.%d.%d\n",hwver/10000,(hwver%10000)/100,hwver%100);
}

static void CheckSpeed()
{
  short speed = JLINKARM_GetSpeed();
  if (speed==-1)
     puts("Using adaptive clocking");
  else
     printf("JTAG speed: %d kHz\n",speed);
}

void printTDO(const unsigned char *buf, int bitLen) {
  printf(" TDO: ");
  for (int i = 0; i < bitLen; i += 8 )
  {
    printf("%.2X", buf[(bitLen - i - 1) / 8]);
  }
  printf("\n");
}

static bool FindCore(unsigned int coreId)
{
//  wjraw 5,1f,00
//  wjraw 4,2,0
//  wjraw 32,00000000,00000000
  unsigned char buf[64];

  int r = JLINKARM_JTAG_StoreGetRaw("\x00", 0, "\x1f", 5);
  //printf("r = %d\n", r);
  JLINKARM_JTAG_StoreGetRaw("\x00", 0, "\x02", 4);
  JLINKARM_JTAG_StoreGetRaw("\x00\x00\x00\x00", buf, "\x00\x00\x00\x00", 32);
  //printf("result %x\n", x);
  //printf("IDCODE %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3]);
  //printTDO(buf, 32);
  int idcode = *(unsigned int*)buf;
  //printf("IDCODE %x\n", idcode);

  if(idcode == coreId) {
    printf("Found core id 0x%.8X\n", idcode);
    return true;
  } else {
    printf("Found unknown core id %08X\n", idcode);
    return coreId ? false : true;
  }
  return false;
}

int JlinkConnect(unsigned short speed, unsigned int coreId)
{
  JLINKARM_EnableLog(LogOut);
  //JLINKARM_SetLogFile("c:\\Temp\\jlink\\rd.log");
  JLINKARM_SetWarnOutHandler(WarnOut);
  if (JLINKARM_OpenEx(LogOut,ErrorOut))
  {
    puts("Open failed !");
    return 0;
  }
  puts("JLink opened successfully");
  JLINKARM_SetErrorOutHandler(ErrorOut);
  JLINKARM_EnableLog(InfoOut);
  JLINKARM_SetInitRegsOnReset(OFF);
  ShowDllInfo();
  ShowHwInfo();
  if (JLINKARM_HasError())
     return 0;
  printf("S/N: %d\n",JLINKARM_GetSN());
  char oemstr[0x100];
  JLINKARM_GetOEMString(oemstr);
  if (strlen(oemstr)>0)
     printf("OEM: %s\n",oemstr);
  JLINKARM_GetFeatureString(oemstr);
  if (strlen(oemstr)>0)
     printf("Features: %s\n",oemstr);
  printf("Capabilities: %X\n",JLINKARM_GetEmuCaps());
  HWSTATUS hwsts;
  if (JLINKARM_GetHWStatus(&hwsts)==JL_OK)
  {
    printf("VTarget = %d.%dV\n",hwsts.vtarget/1000,hwsts.vtarget%1000);
    if (hwsts.tck==0)
       puts("Error: TCK low !");
    if (hwsts.trst==0)
       puts("Error: TRST low !");
  }
  //JLINKARM_SetSpeed(TCKFREQ);
  JLINKARM_SetSpeed(speed);
  CheckSpeed();
  if (hwsts.vtarget<500)
  {
    puts("VTarget too low !");
    return 0;
  }
  if (!FindCore(coreId))
  {
    puts("Trying to connect with 100 kHz !");
	JlinkResetTarget(1);
    JLINKARM_SetSpeed(100);
    if (!FindCore(coreId))
       return 0;
  }
  return 1;
}

void JlinkClose()
{
  JLINKARM_Close();
  puts("JLink closed");
}

void JlinkResetTarget(unsigned delay)
{
  JLINKARM_SetResetDelay(delay);
  JLINKARM_ResetPullsRESET(ON);
  JLINKARM_Reset();
}
