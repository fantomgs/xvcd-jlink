#ifndef _JLINKUTIL_H_INCLUDED
#define _JLINKUTIL_H_INCLUDED

int JlinkConnect(unsigned short freq, unsigned int coreId);
void JlinkClose();
void JlinkResetTarget(unsigned delay);
void printTDO(const unsigned char *buf, int bitLen);

#endif
