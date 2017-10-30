



#ifndef __SPI_H
#define __SPI_H
#include "config.h"




//spi
#define SPI_MISO()						P22	
#define SPI_SCLK_H() 					P21=1
#define SPI_SCLK_L()					P21=0
#define SPI_MOSI_H()					P23=1
#define SPI_MOSI_L()					P23=0					
#define SPI_CS_H()						P20=1
#define SPI_CS_L()						P20=0










void spi_write_byte(u8 ch);
u8 spi_read_byte(void);




#endif
