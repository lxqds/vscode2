#include <STC15F2K60S2.H>
#include "Alarm.h"
#include "oled.h"
#include "MATRIXKEY.h"

sbit BEEP = P0^4;

unsigned char Alarm_Change_Flag = 1;
unsigned char Alarm_Cursor_Position;
unsigned char Alarm_State;
/**
  * @brief  ���ӵĳ�ʼ��
  * @param  ��
  * @retval ��
  */
void Alarm_Init()
{
	if(Alarm_Change_Flag)
	{
		Alarm_Change_Flag = 0;//�״ν���ȡһ��ʱ��
		Alarm = Time;
	}
		OLED_ShowNum(8,0,20,2,16);
		OLED_ShowCHinese(40,0,7);//��
		OLED_ShowNum(56,0,Alarm.Month,2,16);
		OLED_ShowCHinese(72,0,8);//��
		OLED_ShowNum(88,0,Alarm.Day,2,16);
		OLED_ShowCHinese(104,0,9);//��
		OLED_ShowNum(16,2,Alarm.Hour,2,16);
		OLED_ShowCHinese(32,2,10);//ʱ
		OLED_ShowNum(48,2,Alarm.Minute,2,16);
		OLED_ShowCHinese(64,2,11);//��
		OLED_ShowNum(80,2,Alarm.Second,2,16);
		OLED_ShowCHinese(96,2,12);//��

//		OLED_ShowCHinese(40,4,13);//��
//		OLED_ShowCHinese(56,4,14);//��
//		OLED_ShowCHinese(72,4,Alarm.Week-1);//��
	
		Alarm_Cursor_Position = 1;
	
}
/**
  * @brief  ����ȥ��ʼ��
  * @param  ��
  * @retval ��
  */
void Alarm_Deinit()
{
		OLED_Clear();
}
/**
  * @brief  ����ֵ�����޸����ӵ�ʱ��
  * @param  ����ֵ
  * @retval ��
  */
void Alarm_Key(unsigned char Key)
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
				switch(Alarm_Cursor_Position)
				{
					case 1:Alarm.Year = (Key*10+Alarm.Year % 10);break;
					case 2:Alarm.Year = (Alarm.Year / 10)*10 + Key;break;
					case 3:Alarm.Month = (Key*10+Alarm.Month % 10);break;
					case 4:Alarm.Month = (Alarm.Month / 10)*10 + Key;break;
					case 5:Alarm.Day = (Key*10+Alarm.Day % 10);break;
					case 6:Alarm.Day = (Alarm.Day / 10)*10 + Key;break;
					case 7:Alarm.Hour = (Key*10+Alarm.Hour % 10);break;
					case 8:Alarm.Hour = (Alarm.Hour / 10)*10 + Key;break;
					case 9:Alarm.Minute = (Key*10+Alarm.Minute % 10);break;
					case 10:Alarm.Minute = (Alarm.Minute / 10)*10 + Key;break;
					case 11:Alarm.Second = (Key*10+Alarm.Second % 10);break;
					case 12:Alarm.Second = (Alarm.Second / 10)*10 + Key;break;
						
				}
			}break;
		case KEY_ENTER:
		{
			Alarm_Cursor_Position ++;
			if(Alarm_Cursor_Position == 13)
			{
				Alarm_Cursor_Position = 1;
				Alarm_State = 1;
			}
				
			
		}break;
						
		case KEY_BACK:
		{
			      Alarm_Deinit();
		}break;		
		default: break;	
		
	}
}
/**
  * @brief  ����ʱ����ʾ
  * @param  ��
  * @retval ��
  */
void Alarm_Display()
{
	OLED_ShowNum(24,0,Alarm.Year,2,16);
	  OLED_ShowNum(56,0,Alarm.Month,2,16);
	  OLED_ShowNum(88,0,Alarm.Day,2,16);
		OLED_ShowNum(16,2,Alarm.Hour,2,16);
		OLED_ShowNum(48,2,Alarm.Minute,2,16);
		OLED_ShowNum(80,2,Alarm.Second,2,16);
	OLED_ShowString(72,6,"Alarm");
	switch(Alarm_Cursor_Position)
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
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
void Alarm_Ring()
{
	if(Alarm_State)
	{
		if(Change_500ms)
		{
			BEEP = ~BEEP;
		}
	}
	
}
/**
  * @brief  ����ͣ
  * @param  ��
  * @retval ��
  */
void Alarm_Off()
{
	BEEP = 0;
  Alarm_State = 0;	
}	
/**
  * @brief  �������
  * @param  ��
  * @retval ��
  */
void Alarm_Check()
{
	if(Alarm.Hour == Time.Hour&&Alarm.Minute == Time.Minute)
	{
		Alarm_Ring();
	}
	else 
	{
		Alarm_Off();
	}
}
