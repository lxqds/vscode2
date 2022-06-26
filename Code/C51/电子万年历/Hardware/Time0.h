#ifndef __TIMER0_H__
#define __TIMER0_H__

typedef struct {     // 电子万年历变量
    unsigned char Year;
    unsigned char Month;
    unsigned char Day;
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
	  unsigned char Week;
    unsigned int  MS;
} Time_T;

void Timer0_Init();
void Delay(unsigned int xms);

#endif
