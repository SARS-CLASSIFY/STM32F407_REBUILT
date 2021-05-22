#include "stage.h"
#include "oled.h"
#include "martix_key.h"
#include "usart.h"
#include "pwm.h"
#include "dac.h"
#include "timer_capture.h"
u8  state  = 0;//状态机标志位 0->初始界面 1->密码输入状态；->2锁定状态
u8 show_state = 0;//显示状态机页面
extern u32 pwm_fre;
extern u32 my_arr;

extern u32 DAC_VOT;
extern u32 DAC1_FRE;
extern u32 DAC2_FRE;	
extern uint16_t Sine12bit[tableSize];
extern uint16_t Rec12bit[tableSize];
extern uint16_t sawtooth[tableSize];
extern uint16_t Tri_12bit[tableSize];
extern uint16_t sin_out[tableSize];
extern uint16_t tri_out[tableSize];
extern uint16_t rec_out[tableSize];

/*1: 2 3 选择,4确定
	2：2 3 调占空比, 6,7 调频率, 4确定并返回上一级
	


/*------------------------------------------------------------------------------
* brief   : STAGE ONE
* Description    : OLED INIT SHOW
-------------------------------------------------------------------------------*/
void STAGE_ONE(void)
{

	OLED_Init();				//初始化OLED

	OLED_ShowString(0,0,"FUNCTION SELECT",16,1);
	
	OLED_ShowString(0,16,"PWM OUT",16,1);
	
	OLED_ShowString(0,32,"DAC1 OUT",16,1);
	OLED_ShowString(0,49,"DAC2 OUT",16,1);
	
	OLED_Refresh();

}

/*------------------------------------------------------------------------------
* brief   : STAGE ONE 响应函数
-------------------------------------------------------------------------------*/
void STAGE_ONE_RES(void)
{
	u8 key_value = 0;		//按键状态存储变量
	u8 flag = 0;				//按键响应临时变量


	
	
	while(1){
		key_value = MARTIX_KEY_SCAN();
		switch(key_value)
			{
				case 1:	
					flag = (flag + 1)%3;
					break;
				case 2:
					flag = (flag+3-1)%3;
					break;
				case 3:
					state += flag+1;
					flag = 0;
					break;
				case 6:
					
					my_capture();
					break;
				
				default: break;
			}
		//state +1 ~ +3 实行状态转移
		if(state != 0){
//				printf("%d\r\n",state);
				break;
		}
		
		switch(flag)
			{
				case 0:	
					OLED_ShowString(0,16,"PWM OUT",16,0);	
					OLED_ShowString(0,49,"DAC2 OUT",16,1);
					OLED_ShowString(0,32,"DAC1 OUT",16,1);
					OLED_Refresh();
					break;
				case 1:
					OLED_ShowString(0,16,"PWM OUT",16,1);	
					OLED_ShowString(0,49,"DAC2 OUT",16,1);
					OLED_ShowString(0,32,"DAC1 OUT",16,0);
					OLED_Refresh();
					break;
				case 2:
					OLED_ShowString(0,16,"PWM OUT",16,1);	
					OLED_ShowString(0,49,"DAC2 OUT",16,0);
					OLED_ShowString(0,32,"DAC1 OUT",16,1);
					OLED_Refresh();
					break;
				default: break;
			}
	}
}
/*------------------------------------------------------------------------------
* brief   : 状态2显示函数
-------------------------------------------------------------------------------*/
void STAGE_TWO(void)
{
	OLED_Init();				//初始化OLED

	OLED_ShowString(20,0,"PWM SET",16,1);
	
	OLED_ShowString(0,16,"PWM pulse =50 %",16,1);
	
	OLED_ShowString(0,32,"pwm fre = 100KHZ",16,1);

	OLED_Refresh();

}

