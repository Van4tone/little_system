#include "can_fun.h"




/****************************************************************************
****************************** 接收函数     *************************************
****************************************************************************/


void CanIntRecvFun()
{
	u8 i = 0;
	u8 *readreg = GetCanRecvBufAddr();

	// 判断是否第一次读取
	if(can_recv_ctr.flag.start_s1 == 0)
	{
		if(readreg[0]== FRAME_HEAD0 && readreg[1] == FRAME_HEAD1)
		{
			can_recv_ctr.flag.start_s1 = 1;
			can_recv_ctr.len = readreg[2];
			can_recv_ctr.buf = recv_msg_frame.buf;
		}
	}

	//判断buf是否大于缓存的大小
	if(can_recv_ctr.len > DLC_LEN_8B)
	{
		i = 8;
		can_recv_ctr.len = DLC_LEN_8B;
	}
	else
	{
		i = can_recv_ctr.len;
		can_recv_ctr.len = 0;
	}

	// 取值
	while(i--)
	{
		*can_recv_ctr.buf++ = *readreg++;
	}

	//判断是否接收完成
	if(can_recv_ctr.len == 0)
	{
		can_recv_ctr.flag.end_s = 1;
		can_recv_ctr.times = 0;
	}
	else
		can_recv_ctr.times = 59;

	ReleaseCanRecvBuf();
	
}



/****************************************************************************

***************************** 发送函数      *************************************
****************************************************************************/

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

	if(can_send_ctr.len > DLC_LEN_8B)
	{
		can_send_ctr.len = DLC_LEN_8B;
		buf_len = DLC_LEN_8B;
		can_send_ctr.buf += DLC_LEN_8B;
	}
	else
	{
		buf_len = can_send_ctr.len;
		can_send_ctr.len = 0;
	}
	can_send_ctr.times = 59;
	
	CanSendBuf(start_buf,buf_len);
		
}



//打包数据并发送
void CanMsgPack2Send(u8 *buf,u8 len)
{
	if(can_send_ctr.flag.bus_status ==1)
		return;
	if(len == 0)
		return;
	PackCanSendMsg(buf,len);
	CanSendMsgFun(send_msg_frame.buf);
}

// can read msg check
u8 CanRecvMsgCheck(u8 *buf, u8 len)
{
	if(buf[len - 1] == GetCanFrameCheckByte(buf, len - 1))
		return 0;
	else
		return 1;
}




u8 CanCheckRecvMsg(u8 *buf,u8 len)
{
	if(buf[len-1]==GetCanFrameCheckByte(buf,len-1))
		return 0;
	else
		return 1;
}

void CanReadMsgDeal()
{
	if(can_recv_ctr.flag.end_s == 1)
	{
		if(CanCheckRecvMsg(recv_msg_frame.buf,recv_msg_frame.send_frame.len)==0)
		{
			MsgResponseFun();
			can_send_ctr.flag.err_cnt = 0;
		}
		else
		{
			can_send_ctr.flag.err_cnt++;
			CanFunctionInit();
			MsgErrReponse();
		}
		can_send_ctr.flag.start_status = 0;
		can_send_ctr.flag.bus_status = 0;
		can_send_ctr.times = 0;
		can_recv_ctr.times = 0;
		can_recv_ctr.flag.start_s1 = 0;
		can_recv_ctr.flag.end_s = 0;

		if(can_send_ctr.flag.err_cnt > CYCLE_CNT_MAX)
		{
			can_send_ctr.flag.err_cnt = CYCLE_CNT_MAX;
			
			// 错误提示码
			/**************/
		}
	}
}




// can信息封包及功能初始化
u8 CanFunctionInit()
{
	ClearParame(&can_recv_ctr.flag);
	ClearParame(&can_send_ctr.flag);

	can_recv_ctr.buf = recv_msg_frame.buf;
	send_msg_frame.send_frame.head0 = FRAME_HEAD0;
	send_msg_frame.send_frame.head1 = FRAME_HEAD1;
	send_msg_frame.send_frame.len   = FRAME_LEN;

	CanPortInit();
	return 0;
}

