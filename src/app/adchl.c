/*************************************************
  Copyright (C), RoadLighting Tech. Co., Ltd.
  File name: flash.c     
  Author: George      Version: 1.0        Date: 2015/08/20
  Description:  Flash操作
  Others: 无        
  Function List:  
    1. flash_page_erase
	2. flash_write_data
	3. flash_read_data
	4. flash_write_byte
	5. flash_read_byte
  History:                            
    1. Date:
       Author:
       Modification:
*************************************************/
#include "adchl.h"
#include "adc.h"
#include "com.h"
#include "code_swtich.h"
#include "rn8209d.h"
#include "bsp.h"
#include "USART.h"
#include "EEPROM.h"
#include "timerhl.h"
#include "eepromhl.h"


extern u32 time_lamp_on_sec[4], time_lamp_off_sec[4], time_reset_hour;
extern u32 time_saving_sec;
extern u32 time_lamp_on_checking_sec[4];
extern u32 time_lamp_off_checking_sec[4];
extern u8 work_mode;

u8 g_alarm_signal[4] = {0};
static u8 g_status_new[4] = {0}, g_status_old[4] = {0}; 
static u8 g_power_on_flag[4] = {0};
static u8 g_write_sector1_en_flag[4] = {0}, g_write_sector2_en_flag[4] = {0};
u32 g_curr_old[4] = {0}, g_curr_new[4] = {0}, g_curr_tmp[4] = {0};
u8 g_power_switch_flag[4] = {0};

u32 g_unusual_curr[4] = {0};
u8 g_unusual_flag[4] = {0};

Queue *g_queue[4];
u8 saving_flag = 0;

/*
*Function: get_adc_val      
*Description: 无电时采集ad值  
*Calls:  无
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	channel：通道号
*Output: 无 
*Return: 采集到的ad值        
*Others: 无  
*/
u16 get_adc_val(u8 channel)
{
	u16 adc_val = 0, adc_tmp[10] = {0}, i = 0;
	u32 tmp = 0;
	for (i=0; i<10; i++)
	{
		adc_val = Get_ADC10bitResult(channel);
		tmp += adc_val;
	}

	adc_val = tmp / 10;
	
	return adc_val;
}


/*
*Function: set_channel      
*Description: 设置通道  
*Calls: 
	set_port_val
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	channel：通道号
*Output: 无 
*Return: 无        
*Others: 无  
*/
void set_channel(u8 channel)
{
	switch (channel)
	{
		case 0:
			set_port_val(CH_CTRL0, 0);
			set_port_val(CH_CTRL1, 0);
		break;
		
		case 1:
			set_port_val(CH_CTRL0, 1);
			set_port_val(CH_CTRL1, 0);
		break;
		
		case 2:
			set_port_val(CH_CTRL0, 0);
			set_port_val(CH_CTRL1, 1);
		break;
		
		case 3:
			set_port_val(CH_CTRL0, 1);
			set_port_val(CH_CTRL1, 1);
		break;	
	}
}

/*
*Function: set_alarm_status      
*Description: 设置报警状态  点亮相应的led灯  
*Calls:  
	set_port_val
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	val：报警状态值
*Output: 无 
*Return: 无      
*Others: 无  
*/
void set_alarm_status(u8 *val)
{
	if (val[0] == 1)
	{
		set_port_val(LED_1, 0);	
		set_port_val(ALARM_OUT1, 1);
	}
	else
	{		
		set_port_val(LED_1, 1);
		set_port_val(ALARM_OUT1, 0);
	}
	
	if (val[1] == 1)
	{		
		set_port_val(LED_2, 0);
		set_port_val(ALARM_OUT2, 1);
	}
	else
	{		
		set_port_val(LED_2, 1);
		set_port_val(ALARM_OUT2, 0);
	}
	
	if (val[2] == 1)
	{
		set_port_val(LED_3, 0);
		set_port_val(ALARM_OUT3, 1);
	}
	else
	{
		set_port_val(LED_3, 1);
		set_port_val(ALARM_OUT3, 0);
	}
	
	if (val[3] == 1)
	{
		set_port_val(LED_4, 0);
		set_port_val(ALARM_OUT4, 1);
	}
	else
	{
		set_port_val(LED_4, 1);
		set_port_val(ALARM_OUT4, 0);
	}
}


