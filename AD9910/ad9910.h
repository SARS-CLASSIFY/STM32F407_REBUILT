/*-----------------------------------------------
  名称：AD9854串口驱动
  编写：Liu
  日期：2014.3
  修改：无
  内容：

  *refine
------------------------------------------------*/
#ifndef __AD9910_H__
#define __AD9910_H__

#include "delay.h"
#include "sys.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long int

#define AD9910_PWR PCout(7) 
#define AD9910_PROFILE1 PAout(11)
#define AD9910_DRCTL PCout(11)    
#define AD9910_SDIO PDout(0)
#define AD9910_SCLK PDout(3)  
#define AD9910_CS PDout(5)

#define AD9910_MAS_REST PCout(6) 
#define AD9910_PROFILE2 PAout(10)
#define AD9910_PROFILE0 PAout(12)
#define AD9910_UP_DAT PAout(15)
#define AD9910_DRHOLD PCout(12)

void Init_ad9910(void);
void Freq_convert(ulong Freq);            //写频率
void Amp_convert(unsigned int Amplitude); //写幅度
void Txramdata(int wave_num);			  //任意波形发生，参数：波形序号1-2
void Square_wave(uint Sample_interval);                                                   //方波;
void Sawtooth_wave(uint Sample_interval);                                                 //三角波
void SweepFre(ulong SweepMinFre, ulong SweepMaxFre, ulong SweepStepFre, ulong SweepTime); //扫频：Hz, Hz,Hz,us

void Set_RefCLK_OUT(u8 mode);


void Txcfr(void);

#endif
