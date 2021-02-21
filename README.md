# NEC红外编码发射程序

1. 总体思路：

   ​	使用一路PWM产生38KHZ载波，根据NEC编码格式编写地址码，引导码，命令码的调制函数

   最后在sent_data()函数中总体调用，在外围电路接上红外发射管可实现NEC红外发射。

2. 配置说明：

   ​	PWM配置函数中添加了失能PWM函数，为了实现关闭PWM时控制最后时刻的电平高低，采用

   更改CCRX比价寄存器的方式来DISABLE PWM的波形生成。

   ​	REMOTE文件夹中为编写的NEC编码发射函数，添加进工程调用即可。在**TIM3_PWM_Init(myarr,0);**

   初始化以后直接调用sent_data()函数即可。

3. NEC红外编码参考网址：https://www.cnblogs.com/yulongchen/archive/2013/04/12/3017409.html NEC编码规则

   ​									  	https://blog.csdn.net/szw_yx/article/details/76263182 红外编码解析

   停止PWM输出时的电平：https://www.stmcu.org.cn/module/forum/thread-622235-1-1.html 

