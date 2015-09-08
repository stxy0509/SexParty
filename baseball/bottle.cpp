#include "bottle.h"
#include "../game_common.h"
#include <libst.h>
using GAME::C_BOTTLE;
using GAME::STAGE_NONE;
vgSURFACE *C_BOTTLE::Fly[BOTTLE_NR][2][5];
vgSURFACE *C_BOTTLE::BottleBreak[BOTTLE_NR][2][4];
const int C_BOTTLE::StartPosition[BOTTLE_NR][DIRECTIONS][2] = 
{
	{{484,251},{421,313}},
	{{496,351},{445,403}},	
	{{486,337},{467,354}},	
	{{530,267},{482,314}},	
	{{432,277},{395,315}},
	{{529,346},{513,362}},
	{{435,327},{356,404}},
	{{412,301},{413,339}}
};
const int C_BOTTLE::EndPosition[BOTTLE_NR][MAX_PLAYERS][2] = 
{
	{
		{ 68,291},{ 68,429},{196,531},{334,531},{623,531},
		{759,531},{787,428},{787,290},{549, 67},{412, 67}
	},
	{
		{ 74,292},{ 74,430},{198,546},{336,546},{626,546},
		{761,546},{798,430},{798,292},{551, 78},{413, 78}
	},
	{
		{114,287},{114,424},{191,567},{329,567},{619,567},
		{754,567},{825,424},{825,286},{544,111},{406,111}
	},
	{
		{ 84,290},{ 84,428},{196,545},{334,545},{624,545},
		{759,545},{803,427},{803,289},{550, 81},{412, 81}
	},
	{
		{ 92,289},{ 92,427},{195,554},{333,554},{623,554},
		{758,554},{810,428},{810,290},{548, 91},{410, 91}
	},
	{
		{130,276},{130,413},{182,531},{320,531},{610,531},
		{745,531},{787,414},{787,276},{533,128},{395,128}
	},
	{
		{ 55,294},{ 55,431},{199,520},{337,520},{627,520},
		{762,520},{779,432},{779,294},{552, 50},{414, 50}
	},
	{
		{120,266},{120,405},{172,520},{311,520},{598,520},
		{736,520},{777,405},{777,266},{525,118},{387,118}
	}
};
int C_BOTTLE::Loaded = false;
C_BOTTLE::C_BOTTLE(BOTTLE_NAME name,int to):
time(0),state(FLYING)
{
	value = (name != BOTTLE_MAGIC) ? (name + 1) : 15;
	LoadResources();
	DecideFlyBreakPictures(name,to);
}

C_BOTTLE::~C_BOTTLE()
{
	
}

void C_BOTTLE::LoadResources()
{
	if(!Loaded)
	{
		int i,j;
		const char fly_path[]   = "picture/baseball/fly/";
		const char break_path[] = "picture/baseball/break/";
		
		char path1[32];
		char path2[32];
		for(i=0;i<BOTTLE_NR;i++)
		{
			memset(path1,0,sizeof(path1));
			memset(path2,0,sizeof(path2));
			strcpy(path1,fly_path);
			strcpy(path2,break_path);
			sprintf(path1 + strlen(fly_path),  "%d/",i + 1);
			sprintf(path2 + strlen(break_path),"%d/",i + 1);
			for(j=0;j<DIRECTIONS;j++)
			{
				sprintf(path1 + strlen(fly_path)   + 2,"%d/",j + 1);
				sprintf(path2 + strlen(break_path) + 2,"%d/",j + 1);
				load_bmp_files(path1,Fly[i][j],FLY_ACTIONS,          0,255,0);
				load_bmp_files(path2,BottleBreak[i][j],BREAK_ACTIONS,0,255,0);
			}
		}
		Loaded = true;
	}
}

void C_BOTTLE::UnloadResources()
{
	if(Loaded)
	{
		int i,j;
		for(i=0;i<BOTTLE_NR;i++)
		{
			for(j=0;j<DIRECTIONS;j++)
			{
				ReleaseSurface(Fly[i][j],        FLY_ACTIONS);
				ReleaseSurface(BottleBreak[i][j],BREAK_ACTIONS);
			}
		}
		Loaded = false;
	}
}

