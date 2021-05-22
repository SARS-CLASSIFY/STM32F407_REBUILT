#include "oled1.h"
#include "stdlib.h"
#include "delay.h"
#include "oledfont1.h"  	
/*------------------------------------------------------------------------------
* brief   : SSD1306 OLED   IIC COMMUNICATION  SCL->PE0 SDA->PE1
-------------------------------------------------------------------------------*/

u8 OLED1_GRAM[144][8];

//���Ժ���
void OLED1_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED1_WR_Byte(0xA6,OLED1_CMD);//������ʾ
		}
	if(i==1)
		{
			OLED1_WR_Byte(0xA7,OLED1_CMD);//��ɫ��ʾ
		}
}

//��Ļ��ת180��
void OLED1_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED1_WR_Byte(0xC8,OLED1_CMD);//������ʾ
			OLED1_WR_Byte(0xA1,OLED1_CMD);
		}
	if(i==1)
		{
			OLED1_WR_Byte(0xC0,OLED1_CMD);//��ת��ʾ
			OLED1_WR_Byte(0xA0,OLED1_CMD);
		}
}

//��ʱ
void IIC1_delay(void)
{
	u8 t=10;
	while(t--);
}

//��ʼ�ź�
void I2C1_Start(void)
{
	OLED1_SDA_Set();
	OLED1_SCL_Set();
	IIC1_delay();
	OLED1_SDA_Clr();
	IIC1_delay();
	OLED1_SCL_Clr();
	IIC1_delay();
}

//�����ź�
void I2C1_Stop(void)
{
	OLED1_SDA_Clr();
	OLED1_SCL_Set();
	IIC1_delay();
	OLED1_SDA_Set();
}

//�ȴ��ź���Ӧ
void I2C1_WaitAck(void) //�������źŵĵ�ƽ
{
	OLED1_SDA_Set();
	IIC1_delay();
	OLED1_SCL_Set();
	IIC1_delay();
	OLED1_SCL_Clr();
	IIC1_delay();
}

//д��һ���ֽ�
void Send1_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)//��dat��8λ�����λ����д��
		{
			OLED1_SDA_Set();
    }
		else
		{
			OLED1_SDA_Clr();
    }
		IIC1_delay();
		OLED1_SCL_Set();
		IIC1_delay();
		OLED1_SCL_Clr();//��ʱ���ź�����Ϊ�͵�ƽ
		dat<<=1;
  }
}

//����һ���ֽ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED1_WR_Byte(u8 dat,u8 mode)
{
	I2C1_Start();
	Send1_Byte(0x78);
	I2C1_WaitAck();
	if(mode){Send1_Byte(0x40);}
  else{Send1_Byte(0x00);}
	I2C1_WaitAck();
	Send1_Byte(dat);
	I2C1_WaitAck();
	I2C1_Stop();
}

//����OLED��ʾ 
void OLED1_DisPlay_On(void)
{
	OLED1_WR_Byte(0x8D,OLED1_CMD);//��ɱ�ʹ��
	OLED1_WR_Byte(0x14,OLED1_CMD);//������ɱ�
	OLED1_WR_Byte(0xAF,OLED1_CMD);//������Ļ
}

//�ر�OLED��ʾ 
void OLED1_DisPlay_Off(void)
{
	OLED1_WR_Byte(0x8D,OLED1_CMD);//��ɱ�ʹ��
	OLED1_WR_Byte(0x10,OLED1_CMD);//�رյ�ɱ�
	OLED1_WR_Byte(0xAE,OLED1_CMD);//�ر���Ļ
}

//�����Դ浽OLED	
void OLED1_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED1_WR_Byte(0xb0+i,OLED1_CMD); //��������ʼ��ַ
		OLED1_WR_Byte(0x00,OLED1_CMD);   //���õ�����ʼ��ַ
		OLED1_WR_Byte(0x10,OLED1_CMD);   //���ø�����ʼ��ַ
		I2C1_Start();
		Send1_Byte(0x78);
		I2C1_WaitAck();
		Send1_Byte(0x40);
		I2C1_WaitAck();
		for(n=0;n<128;n++)
		{
			Send1_Byte(OLED1_GRAM[n][i]);
			I2C1_WaitAck();
		}
		I2C1_Stop();
  }
}
//��������
void OLED1_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED1_GRAM[n][i]=0;//�����������
			}
  }
	OLED1_Refresh();//������ʾ
}

