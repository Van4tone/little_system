#include "common.h"


void interrupt low_priority Communicate_Timer0Int()
{
	if(RXB0IF && (PIE3 & 1<<3))
	{
		CanIntRecvFun();
		RXB0IF = 0;
	}

	if(TXB0IF && (PIE3 & 1<<2))
	{
		CanSendMsgFun(can_send_ctr.buf);
		TXB0IF = 0;
	}

	if(TMR0IF && TMR0IE)
    {
		Timer0IntDeal(); 
    }
}





void interrupt Extern_Int(void)
{
	if(TMR1IF && TMR1IE)
    {
		Timer1IntDeal(); 
    }
}


void ClearParame(void *data)
{
	unsigned char *temp = (unsigned char*)data;
	temp = 0;
}

void GetNumQuantile(u16 val,u8 *dec_buf)
{
	u8 Hundred,Ten;
	Hundred = 0;
	Ten = 0;
	
	while(val >99)
	{
	   val -= 100;
	   Hundred++;
	}
	dec_buf[2] = Hundred;
	
	while(val>=10)
	{
		val -= 10;
		Ten++;
	}
		
	dec_buf[1] = Ten;
	dec_buf[0] = val;
}


void Int_Init()
{
	IntruptInit();
	GIE = 1;
}

void HardWareInit()
{
	Key_Init();
	Max7219_Init();
	Timer0Init();
	I2C_Init();
	CanFunctionInit();
    ALARM_LAMP_DIR = GPIO_MODE_OUT;
	CMCON = 0x07;
}

void Sys_Init()
{
	System_ParamInit();
	Int_Init();
	HardWareInit();
	
}

void Working()
{
	//cfig_manager->mode.cfg_run_mode = RUN_MODE;
	cfig_manager->mode.cfg_run_mode = CFIG_MODE;

	ScanKeyStatus();
	ScanKeyStatus();

	if(key.cur_key == KEY_HE_NUM1)
		cfig_manager->mode.cfg_run_mode = CFIG_MODE;


	if(cfig_manager->mode.cfg_run_mode == CFIG_MODE)
 		DemericModeTaskDeal();
	else if(cfig_manager->mode.cfg_run_mode == RUN_MODE)
		RunModeTaskDeal();	
}



