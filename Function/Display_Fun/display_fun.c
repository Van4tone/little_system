#include "display_fun.h"
#include "string.h"


void PackSegNumBuf(u8 seg_num,u8 seg_data)
{
	u8 temp = seg_num - 1;
	disp_manager.seg_manager.seg_buf.buf[temp] = seg_data; // 数组编号由0开始，而数码管号码值由1开始，故此处减一处理
	disp_manager.seg_manager.seg_buf.seg_segment_en |= 1<<temp; // 对应8个数码管的使能位（0-7）
}

void PackLedNumBuf(u8 seg_num,u8 led_num,u8 led_status)
{
	u8 temp = disp_manager.led_manager.led_buf.buf[seg_num];

	switch(temp)
	{
		case CUT_RUN_STATUS_ADDR:
		case COAG_RUN_STATUS_ADDR:
		case DOUB_COAG_RUN_STATUS_ADDR:
			if(led_status == 1)
				temp = 0xff;
			else
				temp = 0x00;
			break;
			
		default:
			temp = (temp & (~(1<<led_num))) | (led_status << led_num); // max7219 显示段码管（a,b,c,d,e,f,g,dp）dp 为最高位，a为次高位
			break;
	}

	disp_manager.led_manager.led_buf.buf[seg_num] = temp;
	disp_manager.led_manager.led_buf.led_segment_en |= 1 <<seg_num;
}


void PackCutModeVal(u16 val) //填充切模式下数码管要显示的字符
{
	u8 dec_buf[3] = {0};

	if(val>999)
		val = 999;
	GetNumQuantile(val,dec_buf);

	if(dec_buf[2] == 0)
	{
		dec_buf[2] = CHAR_NULL;
		if(dec_buf[1] == 0)
		dec_buf[1] = CHAR_NULL;
	}

	PackSegNumBuf(SEG_1,digit_seg[dec_buf[2]]);
	PackSegNumBuf(SEG_2,digit_seg[dec_buf[1]]);
	PackSegNumBuf(SEG_3,digit_seg[dec_buf[0]]);
	
}

void SegValFlush() // 刷新显示所用 数码管的值
{
	u8 seg_section_en = disp_manager.seg_manager.seg_buf.seg_segment_en;
	u8 i = 0;

	if(seg_section_en == 0)
		return;
	while(i<8)
	{
		if(seg_section_en & 1<<i)
			Seg_Write_Data(i+1,disp_manager.seg_manager.seg_buf.buf[i]);
		i++;
	}
	disp_manager.seg_manager.seg_buf.seg_segment_en = 0;
}

void LedValFlush() // 刷新显示所用led的值
{
	u8 led_section_en = disp_manager.led_manager.led_buf.led_segment_en;
	u8 i = 0;

	if(led_section_en == 0)
		return;
	while(i<8)
	{
		if(led_section_en & 1<<i)
			Led_Write_Data(i+1, disp_manager.led_manager.led_buf.buf[i]);
		i++;
	}
	disp_manager.led_manager.led_buf.led_segment_en = 0;
}

void RunModeSegFunDeal()
{
	//u16 power_val = 0;
	if(disp_manager.seg_manager.run_seg_en.cut_en) //运行模式下切模式 数码管显示
	{
#if 0
		switch (run_manager->mode.cut_mode)
		{
			case PUR_CUT_MODE:
				power_val = 121;
				break;
			case BLEND_MODE_1:
				power_val = 201;
				break;
			case BLEND_MODE_2:
				power_val = 376;
				break;
			case BLEND_MODE_3:
				power_val = 429;
				break;
			default:
				break;
		}
		PackCutModeVal(power_val);
#endif
		PackCutModeVal(run_manager->modes_power[run_manager->mode.cut_mode]);
		disp_manager.seg_manager.run_seg_en.cut_en = 0;
	}

	SegValFlush();
}
/*************************************************************************************
**************************************************************************************
*************************************************************************************/

