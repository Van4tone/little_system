#ifndef _TIMERS_FUN_H
#define _TIMERS_FUN_H
#include "timers.h"

typedef struct Task_Cfig {
		u8 cycle_count;
		u8 key_count;
		u8 ad_count;
		u8 record_count;
		u8 display_en;
        u8 key_en;
		u8 record_en;
		u8 ad_en;
		u8 can_en;
}task_cfig;


task_cfig task;



unsigned char val = 0;
unsigned char tm1 = 0,tm2 = 0,key_count=0;

void Timer0TaskManager();

void Timer0IntTaskDeal();

void Timer0IntSysDeal();

void Timer0IntDeal();

#endif