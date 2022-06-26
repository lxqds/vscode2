#include <STC15F2K60S2.H>
#include <stdio.h>
#include "Uart.h"
#include "MATRIXKEY.h"
#include "Change_Time.h"
#include "main.h"
static unsigned char Receive_Data;
Key_T ReKey;
/**
  * @brief  串口初始化，4800bps@12.000MHz
  * @param  无
  * @retval 无
  */
void Uart_Init(void)		//9600bps@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x8F;		//设置定时初始值
	TH1 = 0xFD;		//设置定时初始值
//	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时
	
	EA =1;
	ES = 1;
	Change_Time_Init();
}

/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
void Uart_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}
/**
  * @brief  发送字符串
	* @param  要发送的字符串
  * @retval 无
  */
void Uart1_SendString(char *s)
{
	while(*s)
	{
		Uart_SendByte(*s++);
	}
}
char putchar(char c)//重定向，需包括stdio
{
	Uart_SendByte((unsigned char)c);
	return c;
}
/**
  * @brief  串口接收中断
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
void UART1_Rountine(void) interrupt 4
{
	unsigned char Receive_Flag;

	EA = 0;
	
	if(RI == 1)
	{
		Receive_Flag = 1;
		RI = 0;
		Receive_Data = SBUF;
//		ReKey = Keyboard_Map_Key(Receive_Data);
//		Change_Time_Key(ReKey);
//		Change_Time_Display();

	  SBUF = Receive_Data;//将接收到的数据放入到发送寄存器
		if(TI)					//等待发送数据完成
		TI = 0;					    //清除发送完成标志位
		
	}
	EA =1;
}

/**
  * @brief  串口控制
  * @param  无
  * @retval 接受到的数据
  */
Key_T Uart_Control()
{
	if(Receive_Data)
	{
		ReKey = Keyboard_Map_Key(Receive_Data);
		Receive_Data = 0;
	}
	else
	{
		Receive_Data = 0;
		ReKey = Keyboard_Map_Key(Receive_Data);
	}
	Change_Time_Key(ReKey);
	Change_Time_Display();
	return ReKey;
}

bit Uart_Return_Time()
{
	if(Receive_Data)
	{
		return 1;
	}
	else return 0;
}