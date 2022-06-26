#include ".\Library\STC15F.H"
#include ".\Hardware\MatrixKey.h"
//Keyboard
sbit ROW1 = P0^3;
sbit ROW2 = P0^2;
sbit ROW3 = P0^1;
sbit ROW4 = P0^0;
sbit LINE1 = P4^6;
sbit LINE2 = P4^5;
sbit LINE3 = P2^7;
sbit LINE4 = P2^6;

unsigned char Key_KeyNumber=0;

/**
  * @brief  获取按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0,1~16,0表示无按键按下
  */
unsigned char MatrixKey()
{
	unsigned char Temp=0;
	Temp=Key_KeyNumber;
	Key_KeyNumber=0;
	return Temp;
}
/**
  * @brief  获取当前按键的状态，无消抖及松手检测
  * @param  无
  * @retval 按下按键的键码，范围：0,1~16,0表示无按键按下
  */
unsigned char Key_GetState()
{
	unsigned char KeyNumber=0;
	ROW1=ROW2=ROW3=ROW4=1;
	ROW1=0;
	if(LINE1==0){KeyNumber=13;}
	if(LINE2==0){KeyNumber=14;}
	if(LINE3==0){KeyNumber=15;}
	if(LINE4==0){KeyNumber=16;}
	ROW1=ROW2=ROW3=ROW4=1;
	ROW2=0;
	if(LINE1==0){KeyNumber=9;}
	if(LINE2==0){KeyNumber=10;}
	if(LINE3==0){KeyNumber=11;}
	if(LINE4==0){KeyNumber=12;}
	ROW1=ROW2=ROW3=ROW4=1;
	ROW3=0;
	if(LINE1==0){KeyNumber=5;}
	if(LINE2==0){KeyNumber=6;}
	if(LINE3==0){KeyNumber=7;}
	if(LINE4==0){KeyNumber=8;}
	ROW1=ROW2=ROW3=ROW4=1;
	ROW4=0;
	if(LINE1==0){KeyNumber=1;}
	if(LINE2==0){KeyNumber=2;}
	if(LINE3==0){KeyNumber=3;}
	if(LINE4==0){KeyNumber=4;}
	

	return KeyNumber;
}
/**
  * @brief  按键驱动函数，在中断中调用
  * @param  无
  * @retval 无
  */
void MatrixKey_Loop(void)
{
	static unsigned char NowState,LastState;
	LastState=NowState;				//按键状态更新
	NowState=Key_GetState();		//获取当前按键状态
	//如果上个时间点按键按下，这个时间点未按下，则是松手瞬间，以此避免消抖和松手检测
	if(LastState==1 && NowState==0){Key_KeyNumber=1;}
	if(LastState==2 && NowState==0){Key_KeyNumber=2;}
	if(LastState==3 && NowState==0){Key_KeyNumber=3;}
	if(LastState==4 && NowState==0){Key_KeyNumber=4;}
	if(LastState==5 && NowState==0){Key_KeyNumber=5;}
	if(LastState==6 && NowState==0){Key_KeyNumber=6;}
	if(LastState==7 && NowState==0){Key_KeyNumber=7;}
	if(LastState==8 && NowState==0){Key_KeyNumber=8;}
	if(LastState==9 && NowState==0){Key_KeyNumber=9;}
	if(LastState==10 && NowState==0){Key_KeyNumber=10;}
	if(LastState==11 && NowState==0){Key_KeyNumber=11;}
	if(LastState==12 && NowState==0){Key_KeyNumber=12;}
	if(LastState==13 && NowState==0){Key_KeyNumber=13;}
	if(LastState==14 && NowState==0){Key_KeyNumber=14;}
	if(LastState==15 && NowState==0){Key_KeyNumber=15;}
	if(LastState==16 && NowState==0){Key_KeyNumber=16;}
} 