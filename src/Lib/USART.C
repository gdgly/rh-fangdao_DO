
#include <string.h>
#include "USART.h"
#include "timerhl.h"
#include "msg_protocol.h"
#include "com.h"
#include "bsp.h"


u8 uart1_tim_enable_flag = 0, uart2_tim_enable_flag = 0;


COMx_Define	COM1,COM2;
u8	xdata TX1_Buffer[COM_TX1_Lenth];	//????
u8 	xdata RX1_Buffer[COM_RX1_Lenth];	//????
u8	xdata TX2_Buffer[COM_TX2_Lenth];	//????
u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//????

u8 USART_Configuration(u8 UARTx, COMx_InitDefine *COMx)
{
	u8	i;
	u32	j;
	
	if(UARTx == USART1)
	{
		COM1.id = 1;
		COM1.TX_read    = 0;
		COM1.TX_write   = 0;
		COM1.B_TX_busy  = 0;
		COM1.RX_Cnt     = 0;
		COM1.RX_TimeOut = 0;
		COM1.B_RX_OK    = 0;
		for(i=0; i<COM_TX1_Lenth; i++)	TX1_Buffer[i] = 0;
		for(i=0; i<COM_RX1_Lenth; i++)	RX1_Buffer[i] = 0;

		if(COMx->UART_Mode > UART_9bit_BRTx)	return 2;	//????
		if(COMx->UART_Polity == PolityHigh)		PS = 1;	//??????
		else									PS = 0;	//??????
		SCON = (SCON & 0x3f) | COMx->UART_Mode;
		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//?????
		{
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//?1T??
			if(j >= 65536UL)	return 2;	//??
			j = 65536UL - j;
			if(COMx->UART_BRT_Use == BRT_Timer1)
			{
				TR1 = 0;
				AUXR &= ~0x01;		//S1 BRT Use Timer1;
				TMOD &= ~(1<<6);	//Timer1 set As Timer
				TMOD &= ~0x30;		//Timer1_16bitAutoReload;
				AUXR |=  (1<<6);	//Timer1 set as 1T mode
				TH1 = (u8)(j>>8);
				TL1 = (u8)j;
				ET1 = 0;	//????
				TMOD &= ~0x40;	//??
				INT_CLKO &= ~0x02;	//?????
				TR1  = 1;
			}
			else if(COMx->UART_BRT_Use == BRT_Timer2)
			{
				AUXR &= ~(1<<4);	//Timer stop
				AUXR |= 0x01;		//S1 BRT Use Timer2;
				AUXR &= ~(1<<3);	//Timer2 set As Timer
				AUXR |=  (1<<2);	//Timer2 set as 1T mode
				TH2 = (u8)(j>>8);
				TL2 = (u8)j;
				IE2  &= ~(1<<2);	//????
				AUXR &= ~(1<<3);	//??
				AUXR |=  (1<<4);	//Timer run enable
			}
			else return 2;	//??
		}
		else if(COMx->UART_Mode == UART_ShiftRight)
		{
			if(COMx->BaudRateDouble == ENABLE)	AUXR |=  (1<<5);	//?????SysClk/2
			else								AUXR &= ~(1<<5);	//?????SysClk/12
		}
		else if(COMx->UART_Mode == UART_9bit)	//?????SysClk*2^SMOD/64
		{
			if(COMx->BaudRateDouble == ENABLE)	PCON |=  (1<<7);	//?????SysClk/32
			else								PCON &= ~(1<<7);	//?????SysClk/64
		}
		if(COMx->UART_Interrupt == ENABLE)	ES = 1;	//????
		else								ES = 0;	//????
		if(COMx->UART_RxEnable == ENABLE)	REN = 1;	//????
		else								REN = 0;	//????
		P_SW1 = (P_SW1 & 0x3f) | (COMx->UART_P_SW & 0xc0);	//??IO
		if(COMx->UART_RXD_TXD_Short == ENABLE)	PCON2 |=  (1<<4);	//????RXD?TXD, ???, ENABLE,DISABLE
		else									PCON2 &= ~(1<<4);
		return	0;
	}

	if(UARTx == USART2)
	{
		COM2.id = 2;
		COM2.TX_read    = 0;
		COM2.TX_write   = 0;
		COM2.B_TX_busy  = 0;
		COM2.RX_Cnt     = 0;
		COM2.RX_TimeOut = 0;
		COM2.B_RX_OK    = 0;
		for(i=0; i<COM_TX2_Lenth; i++)	TX2_Buffer[i] = 0;
		for(i=0; i<COM_RX2_Lenth; i++)	RX2_Buffer[i] = 0;

		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//?????
		{
			if(COMx->UART_Polity == PolityHigh)		IP2 |=  1;	//??????
			else									IP2 &= ~1;	//??????
			if(COMx->UART_Mode == UART_9bit_BRTx)	S2CON |=  (1<<7);	//9bit
			else									S2CON &= ~(1<<7);	//8bit
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//?1T??
			if(j >= 65536UL)	return 2;	//??
			j = 65536UL - j;
			AUXR &= ~(1<<4);	//Timer stop
			AUXR &= ~(1<<3);	//Timer2 set As Timer
			AUXR |=  (1<<2);	//Timer2 set as 1T mode
			TH2 = (u8)(j>>8);
			TL2 = (u8)j;
			IE2  &= ~(1<<2);	//????
			AUXR |=  (1<<4);	//Timer run enable
		}
		else	return 2;	//????
		if(COMx->UART_Interrupt == ENABLE)	IE2   |=  1;		//????
		else								IE2   &= ~1;		//????
		if(COMx->UART_RxEnable == ENABLE)	S2CON |=  (1<<4);	//????
		else								S2CON &= ~(1<<4);	//????
		P_SW2 = (P_SW2 & ~1) | (COMx->UART_P_SW & 0x01);	//??IO
	}
}


