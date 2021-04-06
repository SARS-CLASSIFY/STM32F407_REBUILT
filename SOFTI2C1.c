#include "SOFTI2C1.h"
#include "delay.h"


/*------------------------------------------------------------------------------
* brief   : initial
-------------------------------------------------------------------------------*/
void SOFTI2C1_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��


  GPIO_InitStructure.GPIO_Pin = SOFTI2C1_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(SOFTI2C1_SCL_GPIO, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = SOFTI2C1_SDA_PIN ;
	GPIO_Init(SOFTI2C1_SDA_GPIO, &GPIO_InitStructure);//��ʼ��
	

	SOFTI2C1_SCL=1;
	SOFTI2C1_SDA=1;
}
/*------------------------------------------------------------------------------
* brief   : generate start signal
-------------------------------------------------------------------------------*/
void SOFTI2C1_Start(void)
{
	SDA_OUT();     //sda�����
	SOFTI2C1_SDA=1;	  	  
	SOFTI2C1_SCL=1;
	delay_us(4);
 	SOFTI2C1_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	SOFTI2C1_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
/*------------------------------------------------------------------------------
* brief   : generate stop signal
-------------------------------------------------------------------------------*/
void SOFTI2C1_Stop(void)
{
	SDA_OUT();//sda�����
	SOFTI2C1_SCL=0;
	SOFTI2C1_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	SOFTI2C1_SCL=1; 
	SOFTI2C1_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
/*------------------------------------------------------------------------------
* brief   : wait ack
* ReturnValue    : 0:fail,1:success
-------------------------------------------------------------------------------*/
u8 SOFTI2C1_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	SOFTI2C1_SDA=1;delay_us(1);	   
	SOFTI2C1_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			SOFTI2C1_Stop();
			return 1;
		}
	}
	SOFTI2C1_SCL=0;//ʱ�����0 	   
	return 0;  
} 
/*------------------------------------------------------------------------------
* brief   : gen ack sig
-------------------------------------------------------------------------------*/
void SOFTI2C1_Ack(void)
{
	SOFTI2C1_SCL=0;
	SDA_OUT();
	SOFTI2C1_SDA=0;
	delay_us(2);
	SOFTI2C1_SCL=1;
	delay_us(2);
	SOFTI2C1_SCL=0;
}
/*------------------------------------------------------------------------------
* brief   : gen nack sig
-------------------------------------------------------------------------------*/    
void SOFTI2C1_NAck(void)
{
	SOFTI2C1_SCL=0;
	SDA_OUT();
	SOFTI2C1_SDA=1;
	delay_us(2);
	SOFTI2C1_SCL=1;
	delay_us(2);
	SOFTI2C1_SCL=0;
}					 				     
/*------------------------------------------------------------------------------
* brief   : send one bit
* ReturnValue    : 1:ack,0:nack
-------------------------------------------------------------------------------*/	  
void SOFTI2C1_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    SOFTI2C1_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        SOFTI2C1_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		SOFTI2C1_SCL=1;
		delay_us(2); 
		SOFTI2C1_SCL=0;	
		delay_us(2);
    }	 
} 	    
/*------------------------------------------------------------------------------
* brief   : receive one data with res
* ReturnValue    : 0:send nack,1:send ack
-------------------------------------------------------------------------------*/
u8 SOFTI2C1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        SOFTI2C1_SCL=0; 
        delay_us(2);
		SOFTI2C1_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        SOFTI2C1_NAck();//����nACK
    else
        SOFTI2C1_Ack(); //����ACK   
    return receive;
}


/*------------------------------------------------------------------------------
* brief   : read one bit without res
-------------------------------------------------------------------------------*/
u8 SOFTI2C1_Receive_Byte()
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        SOFTI2C1_SCL=0; 
        delay_us(2);
		SOFTI2C1_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 

    return receive;
}

/*---------------------------------------------------------------------
*	brief: SOFTI2C1 send string
*	EntryParameter: slaveAddr,regAddr,data,datalen
----------------------------------------------------------------------*/
uint8_t I2CSendString(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data,uint16_t dataLen)
{
		uint8_t cnt;
    if(dataLen == 0 )
    {
        return -1;
    }

    SOFTI2C1_Start();               	           //��������
    SOFTI2C1_Send_Byte(slaveAddr << 1);       //����������ַ
	
		SOFTI2C1_Send_Byte(regAddr);

    for( cnt = 0; cnt < dataLen; cnt++)
    {
        SOFTI2C1_Send_Byte(data[cnt]);
    }

    SOFTI2C1_Stop();                  //��������

    return 0;
}

/*------------------------------------------------------------------------------
* brief   : i2cRecvString
* Description    : string recive
* EntryParameter : slave_addr:slave device I2C addr,string:recive string,len:string lenth
* ReturnValue    : -1:fail,not -1:success
-------------------------------------------------------------------------------*/


uint8_t I2CRecvString(uint8_t slaveAddr,uint8_t regAddr,uint8_t *buff,uint16_t buffSize)
{	
		uint8_t i;
    if(buffSize == 0)
    {
        return -1;
    }

    SOFTI2C1_Start();                                 //��������
		SOFTI2C1_Send_Byte(slaveAddr << 1);
		SOFTI2C1_Send_Byte(regAddr);
	
		SOFTI2C1_Start(); 
    SOFTI2C1_Send_Byte((slaveAddr << 1) + 1);                //����������ַ

    for(i = 0; i < buffSize; i++)
    {
        buff[i] = SOFTI2C1_Receive_Byte();                   //��������
        if(i == (buffSize - 1))
        {
            SOFTI2C1_Ack();
        }
        else
        {
            SOFTI2C1_NAck();                      				//���;ʹ�λ
        }
    }

    SOFTI2C1_Stop();                                 	  //��������

    delay_us(10);

    return 0;
}



