/*------------------------------------------------------------------------------
* brief   : 状态2相应函数
-------------------------------------------------------------------------------*/
void STAGE_TWO_RES(void)
{
	u8 key_value = 0;		//按键状态存储变量
	static u32 pulse  = 50;
	while(1){
		key_value = MARTIX_KEY_SCAN();
		switch(key_value)
		{
			case 1:	
				pulse = (pulse+5)%95;//步进占空比+5
			
				OLED_ShowString(0,16,"PWM pulse =   %",16,1);
				OLED_ShowNum(90,16,pulse,2,16,1);
				OLED_Refresh();
			
				set_pulse(TIM3,my_arr,pulse,0x01);
				break;
			case 2:
				pulse = (pulse+95-5)%95;//步进占空比-5
				//oled显示
				OLED_ShowString(0,16,"PWM pulse =   %",16,1);
				OLED_ShowNum(90,16,pulse,2,16,1);
				OLED_Refresh();
			
				set_pulse(TIM3,my_arr,pulse,0x01);
				break;
			//返回上一级
		  case 3:
				state = 0;
				break;
			
			case 6:
				pwm_fre = (pwm_fre+500)%26100;//频率调节,步进+500HZ
				my_arr = 1000000/pwm_fre;
			
				TIM3_PWM_Init(my_arr,21-1);//1M/arr = fre
				set_pulse(TIM3,my_arr,pulse,0x01);
				//oled显示
				OLED_ShowString(0,32,"pwm fre =    HZ",16,1);
				OLED_ShowNum(80,48,pwm_fre*4,JUDGE_NUM(pwm_fre*4),16,1);
				OLED_Refresh();
			
				break;
			
			case 7:
				pwm_fre = (pwm_fre-500+26100)%26100;//频率调节,步进-500HZ
				my_arr = 1000000/pwm_fre;
			
				TIM3_PWM_Init(my_arr,21-1);//1M/arr = fre
				set_pulse(TIM3,my_arr,pulse,0x01);
				//oled显示
				OLED_ShowString(0,32,"pwm fre =    HZ",16,1);
				OLED_ShowNum(80,48,pwm_fre*4,JUDGE_NUM(pwm_fre*4),16,1);
				OLED_Refresh();			
				break;
			case 10:
				pwm_fre = 12600;//频率调节,步进-500HZ
				my_arr = 1000000/pwm_fre;
			
				TIM3_PWM_Init(my_arr,21-1);//1M/arr = fre
				set_pulse(TIM3,my_arr,pulse,0x01);
				//oled显示
				OLED_ShowString(0,32,"pwm fre 50k   HZ",16,1);
//				OLED_ShowNum(80,48,pwm_fre,JUDGE_NUM(50000),16,1);
				OLED_Clear_Sp1();
				OLED_Refresh();			
				break;
			
			
				case 11:
				pwm_fre = 25500;//频率调节,步进-500HZ
				my_arr = 1000000/pwm_fre;
			
				TIM3_PWM_Init(my_arr,21-1);//1M/arr = fre
				set_pulse(TIM3,my_arr,pulse,0x01);
				//oled显示
				OLED_ShowString(0,32,"pwm fre =100 kHZ",16,1);
				OLED_Clear_Sp1();
//				OLED_ShowNum(80,48,pwm_fre,JUDGE_NUM(100000),16,1);
				OLED_Refresh();			
				break;
			
			default: break;
		}		
		if(state != 1){
				break;
		}
		
	}
	
}

/*------------------------------------------------------------------------------
* brief   : 状态3对应显示函数
-------------------------------------------------------------------------------*/
void STAGE_THREE(void)
{
	OLED_Init();				//初始化OLED

	OLED_ShowString(20,0,"DAC1 SET",16,1);
	
	OLED_ShowString(0,16,"DAC VPP = 2500mv",16,1);
	
	OLED_ShowString(0,32,"DAC1 fre = 50 HZ",16,1);

	OLED_Refresh();

}

