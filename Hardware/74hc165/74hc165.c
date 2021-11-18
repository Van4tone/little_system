
#include "74hc165.h"


void Key_Init()
{
	EXT_DOUT_DIR = GPIO_MODE_OUT;
	EXT_DOUT     = GPIO_ACTIVE_HIGH;
	
    
	EXT_CLK_DIR  = GPIO_MODE_OUT;
	EXT_CLK      = GPIO_ACTIVE_HIGH;
	
	EXT_LOAD_DIR = GPIO_MODE_OUT;
	EXT_LOAD     = GPIO_ACTIVE_HIGH;
	
	EXT_DIN_DIR  = GPIO_MODE_IN;
}


unsigned short int KEY_ReadData(void)
{
	unsigned char i;
	unsigned short int HcVal;
	HcVal = 0;
    
	EXT_CLK = GPIO_ACTIVE_HIGH;
	__delay_us(MS);
	EXT_LOAD = GPIO_ACTIVE_LOW;
	__delay_us(MS);
	EXT_LOAD = GPIO_ACTIVE_HIGH;
	__delay_us(MS);
	
	for(i=0;i<16;i++)
	{
		HcVal <<= 1;
        __delay_us(MS);
		EXT_CLK = GPIO_ACTIVE_LOW;
		__delay_us(MS);
		HcVal |= EXT_DIN;
		__delay_us(MS); 
		EXT_CLK = GPIO_ACTIVE_HIGH;
		__delay_us(MS);
	}
	//__delay_us(2);
  //  HcVal = (HcVal & 0xff00) | ((HcVal >> 1) & 0x00ff);
	HcVal &= (~(3<<6));
   
	HcVal &= (~(7<<13));
	
	return HcVal;
}
















