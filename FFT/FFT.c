#include "arm_math.h" 
#include "FFT.h"


#define Fs 84000 //����Ƶ��
#define mode 0   //mode 0 ADC��������  mode 1 ��������
#define resolution Fs/fftSize //��λ�����ķֱ���

/* ------------------------------------------------------------------- 
* External Input and Output buffer Declarations for FFT Bin Example 
* ------------------------------------------------------------------- */ 
extern uint16_t ADC_DualConvertedValueTab[2048];			//�ⲿ�ź�����
float32_t ADC3ConvertedValue1[TEST_LENGTH_SAMPLES]={0}; //FFT ����
float32_t Output_Stageone[TEST_LENGTH_SAMPLES] = {0};//��ʱFFT���ݴ洢
float32_t  testOutput[TEST_LENGTH_SAMPLES/2];     //FFT ���
float32_t PowerMag[TEST_LENGTH_SAMPLES/4];
long FreMag[TEST_LENGTH_SAMPLES/4];

static float32_t Inputbuf[TEST_LENGTH_SAMPLES];					//ģ�������ź�����

//  
/* ------------------------------------------------------------------ 
* Global variables for FFT Bin Example 
* ------------------------------------------------------------------- */ 
uint32_t fftSize = 1024;  			//FFT����
uint32_t ifftFlag = 0; 					//Ĭ�����任
uint32_t doBitReverse = 1; 			
/* ---------------------------------------------------------------------- 
* Max magnitude FFT Bin test 
* ------------------------------------------------------------------- */ 
 
void FFT(void) 
{ 
  unsigned int ii=0;
  
	arm_status status; 
  arm_rfft_fast_instance_f32 S;  
   
  status = ARM_MATH_SUCCESS; 
	
	/* Initialize the CFFT/CIFFT module */  
  status = arm_rfft_fast_init_f32(&S, fftSize); 
	/* ------------------�����ź�---------------------------------------*/

	if(mode){
		Generate_Data();
		for(ii=0;ii<TEST_LENGTH_SAMPLES;ii++){
			ADC3ConvertedValue1[ii] = (float32_t)Inputbuf[ii];
		}
	}
	else{
		for(ii=0;ii<TEST_LENGTH_SAMPLES;ii++){
			ADC3ConvertedValue1[ii] = (float32_t)ADC_DualConvertedValueTab[ii];
		}
	}
	
	/* Process the data through the CFFT/CIFFT module */ 
	arm_rfft_fast_f32(&S,ADC3ConvertedValue1,Output_Stageone,ifftFlag); 
	
	/* Process the data through the Complex Magnitude Module for  
	calculating the magnitude at each bin */ 
	arm_cmplx_mag_f32(Output_Stageone, testOutput, fftSize);  


} 
/* ---------------------------------------------------------------------- 
* Generate source data
* ------------------------------------------------------------------- */ 
void Generate_Data(void)
{
	int i;
		for(i=0;i<TEST_LENGTH_SAMPLES/2;i++)//�����ź�����
		{

/* 50Hz���Ҳ���������1KHz */
			Inputbuf[i] = 100+1*arm_sin_f32(2*PI*10*i/Fs)+3*arm_sin_f32(2*PI*i*40/Fs)+5*arm_cos_f32(2*PI*i*80/Fs);

		}
}
/* ---------------------------------------------------------------------- 
* ��ȡ��ֵ����ҪƵ����Ϣ
* ------------------------------------------------------------------- */ 
 void Get_PowerMag_Basicfre(float32_t  *testOutput)
 {
	  int i;
	  PowerMag[0] = testOutput[0]/fftSize;
		for(i=1;i<TEST_LENGTH_SAMPLES/4;i++)
		{
			PowerMag[i] = testOutput[i]/fftSize*2;
			FreMag[i] = i*resolution;
		}
		
 }
/* ---------------------------------------------------------------------- 
* �õ���ֵ���Ƶ��(�����ڲ�Ƶ��)
* ------------------------------------------------------------------- */ 
 int Get_FRE(float32_t PowerMag[])
 {

		float32_t max;
		int sub,i;
		for(i=1;i<TEST_LENGTH_SAMPLES/4;i++)
		{
			if(PowerMag[i]>=max){
				max = PowerMag[i];
				sub = i;
			}
		}
		max = 0;
		return sub;
 }
 