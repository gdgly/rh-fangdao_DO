

#ifndef _EEPROMHL_H
#define _EEPROMHL_H




#define CH_1_CURR_ADDR							0x0100
#define CH_1_CURR_ADDR_SIZE						0x3
#define CH_2_CURR_ADDR							0x0104
#define CH_2_CURR_ADDR_SIZE						0x3
#define CH_3_CURR_ADDR							0x0108
#define CH_3_CURR_ADDR_SIZE						0x3
#define CH_4_CURR_ADDR							0x010B
#define CH_4_CURR_ADDR_SIZE						0x3

#define CH_1_VALID_CURR_ADDR					0x0120
#define CH_1_VALID_CURR_ADDR_SIZE				0x3
#define CH_2_VALID_CURR_ADDR					0x0124
#define CH_2_VALID_CURR_ADDR_SIZE				0x3
#define CH_3_VALID_CURR_ADDR					0x0128
#define CH_3_VALID_CURR_ADDR_SIZE				0x3
#define CH_4_VALID_CURR_ADDR					0x012B
#define CH_4_VALID_CURR_ADDR_SIZE				0x3



#define SAVING_LEVEL_ADDR						0x0010




void ee1_write_data(u8 channel, u8 *dat);
void ee2_write_data(u8 channel, u8 *dat);
void ee2_read_data(u8 channel, u8 *dat);
void ee1_copy_to_ee2(u8 channel);

#endif

