#ifndef _SYS_H_
#define _SYS_H_
#include "common.h"
enum {
	RUN_MODE = 0,
	CFIG_MODE = 1,
}RUN_CFIG_MODE;

enum {
	PUR_CUT_MODE = 0,
	BLEND_MODE_1,
	BLEND_MODE_2,
	BLEND_MODE_3,

	POINT_COAG_MODE,
	SOFT_COAG_MODE,
	SPRAY_COAG_MODE,

	STAND_COAG_MODE,
	PRECISE_COAG_MODE,

	CFIG_PID_MODE, // pid模式 p/u/c/g
	/****************************/
	CFIG_R_MODE, // 极板阻抗标定
	CFIG_ALARM_MODE, // 报警模式的开关
	IDLE_MODE,
}ALL_RUN_MODE; //9大类工作模式

enum {
	SIG_MODE = 0,
	DOUB_MODE,
}SIG_DOUB_POLAR_MODE;//单双极模式

enum {
	P_SIG_MODE = 0,
	P_DOUB_MODE,
}PANEL_POLAR_MODE;


enum M{
	CFIG_MODE_MAX = 11,
	CFIG_MODE_MIN = 0,
	C_MODE_PWM_ARRAY_NUM_MAX = 20,
	CFIG_POINT_I_VAL = 10,
	CFIG_POINT_MIN = 0,
	CFIG_PWM_MIN = 0,
	CFIG_PWM_MAX = 512,
}CFIG_MODE_LIMIT_VAL;

#define PID_OBJ_P 0
#define PID_OBJ_I 1
#define PID_OBJ_U 2

enum {
	OBJ_PWM = 0,
	OBJ_V = 1,
	OBJ_C = 2,
	OBJ_P = 3,
	OBJ_R = 4,
	OBJ_A = 5,
	OBJ_I = 6,
}CFIG_MODE_OBJ_VAL;



typedef struct Mode_Byte_Struct {
	unsigned cfg_run_mode : 1;	//配置模式和运行模式
	unsigned sig_doub_mode : 1; 		//单双极
	unsigned panel_polar : 1; 	//极板极性
	unsigned alarm_flag : 1;    //报警标志位
	unsigned cut_mode : 4;      //用作切
	unsigned coag_mode : 4;     //凝的治疗模式
	unsigned doub_mode : 4;     //双极模式
	unsigned work_mode : 4;     //工作模式
    unsigned cfg_mode_val : 4; // 用于配置模式存储模式的值
	
	unsigned record_r_en : 1;   //记录的读使能
	unsigned record_w_en : 1;   //记录的写使能
	unsigned record_r_res_en : 1; //
	unsigned reserve : 1;       //
}mode_struct_type;

//运行模式处理信息结构体

typedef struct Run_Struct {
	mode_struct_type mode;
	
	u16 modes_power[9]; // 9种模式，纯切、切1、切2、切3
					// 点凝、面凝、喷凝
					// 单级、双极的值
	
	
}run_struct_type;


//配置模式处理信息结构体

typedef struct CFIG_MODE_RECORD_BUF {
	u16 pwm[21];
	u16 p_i_u[21];
	u16 u[10];
	u16 i[10];
}cfig_m_cur_mode_type;


typedef struct ALARM_TYPE {
	unsigned e2_e : 1;
	unsigned can_e : 1;
	unsigned p_e :1;
	unsigned u_e :1;
	unsigned i_e :1;
	unsigned r_e :1;
	unsigned r_d_e :1;
	unsigned reserve :1;
}cur_alarm_type;

typedef struct CFIG_MODE_ALARM_BUF {
	u8 reserve;
	cur_alarm_type type;

}alarm_buf;

typedef struct Cfig_Struct {
	mode_struct_type mode;

	u8 PwmArrayNum; // pwm[21]一共21个
	

	u8 pid_obj; // 用于存储pid组数据的选项（比如3组pid数据 0 1 2）
	u8 obj; // pid数据的组号（p/i/d 三组）
	
	
	u16 pwm_val;
	u16 pid_set[9];
	u16 p_ad_val;
	u16 u_ad_val;
	u16 i_ad_val;
	u8 r_ad_val;
	
	
	cfig_m_cur_mode_type cfig_mode_buf;
	u16 panel_res_val[22];
	alarm_buf cur_alarm_val;
}cfig_struct_type;

cfig_struct_type *cfig_manager;

run_struct_type *run_manager;


void GetNumQuantile(u16 val,u8 *dec_buf);


void ClearParame(void* data);

void Int_Init();

void HardWareInit();

void Sys_Init();

void Working();

#endif