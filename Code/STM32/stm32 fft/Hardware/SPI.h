#ifndef __SPI_H__
#define __SPI_H__

void Spi_Init(void);
void SPI_SendByte(unsigned char Dat);
unsigned char SPI_ReadByte(void);

#endif
