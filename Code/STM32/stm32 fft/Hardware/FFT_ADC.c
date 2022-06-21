#include "stm32f10x_adc.h"
#include "stm32f10x.h"                  // Device header
#include "FFTADC.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include "math.h"
#include "SPIOled.h"
#include "Delay.h"
//volatile uint16_t ADC1_ConvertedValue;			//ADC����������
#define ADC1_DR_Address    ((u32)0x4001244C)		//ADC1�ĵ�ַ

#define NPT 256

uint32_t adc_buf[NPT]={0};			//���ڴ洢ADCת�����������	

static long lBufInArray[NPT];					//����FFT������

static long lBufOutArray[NPT];				//FFT��� 

long lBufMagArray[NPT];				//ÿ��Ƶ�ʶ��õķ�ֵ

static long BandValues[32]={0};
static int OldHeight[32]={0};
static int Height[32]={0}; 
static uint8_t adc_dma_flag = 0;					//adc��DMA������ɱ�־

void FFT_Pro(void);
void display2(void);
uint8_t fal_pot[NPT];							// ��¼����������
uint8_t prt = 4;									// ������ʾ�ı���
uint8_t display_num = 1;					// ��ʾ��ʽ������
uint8_t fal_speed = 7;						// �����ٶ�
uint8_t display_auto = 1;					// �Զ���ʾ��־λ

//TIM2���ã�arrΪ�ؼ���ֵ��pscΪԤ��Ƶϵ��
void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 		//ʱ��ʹ��

	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		//�������:TIM����Ƚϼ��Ե�
	TIM_OC2Init(TIM2, & TIM_OCInitStructure);		//��ʼ������TIM2_CH2
	
	TIM_Cmd(TIM2, ENABLE); 			//ʹ��TIMx
	TIM_CtrlPWMOutputs(TIM2, ENABLE); 
}

//DMA1����
void DMA1_Init()
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	  			//ʹ��ADC1ͨ��ʱ��
	
	//DMA1��ʼ��
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;				//ADC1��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_buf; 		//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 				//����(�����赽�ڴ�)
	DMA_InitStructure.DMA_BufferSize = NPT; 						//�������ݵĴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 		//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 			//�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ; //�������ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;    //�ڴ����ݵ�λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular  ; 		//DMAģʽ��ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ; 		//���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   		//��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA1
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);		//ʹ�ܴ�������ж�

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}

//GPIO���ã�PB0
void GPIOInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	  //ʹ��GPIOAʱ��

	//PB0 ��Ϊģ��ͨ����������   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

void Adc1_Init(){
	ADC_InitTypeDef ADC_InitStructure;
	
	TIM2_Init(100,72);		//72000000/72/100=10000Hz��10khz�ɼ�
	DMA1_Init();
	GPIOInit();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //ʹ��ADC1ͨ��ʱ��

	//ADC1��ʼ��
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 			//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;  			//�ر�ɨ�跽ʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			//�ر�����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;   	//ʹ���ⲿ����ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 			//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1; 			//Ҫת����ͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);				//����ADCʱ�ӣ�ΪPCLK2��6��Ƶ����12MHz
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);		//����ADC1ͨ��0Ϊ239.5���������� 
	
	//ʹ��ADC��DMA
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
 
	ADC_ResetCalibration(ADC1);				//��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));				//�ȴ�У׼�Ĵ�����λ���
 
	ADC_StartCalibration(ADC1);				//ADCУ׼
	while(ADC_GetCalibrationStatus(ADC1));				//�ȴ�У׼���
	
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);		//�����ⲿ����ģʽʹ��
}
void FFT_Stop(void);	
void display(unsigned int *p1);
void FFT()
{
	unsigned char i=0,flag=0;

	FFT_Start();
	FFT_Pro();
	display(lBufMagArray);
}
void FFT_Start(void)
{
	 
	Adc1_Init();
	
}
//void FFT_Stop(void)	
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
//	DMA_Cmd(DMA1_Channel1,DISABLE);
////	Adc1_Init();
//	
//}
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
	
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
			
				//����32768�ٳ�65536��Ϊ�˷��ϸ������������
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = (sqrt(X * X + Y * Y)*1.0)/ NPT;
        if(i == 0)	
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}
void FFT_Pro(void)
{
	uint16_t i = 0;//�������
	
	for(i=0;i<NPT;i++)//������Ϊ��Ƭ����ADCֻ�ܲ����ĵ�ѹ ������Ҫǰ����ֱ��ƫִ									
	{									//����ֱ��ƫִ�� 1.56V ��ӦADֵΪ1935
		lBufInArray[i] = ((signed short)(adc_buf[i]-2048)) << 16;		
	}
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);//256��FFT�任
	GetPowerMag();	//�������ӦƵ�ʵ�ģ ��ÿ��Ƶ�ʶ�Ӧ�ķ�ֵ
}
void display(unsigned int *p1)
{
	unsigned int *p2= p1+1;
	unsigned char i;
	for(i=0;i<128;i++)
	{
		OLED_Fill(i,0,i,1+(*p2/16),1);
		OLED_Fill(i,1+(*p2/16),i,63,0);
		p2++;
	}
	OLED_Refresh_Gram();

}



//�жϴ�����
//void  DMA1_Channel1_IRQHandler(void)
//{
//	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET){
//		adc_dma_flag =1;

//		DMA_ClearITPendingBit(DMA1_IT_TC1);
//	}
//}
