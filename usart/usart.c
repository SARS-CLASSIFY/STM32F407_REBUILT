#include "sys.h"
#include "usart.h"	
#include "USART_dma.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

/*------------------------------------------------------------------------------
** brief:	���ڽ���DMAģʽ
*-----------------------------------------------------------------------------*/


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 

 u16 rx_len = 0;                          //DMA�������ݵĳ���
 u8 SendBuffer[30];                   //DMA���ͻ�����
 u8 ReceiveBuffer[30];                //DMA���ջ�����
 u16 BufferLen = 77;                       //DMA�������ĳ���
 u8 rx_flag = 0;                          //����֡���ձ�־
 u8 SAVEBuffer[77];					//�ݴ���յ�������

void uart_init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);                     //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);                    //ʹ��USART1ʱ��
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);                  //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);                 //GPIOA10����ΪUSART1
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;                  //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                             //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                        //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                           //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                             //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);                                    //��ʼ��PA9��PA10

	USART_InitStructure.USART_BaudRate = bound;                              //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;              //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                   //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                      //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	         //�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure);                                //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);                                               //ʹ�ܴ���1 
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);                           //��������ж�

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                        //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                  //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		                     //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                     //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                                         //����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);                             //ʹ�ܴ���1��DMA����   
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);                             //ʹ�ܴ���1��DMA����
	
	Usart_Tx_Config();                                                          //DMA�ķ��ͳ�ʼ������
  Usart_Rx_Config();                                                          //DMA�Ľ��ճ�ʼ������ 
}

//���ڽ���DMA�ڳ�ʼ����ʱ��ʹ��ڿ���״̬��һֱ�ȴ����ݵĵ�������������������κ����飬ֻҪ�ڳ�ʼ�����õ�ʱ�����ú����þ��оͿ����ˡ�
//�ж����ݽ��������ͨ�����ڿ����жϵķ�ʽʵ�֣���������������ֹͣ�󣬾ͻ����IDLE�жϡ�
//IDLE���ж��ڴ��������ݽ��յ�����£��ǲ���һֱ�����ģ������������������ģ������IDLE��־λ�󣬱����н��յ���һ�����ݺ󣬲ſ�ʼ�����ģ�һ�����յ����ݶ�����û�н��յ����ݣ�������IDLE�ж�
void USART1_IRQHandler(void)                                               //�������߿����ж�
{  
  u16 i;  
  if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)                     //������յ�һ֡����
  {
    DMA_Cmd(DMA2_Stream5, DISABLE);                                        //�ر�DMA����ֹ�����ڼ�������  
    //�ȶ�SR,�ٶ�DR,��Ϊ�����IDLE�ж�
    i = USART1->SR;                                                        //��SR�Ĵ���
    i = USART1->DR;                                                        //��DR�Ĵ���
    rx_len = BufferLen - DMA_GetCurrDataCounter(DMA2_Stream5);             //�����һ֡�����ݳ���
    if(rx_len!=0)                                                          //��һ֡���ݳ��Ȳ�Ϊ0
    { 
      rx_flag=1;                                                           //����֡���ձ�־λ��1
      for(i=0;i<rx_len;i++)
      {
        SendBuffer[i]= ReceiveBuffer[i];  		  							//�����յ����ݱ���
				SAVEBuffer[i]= ReceiveBuffer[i];
			}
      for(i=0;i<rx_len;i++)                                                //�����ݻ���������
      {
        ReceiveBuffer[i]=0;
      }
    }   
    DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5); //���DMA2_Steam5������ɱ�־
    DMA_SetCurrDataCounter(DMA2_Stream5, BufferLen);                       //����DMA���յ�Ԫ�ĳ���
    DMA_Cmd(DMA2_Stream5, ENABLE);                                         //��DMA 
  }  
} 
/*------------------------------------------------------------------------------
** brief:	���ͽ��յ�������
** mode : mode = 1 ���ͻ���
**				mode = 0 �����ͻ���
*-----------------------------------------------------------------------------*/
void Send_Received(u8 mode)
{
	if(mode){
    if(rx_flag)                                 															//���յ�һ֡����
    {
      Dma_Send_Enable(DMA2_Stream7,rx_len);                                   //�����յ������ݷ��͵��豸              
      printf("\r\n");
			rx_flag = 0; 		//����֡���ձ�־����
    }   
	}
	else{
		if(rx_flag)                                 															//���յ�һ֡����
    {
      Dma_Send_Enable(DMA2_Stream7,rx_len);                                   //�����յ������ݷ��͵��豸              
			rx_flag = 0; 		//����֡���ձ�־����
    }   
	
	}
}
/*------------------------------------------------------------------------------
** brief:	�ַ���ת����,ʹ�ú��Զ�����ַ�������
** str:		�����ַ�������,��Ҫ�ԡ�s����β
*-----------------------------------------------------------------------------*/

int String2Int(u8 *str)//�ַ���ת���� 
{
		int i;
    int res = 0;
		if(*str == 0)	return 0;
    while(*str!='s')//��������ֲŽ���ת��������0~9��ASCII�룺48~57 
    {
        res = 10*res+  *str -48;//�ַ�'0'��ASCII��Ϊ48,48-48=0�պ�ת��Ϊ����0 
				str++;
    }
		for(i=0;i<rx_len;i++)                                                //�����ݻ���������
		{
			ReceiveBuffer[i]=0;
		}
    return res;
}

