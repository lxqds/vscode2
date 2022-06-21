#include "stm32f10x.h"
#define CPOL 0
#define CPHA 0

//sbit SPI_CLK = P1^5;
//sbit SPI_MOSI = P1^3;
//sbit SPI_CS = P1^1;//拉低有效
//sbit SPI_MISO = P1^3;//记得改
#define OLED_SPI_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)(x))
#define OLED_SPI_W_MOSI(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))
#define OLED_SPI_W_CS(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_14, (BitAction)(x))
#define OLED_SPI_R_MISO			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)

void Spi_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init (GPIOB,&GPIO_InitStructure);
	OLED_SPI_W_CS(0);
	OLED_SPI_W_MOSI(1);
	#if CPOL==0
	OLED_SPI_W_SCL(0);
	#else
	OLED_SPI_W_SCL(1) ;
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
		OLED_SPI_W_SCL(0);
		if(Dat & 0x80)
			OLED_SPI_W_MOSI(1);
		else
			OLED_SPI_W_MOSI(0);
		Dat <<= 1;
		OLED_SPI_W_SCL(1) ;
	}
	OLED_SPI_W_SCL(0);
}

/**
  * @brief  模式0 读数据
  * @param  无
  * @retval 读取的数据
  */
unsigned char SPI_ReadByte(void)
{
	unsigned char i,Dat;
	
	for(i = 0;i < 8;i ++)
	{
		OLED_SPI_W_SCL(0);
		Dat <<= 1;                                                                                                   
		if(OLED_SPI_R_MISO)
			Dat |= 0x01;
		else 
			Dat &= 0xfe;
		OLED_SPI_W_SCL(1) ;
	}
	OLED_SPI_W_SCL(0);
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
	OLED_SPI_W_SCL(0);
	for(i = 0;i < 8;i++)
	{
		OLED_SPI_W_SCL(1) ;
		if(Dat & 0x80)
			OLED_SPI_W_MOSI(1);
		else
			OLED_SPI_W_MOSI(0);
		Dat <<= 1;
		OLED_SPI_W_SCL(0);
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
		OLED_SPI_W_SCL(1) ;
		Dat <<= 1;
		if(OLED_SPI_R_MISO)
			Dat |= 0x01;
		else 
			Dat &= 0xfe;
		OLED_SPI_W_SCL(0);
	}
	OLED_SPI_W_SCL(0);
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
		OLED_SPI_W_SCL(1) ;
		if(Dat & 0x80)
			OLED_SPI_W_MOSI(1);
		else
			OLED_SPI_W_MOSI(0);
		Dat <<= 1;
		OLED_SPI_W_SCL(0);
	}
	OLED_SPI_W_SCL(1) ;
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
		OLED_SPI_W_SCL(1) ;
		Dat <<= 1;
		if(OLED_SPI_R_MISO)
			Dat |= 0x01;
		else 
			Dat &= 0xfe;
		OLED_SPI_W_SCL(0);
	}
	OLED_SPI_W_SCL(1) ;
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
	OLED_SPI_W_SCL(1) ;
	for(i = 0;i < 8;i++)
	{
		OLED_SPI_W_SCL(0);
		if(Dat & 0x80)
			OLED_SPI_W_MOSI(1);
		else
			OLED_SPI_W_MOSI(0);
		Dat <<= 1;
		OLED_SPI_W_SCL(1) ;
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
	
	OLED_SPI_W_SCL(0);
	for(i = 0;i < 8;i ++)
	{
		OLED_SPI_W_SCL(0);
		Dat <<= 1;
		if(OLED_SPI_R_MISO)
			Dat |= 0x01;
		else 
			Dat &= 0xfe;
		OLED_SPI_W_SCL(1) ;
	}
	OLED_SPI_W_SCL(1) ;
	return Dat;
}
#endif













