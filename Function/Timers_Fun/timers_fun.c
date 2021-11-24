#include "timers_fun.h"



void Timer0TaskManager()
{	
	if(task.key_count>=2)
	{
		task.key_en = 1;
		task.key_count = 0;
	}
	else
		task.key_count++;

	if(task.record_count>=4)
	{
		task.record_en = 1;
		task.record_count = 0;
	}
	else
		task.record_count++;

	if(task.ad_count>=1)
	{
		task.ad_en = 1;
		task.ad_count = 0;
	}
	else
		task.ad_count++;
	
	switch(task.cycle_count)
	{
		case 7:
			task.display_en = 1;
			break;
		default:
			if(task.cycle_count>=60)
				task.cycle_count = 0;
			break;
		
	}
	task.cycle_count++;
    
}

void Timer0IntTaskDeal()
{
	Timer0TaskManager();
}

void Timer0IntSysDeal()
{
	Timer0Set(MS_1);
}


void Timer0IntDeal()
{
	Timer0IntSysDeal();
	Timer0IntTaskDeal();
}

void Timer1IntDeal()
{
	
}
