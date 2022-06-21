#include "stm32f10x.h"                  // Device header

#include "stm32_dsp.h"
#include "table_fft.h"
#include "FFTADC.h"

#define NPT 256
uint32_t adc_buf[NPT]={0};			//用于存储ADC转换结果的数组	

static long lBufInArray[NPT];					//传入FFT的数组

static long lBufOutArray[NPT];				//FFT输出 

long lBufMagArray[NPT];				//每个频率对用的幅值

uint8_t adc_dma_flag = 0;					//adc的DMA传输完成标志


void FFT_Start(void)
{
	Adc1_Init();
}
/* 函数名称：GetPowerMag
 * 功能描述：获取FFT后的直流分量
 * 参数：无
 * 返回值：无
 */
void GetPowerMag(void)
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
	
    for(i=0; i<NPT; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
			
				//除以32768再乘65536是为了符合浮点数计算规律
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y)*1.0/ NPT;
        if(i == 0)	
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}
void FFT_Pro(void)
{
	uint16_t i = 0;
	//填充数组
	for(i=0;i<NPT;i++)
	{
		//这里因为单片机的ADC只能测正的电压 所以需要前级加直流偏执
		//加入直流偏执后 2.5V 对应AD值为3103
		lBufInArray[i] = ((signed short)(adc_buf[i])-3103) << 16;		
	}
	//256点FFT变换
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
	//计算出对应频率的模 即每个频率对应的幅值
	GetPowerMag();	
}
