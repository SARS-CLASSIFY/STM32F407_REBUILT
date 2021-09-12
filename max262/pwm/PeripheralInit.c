//-----------------------------------------------------------------
// 程序描述: 
//		在Keil uVision4开发平台下基于STM32的程序模板，外设初始化以及配置
// 作    者: 凌智电子
// 开始日期: 2014-04-24
// 完成日期: 2014-04-24
// 修改日期:
// 版    本: V1.0
//   - V1.0: 初步模板
// 调试工具: 凌智STM32+FPGA电子系统设计开发板、LZE_ST LINK2、2.8寸液晶屏
// 说    明:
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "PeripheralInit.h"
#include "PWMOutput.h"
//-----------------------------------------------------------------
// 功能程序区
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void PeripheralInit(void)
//-----------------------------------------------------------------
// 函数功能: 外设初始化函数
// 入口参数: 无 
// 返 回 值: 无
// 全局变量: 无
// 注意事项: 无
//-----------------------------------------------------------------
void PeripheralInit(void)
{
	TIM2_PWMOutput_Init();
}

void MAX262_GPIO_Init(void)               // MAX262引脚配置
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//使能IO口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  | GPIO_Pin_9          // D0, D1 
															| GPIO_Pin_10 | GPIO_Pin_11         // A0, A1
															| GPIO_Pin_12 | GPIO_Pin_13;        // A2, A3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                // 推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11  | GPIO_Pin_12 ;        // EN, WR 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                // 推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);

}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
