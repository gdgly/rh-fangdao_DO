


#include "config.h"
#include "GPIO.h"
#include "delay.h"
#include "ADC.h"
#include "bsp.h"
#include "USART.h"
#include "rn8209d.h"
#include "com.h"
#include "timerhl.h"
#include "adchl.h"
#include "EEPROM.h"
#include "PCA.h"
#include "msg_protocol.h"
#include "eepromhl.h"
#include "code_switch.h"


extern Queue *g_queue[4];
extern u8 g_alarm_signal[4];
u8 work_mode = 0;

/********************  **************************/
void main(void)
{	
	u16 ad_val = 0;	
	u8 i = 0, flag = 1, arr[3] = 0, status = 0;
	u8 saving_level[4] = {0};
	u32 tmp = 0;
	bsp_init();
	all_irq_enable();	
	rn8209_init();	
	
	work_mode = get_app_wrok_status();
	
	TX1_write2buff(0xEE);
	TX1_write2buff(work_mode);
	
	pwm2_config();
	PWMn_Update(PCA0, 128);	
	
	set_port_val(RS485_DE, 0);
	i = 0;
	while (1)
	{	

		watchdog_clear();
		switch (status)
		{
			case 0:
				set_channel(i);
				status++;
			break;
			
			case 1:			
				if(IS_TIMEOUT_1MS(timer_channel, 1000))
				{	
					app(i);
					i++;
					if (i > 3)
					{
						i = 0;
					}
		
				}		
				status++;
			break;
			
			case 2:
				protocol_action();
				status++;
			break;
			
			case 3:
				set_alarm_status(g_alarm_signal);
				status = 0;
			break;
			
			default:
			break;
		}			
		
		if (IS_TIMEOUT_1MS(timer_led5, 500))
		{
			if (get_port_val(LED_5_R) == 1)
			{
				set_port_val(LED_5, 0);	
			}
			else
			{
				set_port_val(LED_5, 1);
			}			
		}
				
	}
}




