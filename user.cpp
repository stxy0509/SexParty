#include "user.h"
#include "poker.h"
#include "dice.h"
#include "present.h"
#include "startinfo.h"
#include "baseball/baseball.h"
#include "game_common.h"
#include <libst.h>
using GAME::C_USERFUNC;
int C_USERFUNC::IconPos[5][2] = 
{
	{137,134},{387,134},{637,134},
	{262,385},{512,385}
};
C_USERFUNC::C_USERFUNC():
Loaded(false),time(0),sndPlayed(false),
PPOKER(C_POKER::GetSingleton()),
PDICE(C_DICE::GetSingleton()),
PBASEBALL(C_BASEBALL::GetSingleton()),
PPRESENT(C_PRESENT::GetSingleton())
{
	LoadResources();
	FillNameStage();
}

C_USERFUNC::~C_USERFUNC()
{
	UnloadResources();
}

C_USERFUNC *C_USERFUNC::GetSingleton()
{
	static C_USERFUNC *PUF = 0;
	return PUF == 0 ? PUF = new C_USERFUNC() : PUF;
}

void C_USERFUNC::LoadResources()
{
	if(!Loaded)
	{
		caption = vgLoadImageFile("picture/game_icon/caption.bmp");
		sndQuit = LoadWavFile("wav/user/quit.wav");
		sndWait = LoadWavFile("wav/user/wait.wav");
		Loaded = true;
	}
}

void C_USERFUNC::UnloadResources()
{
	if(Loaded)
	{
		ReleaseWave(&sndQuit);
		Loaded = false;
	}
}

void C_USERFUNC::FillNameStage()
{
	nameStage[0] = NameStage(0,STAGE_POKER);
	nameStage[1] = NameStage(1,STAGE_DICE_PK);
	nameStage[2] = NameStage(2,STAGE_BASEBALL);
	nameStage[3] = NameStage(3,STAGE_PRESENT_MANY);
	nameStage[4] = NameStage(4,STAGE_END_GAME);
}

void C_USERFUNC::DisplayFreeGameName()
{
	int i = 0;
	while(i < 5)
	{
		vgDrawSurface(gameIcon[nameStage[i].namei],
			IconPos[i][0],IconPos[i][1],0);
		++i;
	}
}

void C_USERFUNC::SwitchGame(int newGame)
{
	if(PDATA->MainGameStage != newGame)
	{
		switch(PDATA->MainGameStage)
		{
			case STAGE_POKER:
			{
				PPOKER->Reset();
				break;
			}
			case STAGE_DICE_PK:
			{
				PDICE->Reset();
				break;
			}
			case STAGE_BASEBALL:
			{
				PBASEBALL->Reset();
				break;
			}
			case STAGE_PRESENT_MANY:
			{
				PPRESENT->Reset();
				break;
			}
		}
		if(newGame == STAGE_END_GAME)
		{
			C_START_INFO::ConfirmEnd(PDATA->MainGameStage);
		}
		PDATA->MainGameStage = newGame;
	}
}

void C_USERFUNC::Process()
{
	int s,rt;
	int firstMan = PDATA->ChiefMan();
	
	if(!IsTmpKey(firstMan) || tmpKeys.size() > 1)
	{
		tmpKeys.clear();
		tmpKeys.insert(firstMan);
	}
	DisplayFreeGameName();
	DisplayTipNext();
	DisplayPressAnimation(firstMan);
	if(time < 400)
	{
		s = time/100;
		rt = (118 - time%100)/20;
	}
	else
	{
		s = 4;
		rt = (178 - (time - 400)%160)/20;
		if(time == 559)
		{
			time = 0;
		}
	}
	if(PKEY->PressKey(KEY_FUNCTION))
	{
		++s;
		if(s > 4)
		{
			s = 0;
		}
		time = 100*s;
		vgSoundPlay(sndPress,PDATA->Vol*255/100);
	}
	if(time%20 < 10)
	{
		vgDrawSurface(iconFrame,IconPos[s][0],IconPos[s][1],0);
		vgDrawSurface(gameName[nameStage[s].namei],
			IconPos[s][0] + 8,
			IconPos[s][1] + 166,0);
	}
	if(time%100 == 0 && s < 4)
	{
		vgSoundPlay(sndWait,PDATA->Vol*255/100);
	}
	DisplaySmallWatch(rt,IconPos[s][0],IconPos[s][1]);
	++time;
	if(s == 4)
	{
		if(!sndPlayed)
		{
			vgSoundPlay(sndQuit,PDATA->Vol*255/100);
			sndPlayed = true;
		}
		vgDrawSurface(caption,376,132,0);
	}
	else
	{
		sndPlayed = false;
	}
	if(PKEY->PressKey(EnterOfPlayer(firstMan)))
	{
		time = 0;
		tmpKeys.clear();
		sndPlayed = false;
		vgSoundStop(sndWait);
		SubGameStage = STAGE_NONE;
		vgSoundPlay(sndPress,PDATA->Vol*255/100);
		SwitchGame(nameStage[s].stage);
	}
}
