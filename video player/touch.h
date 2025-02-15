#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "stm32f4xx_conf.h"
	 
#define TCS_LOW    	GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define TCS_HIGH   	GPIO_SetBits(GPIOB, GPIO_Pin_14)     
#define PEN_int		(GPIOD->IDR&GPIO_Pin_6)

#define CMD_RDY 0X90  
#define CMD_RDX	0XD0    											 	    
			  
void Touch_Init(void);		 		 
u16 ADS_Read(u8 CMD);	 
u16 Read_XY(u8 CMD);
	 
#endif


