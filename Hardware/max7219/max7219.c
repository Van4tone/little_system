
/* *******************/
#include"max7219.h"


void Max1_Write_Byte(unsigned char byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		MAX1_CLK = GPIO_ACTIVE_LOW;
    __delay_us(20);
        if(byte & 0x80)
            MAX1_DIN = GPIO_ACTIVE_HIGH;
        else
            MAX1_DIN = GPIO_ACTIVE_LOW;
     __delay_us(20);
		byte <<= 1;
		__delay_us(20);
		MAX1_CLK = GPIO_ACTIVE_HIGH;    
        __delay_us(20);
	}
}

void Seg_Write_Data(unsigned char address,unsigned char val)// 数码管显示函数
{
	MAX1_CS = GPIO_ACTIVE_LOW;
	__delay_us(20);
	Max1_Write_Byte(address);
	Max1_Write_Byte(val);
	MAX1_CS = GPIO_ACTIVE_HIGH;
	__delay_us(20);
}

void Max2_Write_Byte(unsigned char byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		MAX2_CLK = GPIO_ACTIVE_LOW;
   	 __delay_us(20);
        if(byte & 0x80)
            MAX2_DIN = GPIO_ACTIVE_HIGH;
        else
            MAX2_DIN = GPIO_ACTIVE_LOW;
     __delay_us(20);
		byte <<= 1;
      __delay_us(20);  
		MAX2_CLK = GPIO_ACTIVE_HIGH;    
        __delay_us(20);
	}
}

void Led_Write_Data(unsigned char address,unsigned char val)// led显示函数
{
	MAX2_CS = GPIO_ACTIVE_LOW;
    __delay_us(20);
	Max2_Write_Byte(address);
	Max2_Write_Byte(val);
	MAX2_CS = GPIO_ACTIVE_HIGH;
    __delay_us(20);
}


void Max7219_Init()
{
	MAX1_DIN_DIR = GPIO_MODE_OUT;
	MAX1_CLK_DIR	= GPIO_MODE_OUT;
	MAX1_CS_DIR	= GPIO_MODE_OUT;
    
	MAX1_CLK = GPIO_ACTIVE_HIGH;
    MAX1_CS  = GPIO_ACTIVE_HIGH;
    MAX1_DIN = GPIO_ACTIVE_HIGH;
    
    MAX2_DIN_DIR = GPIO_MODE_OUT;
	MAX2_CLK_DIR	= GPIO_MODE_OUT;
	MAX2_CS_DIR	= GPIO_MODE_OUT;
    
	MAX2_CLK = GPIO_ACTIVE_HIGH;
    MAX2_CS  = GPIO_ACTIVE_HIGH;
    MAX2_DIN = GPIO_ACTIVE_HIGH;
    
    Seg_Write_Data(SHUTDOWN, 0x01);// 正常模式
    Seg_Write_Data(INTENSITY, 0x02);//亮度
	Seg_Write_Data(DECODE_MODE, 0x00);//编码模式，编码模式
	Seg_Write_Data(SCAN_LIMIT,0x07); //开始扫描段码管 0-7
	Seg_Write_Data(DISPLAY_TEST,0x00); // 正常显示模式
    
    Led_Write_Data(SHUTDOWN, 0x01);
    Led_Write_Data(INTENSITY, 0x02);
	Led_Write_Data(DECODE_MODE, 0x00);//编码模式，非编码
	Led_Write_Data(SCAN_LIMIT,0x07);
	Led_Write_Data(DISPLAY_TEST,0x00);
}
















