#ifndef __FDACOEFS_H
#define __FDACOEFS_H	
#include "sys.h" 

void iir_biquad_stm32(float *y, float *x, float *IIRCoeff, u16 ny);
void iir_biquad_stm32_x(float *y, float *x, float *IIRCoeff, u16 ny);
#endif
