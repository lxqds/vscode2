#include <STC15F2K60S2.H>

#include "StopWatch.h"
#include "oled.h"
#include "MatrixKey.h"
#include "AT24C02.h"

sbit LED  = P4^0;
unsigned char StopWatch_Update = 4,StopWatch_State=0;
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

void Timer1DeInit()
{
	ET1 = 0;
}
void Timer1_Routine() interrupt 3 //1MS
{
//	static unsigned char  T2Counter1,T2Counter2;
	LED = 0;
	TL1 = 0x40;		//设置定时初始值
	TH1 = 0xA2;		//设置定时初始值
	if(StopWatch_State)
	{
		StopWatch.MS++;
	StopWatch_Update = 4;
	if(StopWatch.MS <1000)return;
	StopWatch.MS =0;
	StopWatch.Second++;
	StopWatch_Update =3;
	if(StopWatch.Second <60)return;
	StopWatch.Second =0;
	StopWatch.Minute++;
	StopWatch_Update =2;
	if(StopWatch.Minute <60)return;
	StopWatch.Minute=0;
	StopWatch.Hour++;
	StopWatch_Update =1;
	if(StopWatch.Hour <99)return;
	
	StopWatch.Hour = 0;
	}
	
}
void StopWatch_Start()
{
	Timer1Init();
}
void StopWatch_Continute()
{
	Timer1Init();
}
void StopWatch_Remeber(unsigned char i)
{
	
	AT24C02_WriteData(0x00+i,StopWatch.Hour);
  AT24C02_WriteData(0x01+i,StopWatch.Minute); 
	AT24C02_WriteData(0x02+i,StopWatch.Second); 
	AT24C02_WriteData(0x03+i,StopWatch.MS); 	
}
void StopWatch_Read(unsigned char i)
{
	StopWatch.Hour=AT24C02_ReadData(0x00+i);
	StopWatch.Minute = AT24C02_ReadData(0x01+i);
	StopWatch.Second = AT24C02_ReadData(0x02+i);
	StopWatch.MS = AT24C02_ReadData(0x03+i);
}
void StopWatch_Stop()
{
	Timer1DeInit();
}
void StopWatch_Display_Init()
{
	StopWatch.MS = 0;
	StopWatch.Second = 0;
	StopWatch.Minute =0;
	StopWatch.Hour = 0;
	OLED_Printf(32,4,"%bd:%bd:%bd:%bd",StopWatch.Hour,StopWatch.Minute,StopWatch.Second,StopWatch.MS);
}
void StopWatch_Display_DeInit()
{
	Timer1DeInit();
	OLED_Clear();
}
void StopWatch_Display_Update()
{
		OLED_Printf(32,4,"%bd:%bd:%bd:%bd",StopWatch.Hour,StopWatch.Minute,StopWatch.Second,StopWatch.MS);
}
void StopWatch_Function(Key_T Key)
{
	unsigned char Flag = 1;unsigned char Num1=0,Num2=0;
	StopWatch_Display_Init();
	switch(Key)
	{
		case KEY_ADD: 
		{
			StopWatch_State =~StopWatch_State;
//			Flag =~Flag;
//			switch (Flag)
//			{
//				case 0:StopWatch_Start();StopWatch_Display_Update();LED = ~LED ;break;
//				case 1:StopWatch_Stop();StopWatch_Display_Update();break;
//			}
		}break;
//		case KEY_SUB:
//		{
//			switch (Flag)
//			{
//				case 0:StopWatch_Remeber(Num1);Num1++;break;
//								
//				case 1:StopWatch_Read(Num2);if(Num2<Num1)Num2++;else Num2 = 0;
//							OLED_Printf(32,6,"%bd:%bd:%bd:%bd",StopWatch.Hour,StopWatch.Minute,StopWatch.Second,StopWatch.MS);
//							break;
//			}
//		}
//			break;
		case KEY_BACK:
			StopWatch_Display_DeInit();break;
	}
}