/*
*Function: check_is_frist_lamp_on      
*Description: 检测路灯是否是第一工作  
*Calls:  
	ee2_read_data
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	channel：通道号
*Output: 无 
*Return: TRUE第一上电  FALSE不是第一次上电     
*Others: 无  
*/
bool check_is_frist_lamp_on(u8 channel)
{
	u8 i = 0;
	u8 tmp[3] = {0};
	
	ee2_read_data(channel, tmp);	// 读取通道的电流基数值
	
	for (i=0; i<3; i++)
	{
		if (tmp[i] != 0xFF)
		{
			return FALSE;
 		}
	}
	
	return TRUE;
}

/*
*Function: frist_lamp_on_hander      
*Description: 路灯第一上电处理
*Calls:  
	rn8209_get_curr
*Data Accessed: 
	g_queue
*Data Updated: 
	g_queue
*Input: 
	channel：通道号
*Output: 无 
*Return: 无      
*Others: 无  
*/
void frist_lamp_on_hander(u8 channel)
{
	u8 ret = 0, i = 0, tmp[3] = {0};
	u32 curr_total = 0, curr_average = 0, curr_tmp = 0;
	u32 curr[10] = {0};
	
	curr_tmp = rn8209_get_curr();

	ret = queue_is_full(&g_queue[channel]);
	if (ret == FALSE)	// 队列不满
	{
		queue_en(&g_queue[channel], curr_tmp);
	}
	
	ret = queue_is_full(&g_queue[channel]);
	if (ret == TRUE)			// 队列满
	{
		queue_traverse(&g_queue[channel], curr);
		for (i=0; i<10; i++)
		{
			curr_total += curr[i];		// 计算采集10次电流值得和
		}
		
		curr_average = curr_total / 10;
		curr_average = curr_average * 95 / 100; 
		if (curr_tmp < curr_average)	//  
		{
			tmp[0] = curr[10]>>16;
			tmp[1] = curr[10]>>8;
			tmp[2] = curr[10]&0xFF;	
			ee2_write_data(channel, tmp);

		}
		else
		{
			queue_de(&g_queue[channel]); 
		}
	}
}


/*
*Function: read_lamp_work_curr_base      
*Description: 读取路灯没有被盗的时候的电流基数值  
*Calls:  
	ee2_read_data
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	channel：通道号
*Output: 无 
*Return: 无      
*Others: 无  
*/
u32 read_lamp_work_curr_base(u8 channel)
{
	u8 tmp[3] = {0};
	u32 curr = 0;
	
	ee2_read_data(channel, tmp);
	
	curr = tmp[0];
	curr = (curr<<8) + tmp[1];
	curr = (curr<<8) + tmp[2];
	
	return curr;
}

/*
*Function: write_lamp_work_curr_base      
*Description: 记录路灯工作状态下的电流值 
*Calls:  
	rn8209_get_curr
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	channel：通道号
*Output: 无 
*Return: 无      
*Others: 无  
*/
void write_lamp_work_curr_base(u8 channel)
{
	u32 tmp_curr = 0, curr = 0, curr_total = 0;
	u8 i = 0, tmp[3] = {0};
	
	for (i=0; i<10; i++)
	{
		tmp_curr = rn8209_get_curr();	// 连续采集电流值
		delay_1ms(2);
		curr_total += tmp_curr;
	}
	
	curr = curr_total / 10;
	tmp[0] = curr>>16;
	tmp[1] = curr>>8;
	tmp[2] = curr&0xFF;	
	
	ee1_write_data(tmp);
}


