#ifndef _BOARD_INIT_H_
#define _BOARD_INIT_H_
#include <libst.h>
#include <EagleSDK.h>
class BoardInit 
{
public:
	static void Init(int type,int w,int h,int channel = -1);
private:
	static bool InitDisplay(int type,int w,int h);
	static void InitFileSystem();
	static void InitSound(int type);
	static void InitAllInterrupt(int channel);
	static void Uart0PutString(char *s);
	static void Uart1PutString(char *s);
};
#endif
