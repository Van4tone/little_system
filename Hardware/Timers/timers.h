#ifndef _TIMERS_H
#define _TIMERS_H

#include "gpio_head.h"

#define US100 3000
#define MS_1 	3000

void Timer0Set(u16 time);

void Timer0Init(void);

void Timer1Set(u16 time);

void Timer1Init(void);

void T0Delay();
#endif