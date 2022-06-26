#ifndef __SPIOLED_H__
#define __SPIOLED_H__

void OLED_Init();
void OLED_Clear();
void OLED_ShowChineseWord(unsigned char x,unsigned char y,unsigned char* Str,unsigned char Flag);
//void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char Chr,unsigned char Flag);
void OLED_ShowText(unsigned char x,unsigned char y,unsigned char * Str,unsigned char Flag);
signed int OLED_Printf(unsigned char x,unsigned char y,const char *pFormat,...);
//void OLED_ShowChar(unsigned char  x,unsigned char y,unsigned char chr);



#endif
