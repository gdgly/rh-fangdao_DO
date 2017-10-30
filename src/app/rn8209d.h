
#ifndef __RN8209D_H
#define __RN8209D_H
#include "config.h"









#define RN8209_IARMS_ADDR			0x22
#define RN8209_URMS_ADDR			0x24

u32 rn8209_get_vol1(void);

void rn8209_init(void);
void rn8209_read_data(u8 addr, u8 *dat, u8 len);
void rn8209_write_data(u8 addr, u8 *dat, u8 len);
u16 rn8209_get_vol(void);
u32 rn8209_get_curr(void);
float rn8209_get_curr1(void);

#endif
