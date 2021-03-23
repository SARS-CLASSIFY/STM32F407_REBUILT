# STM32F4 FFT驱动代码

注意：使用前需要安装F4 官方DSP库

1. FFT.h说明：

   1-1	TEST_LENGTH_SAMPLES为进行FFT数据有效位的2倍，例ADC一次采样需处理的数据为1024，则TEST_LENGTH_SAMPLES = 2048

   1-2	FFT()为FFT功能函数；Generate_Data生成测试信号；Get_PowerMag_Basicfre函数用于对FFT后数据处理，每个FFT点位的幅值及频率。

2. FFT.c说明：	

   FS为采样频率；mode为测试模式切换，1为使用生成数据，0为使用ADC采样数据；resolution为FFT点位分辨率

3. 调用说明：

   3-1	FFT.h中 ADC_DualConvertedValueTab传入ADC采样到的数据；

   3-2	FFT.c中更改采样频率与ADC采样频率一致；***（十分重要）ADC采样时钟主频84M

   3-3	切换MODE模式；

   3-4	main.c中定义外部变量

   ```c
   extern uint16_t ADC_DualConvertedValueTab[1024]; 			//存储ADC采样数据
   extern  float32_t testOutput[TEST_LENGTH_SAMPLES/2];	//存储FFT直接转换后数据
   extern float32_t PowerMag[TEST_LENGTH_SAMPLES/4];			//存储各点位幅值
   extern long FreMag[TEST_LENGTH_SAMPLES/4];						//存储各点位对应频率
   ```

   3-5	在DMA传输完成中断中调用FFT()

   3-6	main函数中

   ```c
	  int sub;
   	  Get_PowerMag_Basicfre(testOutput);
   	  sub = Get_FRE(PowerMag);
   	  printf("ADCpower: %f ADCFRE: %ld\r\n",PowerMag[sub],FreMag[sub]);//打印幅值频率
   ```
   
   调用两个函数前者获取FFT结束后的频幅值，后者可以返回幅值最大频率下标，printf可以尝试打印频幅值
   
4. 上传代码：

   1）DSP库相关代码，加入工程使用

   2）ADC DMA配置代码

   3）FFT驱动代码

5.main示例

```c
int main(void)
{ 

	int sub ,i;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	Dac1_Init();
	ADC_SR_Detect_Config();
	while(1)
	{
//		Get_PowerMag_Basicfre(testOutput);//获取FFT结束后的频谱值
		printf("FFT Result:\r\n");
		Get_PowerMag_Basicfre(testOutput);
	  sub = Get_FRE(PowerMag);

		printf("ADCpower: %f ADCFRE: %ld\r\n",PowerMag[sub],FreMag[sub]);//打印幅值频率
	

		delay_ms(500);
	}
}
```


