#ifndef __OLED1_H
#define __OLED1_H 

#include "sys.h"
#include "stdlib.h"	
 

//-----------------OLED端口定义---------------- 

#define OLED1_SCL_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_2)//SCL
#define OLED1_SCL_Set() GPIO_SetBits(GPIOE,GPIO_Pin_2)

#define OLED1_SDA_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_3)//DIN
#define OLED1_SDA_Set() GPIO_SetBits(GPIOE,GPIO_Pin_3)

#define OLED1_RES_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_4)//RES
#define OLED1_RES_Set() GPIO_SetBits(GPIOD,GPIO_Pin_4)


#define OLED1_CMD  0	//写命令
#define OLED1_DATA 1	//写数据

void OLED1_ClearPoint(u8 x,u8 y);
void OLED1_ColorTurn(u8 i);
void OLED1_DisplayTurn(u8 i);
void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_WaitAck(void);
void Send1_Byte(u8 dat);
void OLED1_WR_Byte(u8 dat,u8 mode);
void OLED1_DisPlay_On(void);
void OLED1_DisPlay_Off(void);
void OLED1_Refresh(void);
void OLED1_Clear(void);
void OLED1_DrawPoint(u8 x,u8 y,u8 t);
void OLED1_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED1_DrawCircle(u8 x,u8 y,u8 r);
void OLED1_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED1_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED1_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED1_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED1_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED1_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED1_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED1_Init(void);



/* ----------PERSONAL USE -------------- */

void MEASURE1_SHOW(u32 fre,u8 pulse,u16 vpp);
u8 JUDGE1_NUM(u32 num);
void OLED1_Clear_Sp(void);
#endif