void C_BOTTLE::DecideFlyBreakPictures(BOTTLE_NAME name,int to)
{
	int i;
	if(to == 0 || to == 1)
	{
		for(i=0;i<FLY_ACTIONS;i++)
		{
			fly[i] = Fly[name][RIGHT][i];
		}
		for(i=0;i<BREAK_ACTIONS;i++)
		{
			bottlebreak[i] = BottleBreak[name][RIGHT][i];
		}
		dx = (EndPosition[name][to][0] - StartPosition[name][RIGHT][0])*1.0f/80;
		dy = (EndPosition[name][to][1] - StartPosition[name][RIGHT][1])*1.0f/80;
		x0 = StartPosition[name][RIGHT][0];
		y0 = StartPosition[name][RIGHT][1];
		flip = true;
	}
	else if(to == 2 || to == 3 || to == 4 || to == 5)
	{
		for(i=0;i<FLY_ACTIONS;i++)
		{
			fly[i] = Fly[name][DOWN][i];
		}
		for(i=0;i<BREAK_ACTIONS;i++)
		{
			bottlebreak[i] = BottleBreak[name][DOWN][i];
		}
		dx = (EndPosition[name][to][0] - StartPosition[name][DOWN][0])*1.0f/80;
		dy = (EndPosition[name][to][1] - StartPosition[name][DOWN][1])*1.0f/80;
		x0 = StartPosition[name][DOWN][0];
		y0 = StartPosition[name][DOWN][1];
		flip = false;
	}
	else if(to == 6 || to == 7)
	{
		for(i=0;i<FLY_ACTIONS;i++)
		{
			fly[i] = Fly[name][RIGHT][i];
		}
		for(i=0;i<BREAK_ACTIONS;i++)
		{
			bottlebreak[i] = BottleBreak[name][RIGHT][i];
		}
		dx = (EndPosition[name][to][0] - StartPosition[name][RIGHT][0])*1.0f/80;
		dy = (EndPosition[name][to][1] - StartPosition[name][RIGHT][1])*1.0f/80;
		x0 = StartPosition[name][RIGHT][0];
		y0 = StartPosition[name][RIGHT][1];
		flip = true;
	}
	else
	{
		for(i=0;i<FLY_ACTIONS;i++)
		{
			fly[i] = Fly[name][DOWN][i];
		}
		for(i=0;i<BREAK_ACTIONS;i++)
		{
			bottlebreak[i] = BottleBreak[name][DOWN][i];
		}
		dx = (EndPosition[name][to][0] - StartPosition[name][DOWN][0])*1.0f/80;
		dy = (EndPosition[name][to][1] - StartPosition[name][DOWN][1])*1.0f/80;
		x0 = StartPosition[name][DOWN][0];
		y0 = StartPosition[name][DOWN][1];
		flip = true;
	}
	x = x0;
	y = y0;
	w = fly[0]->w;
	h = fly[0]->h;
}

bool C_BOTTLE::OutOfScreen()
{
	return (x < -w) || x > screenw || (y < -h) || y > screenh;
}

void C_BOTTLE::Move()
{
	if(state == FLYING)
	{
		x = static_cast<int>(x0 + time*dx);
		y = static_cast<int>(y0 + time*dy);
		if(flip)
		{
			vgDrawSurface(fly[time/5%FLY_ACTIONS],x,y,
				DRAWOPTION_FLIPV | DRAWOPTION_FLIPH);
		}
		else
		{
			vgDrawSurface(fly[time/5%FLY_ACTIONS],x,y,0);
		}
	}
	else if(state == BREAKED)
	{
		if(flip)
		{
			vgDrawSurface(bottlebreak[time/5%BREAK_ACTIONS],x,y,
				DRAWOPTION_FLIPV | DRAWOPTION_FLIPH);
		}
		else
		{
			vgDrawSurface(bottlebreak[time/5%BREAK_ACTIONS],x,y,0);
		}
		if(time == 5*BREAK_ACTIONS - 1)
		{
			state = DEAD;
		}
	}
	if(OutOfScreen())
	{
		state = DEAD;
	}
	if(SubGameStage == STAGE_NONE)
	{
		++time;
	}
}
