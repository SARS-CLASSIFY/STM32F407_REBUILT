#include "timer_capture.h"
#include "usart.h"
#include "oled1.h"
/* ------------ 输入捕获测频率参数 ------------------- */
extern u32 fre_kHz;
extern u32 fre_Hz;
extern u16 D;


void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA0
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0复用位定时器5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
 
	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		

	
	TIM_PWMIConfig(TIM5, &TIM5_ICInitStructure);     //PWM输入配置           
  TIM_SelectInputTrigger(TIM5,TIM_TS_TI1FP1);     //选择TI2FP2作为TIMx输入触发 一个信号来之后 启动定时器开始计数     
  TIM_SelectSlaveMode(TIM5, TIM_SlaveMode_Reset);  //配置为主从复位模式
  TIM_SelectMasterSlaveMode(TIM5, TIM_MasterSlaveMode_Enable);                                       
  TIM_ITConfig(TIM5, TIM_IT_CC1|TIM_IT_Update, ENABLE);  //允许更新中断 ,允许CC1IE捕获中断	
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
  TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
 
 
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}






u16 period = 0;
u16 duty  = 0;
u8 CollectFlag = 1;

void TIM5_IRQHandler(void)
{

	if(CollectFlag){
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获2发生捕获事件
		{	
			duty = TIM_GetCapture2(TIM5)+1; 				//采集占空比
			period = TIM_GetCapture1(TIM5)+1; 
		}
		
		CollectFlag = 0;
	}	
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}


/*------------------------------------------------------------------------------
* brief   : 输入捕获函数
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
			
			CollectFlag = 1;//重启捕获
			

		}


}
	