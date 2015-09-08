#ifndef MACHINE_TALK_H
#define MACHINE_TALK_H
#include "uart.h"
#include <list>
class C_TALK : public C_UART
{
private:
	enum COMMAND_BYTE
	{
		RECE_STRAT = 0xCA,
		SEND_START = 0xC9,
		END_BYTE = 0xCB,
		
		SET_PARA = 0xD0,
	};
	static int nextWait;
	static const int POWER1 = 1;
	static const int POWER2 = POWER1*200;
	static const int POWER3 = POWER2*200;
	static const int POWER4 = POWER3*200;
	C_TALK();
	~C_TALK();
	static C_TALK *ptalk;
	static void TalkISR();
public:
	static C_TALK *GetSingleton();
	bool IsReceivedDataValid();
	void ReplyOrUse();
};
#endif