/*------------------------------------------------------------------------------
* brief   : 状态3响应函数
-------------------------------------------------------------------------------*/
void STAGE_THREE_RES(void)
{
	u8 key_value = 0;		//按键状态存储变量
	while(1){
		key_value = MARTIX_KEY_SCAN();
		switch(key_value)
		{
			case 1:	
				DAC_VOT = (DAC_VOT+100)%3300;//幅值+100mv
			
				OLED_ShowString(0,16,"DAC VPP =     mv",16,1);
				OLED_ShowNum(75,16,DAC_VOT,JUDGE_NUM(DAC_VOT),16,1);
				OLED_Refresh();
				set_vol(DAC_VOT,sin_out,tri_out);
				break;
			case 2:
				DAC_VOT = (DAC_VOT-100+3300)%3300;//幅值-100mv
			
				OLED_ShowString(0,16,"DAC VPP =     mv",16,1);
				OLED_ShowNum(75,16,DAC_VOT,JUDGE_NUM(DAC_VOT),16,1);
				OLED_Refresh();
				set_vol(DAC_VOT,sin_out,tri_out);
				break;
			//返回上一级
		  case 3:
				state = 0;
				break;
			
			case 6:
				
			  DAC1_FRE = (DAC1_FRE+500)%27000;//频率+500
				OLED_ShowString(0,32,"DAC1 fre =    HZ",16,1);
				OLED_Clear_Sp1();
				OLED_ShowNum(80,48,DAC1_FRE,JUDGE_NUM(DAC1_FRE),16,1);
				OLED_Refresh();
			
				DAC1_TIM_Config(DAC1_FRE);
			
				break;
			
			case 7:
			  DAC1_FRE = (DAC1_FRE-500+27000)%27000;//频率-500
				OLED_ShowString(0,32,"DAC1 fre =    HZ",16,1);
				OLED_Clear_Sp1();
				OLED_ShowNum(80,48,DAC1_FRE,JUDGE_NUM(DAC1_FRE),16,1);
				OLED_Refresh();
			
				DAC1_TIM_Config(DAC1_FRE);
				break;
			
			
			case 10://100K
			  DAC1_FRE = 25000;//频率-500
				OLED_ShowString(0,32,"DAC1 fre =100KHZ",16,1);
//				OLED_Clear_Sp1();
//				OLED_ShowNum(80,48,DAC1_FRE,JUDGE_NUM(DAC1_FRE),16,1);
				OLED_Refresh();
			
				DAC1_TIM_Config(DAC1_FRE);
				break;
			
			
			case 11://50K
			  DAC1_FRE = 12621;//频率-500
				OLED_ShowString(0,32,"DAC1 fre =100KHZ",16,1);
//				OLED_Clear_Sp1();
//				OLED_ShowNum(80,48,DAC1_FRE,JUDGE_NUM(DAC1_FRE),16,1);
				OLED_Refresh();
			
				DAC1_TIM_Config(DAC1_FRE);
				break;
			
			default: break;
		}		
		if(state != 2){
				break;
		}
	
	}


}

/*------------------------------------------------------------------------------
* brief   : 状态4对应显示函数
-------------------------------------------------------------------------------*/
void STAGE_FOUR(void)
{
	OLED_Init();				//初始化OLED

	OLED_ShowString(20,0,"DAC2 SET",16,1);
	
	OLED_ShowString(0,16,"DAC VPP = 2500mv",16,1);
	
	OLED_ShowString(0,32,"DAC2 fre = 1000 HZ",16,1);

	OLED_Refresh();

}

