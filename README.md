# stm32F4XX数字滤波

1. **ADC.h说明：***为需要重点配置**

   1）ADC.H ADC采用定时器触发DMA模式 PA1 AD读入 PA4DA输出

   2）***ADC由TIM3触发,由此决定采样频率，此处fc = 84000HZ

   3)  开启DMA传输模式,ADC转化完成可认为瞬间传输到目标数组ADC_DualConvertedValueTab[0]

   4)  DMA_BufferSize可以设置DMA传输数据大小，设为多值会填充ADC_DualConvertedValueTab数组，可以由此配置多通道采样

   5）ADC中断中进行滤波处理并用DAC设置值输出

   6） x[] y[]为进行滤波的输入输出数组,IIRCoeff[]为外部变量,滤波器抽头系数

2. **DAC为例程源码,未使用DMA**

3. **filter.c中**

   1）float 型为滤波器抽头系数,MATLAB生成

   2）iir_biquad_stm32（）为高低通及带阻滤波函数

   3）iir_biquad_stm32_x（）为带通滤波函数

   4）改变滤波器类型可以在adc.c中 改变iir_biquad_stm32(y, x, IIRCoeff1, 3);函数及传递的抽头系数

4. **滤波器设计流程见网页文件**,**FDACODE中为MATLAB生成的原始滤波器数据,可作参考**

5. **PROBLEM:DMA中断正常使用**

6. 附上main.c代码

   ```
   #include "sys.h"
   #include "delay.h"
   #include "usart.h"
   #include "led.h"
   #include "adc.h"
   #include "dac.h"
   extern uint16_t ADC_DualConvertedValueTab[2000];
   
   //PA1 AD
   //PA4 DA
   
   int main(void)
   { 
   	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   	delay_init(168);    
   	uart_init(115200);
   	Dac1_Init();
   	ADC_SR_Detect_Config();
   	
   	while(1)
   	{ 
   //		printf("The current value =%d  %d %d %d  		  \r\n",ADC_DualConvertedValueTab[0],ADC_DualConvertedValueTab[1],ADC_DualConvertedValueTab[2],ADC_DualConvertedValueTab[3]);//打印调试信息
          
   		delay_ms(500);
   	}
   }
   
   ```

   

   
