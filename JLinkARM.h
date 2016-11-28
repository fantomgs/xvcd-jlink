#ifndef _JLINKARM_H_INCLUDED
#define _JLINKARM_H_INCLUDED

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JLINKAPI __cdecl
#define ON  1
#define OFF 0

//reset types
#define RESET_SW 6
//BP flags
#define BP_T 0x00000002
#define BP_A 0x00000003
#define BP_S 0x000000F0
#define BP_H 0xFFFFFF00
//regs
//cpu family not 3 (arm7/9 is 7)
#define CPSR    8
#define PC      9
#define R15     9

#define R0      0
#define R1      1
#define R2      2
#define R3      3
#define R4      4
#define R5      5
#define R6      6
#define R7      7
//user
#define USR_R8      10
#define USR_R9      11
#define USR_R10     12
#define USR_R11     13
#define USR_R12     14
#define USR_R13     15
#define USR_SP      15
#define USR_R14     16
#define USR_LR      16
//FIQ
#define FIQ_SPSR    17
#define FIQ_R8      18
#define FIQ_R9      19
#define FIQ_R10     20
#define FIQ_R11     21
#define FIQ_R12     22
#define FIQ_R13     23
#define FIQ_SP      23
#define FIQ_R14     24
#define FIQ_LR      24
//SVC
#define SVC_SPSR    25
#define SVC_R13     26
#define SVC_SP      26
#define SVC_R14     27
#define SVC_LR      27
//ABT
#define ABT_SPSR    28
#define ABT_R13     29
#define ABT_SP      29
#define ABT_R14     30
#define ABT_LR      30
//IRQ
#define IRQ_SPSR    31
#define IRQ_R13     32
#define IRQ_SP      32
#define IRQ_R14     33
#define IRQ_LR      33
//UND
#define UND_SPSR    34
#define UND_R13     35
#define UND_SP      35
#define UND_R14     36
#define UND_LR      36

typedef void (*OUTFUNC)(const char *);

typedef struct
{
  unsigned short vtarget;
  unsigned char  tck;
  unsigned char  tdi;
  unsigned char  tdo;
  unsigned char  tms;
  unsigned char  tres;
  unsigned char  trst;
} HWSTATUS;

typedef struct
{
  int            num_devs;
  unsigned short total_ir,us1;
  unsigned       core_id;
  unsigned       u4,u5;
  unsigned char  core_type,c1,c2,c3;
  unsigned       u7,u8;
} IDDATA;

typedef enum
{
  JL_OK = 0
} JLRESULT;

typedef enum
{
  DEVICE_FAMILY_CORTEXM3 = 3,
  DEVICE_FAMILY_XSCALE = 5
} DEVICE_FAMILY;

typedef enum
{
  CORE_ARM7 = 4,
  CORE_ARM9 = 5
} CORE_TYPE;

