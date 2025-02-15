#include "avifile.h"
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "lcd.h"
#include "usart.h"

/****************************************************************************
	金龙电子工作室
	GD STM32F407学习板
	淘宝店：http://shop71381140.taobao.com/
	本程序只供学习使用，未经作者许可，不得用于其它任何用途
****************************************************************************/

AVI_TypeDef AVI_file;
avih_TypeDef* avihChunk;
strh_TypeDef* strhChunk;
BITMAPINFO* bmpinfo;
WAVEFORMAT* wavinfo;
u32 temp=0x00;
u8 vids_ID;
u8 auds_ID;

u8 AVI_Parser(u8 *buffer)
{
	temp=ReadUnit(buffer,0,4,1);//读"RIFF"
	if(temp!=RIFF_ID)return 1;
	AVI_file.RIFFchunksize=ReadUnit(buffer,4,4,1);//RIFF数据块长度
	temp=ReadUnit(buffer,8,4,1);//读"AVI "
	if(temp!=AVI_ID)return 2;
	temp=ReadUnit(buffer,12,4,1);//读"LIST"
	if(temp!=LIST_ID)return 3;
	AVI_file.LISTchunksize=ReadUnit(buffer,16,4,1);//LIST数据块长度
	temp=ReadUnit(buffer,20,4,1);//读"hdrl"
	if(temp!=hdrl_ID)return 4;
	temp=ReadUnit(buffer,24,4,1);//读"avih"
	if(temp!=avih_ID)return 5;
	AVI_file.avihsize=ReadUnit(buffer,28,4,1);//avih数据块长度	
	return 0;				
}

u8 Avih_Parser(u8 *buffer)
{
	avihChunk=(avih_TypeDef*)buffer;
#ifdef PrintInfo
	printf("\r\navih数据块信息:\n");
	printf("\r\nSecPerFrame:%d",avihChunk->SecPerFrame);
	printf("\r\nMaxByteSec:%d",avihChunk->MaxByteSec);
	printf("\r\nChunkBase:%d",avihChunk->ChunkBase);
	printf("\r\nSpecProp:%d",avihChunk->SpecProp);
	printf("\r\nTotalFrame:%d",avihChunk->TotalFrame);
	printf("\r\nInitFrames:%d",avihChunk->InitFrames);
	printf("\r\nStreams:%d",avihChunk->Streams);
	printf("\r\nRefBufSize:%d",avihChunk->RefBufSize);
	printf("\r\nWidth:%d",avihChunk->Width);
	printf("\r\nHeight:%d",avihChunk->Height);
#endif
	if((avihChunk->Width>320)||(avihChunk->Height>240))return 1;//视频尺寸不支持
	if(avihChunk->Streams!=2)return 2;//视频流数不支持
	return 0;
}

u8 Strl_Parser(u8 *buffer)
{
	temp=ReadUnit(buffer,0,4,1);//读"LIST"
	if(temp!=LIST_ID)return 1;
	AVI_file.strlsize=ReadUnit(buffer,4,4,1);//strl数据块长度
	temp=ReadUnit(buffer,8,4,1);//读"strl"
	if(temp!=strl_ID)return 2;
	temp=ReadUnit(buffer,12,4,1);//读"strh"
	if(temp!=strh_ID)return 3;
	AVI_file.strhsize=ReadUnit(buffer,16,4,1);//strh数据块长度
	strhChunk=(strh_TypeDef*)(buffer+20);		 //108
#ifdef PrintInfo
	printf("\r\nstrh数据块信息:\n");	
	printf("\r\nStreamType:%s",strhChunk->StreamType);
	printf("\r\nHandler:%s",strhChunk->Handler);//编码类型MJPEG
	printf("\r\nStreamFlag:%d",strhChunk->StreamFlag);
	printf("\r\nPriority:%d",strhChunk->Priority);
	printf("\r\nLanguage:%d",strhChunk->Language);
	printf("\r\nInitFrames:%d",strhChunk->InitFrames);
	printf("\r\nScale:%d",strhChunk->Scale);
	printf("\r\nRate:%d",strhChunk->Rate);
	printf("\r\nStart:%d",strhChunk->Start);
	printf("\r\nLength:%d",strhChunk->Length);
	printf("\r\nRefBufSize:%d",strhChunk->RefBufSize);
	printf("\r\nQuality:%d",strhChunk->Quality);
	printf("\r\nSampleSize:%d",strhChunk->SampleSize);
	printf("\r\nFrameLeft:%d",strhChunk->Frame.Left);
	printf("\r\nFrameTop:%d",strhChunk->Frame.Top);
	printf("\r\nFrameRight:%d",strhChunk->Frame.Right);
	printf("\r\nFrameBottom:%d",strhChunk->Frame.Bottom);
#endif
	if(strhChunk->Handler[0]!='M')return 4;
	return 0;
}

