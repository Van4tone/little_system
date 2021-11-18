#ifndef _DISPLAY_FUN_H
#define _DISPLAY_FUN_H
#include "max7219.h"
#include "key_fun.h"

#define SEG_NUM1 	2
#define SEG_NUM2    6
#define SEG_NUM3 	8
#define SEG_NUM4 	4
#define SEG_NUM5    3

#define SEG_NUM6 	7
#define SEG_NUM7 	5
#define SEG_NUM8 	1

#define LED_NUM0 	6
#define LED_NUM1 	5
#define LED_NUM2 	4
#define LED_NUM3 	3
#define LED_NUM4 	2
#define LED_NUM5 	1
#define LED_NUM6 	0
#define LED_NUM7 	7


enum {
	PUR_CUT_ADDR = 0x06,
	BLEND1_ADDR  = 0x05,
	BLEND2_ADDR  = 0x04,
	BLEND3_ADDR  = 0x03,
	
	POINT_COAG_ADDR = 0x02,
	SOFT_COAG_ADDR = 0x01,
	SPRAY_COAG_ADDR = 0x00,

	SIG_POLAR_ADDR = 0x46,
	DOUB_POLAR_ADDR = 0x45,
	
	STAND_COAG_ADDR = 0x44,
	PRECISE_COAG_ADDR = 0x43,
	
	SIG_PANEL_ADDR = 0x42,
	DOUB_PANEL_ADDR = 0x41,
	
	CUT_RUN_STATUS_ADDR = 0x01,
	COAG_RUN_STATUS_ADDR = 0x05,
	DOUB_COAG_RUN_STATUS_ADDR = 0x07,

}LED_ADDR;




 u8 digit_seg[] =  
{	
	/*0     1     2     3     4    5     6     7     8     9*/
	0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B,	
	/*A     b     C     d     E    F     G     H     J     L*/	
	0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E, 0x37, 0x38, 0X0E,
	/*o		P	  V     v     U    u     I     n     r     S*/ 												
	0x1D, 0x67, 0x1C, 0x1C, 0x3E, 0x1C, 0x30, 0x15, 0x05, 0x5B, 
	/*y     t     -     -     _    " "   .     K     O*/
	0x3B, 0x0F, 0x01, 0x40, 0x08, 0x0, 0x80,  0x07,	0x7E,				
};


/************************************************/

// led info type

typedef struct Led_Disp_Info {
		u8 buf[8];
		u8 led_segment_en;
}led_disp_buffer_type;

typedef struct Led_En {
	unsigned cut_en : 1; //切 led使能
	unsigned coag_en :1; // 凝 LED使能
	unsigned sig_doub_en :1; //单双极 led 使能
	unsigned doub_en :1; //双极 led 使能
	unsigned panel_sig_doub_en :1; // 极板单双极 led使能
	unsigned work_status_en :1; // 运行状态 led刷新使能
	unsigned panel_area_en :1; // 极板面积led刷新
	unsigned alarm_en :1; // 报警灯 led刷新
}led_en_type;

typedef struct Led_Info {
	led_disp_buffer_type led_buf;
	led_en_type led_en;
}led_info_type;


// seg info type

typedef struct Seg_Disp_Info {
		u8 buf[8]; // 存储8个数码管的值
		u8 seg_segment_en;// 每个数码管的显示使能位0x0000 0000,8bit
}seg_disp_buffer_type;

typedef struct RunMode_Seg_En {
		unsigned cut_en :1;
		unsigned coag_en :1;
		unsigned doub_en :1;
		unsigned err_en :1;
}run_mode_seg_en_type;

typedef struct CfigMode_Seg_En {
		unsigned mode_en : 1; // 对应数码管 1 的显示使能
		unsigned obj_en : 1;  // 对应数码管 4 的显示使能
		unsigned pwm_array_num_en : 1; // 对应数码管 3 的显示使能
		unsigned value_en : 1; // 设定值和反馈显示（数码管6 7 8 显示 pwm值和 p_ad u_ad i_ad的值）
		unsigned bar_en : 1; // 横杠显示使能
}cfig_mode_seg_en_type;

typedef struct Seg_Info {
	seg_disp_buffer_type seg_buf;
	run_mode_seg_en_type run_seg_en;
	cfig_mode_seg_en_type cfig_seg_en;
}seg_info_type;

/**********************************************/

typedef struct Disp_Info_Struct {
	led_info_type led_manager;
	seg_info_type seg_manager;

}disp_info_type;

disp_info_type disp_manager;

void PackLedNumBuf(u8 seg_num,u8 led_num,u8 led_status);

void PackSegNumBuf(u8 seg_num,u8 seg_data);

void PackCutModeVal(u16 val);

void RunModeSegFunDeal();


void CutModeLedDisplay(u8 cut_mode);

void CoagModeLedDisplay(u8 coage_mode);

void CfigModeSegDisModeVal(u8 mode_val);

void CfigModeSegDisPwmArrayNum(u8 pwm_num);

void CfigModeSegFunDeal();

void SegValFlush();


void LedDisplayFunDeal();

void RunModeSegFunDeal();

void CfigModeSegFunDeal();

void DispFunDeal();

#endif