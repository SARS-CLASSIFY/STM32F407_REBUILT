#ifndef __REMOTE_H
#define __REMOTE_H
#include "pwm.h"
#include "delay.h"
#include "pwm.h"

void preamble_code_gen(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel);
void sent_1(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel);
void sent_0(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel);
void sent_code(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel,u8 code);
void sent_repeat(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel);
void sent_data(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel,
			   u8 addr, u8 cmd,u8 repeat_times);
#endif
