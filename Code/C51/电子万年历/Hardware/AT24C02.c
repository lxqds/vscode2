#include <STC15F2K60S2.H>
#include "Delay.h"
#include "IIC.h"
sbit AT24C02_WP = P2^1;
unsigned char AT24C02_WriteData(unsigned char Address,unsigned char Data)
{
	unsigned char t;
	AT24C02_WP = 0;
	IIC_Start();
	IIC_SendByte(0xAE);
	IIC_ReceiveAck();
	IIC_SendByte(Address);
	IIC_ReceiveAck();
	IIC_SendByte(Data);
	t =IIC_ReceiveAck();
	IIC_Stop();
	Delay(100);
	return  t;
}
//写和读延时5ms
unsigned char AT24C02_ReadData(unsigned char Address)
{
	unsigned char Temp;
	IIC_Start();
	IIC_SendByte(0xAE);
	IIC_ReceiveAck();
	IIC_SendByte(Address);
	IIC_ReceiveAck();
	IIC_Start();
	IIC_SendByte(0xAF);
	IIC_ReceiveAck();
	Temp = IIC_ReadByte(1);
	return Temp;
}
void AT24C02_Init()
{
	IIC_Init();
	AT24C02_WP = 0;
}