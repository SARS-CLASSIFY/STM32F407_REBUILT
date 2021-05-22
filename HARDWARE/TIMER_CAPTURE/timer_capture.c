#include "timer_capture.h"
#include "usart.h"
#include "oled1.h"
/* ------------ ���벶���Ƶ�ʲ��� ------------------- */
extern u32 fre_kHz;
extern u32 fre_Hz;
extern u16 D;


void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA0
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0����λ��ʱ��5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
 
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		

	
	TIM_PWMIConfig(TIM5, &TIM5_ICInitStructure);     //PWM��������           
  TIM_SelectInputTrigger(TIM5,TIM_TS_TI1FP1);     //ѡ��TI2FP2��ΪTIMx���봥�� һ���ź���֮�� ������ʱ����ʼ����     
  TIM_SelectSlaveMode(TIM5, TIM_SlaveMode_Reset);  //����Ϊ���Ӹ�λģʽ
  TIM_SelectMasterSlaveMode(TIM5, TIM_MasterSlaveMode_Enable);                                       
  TIM_ITConfig(TIM5, TIM_IT_CC1|TIM_IT_Update, ENABLE);  //��������ж� ,����CC1IE�����ж�	
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
  TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
 
 
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}






u16 period = 0;
u16 duty  = 0;
u8 CollectFlag = 1;

void TIM5_IRQHandler(void)
{

	if(CollectFlag){
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����2���������¼�
		{	
			duty = TIM_GetCapture2(TIM5)+1; 				//�ɼ�ռ�ձ�
			period = TIM_GetCapture1(TIM5)+1; 
		}
		
		CollectFlag = 0;
	}	
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}


/*------------------------------------------------------------------------------
* brief   : ���벶����
-------------------------------------------------------------------------------*/
void my_capture(void)
{
			if(!CollectFlag){
			
		  fre_kHz=((20000/period)+5)/10;
		  fre_Hz=(2000000/period);
			
			if(duty*100/period<100){
				D=duty*100/period;
			}
	
						/* ---  OLED  --- */
			MEASURE1_SHOW(fre_Hz/2,D,0);
			
			CollectFlag = 1;//��������
			

		}


}
	