/*
*Function: get_lamp_work_status      
*Description: 获取路灯工作状态  
*Calls:  
	rn8209_get_vol
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无 
*Return: 0表示关灯  1表示开灯     
*Others: 无  
*/
u8 get_lamp_work_status(void)
{
	u8 status = 0, i = 0, cnt = 0;
	u16 vol_val = 0; 

	for (i=0; i<10; i++)
	{
		vol_val = rn8209_get_vol();
		delay_1ms(2);
		cnt++;
		
		if (vol_val < 100)	// 路灯有电无电检测
		{	
			if (cnt > 8)
			{
				cnt = 0;
				status = 0;   	 //  road lamp no run
			}		
		}
		else
		{
			cnt = 0;
			status = 1; 	// road lamp run
		}
	}
	
	return status;
}





void unusual_curr(u8 channel)
{
	u32 i = 0, cnt = 0;
	for (i=0; i<10; i++)
	{
		g_unusual_curr[channel] = rn8209_get_curr();

		if (g_unusual_curr[channel] < 0x400)
		{
			cnt++;
			if (cnt > 8)
			{		
				cnt = 0;
				g_unusual_flag[channel] = 1;
				
				TX1_write2buff(0x33);
				TX1_write2buff(channel);
			}
		}
		else
		{
			cnt = 0;
			g_unusual_flag[channel] = 0;
			TX1_write2buff(0x22);
		}
	}
}


/*
*Function: get_lamp_on_alarm_status      
*Description: 获取路灯工作的时候的报警状态  
*Calls:  
	set_port_val
*Data Accessed: 
	curr_old
	curr_tmp
	curr_new
	cnt
*Data Updated:
	curr_old
	curr_tmp
	curr_new
*Input: 
	val：报警状态值
*Output: 无 
*Return: 无      
*Others: 无  
*/
void get_lamp_on_alarm_status(u8 channel)
{
	u8 switch_val = 0, i = 0;
	u32 tmp = 0, cnt = 0;	
	
	switch_val = get_switch_2_val();		// 拨码开关 报警等级设置
	
	if (g_alarm_signal[channel] == 1)
	{
		TX1_write2buff(channel);
		TX1_write2buff(0xBB);
	}
	else
	{
	
		TX1_write2buff(0x11);
		TX1_write2buff(channel);
		
		g_curr_tmp[channel] = g_curr_old[channel] * 95 / 100;
		
		tmp = rn8209_get_curr();
		
		g_curr_new[channel] = tmp * (70 + switch_val * 10) / 100;
		
		if(g_curr_new[channel] >= g_curr_old[channel])		// 采集到的新值大于老值 
		{
			g_curr_old[channel] = g_curr_new[channel];
			
			if(0x1 == work_mode)
			{
				time_lamp_on_checking_sec[channel] = 10;
			}
			else
			{
				time_lamp_on_checking_sec[channel] = 60;		// 60秒
			}
			g_alarm_signal[channel] = 0;
		}
		else if (g_curr_new[channel] > g_curr_tmp[channel]) // 采集到的新值小于老值得95%
		{
			g_curr_old[channel] = g_curr_new[channel];
			
			if(0x1 == work_mode)
			{
				time_lamp_on_checking_sec[channel] = 10;
			}
			else
			{
				time_lamp_on_checking_sec[channel] = 60;		// 60秒
			}
			g_alarm_signal[channel] = 0;
		}
		else if (g_curr_tmp[channel] - g_curr_new[channel] > 0xA00)					// 采集到的新值小于较小  测认为报警	
		{
			TX1_write2buff(0x55);
			if (time_lamp_on_checking_sec[channel] == 0)		// 连续监测一分钟
			{
				if(0x1 == work_mode)
				{
					time_lamp_on_checking_sec[channel] = 10;
				}
				else
				{
					time_lamp_on_checking_sec[channel] = 60;		// 60秒
				}
			
				g_alarm_signal[channel] = 1;
				
			}	
			
		}
		
	}
}