// JLINKARM_AddMirrorArea
JLRESULT JLINKAPI JLINKARM_CP15_ReadReg(unsigned index, unsigned *val);
// JLINKARM_Clock
void JLINKAPI JLINKARM_Close();
JLRESULT JLINKAPI JLINKARM_ClrBPEx(unsigned handle);
// JLINKARM_ClrBP
// JLINKARM_ClrError
// JLINKARM_ClrRESET
// JLINKARM_ClrTDI
// JLINKARM_ClrTMS
// JLINKARM_ClrTRST
// JLINKARM_ClrWP
// JLINKARM_ConfigJTAG
// JLINKARM_DownloadECode
// JLINKARM_ETM_IsPresent
// JLINKARM_ETM_ReadReg
// JLINKARM_ETM_StartTrace
// JLINKARM_ETM_WriteReg
// JLINKARM_EnableCheckModeAfterWrite
// JLINKARM_EnableFlashCache
void JLINKAPI JLINKARM_EnableLog(OUTFUNC LogOutHandler);
void JLINKAPI JLINKARM_EnableLogCom(OUTFUNC LogComOutHandler);
// JLINKARM_EnablePerformanceCnt
// JLINKARM_EnableSoftBPs
// JLINKARM_ExecCommand
// JLINKARM_ExecECode
// JLINKARM_FindBP
// JLINKARM_GetBPInfo
// JLINKARM_GetCompileDateTime
// JLINKARM_GetConfigData
int JLINKAPI JLINKARM_GetDLLVersion();
DEVICE_FAMILY JLINKAPI JLINKARM_GetDeviceFamily();
JLRESULT JLINKAPI JLINKARM_GetDeviceId(int DeviceIndex);
// JLINKARM_GetEmbeddedFWVersion
unsigned JLINKAPI JLINKARM_GetEmuCaps();
void JLINKAPI JLINKARM_GetFeatureString(char *str);
void JLINKAPI JLINKARM_GetFirmwareString(char *fwstr,unsigned maxlen);
// JLINKARM_GetHWInfo
JLRESULT JLINKAPI JLINKARM_GetHWStatus(HWSTATUS *sts);
unsigned JLINKAPI JLINKARM_GetHardwareVersion();
// JLINKARM_GetIRLen
// JLINKARM_GetId
void JLINKAPI JLINKARM_GetIdData(IDDATA *iddata);
// JLINKARM_GetNumBPUnits
// JLINKARM_GetNumBPs
// JLINKARM_GetNumWPs
void JLINKAPI JLINKARM_GetOEMString(char *str);
// JLINKARM_GetPerformanceCnt
// JLINKARM_GetRegisterName
unsigned JLINKAPI JLINKARM_GetSN();
// JLINKARM_GetScanLen
// JLINKARM_GetSelDevice
short JLINKAPI JLINKARM_GetSpeed();
// JLINKARM_GetSpeedInfo
// JLINKARM_GetU32
void JLINKAPI JLINKARM_Go(void);
// JLINKARM_GoAllowSim
// JLINKARM_GoHalt
// JLINKARM_GoIntDis
unsigned char JLINKAPI JLINKARM_Halt(void);
int JLINKAPI JLINKARM_HasError();
// JLINKARM_IsConnected
int JLINKAPI JLINKARM_IsHalted();
// JLINKARM_IsOpen
// JLINKARM_JTAG_GetData
JLRESULT JLINKAPI JLINKARM_JTAG_GetDeviceId(int DeviceIndex);
// JLINKARM_JTAG_GetU16
// JLINKARM_JTAG_GetU32
// JLINKARM_JTAG_GetU8
// JLINKARM_JTAG_StoreData
// JLINKARM_JTAG_StoreGetData
// JLINKARM_JTAG_StoreGetRaw
int JLINKAPI JLINKARM_JTAG_StoreGetRaw(const unsigned char *tdi, unsigned char *p_retVal, const unsigned char *tms, unsigned int bitCount);
// JLINKARM_JTAG_StoreInst
// JLINKARM_JTAG_StoreRaw
// JLINKARM_JTAG_SyncBits
// JLINKARM_JTAG_SyncBytes
// JLINKARM_JTAG_WriteData
// JLINKARM_MeasureCPUSpeed
// JLINKARM_MeasureRTCKReactTime
// JLINKARM_MeasureSCLen
JLRESULT JLINKAPI JLINKARM_Open();
JLRESULT JLINKAPI JLINKARM_OpenEx(OUTFUNC LogOutHandler,OUTFUNC ErrorOutHandler);
JLRESULT JLINKAPI JLINKARM_ReadCodeMem(unsigned addr, unsigned len, void *buf);
int JLINKAPI JLINKARM_ReadDCC(void *buf,int count,int timeout);
void JLINKAPI JLINKARM_ReadDCCFast(void *buf,int count);
// JLINKARM_ReadEmu
// JLINKARM_ReadEmuConfigMem
// JLINKARM_ReadICEReg
JLRESULT JLINKAPI JLINKARM_ReadMem(unsigned addr, unsigned len, void *buf);
JLRESULT JLINKAPI JLINKARM_ReadMemHW(unsigned addr, unsigned len, void *buf);
JLRESULT JLINKAPI JLINKARM_ReadMemIndirect(unsigned addr, unsigned len, void *buf);
JLRESULT JLINKAPI JLINKARM_ReadMemU16(unsigned addr, unsigned count, void *buf);
JLRESULT JLINKAPI JLINKARM_ReadMemU32(unsigned addr, unsigned count, void *buf);
JLRESULT JLINKAPI JLINKARM_ReadMemU8(unsigned addr, unsigned count, void *buf);
unsigned JLINKAPI JLINKARM_ReadReg(unsigned reg);
void JLINKAPI JLINKARM_Reset(void);
void JLINKAPI JLINKARM_ResetNoHalt(void);
void JLINKAPI JLINKARM_ResetPullsRESET(bool xxx);
// JLINKARM_ResetPullsTRST
// JLINKARM_ResetTRST
// JLINKARM_RestoreWorkArea
// JLINKARM_SaveWorkArea
// JLINKARM_SelDevice
// JLINKARM_SelectDeviceFamily
// JLINKARM_SelectIP
// JLINKARM_SelectUSB
// JLINKARM_SetBP
unsigned JLINKAPI JLINKARM_SetBPEx(unsigned addr, unsigned flags);
// JLINKARM_SetEndian
void JLINKAPI JLINKARM_SetErrorOutHandler(OUTFUNC ErrorOutHandler);
// JLINKARM_SetFlashArea
int JLINKAPI JLINKARM_SetInitRegsOnReset(int init);
void JLINKAPI JLINKARM_SetLogFile(const char *file);
// JLINKARM_SetMaxSpeed
// JLINKARM_SetRESET
void JLINKAPI JLINKARM_SetResetDelay(unsigned delay);
// JLINKARM_SetResetPara
char * JLINKAPI JLINKARM_SetResetType(unsigned type); //returns type string
void JLINKAPI JLINKARM_SetSpeed(unsigned short kHz); //0-50000,-1-auto
// JLINKARM_SetTDI
// JLINKARM_SetTMS
// JLINKARM_SetTRST
// JLINKARM_SetWP
void JLINKAPI JLINKARM_SetWarnOutHandler(OUTFUNC WarnOutHandler);
// JLINKARM_SimulateInstruction
// JLINKARM_Step
// JLINKARM_StepComposite
// JLINKARM_StoreBits
// JLINKARM_StoreCmd
// JLINKARM_StoreData
// JLINKARM_TRACE_AddInst
// JLINKARM_TRACE_AddItems
// JLINKARM_TRACE_Control
// JLINKARM_TRACE_Read
// JLINKARM_Test
// JLINKARM_UpdateFirmware
// JLINKARM_UpdateFirmwareIfNewer
// JLINKARM_WaitDCCRead
// JLINKARM_WriteBits
int JLINKAPI JLINKARM_WriteDCC(void *buf,int count,int timeout);
void JLINKAPI JLINKARM_WriteDCCFast(void *buf,int count);
// JLINKARM_WriteEmu
// JLINKARM_WriteEmuConfigMem
// JLINKARM_WriteICEReg
unsigned JLINKAPI JLINKARM_WriteMem(unsigned addr, unsigned len, const void *buf);
unsigned JLINKAPI JLINKARM_WriteMemDelayed(unsigned addr, unsigned len, const void *buf);
unsigned JLINKAPI JLINKARM_WriteMemHW(unsigned addr, unsigned len, const void *buf);
JLRESULT JLINKAPI JLINKARM_WriteReg(unsigned reg, unsigned val);
unsigned JLINKAPI JLINKARM_WriteU16(unsigned addr, unsigned short val);
unsigned JLINKAPI JLINKARM_WriteU32(unsigned addr, unsigned val);
unsigned JLINKAPI JLINKARM_WriteU8(unsigned addr, unsigned char val);

