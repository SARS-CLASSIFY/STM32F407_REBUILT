#include "adc.h"
#include "usart.h"
#include "FFT.h"

volatile uint16_t ADC_DualConvertedValueTab[2048];  //�洢adcת������
/* ------------------------------------------------------------------ 
* �ж�����
* ------------------------------------------------------------------- */ 
void NVIC_Config(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  
  /* Enable the DMA2_Stream0 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
	 /******************ADC�ж�****************************************/

  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}


void ADC_SR_Detect_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
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
  

  /**************************ADC DMA config*************************************/
  DMA_DeInit(DMA2_Stream0);
  while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);//DMA DeInit
  
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;                              //DMA2����ͨ��
  DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC3->DR;                 //�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_DualConvertedValueTab;    //�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //DMA���赽�ڴ�
  DMA_InitStructure.DMA_BufferSize = 1024;        //�������ݵĶ���                             
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //�ڴ����ʧ��
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //ѭ������
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                                             
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                       
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;                   
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);
  
  /**************************ADC TIMʹ��,������ת��ʱʹ��*************************************/
  TIM_Cmd(TIM3, DISABLE);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); //tim
  
  TIM_TimeBaseStructure.TIM_Period = 99;         //psc = 83 
  TIM_TimeBaseStructure.TIM_Prescaler = 9;     //arr = 0 ��168KHZ��Ƶ�ʽ���ADCת��
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
 
	
	TIM_Cmd(TIM3, ENABLE);     //ʹ��TIM3
  /* Enable ADC1 *************************************************************/
  ADC_Cmd(ADC3, ENABLE);
	/* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC3, ENABLE);
  //ʹ��DMA�ж�
  DMA_ITConfig(DMA2_Stream0,DMA_IT_TC , ENABLE);
	

//	ADC_ITConfig(ADC3,ADC_IT_EOC,ENABLE);//ʹ��ADC��������ж�
   /* NVIC */
  NVIC_Config();
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
}


/*************************DMA������ɴ�����**********************/
void DMA2_Stream0_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0) != RESET)
  {
		ADC_Cmd(ADC1, DISABLE);
		ADC_DMACmd(ADC3, DISABLE);
		FFT();
		ADC_Cmd(ADC1, ENABLE);
		ADC_DMACmd(ADC3, ENABLE);
  }
  DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
}
