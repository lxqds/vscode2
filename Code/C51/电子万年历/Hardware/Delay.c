#include "Delay.h"
 
//延时子函数  24MHz
void Delay(unsigned int xms)
{
	unsigned char i, j;
	while(xms--)
	{
		i = 24;
		j = 85;
		do
		{
			while (--j);
		} while (--i);
	}
}
void Delay1us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 3;
	while (--i);
}

void Delay40us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 237;
	while (--i);
}
void Delay30us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 177;
	while (--i);
}
void Delay2us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 9;
	while (--i);
}
void Delay4us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 21;
	while (--i);
}
void Delay5us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}