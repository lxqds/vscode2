#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

unsigned char Key_GetNum(void)
{
	unsigned char KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 1)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 1);
		Delay_ms(20);
		KeyNum = 1;
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 1)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 1);
		Delay_ms(20);
		KeyNum = 2;
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 1)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 1);
		Delay_ms(20);
		KeyNum = 3;
	}
	return KeyNum;
}
