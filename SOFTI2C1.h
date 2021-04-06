#ifndef __SOFTI2C1_H
#define __SOFTI2C1_H
#include "sys.h" 


// ------ configuration start ------
 
#define SOFTI2C1_SCL    PBin(8) //SCL
#define SOFTI2C1_SDA    PBin(9) //SDA	 
#define READ_SDA   			PBin(9)    //����SDA 

#define SOFTI2C1_SCL_PIN  GPIO_Pin_8
#define SOFTI2C1_SDA_PIN  GPIO_Pin_9
#define SOFTI2C1_SCL_GPIO	 GPIOB
#define SOFTI2C1_SDA_GPIO	 GPIOB
// ------ configuration end --------
   	   		   
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ

// ------ For most of the cases ----
uint8_t I2CSendString(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data,uint16_t dataLen);
uint8_t I2CRecvString(uint8_t slaveAddr,uint8_t regAddr,uint8_t *buff,uint16_t buffSize);
void SOFTI2C1_Send_Byte(u8 txd);			
u8 SOFTI2C1_Read_Byte(unsigned char ack);


// -------- Low Layer --------------
void SOFTI2C1_Init(void);         //��ʼ��SOFTI2C1��IO��				 
void SOFTI2C1_Start(void);				//����SOFTI2C1��ʼ�ź�
void SOFTI2C1_Stop(void);	  			//����SOFTI2C1ֹͣ�ź�
u8 SOFTI2C1_Wait_Ack(void); 			//SOFTI2C1�ȴ�ACK�ź�
void SOFTI2C1_Ack(void);					//SOFTI2C1����ACK�ź�
void SOFTI2C1_NAck(void);					//SOFTI2C1������ACK�ź�
u8 SOFTI2C1_Receive_Byte(void);




#endif
















