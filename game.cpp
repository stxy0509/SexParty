#include "game.h"
#include "select_game.h"
#include "select_role.h"
#include "poker.h"
#include "user.h"
#include "admin.h"
#include "goldcoin.h"
#include "present.h"
#include "startinfo.h"
#include "roletip.h"
#include "dice.h"
#include "charge.h"
#include "bonus.h"
#include <libst.h>
#include "baseball/baseball.h"
extern bool canBonus;
using GAME::C_GAME;
using GAME::GAME_STAGE;
C_GAME::C_GAME():
tuneVol(false),loaded(false),
whojq(-1),joinquit(0),
tuneDelay(0),PTIP(0)
{
	LoadPictures();
	PBONUS = new C_BONUS_GAME();
	PCHARGE = C_CHARGE::GetSingleton();
	PDICE = C_DICE::GetSingleton();
	PUSER = C_USERFUNC::GetSingleton();
	PADMIN = C_ADMIN::GetSingleton();
	PPRESENT  = C_PRESENT::GetSingleton();
	PPOKER = C_POKER::GetSingleton();
	PBASEBALL = C_BASEBALL::GetSingleton();
	PSELECT_GAME = C_SELECT_GAME::GetSingleton();
	PSELECT_ROLE = C_SELECT_ROLE::GetSingleton();
}

C_GAME::~C_GAME()
{
	delete PSELECT_ROLE;
	delete PSELECT_GAME;
	delete PPOKER;
	delete PBASEBALL;
	delete PPRESENT;
	delete PDICE;
	delete PUSER;
	delete PADMIN;
	delete PBONUS;
	UnloadPictures();
}

C_GAME *C_GAME::GetSingleton()
{
	static C_GAME *PGAME = 0;
	return PGAME == 0 ? PGAME = new C_GAME() : PGAME;
}

void C_GAME::LoadPictures()
{
	if(!loaded)
	{
		warn = LoadImageFile("picture/tip/warn.bmp",60,60,60);
		demoTip = LoadImageFile("picture/tip/demoTip.bmp",60,60,60);
		volbg = vgLoadImageFile("picture/volbg.bmp");
		point = vgLoadImageFile("picture/point.bmp");
		quit = LoadImageFile("picture/tip/quit.bmp",0,0,255);
		quitmsg = vgLoadImageFile("picture/tip/quitmsg.bmp");
		quitmsg_left = vgLoadImageFile("picture/tip/quitmsg_left.bmp");
		load_bmp_files("picture/keyhelp/u",userMsg,2,0,0,255);
		load_bmp_files("picture/keyhelp/s",sysMsg, 2,0,0,255);
		loaded = true;
	}
}

void C_GAME::UnloadPictures()
{
	if(loaded)
	{
		ReleaseSurface(sysMsg,2);
		ReleaseSurface(userMsg,2);
		ReleaseSurface(&quitmsg_left);
		ReleaseSurface(&quitmsg);
		ReleaseSurface(&quit);
		ReleaseSurface(&point);
		ReleaseSurface(&volbg);
		ReleaseSurface(&demoTip);
		ReleaseSurface(&warn);
		loaded = false;
	}
}

bool C_GAME::CanQuit(int who)
{
	bool isCurrent = false;
	if(PDATA->MainGameStage == STAGE_POKER)
	{
		isCurrent = PPOKER->IsCurrentPlayer(who);
	}
	else if(PDATA->MainGameStage == STAGE_DICE_PK)
	{
		isCurrent = PDICE->IsCurrentPlayer(who);
	}
	else if(PDATA->MainGameStage == STAGE_BASEBALL)
	{
		isCurrent = PBASEBALL->IsCurrentPlayer(who);
	}
	else if(PDATA->MainGameStage == STAGE_PRESENT_MANY)
	{
		isCurrent = PPRESENT->IsCurrentPlayer(who);
	}
	return !isCurrent && !IsActor(who) && !IsTmpActor(who);
}

