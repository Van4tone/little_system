#ifndef __CAN_H_
#define __CAN_H_

#include "common.h"

// can brate 
#define CAN_BRATE_100K 	0
#define CAN_BRATE_125K 	1
#define CAN_BRATE_200K 	2
#define CAN_BRATE_500K 	3

//can work mode
#define CAN_CFIG_MODE 0x80
#define CAN_NORM_MODE 0x00

/**********RXB0CON*******************/
// 模式0（正常模式）中接收报文的过滤标志id配置
#define RXM_BIT   	0x05
#define RXM_ID10  	0x00
#define RXM_ID0   	0x01
#define RXM_ID1   	0x02
#define RXM_ID_ALL 	0x03

// 接收报文远程发送请求
#define RXRTRRO_BIT  0x03
#define RXRTRRO_OK   0x01
#define RXRTRRO_NO   0x00

// 接收缓存区0双缓存区使能位
#define RB0DBEN_BIT  0x02
#define RB0DBEN_EN   0x01
#define RB0DBEN_OFF  0x00

// 过滤器选择
#define FILHT0_BIT   0
#define FILHT0_1     1
#define FILHT0_0     0



// ID 标识符
#define TXB0SID0  0x5599
#define RXB0SID0  0x9955
//
#define CAN_TX_ID_L (TXB0SID0 & 0xe0)
#define CAN_TX_ID_H (TXB0SID0 >>8)

#define CAN_RX_ID_L (RXB0SID0 & 0xe0)
#define CAN_RX_ID_H (RXB0SID0 >>8)

//扩展位
#define TXPRI_H_3 	3
#define TXPRI_H_2 	2
#define TXPRI_H_1 	1

// DLC3:0 配置数据长度位
#define DLC_LEN_8B 8
#define DLC_LEN_7B 7

//֡ 帧头设置及帧长度设置
#define FRAME_HEAD0 0xaa
#define FRAME_HEAD1 0x55
#define FRAME_LEN 30

//msg 长度设置
#define MSG_DATA_LEN (FRAME_LEN-4)

typedef struct CAN_SEND_FLAG {
	unsigned start_status : 1;
	unsigned bus_status : 1;
	unsigned send_en : 1;
	unsigned err_cnt : 5;
}can_send_flag_type;

typedef struct CAN_S_MSG {
	u16 len;
	u16 pre_len;
	u8 *buf;
	u8 times;
	can_send_flag_type flag;
}can_send_ctr_type;

typedef struct CAN_R_FLAG {
	unsigned start_s1 : 1;
	unsigned start_s2 : 1;
	unsigned end_s : 1;
	unsigned err_cnt : 5;
}can_recv_flag_type;

typedef struct CAN_R_MSG {
	u16 len;
	u16 init_len;
	u8 *buf;
	u8 time;
	can_recv_flag_type flag;
}can_recv_ctr_type;


typedef struct CAN_SEND_FRAME {
	u8 head0;
	u8 head1;
	u8 len;

	u8 data[MSG_DATA_LEN];
	u8 check_byte;
}can_send_frame_type;

typedef union CAN_FRAME {
	u8 buf[FRAME_LEN];
	can_send_frame_type send_frame;
}can_frame_msg_type;


can_frame_msg_type send_msg_frame;
can_frame_msg_type recv_msg_frame;

can_recv_ctr_type can_recv_ctr;
can_send_ctr_type can_send_ctr;

void CanPinInit();

u8 CheckCanModeStatus(u8 mode);

u8 CanModeCfig(u8 mode);

void SetCanPortBrate(u8 brate);

u8 CanPortInit();

u8 GetCanFrameCheckByte(u8 *frame,u8 len);

void CanFunctionInit();




#endif