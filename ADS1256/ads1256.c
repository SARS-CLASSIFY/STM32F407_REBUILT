//-----------------------------------------------------------------
// ��������:
// ADS1256��ͨ��8·
// 		
//-----------------------------------------------------------------
#include "stm32f4xx.h"
#include "ads1256.h"
#include "delay.h"
#include "delay_ns.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"

/*-----------------------ADC��������--------------------------------*/
int32_t adc[8];									// �������
int32_t adc_1[8];								
float volt[8]={0,0,0,0,0,0,0,0};									// ʵ�ʵ�ѹֵ
uint8_t i;
uint8_t mode;																			// ɨ��ģʽ, ���˻��߲��
uint8_t ch_num;																		// ͨ����

/*********************************************************************************************************
*	�� �� ��: ADS1256_GPIO_Init
*	����˵��: ADC GPIO��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOF Periph clock enable */ 		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//?????SYSCFG??	
	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	


	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	RST_L;		// ���͸�λ����
	delay_ms(1);
	RST_H;		// ���߸�λ����
	delay_ms(100);
	CS_H;			// ����Ƭѡ����
	SCLK_L;		// ����ʱ������
	DIN_H;		// ������������
}
/*
*********************************************************************************************************
*	�� �� ��: ADS1256_Send8Bit
*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ADS1256_Send8Bit(uint8_t data)
{
	uint8_t i;
	/* �������Ͷ���ֽ�ʱ����Ҫ�ӳ�һ�� */
	delay_us(1);	
	/*��ADS1256 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
	for(i = 0; i < 8; i++)
	{
		if (data & 0x80)
		{
			DIN_H;
		}
		else
		{
			DIN_L;
		}
		SCLK_H;
		delay_us(1);
		data <<= 1;
		SCLK_L;			/* <----  ADS1256 ����SCK�½��ز���DIN����, ���ݱ���ά�� 50nS */
	  delay_us(1);	
	}
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_Recive8Bit
*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************/
uint8_t ADS1256_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;
	Delay_ns(250);
	Delay_ns(250);
	Delay_ns(250);
	Delay_ns(250);
	/*��ADS1256 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
	for (i = 0; i < 8; i++)
	{
		SCLK_H;
		Delay_ns(250);
		Delay_ns(250);
		read = read<<1;
		SCLK_L;
		if (DO_IS_H)
		{
			read++;
		}
		Delay_ns(250);
		Delay_ns(250);
	}
	return read;

}

/*********************************************************************************************************
*	�� �� ��: ADS1256_WriteReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_WriteReg(uint8_t RegID, uint8_t RegValue)
{
	CS_L;	/* SPIƬѡ = 0 */
	ADS1256_Send8Bit(CMD_WREG | RegID);	/* д�Ĵ���������, �����ͼĴ�����ַ */
	ADS1256_Send8Bit(0x00);		/* �Ĵ������� - 1, �˴�д1���Ĵ��� */
	ADS1256_Send8Bit(RegValue);	/* ���ͼĴ���ֵ */
	CS_H;	/* SPIƬѡ = 1 */
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_ReadReg
*	����˵��: ��ָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ��
*	�� �� ֵ: �����ļĴ���ֵ��
*********************************************************************************************************/
uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_L;	/* SPIƬѡ = 0 */
	ADS1256_Send8Bit(CMD_RREG | _RegID);	/* д�Ĵ���������, �����ͼĴ�����ַ */
	ADS1256_Send8Bit(0x00);	/* �Ĵ������� - 1, �˴���1���Ĵ��� */
	delay_us(1); 	/* �����ӳٲ��ܶ�ȡоƬ�������� */
	read = ADS1256_Recive8Bit();	/* ���Ĵ���ֵ */
	CS_H;	/* SPIƬѡ = 1 */

	return read;
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_WriteCmd
*	����˵��: ���͵��ֽ�����
*	��    ��:  _cmd : ����
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_WriteCmd(uint8_t cmd)
{
	CS_L;	/* SPIƬѡ = 0 */
	ADS1256_Send8Bit(cmd);
	CS_H;	/* SPIƬѡ = 1 */
}