//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���	
void OLED1_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED1_GRAM[x][i]|=n;}
	else
	{
		OLED1_GRAM[x][i]=~OLED1_GRAM[x][i];
		OLED1_GRAM[x][i]|=n;
		OLED1_GRAM[x][i]=~OLED1_GRAM[x][i];
	}
}

//����
//x1,y1:�������
//x2,y2:��������
void OLED1_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED1_DrawPoint(uRow,uCol,mode);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
//x,y:Բ������
//r:Բ�İ뾶
void OLED1_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED1_DrawPoint(x + a, y - b,1);
        OLED1_DrawPoint(x - a, y - b,1);
        OLED1_DrawPoint(x - a, y + b,1);
        OLED1_DrawPoint(x + a, y + b,1);
 
        OLED1_DrawPoint(x + b, y + a,1);
        OLED1_DrawPoint(x + b, y - a,1);
        OLED1_DrawPoint(x - b, y - a,1);
        OLED1_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size1:ѡ������ 6x8/6x12/8x16/12x24
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED1_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr1=chr-' ';  //����ƫ�ƺ��ֵ
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_08061[chr1][i];} //����0806����
		else if(size1==12)
        {temp=asc2_12061[chr1][i];} //����1206����
		else if(size1==16)
        {temp=asc2_16081[chr1][i];} //����1608����
		else if(size1==24)
        {temp=asc2_24121[chr1][i];} //����2412����
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED1_DrawPoint(x,y,mode);
			else OLED1_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}


//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED1_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED1_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}

//m^n
u32 OLED1_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//��ʾ����
//x,y :�������
//num :Ҫ��ʾ������
//len :���ֵ�λ��
//size:�����С
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED1_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
	u8 t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED1_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED1_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else 
			{
			  OLED1_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}

//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED1_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	for(i=0;i<size3;i++)
	{
		if(size1==16)
				{temp=Hzk11[num][i];}//����16*16����
		else if(size1==24)
				{temp=Hzk21[num][i];}//����24*24����
		else if(size1==32)       
				{temp=Hzk31[num][i];}//����32*32����
		else if(size1==64)
				{temp=Hzk41[num][i];}//����64*64����
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED1_DrawPoint(x,y,mode);
			else OLED1_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size1)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}

//num ��ʾ���ֵĸ���
//space ÿһ����ʾ�ļ��
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED1_ScrollDisplay(u8 num,u8 space,u8 mode)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED1_ShowChinese(128,24,t,16,mode); //д��һ�����ֱ�����OLED1_GRAM[][]������
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //��ʾ���
				 {
					for(i=1;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								OLED1_GRAM[i-1][n]=OLED1_GRAM[i][n];
							}
						}
           OLED1_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=1;i<144;i++)   //ʵ������
		{
			for(n=0;n<8;n++)
			{
				OLED1_GRAM[i-1][n]=OLED1_GRAM[i][n];
			}
		}
		OLED1_Refresh();
	}
}

