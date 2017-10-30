

#include "code_switch.h"
#include "bsp.h"


u8 get_switch_1_val()
{
	u8 val = 0;
		
	val = (get_port_val(CODE_1) & 0x1) | ((get_port_val(CODE_2) & 0x01)<<1); 

	return val;
}



u8 get_switch_2_val()
{
	u8 val = 0;
		
	val = (get_port_val(CODE_3) & 0x1) | ((get_port_val(CODE_4) & 0x01)<<1); 

	return val;
}


u8 get_app_wrok_status()
{
	u8 val = 0;
		
	val = get_port_val(CODE_1) & 0x1; 

	return val;
}




