


#include "rn8209d.h"
#include "bsp.h"
#include "spi.h"
#include "usart.h"


void rn8209_init(void)
{
	SPI_CS_L();
//	spi_write_byte(0xEA);
//	spi_write_byte(0xA5);		//写使能
		
	spi_write_byte(0xEA);		
	spi_write_byte(0xfA);		//复位
		
	spi_write_byte(0xEA);		
	spi_write_byte(0x5A);		//电流通道A选择

//	spi_write_byte()
//	spi_write_byte(0xEA);		
//	spi_write_byte(0xDC);		//写禁止
		
//	spi_write_byte(0xA9);		//脉冲捕获值
//	spi_write_byte(0x38);
		
	SPI_CS_H();
}

void rn8209_read_data(u8 addr, u8 *dat, u8 len)
{
	u8 i = 0;
	u8 tmp[5] = {0};
	SPI_CS_L();
	
	spi_write_byte(addr);
	tmp[0] = len;
	
	for (i=0; i<len; i++)
	{
		dat[i] = spi_read_byte();
		tmp[i] = dat[i];	
	}
//	tmp[i] = '\0';
	SPI_CS_H();
}



void rn8209_write_data(u8 addr, u8 *dat, u8 len)
{
	u8 i = 0, tmp[2] = {0};
	SPI_CS_L();
	
 	tmp[0] = 0xEA;
 	tmp[1] = 0xE5;
	
 	spi_write_byte(tmp[0]);
 	spi_write_byte(tmp[1]);
	
	spi_write_byte(addr|0x80);
	
	for (i=0; i<len; i++)
	{
		spi_write_byte(dat[i]);
	}
	
 	tmp[0] = 0xEA;
 	tmp[1] = 0xDC;	
 	spi_write_byte(tmp[0]);
 	spi_write_byte(tmp[1]);
	
	SPI_CS_H();
}



u16 rn8209_get_vol(void)
{
	u16 tmp = 0;
	u8 dat[3] = {0}, test = 0;
	u16 vol_val = 0;
	
	rn8209_read_data(RN8209_URMS_ADDR, dat, 3);
	tmp = dat[0];
	tmp = (tmp<<8) + dat[1];
		
	vol_val = (float)tmp * 0.0294918; 
	
	return vol_val;
}



u32 rn8209_get_curr(void)
{
	u8 dat[3] = {0}, test = 0;
	u32 curr_val = 0;
	
	rn8209_read_data(RN8209_IARMS_ADDR, dat, 3);
	
	curr_val = dat[0];
	curr_val = (curr_val<<8)+ dat[1];
	curr_val = (curr_val<<8) + dat[2];
	
	return curr_val;
}



float rn8209_get_curr1(void)
{
	u16 tmp = 0;
	u8 dat[3] = {0}, test = 0;
	float curr_val = 0;
	
	rn8209_read_data(RN8209_IARMS_ADDR, dat, 3);
	
	
	tmp = dat[0];
	tmp = (tmp<<8)+ dat[1];
	
	curr_val = (float)tmp * 0x000606;
	
	TX1_write2buff(0x11);
	test = tmp>>8;
	TX1_write2buff(test);
	test = tmp&0xff;
	TX1_write2buff(test);
	
	return curr_val;
}















