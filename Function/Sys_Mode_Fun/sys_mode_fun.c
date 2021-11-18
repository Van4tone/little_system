#include "sys_mode_fun.h"


u8 work_manager_buf[600];

cfig_struct_type *cfig_manager;
run_struct_type *run_manager;

void System_ParamInit()
{
	memset(&work_manager_buf,0,sizeof(600));
	run_manager = (run_struct_type*)work_manager_buf;
	cfig_manager = (cfig_struct_type*)work_manager_buf;

	MsgPackageInit();
	
	run_manager->mode.work_mode = IDLE_MODE;
	
}


void DemericMode_ParmInit()
{
	cfig_manager->mode.cfg_mode_val = PUR_CUT_MODE;
	cfig_manager->mode.work_mode = IDLE_MODE;
	cfig_manager->PwmArrayNum = 0;
	
	memset(&disp_manager.seg_manager.cfig_seg_en,0xff,sizeof(cfig_mode_seg_en_type));
	memset(&disp_manager.led_manager.led_en,0xff,sizeof(led_en_type));
}

void RunMode_ParamInit()
{
	run_manager->mode.cut_mode = PUR_CUT_MODE; // 切的模式
	run_manager->mode.coag_mode = POINT_COAG_MODE; //单级凝的模式
	run_manager->mode.doub_mode = STAND_COAG_MODE; // 双极凝的模式
	
	run_manager->mode.sig_doub_mode = SIG_MODE; //单双极模式
	run_manager->mode.panel_polar = P_SIG_MODE;  // 极板的单双性

	//run_manager->modes_power[0] = 123;
	//run_manager->modes_power[1] = 224;
	//run_manager->modes_power[2] = 325;
	//run_manager->modes_power[3] = 426;

	memset(&disp_manager.seg_manager.run_seg_en,0xff,sizeof(run_mode_seg_en_type));
}

void DemericModeTaskDeal()
{
	DemericMode_ParmInit();
	while(1)
    {
    	RecordFunTask();
      
		KeyFunTask();
		
		DispFunTask();
		
		AdFunTask();

		MsgFunTask();
    }
}



void RunModeTaskDeal()
{
	RunMode_ParamInit();
	while(1)
	{
		KeyFunTask();
		DispFunTask();
	}
}
