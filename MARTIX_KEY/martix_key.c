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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = KEY_row0_Pin|KEY_row1_Pin|KEY_row2_Pin|KEY_row3_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
}

/*------------------------------------------------------------------------------
* brief   : ROW SCAN
-------------------------------------------------------------------------------*/
char KEY_ROW_SCAN(void)
{
	uint8_t Key_row[1]={0x0f};   //������ʼ״̬
    //�����м��
    Key_row[0] = GPIO_ReadInputDataBit(GPIOD,KEY_row0_Pin)<<3;
    Key_row[0] = Key_row[0] | (GPIO_ReadInputDataBit(GPIOD,KEY_row1_Pin)<<2);
    Key_row[0] = Key_row[0] | (GPIO_ReadInputDataBit(GPIOD,KEY_row2_Pin)<<1);
    Key_row[0] = Key_row[0] | (GPIO_ReadInputDataBit(GPIOD,KEY_row3_Pin));
    
    if(Key_row[0] != 0x0f)        
    {
      delay_ms(20);                   //�������
      if(Key_row[0] != 0x0f)		   //��ʼ״̬����4λ��õ�
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
    char Key_Num=0;      	   //1-16�����Ӧ��Ű�������
    char key_row_num=0;        //��Ӧ���м������
    
    KEY_CLO0_OUT_LOW;          //��1�õ͵�ƽ��ʼ���
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);  //���Գ������
        Key_Num = 0 + key_row_num;

    }
    KEY_CLO0_OUT_HIGH;
    
    KEY_CLO1_OUT_LOW;      		//��ʼ���ڶ���  
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
