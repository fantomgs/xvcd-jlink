#include <stdio.h>

#include "JlinkARM.h"
#include "JlinkUtil.h"

#include "io_jlink.h"

void io_close(void);

int io_init(unsigned short jtagSpeed, unsigned int coreId)
{
  if (!JlinkConnect(jtagSpeed, coreId))
  {
    puts("Connect failed !");
    return 1;
  }
  puts("Connected successfully");
  return 0;
}

int io_scan(const unsigned char *TMS, const unsigned char *TDI, unsigned char *TDO, unsigned int bits)
{
  unsigned char buffer[2*16384];
  int i, res;
  int r, t;

  if (bits > sizeof(buffer)/2)
  {
    fprintf(stderr, "FATAL: out of buffer space for %d bits\n", bits);
    return -1;
  }

  r = JLINKARM_JTAG_StoreGetRaw(TDI, TDO, TMS, bits);
  if (!r)
  {
    perror("FATAL: failed to execute raw command\n");
    return -1;
  }

  return 0;
}

void io_close(void)
{
  JlinkClose();
}
