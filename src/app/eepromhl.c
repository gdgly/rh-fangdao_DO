
#include "EEPROM.h"
#include "eepromhl.h"


void ee1_write_data(u8 channel, u8 *dat)
{
	
	switch (channel)
	{
		case 0:
			EEPROM_write_n(CH_1_CURR_ADDR, dat, CH_1_CURR_ADDR_SIZE);
		break;
		
		case 1:
			EEPROM_write_n(CH_2_CURR_ADDR, dat, CH_2_CURR_ADDR_SIZE);
		break;
		
		case 2:
			EEPROM_write_n(CH_3_CURR_ADDR, dat, CH_3_CURR_ADDR_SIZE);
		break;
		
		case 3:
			EEPROM_write_n(CH_4_CURR_ADDR, dat, CH_4_CURR_ADDR_SIZE);
		break;
	}
}

void ee2_write_data(u8 channel, u8 *dat)
{

	switch (channel)
	{
		case 0:
			EEPROM_write_n(CH_1_VALID_CURR_ADDR, dat, CH_1_VALID_CURR_ADDR_SIZE);
		break;
		
		case 1:
			EEPROM_write_n(CH_2_VALID_CURR_ADDR, dat, CH_2_VALID_CURR_ADDR_SIZE);
		break;
		
		case 2:
			EEPROM_write_n(CH_3_VALID_CURR_ADDR, dat, CH_3_VALID_CURR_ADDR_SIZE);
		break;
		
		case 3:
			EEPROM_write_n(CH_4_VALID_CURR_ADDR, dat, CH_4_VALID_CURR_ADDR_SIZE);
		break;
	}	
}


void ee2_read_data(u8 channel, u8 *dat)
{

	switch (channel)
	{
		case 0:
			EEPROM_read_n(CH_1_VALID_CURR_ADDR, dat, CH_1_VALID_CURR_ADDR_SIZE);
		break;
		
		case 1:
			EEPROM_read_n(CH_2_VALID_CURR_ADDR, dat, CH_2_VALID_CURR_ADDR_SIZE);
		break;
		
		case 2:
			EEPROM_read_n(CH_3_VALID_CURR_ADDR, dat, CH_3_VALID_CURR_ADDR_SIZE);
		break;
		
		case 3:
			EEPROM_read_n(CH_4_VALID_CURR_ADDR, dat, CH_4_VALID_CURR_ADDR_SIZE);
		break;
	}
}


void ee1_copy_to_ee2(u8 channel)
{
	u8 tmp[5] = {0};
	
	switch (channel)
	{
		case 0:
			EEPROM_read_n(CH_1_CURR_ADDR, tmp, CH_1_CURR_ADDR_SIZE);
			EEPROM_write_n(CH_1_VALID_CURR_ADDR, tmp, CH_1_VALID_CURR_ADDR_SIZE);
		break;
		
		case 1:
			EEPROM_read_n(CH_2_CURR_ADDR, tmp, CH_2_CURR_ADDR_SIZE);
			EEPROM_write_n(CH_2_VALID_CURR_ADDR, tmp, CH_2_VALID_CURR_ADDR_SIZE);
		break;
		
		case 2:
			EEPROM_read_n(CH_3_CURR_ADDR, tmp, CH_3_CURR_ADDR_SIZE);
			EEPROM_write_n(CH_3_VALID_CURR_ADDR, tmp, CH_3_VALID_CURR_ADDR_SIZE);
		break;
		
		case 3:
			EEPROM_read_n(CH_4_CURR_ADDR, tmp, CH_4_CURR_ADDR_SIZE);
			EEPROM_write_n(CH_4_VALID_CURR_ADDR, tmp, CH_4_VALID_CURR_ADDR_SIZE);
		break;
	}
}



