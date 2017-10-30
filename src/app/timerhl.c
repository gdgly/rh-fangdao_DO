
#include "timerhl.h"
#include "bsp.h"



static u16 time_ms = 0;
static u8 time_sec = 0;
static u8 time_min = 0;
static u8 time_hour = 0;


u32 time_lamp_on_sec[4] = {0}, time_lamp_off_sec[4] = {0};
u32 time_saving_sec = 0; 
u32 time_lamp_on_checking_sec[4] = {0}, time_lamp_off_checking_sec[4] = {0}; 

volatile u32 g_time_cnt[(u8)timer_max] = {0};



void timer_ms(void)
{

}


void timer_sec(void)
{
	u8 i = 0;
	
	for (i=0; i<4; i++)
	{
		if (time_lamp_off_sec[i] > 0)
		{
			time_lamp_off_sec[i]--;
		}
		
		if (time_lamp_on_checking_sec[i] > 0)
		{
			time_lamp_on_checking_sec[i]--;
		}
		
		if (time_lamp_off_checking_sec[i] > 0)
		{
			time_lamp_off_checking_sec[i]--;
		}
		
		if (time_lamp_on_sec[i] > 0)
		{
			time_lamp_on_sec[i]--;
		}
	}
	
	if (time_saving_sec > 0)
	{
		time_saving_sec--;
	}
	
}


void timer_min(void)
{

}


void timer_hour(void)
{
	
}

void time_init(void)
{
	u8 i = 0;
/*	
	for (i=0; i<tim_max; i++)
	{
		tim_ms[i] = 0;
		tim_sec[i] = 0;
		tim_min[i] = 0;
		tim_hour[i] = 0;
	}
*/
}



void timer0_int (void) interrupt TIMER0_VECTOR
{
	u16 i = 0;

	for (i = 0; i<(u8)timer_max; i++)
    {
		g_time_cnt[i]++;
    }
	
	time_ms++;	

	timer_ms();
	if (time_ms > 999)
	{
		time_ms = 0;
		time_sec++;
		timer_sec();
	
		if (time_sec > 59)
		{
			time_sec = 0;
			time_min++;	
			
			timer_min();
			
			if (time_min > 59)
			{
				
				time_min = 0;
				time_hour++;
				
				timer_hour();
				if (time_hour > 23)
				{
					time_hour = 0;
				}
			}
		}
	}
}

/********************* Timer0ÖÐ¶Ïº¯Êý************************/
/*
void timer0_int (void) interrupt TIMER0_VECTOR
{
	u16 i = 0;
	
	for (i=0; i<tim_max; i++)
	{
		tim_ms[i]++;	
		if (tim_ms > 999)
		{
			tim_ms[i] = 0;
			tim_sec[i]++;
			if (tim_sec[i] > 59)
			{
				tim_sec[i] = 0;
				tim_min[i]++;	
			
				if (tim_min[i] > 59)
				{
					tim_min[i] = 0;
					tim_hour[i]++;
			
					if (tim_hour[i] > 23)
					{
						tim_hour[i] = 0;
					}
				}
			}
		}
	}
}
*/