void CfigModeSegDisModeVal(u8 mode_val) // 数码管1显示模式值（0-8-0-a-b）
{
	if(mode_val == 9)
		mode_val = 28;
	//else if (mode_val == 10)
	//	mode_val = 10;
	
	PackSegNumBuf(SEG_1,digit_seg[mode_val]);
}


void CfigModeSegDisMidline() // 数码管2和5显示中杠
{
	PackSegNumBuf(SEG_2, digit_seg[CHAR_MidLine]);
	PackSegNumBuf(SEG_5, digit_seg[CHAR_MidLine]);
}


void CfigModeSegDisPwmArrayNum(u8 pwm_num) // 数码管3显示pid数据组的组号（p/i/u）
{
	if(cfig_manager->mode.cfg_mode_val == CFIG_PID_MODE)
	{
		switch(pwm_num)
		{
			case PID_OBJ_P:
				pwm_num = CHAR_P;
				break;
			case PID_OBJ_I:
				pwm_num = CHAR_I;
				break;
			case PID_OBJ_U:
				pwm_num = CHAR_U;
				break;
		}
	}
	PackSegNumBuf(SEG_3, digit_seg[pwm_num]);
}

void CfigModeSegDisObjVal(u8 obj_val) // 数码管4显示 p/u/i/g
{
	if(cfig_manager->mode.cfg_mode_val == CFIG_PID_MODE)
		switch(obj_val)
		{
			case PID_OBJ_P:
				obj_val = CHAR_P;
				break;
			case PID_OBJ_I:
				obj_val = CHAR_I;
				break;
		}
	else
	{
		switch (obj_val)
			{
			case OBJ_PWM:
				obj_val = CHAR_P;
				break;
			case OBJ_V:
				obj_val = CHAR_U;
				break;
			case OBJ_C:
				obj_val = CHAR_C;
				break;
			case OBJ_P:
				obj_val = CHAR_G;
				break;		
			}
	}

	if(cfig_manager->mode.cfg_mode_val == CFIG_ALARM_MODE)
		obj_val = CHAR_O;

	PackSegNumBuf(SEG_4, digit_seg[obj_val]);
}

void CfigModeSegDisPidValue()//数码管6 7 8 显示pid的值（PWM,P/I）
{
	u8 pwm_val[3];
	u16 val;

	if(cfig_manager->mode.cfg_mode_val == CFIG_ALARM_MODE)
	{
		if(cfig_manager->pwm_val)
		{
			PackSegNumBuf(SEG_6, digit_seg[CHAR_O]);
			PackSegNumBuf(SEG_7, digit_seg[CHAR_o]);
			PackSegNumBuf(SEG_8, digit_seg[CHAR_n]);
		}
	
		else
		{
			PackSegNumBuf(SEG_6, digit_seg[CHAR_O]);
			PackSegNumBuf(SEG_7, digit_seg[CHAR_F]);
			PackSegNumBuf(SEG_8, digit_seg[CHAR_F]);
		}

		return;
	}

	//上位机是否允许保存
	if(msg_package_manager.cfig_msg_send_manager->recored_en)
	{
		disp_manager.seg_manager.cfig_seg_en.value_flush_en ^= 1;
		
		if(disp_manager.seg_manager.cfig_seg_en.value_flush_en)
		{
			PackSegNumBuf(SEG_6,digit_seg[CHAR_NULL]);
			PackSegNumBuf(SEG_7,digit_seg[CHAR_NULL]);
			PackSegNumBuf(SEG_8,digit_seg[CHAR_NULL]);
			return;
		}
	}

	if(cfig_manager->mode.cfg_mode_val == CFIG_PID_MODE)
		val = cfig_manager->pwm_val;
	else
	{	
		switch (cfig_manager->obj)
			{
			case OBJ_PWM:
				val = cfig_manager->pwm_val;
				break;
			
			case OBJ_V:
				val = cfig_manager->u_ad_val;
				break;
			case OBJ_C:
				val = cfig_manager->i_ad_val;
				break;
			case OBJ_P:
				val = cfig_manager->p_ad_val;
				break;
			default:
					break;
			}
	}

	if(val>999)
		val = 999;
	GetNumQuantile(val,pwm_val);

	if(pwm_val[2] == 0)
	{
		pwm_val[2] = CHAR_NULL;
		if(pwm_val[1] == 0)
			pwm_val[1] = CHAR_NULL;
	}
	PackSegNumBuf(SEG_6,digit_seg[pwm_val[2]]);
	PackSegNumBuf(SEG_7,digit_seg[pwm_val[1]]);
	PackSegNumBuf(SEG_8,digit_seg[pwm_val[0]]);
}

