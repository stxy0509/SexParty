#include "select_game.h"
#include "game_common.h"
#include "startinfo.h"
#include <libst.h>
using GAME::C_SELECT_GAME;
using GAME::nr;
int C_SELECT_GAME::FramePos[4][2] =
{
	{8,382},{270,382},{544,382},{815,382}
};
int C_SELECT_GAME::TextPos[4][2] =
{
	{0,544},{256,544},{512,544},{768,544}
};
C_SELECT_GAME::C_SELECT_GAME():
sndPlayed(false),stage(0),SelectedGame(POKER),
Loaded(false),FrameNumber(0),
demoGame(POKER),chargeOrQuit(0)
{
	LoadResources();
}

C_SELECT_GAME::~C_SELECT_GAME()
{
	UnloadResources();
}

void C_SELECT_GAME::LoadResources()
{
	if(!Loaded)
	{
		load_bmp_files("picture/select_game/",bg,2);
		option1 = vgLoadImageFile("picture/select_game/t1.bmp");
		option2 = vgLoadImageFile("picture/select_game/t2.bmp");
		text = vgLoadImageFile("picture/select_game/text.bmp");
		helpText = vgLoadImageFile("picture/select_game/helptext.bmp");
		frame = LoadImageFile("picture/select_game/frame.bmp",0,255,0);
		frame2 = LoadImageFile("picture/select_game/frame2.bmp",54,54,54);
		sndSelect = LoadWavFile("wav/game/select.wav");
		Loaded = true;
	}
}

void C_SELECT_GAME::UnloadResources()
{
	if(Loaded)
	{
		ReleaseWave(&sndSelect);
		ReleaseSurface(&frame);
		ReleaseSurface(&frame2);
		ReleaseSurface(&helpText);	
		ReleaseSurface(&text);
		ReleaseSurface(&option1);
		ReleaseSurface(&option2);
		ReleaseSurface(bg,2);
		Loaded = false;
	}
}

C_SELECT_GAME * C_SELECT_GAME::GetSingleton()
{
	static C_SELECT_GAME *PSELECT_GAME = 0;
	return PSELECT_GAME == 0 ? PSELECT_GAME = new C_SELECT_GAME() : PSELECT_GAME;
}

