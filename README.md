# STM32_FLASH

- include "stm32flash.h"

- 分两种使用模式

  - 存储uint8_t字符串类型的数据，在main中添加如下定义

    ```c
    uint8_t WRITE_DATA[100];
    uint8_t datatemp[100];	
    
    #define TEXT_LENTH sizeof(WRITE_DATA)	 		  	//数组长度	
    #define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)
    #define FLASH_SAVE_ADDR  0X0800C004 	//设置FLASH 保存地址必须为偶数，且所在扇区,要大于本代码所占用到的扇区
    
    ```

    读写示例

    ```c
    STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)WRITE_DATA,SIZE);
    
    STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)datatemp,SIZE);
    ```

    

  - 存储uint16_t类型数据，main中定义

    ```c
    uint16_t WRITE_DATA[1] = {0x07d0};		
    uint16_t datatemp[1];	
    
    #define TEXT_LENTH sizeof(WRITE_DATA)	 		  	//数组长度	
    #define SIZE TEXT_LENTH/2+((TEXT_LENTH%2)?1:0)
    #define FLASH_SAVE_ADDR  0X0800C004 	//设置FLASH 保存地址必须为偶数,且所在扇区,要大于本代码所占用到的扇区
    
    ```

    读写示例

    ```c
    STMFLASH_Write_Uint16(FLASH_SAVE_ADDR,(u16*)WRITE_DATA,SIZE);
    
    FLASH_ReadData_Uint16(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);
    ```

    