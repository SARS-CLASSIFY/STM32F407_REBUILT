#include "AD9959.H"
#include "delay.h"
u8 CSR_DATA0[1] = {0x10};      // 开 CH0
u8 CSR_DATA1[1] = {0x20};      // 开 CH1
u8 CSR_DATA2[1] = {0x40};      // 开 CH2
u8 CSR_DATA3[1] = {0x80};      // 开 CH3		
																	
u8 FR1_DATA[3] = {0xD0,0x00,0x00};//20倍频 
u8 FR2_DATA[2] = {0x00,0x00};//default Value = 0x0000
u8 CFR_DATA[3] = {0x00,0x03,0x02};//default Value = 0x000302	   														
u8 CPOW0_DATA[2] = {0x00,0x00};//default Value = 0x0000   @ = POW/2^14*360													
u8 LSRR_DATA[2] = {0x00,0x00};	
u8 RDW_DATA[4] = {0x00,0x00,0x00,0x00};																
u8 FDW_DATA[4] = {0x00,0x00,0x00,0x00};
u8 ACRdata[3] = {0x00,0x10,0x00};//default Value = 0x--0000 Rest = 18.91/Iout 

//AD9959初始化
void Init_AD9959(void)  
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;       
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 //PA端口时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_6| GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_Init(GPIOA ,&GPIO_InitStructure) ;
		
	Intserve();  //IO口初始化
  IntReset();  //AD9959复位  
	
  WriteData_AD9959(FR1_ADD,3,FR1_DATA,1);//写功能寄存器1
	
//  WriteData_AD9959(FR2_ADD,2,FR2_DATA,0);
//  WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
//  WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
//  WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
//  WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
//  WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);
//  WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);
//	
//	 Write_frequence(0,1000);
//	 Write_frequence(1,1000);
//	 Write_frequence(2,1000);
//	 Write_frequence(3,1000);
} 
//延时
void delay1 (u32 length)
{
	length = length*12;
   while(length--);
}

//IO口初始化
void Intserve(void)		   
{   
//		AD9959_PWR=0;
	
    CS = 1;
    SCLK = 0;
    UPDATE = 0;
	  SDIO0 = 0;
	
//    PS0 = 0;
//    PS1 = 0;
//    PS2 = 0;
//    PS3 = 0;
//    SDIO1 = 0;
//    SDIO2 = 0;
//    SDIO3 = 0;
}

//AD9959复位
void IntReset(void)	  
{
  Reset = 0;
	delay1(1);
	Reset = 1;
	delay1(30);
	Reset = 0;
}

 //AD9959更新数据
void IO_Update(void)  
{
	UPDATE = 0;
	delay1(2);
	UPDATE = 1;
	delay1(4);
	UPDATE = 0;
}

//SPI数据写入
void WriteData_AD9959(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData,u8 temp)
{
	u8	ControlValue = 0;
	u8	ValueToWrite = 0;
	u8	RegisterIndex = 0;
	u8	i = 0;

	ControlValue = RegisterAddress;
//写入地址
	SCLK = 0;
	CS = 0;	 
	for(i=0; i<8; i++)
	{
		SCLK = 0;
		if(0x80 == (ControlValue & 0x80))
		SDIO0= 1;	  
		else
		SDIO0= 0;	  
		SCLK = 1;
		ControlValue <<= 1;
	}
	SCLK = 0;
//写入数据
	for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i=0; i<8; i++)
		{
			SCLK = 0;
			if(0x80 == (ValueToWrite & 0x80))
			SDIO0= 1;	  
			else
			SDIO0= 0;	  
			SCLK = 1;
			ValueToWrite <<= 1;
		}
		SCLK = 0;		
	}	
	if(temp==1)
	IO_Update();	
  CS = 1;
} 

//设置单个通道频率
void Write_frequence(u8 Channel,u32 Freq)
{	 
		u8 CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
	  u32 Temp;            
	  Temp=(u32)Freq*8.589934592;	   //将输入频率因子分为四个字节  8.589934592=(2^32)/500000000
		
	  CFTW0_DATA[3]=(u8)Temp;
	  CFTW0_DATA[2]=(u8)(Temp>>8);
	  CFTW0_DATA[1]=(u8)(Temp>>16);
	  CFTW0_DATA[0]=(u8)(Temp>>24);
	  if(Channel==0)	  
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);//控制寄存器写入CH0通道
      WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH0设定频率
		}
	  else if(Channel==1)	
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);//控制寄存器写入CH1通道
      WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH1设定频率	
	  }
	  else if(Channel==2)	
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);//控制寄存器写入CH2通道
      WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH2设定频率	
	  }
	  else if(Channel==3)	
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);//控制寄存器写入CH3通道
      WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,3);//CTW0 address 0x04.输出CH3设定频率	
	  }																																																																										 
} 

