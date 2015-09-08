#include "roletip.h"
#include "game_common.h"
#include <libst.h>
using GAME::C_ROLE_TIP;
vgSURFACE *C_ROLE_TIP::tip[4] = {0,};
bool C_ROLE_TIP::loaded = false;
C_ROLE_TIP::C_ROLE_TIP(int type):time(0)
{
	LoadPictures();
	oneTip = new vgSURFACE(*tip[type]);
}

C_ROLE_TIP::~C_ROLE_TIP()
{
	delete oneTip;
}

void C_ROLE_TIP::LoadPictures()
{
	if(!loaded)
	{
		load_bmp_files("picture/tip/",tip,4,0,255,0);
		for(int i=0;i<4;i++)
		{
			AppendDrawMode(tip[i],DRAWMODE_ALPHA);
		}
		loaded = true;
	}
}

void C_ROLE_TIP::UnloadPictures()
{
	if(loaded)
	{
		ReleaseSurface(tip,4);
		loaded = false;
	}
}

bool C_ROLE_TIP::Display()
{
	if(time < 69)
	{
		vgDrawSurface(oneTip,10,0,0);
	}
	else if(time < 120)
	{
		oneTip->alpha = 255 - 5*(time - 69);
		vgDrawSurface(oneTip,10,0,0);
	}
	++time;
	return time >= 120;
}
