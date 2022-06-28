/*
 * @Description:
 * @Author: huacong
 * @Date: 2022-06-26 15:48:24
 * @LastEditTime: 2022-06-29 00:43:58
 * @LastEditors: huacong
 */
#include "main.h"
#include ".\Hardware\Delay.h" /* 意思是当前目录的Hardware目录下的Delay.h文件 */
#include ".\Hardware\MatrixKey.h"
#include ".\Hardware\SPIOled.h"
#include ".\Hardware\Timer1.h"
void main()
{
	unsigned char Key = 0, Next_State = 0, State = 0;
	OLED_Init();
	Timer1Init();
	while (1)
	{
		Key = MatrixKey();
		if (Key)
		{
			Key %= 15; /* 去除0的时候表示没有按下，同时16表示0 */
		}
		if (State != Next_State)
		{
			OLED_Clear(); /* 状态切换清屏 */
		}
		switch (State)
		{
		case 0:
			if (Key < 4)
			{
				Menu_Display(0);
			}

			else
			{
				Menu_Display(1);
			}
			Key_Change_State(&Key, &State);break;
		case 1:
		case 2:
		}
		if (Key)
		{
			LED = ~LED;
			OLED_Printf(1, 1, "%bd", Key);
		}
	}
}
/**
 * @description: 根据按键值改变状态值
 * @return {*}
 * @param {unsigned char} *Key
 * @param {unsigned char} *State
 */
void Key_Change_State(unsigned char *Key, unsigned char *State)
{
	switch (*Key)
	{
	case 1:
		*State = 1;
		break;
	case 2:
		*State = 2;
		break;
	case 3:
		*State = 3;
		break;
	case 4:
		*State = 4;
		break;
	case 5:
		*State = 5;
		break;
	case 6:
		*State = 6;
		break;
	default:
		break;
	}
}
/**
 * @description:显示菜单
 * @return {*}
 * @param {unsigned char} i
 */
void Menu_Display(unsigned char i)
{
	switch (i)
	{
	case 0:
		OLED_Printf(0, 32, "Menu");
		OLED_Printf(2, 0, "1.Change Time");
		OLED_Printf(4, 0, "2.Set Clock");
		OLED_Printf(6, 0, "3.StopWatch");
		break;
	case 1:
		OLED_Printf(0, 32, "Menu2");
		OLED_Printf(2, 0, "4.Change Time");
		OLED_Printf(4, 0, "5.Set Clock");
		OLED_Printf(6, 0, "6.StopWatch");
		break;
	}
}