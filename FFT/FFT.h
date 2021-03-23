#ifndef __FFT_H
#define __FFT_H
#include "adc.h"
#include "arm_math.h" 
#define TEST_LENGTH_SAMPLES 2048 //FFT数据位 定义为有效数据数*2 此处做1024点位的FFT



void FFT(void);
void Generate_Data(void);
void Get_PowerMag_Basicfre(float32_t  *testOutput);
 int Get_FRE(float32_t PowerMag[]);
#endif
