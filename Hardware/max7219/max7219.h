#ifndef __MAX7219_H_
#define __MAX7219_H_

#include "gpio_head.h"

// 寄存器地址
#define NO_OP		 0x00 // 非操作
#define SEGMENT0     0x01
#define SEGMENT1     0x02
#define SEGMENT2     0x03
#define SEGMENT3     0x04
#define SEGMENT4     0x05
#define SEGMENT5     0x06
#define SEGMENT6     0x07
#define SEGMENT7     0x08
#define DECODE_MODE  0x09 // 编码方式
#define INTENSITY	 0x0A // 亮度
#define SCAN_LIMIT   0x0B // 扫描范围
#define SHUTDOWN     0x0C // 关机
#define DISPLAY_TEST 0x0F // 显示测试

// 数码管编号
#define SEG_1           2
#define SEG_2           6
#define SEG_3 			8
#define SEG_4 			4 
#define SEG_5 			3
#define SEG_6 			7 
#define SEG_7 			5
#define SEG_8 			1

// LED序列 编号
//切模式指示灯（纯切、切1、切2、切3）
#define DIG_0           1

//极板粘贴面积显示 10位，dig2 提供8bit dig6提供2bit
#define DIG_2 			3
#define DIG_6 			7

//手控脚控模式显示，单极/双极显示，极板面积显示/关闭
#define DIG_4 			5
/********************************************************/
// 切模式启动显示黄光 8个led
#define DIG_1   		2

//凝启动显示蓝光 8个led
#define DIG_5			6

//双极启动显示蓝光 8个led
#define DIG_7 			8

// 字符val
#define CHAR_A   10
#define CHAR_F   15

#define CHAR_O   38


#define CHAR_o   20
#define CHAR_n   27

#define CHAR_P   21
#define CHAR_I   26
#define CHAR_U   24
#define CHAR_C   12
#define CHAR_G   16

#define CHAR_MidLine 32 // - 中杠
#define CHAR_NULL 35 //" " 不显示




void Max1_Write_Byte(unsigned char byte);

void Seg_Write_Data(unsigned char address,unsigned char val);

void Max2_Write_Byte(unsigned char byte);

void Led_Write_Data(unsigned char address,unsigned char val);

void Max7219_Init();

#endif
