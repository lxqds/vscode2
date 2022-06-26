#ifndef __StopWatch_H__
#define __StopWatch_H__
#include "Time0.h"
#include "MatrixKey.h"
void StopWatch_Display_Init();
void StopWatch_Function(Key_T Key);
void StopWatch_Display_DeInit();
void StopWatch_Display_Update();
void Timer1Init(void);
extern Time_T StopWatch;

#endif