u8 Strf_Parser(u8 *buffer)
{
	temp=ReadUnit(buffer,0,4,1);//读"strf"
	if(temp!=strf_ID)return 1;
	if(strhChunk->StreamType[0]=='v')//第一个流为视频流
	{
		vids_ID='0';
		auds_ID='1';
		bmpinfo=(BITMAPINFO*)(buffer+8);
		wavinfo=(WAVEFORMAT*)(buffer+4332);
	}
	else if(strhChunk->StreamType[0]=='a')//第一个流为音频流
	{
		vids_ID='1';
		auds_ID='0';
		wavinfo=(WAVEFORMAT*)(buffer+8);
		bmpinfo=(BITMAPINFO*)(buffer+4332);
	}
#ifdef PrintInfo		
	printf("\r\nstrf数据块信息(视频流):\n");		
	printf("\r\n本结构体大小:%d",bmpinfo->bmiHeader.Size);
	printf("\r\n图像宽:%d",bmpinfo->bmiHeader.Width);
	printf("\r\n图像高:%d",bmpinfo->bmiHeader.Height);
	printf("\r\n平面数:%d",bmpinfo->bmiHeader.Planes);
	printf("\r\n像素位数:%d",bmpinfo->bmiHeader.BitCount);
	printf("\r\n压缩类型:%s",bmpinfo->bmiHeader.Compression);
	printf("\r\n图像大小:%d",bmpinfo->bmiHeader.SizeImage);
	printf("\r\n水平分辨率:%d",bmpinfo->bmiHeader.XpixPerMeter);
	printf("\r\n垂直分辨率:%d",bmpinfo->bmiHeader.YpixPerMeter);
	printf("\r\n使用调色板颜色数:%d",bmpinfo->bmiHeader.ClrUsed);
	printf("\r\n重要颜色:%d",bmpinfo->bmiHeader.ClrImportant);

	printf("\r\nstrf数据块信息(音频流):\n");
	printf("\r\n格式标志:%d",wavinfo->FormatTag);
	printf("\r\n声道数:%d",wavinfo->Channels);
	printf("\r\n采样率:%d",wavinfo->SampleRate);
	printf("\r\n波特率:%d",wavinfo->BaudRate);
	printf("\r\n块对齐:%d",wavinfo->BlockAlign);
	printf("\r\n本结构体大小:%d",wavinfo->Size);
#endif
	return 0;
}

u16 Search_Movi(u8* buffer)
{
	u16 i;
	for(i=0;i<20480;i++)
	{
	   	if(buffer[i]==0x6d)
			if(buffer[i+1]==0x6f)
				if(buffer[i+2]==0x76)	
					if(buffer[i+3]==0x69)return i;//找到"movi"	
	}
	return 0;		
}

u16 Search_Fram(u8* buffer)
{
	u16 i;
	for(i=0;i<20480;i++)
	{
	   	if(buffer[i]=='0')
			if(buffer[i+1]==vids_ID)
				if(buffer[i+2]=='d')	
					if(buffer[i+3]=='c')return i;//找到"xxdc"	
	}
	return 0;		
}

u32 ReadUnit(u8 *buffer,u8 index,u8 Bytes,u8 Format)//1:大端模式;0:小端模式
{
  	u8 off=0;
  	u32 unit=0;  
  	for(off=0;off<Bytes;off++)unit|=buffer[off+index]<<(off*8);
  	if(Format)unit=__REV(unit);//大端模式
  	return unit;
}


