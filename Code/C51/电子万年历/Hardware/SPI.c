#include ".\Library\STC15F.H"
#include ".\Hardware\SPI.h"
#define CPOL 0
#define CPHA 0

sbit SPI_CLK = P1^5;
sbit SPI_MOSI = P1^3;
sbit SPI_CS = P1^1;//拉低有效
sbit SPI_MISO = P1^3;//记得改

void SPI_Init()
{
	SPI_CS = 0;
	SPI_MOSI = 1;
	#if CPOL==0
	SPI_CLK = 0;
	#else
	SPI_CLK =1 ;
	#endif
}
//模式0
#if CPOL==0&&CPHA==0

/**
  * @brief  模式0 写数据
  * @param  输入数据
  * @retval 无
  */

void SPI_SendByte(unsigned char Dat)
{
	unsigned char i;
	for(i = 0;i < 8;i++)
	{
		SPI_CLK = 0;
		if(Dat & 0x80)
			SPI_MOSI = 1;
		else
			SPI_MOSI = 0;
		Dat <<= 1;
		SPI_CLK = 1;
	}
	SPI_CLK = 0;
}

/**
  * @brief  模式0 读数据
  * @param  无
  * @retval 读取的数据
  */
unsigned char SPI_ReadByte()
{
	unsigned char i,Dat;
	
	for(i = 0;i < 8;i ++)
	{
		SPI_CLK = 0;
		Dat <<= 1;
		if(SPI_MISO)
			Dat |= 0x01;
		else 
			Dat &= 0xfe;
		SPI_CLK = 1;
	}
	SPI_CLK = 0;
	return Dat;
}
#endif

//模式1
#if CPOL==0&&CPHA==1

/**
  * @brief  模式1 写数据
  * @param  输入数据
  * @retval 无
  */
void SPI_SendByte(unsigned char Dat)
{
	unsigned char i;
	SPI_CLK = 0;
	for(i = 0;i < 8;i++)
	{
		SPI_CLK = 1;
		if(Dat & 0x80)
			SPI_MOSI = 1;
		else
			SPI_MOSI = 0;
		Dat <<= 1;
		SPI_CLK = 0;
	}
}

/**
  * @brief  模式1 读数据
  * @param  无
  * @retval 读取的数据
  */
unsigned char SPI_ReadByte()
{
	unsigned char i,Dat;
	
	for(i = 0;i < 8;i ++)
	{
		SPI_CLK = 1;
		Dat <<= 1;
		if(SPI_MISO)
			Dat |= 0x01;
		else 
			Dat &= 0xfe;
		SPI_CLK = 0;
	}
	SPI_CLK = 0;
	return Dat;
}
#endif


//模式2
#if CPOL==1&&CPHA==0

/**
  * @brief  模式2 写数据
  * @param  输入数据
  * @retval 无
  */
void SPI_SendByte(unsigned char Dat)
{
	unsigned char i;
	for(i = 0;i < 8;i++)
	{
		SPI_CLK = 1;
		if(Dat & 0x80)
			SPI_MOSI = 1;
		else
			SPI_MOSI = 0;
		Dat <<= 1;
		SPI_CLK = 0;
	}
	SPI_CLK = 1;
}

/**
  * @brief  模式2 读数据
  * @param  无
  * @retval 读取的数据
  */
unsigned char SPI_ReadByte()
{
	unsigned char i,Dat;
	
	for(i = 0;i < 8;i ++)
	{
		SPI_CLK = 1;
		Dat <<= 1;
		if(SPI_MISO)
			Dat |= 0x01;
		else 
			Dat &= 0xfe;
		SPI_CLK = 0;
	}
	SPI_CLK = 1;
	return Dat;
}
#endif
//模式3
#if CPOL==1&&CPHA==1

/**
  * @brief  模式3 写数据
  * @param  输入数据
  * @retval 无
  */
void SPI_SendByte(unsigned char Dat)
{
	unsigned char i;
	SPI_CLK = 1;
	for(i = 0;i < 8;i++)
	{
		SPI_CLK = 0;
		if(Dat & 0x80)
			SPI_MOSI = 1;
		else
			SPI_MOSI = 0;
		Dat <<= 1;
		SPI_CLK = 1;
	}
}

/**
  * @brief  模式3 读数据
  * @param  无
  * @retval 读取的数据
  */
unsigned char SPI_ReadByte()
{
	unsigned char i,Dat;
	
	SPI_CLK = 0;
	for(i = 0;i < 8;i ++)
	{
		SPI_CLK = 0;
		Dat <<= 1;
		if(SPI_MISO)
			Dat |= 0x01;
		else 
			Dat &= 0xfe;
		SPI_CLK = 1;
	}
	SPI_CLK = 1;
	return Dat;
}
#endif