/*------------------------------------------------------------------------------
* brief   : 状态4响应函数
-------------------------------------------------------------------------------*/
void STAGE_FOUR_RES(void)
{
	u8 key_value = 0;		//按键状态存储变量
	while(1){
		key_value = MARTIX_KEY_SCAN();
		switch(key_value)
		{
			case 1:	
				DAC_VOT = (DAC_VOT+100)%3300;//幅值+100mv
			
				OLED_ShowString(0,16,"DAC VPP =     mv",16,1);
				OLED_ShowNum(75,16,DAC_VOT,JUDGE_NUM(DAC_VOT),16,1);
				OLED_Refresh();
				set_vol(DAC_VOT,sin_out,tri_out);
				break;
			case 2:
				DAC_VOT = (DAC_VOT-100+3300)%3300;//幅值-100mv
			
				OLED_ShowString(0,16,"DAC VPP =     mv",16,1);
				OLED_ShowNum(75,16,DAC_VOT,JUDGE_NUM(DAC_VOT),16,1);
				OLED_Refresh();
				set_vol(DAC_VOT,sin_out,tri_out);
				break;
			//返回上一级
		  case 3:
				state = 0;
				break;
			
			case 6:
				
			  DAC2_FRE = (DAC2_FRE+500)%27000;//频率+500
				OLED_ShowString(0,32,"DAC2 fre =    HZ",16,1);
				OLED_Clear_Sp1();
				OLED_ShowNum(80,48,DAC2_FRE,JUDGE_NUM(DAC2_FRE),16,1);
				OLED_Refresh();
			
				DAC2_TIM_Config(DAC2_FRE);
			
				break;
			
			case 7:
			  DAC2_FRE = (DAC2_FRE-500+27000)%27000;//频率-500
				OLED_ShowString(0,32,"DAC2 fre =    HZ",16,1);
				OLED_Clear_Sp1();
				OLED_ShowNum(80,48,DAC2_FRE,JUDGE_NUM(DAC2_FRE),16,1);
				OLED_Refresh();
			
				DAC2_TIM_Config(DAC2_FRE);
				break;
			
			case 10:
			  DAC2_FRE = 5000;//频率-500
				OLED_ShowString(0,32,"DAC2 fre = 20kHZ",16,1);
//				OLED_Clear_Sp1();
//				OLED_ShowNum(80,48,DAC2_FRE,JUDGE_NUM(DAC2_FRE),16,1);
				OLED_Refresh();
			
				DAC2_TIM_Config(DAC2_FRE);
				break;
			case 11:
			  DAC2_FRE = 2500;//频率-500
				OLED_ShowString(0,32,"DAC2 fre = 20kHZ",16,1);
//				OLED_Clear_Sp1();
//				OLED_ShowNum(80,48,DAC2_FRE,JUDGE_NUM(DAC2_FRE),16,1);
				OLED_Refresh();
			
				DAC2_TIM_Config(DAC2_FRE);
				break;
			
			
			default: break;
		}		
		if(state != 3){
				break;
		}
	
	}


}

/*------------------------------------------------------------------------------
* brief   : 状态机转移显示函数
-------------------------------------------------------------------------------*/

void STATE_CHANGE(void){
	
	if(show_state!=state){
		switch(state)
		{
			case 0:STAGE_ONE(); show_state = state ; break;
			case 1:STAGE_TWO(); show_state = state ; break;
			case 2:STAGE_THREE();show_state = state ; break;
			case 3:STAGE_FOUR(); show_state = state; break;
//			case 4:enter_to_change();show_state = state; break;
//			case 5:WIFI_SHOW();show_state = state;break;
			default: STAGE_ONE(); show_state = state ; break;
		}
	}
}

/*------------------------------------------------------------------------------
* brief   : 状态机转移执行函数
-------------------------------------------------------------------------------*/
void STATE_RES_CHANGE(void)
{
	switch(state)
	{
		case 0: STAGE_ONE_RES();  break;
		case 1:	STAGE_TWO_RES(); break;
		case 2: STAGE_THREE_RES();break;
		case 3: STAGE_FOUR_RES();break;
//		case 4: change_password_res();break;
//		case 5: WIFI_RES();break;
		default:STAGE_ONE_RES();  break;
	}


}
