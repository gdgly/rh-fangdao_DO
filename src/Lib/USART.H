
#ifndef __USART_H
#define __USART_H	 

#include "config.h"

#define	COM_TX1_Lenth	128
#define	COM_RX1_Lenth	128
#define	COM_TX2_Lenth	128
#define	COM_RX2_Lenth	128

#define	USART1	1
#define	USART2	2

#define	UART_ShiftRight	0		//??????
#define	UART_8bit_BRTx	(1<<6)	//8???,?????
#define	UART_9bit		(2<<6)	//9???,?????
#define	UART_9bit_BRTx	(3<<6)	//9???,?????

#define	UART1_SW_P30_P31	0
#define	UART1_SW_P36_P37	(1<<6)
#define	UART1_SW_P16_P17	(2<<6)	//????????
#define	UART2_SW_P10_P11	0
#define	UART2_SW_P46_P47	1


#define	TimeOutSet1		5
#define	TimeOutSet2		5

#define	BRT_Timer1	1
#define	BRT_Timer2	2

typedef struct
{ 
	u8	id;				//???

	u8	TX_read;		//?????
	u8	TX_write;		//?????
	u8	B_TX_busy;		//???

	u8 	RX_Cnt;			//??????
	u8	RX_TimeOut;		//????
	u8	B_RX_OK;		//?????
} COMx_Define; 

typedef struct
{ 
	u8	UART_Mode;			//??,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	u8	UART_BRT_Use;		//?????,   BRT_Timer1,BRT_Timer2
	u32	UART_BaudRate;		//???,       ENABLE,DISABLE
	u8	Morecommunicate;	//??????, ENABLE,DISABLE
	u8	UART_RxEnable;		//????,   ENABLE,DISABLE
	u8	BaudRateDouble;		//?????, ENABLE,DISABLE
	u8	UART_Interrupt;		//????,   ENABLE,DISABLE
	u8	UART_Polity;		//???,     PolityLow,PolityHigh
	u8	UART_P_SW;			//????,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(????????)
	u8	UART_RXD_TXD_Short;	//????RXD?TXD, ???, ENABLE,DISABLE

} COMx_InitDefine; 

extern	COMx_Define	COM1,COM2;
extern	u8	xdata TX1_Buffer[COM_TX1_Lenth];	//????
extern	u8 	xdata RX1_Buffer[COM_RX1_Lenth];	//????
extern	u8	xdata TX2_Buffer[COM_TX2_Lenth];	//????
extern	u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//????

u8	USART_Configuration(u8 UARTx, COMx_InitDefine *COMx);
void TX1_write2buff(u8 dat);	//??????,??+1
void TX2_write2buff(u8 dat);	//??????,??+1
void PrintString1(u8 *puts);
void PrintString2(u8 *puts);
void uart1_rx_data(u8 *rx_data);
void uart1_tx_data(u8 *tx_data, u8 len);
void uart2_rx_data(u8 *rx_data);
void uart2_tx_data(u8 *tx_data, u8 len);
//void COMx_write2buff(COMx_Define *COMx, u8 dat);	//??????,??+1
//void PrintString(COMx_Define *COMx, u8 *puts);

#endif

