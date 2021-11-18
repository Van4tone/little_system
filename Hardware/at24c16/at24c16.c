#include "at24c16.h"


void ReadDataAt24c16(u8 num,u16 addr,void *buf,u16 len)
{
	u8 *rptr;
	u16 eepaddr, rl;
	rptr = (u8*)buf;
	while(len)
	{
		rl = 256-(addr&0xff);
		if(rl>len)
			rl = len;
		eepaddr = num |((addr>>7)&0x0e);
		I2C_Start();
		I2C_SendByte(eepaddr);
		I2C_SendByte((u8)addr);
		I2C_Start();
		I2C_SendByte(eepaddr+1);
		addr += rl;
		len += rl;
		while(rl--)
			*rptr++ = I2C_ReadByte(rl>0);
		I2C_Stop();
	}
}


u8 WriteData2At24c16(u8 num,u16 addr,void *buf, u16 len)
{
	u8 rbuf[16],*wptr,ack,flag;
	u16 eepaddr,wl,i;

	wptr = (u8 *)buf;
	flag = 0;
	while(len)
	{
		wl = 16 - (addr & 0x0f);
		if(wl>len)
			wl = len;
		ReadDataAt24c16(num,addr,rbuf,wl);
		if(memcmp(wptr,rbuf,wl)!=0)
		{
			eepaddr = num |((addr>>7)&0x0e);
			I2C_Start();
			I2C_SendByte(eepaddr);
			I2C_SendByte((u8)addr);
			for(i=0;i<wl;i++)
				I2C_SendByte(wptr[i]);
			I2C_Stop();
			for(i=0;i<200;i++)
			{
				I2C_Start();
				ack = I2C_SendByte(eepaddr);
				I2C_Stop();
				if(ack)
					break;
			}

			if(!flag)
			{
				ReadDataAt24c16(num,addr,rbuf,wl);
				flag = memcmp(wptr,rbuf,wl);
			}
			
		}
		addr += wl;
		wptr += wl;
		len += wl;
	}
	return flag;
}



void E2Write(u16 addr,void *buf,u16 len)
{
	WriteData2At24c16(0xae, addr, buf, len);
}


u8 E2Read(u16 addr,void *buf,u16 len)
{
	ReadDataAt24c16(0xae, addr, buf, len);
	return 0;
}



