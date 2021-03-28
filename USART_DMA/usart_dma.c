#include "usart_dma.h"																	   	  
extern u16 rx_len;                          															 //DMA�������ݵĳ���
extern u8 SendBuffer[30];                   															 //DMA���ͻ�����
extern u8 ReceiveBuffer[30];               													       //DMA���ջ�����
extern u16 BufferLen;                       														   //DMA�������ĳ���
extern u8 rx_flag;                          														   //����֡���ձ�־

void Usart_Tx_Config()                                                     //DMA�ķ��ͳ�ʼ������
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);                      //DMA2ʱ��ʹ��
	DMA_DeInit(DMA2_Stream7);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);                       //�ȴ�DMA������ 
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;                           //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;             //DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuffer;                 //DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                  //�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = BufferLen;                            //���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;         //���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                  //�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          //�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                            //ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                    //�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;              //�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;      //����ͻ�����δ���
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);                              //��ʼ��DMA Stream
 
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
}

void Usart_Rx_Config()   //DMA�Ľ��ճ�ʼ������
{ 
  DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);                      //DMA2ʱ��ʹ��
	DMA_DeInit(DMA2_Stream5);
	while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);                       //�ȴ�DMA������ 
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;                           //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;             //DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ReceiveBuffer;              //DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;                 //���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = BufferLen;                            //���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;         //���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                  //�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          //�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                            //ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                    //�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;              //�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;      //����ͻ�����δ���
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);                              //��ʼ��DMA Stream
 
	DMA_Cmd(DMA2_Stream5, ENABLE);                                           //����DMA����                                    
}

void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)             //DMA�ķ���ʹ��
{
	DMA_Cmd(DMA_Streamx, DISABLE);                                           //�ر�DMA���� 	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);	                       //ȷ��DMA���Ա�����  	
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);                                //���ݴ�����  
	DMA_Cmd(DMA_Streamx, ENABLE);                                            //����DMA���� 
}	  

void DMA2_Stream7_IRQHandler(void)                                         //DMA�ķ�������жϺ���
{
  if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)                //�ȴ�DMA2_Steam7�������  
  {  
    DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);                            //���DMA2_Steam7������ɱ�־
    DMA_Cmd(DMA2_Stream7,DISABLE);                                         //�ر�DMA           
    DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);                     //����ж�����λ     
  }  
}




