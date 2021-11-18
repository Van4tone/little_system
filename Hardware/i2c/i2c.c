#include "i2c.h"


void I2C_Init()
{
    SCL_DIR_OUT;
    SDA_DIR_OUT;
}


void I2C_Start()
{
    SDA_PIN_HIGH;
    NOP();
    SCL_PIN_HIGH;
    NOP();
    SDA_PIN_LOW;
    NOP(); 
    SCL_PIN_LOW;
    NOP();
}

void I2C_Stop()
{
    SDA_PIN_LOW;
    NOP();
    SCL_PIN_HIGH;
    NOP();
    SDA_PIN_HIGH;
    NOP();
}

void I2C_SendNack()
{
	SCL_PIN_LOW;
	NOP();
	SDA_PIN_HIGH;
	NOP();
	SCL_PIN_HIGH;
	NOP();
	SCL_PIN_LOW;
	NOP();
}

unsigned char I2C_SendByte(unsigned char val)
{
	unsigned char i,ack;
	for(i=0;i<8;i++)
	{
		if(val&0x80)
			SDA_PIN_HIGH;
		else
			SDA_PIN_LOW;
		val <<= 1;
		SCL_PIN_HIGH;
		NOP();
		SCL_PIN_LOW;
	}
	
	SDA_DIR_IN; // 主机发送完一个字节，读取从机应答信号
	NOP();
//	SCL_PIN_HIGH;
//	NOP();
	if(I2C_SDA == GPIO_ACTIVE_LOW)
		ack = 1;
	else
		ack = 0;
	
	SCL_PIN_LOW;
	NOP();
	SDA_DIR_OUT;
	return ack;
}


unsigned char I2C_ReadByte(u8 ack)
{
	unsigned char i,data;
	data = 0;
	SDA_DIR_IN;
	NOP();
	for(i=0;i<8;i++)
	{
		data <<= 1;
		SCL_PIN_HIGH;
		if(I2C_SDA == GPIO_ACTIVE_HIGH)
			data |= 0x01;
		else
			data |= 0x00;
		
		SCL_PIN_LOW;
		NOP();	
	}
	SDA_DIR_OUT; // 主机读完一个字节发送给丛机一个应答信号
	if(ack)
    	SDA_PIN_LOW;
	else
		SDA_PIN_HIGH;
	
    NOP();
    SCL_PIN_HIGH;
    NOP();
    SCL_PIN_LOW;
    NOP();
    
	return data;
}




