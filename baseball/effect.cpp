#include "effect.h"
#include "../../../soft/EagleClass/error.h"
#include <libst.h>
using GAME::C_EFFECT;
vgSURFACE *C_EFFECT::Effect[2] = {0,};
int C_EFFECT::Loaded = false;
int C_EFFECT::Pos[MAX_PLAYERS][2] = 
{
	{280,293},{280,425},{250,520},{379,520},{612,520},
	{730,520},{755,450},{755,308},{567,210},{447,210}
};
C_EFFECT::C_EFFECT(int pos):
time(0),position(pos)
{
	LoadResources();
	if(!(pos >=0 && pos < 10))
	{
		ReportError("In file %s line %d : pos = %d is invalid,"
			"should be in range [%d,%d]\n",__FILE__,__LINE__,pos,0,9);
	}
	if(pos < 2)
	{
		effect = Effect[1];
		flip = false;
	}
	else if(pos < 6)
	{
		effect = Effect[0];
		flip = false;
	}
	else if(pos < 8)
	{
		effect = Effect[1];
		flip = true;
	}
	else
	{
		effect = Effect[0];
		flip = true;
	}
}

C_EFFECT::~C_EFFECT()
{
	
}

void C_EFFECT::LoadResources()
{
	if(!Loaded)
	{
		Effect[0] = LoadImageFile("picture/baseball/effect_down.bmp",0,255,0);
		Effect[1] = LoadImageFile("picture/baseball/effect_left.bmp",0,255,0);
		Loaded = true;
	}
}

void C_EFFECT::UnloadResources()
{
	if(Loaded)
	{
		ReleaseSurface(Effect,2);
		Loaded = false;
	}
}

void C_EFFECT::Display()
{
	if(time < 10)
	{
		if(flip)
		{
			vgDrawSurface(effect,Pos[position][0],
				Pos[position][1],
				DRAWOPTION_FLIPV | DRAWOPTION_FLIPH);
		}
		else
		{
			vgDrawSurface(effect,Pos[position][0],
				Pos[position][1],0);
		}
		++time;
	}
}
