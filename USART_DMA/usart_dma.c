#include "usart_dma.h"																	   	  
extern u16 rx_len;                          															 //DMA接收数据的长度
extern u8 SendBuffer[30];                   															 //DMA发送缓存区
extern u8 ReceiveBuffer[30];               													       //DMA接收缓存区
extern u16 BufferLen;                       														   //DMA缓存区的长度
extern u8 rx_flag;                          														   //数据帧接收标志

void Usart_Tx_Config()                                                     //DMA的发送初始化配置
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);                      //DMA2时钟使能
	DMA_DeInit(DMA2_Stream7);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);                       //等待DMA可配置 
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;                           //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;             //DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuffer;                 //DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                  //存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = BufferLen;                            //数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;         //外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                  //存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          //存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                            //使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                    //中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;              //存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;      //外设突发单次传输
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);                              //初始化DMA Stream
 
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
}

void Usart_Rx_Config()   //DMA的接收初始化配置
{ 
  DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);                      //DMA2时钟使能
	DMA_DeInit(DMA2_Stream5);
	while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);                       //等待DMA可配置 
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;                           //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;             //DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ReceiveBuffer;              //DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;                 //外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = BufferLen;                            //数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;         //外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                  //存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          //存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                            //使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                    //中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;              //存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;      //外设突发单次传输
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);                              //初始化DMA Stream
 
	DMA_Cmd(DMA2_Stream5, ENABLE);                                           //开启DMA传输                                    
}

void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)             //DMA的发送使能
{
	DMA_Cmd(DMA_Streamx, DISABLE);                                           //关闭DMA传输 	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);	                       //确保DMA可以被设置  	
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);                                //数据传输量  
	DMA_Cmd(DMA_Streamx, ENABLE);                                            //开启DMA传输 
}	  

void DMA2_Stream7_IRQHandler(void)                                         //DMA的发送完成中断函数
{
  if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)                //等待DMA2_Steam7传输完成  
  {  
    DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);                            //清除DMA2_Steam7传输完成标志
    DMA_Cmd(DMA2_Stream7,DISABLE);                                         //关闭DMA           
    DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);                     //清除中断屏蔽位     
  }  
}




