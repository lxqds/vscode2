#include "stm32f10x.h"                  // Device header

#include "stm32_dsp.h"
#include "table_fft.h"
#include "FFTADC.h"

#define NPT 256
uint32_t adc_buf[NPT]={0};			//���ڴ洢ADCת�����������	

static long lBufInArray[NPT];					//����FFT������

static long lBufOutArray[NPT];				//FFT��� 

long lBufMagArray[NPT];				//ÿ��Ƶ�ʶ��õķ�ֵ

uint8_t adc_dma_flag = 0;					//adc��DMA������ɱ�־


void FFT_Start(void)
{
	Adc1_Init();
}
/* �������ƣ�GetPowerMag
 * ������������ȡFFT���ֱ������
 * ��������
 * ����ֵ����
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
			
				//����32768�ٳ�65536��Ϊ�˷��ϸ������������
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
	//�������
	for(i=0;i<NPT;i++)
	{
		//������Ϊ��Ƭ����ADCֻ�ܲ����ĵ�ѹ ������Ҫǰ����ֱ��ƫִ
		//����ֱ��ƫִ�� 2.5V ��ӦADֵΪ3103
		lBufInArray[i] = ((signed short)(adc_buf[i])-3103) << 16;		
	}
	//256��FFT�任
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
	//�������ӦƵ�ʵ�ģ ��ÿ��Ƶ�ʶ�Ӧ�ķ�ֵ
	GetPowerMag();	
}
