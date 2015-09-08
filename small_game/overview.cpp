#include "overview.h"
#include "../game_common.h"
#include <libst.h>
using GAME::C_OVERVIEW;
using GAME::PDATA;
int C_OVERVIEW::index = 0;
C_OVERVIEW::C_OVERVIEW():bg(0)
{
	++index;
	if(index > 3)
	{
		index = 0;
	}
}

C_OVERVIEW::~C_OVERVIEW()
{
	UnloadResources();
}

void C_OVERVIEW::LoadResources()
{
	if(!resLoaded)
	{
		char buff[36];
		sprintf(buff,"picture/small_game/overview/%d.bmp",(index + 1));
		bg = vgLoadImageFile(buff);
		resLoaded = true;
	}
}

void C_OVERVIEW::UnloadResources()
{
	ReleaseSurface(&bg);
}

bool C_OVERVIEW::Play()
{
	if(frameNumber == 0)
	{
		vgSoundPlay(sndMusic[index1],PDATA->Vol*255/250);
	}
	LoadResources();
	vgDrawSurface(bg,256,162,0);
	DisplayBorder();
	if(frameNumber == soundt)
	{
		vgSoundPlay(sndWarm[index2],PDATA->Vol*255/100);
	}
	if(SubGameStage == STAGE_NONE)
	{
		++frameNumber;
	}
	return frameNumber >= 600;
}

int C_OVERVIEW::NumberOfManActors()
{
	return 0;
}

int C_OVERVIEW::NumberOfGirlActors()
{
	return 0;
}
