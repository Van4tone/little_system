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

// can发送buf函数
u8 CanSendBuf(u8 *buf,u8 len)
{
	u8 buflen;
	u8 *txreg = (u8*)&TXB0D0;

	// 判断是否还在发送
	if(TXB0REQ == TXREQ_RUN_CMD)
		return  1;
	// 判断当前buf长度是否超过can的缓存
	if(len > DLC_LEN_8B)
		buflen = DLC_LEN_8B;
	else
		buflen = len;
	if(buflen == 0)
		return 0;

	// 配置can的发送长度
	TXB0DLC = buflen;
	while(buflen--)
	{
		*txreg++ = *buf++;
	}

	TXBOREQ = TXREQ_RUN_CMD;

	return 0;
}



