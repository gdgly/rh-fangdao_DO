


#ifndef __TIMERHL_H
#define __TIMERHL_H
#include "config.h"


typedef struct 
{
	u16 ms;
	u8 sec;
	u8 min;
	u8 hour;
} TIM;


enum timer
{
    timer1_cnt,
    timer2_cnt,
	timer3_cnt,
	timer4_cnt,
	timer5_cnt,
	timer_led5,
	timer_channel,
	timer_uart1,
	timer_uart2,
	timer_max
};




#define IS_TIMEOUT_1MS(index, count)    ((g_time_cnt[(u8)(index)] >= (count)) ?  	\
                                        ((g_time_cnt[(u8)(index)] = 0) == 0) : 0)




extern volatile u32 g_time_cnt[(u8)timer_max];


#endif
