#include "admin.h"
#include "para.h"
#include "lock/lock.h"
#include "query.h"
#include "game_common.h"
#include <libst.h>
using GAME::C_ADMIN;
int C_ADMIN::fingerPos[4][2] = 
{
	{398,280},{398,324},{398,370},{398,412}
};
C_ADMIN::C_ADMIN():
PPARA(0),PLOCK(0),PQCHARGE(0),
item(TO_QUERY_CHARGE),
loaded(false),inLock(false),inPara(false),
inQueryCharge(false)
{
	LoadResources();
}

C_ADMIN::~C_ADMIN()
{
	UnloadResources();
}

C_ADMIN *C_ADMIN::GetSingleton()
{
	static C_ADMIN *PADMIN = 0;
	return PADMIN == 0 ? PADMIN = new C_ADMIN() : PADMIN;
}

void C_ADMIN::LoadResources()
{
	if(!loaded)
	{
		nr = LoadImageFile("picture/system/nr.bmp",60,60,60);
		finger = vgLoadImageFile("picture/system/finger.bmp");
		menu = LoadImageFile("picture/system/menu.bmp",0,0,255);
		load_bmp_files("picture/keyhelp/set/",message,2,0,0,255);
		loaded = true;
	}
}

void C_ADMIN::UnloadResources()
{
	if(loaded)
	{
		ReleaseSurface(message,2);
		ReleaseSurface(&menu);
		ReleaseSurface(&finger);
		ReleaseSurface(&nr);
		loaded = false;
	}
}

bool C_ADMIN::Process()
{
	using GAME::PKEY;
	
	if(!inPara && !inQueryCharge && !inLock)
	{
		static int tm = 0;
		++tm;
		if(tmpKeys.empty())
		{
			tmpKeys.insert(DOWN_LED);
			tmpKeys.insert(ENTER_LED);
		}
		vgDrawSurface(menu,384,256,0);
		vgDrawSurface(finger,
			fingerPos[item][0],
			fingerPos[item][1],0);
		
		vgDrawSurface(upArrow,   578, 0,0);
		vgDrawSurface(message[0],546,28,0);
		vgDrawSurface(upArrow,   400, 0,0);
		vgDrawSurface(message[1],370,28,0);
		
		int rt = PDATA->LockCharge - PDATA->TotalCharge;
		if(PDATA->ForSale)
		{
			rt = PDATA->LockCharge;
		}
		if(rt < 0)
		{
			rt = 0;
		}
		if(rt > 6*60 || tm%20 < 10)
		{
			display_num_horizontal(nr,5,rt,396,464,2);
		}
		if(PKEY->PressKey(KEY_DOWN))
		{
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
			++item;
			if(item > TO_EXIT)
			{
				item = TO_QUERY_CHARGE;
			}
		}
		else if(PKEY->PressKey(KEY_ENTER))
		{
			tm = 0;
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
			switch(item)
			{
				case TO_QUERY_CHARGE:
					tmpKeys.clear();
					tmpKeys.insert(9);
					inQueryCharge = true;
					break;
				case TO_SET_PARA:
					inPara = true;
					break;
				case TO_LOCK:
					tmpKeys.erase(ENTER_LED);
					inLock = true;
					break;
				case TO_EXIT:
					tmpKeys.clear();
					item = TO_QUERY_CHARGE;
					SubGameStage = STAGE_NONE;
					return true;
			}
		}
	}
	else if(inQueryCharge)
	{
		if(PQCHARGE == 0)	
		{
			PQCHARGE = new C_QUERY_CHARGE();
		}
		if(PQCHARGE->Process())
		{
			tmpKeys.insert(8);
			inQueryCharge = false;
			delete PQCHARGE;
			PQCHARGE = 0;
		}
	}
	else if(inPara)
	{
		if(PPARA == 0)	
		{
			PPARA = new C_PARA();
		}
		if(PPARA->Process())
		{
			inPara = false;
			delete PPARA;
			PPARA = 0;
		}
	}
	else if(inLock)
	{
		if(PLOCK == 0)	
		{
			PLOCK = new C_LOCK();
		}
		if(PLOCK->Process())
		{
			tmpKeys.insert(8);
			inLock = false;
			delete PLOCK;
			PLOCK = 0;
		}
	}
	return false;
}
