#include "key_fun.h"


void ScanKeyStatus()
{
	u16 cur_key_val = KEY_ReadData();
	
	if(cur_key_val != key.pre_key_val)
	{
		key.pre_key_val = cur_key_val;
	}
	else
	{
		if(cur_key_val == 0x1f3f)
		{
			key.cur_key = KEY_NONE;
			key.cur_1s_key = key.cur_key;
			return;
		}
		
		if(key.cur_key != cur_key_val)
		{
			key.times = 0;
			key.cur_key = cur_key_val;
		}
		else
		{
			if(key.times > KEY_LJ_TIME)
				key.cur_1s_key = key.cur_key;
			else
				key.times++;
		}
	}
}


u8 KeyGetNum(u8 key_num)
{
	if(key.flag == 0 || key_num == K11 && key.flag == K_H1)
	{
		key.flag = key_num;
		key.cur_1s_key = 0;
		return key.flag;
	}
	else
		return K_R;
}


u8 KeyGet2Num(u8 key_num)
{
	if(key.flag == key_num)
	{
		if(key.cur_1s_key)
		{
			key.times -= KEY_LJ_JG_TIME;
			key.cur_1s_key = KEY_NONE;
			return (key_num+100);
		}
		return K_R;
	}
	return KeyGetNum(key_num);
}


u8 KeyValDeal()
{
	ScanKeyStatus();
	
	switch(key.cur_key)
	{
		case KEY_NUM1:
			return KeyGet2Num(K1);
		case KEY_NUM2:
			return KeyGet2Num(K2);
		case KEY_NUM3:
			return KeyGet2Num(K3);
		case KEY_NUM4:
			return KeyGet2Num(K4);
		case KEY_NUM5:
			return KeyGet2Num(K5);
		case KEY_NUM6:
			return KeyGet2Num(K6);
		case KEY_NUM7:
			return KeyGetNum(K7);
		case KEY_NUM8:
			return KeyGetNum(K8);
		case KEY_NUM9:
			return KeyGetNum(K9);
		case KEY_NUM10:
			return KeyGetNum(K10);
		case KEY_NUM11:
			return KeyGetNum(K11);
		default:
			key.flag = 0;
			key.cur_1s_key = KEY_NONE;
			return K0;
	}
}


void SigleStepAddVal(u16 *data,u8 direction,u16 maxval,u16 minval)
{
	switch(direction)
	{
		case DIR_UP:
			if(*data >= maxval)
				*data = maxval;
			else
				*data += 1;
			break;
		case DIR_DOWN:
			if(*data > maxval)
				*data = maxval;
			
			if(*data == minval)
				*data = minval;
			else
				*data -= 1;
			break;
			
		default:
				break;
			/*
		case DIR_DOWN:
			if(*data < minval)
				*data = minval;
			else if(*data > maxval)
				*data = maxval;
			else
				*data -= 1;
			break;*/
	}
}

/**********************************配置模式按键处理子函数********************************************/





void KeySwitchVal(u8 key_num,u8 up1,u8 up2,u8 down1,u8 down2,u16 max,u16 min,u16 *val)
{
	if(key_num ==up1 || key_num == up2)
		SigleStepAddVal(val,DIR_UP,max,min);
	else if(key_num == down1 || key_num == down2)
		SigleStepAddVal(val,DIR_DOWN,max,min);
	else
		return;
}












/**************#######################################################********************************/


/***********************运行模式按键处理子函数*************************************************************/

//运行模式下配置切模式（大类）下各个小模式（纯切、切1、切2、切3）的值
void RunMode_CutModeCtrl(u8 key_num)
{

}

//运行模式下 凝模式（大类）下各个小模式的值（点凝、软凝、喷凝）
void RunMode_CoagCtrl(u8 key_num)
{

}

//双极模式下 标准凝和精确凝的值
void RunMode_DoubCtrl(u8 key_num)
{

}

