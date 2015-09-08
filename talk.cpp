#include "talk.h"
#include "game.h"
using GAME::PDATA;
C_TALK* C_TALK::ptalk = 0;
C_TALK::C_TALK():C_UART(1)
{
	SetReceHeadFlag(RECE_STRAT);
	SetReceEndFlag(END_BYTE);
	Configure(57600,TalkISR);
}

C_TALK::~C_TALK()
{
	
}

C_TALK* C_TALK::GetSingleton()
{
	if(ptalk == 0)
	{
		ptalk = new C_TALK();
	}
	return ptalk;
}

void C_TALK::TalkISR()
{
	ptalk->ReceiveDataPassively();
}

bool C_TALK::IsReceivedDataValid()
{
	if(GetByte(1) != SET_PARA)
	{
		return false;
	}
	int i = 2;
	while(GetByte(i) < POWER2)
	{
		++i;
	}
	return GetByte(i) == END_BYTE;
}

void C_TALK::ReplyOrUse()
{
	PDATA->LineNumber    = GetByte(2)*POWER2 + GetByte(3);
	PDATA->MachineNumber = GetByte(4)*POWER4 + GetByte(5)*POWER3 + 
		GetByte(6)*POWER2 + GetByte(7);
	PDATA->LockCharge    = (GetByte(8)*POWER2 + GetByte(9))*60;
	PDATA->ForSale = GetByte(10);
	PDATA->Write();
	U8 buff[4];
	buff[0] = SEND_START;
	buff[1] = SET_PARA;
	buff[2] = 1;
	buff[3] = END_BYTE;
	TransData(buff,4);
}
