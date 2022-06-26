#ifndef __UART_H__
#define __UART_H__
#include "MATRIXKEY.h"
#include "Time0.h"
Key_T Uart_Control();
void Uart_Init();
void Uart_SendByte(unsigned char Byte);
char putchar(char c);
void Uart1_SendString(char *s);
bit Uart_Return_Time();

extern Time_T Time;
#endif