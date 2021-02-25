#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	    
								    
 //////////////////////////////////////////////////////////////////////////////////	 
//DACREBUILT						  
//////////////////////////////////////////////////////////////////////////////////
#define DAC_DHR12R1   0x40007408   //DAC通道1输出地址  (u32)&(DAC->DHR12R1)
#define DAC_DHR12R2   0x40007414   //DAC通道2输出地址								    

#define tableSize 256    //256位数据

void Dac1_Init(void);//回环模式初始化		 	 
void Dac1_Set_Vol(u16 vol);
void DAC_DMA_Config(void);
void DAC1_TIM_Config(uint32_t fre);
void DAC2_TIM_Config(uint32_t fre);
void set_vol(uint16_t VPP, uint16_t * wave);
#endif