void CfigModeSegFunDeal() // 配置模式下 数码管使能及数据刷新
{
	// seg1 flush,按键S7选择模式（显示0~8-p-a-b）
	if(disp_manager.seg_manager.cfig_seg_en.mode_en) 
	{
		if(cfig_manager->mode.cfg_mode_val == CFIG_PID_MODE)
			CfigModeSegDisModeVal(cfig_manager->PwmArrayNum);
		else
			CfigModeSegDisModeVal(cfig_manager->mode.cfg_mode_val);
		disp_manager.seg_manager.cfig_seg_en.mode_en = 0;
	}

	// seg 2 and seg 5 显示中杠
	if(disp_manager.seg_manager.cfig_seg_en.bar_en)
	{
		CfigModeSegDisMidline();
		disp_manager.seg_manager.cfig_seg_en.bar_en = 0;
	}


	// seg3 flush （cfig_manager->mode.cfg_mode_val==9时显示 p/i/u,其他值时则显示0-9-o）
	if(disp_manager.seg_manager.cfig_seg_en.pwm_array_num_en) 
	{
		if(cfig_manager->mode.cfg_mode_val == CFIG_PID_MODE)
			CfigModeSegDisPwmArrayNum(cfig_manager->pid_obj);
		else
			CfigModeSegDisPwmArrayNum(cfig_manager->PwmArrayNum);
		
		disp_manager.seg_manager.cfig_seg_en.pwm_array_num_en = 0;
	}
	
	// 数码管4显示obj的数据（p i d 的值）和PWM的值
	if(disp_manager.seg_manager.cfig_seg_en.obj_en)
	{
		CfigModeSegDisObjVal(cfig_manager->obj);
		disp_manager.seg_manager.cfig_seg_en.obj_en = 0;
	}

	//数码管 6 7 8 的值显示刷新（0~999））
	if(disp_manager.seg_manager.cfig_seg_en.value_en)
	{
		CfigModeSegDisPidValue();
		disp_manager.seg_manager.cfig_seg_en.value_en = 0;
	}
	
	
	SegValFlush();
}

void CutModeLedDisplay(u8 cut_mode) // 切的4个指示灯
{
	PackLedNumBuf(0,PUR_CUT_ADDR&0x0f,cut_mode==PUR_CUT_MODE);
	PackLedNumBuf(0,BLEND1_ADDR&0x0f,cut_mode==BLEND_MODE_1);
	PackLedNumBuf(0,BLEND2_ADDR&0x0f,cut_mode==BLEND_MODE_2 );
	PackLedNumBuf(0,BLEND3_ADDR&0x0f,cut_mode==BLEND_MODE_3);
	
}

void CoagModeLedDisplay(u8 coage_mode) // 凝的3个指示灯
{
	PackLedNumBuf(0,POINT_COAG_ADDR&0x0f,coage_mode==POINT_COAG_MODE);
	PackLedNumBuf(0,SOFT_COAG_ADDR&0x0f,coage_mode==SOFT_COAG_MODE);
	PackLedNumBuf(0,SPRAY_COAG_ADDR&0x0f,coage_mode==SPRAY_COAG_MODE);
}

void DoubPolarCoagLedDisplay(u8 doub_mode) // 双极凝的2个指示灯
{
	PackLedNumBuf(4,STAND_COAG_ADDR&0x0f,doub_mode == STAND_COAG_MODE);
	PackLedNumBuf(4,PRECISE_COAG_ADDR&0x0f,doub_mode == PRECISE_COAG_MODE);
}

