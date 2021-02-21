#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F4工程模板-库函数版本
//淘宝店铺：http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	

void TIM3_PWM_Init(u32 arr,u32 psc);
void set_pulse(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel);
void close_pwm(TIM_TypeDef* TIMx,u8 channel);
#endif