// 运行模式的切模式切换（纯切、切1、切2、切3）
void RunMode_CutModSwitch(u8 key_num)
{
	u16 cut_temp = run_manager->mode.cut_mode;

	if(cut_temp == BLEND_MODE_3)
		cut_temp = PUR_CUT_MODE; 
	else
		KeySwitchVal(key_num,K7,K0,K0,K0,BLEND_MODE_3,PUR_CUT_MODE,&cut_temp);

	disp_manager.seg_manager.run_seg_en.cut_en = 1;

	run_manager->mode.cut_mode = cut_temp; 
	
}

// 运行模式的凝模式切换（点凝、软凝、喷凝）
void RunMode_CoagModSwitch(u8 key_num)
{

}

//单双极模式切换
void RunMode_WBiModSwitch(u8 key_num)
{

}

//双极凝模式的切换（标准凝和精确凝）
void RunMode_DoubModSwitch(u8 key_num)
{

}

//极板的单双极切换
void RunMode_PBiModSwitch(u8 key_num)
{

}

/**********************************运行模式按键处理函数***************************************************/
void RunModeKeyFunDeal()
{
	

	u8 temp_key_val = KeyValDeal();
	
	//if(temp < 100)

	if(run_manager->mode.work_mode != IDLE_MODE)
			return;
	
	switch(temp_key_val)
	{
		case K1:
		case K2:
		case K1 + 100:
		case K2 + 100:
			RunMode_CutModeCtrl(temp_key_val);
			break;
            
        case K3:
		case K4:
        case K3 + 100:
		case K4 + 100:
			RunMode_CoagCtrl(temp_key_val);
			break;
            
        case K5:
        case K5 + 100:
		case K6:
        case K6 + 100:
			RunMode_DoubCtrl(temp_key_val);
			break;
            
        case K7:
            RunMode_CutModSwitch(temp_key_val);
            break;
            
		case K8:
			RunMode_CoagModSwitch(temp_key_val);
			break;
            
        case K9:
            RunMode_WBiModSwitch(temp_key_val);
            break;
            
        case K10:
            RunMode_DoubModSwitch(temp_key_val);
            break;
            
        case K11:
            RunMode_PBiModSwitch(temp_key_val);
            break;
            
		default:
			break;
			
	
	}
	
}

/**************************************配置模式按键处理函数*****************************************/

void CfigModeSwitchPwmArrayNum(u8 key_num) // 切换PWM[]参数（0-20）
{
	u16 temp = cfig_manager->PwmArrayNum;
	u16 max = C_MODE_PWM_ARRAY_NUM_MAX;

	if(key_num == K1)
		SigleStepAddVal(&temp,DIR_UP,max,0);
	else if(key_num == K2)
		SigleStepAddVal(&temp,DIR_DOWN,max,0);
	else
		return;

	if(cfig_manager->mode.cfg_mode_val == 9)
	{
		if(temp>8)
			temp = 8;
	}
	
	//刷新数码管3的数据
	disp_manager.seg_manager.cfig_seg_en.pwm_array_num_en = 1;
	// 获取数码管3的数据
	cfig_manager->PwmArrayNum = temp;

	// 初始化显示 pwm值，刷新数码管 6 7 8
	disp_manager.seg_manager.cfig_seg_en.value_en = 1;

	//修改 pwm值，此时值为历史记录值
	if(cfig_manager->mode.cfg_mode_val < CFIG_R_MODE)
	{
		cfig_manager->pwm_val = cfig_manager->cfig_mode_buf.pwm[cfig_manager->PwmArrayNum]; // cfig_mode_buf的值是从eeprom中读取出的

		if(cfig_manager->PwmArrayNum <10)
		{
			cfig_manager->p_ad_val = cfig_manager->cfig_mode_buf.p_i_u[cfig_manager->PwmArrayNum];
			cfig_manager->i_ad_val = cfig_manager->cfig_mode_buf.i[cfig_manager->PwmArrayNum];
			cfig_manager->u_ad_val = cfig_manager->cfig_mode_buf.u[cfig_manager->PwmArrayNum];
		}
		else if(cfig_manager->PwmArrayNum == 10)
			cfig_manager->i_ad_val = cfig_manager->cfig_mode_buf.p_i_u[cfig_manager->PwmArrayNum];
		else
			cfig_manager->u_ad_val = cfig_manager->cfig_mode_buf.p_i_u[cfig_manager->PwmArrayNum];
	}
	else if(cfig_manager->mode.cfg_mode_val == CFIG_R_MODE)
		cfig_manager->pwm_val = cfig_manager->panel_res_val[cfig_manager->PwmArrayNum];
	else if(cfig_manager->mode.cfg_mode_val == CFIG_ALARM_MODE)
		cfig_manager->pwm_val = (cfig_manager->cur_alarm_val.reserve >> cfig_manager->PwmArrayNum) & 0x01;
		
}


