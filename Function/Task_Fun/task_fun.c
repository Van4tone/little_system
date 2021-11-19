#include "task_fun.h"



void RecordFunTask()
{
	if(task.record_en == 1)
	{
		RecordFunDeal();
		task.record_en = 0;
	}
}

void AdFunTask()
{
	if(task.ad_en == 1)
		AdFunDeal();
	task.ad_en = 0;
}


void KeyFunTask()
{
	if(task.key_en == 1)
	{
		KeyFunDeal();
		task.key_en = 0;
	}
}



void DispFunTask()
{
	if(task.display_en == 1)
	{
		DispFunDeal();
		task.display_en = 0;
	}
}

void MsgFunTask()
{
	if(task.can_en == 1 )
	{
		PackMsg2SendDeal();
		task.can_en = 0;
	}
	PackMsgRecvDeal();
	
}


