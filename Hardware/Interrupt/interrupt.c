#include "interrupt.h"


void IntruptInit(void)
{
	INTCON &= 0x80;
	INTCON3 &= 0xFC;

	PIR1 &= 0x00;
	PIR2 &= 0x00;
	PIR3 &= 0x00;

	RCON |= 0x80;
	INTCON2 &= 0xF8;
	INTCON3 &= 0x1F;

	IPR1 &= 0x00;
	IPR2 &= 0x00;
	IPR3 &= 0x00;

	INTCON &= 0xC7;
	INTCON2 &= 0x0F;
	INTCON3 &= 0xE7;

	PEIE = 1;

	PIE1 & = 0x00;
	PIE2 & = 0x00;
	PIE3 & = 0x00;
	
}
