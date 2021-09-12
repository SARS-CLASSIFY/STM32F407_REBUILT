//-----------------------------------------------------------------
// ��������: 
//		����TIM4���PWM
// ��    ��: ���ǵ���
// ��ʼ����: 2018-05-27
// �������: 2018-05-27
// ��    ��: V1.1
//   - V1.0: TIM4ͨ��3���PWM
//   - V1.1: TIM4����ͨ��4���PWM
// ���Թ���: ����STM32���Ŀ����塢LZE_ST LINK2
// ˵    ��:
//    - ����ʹ�õ�ϵͳʱ��Ƶ��Fsysclk=72MHz��
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "PWMOutput.h"

u32 counterf;             // ��װ��ֵ
u16 divf;                 // Ԥ��Ƶ��
u16 radio_num;
//-----------------------------------------------------------------
// ��ʼ��������
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void TIM2_GPIO_Init(void)
//-----------------------------------------------------------------
// ��������: ����TIM2 PWM���I/O��ʼ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: TIM2ͨ��1��ӦI/O�ڵ�PA.0
//-----------------------------------------------------------------

void TIM2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// ʹ��GPIOB�͸���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  			
	// �����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; 								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//-----------------------------------------------------------------
// TIM2_Mode_Init(void)
//-----------------------------------------------------------------
// ��������: TIM2���ã�����ʱ�ӡ���ʱʱ����PWM1����Ƚ�ģʽ
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: TIMx�Ķ�ʱʱ��T=((Period+1)*(Prescaler+1))/TIMx_clk
//      - Period:�Զ�װ��ֵ
//			- Prescaler����Ƶֵ
//			- TIMx_clk: APB1ʱ��2������Ϊ72M
//-----------------------------------------------------------------
void TIM2_Mode_Init(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  // ʹ��TIM2ʱ��  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  // ��������һ�������¼�װ���Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Period = counterf-1;
	// ����TIMxʱ�ӵ�Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_Prescaler =cycle_num*(720000.000/freq)-1.00;  //Ԥ��Ƶֵ��num�������棬�ɰ�������
	TIM_TimeBaseStructure.TIM_Prescaler =divf-1;
	//�����������������f=1/T=(TIMx_clk/(Period+1)*(Prescaler+1))=72M/(100*TIM_Prescaler)
	
	// ����ʱ�ӷָ�:Tdts = Tck_int�����ָ�
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 		
	// TIMx���ϼ���ģʽ��������ֵ��0---�Զ�װ��ֵ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  				
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 									
	
	// ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ���ô�װ�벶��ȽϼĴ���������ֵ����ռ�ձ�
	TIM_OCInitStructure.TIM_Pulse = radio_num; 	    //ռ�ձ�ֵ��radio_num�������棬�ɰ�������
	// �������:TIMx����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 					
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); 
		
	// ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	// ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM2, ENABLE); 																
	// ʹ��TIMx
	TIM_Cmd(TIM2, ENABLE);  																						
	
}

//-----------------------------------------------------------------
// void TIM2_PWMOutput_Init(void)
//-----------------------------------------------------------------
// ��������: TIM2��PWM���I/O��PWM���ģʽ��ʼ��
// ��ڲ���: �� 
// �� �� ֵ: ��
// ע������: ��
//-----------------------------------------------------------------
void TIM2_PWMOutput_Init(void)
{
	TIM2_GPIO_Init();
	TIM2_Mode_Init();
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