void C_SELECT_GAME::ShowSelectedGame()
{
	if(!sndPlayed)
	{
		vgSoundPlay(sndSelect,PDATA->Vol*255/100);
		sndPlayed = true;
	}
	if(!vgSoundIsPlaying(sndBegin))
	{
		vgSoundPlay(sndBegin,PDATA->Vol*255/250);
	}
	Draw1024X768Splitly(bg);	
	vgDrawSurface(helpText,0,625,0);
	DisplayGameTime(10,30);
	if(!PDATA->InDemo && PGAMETIME->TimeOver())
	{
		int FramePos[2][2][2] = 
		{
			{{283,375},{283,412}},
			{{570,355},{570,318}}
		};
		int TimePos[2][2][2] = 
		{
			{{420,381},{420,419}},
			{{589,362},{589,325}}
		};
		if(stage == 0)
		{
			int firstMan = PDATA->ChiefMan();
			if(!IsInKeys(firstMan) || keys.size() > 1)
			{
				keys.clear();
				keys.insert(firstMan);
			}
			DisplayPressAnimation(firstMan);
			vgDrawSurface(option1,231,256,0);
			vgDrawSurface(option1,537,256,DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			
			vgDrawSurface(frame2,
				FramePos[0][chargeOrQuit][0],
				FramePos[0][chargeOrQuit][1],0);
			vgDrawSurface(frame2,
				FramePos[1][chargeOrQuit][0],
				FramePos[1][chargeOrQuit][1],0);
			display_num_horizontal(nr,1,(118 - FrameNumber%100)/20,
				TimePos[0][chargeOrQuit][0],
				TimePos[0][chargeOrQuit][1],0);
			display_num_horizontal(nr,1,(118 - FrameNumber%100)/20,
				TimePos[1][chargeOrQuit][0],
				TimePos[1][chargeOrQuit][1],0,true);
			if(FrameNumber%100 == 99)
			{
				chargeOrQuit = 1 - chargeOrQuit;
			}
			if(SubGameStage == STAGE_NONE && PKEY->PressKey(EnterOfPlayer(firstMan)))
			{
				vgSoundPlay(sndPress,PDATA->Vol*255/100);
				FrameNumber = 0;
				stage = (chargeOrQuit == 0) ? 2 : 1;
				if(stage == 2)
				{
					SubGameStage = STAGE_CHARGE;
				}
				chargeOrQuit = 0;
			}
		}
		else if(stage == 1) //退出
		{
			int firstMan = PDATA->ChiefMan();
			if(!IsInKeys(firstMan) || keys.size() > 1)
			{
				keys.clear();
				keys.insert(firstMan);
			}
			DisplayPressAnimation(firstMan);
			vgDrawSurface(option2,231,256,0);
			vgDrawSurface(option2,537,256,DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			
			vgDrawSurface(frame2,
				FramePos[0][chargeOrQuit][0],
				FramePos[0][chargeOrQuit][1],0);
			vgDrawSurface(frame2,
				FramePos[1][chargeOrQuit][0],
				FramePos[1][chargeOrQuit][1],0);
			display_num_horizontal(nr,1,(118 - FrameNumber%100)/20,
				TimePos[0][chargeOrQuit][0],
				TimePos[0][chargeOrQuit][1],0);
			display_num_horizontal(nr,1,(118 - FrameNumber%100)/20,
				TimePos[1][chargeOrQuit][0],
				TimePos[1][chargeOrQuit][1],0,true);
			if(FrameNumber%100 == 99)
			{
				chargeOrQuit = 1 - chargeOrQuit;
			}
			if(PKEY->PressKey(EnterOfPlayer(firstMan)) && SubGameStage == STAGE_NONE)
			{
				vgSoundPlay(sndPress,PDATA->Vol*255/100);
				FrameNumber = 0;
				stage = (chargeOrQuit == 0) ? 0 : 3;
				chargeOrQuit = 0;
			}
		}
		else if(stage == 2) //充值
		{
			if(SubGameStage == STAGE_NONE)
			{
				stage = 0;
				FrameNumber = 0;
			}
		}
		else if(stage == 3)	//确认退出
		{
			stage = 0;
			FrameNumber = 0;
			C_START_INFO::DirectEnd();
			vgSoundStop(sndBegin);
			vgSoundStop(sndToCharge);
			PDATA->MainGameStage = STAGE_END_GAME;
		}
	}
	else
	{
		if(FrameNumber%25 < 12)
		{
			vgDrawSurface(frame,
				FramePos[SelectedGame][0],
				FramePos[SelectedGame][1],0);
			DrawPartOfSurface(text,1,4,SelectedGame,
				TextPos[SelectedGame][0],
				TextPos[SelectedGame][1]);
		}
		SelectGame();
	}
	if(SubGameStage == STAGE_NONE) 
	{
		++FrameNumber;
	}
}

void C_SELECT_GAME::Reset()
{
	vgSoundStop(sndBegin);
	vgSoundStop(sndSelect);
	FrameNumber = 0;
	sndPlayed = false;
}

void C_SELECT_GAME::SelectGame()
{
	if(!PDATA->InDemo)
	{
		int TimePos[4][2] = 
		{
			{45,310},{315,310},{585,310},{860,310}
		};
		int firstMan = PDATA->ChiefMan();
		if(!IsInKeys(firstMan) || keys.size() > 1)
		{
			keys.clear();
			keys.insert(firstMan);
		}
		if(SubGameStage == STAGE_NONE && PKEY->PressKey(EnterOfPlayer(firstMan)))
		{
			keys.clear();
			sndPlayed = false;
			vgSoundStop(sndBegin);
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
			switch(SelectedGame)
			{
				case POKER:
					PDATA->MainGameStage = STAGE_POKER;
					break;
				case DICE_PK:
					PDATA->MainGameStage = STAGE_DICE_PK;
					break;	
				case BASEBALL:
					PDATA->MainGameStage = STAGE_BASEBALL;
					break;
				case BIG_PRESENT:
					PDATA->MainGameStage = STAGE_PRESENT_MANY;
					break;		
			}
		}
		DisplaySmallWatch((118 - FrameNumber%100)/20,
			TimePos[SelectedGame][0],
			TimePos[SelectedGame][1]);
		DisplayTipNext();
		DisplayPressAnimation(firstMan);
		SelectedGame = FrameNumber/100%4;
		if(PKEY->PressKey(KEY_FUNCTION))
		{
			++SelectedGame;
			if(SelectedGame > 3)
			{
				SelectedGame = 0;
			}
			FrameNumber = SelectedGame*100;
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
		}
	}
	else
	{
		static int time = 0;
		if(SubGameStage == STAGE_NONE)
		{
			int x = rand()%150;
			if(x < 2)
			{
				++SelectedGame;
				if(SelectedGame > BIG_PRESENT)
				{
					SelectedGame = POKER;
				}
			}
			++time;
			if(time >= 300 && (SelectedGame == demoGame))
			{
				time = 0;
				++demoGame;
				sndPlayed = false;
				vgSoundStop(sndBegin);
				if(demoGame > BIG_PRESENT)
				{
					demoGame = POKER;
				}
				switch(SelectedGame)
				{
					case POKER:
						PDATA->MainGameStage = STAGE_POKER;
						break;
					case DICE_PK:
						PDATA->MainGameStage = STAGE_DICE_PK;
						break;	
					case BASEBALL:
						PDATA->MainGameStage = STAGE_BASEBALL;
						break;
					case BIG_PRESENT:
						PDATA->MainGameStage = STAGE_PRESENT_MANY;
						break;	
				}
			}
		}
	}
}
