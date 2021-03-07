#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 						  
////////////////////////////////////////////////////////////////////////////////// 	 


#define ADC1_DR_Address    ((u32)0x4001244C)		//ADC1??
 

void ADC_SR_Detect_Config(void);
void  DMA2_Stream0_IRQHandler(void);
#endif 