//-----------------------------------------------------------------
// void ADS1256_CfgADC(unsigned char gain,unsigned char drate)
//-----------------------------------------------------------------
// ��������: ����ADC���������漰ת������
// ��ڲ���: ���棬����
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: void ADS1256_WriteCmd(unsigned char cmd) 
//           void ADS1256_WriteReg(unsigned char Reg_ID,unsigned char Reg_Date)
//           void ADS1256_Send8Bit(unsigned char date)
// ע������: 
//-----------------------------------------------------------------
void ADS1256_CfgADC(uint8_t gain,uint8_t drate)   // ��ʼ�����ã����������Լ�ת������
{
	ADS1256_WriteCmd(CMD_RESET);                // д��λָ��
	ADS1256_WriteReg(REG_STATUS,0XF4);          // д״̬�����ݴ���Ĭ�ϸ�λ��ǰ��������������ֹʹ�û���
	ADS1256_WriteCmd(CMD_SELFCAL);              // ��У׼
	delay_us(20);
	{
		uint8_t buf[4];
		/* ״̬�Ĵ�������
			Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)

			Bit 3 ORDER: Data Output Bit Order
				0 = Most Significant Bit First (default)
				1 = Least Significant Bit First
			Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
			byte first. The ORDER bit only controls the bit order of the output data within the byte.

			Bit 2 ACAL : Auto-Calibration
				0 = Auto-Calibration Disabled (default)
				1 = Auto-Calibration Enabled
			When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
			the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
			values.

			Bit 1 BUFEN: Analog Input Buffer Enable
				0 = Buffer Disabled (default)
				1 = Buffer Enabled

			Bit 0 DRDY :  Data Ready (Read Only)
				This bit duplicates the state of the DRDY pin.

			ACAL=1ʹ����У׼���ܡ��� PGA��BUFEEN, DRATE�ı�ʱ��������У׼
		*/
		buf[0] = (0 << 3) | (1 << 2) | (1 << 1);
		
		/* ����λ0��ʾAINP�� AIN0,  ����λ8��ʾ AINN �̶��� AINCOM */
		buf[1] = 0x08;                      //ͨ������ѡ��
		
		/*	ADCON: A/D Control Register (Address 02h)
			Bit 7 Reserved, always 0 (Read Only)
			Bits 6-5 CLK1, CLK0 : D0/CLKOUT Clock Out Rate Setting
				00 = Clock Out OFF
				01 = Clock Out Frequency = fCLKIN (default)
				10 = Clock Out Frequency = fCLKIN/2
				11 = Clock Out Frequency = fCLKIN/4
				When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.

			Bits 4-2 SDCS1, SCDS0: Sensor Detect Current Sources
				00 = Sensor Detect OFF (default)
				01 = Sensor Detect Current = 0.5 �� A
				10 = Sensor Detect Current = 2 �� A
				11 = Sensor Detect Current = 10�� A
				The Sensor Detect Current Sources can be activated to verify  the integrity of an external sensor supplying a signal to the
				ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.

			Bits 2-0 PGA2, PGA1, PGA0: Programmable Gain Amplifier Setting
				000 = 1 (default)
				001 = 2
				010 = 4
				011 = 8
				100 = 16
				101 = 32
				110 = 64
				111 = 64
		*/
		buf[2] = (0 << 5) | (0 << 3) | (gain << 0);
		
		buf[3] = drate;	// DRATE_10SPS;	/* ѡ������������� */
		CS_L;
		ADS1256_Send8Bit(CMD_WREG|0);          // д�Ĵ���
		ADS1256_Send8Bit(0x03);                // ����д��4������
		ADS1256_Send8Bit(buf[0]);              // ����״̬�Ĵ���
		ADS1256_Send8Bit(buf[1]);              // ��������ͨ������
		ADS1256_Send8Bit(buf[2]);              // ����ADCON���ƼĴ���������
		ADS1256_Send8Bit(buf[3]);	             // ������������
		CS_H;
	}
	delay_us(50);
}

