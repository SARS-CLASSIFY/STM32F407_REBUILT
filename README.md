# AD9959驱动

1.引脚定义

#define CS				PAout(1)
#define SCLK		    PAout(2)
#define SDIO0		  PAout(3)
#define UPDATE	   PAout(6)
#define Reset		    PAout(7)、

2.调用示例

```c
Init_AD9959();
AD9959_Set_Freq(10000); //频率设置
AD9959_SetPhase4Channel(0,0,90,0);//相位设置，对应4通道
AD9959_SetAmp4Channel(1023,1023,1023,1023);//幅值设置
```
VPP(MAX) = 460mv