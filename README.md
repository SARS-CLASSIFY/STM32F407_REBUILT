# SSD1306驱动

- ssd1306 128*64 oled IIC 通信驱动

- 添加了简单的频率，峰峰值，占空比测量显示函数，方便使用

- 调用示例

  ```c
  	OLED_Init();
  		
  	u16 vpp =233;
  	u32 fre =115200;
  	u8 pulse = 50;
  	
  	MEASURE_SHOW(fre,pulse,vpp);
  ```

  

