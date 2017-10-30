
#ifndef __COM_H
#Define __COM_H
#include "config.h"



#define ON					1
#define OFF					0
#define HIGH				1
#define LOW					0



#define QUEUE_SIZE 			11

typedef struct
{
	u8 front;
	u8 rear;
	u8 d[QUEUE_SIZE];
} Queue, *QueuePtr;







void queue_init(Queue *queue);
u8 queue_en(Queue *queue, u8 e);
u8 queue_de(Queue *queue, u8 *e);
u8 queue_is_empty(Queue *queue);
u8 queue_is_full(Queue *queue);
u8 queue_traverse(Queue *queue, u8 *pd);
u8 check_sum(u8 *d, u8 len);
void delay(u32 cnt);
void delay_1ms(u32 cnt);		//@22.1184MHz
void to_bin(u8 num, u8 d[]);
void to_bin2(u8 num, u8 d[]);
float ad_to_vol(u16 ad_val);
void soft_reset(void);



#endif
