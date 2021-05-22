#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	    
								    
 //////////////////////////////////////////////////////////////////////////////////	 
//DACREBUILT						  
//////////////////////////////////////////////////////////////////////////////////
#define DAC_DHR12R1   0x40007408   //DACͨ��1�����ַ  (u32)&(DAC->DHR12R1)
#define DAC_DHR12R2   0x40007414   //DACͨ��2�����ַ								    

#define tableSize 64    //256λ����

void Dac1_Init(void);//�ػ�ģʽ��ʼ��		 	 
void Dac1_Set_Vol(u16 vol);
void DAC_DMA_Config(uint16_t * DATA_ADDR1,uint16_t * DATA_ADDR2);
void DAC1_TIM_Config(uint32_t fre);
void DAC2_TIM_Config(uint32_t fre);
void set_vol(uint16_t VPP, uint16_t * sin_out,uint16_t * tri_out);
#endif



