#include "main.h"
#include ".\Hardware\Delay.h" /* 意思是当前目录的Hardware目录下的Delay.h文件 */
#include ".\Hardware\MatrixKey.h"
#include ".\Hardware\SPIOled.h"
#include ".\Hardware\Timer1.h"
void main()
{
	unsigned char Key = 0;
	OLED_Init();
	Timer1Init();
	while (1)
	{
		Key = MatrixKey();
		if (Key)
		{
			LED = ~LED;
			OLED_Printf(1, 1, "%bd", Key);
		}
		LED = 1;
		Delay(1000);
		LED = 0;
		Delay(1000);
	}
}
