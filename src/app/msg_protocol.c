
#include <string.h>
#include "msg_protocol.h"
#include "USART.h"
#include "EEPROM.h"
#include "timerhl.h"
#include "com.h"
#include "eepromhl.h"


extern u8 g_alarm_signal[4];
extern u32 time_lamp_on_sec[4];
extern u8 saving_flag;
extern u32 g_curr_old[4], g_curr_new[4], g_curr_tmp[4];
extern u8 work_mode;

void frame_anlayze(u8 *rx_data, FrameStr frame)
{
	u8 data_len = 0, i = 0;
	u8 crc = 0; 

	frame->sync_1 = rx_data[0];
	frame->len = rx_data[1] | rx_data[2]<<8;
	frame->addr = rx_data[3]<<8 | rx_data[4];
	frame->child_addr = rx_data[5]<<8 | rx_data[6];
	frame->sync_2 = rx_data[7];
	frame->cmd = rx_data[8]<<8 | rx_data[9];
	
	data_len = frame->len - 2;
	memcpy(frame->pd, &rx_data[10], data_len);
		
	data_len = 10 + data_len;
	frame->crc = rx_data[data_len++];
	frame->end_field = rx_data[data_len];
	
	frame->para_total = 0;
}


// void frame_pack(FrameStr frame, u8 *tx_data)
// {
// 	u8 i = 0, crc_data_len = 0, crc = 0, tmp[50] = {0};
// 	
// 	tx_data[0] = frame->sync_1;
// 	tx_data[1] = frame->len>>8;
// 	tx_data[2] = frame->len&0xFF;
// 	tx_data[3] = frame->addr>>8;
// 	tx_data[4] = frame->addr&0xFF;
// 	tx_data[5] = frame->sync_2;
// 	tx_data[6] = frame->cmd>>8;
// 	tx_data[7] = frame->cmd&0xFF;
// 	for (i=0; i<frame->len-2; i++)
// 	{
// 		tx_data[8+i] = frame->pd[i];
// 	}
// 	
// 	crc_data_len = frame->len + 5;
// 	for (i=0; i<crc_data_len; i++)
// 	{
// 		tmp[i] = tx_data[i+1];
// 	}
// 	
// 	tx_data[crc_data_len+1] = check_sum(tmp, crc_data_len);
// 	tx_data[crc_data_len+2] = frame->end_field;
// }


void get_alarm_msg(FrameStr rx_f, u8 *tx_data, u8 *tx_data_len)
{
	u8 crc_data_len = 0, crc = 0;
	u8 tmp[8] = {0}, i = 0;
	u8 rx_para1 = 0, rx_para2 = 0, tx_para2[10] = {0}, tx_para1 = 0;
	u8 para2_cnt = 0;
	u16 tx_cmd = CTU_ALARM_STATUS_CMD, data_len = 0;
	u16 rx_cmd = 0;
	
	rx_para1 = rx_f->pd[0];	
 	rx_para2 = rx_f->pd[1];
	
	to_bin(rx_para1, tmp);
	for (i=0; i<8; i++)
	{
		if (tmp[i] == 1)
		{
			tx_para2[para2_cnt++] = i+1;
			tx_para2[para2_cnt++] = g_alarm_signal[i];	
			tx_para1++;			
		}
	}
	
	if (rx_para2 == 1)
	{
		saving_flag = 1;
		for (i=0; i<4; i++)
		{
			if (0x1 == work_mode)
			{
				time_lamp_on_sec[i] = 15;
			}
			else
			{
				time_lamp_on_sec[i] = 60;
			}
			
			
			g_curr_old[i] = 0;
			g_curr_new[i] = 0;
		}		
	}
	
	data_len = 2 + 1 + 2 + para2_cnt;
	tx_data[0] = SYNC;
	tx_data[1] = data_len&0xFF;
	tx_data[2] = data_len>>8;	
	tx_data[3] = rx_f->addr>>8;
	tx_data[4] = rx_f->addr&0xFF;
	tx_data[5] = SYNC;	
	tx_data[6] = tx_cmd>>8;
	tx_data[7] = tx_cmd&0xFF;
	tx_data[8] = rx_f->child_addr>>8;
	tx_data[9] = rx_f->child_addr&0xFF;
	tx_data[10] = tx_para1;
	for (i=0; i<para2_cnt; i++)
	{
		tx_data[11+i] = tx_para2[i];
	}
	
	crc_data_len = data_len + 5;
	memcpy(tmp, &tx_data[6], data_len);
	
	tx_data[crc_data_len+1] = ~check_sum(tmp, data_len);
	tx_data[crc_data_len+2] = END_FIELD;
	*tx_data_len = data_len + 8;
}




void protocol_action(void)
{
	Frame frame_struct, frame_struct2;
	u8 rx_data[50] = {0}, tx_data[50] = {0}, tx_data_len = 0;
	u16 cmd = 0;
	u8 para_total = 0;

	memset(&frame_struct2, 0, sizeof(Frame));
		
	uart2_rx_data(rx_data);

	frame_anlayze(rx_data, &frame_struct);

	switch (frame_struct.cmd)
	{
		case CHILD_ALARM_STATUS_CMD:						
			if (frame_struct.len == 4)
			{
				get_alarm_msg(&frame_struct, tx_data, &tx_data_len);
				uart2_tx_data(tx_data, tx_data_len);
			}
		break;

	}

}



