#include "boardinit.h"
void BoardInit::Uart0PutString(char *s)
{
	while(*s)
	{
		while(!((*R_ULSR0) & 0x20)) ;
		*R_UTHB0 = *s++;
	}
	while(!((*R_ULSR0) & 0x40)) ;
}

void BoardInit::Uart1PutString(char *s)
{
	gpio_output_high(45);
	while(*s)
	{
		while(!((*R_ULSR1) & 0x20)) ;
		*R_UTHB1 = *s++;
	}
	while(!((*R_ULSR1) & 0x40)) ;
	gpio_output_low(45);
}

bool BoardInit::InitDisplay(int type,int w,int h)
{	
	if(type == BOARD_PST1)
	{
		SetTextureMemAddr(0xC2000000,0x2000000);
		//set frame buffer(6M bytes)
		SetFrameBufferStartAddr(0xC0C00000); 
	}
	else if(type == BOARD_PST2)
	{
		SetTextureMemAddr(0xE0000000 ,0x4000000);
		//set frame buffer(max 6M bytes)
		SetFrameBufferStartAddr(0xC1000000); 
	}
	else if(type == BOARD_PST3)
	{
		SetTextureMemAddr(0xE0000000 ,0x4000000);//8
		//set frame buffer(max 6M bytes)
		SetFrameBufferStartAddr(0xC1000000); 
	}
	SetBoardType(type);
	if(vgSetScreenMode(w,h,SCREENMODE_DOUBLEBUF) == 0) 
	{
		return false;
	}
	vgClearScreen(0,0,0);
	vgFlip();
	// disable H264 HCLK/SCK Clock
	*(volatile unsigned int *)0xFFE02030 = 0xfffffff8;
	return true;
}

void BoardInit::InitFileSystem()
{
	U32 block = ReadStartBlockNumber();
	vgSetNandOffset(block);
	SetFATStartBlockNum(block);
	InitNandMem();
	vgSetNandCfg(0,1,1,2,1); // 100Mhz
	vgFileInit(); 	
}

void BoardInit::InitAllInterrupt(int channel)
{
	SetNonCacheArea();
	InitInterrupt();
	#ifdef UART_DEBUG
		if(channel == 0)
		{
			SetUserDefineDebugFunction(Uart0PutString);
			UartConfig(0,115200);
		}
		else if(channel == 1)
		{
			SetUserDefineDebugFunction(Uart1PutString);
			gpio_set_port_output(45);
			UartConfig(1,115200);
			gpio_output_high(45);
		}
	#endif
}

void BoardInit::Init(int type,int w,int h,int channel)
{
	InitAllInterrupt(channel);
	InitFileSystem();
	InitDisplay(type,w,h);
	InitSound(type);
}

void BoardInit::InitSound(int type)
{
	if(type == BOARD_PST1)
	{
		SndInitMemory(0xC1000000,0x1000000);
	}
	else if(type == BOARD_PST2 || type == BOARD_PST3)
	{
		SndInitMemory(0xC2000000,0x2000000);
	}
	vgSoundInit();
}
