#include "drv_uart.h"


/**
  * @brief :串口初始化
  * @param :
  *			@UartBaudRate:串口波特率
  * @note  :无
  * @retval:无
  */
void drv_uart_init( uint32_t UartBaudRate )
{
	GPIO_InitTypeDef	UartGpioInitStructer;
	USART_InitTypeDef	UartinitStructer;
	
	//在配置过程中，为防止TX RX不再同一个端口上，增强可移植性，固分开配置
	//初始化串口TX RX 引脚 
	RCC_APB2PeriphClockCmd( UART_TX_GPIO_CLK | UART_RX_GPIO_CLK, ENABLE );	//打开TX RX 端口时钟
	
	UartGpioInitStructer.GPIO_Mode = GPIO_Mode_AF_PP;
	UartGpioInitStructer.GPIO_Speed = GPIO_Speed_2MHz;
	//TX
	UartGpioInitStructer.GPIO_Pin = UART_TX_GPIO_PIN;
	GPIO_Init( UART_TX_GPIO_PORT, &UartGpioInitStructer );		//初始化TX引脚  配置为复用功能
	//RX
	UartGpioInitStructer.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_AF_PP;//GPIO_Mode_AF_OD;//GPIO_Mode_AIN;//GPIO_Mode_IN_FLOATING;
	UartGpioInitStructer.GPIO_Pin = UART_RX_GPIO_PIN;
	GPIO_Init( UART_RX_GPIO_PORT, &UartGpioInitStructer );		//初始化RX引脚  配置为输入
	
	//配置USART外设
	USART_DeInit( UART_PORT );		//外设复位
	
	if( USART1 == UART_PORT )		//使能外设时钟
	{
		RCC_APB2PeriphClockCmd( UART_PORT_CLK, ENABLE );			
	}																	//不同的USART外设可能在不同的APB时钟上														
	else																//STM32F103单片机只有USART1在APB2上，如平台有差异做相应改变即可
	{
		RCC_APB1PeriphClockCmd( UART_PORT_CLK, ENABLE );
	}
	
	UartinitStructer.USART_BaudRate = UartBaudRate;						//设置波特率
	UartinitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//不使用流控制
	UartinitStructer.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		//发送和接收	
	UartinitStructer.USART_Parity = USART_Parity_No;					//不带校验
	UartinitStructer.USART_StopBits = USART_StopBits_1;					//一个停止位
	UartinitStructer.USART_WordLength = USART_WordLength_8b;			//8个数据位
	
	USART_Cmd( UART_PORT, DISABLE );									//失能外设
	USART_Init( UART_PORT, &UartinitStructer );							//初始化外设
	USART_Cmd( UART_PORT, ENABLE );										//使能外设	
}

/**
  * @brief :串口发送数据
  * @param :
  *			@TxBuffer:发送数据首地址
  *			@Length:数据长度
  * @note  :无
  * @retval:无
  */
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length )
{
	while( Length-- )
	{
		while( RESET == USART_GetFlagStatus( UART_PORT, USART_FLAG_TXE ));
		UART_PORT->DR = *TxBuffer;
		TxBuffer++;
	}
}

/**
  * @brief :串口接收数据
  * @param :
  *			@RxBuffer:发送数据首地址
  * @note  :无
  * @retval:接收到的字节个数
  */
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer )
{
	uint8_t l_RxLength = 0;
	uint16_t l_UartRxTimOut = 0x7FFF;
	
	while( l_UartRxTimOut-- )			//等待查询串口数据
	{
		if( RESET != USART_GetFlagStatus( UART_PORT, USART_FLAG_RXNE ))
		{
			*RxBuffer = (uint8_t)UART_PORT->DR;
			RxBuffer++;
			l_RxLength++;
			l_UartRxTimOut = 0x7FFF;	//接收到一个字符，回复等待时间
		}
		if( 64 == l_RxLength )
		{
			break;		//不能超过64个字节
		}
	}
	
	return l_RxLength;					//等待超时，数据接收完成
}



/*------------------------------------------------------------------------------
* brief   : str->int_64
-------------------------------------------------------------------------------*/

int64_t myatoi(char *str)
{
  int64_t val = 0;
  uint8_t i = 0, isPositive = 1;

  while (str[i] == ' ') // skip leading spaces
    i++;

  isPositive = (str[i] != '-'); // handle sign
  if (str[i] == '+' || str[i] == '-')
    i++;

  while (str[i] >= '0' && str[i] <= '9') // read until isdigit(str[i])=false, this will also handle '\0'
  {
    val *= 10;
    val += str[i++] - '0';
  }

  if (!isPositive) // handle negative number
    val = -val;
  return val;
}
/*------------------------------------------------------------------------------
* brief   : a->f
-------------------------------------------------------------------------------*/
double myatof(char *str)
{
  double val = 0, floatPart = 0;
  uint16_t dotPos = 0, len = 0;

  val = myatoi(str); // get int part

  while ((str[len] >= '0' && str[len] <= '9') || str[len] == '.' || str[len] == '-') // get float string length
    len++;
  for (; dotPos < len; dotPos++) // get dot position
    if (str[dotPos] == '.')
      break;

  if (dotPos + 1 >= len) // no float part detected, treat as myatoi with double output
    return val;

  floatPart = myatoi(str + dotPos + 1);
  while (dotPos + 1 < len)
  {
    floatPart /= 10.0;
    dotPos++;
  }
  val += val < 0 ? -floatPart : floatPart;
  return val;
}

