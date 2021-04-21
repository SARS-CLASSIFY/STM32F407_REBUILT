# MARTIX_KEY （矩阵键盘驱动）

- PD0-PD7对应R1-C4

- MARTIX_KEY_SCAN() 函数返回对应的1~16按键值

- 调用示例

  ```c
  	martix_key_init();
  	u8 t = 0;
  	t=MARTIX_KEY_SCAN();
  ```

  

