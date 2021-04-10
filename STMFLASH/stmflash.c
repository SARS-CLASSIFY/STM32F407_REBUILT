#include "stmflash.h"
#include "delay.h"
#include "usart.h" 



 

u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}  
/*------------------------------------------------------------------------------
 * brief   : ��ȡָ����ַ�İ���(16λ����) 
 * Description    : string recive
 * EntryParameter : addr:����ַ 
 * ReturnValue    : 0~11,��addr���ڵ�����
 -------------------------------------------------------------------------------*/ 
uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}

/*------------------------------------------------------------------------------
* brief   : ��ָ����ַ��ʼд��ָ�����ȵ�����
* EntryParameter : WriteAddr:��ʼ��ַ	pBuffer:����ָ�� 
*	NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.)
* ReturnValue    : NONE
* Attention	:	 ��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
							 д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
							 д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
							 û����Ҫ����,��������������Ȳ�����,Ȼ����������д. 
							 �ú�����OTP����Ҳ��Ч!��������дOTP��!
							 OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F
-------------------------------------------------------------------------------*/

void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
  FLASH_Status status = FLASH_COMPLETE;
	u32 addrx=0;
	u32 endaddr=0;	
  if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//�Ƿ���ַ
	FLASH_Unlock();									//���� 
  FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
 		
	addrx=WriteAddr;				//д�����ʼ��ַ
	endaddr=WriteAddr+NumToWrite*4;	//д��Ľ�����ַ
	if(addrx<0X1FFF0000)			//ֻ�����洢��,����Ҫִ�в�������!!
	{
		while(addrx<endaddr)		//ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
			{   
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				if(status!=FLASH_COMPLETE)break;	//����������
			}else addrx+=4;
		} 
	}
	if(status==FLASH_COMPLETE)
	{
		while(WriteAddr<endaddr)//д����
		{
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//д������
			{ 
				break;	//д���쳣
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
  FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
} 

/*------------------------------------------------------------------------------
* brief   : ��ָ����ַ��ʼ����ָ�����ȵ�����
* EntryParameter : ReadAddr:��ʼ��ַ	pBuffer:����ָ��
* ReturnValue    : NONE
-------------------------------------------------------------------------------*/

void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;//ƫ��4���ֽ�.	
	}
}


/*-------------------  * brief   : uint_16 part  -------------------------*/


/*------------------------------------------------------------------------------
* brief   : ��ָ����ַ��ʼ�������ֽڵ�����
* EntryParameter : address ���ݵ�ַ
* ReturnValue    : NONE
-------------------------------------------------------------------------------*/
uint16_t FLASH_ReadHalfWord(uint32_t address)
{
 return *(__IO uint16_t*)address; 
}


/*------------------------------------------------------------------------------
* brief   : ��ָ����ַ��ʼ����2�ֽڵ�����
* EntryParameter : startAddress ���ݵ�ַ	readData�����ݴ��ַ countToRead��ȡ������
* ReturnValue    : NONE
-------------------------------------------------------------------------------*/
void FLASH_ReadData_Uint16(uint32_t startAddress,uint16_t *readData,uint16_t countToRead)
{
		uint16_t dataIndex;
		for(dataIndex=0;dataIndex<countToRead;dataIndex++)
		{
			readData[dataIndex]=FLASH_ReadHalfWord(startAddress+dataIndex*2);
		}
}

/*------------------------------------------------------------------------------
* brief   : ��ָ����ַд��2�ֽڵ�����
* EntryParameter : startAddress ���ݵ�ַ	readData�����ݴ��ַ countToRead��ȡ������
* ReturnValue    : NONE
-------------------------------------------------------------------------------*/
void STMFLASH_Write_Uint16(u32 WriteAddr,u16 *pBuffer,u32 NumToWrite)	
{ 
  FLASH_Status status = FLASH_COMPLETE;
	u32 addrx=0;
	u32 endaddr=0;	
  if(WriteAddr<STM32_FLASH_BASE||WriteAddr%2)return;	//�Ƿ���ַ
	FLASH_Unlock();									//���� 
  FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
 		
	addrx=WriteAddr;				//д�����ʼ��ַ
	endaddr=WriteAddr+NumToWrite*2;	//д��Ľ�����ַ
	if(addrx<0X1FFF0000)			//ֻ�����洢��,����Ҫִ�в�������!!
	{
		while(addrx<endaddr)		//ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
			{   
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				if(status!=FLASH_COMPLETE)break;	//����������
			}else addrx+=2;
		} 
	}
	if(status==FLASH_COMPLETE)
	{
		while(WriteAddr<endaddr)//д����
		{
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//д������
			{ 
				break;	//д���쳣
			}
			WriteAddr+=2;
			pBuffer++;
		} 
	}
  FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
}












