#include "ff.h"
#include "lcd.h"
#include "sdio_sd.h"
#include "diskio.h"

/****************************************************************************
	金龙电子工作室
	GD STM32F407学习板
	淘宝店：http://shop71381140.taobao.com/
	本程序只供学习使用，未经作者许可，不得用于其它任何用途
****************************************************************************/

u8 tmp_buffer[512];

u16 X1R5G5B5_R5G6B5(u16 input)
{
    u16  rgb;
    rgb=((input&0xF800)<<1) + ((input&0x3E0)<<1) + (input&0x1F);
    return rgb;
}

void  DispImage(char *file,u16 StartX,u16 StartY,u16 width,u16 height)
{
    FIL file1;
    u16  n,result,color;
    UINT br;

	LCD_Mode(7);
	LCD_Window(StartX+width-1,StartY+height,width,height);
    result=f_open(&file1,file,FA_OPEN_EXISTING|FA_READ);
    if(result!=FR_OK )return;//打开成功
    f_read(&file1,tmp_buffer,54,&br);//读取54字节文件头
	LCD_Cursor(StartX,StartY);
	LCD_REG=0x22;	
	while(!f_eof(&file1))
	{
		f_read(&file1,tmp_buffer,512,&br);
		for(n=0;n<512;n+=2)
		{
	        color=tmp_buffer[n]|tmp_buffer[n+1]<<8;
	      	LCD_RAM=((color&0x7FE0)<<1)+(color&0x1F);//X1RGB555_转RGB565
		}
	}
    f_close(&file1);
}

