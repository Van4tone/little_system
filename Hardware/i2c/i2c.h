#ifndef __I2C_H_
#define __I2C_H_

#include "gpio_head.h"

#define SCL_DIR_OUT  (I2C_SCL_DIR = GPIO_MODE_OUT)
#define SDA_DIR_OUT  (I2C_SDA_DIR = GPIO_MODE_OUT)
#define SDA_DIR_IN	 (I2C_SDA_DIR = GPIO_MODE_IN)

#define SCL_PIN_HIGH (I2C_SCL = GPIO_ACTIVE_HIGH)
#define SDA_PIN_HIGH (I2C_SDA = GPIO_ACTIVE_HIGH)

#define SCL_PIN_LOW  (I2C_SCL = GPIO_ACTIVE_LOW)
#define SDA_PIN_LOW  (I2C_SDA = GPIO_ACTIVE_LOW)

void I2C_Init();

void I2C_Start();

void I2C_Stop();

unsigned char I2C_SendByte(unsigned char val);

unsigned char I2C_ReadByte(u8 ack);

void I2C_SendNack();

#endif