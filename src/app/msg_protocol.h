

#ifndef __MSG_PROTOCOL_H
#define __MSG_PROTOCOL_H
#include "config.h"

#define FRAME_DATA_FIELD_LEN			50
#define SYNC							0x68
#define END_FIELD						0x16

typedef struct
{
	u8 sync_1;
	u16 len;
	u16 addr;
	u16 child_addr;
	u8 sync_2;
	u16 cmd;
	u8 pd[FRAME_DATA_FIELD_LEN];
	u8 crc;
	u8 end_field;
	u8 para_total;

} Frame, *FrameStr;



#define CHILD_ALARM_STATUS_CMD				0x4001
#define CTU_ALARM_STATUS_CMD				0xC001

#define SAVING_STATUS_CMD					0x4002


void get_alarm_msg(FrameStr rx_f, u8 *tx_data, u8 *tx_data_len);
void protocol_action(void);
void frame_anlayze(u8 *d, FrameStr frame);


#endif
