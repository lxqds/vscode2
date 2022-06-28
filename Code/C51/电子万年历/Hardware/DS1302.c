#include ".\Library\STC15F.H"

sbit DS1302_SCLK = P2^4;
sbit DS1302_IO = P2^3;
sbit DS1302_CE = P2^2;
//定义地址，分别是秒，分钟
#define DS1302_SECOND 		0x80	//秒
#define DS1302_MINUTE 		0x82	//分钟
#define DS1302_HOUR 		  0x84	//小时	
#define DS1302_DATE 			0x86	//日
#define DS1302_MONTH 			0x88	//月
#define DS1302_DAY 				0x8A	//星期
#define DS1302_YEAR 		  0x8C	//年
#define DS1302_WP 				0x8E	//写保护

unsigned char DS1302_Time[7]={22,1,16,23,18,0,7};   //设置当前时间

/**
  * @brief  初始化
  * @param  无
  * @retval  无！！
  */
void DS1302_Init(void)
{
	DS1302_CE = 0;
	DS1302_SCLK = 0;
}
/**
  * @brief  写入数据
  * @param  写入的命令
	* @param	写入的数据
  * @retval  无
  */
void DS1302_WriteByte(unsigned char Command, Data)
{
	unsigned char i;
	DS1302_CE = 1;
	
	for (i = 0;i < 8; i++)
	{
		DS1302_IO = Command&(0x01<<i);
		DS1302_SCLK = 1;
	//	Delay(); 51太慢不用
		DS1302_SCLK = 0;
	}
	
	for (i = 0;i < 8; i++)
	{
		DS1302_IO = Data&(0x01<<i);
		DS1302_SCLK = 1;
		//	Delay(); 51太慢不用
		DS1302_SCLK = 0;
	}
	DS1302_CE = 0;
}

/**
  * @brief  读取数据
  * @param  写入命令
  * @retval 读出的数据
  */
unsigned char DS1302_ReadByte(unsigned char Command)	
{
	unsigned char i,Data = 0x00;
	Command |=0x01;
	DS1302_CE = 1;
	
	for (i = 0;i < 8; i++)
	{
		DS1302_IO = Command&(0x01<<i);
		DS1302_SCLK = 0;
		//	Delay(); 51太慢不用
		DS1302_SCLK = 1;
		
	}
	for(i=0;i<8;i++)
	{
		DS1302_SCLK = 1;
		//	Delay(); 51太慢不用
		DS1302_SCLK = 0;
		if(DS1302_IO)
		{
			Data |= (0x01<<i)	;
		}
	}
	DS1302_CE = 0;
	DS1302_IO = 0;
	return Data;
}

/**
  * @brief  向ds1302设置时间，DS1302_Time数组已定义
  * @param  无
  * @retval  无
  */
void DS1302_SetTime()
{
	DS1302_WriteByte(DS1302_WP,0x00);
	DS1302_WriteByte(DS1302_YEAR,DS1302_Time[0]/10*16+DS1302_Time[0]%10);
	DS1302_WriteByte(DS1302_MONTH,DS1302_Time[1]/10*16+DS1302_Time[1]%10);
	DS1302_WriteByte(DS1302_DATE,DS1302_Time[2]/10*16+DS1302_Time[2]%10);
	DS1302_WriteByte(DS1302_HOUR,DS1302_Time[3]/10*16+DS1302_Time[3]%10);
	DS1302_WriteByte(DS1302_MINUTE,DS1302_Time[4]/10*16+DS1302_Time[4]%10);
	DS1302_WriteByte(DS1302_SECOND,DS1302_Time[5]/10*16+DS1302_Time[5]%10);
	DS1302_WriteByte(DS1302_DAY,DS1302_Time[6]/10*16+DS1302_Time[6]%10);
	DS1302_WriteByte(DS1302_WP,0x80);
}

/**
  * @brief  读取ds1302的时间
  * @param  无
  * @retval  无
  */
void DS1302_ReadTime()
{
	unsigned char Temp;
	Temp = DS1302_ReadByte(DS1302_YEAR);
	DS1302_Time[0] = Temp/16*10+Temp%16;
	Temp = DS1302_ReadByte(DS1302_MONTH);
	DS1302_Time[1] = Temp/16*10+Temp%16;
	Temp = DS1302_ReadByte(DS1302_DATE);
	DS1302_Time[2] = Temp/16*10+Temp%16;
	Temp = DS1302_ReadByte(DS1302_HOUR);
	DS1302_Time[3] = Temp/16*10+Temp%16;
	Temp = DS1302_ReadByte(DS1302_MINUTE);
	DS1302_Time[4] = Temp/16*10+Temp%16;
	Temp = DS1302_ReadByte(DS1302_SECOND);
	DS1302_Time[5] = Temp/16*10+Temp%16;
	Temp = DS1302_ReadByte(DS1302_DAY);
	DS1302_Time[6] = Temp/16*10+Temp%16;

}