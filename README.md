# ADF4351 F4标准库移植版

### COPY FORM  wh201906

- 调用示例

  ```c
  // CLK设置说明 100M为晶振频率 freqpfd设为晶振1/4 isDoubled 及 is2Divided用于分频倍频确保占空比 div由所需要频率范围确定 (flow-fhigh)*div = (2.2G-4.4G)
  //若所需频率范围单个整数无法分频，则设双结构体CLK来获取相应的频率范围
  
  ADF4351_CLKConfig CLK;//设置CLK结构体变量
  
  ADF4351_Init();
  ADF4351_SetCLKConfig(&CLK, 100, 25, 0, 1, 32, 0.001); //  68.75-137.5MHZ
  ADF4351_WriteCLKConfig(&CLK);
  ADF4351_SetFreq(&CLK, 132.151);
  
  ```
  
  

