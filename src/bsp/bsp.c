

#include "bsp.h"
#include "GPIO.h"
#include "USART.h"
#include "ADC.h"
#include "PCA.h"
#include "timer.h"


void gpio_config(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	
	gpio_init_structure.Mode = GPIO_HighZ;
	gpio_init_structure.Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Inilize(GPIO_P1,&gpio_init_structure);
}

void all_irq_disable(void)
{
	EA = 0;
}

void all_irq_enable(void)
{
	EA = 1;
}

void watchdog_config(void)
{
	WDT_CONTR = 0X3F;
}

void watchdog_clear(void)
{
	WDT_CONTR = 0X3F;
}

void uart1_config(void)
{
	COMx_InitDefine uart_init_structure;
		
	uart_init_structure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	uart_init_structure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	uart_init_structure.UART_BaudRate  = 9600;				//波特率, 一般 110 ~ 115200
	uart_init_structure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	uart_init_structure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	uart_init_structure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	uart_init_structure.UART_Polity    = PolityHigh;			//中断优先级, PolityLow,PolityHigh
	uart_init_structure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	uart_init_structure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &uart_init_structure);
}


void uart2_config(void)
{
	COMx_InitDefine uart_init_structure;
		
	uart_init_structure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	uart_init_structure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	uart_init_structure.UART_BaudRate  = 9600;				//波特率, 一般 110 ~ 115200
	uart_init_structure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	uart_init_structure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	uart_init_structure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	uart_init_structure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	uart_init_structure.UART_P_SW      = UART2_SW_P10_P11;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	uart_init_structure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART2, &uart_init_structure);
}







void spi_init(void)
{
	SPI_Disable();
}



void pwm2_config(void)
{
	PCA_InitTypeDef pca_init_structure;
	
	ACC = P_SW1;
	ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
	ACC |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
	P_SW1 = ACC; 
	
	pca_init_structure.PCA_IoUse = PCA_P34_P35_P36_P37;
	pca_init_structure.PCA_Clock = PCA_Clock_1T;
	pca_init_structure.PCA_Polity = PolityLow;
	pca_init_structure.PCA_Mode     = PCA_Mode_PWM;		
	pca_init_structure.PCA_PWM_Wide = PCA_PWM_8bit;		
	pca_init_structure.PCA_Interrupt_Mode = DISABLE;		
	pca_init_structure.PCA_Value    = 128 << 8;			
	PCA_Init(PCA0, &pca_init_structure);	
}



/*
void timer0_config(void)
{
	TIM_InitTypeDef tim_init_structure;
	
	tim_init_structure.TIM_Mode = TIM_16BitAutoReload;
	tim_init_structure.TIM_Polity = PolityLow;
	tim_init_structure.TIM_Interrupt = TIM_CLOCK_1T;
	tim_init_structure.TIM_ClkOut = ENABLE;
	tim_init_structure.TIM_Value = 65536UL - (MAIN_Fosc / 100000UL);
	tim_init_structure.TIM_Run = ENABLE;
	Timer_Inilize(Timer0,&tim_init_structure);
}
*/

void timer0_config(void)		
{
	AUXR |= 0x80;		//
	TMOD &= 0xF0;		//
	TL0 = 0x9A;		//
	TH0 = 0xA9;		
	TF0 = 0;		//
	TR0 = 1;		//
	ET0 = 1;
}


void adc_config(void)
{
	ADC_InitTypeDef adc_init_structure;
	
	adc_init_structure.ADC_Px = ADC_P13 | ADC_P14 | ADC_P15 | ADC_P16 | ADC_P17;
	adc_init_structure.ADC_Speed = ADC_360T;
	adc_init_structure.ADC_Power = ENABLE;
	adc_init_structure.ADC_AdjResult = ADC_RES_H8L2;
	adc_init_structure.ADC_Polity = PolityLow;
	adc_init_structure.ADC_Interrupt = DISABLE;
	ADC_Inilize(&adc_init_structure);					//初始化
	ADC_PowerControl(ENABLE);
}


u8 get_port_val(u8 port)
{
	u8 val = 0;
	
	switch (port)
	{
		case CODE_1:
			val = CODE_1();
		break;
		
		case CODE_2:
			val = CODE_2();
		break;
		
		case CODE_3:
			val = CODE_3();
		break;
		
		case CODE_4:
			val = CODE_4();
		break;
		
		case LED_5_R:
			val = LED_5_R();
		break;
	}
	
	return val;
}


void set_port_val(u8 port, u8 val)
{
	switch (port)
	{
		case CH_CTRL0:
			if (val == 1)
			{
				CH_CTRL0_H();
			}
			else
			{
				CH_CTRL0_L();
			}
		break;
		
		case CH_CTRL1:
			if (val == 1)
			{
				CH_CTRL1_H();
			}
			else
			{
				CH_CTRL1_L();
			}
		break;
		
		case LED_1:
			if (val == 1)
			{
				LED_1_H();
			}
			else
			{
				LED_1_L();
			}
		break;
		
		case LED_2:
			if (val == 1)
			{
				LED_2_H();
			}
			else
			{
				LED_2_L();
			}
		break;
		
		case LED_3:
			if (val == 1)
			{
				LED_3_H();
			}
			else
			{
				LED_3_L();
			}
		break;
		
		case LED_4:
			if (val == 1)
			{
				LED_4_H();
			}
			else
			{
				LED_4_L();
			}
		break;
		
		case LED_5:
			if (val == 1)
			{
				LED_5_H();
			}
			else
			{
				LED_5_L();
			}
		break;
		
		case RS485_DE:
			if (val == 1)
			{
				RS485_DE_H();
			}
			else
			{
				RS485_DE_L();
			}
		break;

		case ALARM_OUT1:
			if (val == 1)
			{
				ALARM_OUT1_H();
			}
			else
			{
				ALARM_OUT1_L();
			}
		break;
		
		case ALARM_OUT2:
			if (val == 1)
			{
				ALARM_OUT2_H();
			}
			else
			{
				ALARM_OUT2_L();
			}
		break;
		
		case ALARM_OUT3:
			if (val == 1)
			{
				ALARM_OUT3_H();
			}
			else
			{
				ALARM_OUT3_L();
			}
		break;
		
		case ALARM_OUT4:
			if (val == 1)
			{
				ALARM_OUT4_H();
			}
			else
			{
				ALARM_OUT4_L();
			}
		break;
	}
}




void bsp_init(void)
{
	gpio_config();
	watchdog_config();
	uart1_config();
	uart2_config();
// 	pwm2_config();
	spi_init();
	adc_config();
	timer0_config();
}



