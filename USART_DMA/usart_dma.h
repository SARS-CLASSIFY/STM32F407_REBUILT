#ifndef __USART__DMA_H
#define	__USART__DMA_H	   
#include "sys.h"

void Usart_Tx_Config(void);                                       //DMA�ķ��ͳ�ʼ������
void Usart_Rx_Config(void);                                       //DMA�Ľ��ճ�ʼ������ 
void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);   //DMA�ķ���ʹ��
#endif






























