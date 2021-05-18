# AD9910驱动代码

1. 引脚配置

   ```c
    /*-------------- Freq_convert() 函数直接用于设置频率 ------
      -------------- Amp_convert()  函数用于设置幅值     -----
    --		PWR 			-> PC7
    --   PROFILE1 	-> PA11
    --		DRCTL			-> PC11
    --		SDIO			-> PD0
    --		SCLK			-> PD3
    --		CS				-> PD5
    --		REST			-> PC6
    --		PROFILE2	-> PA10
    --		PROFILE0	-> PA12
    --		UP_DAT		-> PA15
    --		DRHOLD		-> PC12	 
    */
   ```

  2.调用示例

```
	Init_ad9910();			//模块初始化
	Freq_convert(2000000);	//频率设置，精度1HZ
	Amp_convert(150);		//幅值设置 1~649
```

