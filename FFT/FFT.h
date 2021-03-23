#ifndef __FFT_H
#define __FFT_H
#include "adc.h"
#include "arm_math.h" 
#define TEST_LENGTH_SAMPLES 2048 //FFT����λ ����Ϊ��Ч������*2 �˴���1024��λ��FFT



void FFT(void);
void Generate_Data(void);
void Get_PowerMag_Basicfre(float32_t  *testOutput);
 int Get_FRE(float32_t PowerMag[]);
#endif