/*
*Function: get_lamp_switch_alarm_status      
*Description: 从无电切换到无电时 会有电流变化，通过本函数检测报警状态  
*Calls:  
	read_lamp_work_curr_base
	rn8209_get_curr
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	channel：通道号
*Output: 无 
*Return: 报警状态   0不报警 1报警     
*Others: 无  
*/
u8 get_lamp_switch_alarm_status(u8 channel)
{
	u32 curr_base = 0, cnt = 0, tmp = 0;
	u8 status = 0, i = 0;
	
	curr_base = read_lamp_work_curr_base(channel);  // 读取 电流基数值
	
	curr_base = curr_base * 90 / 100;			// 
	for (i=0; i<10; i++)
	{
		tmp = rn8209_get_curr();
		delay_1ms(2);
		if (tmp < curr_base)
		{
			cnt++;
			if (cnt > 8)
			{
				cnt = 0;				
				status = 1;
			}
		}
		else
		{
			cnt = 0;
		}
	}
	
	return status;
}

/*
*Function: get_lamp_off_alarm_status      
*Description: 获取路灯不工作时报警状态  
*Calls:  
	set_port_val
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	ad_to_vol
*Output: 无 
*Return: 无      
*Others: 无  
*/
void get_lamp_off_alarm_status(u8 channel)
{
	float vol_val = 0, vol_base = 0;
	u16 ad_val = 0;
	
	ad_val = Get_ADC10bitResult(3+channel);	
	vol_val = ad_to_vol(ad_val);
	
	if (vol_val > 1.4)
	{			
		if (time_lamp_off_checking_sec[channel] == 0)		//无电检测时间
		{	
			if(0x1 == work_mode)
			{
				time_lamp_off_checking_sec[channel] = 10;	// 10 秒钟
			}
			else
			{
				time_lamp_off_checking_sec[channel] = 60;	// 60 秒钟
			}		
			
			g_alarm_signal[channel] = 1;
		}	
	}
	else
	{
	
		if(0x1 == work_mode)
		{
			time_lamp_off_checking_sec[channel] = 10;	// 10 秒钟
		}
		else
		{
			time_lamp_off_checking_sec[channel] = 60;	// 60 秒钟
		}	
		
		g_alarm_signal[channel] = 0;
	}
}


/*
*Function: set_alarm_status      
*Description: 设置报警状态  点亮相应的led灯  
*Calls:  
	set_port_val
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	val：报警状态值
*Output: 无 
*Return: 无      
*Others: 无  
*/
void app(u8 channel)
{		

	g_power_on_flag[channel] = check_is_frist_lamp_on(channel);	
		
	// 检测是否有电	
	g_status_old[channel] = g_status_new[channel];		
	g_status_new[channel] = get_lamp_work_status();
	
	if (g_status_new[channel] != g_status_old[channel])
	{
		if(g_status_new[channel] == 1)		// 从无电切换到有电
		{	
			if (0x1 == work_mode)
			{
				time_lamp_on_sec[channel] = 10;	// 测试模式
			}
			else
			{
				time_lamp_on_sec[channel] = 60;			// 1分钟
			}
			TX1_write2buff(0xA1);
			g_curr_tmp[channel] = 0;
			g_curr_new[channel] = 0;
			g_curr_old[channel] = 0;
		}
		else								// 从有电切换到无电
		{		
			if (0x1 == work_mode)
			{
				time_lamp_off_sec[channel] = 10;
			}
			else
			{
				time_lamp_off_sec[channel] = 60;	// 30秒
			}	
			TX1_write2buff(0xA2);
		}	
	}
	
	// 有电检测报警
	if (g_status_new[channel] == 1)		
	{		
		if (time_lamp_on_sec[channel] == 0)  // 状态切换等待时间到
		{
			get_lamp_on_alarm_status(channel);	// 获取有电报警状态
		}
	}		
	else							// 无电检测报警
	{
		if (time_lamp_off_sec[channel] == 0)	// 状态切换等待时间到
		{			
			get_lamp_off_alarm_status(channel);
		}
	} 		
}