/*************** ???????? *******************************/

void TX1_write2buff(u8 dat)	//??????,??+1
{
	TX1_Buffer[COM1.TX_write] = dat;	//?????
	if(++COM1.TX_write >= COM_TX1_Lenth)	
	{	
		COM1.TX_write = 0;
	}
	if(COM1.B_TX_busy == 0)		//??
	{  
		COM1.B_TX_busy = 1;		//???
		TI = 1;					//??????
	}
}

void TX2_write2buff(u8 dat)	//??????,??+1
{
	TX2_Buffer[COM2.TX_write] = dat;	//?????
	if(++COM2.TX_write >= COM_TX2_Lenth)	COM2.TX_write = 0;

	if(COM2.B_TX_busy == 0)		//??
	{  
		COM2.B_TX_busy = 1;		//???
		SET_TI2();				//??????
	}
}

void PrintString1(u8 *puts)
{
    for (; *puts != '\0';	puts++)  TX1_write2buff(*puts); 	//?????0??
}

void PrintString2(u8 *puts)
{
    for (; *puts != '\0';	puts++)  TX2_write2buff(*puts); 	//?????0??
}


void uart1_tx_data(u8 *tx_data, u8 len)
{	
	u8 i = 0;
	for (i=0; i<len; i++)
	{
		TX1_write2buff(tx_data[i]);
	}
}


void uart1_rx_data(u8 *rx_data)
{
	Frame f;
	u16 len = 0, i = 0;
	u8 tmp[50] = {0},  start_field = 0, end_field = 0, crc_field = 0, crc = 0;
	
	if (uart1_tim_enable_flag == 1)
	{	
		if (IS_TIMEOUT_1MS(timer_uart1, 30))
		{
			start_field = RX1_Buffer[0];
			len = RX1_Buffer[1] | RX1_Buffer[2]<<8;
			for (i=0; i<len+7; i++)
			{
				tmp[i] = RX1_Buffer[i+1];
			}
			
			crc_field = RX1_Buffer[len+7+1];
			end_field = RX1_Buffer[len+7+2];
			crc = check_sum(tmp, len+7);
			
			if (start_field == SYNC && end_field == END_FIELD && crc_field == crc)
			{
				memcpy(rx_data, RX1_Buffer, COM_RX1_Lenth);
			}
			
			uart1_tim_enable_flag = 0;
			COM1.RX_Cnt = 0;
			memset(RX1_Buffer, 0, sizeof(RX1_Buffer));			
		}
	}
}


