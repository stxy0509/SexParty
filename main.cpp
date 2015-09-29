#include "main.h"
#include "admin.h"
#include "game.h"
#include "game_common.h"
#include "startinfo.h"
#include "uptimer.h"
#include "downtimer.h"
#include "talk.h"
#include "lock/lock.h"
#include <libst.h>
#include <stdexcept>
#include "encrypt/encrypt.h"
#include "../../soft/EagleClass/error.h"
using GAME::PDATA;
using GAME::PKEY;
bool enterSetting = false;
const char *version = "0003";
C_DEBUG debug(800,600);
namespace
{
	U32 Start;
	U32 Ms = 0, Second = 0;
}

U32 GetSecond()
{
	return Second;
}

void Timer0ISR()
{
	++Ms;
	GAME::PKEY->DealMsKeys();
}

void Timer1ISR()
{
	++Second;
	if(Second & 0x01)
	{
		RunningCueLedOff();
	}
	else
	{
		RunningCueLedOn();
	}
	if(Second % 60 == 0)
	{
		++PDATA->RunTime;
	}
	C_DOWN_TIMER *pTimer = C_DOWN_TIMER::GetTopTimer();
	if(pTimer)
	{
		pTimer->DecTime();
	}
	C_UP_TIMER::DealAllTimer();
	if(PDATA->InDemo)
	{
		GAME::PDEMOTIME->CountTime();
	}
	else              
	{
		GAME::PGAMETIME->CountTime();
	}
}

void ProcessSetting()
{
	if(!enterSetting)
	{
		return;
	}
	using GAME::C_ADMIN;
	bool done = false;
	C_ADMIN *padmin = C_ADMIN::GetSingleton();
	while(!done)
	{
		vgClearScreen(40,40,40);
		done = padmin->Process();
		vgASyncFlip();
		PDATA->Write();
	}
	delete padmin;
}

int main()
{
	using GAME::C_GAME;
	using GAME::KEY_RIGHT;
	using GAME::KEY_FUNCTION;
	using GAME::KEY_ENTER0;
	using GAME::KEY_ENTER1;
	using GAME::KEY_ENTER2;
	using GAME::KEY_ENTER3;
	using GAME::KEY_ENTER4;
	using GAME::KEY_ENTER5;
	using GAME::KEY_ENTER6;
	using GAME::KEY_ENTER7;
	using GAME::KEY_ENTER8;
	using GAME::KEY_ENTER9;
	using GAME::KEY_LOCK;
	using GAME::SubGameStage;
	using GAME::screenw;
	using GAME::screenh;
	
	BoardInit::Init(BOARD_PST2,screenw,screenh);
	ENCRYPT::CheckEncrypt();
	GAME::AllLedOn();
	debug.Load("abc.bmp");
	try 
	{
		PDATA = C_DATA::GetSingleton();
		srand(PDATA->RunTime + PDATA->TotalCharge);
		GAME::InitNumberDigit();
		PKEY = C_KEY::GetSingleton();
		PKEY->AddPressReleaseKeys(12,KEY_RIGHT,KEY_FUNCTION,
			KEY_ENTER0,KEY_ENTER1,KEY_ENTER2,KEY_ENTER3,
			KEY_ENTER4,KEY_ENTER5,KEY_ENTER6,KEY_ENTER7,
			KEY_ENTER8,KEY_ENTER9);
		PKEY->AddLockKeys(KEY_LOCK);
		SetAndEnableTimer(0,Timer0ISR,1);
		SetAndEnableTimer(1,Timer1ISR,1000);
		C_TALK *ptalk = C_TALK::GetSingleton();	
		GAME::LoadResForAdmin();
		if(PKEY->PressKey(KEY_LOCK))
		{
			enterSetting = true;			
		}
		ProcessSetting();
		C_START_INFO::Init();
		GAME::LoadPictureSound();
		C_GAME *PGAME = C_GAME::GetSingleton();
		C_LOCK *plock = new C_LOCK();
		C_START_INFO::Exit();
		if(PDATA->MustDecode1())
		{
			bool f = false;
			while(!f)
			{
				f = plock->Process();
				vgFlip();
			}
		}
		delete plock;
		plock = 0;
		GAME::PGAMETIME = &PDATA->GameTime;
		GAME::PDEMOTIME = new C_CONSUME_TIME(3600);
		GAME::PGAMETIME->Start();
		GAME::PDEMOTIME->Start();
		while(1)
		{
			if(PDATA->MustDecode2())
			{
				C_LOCK *plock = new C_LOCK();
				bool f = false;
				while(!f)
				{
					f = plock->Process();
					vgFlip();
				}
				delete plock;
				plock = 0;
			}
			else
			{
				Start = Ms;
				vgClearScreen(0,0,0);
				PGAME->Run();
				vgASyncFlip();
				if(!PDATA->InDemo)
				{
					PDATA->Write();
				}
				while((Ms - Start) < 50) ;
			}
		}
	} 
	catch(C_ERROR_REPORT &er)
	{
		debug.setMessage(er.what());
		vgClearScreen(0,0,0);
		debug.showMessage(50,50);
		vgFlip();
	}
	catch(std::exception &e)
	{
		debug.setMessage(e.what());
		vgClearScreen(0,0,0);
		debug.showMessage(50,50);
		vgFlip();
	}
	return 0;
}
