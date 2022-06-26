#include <STC15F2K60S2.H>
#include "main.h"
#include "Time0.h"
#include "oled.h"
#include "Clock.h"
#include "DS1302.h"
#include "Change_Time.h"
//#include "Breath.h"
//#include "Timer1.h"
#include "MATRIXKEY.h"
#include "Alarm.h"
#include "DHT11.h"
#include "Uart.h"
#include <stdio.h>
#include <stdarg.h>
#include "IIC.h"
#include "AT24C02.h"
#include "StopWatch.h"

//LED
sbit LED = P4^0;
//Beep
sbit BEEP = P0^4;
//Keyboard
sbit ROW1 = P0^3;
sbit ROW2 = P0^2;
sbit ROW3 = P0^1;
sbit ROW4 = P0^0;
sbit LINE1 = P4^6;
sbit LINE2 = P4^5;
sbit LINE3 = P2^7;
sbit LINE4 = P2^6;
//DTH11
sbit DHT11_DATA = P2^5;
//DS1302
sbit DS1302_SCLK = P2^4;
sbit DS1302_IO = P2^3;
sbit DS1302_CE = P2^2;
//AT24C02  地址111
sbit AT24C02_WP = P2^1;
sbit AT24C02_SCL = P2^0;
sbit AT24C02_SDA = P4^4;
//NRF24L01
sbit NRF24L01_IRQ = P4^3;
sbit NRF24L01_CSN = P4^2;
sbit NRF24L01_SCK = P4^1;
sbit NRF24L01_CE = P3^7;
sbit NRF24L01_MOSI = P1^3;
sbit NRF24L01_MISO = P1^1;
//OLED
//sbit OLED_CS = P1^1;
//sbit OLED_DC = P4^7;
//sbit OLED_RES = P1^2;
//sbit OLED_MOSI = P1^3;
//sbit OLED_CLK = P1^5;

Key_T Key = KEY_BUTT;  // 按键值
Fun_T Fun = Fun_Clock;  // 功能判断
unsigned Fun_Change = 1;  // 功能改变，0无效，other有效
unsigned char T_H[5];
unsigned char KeyNum,State;

/**
	* @brief  初始化蜂鸣器接口为推挽模式
  * @param  无
  * @retval 无
  */
void BEEP_Init()
{
	P0M1 &= 0x00;
	P0M0 |= 0x10;
	BEEP = 0;
}
void main() 
{
	unsigned char State = 0,Temp =1;
	
	BEEP_Init();
  OLED_Init();
	Timer0_Init();
	DS1302_Init();
	Clock_Display_Init();
	Alarm_Off();
	Uart_Init();
	AT24C02_Init();

	while(1)
	{ 
		Key = Keyboard_Map_Key(MatrixKey());
		if(Uart_Return_Time())
			Uart_ReturnTime();
		if(Key < 16)
		{
			OLED_Printf(0,6,"k%bdF%bd",Key,Fun);
		}
		
		Alarm_Check();

		Fun_Select(Fun);
		Response_Key(Key);
  }
}
/**
  * @brief  将键值与功能对应
  * @param  按键值
  * @retval 无
  */
void Response_Key(Key_T Key)
{
	switch (Key) {
        case KEY_FUN: 
					{  
            Fun++;// 加1
						OLED_Clear();
            Fun_Change = 1;
            if (Fun >= Fun_Butt) 
						{
                Fun = Fun_Clock;
            }
            break;
         }
        case KEY_BACK: 
					{
						OLED_Clear();
            Fun_Change = 1;
            Fun = Fun_Clock;
            break;
          }
        default: break;
    }
}
 /**
  * @brief  功能选择
  * @param  功能值
  * @retval 无
  */
void Fun_Select(Fun_T fun)
{
	switch (fun)
	{
		case Fun_Clock:
		{
			if(Fun_Change)
			{
				Fun_Change = 0;
				Time_Update = 1;
				Clock_Display_Init();
			}
				DHT_Display();//温湿度显示
			Clock_Display_Update();
			break;
		}
		case Fun_Change_Time:
		{
			if(Fun_Change)
			{
				Fun_Change = 0;
				Change_Time_Init();
			}	
			Change_Time_Key(Key);
			Change_Time_Display();
			
			break;
		}
		case Fun_Alarm:
		{
			if(Fun_Change)
			{
				Fun_Change = 0;
				Alarm_Init();
			}	
			Alarm_Key(Key);
			Alarm_Display();
			
			break;
		}
		case Fun_DHT:
		{
			if(Fun_Change)
			{
				Fun_Change = 0;
			}
			OLED_Printf(0,0,"AT24C02Write:%bd ",AT24C02_WriteData(0x00,1));
			OLED_Printf(0,2,"AT24C02Read:%bd",AT24C02_ReadData(0x00));
			break;
		}
		case Fun_Uart:
		{
			if(Fun_Change)
			{
				Fun_Change = 0;
				Uart_Init();
			}
			OLED_Printf(0,6,"UART");
			Response_Key(Uart_Control());
			printf("Year:%bd Month:%bd Day:%bd Hour:%bd Minute:%bd Second:%bd",Time.Year,Time.Month,Time.Day,Time.Hour,Time.Minute,Time.Second);
			break;
		}	
		default: break;
	}
}

void Uart_ReturnTime()
{
	printf("Year:%bd Month:%bd Day:%bd Hour:%bd Minute:%bd Second:%bd",Time.Year,Time.Month,Time.Day,Time.Hour,Time.Minute,Time.Second);
}