void uart2_tx_data(u8 *tx_data, u8 len)
{	
	u8 i = 0;
	set_port_val(RS485_DE, 1);
	for (i=0; i<len; i++)
	{
		TX2_write2buff(tx_data[i]);
	}
	delay_1ms(30);
	set_port_val(RS485_DE, 0);

}


void uart2_rx_data(u8 *rx_data)
{
	Frame f;
	u16 len = 0, i = 0;
	u8 tmp[50] = {0},  start_field = 0, end_field = 0, crc_field = 0, crc = 0;
	
	if (uart2_tim_enable_flag == 1)
	{	
		if (IS_TIMEOUT_1MS(timer_uart2, 30))
		{
			
			start_field = RX2_Buffer[0];
			len = RX2_Buffer[1] | RX2_Buffer[2]<<8;
			
			for (i=0; i<len; i++)
			{
				tmp[i] = RX2_Buffer[i+8];
			}
			
			crc_field = RX2_Buffer[len+7+1];
			end_field = RX2_Buffer[len+7+2];
			crc = check_sum(tmp, len);
			
			if (start_field == SYNC && end_field == END_FIELD && crc_field == ~crc)
			{
				memcpy(rx_data, RX2_Buffer, COM_RX2_Lenth);		
			}
			
			uart2_tim_enable_flag = 0;			
			COM2.RX_Cnt = 0;
			memset(RX2_Buffer, 0, sizeof(RX2_Buffer));			
		}
	}
}




/********************* UART1????************************/
void UART1_int (void) interrupt UART1_VECTOR
{
	
	if(RI)
	{
		RI = 0;		
		if(COM1.B_RX_OK == 0)
		{
			uart1_tim_enable_flag = 1;			
			IS_TIMEOUT_1MS(timer_uart1, 0);
			RX1_Buffer[COM1.RX_Cnt++] = SBUF;
			
			if(COM1.RX_Cnt >= COM_RX1_Lenth)
			{
				COM1.RX_Cnt = 0;
			}
			COM2.RX_TimeOut = TimeOutSet2;
		}
	}

	if(TI)
	{
		TI = 0;
		if(COM1.TX_read != COM1.TX_write)
		{
		 	SBUF = TX1_Buffer[COM1.TX_read];
			if(++COM1.TX_read >= COM_TX1_Lenth)		
				COM1.TX_read = 0;
		}
		else	COM1.B_TX_busy = 0;
	}
}

/********************* UART2????************************/
void UART2_int (void) interrupt UART2_VECTOR
{
	if(RI2)
	{
		CLR_RI2();
		if(COM2.B_RX_OK == 0)
		{
			uart2_tim_enable_flag = 1;
			IS_TIMEOUT_1MS(timer_uart2, 0);
			RX2_Buffer[COM2.RX_Cnt++] = S2BUF;
			
			if(COM2.RX_Cnt >= COM_RX2_Lenth)	
			{	
				COM2.RX_Cnt = 0;
			}
 			COM2.RX_TimeOut = TimeOutSet2;
		}
	}

	if(TI2)
	{
		CLR_TI2();
		if(COM2.TX_read != COM2.TX_write)
		{
		 	S2BUF = TX2_Buffer[COM2.TX_read];
			if(++COM2.TX_read >= COM_TX2_Lenth)		COM2.TX_read = 0;
		}
		else	COM2.B_TX_busy = 0;
	}

}



