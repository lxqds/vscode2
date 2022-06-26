#ifndef _CLOCK_H__
#define _CLOCK_H__
#include "Time0.h"

void Clock_Display_Update();
void Clock_Display_Init();
void Clock_Time_To_DS1302();

extern unsigned char Time_Update ;
extern Time_T Time;
extern Time_T Alarm;
#endif