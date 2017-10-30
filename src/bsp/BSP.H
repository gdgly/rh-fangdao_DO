

#ifndef __BSP_H
#define __BSP_H
#include "config.h"


#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5


#define RN_IRQ()						P32
#define CODE_1()						P34
#define CODE_2()						P35
#define CODE_3()						P36
#define CODE_4()						P37	
#define LED_5_R()						P24



#define CH_CTRL0_H()					P54=1
#define CH_CTRL0_L()					P54=0
#define CH_CTRL1_H()					P55=1
#define CH_CTRL1_L()					P55=0
#define LED_1_H()						P00=1
#define LED_1_L()						P00=0
#define LED_2_H()						P27=1
#define LED_2_L()						P27=0
#define LED_3_H()						P26=1
#define LED_3_L()						P26=0
#define LED_4_H()						P25=1
#define LED_4_L()						P25=0
#define LED_5_H()						P24=1
#define LED_5_L()						P24=0
#define ALARM_OUT1_H()					P01=1
#define ALARM_OUT1_L()					P01=0
#define ALARM_OUT2_H()					P02=1
#define ALARM_OUT2_L()					P02=0
#define ALARM_OUT3_H()					P03=1
#define ALARM_OUT3_L()					P03=0
#define ALARM_OUT4_H()					P33=1
#define ALARM_OUT4_L()					P33=0
#define RS485_DE_H()					P12=1
#define RS485_DE_L()					P12=0



typedef enum
{
	CH_CTRL0,
	CH_CTRL1,
	LED_1,
	LED_2,
	LED_3,
	LED_4,
	LED_5,
	RS485_DE,
	ALARM_OUT1,
	ALARM_OUT2,
	ALARM_OUT3,
	ALARM_OUT4
} SET_PORT;

typedef enum
{
	CODE_1,
	CODE_2,
	CODE_3,
	CODE_4,
	LED_5_R
} GET_PORT;





void all_irq_disable(void);
void all_irq_enable(void);
void watchdog_clear(void);
void pwm2_config(void);
u8 get_port_val(u8 port);
void set_port_val(u8 port, u8 val);
void bsp_init(void);







#endif
