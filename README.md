# STM32F4 US-015超声波测距

- PA0->Echo	PA4->Trig
- 添加 timer_capture.c  dist_us.c
- Dist_US_Start();即可测量
- 实际10cm以上返回脉冲会间断，初步考虑与模块相关