//x,y���������
//sizex,sizey,ͼƬ����
//BMP[]��Ҫд���ͼƬ����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED1_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
	u16 j=0;
	u8 i,n,temp,m;
	u8 x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		 for(i=0;i<sizex;i++)
		 {
				temp=BMP[j];
				j++;
				for(m=0;m<8;m++)
				{
					if(temp&0x01)OLED1_DrawPoint(x,y,mode);
					else OLED1_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}
//OLED�ĳ�ʼ��
void OLED1_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE,ENABLE);//ʹ��PORTA~E,PORTGʱ��
  	
	//GPIO��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	
	//GPIO��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	OLED1_RES_Clr();
	delay_ms(200);
	OLED1_RES_Set();
	
	OLED1_WR_Byte(0xAE,OLED1_CMD);//--turn off oled panel
	OLED1_WR_Byte(0x00,OLED1_CMD);//---set low column address
	OLED1_WR_Byte(0x10,OLED1_CMD);//---set high column address
	OLED1_WR_Byte(0x40,OLED1_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED1_WR_Byte(0x81,OLED1_CMD);//--set contrast control register
	OLED1_WR_Byte(0xCF,OLED1_CMD);// Set SEG Output Current Brightness
	OLED1_WR_Byte(0xA1,OLED1_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED1_WR_Byte(0xC8,OLED1_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED1_WR_Byte(0xA6,OLED1_CMD);//--set normal display
	OLED1_WR_Byte(0xA8,OLED1_CMD);//--set multiplex ratio(1 to 64)
	OLED1_WR_Byte(0x3f,OLED1_CMD);//--1/64 duty
	OLED1_WR_Byte(0xD3,OLED1_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED1_WR_Byte(0x00,OLED1_CMD);//-not offset
	OLED1_WR_Byte(0xd5,OLED1_CMD);//--set display clock divide ratio/oscillator frequency
	OLED1_WR_Byte(0x80,OLED1_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED1_WR_Byte(0xD9,OLED1_CMD);//--set pre-charge period
	OLED1_WR_Byte(0xF1,OLED1_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED1_WR_Byte(0xDA,OLED1_CMD);//--set com pins hardware configuration
	OLED1_WR_Byte(0x12,OLED1_CMD);
	OLED1_WR_Byte(0xDB,OLED1_CMD);//--set vcomh
	OLED1_WR_Byte(0x40,OLED1_CMD);//Set VCOM Deselect Level
	OLED1_WR_Byte(0x20,OLED1_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED1_WR_Byte(0x02,OLED1_CMD);//
	OLED1_WR_Byte(0x8D,OLED1_CMD);//--set Charge Pump enable/disable
	OLED1_WR_Byte(0x14,OLED1_CMD);//--set(0x10) disable
	OLED1_WR_Byte(0xA4,OLED1_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED1_WR_Byte(0xA6,OLED1_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED1_Clear();
	OLED1_WR_Byte(0xAF,OLED1_CMD);
}



/** ------------------------------ �Զ������ֵ��ʾ���� ---------------------------------- **/


/*------------------------------------------------------------------------------
* brief   : measure Presentation 
* Description    : fre vpp pulse show
* EntryParameter : u32 fre ,u8 pulse,u16 vpp
* ReturnValue    : NONE
-------------------------------------------------------------------------------*/

void MEASURE1_SHOW(u32 fre, u8 pulse,u16 vpp)
{	
	OLED1_Clear_Sp();
//	OLED1_Init();				//��ʼ��OLED
	OLED1_ShowChinese(0,0,0,16,1);//Ƶ
	OLED1_ShowChinese(16,0,1,16,1);//��
	OLED1_ShowChar(32,0,':',16,1);
	
	OLED1_ShowChinese(0,18,2,16,1);//ռ
	OLED1_ShowChinese(16,18,3,16,1);//��
	OLED1_ShowChinese(32,18,4,16,1);//��
	OLED1_ShowChar(48,18,':',16,1);
	
	OLED1_ShowChinese(0,37,5,16,1);
	OLED1_ShowChinese(16,37,5,16,1);
	OLED1_ShowChinese(32,37,6,16,1);
	OLED1_ShowChar(48,37,':',16,1);
	//fre
	OLED1_ShowNum(50,0,fre,JUDGE1_NUM(fre),16,1);
	OLED1_ShowString(105,0,"HZ",16,1);
	//pulse
	OLED1_ShowNum(65,18,pulse,JUDGE1_NUM(pulse),16,1);
	OLED1_ShowChar(105,18,'%',16,1);
	//vpp
	OLED1_ShowNum(65,37,vpp,JUDGE1_NUM(vpp),16,1);
	OLED1_ShowString(105,37,"mv",16,1);
	
	
	OLED1_Refresh();
}


/*------------------------------------------------------------------------------
* brief   : judge num place
-------------------------------------------------------------------------------*/
u8 JUDGE1_NUM(u32 num)
{
	 u8 intcount = 0,count=0;
	 while(num !=0)                                           //�ж�����һ���������
	 {
		count++;                                            //����Ϊ������ʱ��λ����һ
		num /=10;                                              //������λ
	 }
	 return count;
}
/*------------------------------------------------------------------------------
* brief   : special clear
-------------------------------------------------------------------------------*/
void OLED1_Clear_Sp(void)
{
	u8 i,n;
	for(i=0;i<2;i++)
	{
	   for(n=50;n<105;n++)
			{
			 OLED1_GRAM[n][i]=0;//�����������
			}
  }
	for(i=2;i<8;i++)
	{
	   for(n=65;n<105;n++)
			{
			 OLED1_GRAM[n][i]=0;//�����������
			}
  }
	OLED1_Refresh();//������ʾ
}
