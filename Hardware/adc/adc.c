#include "adc.h"


void Adc_Init()
{
	//AD_GPIO_INIT;
    P_AD_DIR = GPIO_MODE_IN;
	U_AD_DIR = GPIO_MODE_IN; 
	I_AD_DIR = GPIO_MODE_IN;
	REM_AD_DIR     = GPIO_MODE_IN;	
	MODE_AD_DIR    = GPIO_MODE_IN;
	R_AD_DIR       = GPIO_MODE_IN;

	// 检测信号引脚初始化
	PLATE_MODE_DIR = GPIO_MODE_IN;
	VSENSE_DIR  = GPIO_MODE_IN;
	LIGASURE_CHECK_DIR = GPIO_MODE_IN;
	CUT_COAG_CHECK_DIR = GPIO_MODE_IN;

	ad_i_sum = 0;
	ad_u_sum = 0;
	ad_p_sum = 0;
	ad_r_sum = 0;
	
	ADCON0 = (0<<1)|(1<<0);
	ADCON1 = (0<<5)|(0<<4)|(9<<0);
	ADCON2 = (1<<7)|(0<<3)|(6<<0);
	ADIE = STOP;
}

unsigned short int Ad_SetChannel_GetData(unsigned char channel)
{
	unsigned short int i;
	i = AD_DELAY;
	
	ADCON0 = channel<<2|(0<<1)|(1<<0);
	ADC_START;
	while(i--&&GODONE);
	return (((ADRESH<<8)|(ADRESL))&0x3FF);
	
}

unsigned short int AdGetOneVal(unsigned char channel)
{
	unsigned short int Adtemp[4];
	Adtemp[0] = Ad_SetChannel_GetData(channel);
	Adtemp[1] = Ad_SetChannel_GetData(channel);
	Adtemp[2] = Ad_SetChannel_GetData(channel);
	
	if(Adtemp[0] > Adtemp[1])
	{
		Adtemp[3] = Adtemp[0];
		Adtemp[0] = Adtemp[1];
		Adtemp[1] = Adtemp[3];
	}
	
	if(Adtemp[2] > Adtemp[1])
		return Adtemp[1];
	if(Adtemp[2] > Adtemp[0])
		return Adtemp[0];
	else
		return Adtemp[2];
}

u16 AdGetValue(unsigned char channel)
{
	unsigned short int Adtemp[4];
	Adtemp[0] = AdGetOneVal(channel);
	Adtemp[1] = AdGetOneVal(channel);
	Adtemp[2] = AdGetOneVal(channel);
	
	if(Adtemp[0] > Adtemp[1])
	{
		Adtemp[3] = Adtemp[0];
		Adtemp[0] = Adtemp[1];
		Adtemp[1] = Adtemp[3];
	}
	
	if(Adtemp[2] > Adtemp[1])
		return Adtemp[1];
	if(Adtemp[2] > Adtemp[0])
		return Adtemp[0];
	else
		return Adtemp[2];
}

u16 Filtering_AdData(u16 data,u16 *sum)
{
	u16 f_data = *sum>>4;
	s16 ad_temp;
	ad_temp = data - f_data;
	*sum += ad_temp;

	return f_data;
}

void PackAdData(u8 channel)
{
	u16 ad_temp_val = AdGetValue(channel);
	switch(channel)
	{
		case P_AD_CHANNEL:
				ad_data.ad_struct.p_ad = Filtering_AdData(ad_temp_val, &ad_p_sum);
			break;
			
		case U_AD_CHANNEL:
				ad_data.ad_struct.u_ad = Filtering_AdData(ad_temp_val, &ad_u_sum);
			break;
			
		case I_AD_CHANNEL:
				ad_data.ad_struct.i_ad = Filtering_AdData(ad_temp_val, &ad_i_sum);
			break;
			
		case M_AD_CHANNEL:
				ad_data.ad_struct.m_ad = Filtering_AdData(ad_temp_val, &ad_m_sum);
			break;
			
		case R_AD_CHANNEL:
				ad_data.ad_struct.r_ad = Filtering_AdData(ad_temp_val, &ad_r_sum);
			break;
	}
}

void AdValueDeal()
{
	PackAdData(ad_data.ad_struct.cur_channel);

	switch (ad_data.ad_struct.cur_channel)
	{
		case P_AD_CHANNEL:
			ad_data.ad_struct.cur_channel = U_AD_CHANNEL;
			break;
		case U_AD_CHANNEL:
			ad_data.ad_struct.cur_channel = I_AD_CHANNEL;
			break;
		case I_AD_CHANNEL:
			ad_data.ad_struct.cur_channel = R_AD_CHANNEL;
			break;
		case R_AD_CHANNEL:
			ad_data.ad_struct.cur_channel = P_AD_CHANNEL;
			break;
		case M_AD_CHANNEL:
			ad_data.ad_struct.cur_channel = P_AD_CHANNEL;
			break;
		default:
			ad_data.ad_struct.cur_channel = P_AD_CHANNEL;
			break;
	}
}


void AdFunDeal()
{
	static u8 cycle = 0;
	AdValueDeal();
	if(cfig_manager->mode.cfg_run_mode == CFIG_MODE && cfig_manager->mode.cfg_mode_val<CFIG_ALARM_MODE)
	{
		if(cycle++>5)
		{
			//	未进行信息的交互******/
			cfig_manager->p_ad_val = ad_data.ad_struct.p_ad;
			cfig_manager->u_ad_val = ad_data.ad_struct.u_ad;
			cfig_manager->i_ad_val = ad_data.ad_struct.i_ad;
		
			cfig_manager->r_ad_val = ad_data.ad_struct.r_ad;
			if(cfig_manager->obj != OBJ_PWM)
				disp_manager.seg_manager.cfig_seg_en.value_en = 1;
			cycle = 0;
		}
	}

}



