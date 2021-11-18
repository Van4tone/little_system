#ifndef __GPIO_HEAD_H_
#define __GPIO_HEAD_H_

#include <xc.h>

#define _XTAL_FREQ 6000000

// max7219 no1 pin
#define MAX1_DIN_DIR    	TRISD1
#define MAX1_DIN			LATD1

#define MAX1_CLK_DIR     	TRISD0
#define MAX1_CLK			LATD0

#define MAX1_CS_DIR			TRISC3
#define MAX1_CS				LATC3

// max7219 no2 pin
#define MAX2_DIN_DIR    	TRISC5
#define MAX2_DIN			LATC5

#define MAX2_CLK_DIR     	TRISD0
#define MAX2_CLK			LATD0

#define MAX2_CS_DIR			TRISC4
#define MAX2_CS				LATC4

//报警灯 pin
#define ALARM_LAMP_DIR     	TRISD4
#define ALARM_LAMP         	RD4

// 扩展按键芯片
#define EXT_DOUT_DIR  		TRISC7
#define EXT_CLK_DIR   		TRISC6
#define EXT_DIN_DIR   		TRISD3
#define EXT_LOAD_DIR  		TRISD2

#define EXT_DOUT   			RC7
#define EXT_CLK    			RC6
#define EXT_DIN    			RD3
#define EXT_LOAD   			RD2

//speaker pin
#define SPKER_SWITCH_DIR   TRISD5 
#define SPKER_SWITCH       RD5

#define SPKER_SOURCE_DIR   TRISC0
#define SPKER_SOURCE       RC0

// Timer define
#define PRIORITY_LOW 		0
#define PRIORITY_HIGH 		1

// I2C pin
#define I2C_SDA_DIR      TRISC1
#define I2C_SDA          RC1

#define I2C_SCL_DIR      TRISC2
#define I2C_SCL          RC2

//CAN Pin
#define CAN_RX_DIR		TRISB3
#define CAN_TX_DIR		TRISB2

//功率模拟信号
#define P_AD_DIR		TRISA0
//电压模拟信号
#define U_AD_DIR		TRISA1
//电流模拟信号
#define I_AD_DIR		TRISA2

// 极板电阻模拟信号
#define REM_AD_DIR		TRISA3
// 极板模式检测信号
#define PLATE_MODE_DIR 	TRISE1

//电源电压模拟信号
#define VSENSE_DIR	TRISA5
#define VSENSE		RA5
//功率启动信号
#define POWER_START_DIR       TRISB0
#define POWER_START			  RB0
//功率停止信号
#define POWER_STOP_DIR	TRISD7
#define POWER_STOP		RD7
//功率模式检测信号
#define MODE_AD_DIR		TRISE0
#define MODE_AD			RE0

//大血管闭合启动信号
#define LIGASURE_ON_DIR TRISB1
#define LIGASURE_ON		RB1

//大血管闭合检测信号
#define LIGASURE_CHECK_DIR	TRISA4
#define LIGASURE_CHECK	    RA4

//大血管闭合阻值检测信号
#define R_AD_DIR        TRISE2

// 切凝继电器输入检测
#define CUT_COAG_CHECK_DIR TRISD6
#define CUT_COAG_CHECK     RD6

//清楚看门狗
#define CLR_WDT_DIR		TRISB5
#define CLR_WDT			RB5

//连接两地信号
#define CONNECT_DIR		TRISB4
#define CONNECT			RB4

// GPIO 高低电平及输入输出定义
#define GPIO_ACTIVE_HIGH  	1
#define GPIO_ACTIVE_LOW   	0

#define GPIO_MODE_OUT   	0
#define GPIO_MODE_IN  		1

// 状态及标志位
#define STATUS_ON  			1
#define STATUS_OFF  		0
#define CLEAR_FLAG			0

#define SUCCESS 			0
#define FAILURE				1

#define START               1
#define STOP                0

typedef  unsigned char u8;
typedef  unsigned short int u16;
typedef  short int s16;
#endif