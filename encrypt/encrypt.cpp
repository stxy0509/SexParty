#include "encrypt.h"
#include "i2c.h"
#include <libst.h>
#ifndef USER_CONFIG
#define USER_CONFIG 1
#endif
#include <FS8806.h>
namespace ENCRYPT 
{
	U8 chipID[3];
	U8 key[24] =
	{
		0x48,0x26,0x68,0x70,0x73,0x74,0x65,0x72,0x74,0x65,
		0x63,0x68,0x6E,0x6F,0x6C,0x6F,0x67,0x79,0x31,0x31,
		0x30,0x36,0x31,0x36
	};
	U8 response[8];
	U8 challenge[8] =
	{
		0x88,0x99,0xaa,0x11,0x22,0x33,0x44,0xff
	};
	//Reset Command
	U8 RstCmd[10]=
	{
		0x64,0xcb,0x39,0x48,0x7e,0xc7,0xf4,0x06,0x48,0x93
	};
	// Get Reset Status Command
	U8 GetRstStat[10]=
	{
		0xb8,0x90,0xc1,0x46,0xa6,0xdd,0x02,0xcf,0x4f,0xb7
	};
	// Get Reset Response Command
	U8 RstRespon[20];
}

C_LINKAGE void ENCRYPT::Delay(unsigned long cnt)
{
    for (;cnt>0; cnt--);
}

C_LINKAGE char ENCRYPT::ReadCode(U8 *s1,U8 *s2, int n)//¶ÁÈ¡´úÂë³ÌÐò
{
    int i;
    U8 *tmp;
    unsigned long addr = (unsigned long)s2;

    if(addr < 0xC0000000)
    {
        addr = (unsigned long)(0xC0000000 + (unsigned long)s2);
    }
    tmp = (U8 *)(addr);
    for(i=0;i<n;i++)
    {
        s1[i]=  tmp[i];
    }
    return 1;
}

C_LINKAGE void  ENCRYPT::Delay10ms(U8 nCnt)
{
    int delay = 0x10000;
    while(nCnt--)
    {
        delay = 0x10000;
        while(delay--)
        {
        }
    }
}

void ENCRYPT::CheckEncrypt()
{
	U8  ret,flag;
	I2c_Init();
    I2c_SendStr(BusSlvAdr,RstCmd,10);
    Delay(0x0000FFFF);
    I2c_SendStr(BusSlvAdr,GetRstStat,10);
    Delay(0x0000FFFF);
    I2c_RcvStr(BusSlvAdr, RstRespon, 10);
    InitFS8806Lib(0, 0xC4, 0, 0, 100, key, 0x88);
    ResetChipOpr();
    GetChipIdOpr(chipID);
    flag = 0;
    while(!flag)
    {
        ret = Authentication(response, challenge, &flag);
    }
    if(ret != 1)
    {
        SDK_printf("Authentication failed!\r\n");
        Eagle_Dead();
    }
}
