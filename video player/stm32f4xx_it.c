//V1.0.0
#include "stm32f4xx_it.h"
#include "main.h"

//NMI exception handler
void NMI_Handler(void)
{
}

//Hard Fault exception handler
extern 	u32 Strsize;
extern 	u16 Strtype;
extern 	FIL fileR;
void HardFault_Handler(void)
{
	LCD_Window(239,319,240,320);//»Ö¸´ÏÔÊ¾´°¿Ú
	LCD_Mode(4);
  	while (1)
  	{	
		LCD_Num(20,110,Strtype,5,WHITE);
		LCD_Num(20,140,Strsize,10,WHITE);
		LCD_Num(20,170,f_tell(&fileR),10,WHITE);	
		LCD_String(20,0,"HardFault!",RED);
  	}
}

//Memory Manage exception handler
void MemManage_Handler(void)
{
  	while (1)
  	{
  	}
}

//Bus Fault exception handler
void BusFault_Handler(void)
{
  	while (1)
  	{
  	}
}

//Usage Fault exception handler
void UsageFault_Handler(void)
{
  	while (1)
  	{
  	}
}

//SVCall exception handler
void SVC_Handler(void)
{
}

//Debug Monitor exception handler
void DebugMon_Handler(void)
{
}

//PendSVC exception handler
void PendSV_Handler(void)
{
}

//SysTick handler
extern u32 ntime;
void SysTick_Handler(void)
{
	ntime--;
}

void SDIO_IRQHandler(void)
{
  	SD_ProcessIRQSrc();
}

void SD_SDIO_DMA_IRQHANDLER(void)
{
  	SD_ProcessDMAIRQ();
}