void CfigModeObjCtr(u8 key_num) // 切换4号数码管 P/U/C/G的显示
{
	u16 temp = cfig_manager->obj;

	disp_manager.seg_manager.cfig_seg_en.obj_en = 1;
	disp_manager.seg_manager.cfig_seg_en.value_en = 1;

	if(cfig_manager->mode.cfg_mode_val == CFIG_PID_MODE)
	{
		if(cfig_manager->obj == PID_OBJ_P)
			cfig_manager->obj = PID_OBJ_I;
		else
			cfig_manager->obj = PID_OBJ_U;

		cfig_manager->pwm_val = cfig_manager->pid_set[3*cfig_manager->pid_obj + cfig_manager->obj];
		return;
	}

	if(cfig_manager->mode.cfg_mode_val == CFIG_R_MODE)
	{
		if(temp == OBJ_PWM)
			temp = OBJ_R;
		else
			temp = OBJ_PWM;
	}
	else if(cfig_manager->mode.cfg_mode_val == CFIG_ALARM_MODE)
	{
		temp = OBJ_A;
	}
	else
		KeySwitchVal(key_num, K3, K3, K4, K4, OBJ_P, OBJ_PWM, &temp);

	cfig_manager->obj = temp;
}

void CfigModePwmValCtr(u8 key_num) // 按键K5 K6 修改数码管 6 7 8 的pwm数值
{
	u16 temp = cfig_manager->pwm_val;

	if((cfig_manager->obj != OBJ_PWM && cfig_manager->obj != OBJ_A && cfig_manager->mode.cfg_mode_val != CFIG_PID_MODE) || cfig_manager->mode.cfg_mode_val != CFIG_R_MODE)
	{
		if(cfig_manager->mode.cfg_mode_val != CFIG_ALARM_MODE)
			KeySwitchVal(key_num, K5, K5+100, K6, K6+100, CFIG_PWM_MAX, CFIG_PWM_MIN, &temp);
		else
			temp ^= 1;
	}
	disp_manager.seg_manager.cfig_seg_en.value_en = 1;
	cfig_manager->pwm_val = temp;
}

