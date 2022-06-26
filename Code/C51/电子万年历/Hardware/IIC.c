#include <STC15F2K60S2.H>
#include "Delay.h"
sbit AT24C02_WP = P2^1;
sbit AT24C02_SCL = P2^0;
sbit AT24C02_SDA = P4^4;
sbit IIC_SDA = P4^4;
sbit IIC_SCL = P2^0;
/**
  * @brief  串口控制
  * @param  无
  * @retval 接受到的数据
  */
void IIC_Init(void)
{
	IIC_SCL = 1;
	Delay4us();
	IIC_SDA = 1;
	Delay4us();
}
/**
  * @brief  串口控制
  * @param  无
  * @retval 接受到的数据
  */
void IIC_Start(void)
{
	IIC_SDA = 1; 
	Delay5us();//确保sda高电平
	IIC_SCL = 1;
	Delay5us();//确保scl高电平
	IIC_SDA = 0; 
	Delay5us();//保持4.7us
	IIC_SCL = 0;
}
/**
  * @brief  串口控制
  * @param  无
  * @retval 接受到的数据
  */
void IIC_Stop(void)
{
	IIC_SCL = 0;
	IIC_SDA = 0;
	Delay4us();
	IIC_SCL = 1;
	Delay4us();//与sda保持4us时差
	IIC_SDA = 1;
	Delay5us();//维持4.7us	
}
void IIC_Ack(void)
{
	IIC_SCL = 0;
	IIC_SDA = 0;
	Delay2us();
	IIC_SCL = 1;
	Delay5us();
	IIC_SCL = 0;
}
void IIC_Not_Ack(void)
{
	IIC_SCL = 0;
	IIC_SDA = 1;
	Delay2us();
	IIC_SCL = 1;
	Delay5us();
	IIC_SCL = 0;
}
unsigned char IIC_ReceiveAck()
{
	unsigned char AckBit;
	IIC_SDA = 1;
	Delay5us();
	IIC_SCL = 1;
	Delay5us();
	AckBit = IIC_SDA;
	IIC_SCL = 0;
	return AckBit;
}
void IIC_SendByte(unsigned char SendByte)
{	
	unsigned char i;//	SDA_OUT
	IIC_SCL = 0;
	for (i = 0;i < 8;i++ )
	{
//		IIC_SDA = SendByte&(0x80>>i);
		if((SendByte & 0x80)>>7)
		{
			IIC_SDA = 1;
		}
		else
		{
			IIC_SDA = 0;
		}
		SendByte<<=1;
		Delay2us();
		IIC_SCL = 1;
		Delay2us();
		IIC_SCL = 0;
		Delay2us();
	}
	Delay(100);
}

unsigned char IIC_ReadByte(unsigned char Ack)//1 Ack;0 nor Ack
{
	unsigned char i,Receive = 0;//	SDA_IN
	for(i = 0;i < 8;i ++)
	{
		IIC_SCL = 0;
		Delay2us();
		IIC_SCL = 1;
		Receive <<=1;
		if(IIC_SDA == 1)
		{
			Receive++;
		}
		Delay1us();
	}
	if(!Ack)
		IIC_Not_Ack();
	else
		IIC_Ack();
	IIC_SCL = 0;
		Delay2us();
	return Receive;
}










