#ifndef _DS1302_H__
#define _DS1302_H__

void DS1302_Init(void);
void DS1302_WriteByte(unsigned char Command,Data);
unsigned char DS1302_ReadByte(unsigned char Command);
extern unsigned char DS1302_Time[7];
void DS1302_ReadTime();
void DS1302_SetTime();

#endif