//-----------------------------------------------------------------
// unsigned long ADS1256_GetAdc(unsigned char channel)
//-----------------------------------------------------------------
// ��������: ��ȡADC�Ĳ������
// ��ڲ���: ��
// ���ز���: ADC�������
// ȫ�ֱ���: ��
// ����ģ��: void ADS1256_WriteCmd(unsigned char cmd) 
//           void ADS1256_WriteReg(unsigned char Reg_ID,unsigned char Reg_Date)
//           void ADS1256_Send8Bit(unsigned char date)
// ע������: 
//-----------------------------------------------------------------
uint32_t ADS1256_GetAdc(uint8_t channel)
{
	uint32_t read;
	read = 0;
	/**/
// 	while(DRDY);           //��DRDY��Ϊ�͵�ƽʱ�����ݿ�ʼ����
	ADS1256_WriteReg(REG_MUX,channel);       // д���ȡ��ͨ��
	ADS1256_WriteCmd(CMD_SYNC);              //ͬ��A/Dת������
// 	Delay_1us(1);
	ADS1256_WriteCmd(CMD_WAKEUP);            //���SYNC���˳�����ģʽ
	while(!DRDY_IS_L);                             // �ȴ�����ת�����
	
	CS_L;                                    //Ƭѡ����
	ADS1256_Send8Bit(CMD_RDATA);             //��ȡ��������
	
	delay_us(1);                
	 
	//��������3�����ݣ����ֽ���ǰ
	read = ((uint32_t)ADS1256_Recive8Bit() << 16);          
	read +=( (uint32_t)ADS1256_Recive8Bit() << 8);
	read += ADS1256_Recive8Bit() ;
	CS_H;
//  		read=0x678965;
	return read;	
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------




/*------------------------------------------------------------------------------
* brief   : personal using code
-------------------------------------------------------------------------------*/
void ADS1256_INIT(void)
{
	ADS1256_GPIO_Init();						// ADC GPIO��ʼ��
	ADS1256_CfgADC(PGA_1, DATARATE_1K);	// ����ADC������ ����1:1, ����������� 15Hz  	
}


/*------------------------------------------------------------------------------
* brief   : ����ģʽ,����洢��volt������
* ch_num	: ͨ����Ŀ
-------------------------------------------------------------------------------*/
void ADS1256_TRANS_SIGLE(uint8_t ch_num)
{
//		for (i = 0; i < ch_num; i++)
//		{
//			adc[i] = ADS1256_GetAdc(i<<4 | 0x08);	 	// ��ȫ�ֻ�������ȡ��������� ������������жϷ�������ж�ȡ��				
//			adc_1[i] = (adc[i] ^ 0x800000);				// 4194303 = 2.5V , ��������ֵ��ʵ�ʿ��Ը���2.5V��׼��ʵ��ֵ���й�ʽ���� 
//			volt[i] = (((0.596047*adc_1[i])-5000000)/1000000);  // ������������v����λ
//		}	


			adc[ch_num-1] = ADS1256_GetAdc((ch_num-1)<<4 | 0x08);	 	// ��ȫ�ֻ�������ȡ��������� ������������жϷ�������ж�ȡ��				
			adc_1[ch_num-1] = (adc[ch_num-1] ^ 0x800000);				// 4194303 = 2.5V , ��������ֵ��ʵ�ʿ��Ը���2.5V��׼��ʵ��ֵ���й�ʽ���� 
			volt[ch_num-1] = (((0.596047*adc_1[ch_num-1])-5000000)/1000000);  // ������������v����λ
	
}

/*------------------------------------------------------------------------------
* brief   : ���ģʽ ����4·
-------------------------------------------------------------------------------*/
void ADS1256_TRANS_DIFF(uint8_t ch_num)
{
//		for (i = 0; i < ch_num; i++)
//		{				
//			adc[i] = ADS1256_GetAdc((2*i)<<4 | 2*i+1);	 	// ��ȫ�ֻ�������ȡ��������� ������������жϷ�������ж�ȡ��			
//			adc_1[i] = (adc[i] ^ 0x800000);				// 4194303 = 2.5V , ��������ֵ��ʵ�ʿ��Ը���2.5V��׼��ʵ��ֵ���й�ʽ���� 		
//			volt[i] = (((0.596047*adc_1[i])-5000000)/1000000);  // ������������v����λ
//		}				
			adc[ch_num-1] = ADS1256_GetAdc((2*(ch_num-1))<<4 | 2*i+1);	 	// ��ȫ�ֻ�������ȡ��������� ������������жϷ�������ж�ȡ��			
			adc_1[ch_num-1] = (adc[ch_num-1] ^ 0x800000);				// 4194303 = 2.5V , ��������ֵ��ʵ�ʿ��Ը���2.5V��׼��ʵ��ֵ���й�ʽ���� 		
			volt[ch_num-1] = (((0.596047*adc_1[ch_num-1])-5000000)/1000000);  // ������������v����λ
}

/*------------------------------------------------------------------------------
* brief   : ��ֵ�˲�
* times		: ȡ��ֵ����
-------------------------------------------------------------------------------*/
float ADS1256_TRANS_EQUAL(uint8_t ch_num, uint8_t times)
{
		uint8_t j;	
		float voltage = 0;
		for(j = 0;j<times;j++){
			ADS1256_TRANS_SIGLE(ch_num);
			voltage += volt[ch_num-1];
		}		
		return voltage/times;
}

/*------------------------------------------------------------------------------
* brief   : ���ģʽ��ֵ�˲� 1-2ͨ��Ϊһ�� ��ֵΪ1ͨ��-2ͨ��
* times		: ��ֵ�˲�����
-------------------------------------------------------------------------------*/
float ADS1256_DIFF_EQUAL(uint8_t ch_num,uint8_t times)
{
		uint8_t j;	
		float voltage = 0;
		for(j = 0;j<times;j++){
			ADS1256_TRANS_DIFF(ch_num);
			voltage += volt[ch_num-1];
		}		
		return voltage/times;

}	

