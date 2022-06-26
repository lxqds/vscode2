#ifndef __MAIN_H__
#define __MAIN_H__
#include "MATRIXKEY.h"
typedef enum 
{
	Fun_Clock = 0,
	Fun_Alarm,
	Fun_DHT,
	Fun_Change_Time,
	Fun_Uart,
	Fun_Butt
} Fun_T;
void Response_Key(Key_T Key);  // 按键响应
void Fun_Select(Fun_T Fun);  // 功能选择
void Uart_ReturnTime();
#endif