#ifndef __ADC_H_
#define __ADC_H_

#include "common.h"

#define AD_DELAY 500

#define ADC_START (GODONE = START)

#define P_AD_CHANNEL 0
#define U_AD_CHANNEL 1
#define I_AD_CHANNEL 2
#define R_AD_CHANNEL 3

#define M_AD_CHANNEL 5
#define PLANE_AD_CHANNEL 6


typedef struct Ad_Err {
	unsigned u_ad_err : 1;
	unsigned p_ad_err : 1;
	unsigned r_drop_err : 1;
	unsigned r_lost_err : 1;
	unsigned r_lost_first : 1;
	unsigned reserve : 3;
	
}ad_status_type;

typedef struct AD_Struct{
	u16 i_ad;
	u16 u_ad;
	u16 p_ad;
	u16 r_ad;
	u16 m_ad;
	u8 cur_channel;
	ad_status_type ad_err_words;
}ad_struct_type;


typedef union AD_Info {
	ad_struct_type ad_struct;

}ad_info_union;


ad_info_union ad_data;

u16 ad_i_sum,ad_u_sum,ad_p_sum,ad_r_sum,ad_m_sum;



void Adc_Init();
unsigned short int Ad_SetChannel_GetData(unsigned char channel);
unsigned short int AdGetOneVal(unsigned char channel);

#endif