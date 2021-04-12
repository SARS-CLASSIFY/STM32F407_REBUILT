#ifndef __TIMER_CAPTURE_H
#define __TIMER_CAPTURE_H
#include "sys.h"

void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
void TIM_CAPTURE_ENABLE(void);
uint32_t  TIM_CAP_READ(void);
void TIM_CAP_FRE(void);
#endif























