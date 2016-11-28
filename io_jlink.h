#ifndef __IO_JLINK_H__
#define __IO_JLINK_H__

void io_close(void);
int io_init(unsigned short jtagSpeed, unsigned int coreId);
int io_scan(const unsigned char *TMS, const unsigned char *TDI, unsigned char *TDO, unsigned int bits);

#endif