#ifndef __SPIOLED_H__
#define __SPIOLED_H__
#include "stm32f10x.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChineseWord(unsigned char x,unsigned char y,unsigned char* Str,unsigned char Flag);
//void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char Chr,unsigned char Flag);
void OLED_ShowText(unsigned char x,unsigned char y,unsigned char * Str,unsigned char Flag);
signed int OLED_Printf(unsigned char x,unsigned char y,const char *pFormat,...);
//void OLED_ShowChar(unsigned char  x,unsigned char y,unsigned char chr);

void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char Chr,unsigned char Flag);


void OLED_Refresh_Gram(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot); 
#endif
