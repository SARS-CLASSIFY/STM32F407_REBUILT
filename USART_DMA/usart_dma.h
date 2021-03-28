#ifndef __USART__DMA_H
#define	__USART__DMA_H	   
#include "sys.h"

void Usart_Tx_Config(void);                                       //DMA的发送初始化配置
void Usart_Rx_Config(void);                                       //DMA的接收初始化配置 
void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);   //DMA的发送使能
#endif






























