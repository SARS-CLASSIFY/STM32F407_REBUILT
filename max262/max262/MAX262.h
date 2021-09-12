//-----------------------------------------------------------------
// MAX262����ͷ�ļ�
// ͷ�ļ���: MAX262.h
// ��    ��: ���ǵ���
// ��дʱ��: 2014-01-28
// �޸�����:
//-----------------------------------------------------------------

#ifndef _MAX262_H
#define _MAX262_H

//-----------------------------------------------------------------
// �궨��
//-----------------------------------------------------------------
#define	D0				(GPIOB,GPIO_Pin_8)
#define D0_L			GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define D0_H			GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define	D1				(GPIOB,GPIO_Pin_9)
#define	D1_L			GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define	D1_H			GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define	A0		    (GPIOB,GPIO_Pin_10)
#define	A0_L	    GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define	A0_H	    GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define A0_IS_L	  (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == Bit_RESET)

#define	A1		    (GPIOB,GPIO_Pin_11)
#define	A1_L	    GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define	A1_H	    GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define A1_IS_H		(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == Bit_SET)

#define	A2				(GPIOB,GPIO_Pin_12)
#define	A2_L	    GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define	A2_H	    GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define	A3		    (GPIOB,GPIO_Pin_13)
#define	A3_L	    GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define	A3_H	    GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define	LE		    (GPIOA,GPIO_Pin_11)
#define	LE_L	    GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define	LE_H	    GPIO_SetBits(GPIOA,GPIO_Pin_11)

#define	WR		    (GPIOA,GPIO_Pin_12)
#define	WR_L	    GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define	WR_H	    GPIO_SetBits(GPIOA,GPIO_Pin_12)

// #define Fn   63   //139.8kʱ��Ƶ��
#define NF1 139.8
#define NF2 40.48


//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern void Filter1(u8 mode,float q);
extern void Filter2(u8 mode,float q);
// extern u8 Fn(float f);
extern u8 Qn(float q);
#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
