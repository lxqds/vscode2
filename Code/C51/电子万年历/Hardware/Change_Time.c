#include <STC15F2K60S2.H>
#include "DS1302.h"
#include "oled.h"
#include "Clock.h"
#include "MATRIXKEY.h"
//#include "Uart.h"
unsigned char Cursor_Position;
void Change_Time_Init()//初始化
{
		OLED_ShowNum(8,0,20,2,16);
		OLED_ShowNum(24,0,Time.Year,2,16);
		OLED_ShowCHinese(40,0,7);//年
		OLED_ShowNum(56,0,Time.Month,2,16);
		OLED_ShowCHinese(72,0,8);//月
		OLED_ShowNum(88,0,Time.Day,2,16);
		OLED_ShowCHinese(104,0,9);//日
		OLED_ShowNum(16,2,Time.Hour,2,16);
		OLED_ShowCHinese(32,2,10);//时
		OLED_ShowNum(48,2,Time.Minute,2,16);
		OLED_ShowCHinese(64,2,11);//分
		OLED_ShowNum(80,2,Time.Second,2,16);
		OLED_ShowCHinese(96,2,12);//秒

		OLED_ShowCHinese(40,4,13);//星
		OLED_ShowCHinese(56,4,14);//期
		OLED_ShowCHinese(72,4,Time.Week);//天
	
		Cursor_Position = 1;
}
void Change_Time_Deinit()//去初始化
{
	OLED_Clear();
	Clock_Time_To_DS1302();
}
/**
  * @brief  //根据按键的输入修改时间
  * @param  按键值
  * @retval 无
  */
void Change_Time_Key(Key_T Key)
{
	
	switch(Key)
	{
		case KEY_0:
		case KEY_1:
		case KEY_2:
		case KEY_3:
		case KEY_4:
		case KEY_5:
		case KEY_6:
		case KEY_7:
		case KEY_8: 
		case KEY_9:
			{
				switch(Cursor_Position)
				{
					case 1:Time.Year = (Key*10+Time.Year % 10);break;
					case 2:Time.Year = (Time.Year / 10)*10 + Key;break;
					case 3:Time.Month = (Key*10+Time.Month % 10);break;
					case 4:Time.Month = (Time.Month / 10)*10 + Key;break;
					case 5:Time.Day = (Key*10+Time.Day % 10);break;
					case 6:Time.Day = (Time.Day / 10)*10 + Key;break;
					case 7:Time.Hour = (Key*10+Time.Hour % 10);break;
					case 8:Time.Hour = (Time.Hour / 10)*10 + Key;break;
					case 9:Time.Minute = (Key*10+Time.Minute % 10);break;
					case 10:Time.Minute = (Time.Minute / 10)*10 + Key;break;
					case 11:Time.Second = (Key*10+Time.Second % 10);break;
					case 12:Time.Second = (Time.Second / 10)*10 + Key;break;
						
				}
			}break;
		case KEY_ENTER:
		{
			Cursor_Position ++;
			if(Cursor_Position == 13)
				Cursor_Position = 1;
			
		}break;
		case KEY_BACK:
		{
			      Change_Time_Deinit();
		}break;			
						
		default: break;	
		
	}
}
/**
  * @brief  将修改的时间显示出来
  * @param  无
  * @retval 无
  */
void Change_Time_Display()
{
	  OLED_ShowNum(24,0,Time.Year,2,16);
	  OLED_ShowNum(56,0,Time.Month,2,16);
	  OLED_ShowNum(88,0,Time.Day,2,16);
		OLED_ShowNum(16,2,Time.Hour,2,16);
		OLED_ShowNum(48,2,Time.Minute,2,16);
		OLED_ShowNum(80,2,Time.Second,2,16);
	OLED_ShowString(72,6,"SetTime");
	switch(Cursor_Position)
	{
		case 1:OLED_ShowString(24,0,"_");break;
		case 2:OLED_ShowString(32,0,"_");break;
		case 3:OLED_ShowString(56,0,"_");break;
		case 4:OLED_ShowString(64,0,"_");break;
		case 5:OLED_ShowString(88,0,"_");break;
		case 6:OLED_ShowString(96,0,"_");break;
		case 7:OLED_ShowString(16,2,"_");break;
		case 8:OLED_ShowString(24,2,"_");break;
		case 9:OLED_ShowString(48,2,"_");break;
		case 10:OLED_ShowString(56,2,"_");break;
		case 11:OLED_ShowString(80,2,"_");break;
		case 12:OLED_ShowString(88,2,"_");break;	
	}
	 
}