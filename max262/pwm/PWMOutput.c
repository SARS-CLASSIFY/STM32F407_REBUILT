//-----------------------------------------------------------------
// 程序描述: 
//		利用TIM4输出PWM
// 作    者: 凌智电子
// 开始日期: 2018-05-27
// 完成日期: 2018-05-27
// 版    本: V1.1
//   - V1.0: TIM4通道3输出PWM
//   - V1.1: TIM4增加通道4输出PWM
// 调试工具: 凌智STM32核心开发板、LZE_ST LINK2
// 说    明:
//    - 调试使用的系统时钟频率Fsysclk=72MHz；
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "PWMOutput.h"

u32 counterf;             // 重装载值
u16 divf;                 // 预分频数
u16 radio_num;
//-----------------------------------------------------------------
// 初始化程序区
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void TIM2_GPIO_Init(void)
//-----------------------------------------------------------------
// 函数功能: 设置TIM2 PWM输出I/O初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项: TIM2通道1对应I/O口的PA.0
//-----------------------------------------------------------------

void TIM2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 使能GPIOB和复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  			
	// 复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; 								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//-----------------------------------------------------------------
// TIM2_Mode_Init(void)
//-----------------------------------------------------------------
// 函数功能: TIM2配置，包括时钟、定时时基、PWM1输出比较模式
// 入口参数: 无
// 返 回 值: 无
// 注意事项: TIMx的定时时基T=((Period+1)*(Prescaler+1))/TIMx_clk
//      - Period:自动装载值
//			- Prescaler：分频值
//			- TIMx_clk: APB1时钟2倍，即为72M
//-----------------------------------------------------------------
void TIM2_Mode_Init(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  // 使能TIM2时钟  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  // 设置在下一个更新事件装入自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Period = counterf-1;
	// 设置TIMx时钟的预分频值
//	TIM_TimeBaseStructure.TIM_Prescaler =cycle_num*(720000.000/freq)-1.00;  //预分频值由num变量代替，由按键控制
	TIM_TimeBaseStructure.TIM_Prescaler =divf-1;
	//调整输出方波的周期f=1/T=(TIMx_clk/(Period+1)*(Prescaler+1))=72M/(100*TIM_Prescaler)
	
	// 设置时钟分割:Tdts = Tck_int，不分割
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 		
	// TIMx向上计数模式（即计数值从0---自动装载值）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  				
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 									
	
	// 选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 比较输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 设置待装入捕获比较寄存器的脉冲值，即占空比
	TIM_OCInitStructure.TIM_Pulse = radio_num; 	    //占空比值由radio_num变量代替，由按键控制
	// 输出极性:TIMx输出比较极性高
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 					
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); 
		
	// 使能TIMx在CCR3上的预装载寄存器	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	// 使能TIMx在ARR上的预装载寄存器
	TIM_ARRPreloadConfig(TIM2, ENABLE); 																
	// 使能TIMx
	TIM_Cmd(TIM2, ENABLE);  																						
	
}

//-----------------------------------------------------------------
// void TIM2_PWMOutput_Init(void)
//-----------------------------------------------------------------
// 函数功能: TIM2的PWM输出I/O和PWM输出模式初始化
// 入口参数: 无 
// 返 回 值: 无
// 注意事项: 无
//-----------------------------------------------------------------
void TIM2_PWMOutput_Init(void)
{
	TIM2_GPIO_Init();
	TIM2_Mode_Init();
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
