#ifndef _KEY_FUN_H
#define _KEY_FUN_H
#include "common.h"


#define KEY_NONE 0

#define KEY_NUM0 0X1F3F
#define KEY_NUM1 0x1f3e
#define KEY_NUM2 0x1f3d
#define KEY_NUM3 0x1f3b
#define KEY_NUM4 0X1f37
#define KEY_NUM5 0X1f2f
#define KEY_NUM6 0x1f1f
#define KEY_NUM7 0x1e3f
#define KEY_NUM8 0x1d3f
#define KEY_NUM9 0x1b3f
#define KEY_NUM10 0x173f
#define KEY_NUM11 0x0f3f

#define KEY_HE_NUM2 0x0d3f
#define KEY_HE_NUM1 0x0b3f
#define KEY_HE_NUM0 0X073F

#define KEY_LJ_TIME 80
#define KEY_LJ_JG_TIME 2

#define DIR_UP		1
#define DIR_DOWN	0

enum K{
	K0 = 0,
	K1 ,
	K2,
	K3,
	K4,
	K5,
	K6,
	K7,
	K8,
	K9,
	K10,
	K11,
	K_R,
	K_H0,
	K_H1,
	K_H2,
}KEY_NUM;



typedef struct Key_Val {
	u16 cur_1s_key; //连击时的值
	u16 cur_key;  // 当前键值
	u16 pre_key_val; // 为防抖设计的预前键值
	u16 times; // 连击生效时间
	u8 flag; // 按键标志位，判断是否有按键按下，如果按下判断是否为连击
}key_val;

key_val key;



void ScanKeyStatus();

u8 KeyGetNum(u8 key_num);

u8 KeyGet2Num(u8 key_num);

u8 KeyValDeal();


void SigleStepAddVal(u16 *data,u8 direction,u16 maxval,u16 minval);

void KeySwitchVal(u8 key_num,u8 up1,u8 up2,u8 down1,u8 down2,u16 max,u16 min,u16 *val);

/*****运行模式子函数*************************/

void RunMode_CutModeCtrl(u8 key_num);

void RunMode_CoagCtrl(u8 key_num);

void RunMode_DoubCtrl(u8 key_num);

void RunMode_CutModSwitch(u8 key_num);

void RunMode_CoagModSwitch(u8 key_num);

void RunMode_WBiModSwitch(u8 key_num);

void RunMode_DoubModSwitch(u8 key_num);

void RunMode_PBiModSwitch(u8 key_num);

/*****配置模式子函数**************************************/
void CfigModeSwitchPwmArrayNum(u8 key_num);

void CfigModeObjCtr(u8 key_num);

void CfigModePwmValCtr(u8 key_num);

void CfigModeWorkModeCtr(u8 key_num);

void CfigModeSaveVal2E2rom(u8 key_num);

/**************************************************************/
void KeyFunDeal();

void RunModeKeyFunDeal();

void CfigModeKeyFunDeal();

#endif