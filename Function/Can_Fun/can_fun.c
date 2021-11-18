#include "can_fun.h"


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
		if(CanCheckRecvMsg(recv_msg_frame.buf,recv_msg_frame.send_frame.len)==0);
			
			
	}
}



