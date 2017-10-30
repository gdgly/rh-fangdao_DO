

#include "spi.h"
#include "com.h"



void spi_write_byte(u8 ch)
{
	u8 data val = 0;
	u8 data i = 0;
	u8 data miso = 0;
	u8 k=0,j=0;

	for (i=0; i<8; i++)
	{
		SPI_SCLK_H();
		if (ch & 0x80)
		{
			SPI_MOSI_H();
		}
		else
		{
			SPI_MOSI_L();
		}
		
		ch <<= 1;
		SPI_SCLK_L();
	}
}

u8 spi_read_byte(void)
{
	u8 i = 0, ch = 0, tmp = 0;
	
	for (i=0; i<8; i++)
	{
		ch = ch<<1;
		SPI_SCLK_H();
		delay(6);
		SPI_SCLK_L();
		tmp = SPI_MISO();
		if (tmp)
		{
			ch |= 1;
		}
		else
		{
			ch |= 0;
		}
	}

	return ch;
}


u8 spi_send_byte1(u8 ch)
{
	u8 data val = 0;
	u8 data i = 0,j=0;
	u8 data miso = 0;

// 	for (i=0; i<8; i++)
// 	{
// 		SPI_SCLK_H();
// 		if (ch & 0x80)
// 		{
// 			SPI_MOSI_H();
// 		}
// 		else
// 		{
// 			SPI_MOSI_L();
// 		}
// 		
// 		ch <<= 1;
// 		SPI_SCLK_L();
// 		
// 		miso = SPI_MISO();
// 		val <<= 1;
// 		if (miso)
// 		{
// 			val++;
// 		}
// 		SPI_SCLK_H();
// 	}

	
	return val;
}


