#include "remote.h"

/*********�����뷢�ͳ���*******************************************************************
** arr�� 			 		��ʱ������װ��ֵһ����Ϊ1000
** pulse: 					�ٷ�֮X
** TIM_TypeDef* TIMx :		��ʱ��X
** channel :				ͨ��X,һ���ĸ�ͨ��,������ģʽ������0x04=0100���������ڶ���ͨ��	
******************************************************************************************/
void preamble_code_gen(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel)
{
	set_pulse(TIMx,arr,50,0x08);
	delay_us(9000);//9ms�͵�ƽ��4.5ms�ߵ�ƽ
	close_pwm(TIMx,0x08);
	delay_us(4500);
}

/*********����0*******************************************************************
** arr�� 			 		��ʱ������װ��ֵһ����Ϊ1000
** pulse: 					�ٷ�֮X
** TIM_TypeDef* TIMx :		��ʱ��X
** channel :				ͨ��X,һ���ĸ�ͨ��,������ģʽ������0x04=0100���������ڶ���ͨ��	
******************************************************************************************/
void sent_0(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel)
{
	set_pulse(TIMx,arr,50,0x08);
	delay_us(560);
	close_pwm(TIMx,0x08);
	delay_us(560);
}


/*********����0*******************************************************************
** arr�� 			 		��ʱ������װ��ֵһ����Ϊ1000
** pulse: 					�ٷ�֮X
** TIM_TypeDef* TIMx :		��ʱ��X
** channel :				ͨ��X,һ���ĸ�ͨ��,������ģʽ������0x04=0100���������ڶ���ͨ��	
******************************************************************************************/
void sent_1(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel)
{
	set_pulse(TIMx,arr,50,0x08);
	delay_us(560);
	close_pwm(TIMx,0x08);
	delay_us(1680);
}

/*********���͵�ַ���������*******************************************************************
** arr�� 			 		��ʱ������װ��ֵһ����Ϊ1000
** pulse: 					�ٷ�֮X
** TIM_TypeDef* TIMx :		��ʱ��X
** channel :				ͨ��X,һ���ĸ�ͨ��,������ģʽ������0x04=0100���������ڶ���ͨ��
** code	:					8bit�ĵ�ַ���������
******************************************************************************************/
void sent_code(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel,u8 code)
{
	u8 code_cvt = ~code;
	u8 i;
	//����ԭ��
	for(i = 8;i>0;i--){
		if(code&0x01){
			sent_1(TIMx,arr,pulse,channel);
		}
		else{
			sent_0(TIMx,arr,pulse,channel);			
		}
		code>>=1;//�ȷ��͵�λ
	}
	//���ͷ���
	for(i = 8;i>0;i--){
		if(code_cvt&0x01){
			sent_1(TIMx,arr,pulse,channel);
		}
		else{
			sent_0(TIMx,arr,pulse,channel);			
		}
		code_cvt>>=1;//�ȷ��͵�λ
	}
}
/*********�����ظ���*******************************************************************
** arr�� 			 		��ʱ������װ��ֵһ����Ϊ1000
** pulse: 					�ٷ�֮X
** TIM_TypeDef* TIMx :		��ʱ��X
** channel :				ͨ��X,һ���ĸ�ͨ��,������ģʽ������0x04=0100���������ڶ���ͨ��
******************************************************************************************/
void sent_repeat(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel)
{
	set_pulse(TIMx,arr,50,0x08);
	delay_us(9000);//9ms�ߵ�ƽ
	close_pwm(TIMx,0x08);
	delay_us(2250);//2.25ms�͵�ƽ
	set_pulse(TIMx,arr,50,0x08);
	delay_us(560);	
	close_pwm(TIMx,0x08);
	delay_us(98190);//�ܹ�110ms
}

/*********���ݷ��ͺ���*******************************************************************
** arr�� 			 		��ʱ������װ��ֵһ����Ϊ1000
** pulse: 					�ٷ�֮X
** TIM_TypeDef* TIMx :		��ʱ��X
** channel :				ͨ��X,һ���ĸ�ͨ��,������ģʽ������0x04=0100���������ڶ���ͨ��
** addr :					��ַ��
** cmd  ��    				������
** repeat_times :			�ظ�����
******************************************************************************************/
void sent_data(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel,
			   u8 addr, u8 cmd,u8 repeat_times)
{
	
	preamble_code_gen(TIMx,arr,pulse,channel);//����������
	sent_code(TIMx,arr,pulse,channel,addr);//���͵�ַ��
	sent_code(TIMx,arr,pulse,channel,cmd);//����cmd
	while(repeat_times>0){
		sent_repeat(TIMx,arr,pulse,channel);
		repeat_times--;
	}
	sent_1(TIMx,arr,pulse,channel);//ֹͣλ
}