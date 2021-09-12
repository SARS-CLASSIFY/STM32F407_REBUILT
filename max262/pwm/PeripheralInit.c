//-----------------------------------------------------------------
// ��������: 
//		��Keil uVision4����ƽ̨�»���STM32�ĳ���ģ�壬�����ʼ���Լ�����
// ��    ��: ���ǵ���
// ��ʼ����: 2014-04-24
// �������: 2014-04-24
// �޸�����:
// ��    ��: V1.0
//   - V1.0: ����ģ��
// ���Թ���: ����STM32+FPGA����ϵͳ��ƿ����塢LZE_ST LINK2��2.8��Һ����
// ˵    ��:
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "PeripheralInit.h"
#include "PWMOutput.h"
//-----------------------------------------------------------------
// ���ܳ�����
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void PeripheralInit(void)
//-----------------------------------------------------------------
// ��������: �����ʼ������
// ��ڲ���: �� 
// �� �� ֵ: ��
// ȫ�ֱ���: ��
// ע������: ��
//-----------------------------------------------------------------
void PeripheralInit(void)
{
	TIM2_PWMOutput_Init();
}

void MAX262_GPIO_Init(void)               // MAX262��������
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//ʹ��IO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  | GPIO_Pin_9          // D0, D1 
															| GPIO_Pin_10 | GPIO_Pin_11         // A0, A1
															| GPIO_Pin_12 | GPIO_Pin_13;        // A2, A3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                // �������
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11  | GPIO_Pin_12 ;        // EN, WR 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                // �������
	GPIO_Init(GPIOA,&GPIO_InitStructure);

}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