void C_GAME::DisplayHelpMessage()
{
	if(SubGameStage == STAGE_NONE)
	{
		static U32 time = 0;
		if(PKEY->PressKey(KEY_LOCK))
		{
			vgDrawSurface(upArrow,  960, 0,0);
			vgDrawSurface(sysMsg[1],896,28,0);	
		}
		else if(PDATA->InDemo)
		{
			if(time%30 < 15)
			{
				vgDrawSurface(demoTip,(screenw - demoTip->w)/2,
					(screenh - demoTip->h)/2,0);
			}
		}
		else 
		{
			vgDrawSurface(upArrow,   960,0,0);
			vgDrawSurface(userMsg[1],896,28,0);
			if(PDATA->MainGameStage != STAGE_SELECT_GAME && 
			   PDATA->MainGameStage != STAGE_SELECT_ROLE && 
			   PDATA->MainGameStage != STAGE_END_GAME)
			{
				vgDrawSurface(upArrow,   785,0, 0);
				vgDrawSurface(userMsg[0],760,28,0);
				vgDrawSurface(quit,      10,100,0);
				DisplayJoinMessage();
			}
			int totalT = PGAMETIME->GetTotalSeconds();
			if(totalT > 0 && totalT < WARN_SECONDS)
			{
				int dt = totalT/20;
				if(dt < 20)
				{
					dt = 20;
				}
				if(time%dt < dt/2)
				{
					vgDrawSurface(warn,10,100,0);
				}
			}
		}
		++time;
	}
}

void C_GAME::DealMainGameStage()
{
	switch(PDATA->MainGameStage)
	{	
		case STAGE_SELECT_ROLE:
		{
			PSELECT_ROLE->ShowSelectedRole();
			break;
		}
		case STAGE_SELECT_GAME:
		{
			PSELECT_GAME->ShowSelectedGame();
			break;
		}
		case STAGE_POKER:
		{
			PPOKER->Play();
			break;
		}
		case STAGE_DICE_PK:
		{
			PDICE->Play();
			break;
		}
		case STAGE_BASEBALL:
		{
			PBASEBALL->Play();
			break;
		}
		case STAGE_PRESENT_MANY:
		{
			PPRESENT->Process();
			break;
		}
		case STAGE_END_GAME:
		{
			C_START_INFO::End();
			break;
		}
	}
}

void C_GAME::DealSubGameStage()
{
	if(SubGameStage != STAGE_NONE)
	{
		tuneVol = false;
		tuneDelay = 0;
	}
	switch(SubGameStage)
	{
		case STAGE_CHARGE:
		{
			if(PCHARGE->charge())
			{
				SubGameStage = STAGE_NONE;
			}
			break;
		}
		case STAGE_NONE:
		{
			if(PKEY->PressKey(KEY_LOCK))
			{
				if(PKEY->PressKey(KEY_RIGHT))
				{
					vgSoundPlay(sndPress,PDATA->Vol*255/100);
					if(PDATA->MainGameStage != STAGE_END_GAME && SubGameStage != STAGE_CHARGE)
					{
						SubGameStage = STAGE_CHARGE;
					}
				}
			}
			else if(PKEY->PressKey(KEY_RIGHT))
			{
				static int dir = 1;
				if(PDATA->Vol >= 100)
				{
					dir = -1;
				}
				else if(PDATA->Vol == 0)
				{
					dir = 1;
				}
				vgSoundPlay(sndPress,PDATA->Vol*255/100);
				tuneVol = true;
				tuneDelay = 0;
				if(dir == 1)
				{
					if(PDATA->Vol < 100)
					{
						PDATA->Vol = (PDATA->Vol/20 + 1)*20;
					}
				}
				else
				{
					if(PDATA->Vol >= 20)
					{
						PDATA->Vol = (PDATA->Vol/20 - 1)*20;
					}
					else
					{
						PDATA->Vol = 0;
					}
				}
				SetPlayingSoundVol(PDATA->Vol*255/100);
			}
			
			if(tuneVol)
			{
				++tuneDelay;
				vgDrawSurface(volbg,384,284,0);
				for(int i=0;i<PDATA->Vol/20;i++)
				{
					vgDrawSurface(point,445 + 40*i,285,0);
				}
				if(tuneDelay >= 50)
				{
					tuneVol = false;
					tuneDelay = 0;
				}
			}
			break;
		}
		case STAGE_USER_FUNCTION:
		{
			PUSER->Process();
			break;
		}	
		case STAGE_BONUS_GAME:
		{
			if(PBONUS->Play())
			{
				needBonus = false;
				if(PrevSubStage != STAGE_NONE)
				{
					SubGameStage = PrevSubStage;
					PrevSubStage = STAGE_NONE;
				}
				else
				{
					SubGameStage = STAGE_NONE;
				}
			}
			break;
		}
	}
}

