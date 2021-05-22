#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "timer_capture.h"
#include "pwm.h"
#include "oled.h"
#include "oled1.h"
#include "dac.h"
#include "stage.h"
/* ----------------PA0 -> 输入捕获-----------------------
									 PB1 -> PWM输出
									 PE0 -> OLED SCL
									 PE1 -> OLED SDA
									 PA4 -> DAC1
									 PA5 -> DAC2
-------------------------------------------------------*/

/* ------------ 输入捕获测频率参数 ------------------- */
extern u16 period;
extern u16 duty ;
extern u8 CollectFlag ;
u32 fre_kHz=0;
u32 fre_Hz=0;
u16 D;
/* ---------------PWM输出频率 -------------------------*/
u32 pwm_fre = 25500;
u32 my_arr;

/* ------------ DAC输出波形 --------------------------*/
extern uint16_t Sine12bit[tableSize];
extern uint16_t Rec12bit[tableSize];
extern uint16_t sawtooth[tableSize];
extern uint16_t Tri_12bit[tableSize];
extern uint16_t sin_out[tableSize];
extern uint16_t tri_out[tableSize];
extern uint16_t rec_out[tableSize];
u32 DAC_VOT = 3000;//初始值3.0V
u32 DAC1_FRE = 50;	//50HZ
u32 DAC2_FRE = 2500;	//1000HZ


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
	martix_key_init();
	//TIM CAPTURE
	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以2Mhz的频率计数 
	OLED_Init();
	OLED1_Init();
	//PWM OUT
	my_arr = 1000000/pwm_fre;
	TIM3_PWM_Init(my_arr,21-1);//1M/arr = fre		

	
	                                                                
	//DAC OUT
	Dac1_Init();
	DAC1_TIM_Config(12621);
	DAC2_TIM_Config(2500);
	set_vol(3000,sin_out,tri_out);
	DAC_DMA_Config(Tri_12bit,Sine12bit);
	

	STAGE_ONE();//起始状态
  while(1)
	{


		STATE_CHANGE();
		STATE_RES_CHANGE();
		
		delay_ms(100);
	}
}



