

#ifndef __FAN_H
#define __FAN_H
#include "config.h"


#define CHANNEL_TOTAL			4
#define ELEC_VOL				100


#define TIME_CHECKING_CNT		10





void set_alarm_status(u8 *val);
void set_channel(u8 channel);
void app(u8 channel);

#endif
