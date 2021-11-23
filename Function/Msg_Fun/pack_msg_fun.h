#ifndef _PACK_MSG_FUN_H
#define _PACK_MSG_FUN_H
#include "common.h"

#define NORM_MSG_MODE 0
#define CMD_MSG_MODE 1

typedef	struct ERROR_BITS{
		unsigned can : 1;
		unsigned e2rom : 1;
		unsigned p : 1;
		unsigned u : 1;
		unsigned i : 1;
		unsigned reserve : 3;
}errbits_type;

typedef union ERROR_UNION { 
	u8 buf;
	errbits_type bits;
}err_union_type;


typedef struct RUN_MODE_MSG{
	unsigned cfg_run_mode : 1;
	unsigned sig_doub_mode : 1;
	
	unsigned start_en : 1;
	unsigned work_mode : 4; //工作模式（有9种）
	unsigned reserves : 1;
	unsigned cmd : 2;
	unsigned volt_switch : 2; // 电压切换位
	unsigned cut_mode : 4; // 切的具体模式
	unsigned coag_mode : 4; // 凝的具体模式
	unsigned doub_mode : 4;//双极凝的模式

	// 当前的值
	u16 cut_val;
	u16 coag_val;
	u16 doub_val;
	err_union_type dwn_err_flag;
}run_mode_msg_type;




typedef struct CFIG_MODE_MSG{
	unsigned cfg_run_mode : 1; // 配置或者运行模式
	unsigned  cfg_mode_val: 4; // 模式值
	unsigned start_en : 1;  // 启动使能位
	unsigned recored_en : 1; // 保存位
	unsigned peri_recored_en : 1; // 允许保存键可操作位
	unsigned work_mode : 4; //工作模式（有9种）
	unsigned cmd : 2;
	unsigned volt_switch : 2; // 电压切换位
	u8 PwmArrayNum;
	u16 pwmval;
	u8 pid_obj;
	u8  ref_obj;
	err_union_type dwn_err_flag;
}cfig_mode_msg_type;







typedef struct Communications_Msg_Package {
	cfig_mode_msg_type *cfig_msg_send_manager;
	cfig_mode_msg_type *cfig_msg_recv_manager;
	run_mode_msg_type *run_msg_send_manager;
	run_mode_msg_type *run_msg_recv_manager;

	unsigned cmd : 4;
	unsigned pre_cmd : 4;

}Com_Msg_Type;




Com_Msg_Type msg_package_manager;





void MsgPackageInit();

void CfigModeSendMsgPackagePare();

void RunModeSendMsgPackagePare();

void PackMsg2SendDeal();

void CfigModeMsgResponse();

void RunModeMsgResponse();

void MsgResponseFun();

void MsgErrReponse();

void PackMsgRecvDeal();

#endif