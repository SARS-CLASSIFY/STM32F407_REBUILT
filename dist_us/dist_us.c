#include "dist_us.h"
#include "usart.h"


extern u8 TIM5CH1_CAPTURE_STA; //capture state

/*-----------------------   brief   : DIST_US SET  ----------------------------*/
int8_t Dist_US_state=-1;				//-1 :wait ;0 :measure
uint32_t Time_Echo_us = 0;
/*-----------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
* brief   : GPIO SET GPIO4->Tirg	
-------------------------------------------------------------------------------*/
void Dist_GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//GPIO_Clock_Enable			

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 										
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
		GPIO_Init(GPIOA,&GPIO_InitStructure); 								
}


/*------------------------------------------------------------------------------
* brief   : 			 dist_us start
* ReturnValue    : the distance measured
-------------------------------------------------------------------------------*/
uint32_t Dist_US_Start(void)
{
	
  uint8_t i;
  uint32_t Dist_US_distI = 0;
	
  
	Dist_GPIO_Config();
	
	//reset
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	delay_us(40);

  //trig
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
  delay_us(15);
  GPIO_ResetBits(GPIOA,GPIO_Pin_4);
  
  //capture echo
	if(Dist_US_state == -1){
		TIM5_CH1_Cap_Init(0XFFFFFFFF,42-1); //1MHZ COUNT
		Dist_US_state=0;		
	}
	
	if(TIM5CH1_CAPTURE_STA&0X80){					//get pulse
		Time_Echo_us = TIM_CAP_READ();
		

		Dist_US_distI = Time_Echo_us*340/2000;

		

		printf("dis = %d mm\r\n",Dist_US_distI);		//ADJUST
		
		Time_Echo_us = 0;

		Dist_US_state = -1;
	}
	
  for(i=0;i<85;i++)			//85ms as the max wait time
  {
    delay_ms(1);
    if(Dist_US_state==-1)
      break;
  }
	
	return Dist_US_distI;
}




