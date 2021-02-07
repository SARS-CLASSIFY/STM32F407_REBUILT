#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"	

void TIM3_PWM_Init(u32 arr,u32 psc);
void set_pulse(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel);
#endif
