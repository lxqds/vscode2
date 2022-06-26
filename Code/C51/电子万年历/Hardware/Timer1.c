#include ".\Library\STC15F.H"
#include ".\Hardware\MatrixKey.h"
void Timer1Init(void)		//1毫秒@24.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x40;		//设置定时初始值
	TH1 = 0xA2;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
	EA = 1;
}
void Timer1_Routine() interrupt 3 //1MS
{
	static unsigned int T1Count1,T1Count2;
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	T1Count1++;
	T1Count2++;
	if(T1Count1>=10)
	{
		T1Count1 = 0;
		MatrixKey_Loop();
	}	
	if(T1Count2>=500)
	{
		T1Count2 = 0;
	}	
}