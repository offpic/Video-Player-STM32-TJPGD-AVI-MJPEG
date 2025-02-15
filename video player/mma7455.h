#ifndef __MMA_H__
#define __MMA_H__

#include "stm32f4xx_conf.h"

#define MMA_CS_LOW() 	GPIO_ResetBits(GPIOD, GPIO_Pin_6)	   
#define MMA_CS_HIGH()	GPIO_SetBits(GPIOD, GPIO_Pin_6)

#define CMD_GETX		0x06
#define CMD_GETY		0x07
#define CMD_GETZ		0x08

void MMA_Init(void);
void MMA_Write(u8 cmd,u8 data);
u8 MMA_Read(u8 cmd);
void MMA_Start(void);
u8 ReadXYZ(u8 CMD);

u8 SPI1_SendByte(u8 byte);

#endif


