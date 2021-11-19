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



