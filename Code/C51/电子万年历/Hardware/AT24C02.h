#ifndef __AT24C02_H__
#define __AT24C02_H__

unsigned char AT24C02_WriteData(unsigned char Address,unsigned char Data);
unsigned char AT24C02_ReadData(unsigned char Address);
void AT24C02_Init();
#endif