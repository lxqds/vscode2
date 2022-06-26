#include <STC15F2K60S2.H>
#include "MATRIXKEY.h"
#include "Time0.h"
#include "Alarm.h"

Time_T StopWatch ={0,0,0,0,0,0,0,0};
Time_T Time = {2022,3,1,20,44,0};
Time_T Alarm = {2022,3,19,20,44,0};
unsigned char Time_Update = 1;
bit Change_500ms = 0;

/**
  * @brief  定时器0初始化，1000us@24.000MHz
  * @param  无
  * @retval 无
  */

void Timer0_Init()		//1000微秒@24.000MHz
{
	
  AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初始值
	TH0 = 0xA2;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;		//是否允许T0中断，1为允许
	EA = 1;			//CPU中断总开关
	PT0 = 0; //定时器中断优先级，0为最低，1为最高
}


void Timer0_Routine() interrupt 1 //1000us
{
	static unsigned int T1Count1,T1Count2;
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	Time.MS ++ ;
	T1Count1 ++;
	T1Count2 ++;
	if(T1Count1>=10)
	{
		T1Count1 = 0;
		MatrixKey_Loop();
	}
	if(T1Count2 >=500)
	{
		T1Count2 = 0;
		Change_500ms = ~Change_500ms;
	}
	if(Time.MS<1000)
	{
		return;
	}
	Time.MS = 00;
	Time.Second ++;
	Time_Update = 6;
	
  if(Time.Second < 60)
	{
		return;
	}
	Time.Second = 0;
	Time.Minute ++;
	Time_Update = 5;
	
	if(Time.Minute<60)
	{
		return;
	}
	Time.Minute = 0;
	Time.Hour ++;
	Time_Update = 4;
	
	if(Time.Hour < 24)
	{
		return;
	}
	Time.Hour = 0;
	Time.Day ++;
	Time_Update = 3;
	
	if((Time.Month == 2)&&((Time.Day == 30)||
												 (!(!(Time.Year %4) && ((Time.Year % 100)||!(Time.Year % 400)))&&
													(Time.Day == 29)))||
		 ((Time.Day == 32) &&((Time.Month == 1)||
													(Time.Month == 3)||
													(Time.Month == 5)||
													(Time.Month == 7)||
													(Time.Month == 8)||
													(Time.Month == 10)||
	                        (Time.Month == 12)))||
		 ((Time.Day == 31) && ((Time.Month == 4)||
													 (Time.Month == 6)||
													 (Time.Month == 9)||
													 (Time.Month == 11))))
	{
		Time.Day = 1;
	  Time.Month ++;
	  Time_Update = 2;
	}
	else
	{
		return;
	}
	
	if(Time.Month < 13)
	{
		return;
	}
	Time.Month = 1;
	Time.Year ++;
	Time_Update = 1;
	
	if(Time.Year > 99)
	{
		Time.Year = 00;
	}
}
/*定时器中断函数模板
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
		
	}
}
*/
