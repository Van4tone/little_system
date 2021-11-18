#include "record_fun.h"




void GetPUIValFormE2rom(u8 cur_mod, u8 *buf)
{
	u16 start_addr = 0;
	u8 temp = cur_mod;

	u8 *tbuf = buf;
	u8 check_byte = 0;

	while(temp--)
	{
		start_addr +=  150;
	}

	E2Read(start_addr, tbuf, 124);
	E2Read(start_addr + 124, &check_byte, 1);

//	if(check_byte != CanFrameCheckByte((u8*)tbuf,124))
		
}

void SetPUIVal2E2rom()
{
	u16 start_addr = 0;
	u8 temp = cfig_manager->mode.cfg_mode_val;
	u8 *tbuf = (u8*)&cfig_manager->cfig_mode_buf;
	u8 check_byte = 0;

	while(temp--)
	{
		start_addr += 150;
	}

	//check_byte = CanFrameCheckByte((u8*)tbuf,124);
	E2Write(start_addr, tbuf, 124);
//	E2Write(start_addr+124, &check_byte, 1);
}

void RecordFunDeal()
{	
	if(cfig_manager->mode.cfg_run_mode == CFIG_MODE)
	{
		if(cfig_manager->mode.cfg_mode_val < CFIG_PID_MODE)
		{
			if(cfig_manager->mode.record_r_en == 1)
			{
			//	GetPUIValFormE2rom(cfig_manager->mode.cfg_mode_val,(u8*)(&(cfig_manager->cfig_mode_buf)));

				cfig_manager->obj = OBJ_PWM;

				disp_manager.seg_manager.cfig_seg_en.obj_en = 1;
				cfig_manager->pwm_val = cfig_manager->cfig_mode_buf.pwm[cfig_manager->PwmArrayNum];
				cfig_manager->p_ad_val = cfig_manager->cfig_mode_buf.p_i_u[cfig_manager->PwmArrayNum];
				cfig_manager->i_ad_val = cfig_manager->cfig_mode_buf.i[cfig_manager->PwmArrayNum];
				cfig_manager->u_ad_val = cfig_manager->cfig_mode_buf.u[cfig_manager->PwmArrayNum];

				disp_manager.seg_manager.cfig_seg_en.value_en = 1;

				cfig_manager->mode.record_r_en = 0;
			}
			else if(cfig_manager->mode.record_w_en==1)
			{
			//	SetPUIVal2E2rom();
				cfig_manager->mode.record_w_en = 0;
			}
		}
	}
}