//设置单个通道幅度<1023
void Write_Amplitude(u8 Channel, u16 Ampli)
{ 
	u16 A_temp;  
	u8 ACR_DATA[3] = {0x00,0x00,0x00};
	
  A_temp=Ampli|0x1000;
	ACR_DATA[2] = (u8)A_temp;  //低位数据
  ACR_DATA[1] = (u8)(A_temp>>8); //高位数据
  if(Channel==0)
  {
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1); 
    WriteData_AD9959(ACR_ADD,3,ACR_DATA,1); 
	}
  else if(Channel==1)
  {
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1); 
    WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
  else if(Channel==2)
  {
	  WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1); 
    WriteData_AD9959(ACR_ADD,3,ACR_DATA,1); 
	}
  else if(Channel==3)
  {
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1); 
    WriteData_AD9959(ACR_ADD,3,ACR_DATA,1); 
	}
}

//设置单个通道相位
void Write_Phase(u8 Channel,u16 Phase)
{
	u16 P_temp=0;
  P_temp=(u16)Phase*45.511111;  //将输入相位差写入，进度1度，45.511111=2^14）/360
	CPOW0_DATA[1]=(u8)P_temp;
	CPOW0_DATA[0]=(u8)(P_temp>>8);
	if(Channel==0)
  {
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,0); 
    WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  }
  else if(Channel==1)
  {
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,0); 
    WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  }
  else if(Channel==2)
  {
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,0); 
    WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  }
  else if(Channel==3)
  {
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,0); 
    WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  }
}

//设置4个通道的频率
void AD9959_SetFrequency4Channel(u32 Freq0,u32 Freq1,u32 Freq2,u32 Freq3)
{
	Write_frequence(0,Freq0);
	Write_frequence(1,Freq1);
	Write_frequence(2,Freq2);
	Write_frequence(3,Freq3);			
}

//设置4个通道的幅度
void AD9959_SetAmp4Channel(u32 Amp0,u32 Amp1,u32 Amp2,u32 Amp3)
{
	Write_Amplitude(0,Amp0);
	Write_Amplitude(1,Amp1);
	Write_Amplitude(2,Amp2);
	Write_Amplitude(3,Amp3);			
}

//设置4个通道的相位
void AD9959_SetPhase4Channel(u32 Phase0,u32 Phase1,u32 Phase2,u32 Phase3)
{
	Write_Phase(0,Phase0);
	Write_Phase(1,Phase1);
	Write_Phase(2,Phase2);
	Write_Phase(3,Phase3);			
}

//四通道同频
void AD9959_Set_Freq(u32 Freq)
{
		u8 CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
	  u32 Temp;            
	  Temp=(u32)Freq*8.589934592;	   //将输入频率因子分为四个字节  8.589934592=(2^32)/500000000
	  CFTW0_DATA[3]=(u8)Temp;
	  CFTW0_DATA[2]=(u8)(Temp>>8);
	  CFTW0_DATA[1]=(u8)(Temp>>16);
	  CFTW0_DATA[0]=(u8)(Temp>>24);
		
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);//控制寄存器写入CH0通道
    WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH0设定频率
	
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);//控制寄存器写入CH1通道
    WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH1设定频率	

	
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);//控制寄存器写入CH2通道
    WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH2设定频率	
	
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);//控制寄存器写入CH3通道
    WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,3);//CTW0 address 0x04.输出CH3设定频率
}

//简单扫频控制
void mode1(uint32_t starthz, uint32_t stophz, uint32_t stephz)
{
	uint32_t i = 0;
	if (starthz < stophz) //从低到高
	{
		for (i = 0; i < ((stophz - starthz) / stephz); i++)
		{
			AD9959_Set_Freq(starthz + i * stephz);
						delay_ms(1);
		}
	}
	else //从高到低
	{
		for (i = ((starthz - stophz) / stephz); i > 0; i--)
		{
			AD9959_Set_Freq( starthz + i * stephz);
						delay_ms(1);
		}
	}
}