void C_GAME::EnterOrExitDemo()
{
	if(!PDATA->InDemo)
	{
		if(PGAMETIME->TimeOver() &&
		   PDATA->MainGameStage != STAGE_SELECT_GAME &&
		   PDATA->MainGameStage != STAGE_END_GAME) 
		{
			ResetPrevGame();
			PDATA->MainGameStage = STAGE_SELECT_GAME;
		}
	}
	else if(!PGAMETIME->TimeOver() && SubGameStage == STAGE_NONE)
	{
		GAME::needBonus = false;
		PDATA->InDemo = false;
		ResetPrevGame();
		ResetRole();
		PDATA->ClearAllPlayer();
		PDATA->MainGameStage = STAGE_SELECT_ROLE;
	}
}

void C_GAME::DealScoreCoin()
{
	int i;
	static int frm = 0;
	if(SubGameStage == STAGE_NONE)
	{
		++frm;
		if(frm%3 == 0)
		{
			if(!IsScoreUpdateFinish())
			{
				if(!vgSoundIsPlaying(sndChange))
				{
					vgSoundPlay(sndChange,PDATA->Vol*255/100);
				}
				for(i=0;i<MAX_PLAYERS;i++)
				{
					playerScoreDigit[i].process();
					playerIncrementDigit[i].process();
				}
			}
			else
			{
				vgSoundStop(sndChange);
			}
		}
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(PCOIN[i]) 
			{	
				PCOIN[i]->Display();
				if(PCOIN[i]->IsFlyingFinish())
				{
					delete PCOIN[i];
					PCOIN[i] = 0;
				}
			}
		}
	}
}

void C_GAME::DealJoinQuit()
{
	if(PDATA->InDemo)
	{
		return;
	}
	if(SubGameStage == STAGE_NONE && whojq < 0)
	{
		whojq = WhoJoinGame();
		if(whojq < MAX_PLAYERS)
		{
			if(whojq%2 == 0)
			{
				PDATA->Players[whojq].Index = GetFreeMan();
			}
			else
			{
				PDATA->Players[whojq].Index = GetFreeGirl();
			}
			AddPickedRole(PDATA->Players[whojq].Index,whojq%2 == 0);
			PDATA->Players[whojq].Increment = 0;
			PDATA->Players[whojq].Score = 0;
			PDATA->Players[whojq].Deleting = false;
			joinquit = 1;
		}
		else
		{
			whojq = WhoQuitGame();
			if(whojq < MAX_PLAYERS && CanQuit(whojq))
			{
				if(whojq%2 == 0)
				{
					if(PDATA->NumberOfMans() > 1)
					{
						joinquit = -1;
						PDATA->Players[whojq].Deleting = true;
					}
				}
				else
				{
					if(PDATA->NumberOfGirls() > 1)
					{
						joinquit = -1;
						PDATA->Players[whojq].Deleting = true;
					}
				}
				
			}
			if(joinquit >= 0)
			{
				whojq = -1;
			}
		}
	}
	if(joinquit > 0)
	{
		if(PTIP == 0) 
		{
			PTIP = new C_ROLE_TIP(
				whojq%2 == 0 ? C_ROLE_TIP::MAN_JOIN : C_ROLE_TIP::GIRL_JOIN);
		}
		if(PTIP->Display())
		{
			delete PTIP;
			PTIP = 0;
			whojq = -1;
			joinquit = 0;
		}
	}
	else if(joinquit < 0)
	{
		if(PTIP == 0)
		{
			int kt = PKEY->PressTime(PlayersEnter[whojq]);
			if(kt < 2000)
			{
				PDATA->Players[whojq].Deleting = false;
				whojq = -1;
				joinquit = 0;
			}
			else if(kt < JOIN_QUIT_DELAY)
			{
				DisplayQuitMessage(whojq,(999 + JOIN_QUIT_DELAY - kt)/1000);
			}
			else
			{
				PTIP = new C_ROLE_TIP(
					whojq%2 == 0 ? C_ROLE_TIP::MAN_QUIT : C_ROLE_TIP::GIRL_QUIT);
			}
		}
		else
		{
			if(PTIP->Display() && !IsActor(whojq) && !IsTmpActor(whojq))
			{
				delete PTIP;
				PTIP = 0;
				SubPickedRole(PDATA->Players[whojq].Index,whojq%2 == 0);
				PDATA->ClearPlayer(whojq);
				whojq = -1;
				joinquit = 0;
			}
		}
	}
}

