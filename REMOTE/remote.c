#include "remote.h"

/*********引导码发送程序*******************************************************************
** arr： 			 		定时器的重装载值一般设为1000
** pulse: 					百分之X
** TIM_TypeDef* TIMx :		定时器X
** channel :				通道X,一共四个通道,二进制模式开启如0x04=0100，代表开启第二个通道	
******************************************************************************************/
void preamble_code_gen(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel)
{
	set_pulse(TIMx,arr,50,0x08);
	delay_us(9000);//9ms低电平及4.5ms高电平
	close_pwm(TIMx,0x08);
	delay_us(4500);
}

/*********发送0*******************************************************************
** arr： 			 		定时器的重装载值一般设为1000
** pulse: 					百分之X
** TIM_TypeDef* TIMx :		定时器X
** channel :				通道X,一共四个通道,二进制模式开启如0x04=0100，代表开启第二个通道	
******************************************************************************************/
void sent_0(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel)
{
	set_pulse(TIMx,arr,50,0x08);
	delay_us(560);
	close_pwm(TIMx,0x08);
	delay_us(560);
}


/*********发送0*******************************************************************
** arr： 			 		定时器的重装载值一般设为1000
** pulse: 					百分之X
** TIM_TypeDef* TIMx :		定时器X
** channel :				通道X,一共四个通道,二进制模式开启如0x04=0100，代表开启第二个通道	
******************************************************************************************/
void sent_1(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel)
{
	set_pulse(TIMx,arr,50,0x08);
	delay_us(560);
	close_pwm(TIMx,0x08);
	delay_us(1680);
}

/*********发送地址码或命令码*******************************************************************
** arr： 			 		定时器的重装载值一般设为1000
** pulse: 					百分之X
** TIM_TypeDef* TIMx :		定时器X
** channel :				通道X,一共四个通道,二进制模式开启如0x04=0100，代表开启第二个通道
** code	:					8bit的地址码或命令码
******************************************************************************************/
void sent_code(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel,u8 code)
{
	u8 code_cvt = ~code;
	u8 i;
	//发送原码
	for(i = 8;i>0;i--){
		if(code&0x01){
			sent_1(TIMx,arr,pulse,channel);
		}
		else{
			sent_0(TIMx,arr,pulse,channel);			
		}
		code>>=1;//先发送低位
	}
	//发送反码
	for(i = 8;i>0;i--){
		if(code_cvt&0x01){
			sent_1(TIMx,arr,pulse,channel);
		}
		else{
			sent_0(TIMx,arr,pulse,channel);			
		}
		code_cvt>>=1;//先发送低位
	}
}
/*********发送重复码*******************************************************************
** arr： 			 		定时器的重装载值一般设为1000
** pulse: 					百分之X
** TIM_TypeDef* TIMx :		定时器X
** channel :				通道X,一共四个通道,二进制模式开启如0x04=0100，代表开启第二个通道
******************************************************************************************/
void sent_repeat(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel)
{
	set_pulse(TIMx,arr,50,0x08);
	delay_us(9000);//9ms高电平
	close_pwm(TIMx,0x08);
	delay_us(2250);//2.25ms低电平
	set_pulse(TIMx,arr,50,0x08);
	delay_us(560);	
	close_pwm(TIMx,0x08);
	delay_us(98190);//总共110ms
}

/*********数据发送函数*******************************************************************
** arr： 			 		定时器的重装载值一般设为1000
** pulse: 					百分之X
** TIM_TypeDef* TIMx :		定时器X
** channel :				通道X,一共四个通道,二进制模式开启如0x04=0100，代表开启第二个通道
** addr :					地址码
** cmd  ：    				命令码
** repeat_times :			重复次数
******************************************************************************************/
void sent_data(TIM_TypeDef* TIMx,u32 arr,u32 pulse,u8 channel,
			   u8 addr, u8 cmd,u8 repeat_times)
{
	
	preamble_code_gen(TIMx,arr,pulse,channel);//发送引导码
	sent_code(TIMx,arr,pulse,channel,addr);//发送地址码
	sent_code(TIMx,arr,pulse,channel,cmd);//发送cmd
	while(repeat_times>0){
		sent_repeat(TIMx,arr,pulse,channel);
		repeat_times--;
	}
	sent_1(TIMx,arr,pulse,channel);//停止位
}