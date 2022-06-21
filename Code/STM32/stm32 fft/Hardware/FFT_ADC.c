#include "stm32f10x_adc.h"
#include "stm32f10x.h"                  // Device header
#include "FFTADC.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include "math.h"
#include "SPIOled.h"
#include "Delay.h"
//volatile uint16_t ADC1_ConvertedValue;			//ADC采样的数据
#define ADC1_DR_Address    ((u32)0x4001244C)		//ADC1的地址

#define NPT 256

uint32_t adc_buf[NPT]={0};			//用于存储ADC转换结果的数组	

static long lBufInArray[NPT];					//传入FFT的数组

static long lBufOutArray[NPT];				//FFT输出 

long lBufMagArray[NPT];				//每个频率对用的幅值

static long BandValues[32]={0};
static int OldHeight[32]={0};
static int Height[32]={0}; 
static uint8_t adc_dma_flag = 0;					//adc的DMA传输完成标志

void FFT_Pro(void);
void display2(void);
uint8_t fal_pot[NPT];							// 记录下落点的坐标
uint8_t prt = 4;									// 量化显示的比例
uint8_t display_num = 1;					// 显示样式的数量
uint8_t fal_speed = 7;						// 下落速度
uint8_t display_auto = 1;					// 自动显示标志位

//TIM2配置，arr为重加载值，psc为预分频系数
void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 		//时钟使能

	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			//根据指定的参数初始化TIMx的时间基数单位
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		//输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM2, & TIM_OCInitStructure);		//初始化外设TIM2_CH2
	
	TIM_Cmd(TIM2, ENABLE); 			//使能TIMx
	TIM_CtrlPWMOutputs(TIM2, ENABLE); 
}

//DMA1配置
void DMA1_Init()
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	  			//使能ADC1通道时钟
	
	//DMA1初始化
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;				//ADC1地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_buf; 		//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 				//方向(从外设到内存)
	DMA_InitStructure.DMA_BufferSize = NPT; 						//传输内容的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 		//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 			//内存地址固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ; //外设数据单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;    //内存数据单位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular  ; 		//DMA模式：循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ; 		//优先级：高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   		//禁止内存到内存的传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //配置DMA1
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);		//使能传输完成中断

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}

//GPIO配置，PB0
void GPIOInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	  //使能GPIOA时钟

	//PB0 作为模拟通道输入引脚   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

void Adc1_Init(){
	ADC_InitTypeDef ADC_InitStructure;
	
	TIM2_Init(100,72);		//72000000/72/100=10000Hz，10khz采集
	DMA1_Init();
	GPIOInit();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //使能ADC1通道时钟

	//ADC1初始化
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 			//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;  			//关闭扫描方式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			//关闭连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;   	//使用外部触发模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 			//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1; 			//要转换的通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);				//配置ADC时钟，为PCLK2的6分频，即12MHz
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);		//配置ADC1通道0为239.5个采样周期 
	
	//使能ADC、DMA
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
 
	ADC_ResetCalibration(ADC1);				//复位校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));				//等待校准寄存器复位完成
 
	ADC_StartCalibration(ADC1);				//ADC校准
	while(ADC_GetCalibrationStatus(ADC1));				//等待校准完成
	
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);		//设置外部触发模式使能
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
	
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
			
				//除以32768再乘65536是为了符合浮点数计算规律
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
	uint16_t i = 0;//填充数组
	
	for(i=0;i<NPT;i++)//这里因为单片机的ADC只能测正的电压 所以需要前级加直流偏执									
	{									//加入直流偏执后 1.56V 对应AD值为1935
		lBufInArray[i] = ((signed short)(adc_buf[i]-2048)) << 16;		
	}
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);//256点FFT变换
	GetPowerMag();	//计算出对应频率的模 即每个频率对应的幅值
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



//中断处理函数
//void  DMA1_Channel1_IRQHandler(void)
//{
//	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET){
//		adc_dma_flag =1;

//		DMA_ClearITPendingBit(DMA1_IT_TC1);
//	}
//}
