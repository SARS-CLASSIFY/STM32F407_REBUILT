DMA串口收发

- 实现DMA串口收发，实现单一函数调用获取串口发送的字符并转INT型

- 调用说明

  - USART_DAM中文件加入工程

  - usart文件夹代替原有SYSTEM中文件夹

  - main.c中

    ```c
    #include "usart_dma.h"
    #include "usart.h"
    extern  u8 SAVEBuffer[77];					//暂存接收到的数据
    extern u8 rx_flag; 
    ```

  - String2Int()函数可将接收到的字符串转INT型，不支持负值，且发送字符串必须以's'结尾

  - Send_Received（）可发送接收到的字符串一次

  - 使用示例

    ```c
    		if(String2Int(SAVEBuffer)!=0 ){
    			printf("%d\r\n",String2Int(SAVEBuffer));	
    			memset(SAVEBuffer,0,sizeof(SAVEBuffer));
    		}
    //printf中为处理步骤，可用于串口设定频率等
    ```

    

