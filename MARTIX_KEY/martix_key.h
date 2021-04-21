#ifndef _MARTIX_KEY_H
#define _MARTIX_KEY_H
#include "sys.h"
/* ----------------------- GPIO DEFINE ---------------------- */
#define KEY_col0_Pin GPIO_Pin_0
#define KEY_col0_GPIO_Port GPIOD
#define KEY_col1_Pin GPIO_Pin_1
#define KEY_col1_GPIO_Port GPIOD
#define KEY_col2_Pin GPIO_Pin_2
#define KEY_col2_GPIO_Port GPIOD
#define KEY_col3_Pin GPIO_Pin_3
#define KEY_col3_GPIO_Port GPIOD
#define KEY_row0_Pin GPIO_Pin_4
#define KEY_row0_GPIO_Port GPIOD
#define KEY_row1_Pin GPIO_Pin_5
#define KEY_row1_GPIO_Port GPIOD
#define KEY_row2_Pin GPIO_Pin_6
#define KEY_row2_GPIO_Port GPIOD
#define KEY_row3_Pin GPIO_Pin_7
#define KEY_row3_GPIO_Port GPIOD

/*-------------------  FUNCTIONS  ------------------------ */
void martix_key_init(void);
char MARTIX_KEY_SCAN(void);
char KEY_ROW_SCAN(void);

/* ------------------  GPIO DEFINE ----------------------- */
#define KEY_CLO0_OUT_LOW  GPIO_ResetBits(GPIOD,GPIO_Pin_0)
#define KEY_CLO1_OUT_LOW  GPIO_ResetBits(GPIOD,GPIO_Pin_1)
#define KEY_CLO2_OUT_LOW  GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define KEY_CLO3_OUT_LOW  GPIO_ResetBits(GPIOD,GPIO_Pin_3)

#define KEY_CLO0_OUT_HIGH  GPIO_SetBits(GPIOD,GPIO_Pin_0)
#define KEY_CLO1_OUT_HIGH  GPIO_SetBits(GPIOD,GPIO_Pin_1)
#define KEY_CLO2_OUT_HIGH  GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define KEY_CLO3_OUT_HIGH  GPIO_SetBits(GPIOD,GPIO_Pin_3)

#endif
