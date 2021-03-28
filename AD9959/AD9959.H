#ifndef _AD9959_H_
#define _AD9959_H_
#include "sys.h"
#include "stdint.h"
//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR 通道选择寄存器
#define FR1_ADD   0x01   //FR1 功能寄存器1
#define FR2_ADD   0x02   //FR2 功能寄存器2
#define CFR_ADD   0x03   //CFR 通道功能寄存器
#define CFTW0_ADD 0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR 幅度控制寄存器
#define LSRR_ADD  0x07   //LSR 通道线性扫描寄存器
#define RDW_ADD   0x08   //RDW 通道线性向上扫描寄存器
#define FDW_ADD   0x09   //FDW 通道线性向下扫描寄存器
//AD9959管脚宏定义
#define CS			PAout(1)
#define SCLK		PAout(2)
#define SDIO0		PAout(3)
#define UPDATE	PAout(6)
#define Reset		PAout(7)

//#define PS0			PAout(7)
//#define PS1			PAout(2)
//#define PS2			PBout(10)
//#define PS3			PCout(0)
//#define SDIO1		PAout(4)
//#define SDIO2		PAout(3)
//#define SDIO3		PAout(8)
//#define AD9959_PWR		PAout(9)


void delay1 (u32 length);
void IntReset(void);	  //AD9959复位
void IO_Update(void);   //AD9959更新数据
void Intserve(void);		   //IO口初始化
void WriteData_AD9959(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData,u8 temp);
void Init_AD9959(void);
void Write_frequence(u8 Channel,u32 Freq);
void Write_Amplitude(u8 Channel, u16 Ampli);
void Write_Phase(u8 Channel,u16 Phase);
void AD9959_SetFrequency4Channel(u32 Freq0,u32 Freq1,u32 Freq2,u32 Freq3);
void AD9959_SetAmp4Channel(u32 Amp0,u32 Amp1,u32 Amp2,u32 Amp3);
void AD9959_SetPhase4Channel(u32 Phase0,u32 Phase1,u32 Phase2,u32 Phase3);
void AD9959_Set_Freq(u32 Freq);
void mode1(uint32_t starthz, uint32_t stophz, uint32_t stephz);

#endif