void SigOrDoubLedDisplay(u8 sig_doub) // 单双极模式的2个指示灯显示
{
	PackLedNumBuf(4,SIG_POLAR_ADDR&0x0f,sig_doub==SIG_MODE);
	PackLedNumBuf(4,DOUB_POLAR_ADDR&0x0f,sig_doub==DOUB_MODE);
}

void PanelSigDouLedDisplay(u8 panle_polar) // 单极板或双极板的指示灯显示
{
	PackLedNumBuf(4,SIG_PANEL_ADDR&0x0f,panle_polar==P_SIG_MODE);
	PackLedNumBuf(4,DOUB_PANEL_ADDR&0x0f,panle_polar==P_DOUB_MODE);
}

void PanelAreaLedDisplay() // 极板的面积显示
{

}

void RunModeWorkStatusLedDisplay(u8 run_mode)
{
	PackLedNumBuf(CUT_RUN_STATUS_ADDR,0,run_mode<POINT_COAG_MODE);
	PackLedNumBuf(COAG_RUN_STATUS_ADDR,0,run_mode<STAND_COAG_MODE && run_mode>BLEND_MODE_3);
	PackLedNumBuf(DOUB_COAG_RUN_STATUS_ADDR,0,run_mode<CFIG_R_MODE && run_mode>SPRAY_COAG_MODE);
}

void LedDisplayFunDeal() // led灯显示处理函数
{
	if(disp_manager.led_manager.led_en.cut_en) // 切模式的指示灯显示处理
	{
		if(cfig_manager->mode.cfg_run_mode == CFIG_MODE)
			CutModeLedDisplay(cfig_manager->mode.cfg_mode_val);
		else
			CutModeLedDisplay(cfig_manager->mode.cut_mode);
		disp_manager.led_manager.led_en.cut_en = 0;
	}

	if(disp_manager.led_manager.led_en.coag_en) // 凝模式的指示灯显示处理
	{
		if(cfig_manager->mode.cfg_run_mode == CFIG_MODE)
			CoagModeLedDisplay(cfig_manager->mode.cfg_mode_val);
		else
			CoagModeLedDisplay(cfig_manager->mode.coag_mode);
		disp_manager.led_manager.led_en.coag_en = 0;
	}

	if(disp_manager.led_manager.led_en.doub_en) //双极凝(标准凝/精确凝）指示灯显示
	{
		if(cfig_manager->mode.cfg_run_mode == CFIG_MODE)
			DoubPolarCoagLedDisplay(cfig_manager->mode.cfg_mode_val);
		else
			DoubPolarCoagLedDisplay(cfig_manager->mode.doub_mode);
	}

	if(disp_manager.led_manager.led_en.sig_doub_en) // 单双极模式指示灯
	{	
		SigOrDoubLedDisplay(cfig_manager->mode.sig_doub_mode);
	}

	if(disp_manager.led_manager.led_en.work_status_en) // 工作模式下，运行的模式指示灯
	{
		RunModeWorkStatusLedDisplay(cfig_manager->mode.work_mode);
		disp_manager.led_manager.led_en.work_status_en = 0;
	}

	if(disp_manager.led_manager.led_en.panel_sig_doub_en) // 极板单双极显示指示灯（2个）
	{
		PanelSigDouLedDisplay(cfig_manager->mode.panel_polar);
		disp_manager.led_manager.led_en.panel_sig_doub_en = 0;
	}

	if(disp_manager.led_manager.led_en.panel_area_en) // 极板面积显示指示灯
	{
		PanelAreaLedDisplay();
	}
	
	LedValFlush();
}

void DispFunDeal()
{
	if(cfig_manager->mode.cfg_run_mode == RUN_MODE)
	{
		RunModeSegFunDeal();
	}
	else
	{
		CfigModeSegFunDeal();
	}
	LedDisplayFunDeal();
}







