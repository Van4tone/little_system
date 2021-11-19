#include "pack_msg_fun.h"


void MsgPackageInit() // 要发送的信息包 buffer封装
{
	msg_package_manager.cmd = CMD_MODE;
	msg_package_manager.cfig_msg_send_manager = (cfig_mode_msg_type*)(send_msg_frame.buf + 3);
	msg_package_manager.cfig_msg_recv_manager = (cfig_mode_msg_type*)(recv_msg_frame.buf + 3);

	msg_package_manager.run_msg_send_manager = (run_mode_msg_type*)(send_msg_frame.buf + 3);
	msg_package_manager.run_msg_recv_manager = (run_mode_msg_type*)(recv_msg_frame.buf + 3);
}

//配置模式下的发送信息包打包
void CfigModeMsgPackagePare()
{
	msg_package_manager.cfig_msg_send_manager->cfg_run_mode = CFIG_MODE;
	msg_package_manager.cfig_msg_send_manager->cfg_mode_val = cfig_manager->mode.cfg_mode_val;

	msg_package_manager.cfig_msg_send_manager->cmd = NORM_MSG_MODE;
	msg_package_manager.cfig_msg_send_manager->volt_switch = 0;
	msg_package_manager.cfig_msg_send_manager->pwmval = cfig_manager->pwm_val;
	if(cfig_manager->mode.cfg_mode_val == CFIG_PID_MODE)
	{
		msg_package_manager.cfig_msg_send_manager->pid_obj = cfig_manager->pid_obj;
		msg_package_manager.cfig_msg_send_manager->ref_obj = cfig_manager->obj;
	}
}

//运行模式下的发送信息包打包
void RunModeMsgPackagePare()
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

void PackMsg2Send() // 打包信息并发送
{
	if(cfig_manager->mode.cfg_run_mode == CFIG_MODE)
	{
		if(cfig_manager->mode.cfg_mode_val >= CFIG_R_MODE)
			return;
		CfigModeMsgPackagePare();
		CanMsgPack2Send((u8*)msg_package_manager.cfig_msg_send_manager,sizeof(cfig_mode_msg_type));
	}
	else
	{
		RunModeMsgPackagePare();
		CanMsgPack2Send((u8*)msg_package_manager.run_msg_send_manager,sizeof(run_mode_msg_type));
	}

	if(msg_package_manager.cmd != NORM_MSG_MODE)
	{
		msg_package_manager.pre_cmd = msg_package_manager.cmd;
		msg_package_manager.cmd = NORM_MSG_MODE;
		
	}
}


