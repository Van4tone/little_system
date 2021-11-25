#include "timers.h"

void Timer0Set(u16 time)
{

	u16 last_time = 0xffff - time + 40;

	TMR0IF = 0;						//清除中断标志位
	TMR0H = last_time >> 8;
	TMR0L = last_time;

}

void Timer0Init(void)
 {
	//配置定时器0
	T0CON = 0x00; 					//禁止定时器0,16位定时器,内部指令周期, 8分频,即实际为3M

	Timer0Set(US100);
	TMR0IP = 0;						//Timer0为低优先级
	TMR0IE = 1;						//Timer0中断使能	
	//启动定时器
	TMR0ON = 1;						//使能定时器0
}


void Timer1Set(u16 time)
{
	u16 last_time = 0xffff - time + 40;

	TMR1IF = 0;						//清除中断标志位
	TMR1H = last_time >> 8;
	TMR1L = last_time;
}

void Timer1Init(void)
{
	T1CON = 0x90;
	Timer1Set(US100);

	TMR1IP = 1;
	TMR1IF = 0;
	TMR1IE = 1;
	TMR1ON = 0;
	
}