void CfigModeWorkModeCtr(u8 key_num) // 9种治疗模式的切换 数码管1 显示（0-8）,对应显示板下面指示灯的开关
{
	u16 mode_temp = cfig_manager->mode.cfg_mode_val;

	if(key_num == K7)
		SigleStepAddVal(&mode_temp,DIR_UP,CFIG_MODE_MAX,CFIG_MODE_MIN);
	else if(key_num == K8)
		SigleStepAddVal(&mode_temp, DIR_DOWN, CFIG_MODE_MAX, CFIG_MODE_MIN);
	else
		return;

	//按键音

	// 显示刷新**** 切模式 4个 LED 凝模式 3个LED，单双极 2个LED ，对应数码管1的（0-8）
	disp_manager.led_manager.led_en.coag_en = 1;
	disp_manager.led_manager.led_en.cut_en = 1;
	disp_manager.led_manager.led_en.doub_en = 1;

	if(mode_temp == SPRAY_COAG_MODE) // 
		cfig_manager->mode.sig_doub_mode = SIG_MODE; // 喷凝到标准凝之间的单双极模式切换
	else if(mode_temp == STAND_COAG_MODE)
		cfig_manager->mode.sig_doub_mode = DOUB_MODE;


	if(mode_temp != cfig_manager->mode.cfg_mode_val)
	{
		cfig_manager->PwmArrayNum = 0;
		disp_manager.seg_manager.cfig_seg_en.pwm_array_num_en = 1;
	}

	// 获取数值
    cfig_manager->mode.cfg_mode_val = mode_temp; 
	
	//单双极led灯刷新
	disp_manager.led_manager.led_en.sig_doub_en = 1;

	//数码管刷新
	disp_manager.seg_manager.cfig_seg_en.mode_en = 1; // 数码管 1（模式） 刷新 
	disp_manager.seg_manager.cfig_seg_en.obj_en = 1; // 数码管 4 （p/u/c/g ）刷新

   

   // 历史标定值读取使能
   cfig_manager->mode.record_r_en = 1;

   if(cfig_manager->mode.cfg_mode_val == CFIG_PID_MODE) // pid模式时 首先通过 pid_obj 选择pid数据组号（可以3组），每组3个数据 P I D(目前使用了P I)
   {
   		cfig_manager->pid_obj = PID_OBJ_P;//第一组pid数据
		cfig_manager->obj = PID_OBJ_P;
   }
	
}

void CfigModeSaveVal2E2rom(u8 key_num) // 按键 K9 数据保存到eeprom中
{
	u16 temp_pwm = cfig_manager->pwm_val;

	if(cfig_manager->PwmArrayNum && cfig_manager->mode.cfg_mode_val < CFIG_PID_MODE) // 保证pwm[]中的数值从第二个值开始越来越大
	{
		if(cfig_manager->pwm_val<cfig_manager->cfig_mode_buf.pwm[cfig_manager->PwmArrayNum - 1])
		{
			if(cfig_manager->PwmArrayNum !=CFIG_POINT_I_VAL && (cfig_manager->PwmArrayNum !=(CFIG_POINT_I_VAL+1)))
				return;
		}
	}

	if(cfig_manager->obj != OBJ_PWM && cfig_manager->obj!=OBJ_V && cfig_manager->obj != OBJ_R && cfig_manager->obj!=OBJ_A && cfig_manager->mode.cfg_mode_val!=CFIG_PID_MODE)
		return;

	if(cfig_manager->mode.cfg_mode_val<CFIG_PID_MODE)
	{
	
	}
}

void CfigModeKeyFunDeal() // 配置模式下的按键功能函数
{
	u8 key_temp = KeyValDeal();
	
	//if(temp < 100) // 连击声音配置

//	if(cfig_manager->mode.work_mode != IDLE_MODE)
//		return;
	 
	switch(key_temp)
	{
		case K1:
		case K2:
			CfigModeSwitchPwmArrayNum(key_temp);
			break;
            
        case K3:
		case K4:
			CfigModeObjCtr(key_temp);
			break;
            
        case K5:
        case K5 + 100:
		case K6:
        case K6 + 100:
			CfigModePwmValCtr(key_temp);
			break;
            
        case K7:
		case K8:
			CfigModeWorkModeCtr(key_temp);
			break;
		
		case K9:
			CfigModeSaveVal2E2rom(key_temp);
			break;
		
		case K10:
			
			break;
		
		default:
			return;
			
	}
}



void KeyFunDeal()
{
	if(cfig_manager->mode.cfg_run_mode == RUN_MODE)
    {
		RunModeKeyFunDeal();
    }
	else
	{
		CfigModeKeyFunDeal();
    }
}