JLRESULT JLINKAPI JLINK_Open();
JLRESULT JLINKAPI JLINK_OpenEx(OUTFUNC LogOutHandler,OUTFUNC ErrorOutHandler);
// JLINK_CP15_ReadReg
// JLINK_Clock
// JLINK_Close
// JLINK_ClrBP
// JLINK_ClrBPEx
// JLINK_ClrError
// JLINK_ClrRESET
// JLINK_ClrTDI
// JLINK_ClrTMS
// JLINK_ClrTRST
// JLINK_ClrWP
// JLINK_ConfigJTAG
// JLINK_ETM_IsPresent
// JLINK_ETM_ReadReg
// JLINK_ETM_StartTrace
// JLINK_ETM_WriteReg
// JLINK_EnableCheckModeAfterWrite
// JLINK_EnableFlashCache
// JLINK_EnableLog
// JLINK_EnableLogCom
// JLINK_EnableSoftBPs
// JLINK_ExecCommand
// JLINK_FindBP
// JLINK_GetCompileDateTime
// JLINK_GetConfigData
// JLINK_GetDLLVersion
unsigned JLINKAPI JLINK_GetEmuCaps();
void JLINKAPI JLINK_GetFeatureString(char *str);
// JLINK_GetFirmwareString
JLRESULT JLINKAPI JLINK_GetHWStatus(HWSTATUS *sts);
unsigned JLINKAPI JLINK_GetHardwareVersion();
// JLINK_GetIRLen
// JLINK_GetId
void JLINKAPI JLINK_GetIdData(IDDATA *iddata);
// JLINK_GetNumBPUnits
// JLINK_GetNumBPs
// JLINK_GetNumWPs
void JLINKAPI JLINK_GetOEMString(char *str);
unsigned JLINKAPI JLINK_GetSN();
// JLINK_GetScanLen
// JLINK_GetSelDevice
short JLINKAPI JLINK_GetSpeed();
// JLINK_GetU32
// JLINK_Go
// JLINK_GoAllowSim
// JLINK_GoIntDis
// JLINK_Halt
int JLINKAPI JLINK_HasError();
// JLINK_IsConnected
// JLINK_IsHalted
// JLINK_IsOpen
JLRESULT JLINKAPI JLINK_JTAG_GetDeviceId(int DeviceIndex);
// JLINK_JTAG_StoreData
//JLINK_JTAG_StoreGetRaw
// JLINK_JTAG_SyncBits
// JLINK_JTAG_SyncBytes
// JLINK_MeasureSCLen
// JLINK_ReadCodeMem
int JLINKAPI JLINK_ReadDCC(void *buf,int count,int timeout);
void JLINKAPI JLINK_ReadDCCFast(void *buf,int count);
// JLINK_ReadEmuConfigMem
// JLINK_ReadICEReg
// JLINK_ReadMem
// JLINK_ReadMemHW
// JLINK_ReadMemU16
// JLINK_ReadMemU32
// JLINK_ReadMemU8
// JLINK_ReadReg
// JLINK_Reset
// JLINK_ResetNoHalt
// JLINK_ResetPullsRESET
// JLINK_ResetPullsTRST
// JLINK_ResetTRST
// JLINK_SelDevice
// JLINK_SelectIP
// JLINK_SelectUSB
// JLINK_SetBP
// JLINK_SetBPEx
// JLINK_SetEndian
// JLINK_SetErrorOutHandler
// JLINK_SetFlashArea
int JLINKAPI JLINK_SetInitRegsOnReset(int init);
void JLINKAPI JLINK_SetLogFile(const char *file);
// JLINK_SetMaxSpeed
// JLINK_SetRESET
// JLINK_SetResetDelay
// JLINK_SetResetPara
// JLINK_SetResetType
void JLINKAPI JLINK_SetSpeed(short kHz); //0-50000, -1-auto
// JLINK_SetTDI
// JLINK_SetTMS
// JLINK_SetTRST
// JLINK_SetWP
// JLINK_SimulateInstruction
// JLINK_Step
// JLINK_StepComposite
// JLINK_StoreBits
// JLINK_Test
// JLINK_UpdateFirmware
// JLINK_UpdateFirmwareIfNewer
// JLINK_WaitDCCRead
int JLINKAPI JLINK_WriteDCC(void *buf,int count,int timeout);
void JLINKAPI JLINK_WriteDCCFast(void *buf,int count);
// JLINK_WriteEmuConfigMem
// JLINK_WriteICEReg
// JLINK_WriteMem
// JLINK_WriteMemDelayed
// JLINK_WriteReg
// JLINK_WriteU16
// JLINK_WriteU32
// JLINK_WriteU8
// _JLINK_JTAG_StoreGetData@12
// _JLINK_JTAG_StoreInst@8

#ifdef __cplusplus
};
#endif

#endif
