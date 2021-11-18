#ifndef __AT24C16_H_
#define __AT24C16_H_

#include "common.h"

void ReadDataAt24c16(u8 num,u16 addr,void *buf,u16 len);

u8 WriteData2At24c16(u8 num,u16 addr,void *buf, u16 len);

void E2Write(u16 addr,void *buf,u16 len);

u8 E2Read(u16 addr,void *buf,u16 len);

#endif