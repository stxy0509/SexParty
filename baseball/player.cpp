#include "player.h"
#include "../uptimer.h"
#include "../game_common.h"
#include "../../../soft/EagleClass/error.h"
#include <libst.h>
using GAME::C_PLAYER;
int C_PLAYER::Loaded = false;
vgWAVE *C_PLAYER::sndStrike = 0;
vgSURFACE *C_PLAYER::Actions[PLAYER_TYPES][DIRECTIONS];
int C_PLAYER::PlayerPos[MAX_PLAYERS][2] = 
{
	{-43,205},{-43,340},{113,520},{247,520},{536,520},
	{675,520},{779,265},{779,130},{388,-42},{251,-42}
};
C_PLAYER::C_PLAYER(int pos):
timer(new C_UP_TIMER(4)),CurrentAction(0),
time(0),position(pos),exist(false),IsIdle(true)
{
	C_UP_TIMER::AddTimer(timer);
	LoadResources();
	DecideToUseWhichPlayer(pos);
}

C_PLAYER::~C_PLAYER()
{
	C_UP_TIMER::DelTimer(timer);
	delete timer;
}

void C_PLAYER::LoadResources()
{
	if(!Loaded)
	{
		int i;
		char full_path[64];
		const char path[] = "picture/baseball/player/";
		
		for(i=0;i<PLAYER_TYPES;i++)
		{
			memset(full_path,0,sizeof(full_path));
			strcpy(full_path,path);
			sprintf(full_path + strlen(path),"%d/",i + 1);
			load_bmp_files(full_path,Actions[i],DIRECTIONS,0,255,0);
		}
		sndStrike = LoadWavFile("wav/baseball/play.wav");
		Loaded = true;
	}
}

void C_PLAYER::UnloadResources()
{
	if(Loaded)
	{
		int i;
		ReleaseWave(&sndStrike);
		for(i=0;i<PLAYER_TYPES;i++)
		{
			ReleaseSurface(Actions[i],DIRECTIONS);
		}
		Loaded = false;
	}
}

void C_PLAYER::DecideToUseWhichPlayer(int pos)
{
	int type,dir;
	switch(pos)
	{
		case 0:
		{
			type = YELLOW_PLAYER;
			dir  = RIGHT;
			flip = true;
			break;
		}
		case 1:
		{
			type = RED_PLAYER;
			dir  = RIGHT;
			flip = true;
			break;
		}
		case 2:
		case 4:
		{
			type = YELLOW_PLAYER;
			dir  = DOWN;
			flip = false;
			break;
		}
		case 3:
		case 5:
		{
			type = RED_PLAYER;
			dir  = DOWN;
			flip = false;
			break;
		}
		case 6:
		{
			type = YELLOW_PLAYER;
			dir  = RIGHT;
			flip = false;
			break;
		}
		case 7:
		{
			type = RED_PLAYER;
			dir  = RIGHT;
			flip = false;
			break;
		}
		case 8:
		{
			type = YELLOW_PLAYER;
			dir  = DOWN;
			flip = true;
			break;
		}
		case 9:
		{
			type = RED_PLAYER;
			dir  = DOWN;
			flip = true;
			break;
		}
		default:
		{
			ReportError("In file %s line %d : pos = %d is invalid,"
				"valid range is [%d,%d]\n",__FILE__,__LINE__,pos,0,9);	
		}
	}
	PlayAction = Actions[type][dir];
}

void C_PLAYER::Play()
{
	using GAME::PKEY;
	if(exist)
	{
		if(IsIdle)
		{
			CurrentAction = 0;
			DisplayPlayerAction(6);
			if(!PDATA->InDemo)
			{
				if(SubGameStage != STAGE_NONE)
				{
					timer->Pause();
				}
				else 
				{
					timer->Resume();
					if(PKEY->PressKey(EnterOfPlayer(position)) || 
						timer->GetElapsedTime() == 4)
					{
						vgSoundPlay(sndPress,PDATA->Vol*255/100);
						timer->Start();
						vgSoundPlay(sndStrike,PDATA->Vol*255/100);
						IsIdle = false;
						time = 0;
						return;
					}
				}
			}
			else
			{
				if(SubGameStage == STAGE_NONE && rand() % 150 < 10)
				{
					vgSoundPlay(sndStrike,PDATA->Vol*255/100);
					IsIdle = false;
					time = 0;
					return;
				}
			}
		}
		else
		{
			CurrentAction = time/3%BUSY_ACTIONS;
			DisplayPlayerAction(CurrentAction);
			if(time == 3*BUSY_ACTIONS - 1)
			{
				time = 0;
				IsIdle = true;
				return;
			}
		}
		if(SubGameStage == STAGE_NONE)
		{
			++time;
		}
	}
	else
	{
		DisplayPlayerAction(8);
	}
}

void C_PLAYER::SitQuietly()
{
	CurrentAction = 0;
	DisplayPlayerAction(exist ? 6 : 8);
}

void C_PLAYER::DisplayPlayerAction(int n)
{
	if(flip)
	{
		DrawPartOfSurface(PlayAction,3,3,n,
			PlayerPos[position][0],
			PlayerPos[position][1],
			DRAWOPTION_FLIPV | DRAWOPTION_FLIPH);
	}
	else
	{
		DrawPartOfSurface(PlayAction,3,3,n,
			PlayerPos[position][0],
			PlayerPos[position][1],0);
	}
}

void C_PLAYER::SetPlayerCome()
{
	if(!exist)
	{
		exist = true;
		time = 0;
		IsIdle = true;
		timer->Start();
	}
}

void C_PLAYER::SetPlayerLeave()
{
	if(exist)
	{
		exist = false;
		time = 0;
		IsIdle = true;
		timer->Stop();
	}
}
