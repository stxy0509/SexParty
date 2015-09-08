#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_
#include <libst/linkage.h>
#include <EagleSDK.h>
namespace ENCRYPT
{
	extern U8 chipID[3];
	extern U8 key[24];
	extern U8 response[8];
	extern U8 challenge[8];
	//Reset Command
	extern U8 RstCmd[10];
	// Get Reset Status Command
	extern U8 GetRstStat[10];
	// Get Reset Response Command*/
	extern U8 RstRespon[20];
	C_LINKAGE 
	{
		void Delay(unsigned long cnt);
		char ReadCode(U8 *s1,U8 *s2, int n);
		void Delay10ms(U8 nCnt);
	}
	void CheckEncrypt();
}
#endif
