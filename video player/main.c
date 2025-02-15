#include "main.h"

/****************************************************************************
	金龙电子工作室
	GD STM32F407学习板
	淘宝店：http://shop71381140.taobao.com/
	本程序只供学习使用，未经作者许可，不得用于其它任何用途
****************************************************************************/

int main(void)
{
	Key_Init();
	LED_Init();
	LCD_Init();
	Touch_Init();
	COM1Init(115200);
	LCD_Clear(BLUE);
  	LCD_String(20,20,"Vido play!",RED);
	delay_ms(500);
	AVI_play();
  	while(1)
	{
	  	delay_ms(500);
		LEDTog(LED1);
	}
}


