#include <STC15F2K60S2.H>
#include <Delay.h>
#include "oled.h"
#include "DHT11.h"
sbit DHT11_DATA = P2^5;
//LED
sbit LED = P4^0;

//复位
void DHT_RST()
{
	DHT11_DATA = 0;
	Delay(20);
	DHT11_DATA =1;
	Delay30us();
}

/**
  * @brief  检测DHT是否存在
  * @param  无
  * @retval 1为不存在，0为存在
  */
unsigned char DHT_Check()
{
	unsigned char DHT_Flag = 0;
	while(DHT11_DATA && (DHT_Flag< 100))
	{
		DHT_Flag ++;
		Delay1us();
	};
	if(DHT_Flag >= 100) return 1;
	else DHT_Flag =0;
	while ((!DHT11_DATA)&&(DHT_Flag <100))
	{
		DHT_Flag ++;
		Delay1us();
	};
	if(DHT_Flag >= 100) return 1;
	return 0;
}

/**
  * @brief  从DHT1读取1个位
  * @param  无
  * @retval 返回0或1
  */
unsigned char DHT_Read_Bit()
{
	unsigned char DHT_Flag = 0;
	while(DHT11_DATA && (DHT_Flag< 100))
	{
		DHT_Flag ++;
		Delay1us();
	}
	DHT_Flag =0;
	while ((!DHT11_DATA)&&(DHT_Flag <100))
	{
		DHT_Flag ++;
		Delay1us();
	}
	Delay40us();
	if(DHT11_DATA)return 1;
	else return 0;
}
/**
  * @brief  从DHT1读取1个字节
  * @param  无
  * @retval 读到的数据
  */
unsigned char DHT_Read_Byte()
{
	unsigned char i,Dat;
	Dat = 0;
	for (i =0;i<8;i++)
	{
		Dat<<=1;
		Dat|=DHT_Read_Bit();
	}
	return Dat;
}
/**
  * @brief  从DHT读取一次数据
  * @param  温度值和湿度值
  * @retval 0为正常，1为失败
  */
unsigned char DHT_Read_Data(unsigned char *Temp,unsigned char *Humi)
{
	unsigned char Buff[5];
	unsigned char i;
	DHT11_DATA =1;
	DHT_RST();
	if(DHT_Check() == 0)
	{
		for (i = 0;i < 5;i++)
		{
				Buff[i]=DHT_Read_Byte();
		}
		DHT11_DATA =1;
//		OLED_ShowNum(48,0,Buff[0],2,16);
//		OLED_ShowNum(48,2,Buff[1],2,16);
//		OLED_ShowNum(48,4,Buff[2],2,16);
//		OLED_ShowNum(18,4,Buff[3],2,16);		
		if((Buff[0]+Buff[1]+Buff[2]+Buff[3])==Buff[4])
		{
			*Temp = Buff[2];
			*Humi = Buff[0];

		}
					
	}	
	else 
	{
		return 1;
	}
	
	return 0;
}
unsigned char DHT_Init()
{
	DHT_RST();
	return DHT_Check();
}

void DHT_Display()
{
	
	unsigned char T,H;
	LED = 1;
	if(Time_Update == 6)
	{
		DHT_Read_Data(&T,&H);
		OLED_ShowCHinese(56,6,15);//温
		OLED_ShowNum(72,6,T,2,16);
		OLED_ShowCHinese(96,6,16);//湿
		OLED_ShowNum(112,6,H,2,16);
	
	}
		
		
}