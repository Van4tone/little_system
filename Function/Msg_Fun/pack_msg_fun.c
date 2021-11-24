#include "pack_msg_fun.h"


void MsgPackageInit() // 要发送的信息包 buffer封装
{
	msg_package_manager.cmd = CMD_MSG_MODE;
	msg_package_manager.cfig_msg_send_manager = (cfig_mode_msg_type*)(send_msg_frame.buf + 3);
	msg_package_manager.cfig_msg_recv_manager = (cfig_mode_msg_type*)(recv_msg_frame.buf + 3);

	msg_package_manager.run_msg_send_manager = (run_mode_msg_type*)(send_msg_frame.buf + 3);
	msg_package_manager.run_msg_recv_manager = (run_mode_msg_type*)(recv_msg_frame.buf + 3);
}

//配置模式下的发送信息包打包
void CfigModeSendMsgPackagePare()
{
	msg_package_manager.cfig_msg_send_manager->cfg_run_mode = CFIG_MODE;
	msg_package_manager.cfig_msg_send_manager->cfg_mode_val = cfig_manager->mode.cfg_mode_val;

	msg_package_manager.cfig_msg_send_manager->cmd = NORM_MSG_MODE;
	msg_package_manager.cfig_msg_send_manager->volt_switch = 0;
	msg_package_manager.cfig_msg_send_manager->PwmArrayNum = cfig_manager->PwmArrayNum;
	msg_package_manager.cfig_msg_send_manager->pwmval = cfig_manager->pwm_val;
	if(cfig_manager->mode.cfg_mode_val == CFIG_PID_MODE)
	{
		msg_package_manager.cfig_msg_send_manager->pid_obj = cfig_manager->pid_obj;
		msg_package_manager.cfig_msg_send_manager->ref_obj = cfig_manager->obj;
	}
}

//运行模式下的发送信息包打包
void RunModeSendMsgPackagePare()
{
	msg_package_manager.run_msg_send_manager->cfg_run_mode = RUN_MODE;
	msg_package_manager.run_msg_send_manager->sig_doub_mode = run_manager->mode.sig_doub_mode;
	msg_package_manager.run_msg_send_manager->cmd = NORM_MSG_MODE;
	msg_package_manager.run_msg_send_manager->volt_switch = 0;
	msg_package_manager.run_msg_send_manager->cut_mode = run_manager->mode.cut_mode;
	msg_package_manager.run_msg_send_manager->coag_mode = run_manager->mode.coag_mode;
	msg_package_manager.run_msg_send_manager->doub_mode = run_manager->mode.doub_mode;
	msg_package_manager.run_msg_send_manager->cut_val = run_manager->modes_power[run_manager->mode.cut_mode];
	msg_package_manager.run_msg_send_manager->coag_val = run_manager->modes_power[run_manager->mode.coag_mode];
	msg_package_manager.run_msg_send_manager->doub_val = run_manager->modes_power[run_manager->mode.doub_mode];
}

void PackMsg2SendDeal() // 打包信息并发送
{
	if(cfig_manager->mode.cfg_run_mode == CFIG_MODE)
	{
		if(cfig_manager->mode.cfg_mode_val >= CFIG_R_MODE)
			return;
		
		CfigModeSendMsgPackagePare();
		CanMsgPack2Send((u8*)msg_package_manager.cfig_msg_send_manager,sizeof(cfig_mode_msg_type));
	}
	else
	{
		RunModeSendMsgPackagePare();
		CanMsgPack2Send((u8*)msg_package_manager.run_msg_send_manager,sizeof(run_mode_msg_type));
	}

	if(msg_package_manager.cmd != NORM_MSG_MODE)
	{
		msg_package_manager.pre_cmd = msg_package_manager.cmd;
		msg_package_manager.cmd = NORM_MSG_MODE;	
	}
}

// 配置模式下的消息响应
void CfigModeMsgResponse()
{
	static u8 seg_flush_times = 0;

	// 刷新运行状态灯和音频
	if(msg_package_manager.cfig_msg_send_manager->start_en == 1)
	{
		cfig_manager->mode.work_mode = msg_package_manager.cfig_msg_recv_manager->work_mode;
		disp_manager.led_manager.led_en.work_status_en = 1;

		// 声音配置
		
	}
	
	//
	if(msg_package_manager.cfig_msg_send_manager->recored_en != msg_package_manager.cfig_msg_recv_manager->recored_en)
	{
		msg_package_manager.cfig_msg_send_manager->recored_en = msg_package_manager.cfig_msg_recv_manager->recored_en;
		disp_manager.seg_manager.cfig_seg_en.value_en = 1;
	}

	// 管理值的保存和led灯的刷新
	if(msg_package_manager.cfig_msg_send_manager->recored_en == 1)
	{
		//3个数值数码管（6 7 8）闪烁5次
		if(seg_flush_times++ >5)
		{
			seg_flush_times = 0;
			disp_manager.seg_manager.cfig_seg_en.value_en = 1;
		}
		else
			disp_manager.seg_manager.cfig_seg_en.value_flush_en = 0;
		
	}

	if(msg_package_manager.cfig_msg_recv_manager->recored_en == 1)
		msg_package_manager.cfig_msg_send_manager->recored_en = 0;
	

}

// 运行模式下的消息响应

void RunModeMsgResponse()
{
	if(run_manager->mode.cfg_run_mode != msg_package_manager.run_msg_recv_manager->cfg_run_mode)
	{
		if(msg_package_manager.run_msg_recv_manager->cfg_run_mode != IDLE_MODE)
			run_manager->mode.record_w_en = 1;
	
	run_manager->mode.cfg_run_mode = msg_package_manager.run_msg_recv_manager->cfg_run_mode;
	disp_manager.led_manager.led_en.work_status_en = 1;

	//声音配置
	
	}
	if(msg_package_manager.run_msg_recv_manager->dwn_err_flag.bits.can);
}

void MsgResponseFun() //消息响应处理函数
{
	if(cfig_manager->mode.cfg_run_mode == CFIG_MODE)
		CfigModeMsgResponse();
	else
		RunModeMsgResponse();

	if(msg_package_manager.pre_cmd != NORM_MSG_MODE)
	{
		msg_package_manager.pre_cmd = NORM_MSG_MODE;
	}
}

void MsgErrReponse()
{
	if(msg_package_manager.pre_cmd != NORM_MSG_MODE)
	{
		if(msg_package_manager.cmd != msg_package_manager.pre_cmd)
			msg_package_manager.cmd = msg_package_manager.pre_cmd;
		msg_package_manager.pre_cmd = NORM_MSG_MODE;
	}
	cfig_manager->mode.work_mode = IDLE_MODE;
	disp_manager.led_manager.led_en.work_status_en = 1;

	// 声音处理
	
}


// 信息接收处理
void PackMsgRecvDeal()
{
	CanReadMsgDeal();
}


