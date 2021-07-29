ADS1256

- 修复了ADS1256头文件与定时器中断冲突的BUG

- 调用说明

  - main.c中

    ```c
    ADS1256_INIT();//初始化,其中PGA倍率和DATARATE可以自行更改设置
    ```


  - 一般在定时器中断中调用

    float ADS1256_TRANS_EQUAL(uint8_t ch_num, uint8_t times)；//获取某一通道值并均值滤波

    float ADS1256_DIFF_EQUAL(uint8_t ch_num,uint8_t times)；//获取某两通道差分值并均值滤波
