# **stm32f407 TIM_CAP_REBUILT**

对于stm32f4的输入捕获相关代码作了整理，核心文件未timer_capture.c及timer_capture.h加入工程即可使用。

#### 1）main函数调用示例：

1.需要引入timer_capture.h头文件

2.配置基本功能初始化函数

3.在while循环中调用TIM_CAP_READ(测量上升沿时间)或TIM_CAP_FRE（测量pwm频率）



#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "timer_capture.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组
	delay_init(168);  
	uart_init(115200);
	**TIM5_CH1_Cap_Init(0XFFFFFFFF,42-1);** 
	

    while(1)
    {
    	TIM_CAP_FRE();
    	delay_ms(5);
    }
}

**2）移植说明**（THE MOST IMPORTANT OF REBUILT）

使用不同型号的F4芯片或者想要开启不同定时器不同通道以及使用不同的引脚复用功能需要：

1.在timer_capture.c中在有‘’***>_<***‘’注释处语句着重修改

2.使用KEIL的replace功能将TIM5全部替换成TIMX,并将GPIOA全部替换为GPIOX

3.修改timer_capture.h及main函数中的相应代码

attention:芯片或者设置或者定时器的不同可能导致定时器总线的时钟频率不同，本例采用1M的时钟采样频率，所以分频系数需要自己根据实际情况设计

