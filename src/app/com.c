

#include "com.h"



void delay(u32 cnt)
{
	while(cnt--);
}


void delay_1ms(u32 cnt)		//@22.1184MHz
{

	unsigned char i, j;
	while (cnt)
	{
		_nop_();
		_nop_();
		i = 22;
		j = 128;
		do
		{
			while (--j);
		} while (--i);
		cnt--;
	}
}


void queue_init(Queue *queue)
{
	queue->front = 0;
	queue->rear = 0;
}


u8 queue_en(Queue *queue, u8 e)
{

	if ((queue->rear+1)%QUEUE_SIZE == queue->front)
	{
		return FALSE;
	}
	
	queue->d[queue->rear] = e;
	queue->rear = (queue->rear+1) % QUEUE_SIZE;
	
	return TRUE;
}


u8 queue_de(Queue *queue, u8 *e)
{
	
	if (queue->front == queue->rear)
	{
		return FALSE;
	}
	
	
	*e = queue->d[queue->front];
	queue->front = (queue->front+1) % QUEUE_SIZE;
	
	return TRUE;
}

u8 queue_is_empty(Queue *queue)
{
	if (queue->front == queue->rear)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


u8 queue_is_full(Queue *queue)
{
	if ((queue->rear+1)%QUEUE_SIZE == queue->front)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


u8 queue_traverse(Queue *queue, u8 *pd)
{
	u8 p = queue->front;
	
	if (queue->rear == queue->front)
	{
		return FALSE;
	}
	
	while (p != queue->rear)
	{
		*pd++ = queue->d[p];
		p = (p+1) % QUEUE_SIZE;
	}

	return TRUE;
}


u8 check_sum(u8 *d, u8 len)
{
	u16 sum = 0;
	u8 i = 0;
	
	for (i=0; i<len; i++)
	{
		sum += d[i];	
		if (sum & 0xFF00)
		{
			sum = (sum & 0x00FF);
		}
	}
	
	sum ^= 0x00FF;	
	
	return (u8)sum;
}

/*********************************************************** 
Function   	: toBin
Description	: decimal base covertion binary
Parameter  	: num is decimal base. d[] is seve binary values
Return     	: void
Date		: 2014.7.27
Authot		: George	
************************************************************/
void to_bin2(u8 num, u8 d[])
{
	u8 i;
	
	for(i=0; i<8; i++)
	{
		d[8-1-i] = num>>i&1;
	}
}


/*********************************************************** 
Function   	: toBin2
Description	: decimal base covertion binary
Parameter  	: num is decimal base. d[] is seve binary values
Return     	: void
Date		: 2014.7.27
Authot		: George	
************************************************************/
void to_bin(u8 num, u8 d[])
{
	u8 i;
	
	for(i=0; i<8; i++)
	{
		d[i] = num>>i&1;
	}
}



float ad_to_vol(u16 ad_val)
{
	float vol = 0;

	vol = (float)ad_val * 5 / 1024;

	return vol;
}


void soft_reset(void)
{
	((void(code*) (void))0x0000)();
}


