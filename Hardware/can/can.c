#include "can.h"


//can pin脚初始化
void CanPinInit()
{
	CAN_TX_DIR = GPIO_MODE_OUT;
	CAN_RX_DIR = GPIO_MODE_IN;
}

// Can 模式状态检测
u8 CheckCanModeStatus(u8 mode)
{
	u16 i;
	for(i=0;i<500;i++)
	{
		if((CANSTAT &(0x7<<5)) == mode)
			return 0;

	}
	return 1;
}

//can 模式配置
u8 CanModeCfig(u8 mode)
{
	CANCON = CANCON & (~(7<<5))|mode;
	return CheckCanModeStatus(mode);
}

//can 波特率设置
void SetCanPortBrate(u8 brate)
{

	switch(brate)
	{
		case CAN_BRATE_125K:
			BRGCON1 = 0x02;
			BRGCON2 = 0x90;
			BRGCON3 = 0x02;
			break;


			
		default:
			break;
	}
}
// can口功能设置
u8 CanPortInit()
{
	// can pins Set
	CanPinInit();

	//can mode set 
	if(CanModeCfig(CAN_CFIG_MODE)==1)
		return 1;
	
	//can brate set
	SetCanPortBrate(CAN_BRATE_125K);

	// 接收方ID
	TXB0SIDL = CAN_TX_ID_L;
	TXB0SIDH = CAN_TX_ID_H;

	// 通道优先级
	TXB0CON = TXPRI_H_3; //最高优先级
	TXB0DLC = DLC_LEN_8B ;// 默认字长

	// 初始发送缓存
	TXB0D0 = 0x00;
	TXB0D1 = 0x00;
	TXB0D2 = 0x00;
	TXB0D3 = 0x00;
	TXB0D4 = 0x00;
	TXB0D5 = 0x00;
	TXB0D6 = 0x00;
	TXB0D7 = 0x00;

	//接收缓存配置
	// 选择ID0过滤报文 过滤器为0 缓存为单缓冲
	RXB0CON = (RXM_ID0 << RXM_BIT) |(RB0DBEN_OFF<<RB0DBEN_BIT)|FILHT0_0;

	// 自身ID号
	RXF0SIDH = CAN_RX_ID_H;
	RXF0SIDL = CAN_RX_ID_L;

	//屏蔽的ID
	RXM0SIDH = 0xff;
	RXM0SIDL = 0xe0;

	//切换到正常通讯模式
	if(CanModeCfig(CAN_NORM_MODE)==1)
		return 1;
	
	//配置中断
	PIR3 = 0; // 清除标志位
	PIE3 = 5; // 使能中断，使能发送和接收的中断
	IPR3 = 0; // 设置优先级，发送和接收的都为最低优先级

	TXB0REQ = 0; // 初始化请求位
	RXB0FUL = 0; // 接收缓存清零
	
	return 0;
}

// 获取帧的校验位
u8 GetCanFrameCheckByte(u8 *frame,u8 len)
{
	u8 i = 0;
	u8 check = 0;
	while(i<len)
	{
		check += frame[i++];
	}
	return check + 1;
}

//将要发送的数据封装到帧的buf里
void PackCanSendMsg(u8 *buf,u8 len)
{
	u8 i = 3;
	u8 temp;
	if(len >(FRAME_LEN - 4))
		len = FRAME_LEN - 4;
	
	send_msg_frame.send_frame.len = len + 4;
	temp = send_msg_frame.send_frame.len - 1;
	while(i<temp)
	{
		send_msg_frame.buf[i] = buf[i-3];
		i++;
	}
	send_msg_frame.buf[i] = GetCanFrameCheckByte(send_msg_frame.buf, temp);
}

//CAN发送数据功能函数
void CanSendMsgFun(u8 *buf)
{
	u8 buf_len;
	u8 *start_buf;

	if(can_send_ctr.flag.start_status ==0)
	{
		if(buf[2] == 0)
			return;
		can_send_ctr.buf = buf;
		can_send_ctr.len = buf[2];
		can_send_ctr.flag.start_status = 1;
		can_send_ctr.flag.bus_status = 1;
	}

	if(can_send_ctr.len == 0)
	{
		can_send_ctr.times = 0;
		can_send_ctr.times = 59;
		return;
	}

	buf = can_send_ctr.buf;
		
}


void CanMsgPack2Send(u8 *buf,u8 len)
{
	if(can_send_ctr.flag.bus_status ==1)
		return;
	if(len == 0)
		return;
	PackCanSendMsg(buf,len);
	CanSendMsgFun(send_msg_frame.buf);
}


// can信息封包及功能初始化
void CanFunctionInit()
{
	//can_recv_ctr.flag = 0;
	//can_send_ctr.flag = 0;

	can_recv_ctr.buf = recv_msg_frame.buf;
	send_msg_frame.send_frame.head0 = FRAME_HEAD0;
	send_msg_frame.send_frame.head1 = FRAME_HEAD1;
	send_msg_frame.send_frame.len   = FRAME_LEN;

	CanPortInit();
}


