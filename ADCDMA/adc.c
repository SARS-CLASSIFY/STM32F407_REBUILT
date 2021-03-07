#include "adc.h"
#include "usart.h"
#include "filter.h"

volatile uint16_t ADC_DualConvertedValueTab[2000];  //�洢adcת������
float x[5] = {0};//�洢�˲���������
float y[5] = {0};
extern float IIRCoeff[5];//highpass
extern float IIRCoeff1[5];//lowpass
extern float IIRCoeff2[5];//BANDPASS
extern float IIRCoeff3[5];//BANDSTOP


void ADC_SR_Detect_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  /********************(GPIO ADC DMA TIM)**********************/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //ADC3
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //GPIOA
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3
  
  /**************************ADC GPIO config************************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;        //ADC123_IN1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /**************************ADC DMA NVIC config**********************************/  
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  /**************************ADC DMA config*************************************/
  DMA_DeInit(DMA2_Stream0);
  while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);//DMA DeInit
  
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;                              //DMA2����ͨ��
  DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC3->DR;                 //�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_DualConvertedValueTab;    //�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //DMA���赽�ڴ�
  DMA_InitStructure.DMA_BufferSize = 1;        //�������ݵĶ���                             
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //�ڴ����ʧ��
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //ѭ������
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                                             
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;                       
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;                   
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
//  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);            //DMA�ж�ʹ��,ʹ�ܺ������DMA�ж�?
//  DMA_ClearFlag(DMA2_Stream0,DMA_IT_TC);
  DMA_Cmd(DMA2_Stream0, ENABLE);
  
  /**************************ADC TIMʹ��,������ת��ʱʹ��*************************************/
  TIM_Cmd(TIM3, DISABLE);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); //tim
  
  TIM_TimeBaseStructure.TIM_Period = 9;         //psc = 83 
  TIM_TimeBaseStructure.TIM_Prescaler = 99;     //arr = 0 ��84KHZ��Ƶ�ʽ���ADCת��
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //����ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM3, ENABLE); // enable TIM3 reload
  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);  //TIM3 UPDATA
  
  /***************************ADC Common Init**********************************/  
  ADC_DeInit();
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;                   
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;       
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
  /**********************ADC Init CONFIG******************************/  
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                        //
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                 //
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                            //ʹ������ת��,��ʱ�Ӵ���ת��ì��,����ֻ���Կ���һ��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //������
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;        // TIM3_TRGO
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                    //����ͨ����(���ݴ�С)
  ADC_Init(ADC3, &ADC_InitStructure);
  
  
  ADC_RegularChannelConfig(ADC3,ADC_Channel_1,1,ADC_SampleTime_144Cycles); //����ͨ����
 
 /******************ADC�ж�****************************************/
  ADC_ITConfig(ADC3,ADC_IT_EOC,ENABLE);//ʹ��ADC��������ж�
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
 
 /*****************************************************************/
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
  ADC_DMACmd(ADC3, ENABLE); //ʹ��ADC DMA 
  ADC_Cmd(ADC3, ENABLE);    //ʹ��ADC 
  TIM_Cmd(TIM3, ENABLE);     //ʹ��TIM3
  //ADC_SoftwareStartConv(ADC3); //������� ADC 
 
}
/***********************ADC�����ж�********************************
**brief: ADC�жϴ�����,�����˲�����
*******************************************************************/
void ADC_IRQHandler(void)
{

	if(ADC_GetITStatus(ADC3, ADC_IT_EOC)!=RESET)
	{
		
		ADC_ClearITPendingBit(ADC3, ADC_IT_EOC);
	}

	x[0] = ((float)ADC_DualConvertedValueTab[0]); 
	iir_biquad_stm32(y, x, IIRCoeff1, 3);
	DAC_SetChannel1Data(DAC_Align_12b_R,y[0]+2048);//DAC�������
}


/*************************DMA������ɴ�����**********************/
void  DMA2_Stream0_IRQHandler(void)
{
	ADC_DualConvertedValueTab[1] = 1;	
	if(DMA_GetITStatus(DMA2_Stream0,DMA_IT_TCIF1)!=RESET){
		DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF1);
	}
}
