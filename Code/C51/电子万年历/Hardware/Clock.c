#include <STC15F2K60S2.H>
#include "oled.h"
#include "DS1302.h"
#include "Timer1.h"
#include "Time0.h"
#include "MATRIXKEY.h"
#include "Clock.h"
#include "DHT11.h"

unsigned char Clock_Week_Calc(unsigned int y, unsigned char m, unsigned char d)    // 星期计算
{
    unsigned int Week_Cnt = 0;
    unsigned char Week_Num;

    switch (m) {     // 从这一年的元旦算到该天为止( 含该天) 的天数
        case 12: Week_Cnt += 30;
        case 11: Week_Cnt += 31;
        case 10: Week_Cnt += 30;
        case  9: Week_Cnt += 31;
        case  8: Week_Cnt += 31;
        case  7: Week_Cnt += 30;
        case  6: Week_Cnt += 31;
        case  5: Week_Cnt += 30;
        case  4: Week_Cnt += 31;
        case  3: Week_Cnt += 28;
        case  2: Week_Cnt += 31;
        case  1: Week_Cnt += d; break;
        default: break;
    }
    if ((!(y % 4) && ((y % 100) || !(y % 400))) && (m > 2)) // 闰年补一天
		{    
        Week_Cnt++;
    }
    Week_Num = (y - 1 + (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400 + Week_Cnt) % 7;

    return Week_Num;
}
void Clock_Time_To_DS1302()//将时间写入DS1302
{
	  DS1302_Time[0] = Time.Year;
		DS1302_Time[1] = Time.Month;
		DS1302_Time[2] = Time.Day;
		DS1302_Time[3] = Time.Hour;
		DS1302_Time[4] = Time.Minute;
		DS1302_Time[5] = Time.Second;
//		DS1302_Time[6] = Time.Week;
	  DS1302_SetTime();
}
void Clock_Display_Init()//时间初始化
{
		DS1302_ReadTime();
		Time.Year = DS1302_Time[0];
		Time.Month = DS1302_Time[1];
		Time.Day = DS1302_Time[2];
		Time.Hour = DS1302_Time[3];
		Time.Minute = DS1302_Time[4];
		Time.Second = DS1302_Time[5];
		Time.Week = Clock_Week_Calc(Time.Year,Time.Month,Time.Day);
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
}	


void Clock_Display_Update()
{
	switch(Time_Update)//判断更新
	{
		case 1:	Time_Update = 0;
						OLED_ShowNum(24,0,Time.Year,2,16);//更新年
		case 2:	Time_Update = 0;
						OLED_ShowNum(56,0,Time.Month,2,16);//月
		case 3:	Time_Update = 0;
						OLED_ShowNum(88,0,Time.Day,2,16);//日
						OLED_ShowCHinese(72,4,Clock_Week_Calc(Time.Year,Time.Month,Time.Day));//天
		case 4:	Time_Update = 0;
						OLED_ShowNum(16,2,Time.Hour,2,16);//时
		case 5:	Time_Update = 0;
						OLED_ShowNum(48,2,Time.Minute,2,16);//分
		case 6:	Time_Update = 0;
						OLED_ShowNum(80,2,Time.Second,2,16);//秒
						break;
						
		default:break;
		
	}
}
