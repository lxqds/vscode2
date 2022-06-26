#include <STC15F2K60S2.H>

//LED
sbit LED = P4^0;

unsigned char Counter=0,Compare=0,Flag=0;	//计数值和比较值，用于输出PWM
unsigned int Time_Counter=0;

void Breath()
{
	Counter++;
	Counter%=100; //计数值变化范围限制在0~99
	Time_Counter++;
	Time_Counter%=200; //每20ms改变一次Compare的值
	if(Time_Counter==0)
	{
		if(Flag == 0)     // Flag仅为标志位，控制Compare的增或者减
		{
			Compare++;
			if(Compare == 100)
			{
				Flag = ~Flag; // 取反达到控制的目的
			}
		}
		else
		{
			Compare--;
			if(Compare == 0)
			{
				Flag = ~Flag;
			}
		}
		
	}
	
	if(Counter<Compare) //计数值小于比较值
	{
		LED = 1;
	}
	else
	{
		LED = 0;
	}
}