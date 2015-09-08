#include "bottleinfo.h"
#include <libst.h>
#include "../../../EagleClass/error.h"
using GAME::C_BOTTLE_INFO;
vgSURFACE *C_BOTTLE_INFO::Number[2];
vgSURFACE *C_BOTTLE_INFO::Plus[2];
int C_BOTTLE_INFO::Loaded = false;
int C_BOTTLE_INFO::PlusPos[MAX_PLAYERS][2] = 
{
	{280,293},{280,425},{250,520},{379,520},{612,520},
	{730,520},{755,450},{755,308},{567,210},{447,210}
};
C_BOTTLE_INFO::C_BOTTLE_INFO(int n,int pos):
time(0),position(pos),value(n)
{
	if(!(pos >= 0 && pos <= 9))
	{
		ReportError("In file %s line %d : pos = %d is invalid,"
					"should be in range [%d,%d]\n",__FILE__,__LINE__,n,0,9);
	}
	LoadResources();

	if(pos < 2)
	{
		flip = false;
		dx = 0;
		dy = 1;
		number = new vgSURFACE(*Number[1]);
		plus = new vgSURFACE(*Plus[1]);
	}
	else if(pos < 6)
	{
		flip = false;
		dx = 1;
		dy = 0;
		number = new vgSURFACE(*Number[0]);
		plus = new vgSURFACE(*Plus[0]);
	}
	else if(pos < 8)
	{
		flip = true;
		dx = 0;
		dy = -1;
		number = new vgSURFACE(*Number[1]);
		plus = new vgSURFACE(*Plus[1]);
	}
	else
	{
		flip = true;
		dx = -1;
		dy = 0;
		number = new vgSURFACE(*Number[0]);
		plus = new vgSURFACE(*Plus[0]);
	}
	x = PlusPos[pos][0];
	y = PlusPos[pos][1];
}

C_BOTTLE_INFO::~C_BOTTLE_INFO()
{
	delete number;
	delete plus;
}

void C_BOTTLE_INFO::LoadResources()
{
	if(!Loaded)
	{
		int i;
		Number[0] = vgLoadImageFile("picture/baseball/number/down/number.bmp");
		Number[1] = vgLoadImageFile("picture/baseball/number/left/number.bmp");
		Plus[0] = vgLoadImageFile("picture/baseball/number/down/plus.bmp");
		Plus[1] = vgLoadImageFile("picture/baseball/number/left/plus.bmp");
		for(i=0;i<2;i++)
		{
			vgSetColorKey(Number[i],0,255,0);
			vgSetDrawMode(Number[i],DRAWMODE_TRANSPARENCY|DRAWMODE_ALPHA);
			vgSetColorKey(Plus[i],0,255,0);
			vgSetDrawMode(Plus[i],DRAWMODE_TRANSPARENCY|DRAWMODE_ALPHA);
		}
		Loaded = true;
	}
}

void C_BOTTLE_INFO::UnloadResources()
{
	if(Loaded)
	{
		ReleaseSurface(Number,2);
		ReleaseSurface(Plus,2);
		Loaded = false;
	}
}

void C_BOTTLE_INFO::Display()
{
	if(time < 40)
	{
		U32 option = 0;
		if(time < 20)
		{
			
		}
		else
		{
			if(plus->alpha >= 13)
			{
				plus->alpha   -= 13;
				number->alpha -= 13;
			}
			else if(plus->alpha > 0)
			{
				plus->alpha   = 0;
				number->alpha = 0;
			}
		}
		if(flip) 
		{
			option = DRAWOPTION_FLIPV | DRAWOPTION_FLIPH;
		}
		vgDrawSurface(plus,  x,          y,          option);
		if(position == 0 || position == 1)
		{
			display_num_vertical(number,1,value,x + dx * 14,y + 14 * dy,1,flip);
		}
		else if(position == 6 || position == 7)
		{
			int n = GetDigitsOfNumber(value);
			display_num_vertical(number,1,value,x + dx * 14,y + n*14 * dy,1,flip);
		}
		else if(position == 2 || position == 3 || position == 4 || position == 5)
		{
			display_num_horizontal(number,1,value,x + dx * 14,y + 14 * dy,1,flip);
		}
		else
		{
			int n = GetDigitsOfNumber(value);
			display_num_horizontal(number,1,value,x + n * dx * 14,y + 14 * dy,1,flip);
		}
	}
	++time;
}
