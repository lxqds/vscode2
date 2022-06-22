#include "stm32f10x.h"
#include "SPIOled.h"
#include "stdio.h"
#include "Key.h"
#include "FFTADC.h"

extern volatile uint16_t ADC1_ConvertedValue;			//ADC采样的数据

unsigned char CD4066Control(unsigned char s);
unsigned char CD4051Control(unsigned char i);
int main()
{
	unsigned char a=0,K,State=0,Last_State=0,t;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init (GPIOA,&GPIO_InitStructure);
	GPIO_SetBits (GPIOA,GPIO_Pin_1);
	GPIO_ResetBits (GPIOA,GPIO_Pin_1);
	OLED_Init();
	Key_Init();
	while(1)
	{
		K=Key_GetNum();
		a = CD4066Control(3);
		if(Last_State !=State)
		{
				OLED_Clear();
				State = Last_State ;
		}

		switch(State)
		{
			case 0:
			{
					if(K)
					{
						Last_State=K;
					}
					OLED_Printf(32,0,"Menu");
					OLED_Printf(8,2,"K1:FFT");
					OLED_Printf(8,4,"K2:Volume");
					OLED_Printf(8,6,"K3:Source");
			}break;
			case 1:
			{
				FFT();
//				OLED_Printf(32,0,"FFT");
//				OLED_Printf(0,6,"K1:- K2:+ K3:<-");
				switch(K)
				{
					case 1:break;
					case 2:break;
					case 3:Last_State = 0;break;
				}
			}	break;	
			case 2:
			{
				switch(K)
				{
					case 1:t=CD4051Control(0);break;
					case 2:t=CD4051Control(1);break;
					case 3:Last_State = 0;break;
				}
				OLED_Printf(32,0,"Volume");
				OLED_Printf(0,4,"Volume:%d",t);
				OLED_Printf(0,6,"K1:- K2:+ K3:<-");
			}break;
			case 3:
			{
				switch(K)
				{
					case 1:t=CD4066Control(0);break;
					case 2:t=CD4066Control(1);break;
					case 3:Last_State = 0;break;
				}
				OLED_Printf(32,0,"Source");
				OLED_Printf(0,4,"Source:%d",t);
				OLED_Printf(0,6,"K1:- K2:+ K3:<-");
			}break;
//			case 4:
		}
	}
}
/**
  * @brief  CD4051控制
  * @param  1，音量++，0，音量--
  * @retval 当前音量等级
  */
unsigned char CD4051Control(unsigned char i)
{
	static unsigned char t=4;
	unsigned char a,b,c;
	if(i>0)
	{
		t ++;
	}
	if(i==0)
	{
		t --;
	}
	
	t%=8;
	c = t/4;
	b = (t%4)/2;
	a = (t%4%2)/1;
	GPIO_WriteBit(GPIOA, GPIO_Pin_6,(BitAction)(a));
	GPIO_WriteBit(GPIOA, GPIO_Pin_7,(BitAction)(b));
	GPIO_WriteBit(GPIOA, GPIO_Pin_8,(BitAction)(c));
  OLED_Printf(0,2,"c=%d b=%d a=%d",c,b,a);	
	return t;
}
/**
  * @brief  CD4066控制
  * @param  1，音源++，0，音源--
  * @retval 当前音源
  */
unsigned char CD4066Control(unsigned char s)
{
	static unsigned char i=1;
	switch (s)
	{
		case 0:i--;break;
		case 1:i++;break;
	}
	i%=4;
	switch(i)
	{
		case 0:GPIO_SetBits (GPIOA,GPIO_Pin_2);
						GPIO_ResetBits (GPIOA,GPIO_Pin_3);
						GPIO_ResetBits (GPIOA,GPIO_Pin_4);
						GPIO_ResetBits (GPIOA,GPIO_Pin_5);break;
		case 1:GPIO_SetBits (GPIOA,GPIO_Pin_3);
						GPIO_ResetBits (GPIOA,GPIO_Pin_2);
						GPIO_ResetBits (GPIOA,GPIO_Pin_4);
						GPIO_ResetBits (GPIOA,GPIO_Pin_5);break;
		case 2:GPIO_SetBits (GPIOA,GPIO_Pin_4);
						GPIO_ResetBits (GPIOA,GPIO_Pin_2);
						GPIO_ResetBits (GPIOA,GPIO_Pin_3);
						GPIO_ResetBits (GPIOA,GPIO_Pin_5);break;
		case 3:GPIO_SetBits (GPIOA,GPIO_Pin_5);
						GPIO_ResetBits (GPIOA,GPIO_Pin_2);
						GPIO_ResetBits (GPIOA,GPIO_Pin_3);
						GPIO_ResetBits (GPIOA,GPIO_Pin_4);break;
	}
	return i+1;
}
