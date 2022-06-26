#include ".\Library\STC15F.H"
#include ".\Hardware\SPI.h"
#include ".\Hardware\Delay.h"
#include <stdio.h>
#include <stdarg.h>
#include ".\Hardware\SPIOled.h"
#include ".\Hardware\OLEDWord.h"
sbit OLED_DC = P4^7;
sbit OLED_CS = P1^1;
sbit OLED_RES = P1^2;
	
//0 为命令 1为数据
void OLED_WriteByte(unsigned char Data,unsigned char Cmd)
{
	OLED_DC = Cmd;
	SPI_SendByte(Data);
	OLED_DC = 1;
}
void OLED_WriteCMD(unsigned char CMD)
{
	OLED_WriteByte(CMD,0);
}
void OLED_WriteData(unsigned char Data)
{
	OLED_WriteByte(Data,1);
}

void OLED_Clear()
{
	unsigned char i,n;
	for(i = 0;i < 8;i++)
	{
		OLED_WriteCMD(0xb0+i);
		OLED_WriteCMD(0x02);
		OLED_WriteCMD(0x10);
		for(n=0;n<128;n++)
		OLED_WriteData(0);
	}
}
//开启显示		
void OLED_Display_ON()
{
	OLED_WriteCMD(0x8D);
	OLED_WriteCMD(0x14);
	OLED_WriteCMD(0xAF);
}	
//关闭显示
void OLED_Display_OFF()
{
	OLED_WriteCMD(0x8D);
	OLED_WriteCMD(0x10);
	OLED_WriteCMD(0xAE);
}
//设置光标
void OLED_Set_Position(unsigned char x,unsigned char y)
{
	OLED_WriteCMD(0xb0+y);
	OLED_WriteCMD((((x+2)&0xf0)>>4)|0x10);
	OLED_WriteCMD((x+2)&0x0f);
}
//OLED 初始化
void OLED_Init()
{
	
	SPI_Init();
	OLED_CS = 0;
	OLED_RES = 1;
	Delay(100);
	OLED_RES = 0;
	Delay(100);
	OLED_RES = 1;
	
	OLED_WriteCMD(0xAE);
	OLED_WriteCMD(0x02);
	OLED_WriteCMD(0x10);
	OLED_WriteCMD(0x40);
	OLED_WriteCMD(0x81);
	OLED_WriteCMD(0xCF);
	OLED_WriteCMD(0xA1);
	OLED_WriteCMD(0xC8);
	OLED_WriteCMD(0xA6);
	OLED_WriteCMD(0xA8);
	OLED_WriteCMD(0x3F);
	OLED_WriteCMD(0xD3);
	OLED_WriteCMD(0x00);
	OLED_WriteCMD(0xD5);
	OLED_WriteCMD(0x80);
	OLED_WriteCMD(0xD9);
	OLED_WriteCMD(0xF1);
	OLED_WriteCMD(0xDA);
	OLED_WriteCMD(0x12);
	OLED_WriteCMD(0xDB);
	OLED_WriteCMD(0x40);
	OLED_WriteCMD(0x20);
	OLED_WriteCMD(0x02);
	OLED_WriteCMD(0x8D);
	OLED_WriteCMD(0x14);
	OLED_WriteCMD(0xA4);
	OLED_WriteCMD(0xA6);
	OLED_WriteCMD(0xAF);
	
	OLED_WriteCMD(0xAF);
	
	OLED_Clear();
	
	OLED_Set_Position(0,0);
}

void OLED_ShowChineseWord(unsigned char x,unsigned char y,unsigned char* Str,unsigned char Flag)
{
	unsigned char i = 0;
	unsigned int Index;
	for(Index = 0;Index < sizeof(Hzk)/35;Index ++)
	{
		if(Hzk[Index].Name[0] == Str[0]&&Hzk[Index].Name[1] == Str[1])
		{
			OLED_Set_Position(x,y);//设置OLED光标位置
			for(i= 0;i < 16;i ++)
			{
				if(Flag ==0) OLED_WriteData(Hzk[Index].Dat[i]);
				else OLED_WriteData(~Hzk[Index].Dat[i]);
			}
			OLED_Set_Position(x,y+1);
			for(i = 0;i< 16;i++)
			{
				if(Flag ==0) OLED_WriteData(Hzk[Index].Dat[i+16]);
				else OLED_WriteData(~Hzk[Index].Dat[i+16]);
			}
		}
	}
	
}

void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char Chr,unsigned char Flag)
{
	unsigned char c = 0,i=0;
	c = Chr-' ';//得到偏移后的值
	if(x > 127){x = 0;y=y+2;}
	OLED_Set_Position(x,y);
	for(i = 0;i< 8;i++)
	{
		if(Flag ==0) OLED_WriteData(F8X16[c*16+i]);
		else OLED_WriteData(~F8X16[c*16+i]);
	}
	OLED_Set_Position(x,y+1);
	for(i = 0;i< 8;i++)
	{
		if(Flag ==0) OLED_WriteData(F8X16[c*16+i+8]);
		else OLED_WriteData(~F8X16[c*16+i+8]);
	}
}

void OLED_ShowText(unsigned char x,unsigned char y,unsigned char * Str,unsigned char Flag)
{
	unsigned char TempStr[3] = {'\0'};
	while(*Str !='\0')
	{
		if(*Str&0x80)
		{
			TempStr[0]= *Str++;
			TempStr[1]= *Str++;
			OLED_ShowChineseWord(x,y,TempStr,Flag);
			x+=16;
			if(*Str &0x80)
			{
				if(x >= 112){y++;y++;x=0;}
			}
			else
			{
				if(x>=120){y++;y++;x=0;}
			}
		}
		else
		{
			OLED_ShowChar(x,y,*Str++,Flag);
			x+=8;
			if(*Str&0x80)
			{
				if(x >= 112){y++;y++;x=0;}
			}
			else
			{
				if(x>=120){y++;y++;x=0;}
			}
		}
	}
}

signed int OLED_Printf(unsigned char x,unsigned char y,const char *pFormat,...)
	
{
	char pStr[25] = {'\0'};
	va_list ap;
	signed int result;
	
	//Forward call tp vprintf
	va_start(ap,pFormat);
	result = vsprintf((char *)pStr,pFormat,ap);
	va_end(ap);
	
	OLED_ShowText(x,y,(const unsigned char *)pStr,0);
	
	return result;
}