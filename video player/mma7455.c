#include "MMA7455.h"
#include "delay.h"
#include "spi.h"

u8 MMA_Read(u8 cmd)
{
  	u8 data;
  	cmd&=0x3f;
  	MMA_CS_LOW();
  	SPI1_RWByte(cmd<<1);
	data=SPI1_RWByte(0xFF);	   
  	MMA_CS_HIGH();
  	return data;
}

void MMA_Write(u8 cmd,u8 data)
{
  	cmd&=0x3f;
  	MMA_CS_LOW();
  	SPI1_RWByte((cmd<<1)+0x80);
    SPI1_RWByte(data);
  	MMA_CS_HIGH();
}

u8 ReadXYZ(u8 CMD)
{
	u16 i, j;
	u16 buf[10];
	u16 sum=0;
	u16 temp;
	for(i=0;i<10;i++)buf[i]=MMA_Read(CMD);				    
	for(i=0;i<9; i++)
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	for(i=3;i<7;i++)sum+=buf[i];
	temp=sum/(4);
	return temp;   
}

void MMA_Start(void)
{
	MMA_Write(0x16,0x15);
	delay_us(10);
}

void MMA_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//INT1:PD3
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//INT2:PB5
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	SPI1_Init();
	CSPin_init();
}