void C_GAME::DealSubGameRequest()
{
	if(SubGameStage == STAGE_NONE)
	{
		if(PDATA->MainGameStage != STAGE_END_GAME)
		{
			if(PKEY->PressKey(KEY_LOCK))
			{
				if(PKEY->PressKey(KEY_RIGHT))
				{
					vgSoundPlay(sndAdmin,PDATA->Vol*255/100);
					SubGameStage = STAGE_CHARGE;
				}
			}
		}
		if(PDATA->MainGameStage == STAGE_POKER    ||
		   PDATA->MainGameStage == STAGE_DICE_PK  ||
		   PDATA->MainGameStage == STAGE_BASEBALL ||
		   PDATA->MainGameStage == STAGE_PRESENT_MANY)
		{
			if(PKEY->PressTime(KEY_FUNCTION) > 1500)
			{
				vgSoundPlay(sndUser,PDATA->Vol*255/100);
				PKEY->ZeroPressTime(KEY_FUNCTION);
				if(!PDATA->InDemo)
				{
					SubGameStage = STAGE_USER_FUNCTION;
				}
			}
		}
	}
	if(needBonus)
	{
		if(SubGameStage == STAGE_SYSTEM ||
		   PDATA->MainGameStage == STAGE_END_GAME)
		{
			needBonus = false;
		}
	}
}

void C_GAME::Run()
{
	DealSubGameRequest();
	DealMainGameStage();
	DealSubGameStage();
	DealScoreCoin();
	DisplayHelpMessage();
	DealJoinQuit();
	EnterOrExitDemo();
	ControlLed();
}

void C_GAME::ResetPrevGame()
{
	int i;
	if(PDATA->MainGameStage == STAGE_POKER)
	{
		PPOKER->Reset();
	}
	else if(PDATA->MainGameStage == STAGE_DICE_PK)
	{
		PDICE->Reset();
	}
	else if(PDATA->MainGameStage == STAGE_BASEBALL)
	{
		PBASEBALL->Reset();
	}
	else if(PDATA->MainGameStage == STAGE_PRESENT_MANY)
	{
		PPRESENT->Reset();
	}
	else if(PDATA->MainGameStage == STAGE_SELECT_GAME)
	{
		PSELECT_GAME->Reset();
	}
	else if(PDATA->MainGameStage == STAGE_SELECT_ROLE)
	{
		PSELECT_ROLE->Reset();
	}
	else if(PDATA->MainGameStage == STAGE_END_GAME)
	{
		C_START_INFO::Reset();
	}
	for(i=0;i<MAX_PLAYERS;i++)
	{
		if(PCOIN[i])
		{
			delete PCOIN[i];
			PCOIN[i] = 0;
		}
	}
}

void C_GAME::DisplayQuitMessage(int who,int sec)
{
	int MsgPos[MAX_PLAYERS][2] = 
	{
		{100,201},{100,388},{117,557},{300,557},{554,557},
		{748,557},{844,399},{844,209},{528,131},{339,131}		
	};
	int TimePos[MAX_PLAYERS][2] = 
	{
		{147,269},{147,456},{186,573},{369,573},{623,573},
		{816,573},{860,476},{860,286},{606,179},{417,179}
	};
	switch(who)
	{
		case 0:
		case 1:
			vgDrawSurface(quitmsg_left,MsgPos[who][0],MsgPos[who][1],0);
			display_num_vertical(lednr_s_left,
				1,sec,
				TimePos[who][0],
				TimePos[who][1],1);
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			vgDrawSurface(quitmsg,MsgPos[who][0],MsgPos[who][1],0);
			display_num_horizontal(lednr_s,
				1,sec,
				TimePos[who][0],
				TimePos[who][1],1);
			break;
		case 6:
		case 7:
			vgDrawSurface(quitmsg_left,MsgPos[who][0],MsgPos[who][1],
				DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			display_num_vertical(lednr_s_left,
				1,sec,
				TimePos[who][0],
				TimePos[who][1],1,true);
			break;
		case 8:
		case 9:
			vgDrawSurface(quitmsg,MsgPos[who][0],MsgPos[who][1],
				DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			display_num_horizontal(lednr_s,
				1,sec,
				TimePos[who][0],
				TimePos[who][1],1,true);
			break;
	}
}
