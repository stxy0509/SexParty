#include "charge.h"
#include "game_common.h"
#include <libst.h>
using GAME::nr;
using GAME::PKEY;
using GAME::PDATA;
using GAME::sndCharge;
using GAME::sndPress;
using GAME::PGAMETIME;
using GAME::tmpKeys;
using GAME::upArrow;
using GAME::KEY_LOCK;
using GAME::KEY_RIGHT;
using GAME::KEY_DOWN;
using GAME::KEY_ENTER;
vgSURFACE *C_CHARGE::bg = 0;
vgSURFACE *C_CHARGE::cnr = 0;
vgSURFACE *C_CHARGE::warn = 0;
vgSURFACE *C_CHARGE::message[3] = {0,};
C_CHARGE::C_CHARGE():amount(0)
{
	compKeys.insert(8);
	compKeys.insert(9);
}

C_CHARGE::~C_CHARGE()
{
	
}

C_CHARGE *C_CHARGE::GetSingleton()
{
	static C_CHARGE *P = 0;
	if(P == 0) P = new C_CHARGE();
	return P;
}

bool C_CHARGE::charge()
{
	LoadResources();
	if(tmpKeys != compKeys)
	{
		tmpKeys = compKeys;
	}
	
	if(PKEY->PressKey(KEY_LOCK))
	{
		vgDrawSurface(bg,384,320,0);
		display_num_horizontal(cnr,3,amount,
			453,401,1);
		vgDrawSurface(upArrow,   578, 0,0);
		vgDrawSurface(message[0],592,28,0);
		vgDrawSurface(upArrow,   960, 0,0);
		vgDrawSurface(message[1],896,28,0);
		vgDrawSurface(upArrow,   400, 0,0);
		vgDrawSurface(message[2],370,28,0);
		if(PKEY->PressKey(KEY_RIGHT))
		{
			vgSoundPlay(sndCharge,PDATA->Vol*255/100);
			amount += 30;
		}
		if(PKEY->PressKey(KEY_DOWN))
		{
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
			amount = 0;
		}
		if(PKEY->PressKey(KEY_ENTER))
		{
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
			if(amount > 0)
			{
				PDATA->TotalCharge += amount;
				PDATA->CurrentTotal += amount;
				if(PDATA->InDemo)
				{
					if(PGAMETIME->TimeOver())
					{
						PGAMETIME->SetTotalSeconds(amount*60);
					}
					else
					{
						*PGAMETIME += amount*60;
					}
				}
				else
				{
					*PGAMETIME += amount*60;
				}
				PDATA->CurrentCharge[PDATA->CurrentIndex] = amount;
				++PDATA->CurrentIndex;
				PDATA->CurrentIndex %= 30;
				PDATA->Write();
			}
			amount = 0;
			UnloadResources();
			return true;
		}
	}
	else 
	{
		vgDrawSurface(warn,384,320,0);
		if(PKEY->PressKey(KEY_RIGHT))
		{
			tmpKeys.clear();
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
			amount = 0;
			UnloadResources();
			return true;
		}
	}
	return false;
}

void C_CHARGE::LoadResources()
{
	if(bg == 0)
	{
		bg = vgLoadImageFile("picture/charge/bg.bmp");
		cnr = vgLoadImageFile("picture/charge/nr.bmp");
		warn = vgLoadImageFile("picture/charge/warn.bmp");
		load_bmp_files("picture/keyhelp/charge/",message,3,0,0,255);
	}
}

void C_CHARGE::UnloadResources()
{
	if(bg != 0)
	{
		ReleaseSurface(message,3);
		ReleaseSurface(&warn);
		ReleaseSurface(&cnr);
		ReleaseSurface(&bg);
	}
}
