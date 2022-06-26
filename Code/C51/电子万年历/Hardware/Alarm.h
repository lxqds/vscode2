#ifndef __ALARM_H__
#define __ALARM_H__
#include "Time0.h"
#include "oled.h"
#include "MATRIXKEY.h"

void Alarm_Init();
void Alarm_Key(unsigned char Key);
void Alarm_Display();
void Alarm_Check();
void Alarm_Ring();
void Alarm_Off();

extern Time_T Time;
extern Time_T Alarm;
extern bit Change_500ms;

#endif


