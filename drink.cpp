#include "drink.h"
#include "game_common.h"
#include "small_game/smallgame.h"
#include <libst.h>
using GAME::C_DRINK;
bool C_DRINK::Loaded = false;
vgSURFACE *C_DRINK::text[2] = {0,};
vgSURFACE *C_DRINK::action[2] = {0,};
C_DRINK::C_DRINK():
amount(0),FrameNumber(0)
{
	soundt = GetRandNumber(6,12)*10;
	sndMusic = C_SMALL_GAME::sndMusic[rand()%2];
	sndFg = C_SMALL_GAME::sndWarm[rand()%2];
	which = rand()%3;
	picNum = (which > 0) ? 1 : 2;
	LoadPictures();
}

C_DRINK::~C_DRINK()
{
	vgSoundStop(sndMusic);
	vgSoundStop(sndFg);
	UnloadPictures();
	ResetGameTip();
}

void C_DRINK::LoadPictures()
{
	if(!Loaded)
	{
		char path[32];
		sprintf(path,"picture/drink/%d/",which + 1);
		
		if(which == 0)
		{
			load_bmp_files(path,action,picNum,0,0,255);
		}
		else
		{
			load_bmp_files(path,action,picNum);
		}
		load_bmp_files("picture/drink/d",text,2,255,255,255);
		Loaded = true;
	}
}

void C_DRINK::UnloadPictures()
{
	if(Loaded)
	{
		ReleaseSurface(text,2);
		ReleaseSurface(action,picNum);
		Loaded = false;
	}
}

bool C_DRINK::Play(int HowMany)
{
	if(FrameNumber == 0)
	{
		amount = HowMany;
		vgSoundPlay(sndMusic,PDATA->Vol*255/250);
	}
	if(FrameNumber == soundt)
	{
		vgSoundPlay(sndFg,PDATA->Vol*255/100);
	}
	if(which == 0)
	{
		int T = FrameNumber % 80;
		vgDrawSurface(window2,256,162,0);
		if(T < 50)
		{
			if(T < 34)
			{
				vgSetTexture(action[0]);
				vgDrawTexture(240,    316,action[0]->w + T - 34,action[0]->h,34 - T,0,0);
				vgSetTexture(action[1]);
				vgDrawTexture(562 - T,219,action[1]->w + T - 34,action[1]->h,0,     0,0);
			}
			else
			{
				vgDrawSurface(action[0],206 + T,316,0);
				vgDrawSurface(action[1],562 - T,219,0);
			}
		}
		else
		{
			vgDrawSurface(action[0],256,316,0);
			vgDrawSurface(action[1],512,219,0);
		}
	}
	else
	{
		vgDrawSurface(action[0],256,162,0);
	}
	vgDrawSurface(text[HowMany],384,352,0);
	DisplayBorder();
	++FrameNumber;
	return DisplayTip();
}

