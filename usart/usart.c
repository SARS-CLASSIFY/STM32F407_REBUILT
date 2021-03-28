#include "sys.h"
#include "usart.h"	
#include "USART_dma.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

/*------------------------------------------------------------------------------
** brief:	串口接收DMA模式
*-----------------------------------------------------------------------------*/


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 

 u16 rx_len = 0;                          //DMA接收数据的长度
 u8 SendBuffer[30];                   //DMA发送缓存区
 u8 ReceiveBuffer[30];                //DMA接收缓存区
 u16 BufferLen = 77;                       //DMA缓存区的长度
 u8 rx_flag = 0;                          //数据帧接收标志
 u8 SAVEBuffer[77];					//暂存接收到的数据

void uart_init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);                     //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);                    //使能USART1时钟
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);                  //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);                 //GPIOA10复用为USART1
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;                  //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                             //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                        //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                           //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                             //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);                                    //初始化PA9，PA10

	USART_InitStructure.USART_BaudRate = bound;                              //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;              //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                   //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                      //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	         //收发模式
  USART_Init(USART1, &USART_InitStructure);                                //初始化串口1
	
  USART_Cmd(USART1, ENABLE);                                               //使能串口1 
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);                           //开启相关中断

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                        //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                  //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		                     //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                     //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                         //根据指定的参数初始化VIC寄存器

	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);                             //使能串口1的DMA发送   
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);                             //使能串口1的DMA接收
	
	Usart_Tx_Config();                                                          //DMA的发送初始化配置
  Usart_Rx_Config();                                                          //DMA的接收初始化配置 
}

//串口接收DMA在初始化的时候就处于开启状态，一直等待数据的到来，在软件上无需做任何事情，只要在初始化配置的时候设置好配置就行就可以了。
//判断数据接收完成是通过串口空闲中断的方式实现，即当串口数据流停止后，就会产生IDLE中断。
//IDLE的中断在串口无数据接收的情况下，是不会一直产生的，产生的条件是这样的，当清除IDLE标志位后，必须有接收到第一个数据后，才开始触发的，一旦接收的数据断流，没有接收到数据，即产生IDLE中断
void USART1_IRQHandler(void)                                               //接收总线空闲中断
{  
  u16 i;  
  if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)                     //如果接收到一帧数据
  {
    DMA_Cmd(DMA2_Stream5, DISABLE);                                        //关闭DMA，防止处理期间有数据  
    //先读SR,再读DR,是为了清除IDLE中断
    i = USART1->SR;                                                        //读SR寄存器
    i = USART1->DR;                                                        //读DR寄存器
    rx_len = BufferLen - DMA_GetCurrDataCounter(DMA2_Stream5);             //获得这一帧的数据长度
    if(rx_len!=0)                                                          //这一帧数据长度不为0
    { 
      rx_flag=1;                                                           //数据帧接收标志位置1
      for(i=0;i<rx_len;i++)
      {
        SendBuffer[i]= ReceiveBuffer[i];  		  							//将接收的数据保存
				SAVEBuffer[i]= ReceiveBuffer[i];
			}
      for(i=0;i<rx_len;i++)                                                //将数据缓存区清零
      {
        ReceiveBuffer[i]=0;
      }
    }   
    DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5); //清除DMA2_Steam5传输完成标志
    DMA_SetCurrDataCounter(DMA2_Stream5, BufferLen);                       //设置DMA接收单元的长度
    DMA_Cmd(DMA2_Stream5, ENABLE);                                         //打开DMA 
  }  
} 
/*------------------------------------------------------------------------------
** brief:	发送接收到的数据
** mode : mode = 1 发送换行
**				mode = 0 不发送换行
*-----------------------------------------------------------------------------*/
void Send_Received(u8 mode)
{
	if(mode){
    if(rx_flag)                                 															//接收到一帧数据
    {
      Dma_Send_Enable(DMA2_Stream7,rx_len);                                   //将接收到的数据发送到设备              
      printf("\r\n");
			rx_flag = 0; 		//数据帧接收标志清零
    }   
	}
	else{
		if(rx_flag)                                 															//接收到一帧数据
    {
      Dma_Send_Enable(DMA2_Stream7,rx_len);                                   //将接收到的数据发送到设备              
			rx_flag = 0; 		//数据帧接收标志清零
    }   
	
	}
}
/*------------------------------------------------------------------------------
** brief:	字符串转整型,使用后自动清空字符串内容
** str:		传入字符串参数,需要以‘s’结尾
*-----------------------------------------------------------------------------*/

int String2Int(u8 *str)//字符串转数字 
{
		int i;
    int res = 0;
		if(*str == 0)	return 0;
    while(*str!='s')//如果是数字才进行转换，数字0~9的ASCII码：48~57 
    {
        res = 10*res+  *str -48;//字符'0'的ASCII码为48,48-48=0刚好转化为数字0 
				str++;
    }
		for(i=0;i<rx_len;i++)                                                //将数据缓存区清零
		{
			ReceiveBuffer[i]=0;
		}
    return res;
}

