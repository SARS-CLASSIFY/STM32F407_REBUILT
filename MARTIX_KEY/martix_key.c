#include "martix_key.h"
#include "delay.h"



/*------------------------------------------------------------------------------
* brief   : MARTIX_KEY_GPIO INIT
-------------------------------------------------------------------------------*/
void martix_key_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  //PIN INIT
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = KEY_row0_Pin|KEY_row1_Pin|KEY_row2_Pin|KEY_row3_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
}

/*------------------------------------------------------------------------------
* brief   : ROW SCAN
-------------------------------------------------------------------------------*/
char KEY_ROW_SCAN(void)
{
	uint8_t Key_row[1]={0x0f};   //按键初始状态
    //进行行检测
    Key_row[0] = GPIO_ReadInputDataBit(GPIOD,KEY_row0_Pin)<<3;
    Key_row[0] = Key_row[0] | (GPIO_ReadInputDataBit(GPIOD,KEY_row1_Pin)<<2);
    Key_row[0] = Key_row[0] | (GPIO_ReadInputDataBit(GPIOD,KEY_row2_Pin)<<1);
    Key_row[0] = Key_row[0] | (GPIO_ReadInputDataBit(GPIOD,KEY_row3_Pin));
    
    if(Key_row[0] != 0x0f)        
    {
      delay_ms(20);                   //软件消抖
      if(Key_row[0] != 0x0f)		   //初始状态右移4位后得到
        {   

                switch(Key_row[0])
                {
                    case 0x07:         //0111 
                        return 1;
                    case 0x0b:         //1011
                        return 2;
                    case 0x0d:         //1101 
                        return 3;
                    case 0x0e:         //1110 
                        return 4;
                    default :
                        return 0;
                }
        }
      else return 0;
    }
    else return 0;
}

/*------------------------------------------------------------------------------
* brief   : MARTIX_KEY_SCAN
* Description    : return key value
* ReturnValue    : u8 1~16 for 1 to 16 key
-------------------------------------------------------------------------------*/
char MARTIX_KEY_SCAN(void)
{    
    char Key_Num=0;      	   //1-16代表对应序号按键按下
    char key_row_num=0;        //对应的行检测数据
    
    KEY_CLO0_OUT_LOW;          //列1置低电平开始检测
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);  //可以持续检测
        Key_Num = 0 + key_row_num;

    }
    KEY_CLO0_OUT_HIGH;
    
    KEY_CLO1_OUT_LOW;      		//开始检测第二列  
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);
        Key_Num = 4 + key_row_num;

    }
    KEY_CLO1_OUT_HIGH;
    
    KEY_CLO2_OUT_LOW;    
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);
    Key_Num = 8 + key_row_num;

    }
    KEY_CLO2_OUT_HIGH;
    
    KEY_CLO3_OUT_LOW;    
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    {
        while(KEY_ROW_SCAN() != 0);
        Key_Num = 12 + key_row_num;

    }
    KEY_CLO3_OUT_HIGH;
    
    return Key_Num;
}
