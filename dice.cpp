#include "dice.h"
#include "diceh.h"
#include "drink.h"
#include "game_common.h"
#include "small_game/smallgame.h"
#include <libst.h>
using GAME::C_DICE;
using GAME::C_DRINK;
using GAME::C_DICE_HANDLE;
int C_DICE::CupPos[MAX_PLAYERS][2] = 
{
	{ 72,174},{ 72,520},{ 87,634},{435,634},{534,634},
	{879,634},{888,525},{888,180},{660, 76},{313, 76}
};
C_DICE::C_DICE():
Loaded(false),Stage(0),
DrinkType(DRINK_NONE),DrinkTimes(1),
m_time(0),sndbgPlayed(false)
{
	LoadResources();
	C_DICE_HANDLE::LoadResources();
	for(int i=0;i<MAX_PLAYERS;i++)
	{
		PHDICE[i] = 0;
	}
	if(rand()%100 < 30)
	{
		DrinkTimes = 2;
	}
}

C_DICE::~C_DICE()
{
	UnloadResources();
}

void C_DICE::LoadResources()
{
	if(!Loaded)
	{
		load_bmp_files("picture/dice/cup/down/",cup,2,0,0,255);
		load_bmp_files("picture/dice/cup/left/",cup_left,2,0,0,255);
		load_bmp_files("picture/dice/bg/",bg,2);
		caption = vgLoadImageFile("picture/dice/caption.bmp");
		sndStart = LoadWavFile("wav/dice/start.wav");
		Loaded = true;
	}
}

void C_DICE::UnloadResources()
{
	if(Loaded)
	{
		ReleaseWave(&sndStart);
		ReleaseSurface(bg,2);
		ReleaseSurface(cup_left,2);
		ReleaseSurface(cup,2);
		Loaded = false;
	}
}

void C_DICE::DisplayDrinker(int n)
{
	if(n < 0 || n >= MAX_PLAYERS) 
	{
		return;
	}
	if(n < 2)
	{
		vgDrawSurface(cup_left[DrinkType],CupPos[n][0],CupPos[n][1],0);
	}
	else if(n < 6)
	{
		vgDrawSurface(cup[DrinkType],CupPos[n][0],CupPos[n][1],0);
	}
	else if(n < 8)
	{
		vgDrawSurface(cup_left[DrinkType],CupPos[n][0],CupPos[n][1],
			DRAWOPTION_FLIPH);
	}
	else
	{
		vgDrawSurface(cup[DrinkType],CupPos[n][0],CupPos[n][1],
			DRAWOPTION_FLIPV);
	}
}

void C_DICE::FlagDrinkers()
{
	if(Stage == 1 && !actor.empty())
	{
		SET_IT it = actor.begin();
		while(it != actor.end())
		{
			DisplayDrinker(*it);
			++it;
		}
	}
}

void C_DICE::PlayDice()
{
	int i;
	for(i=0;i<MAX_PLAYERS;i++)
	{
		if(PDATA->SeatHaveRole(i))
		{
			if(PHDICE[i] == 0)
			{
				PHDICE[i] = new C_DICE_HANDLE(i);
			}
			else
			{
				PHDICE[i]->Play();
			}
		}
		else if(PHDICE[i] != 0)
		{
			delete PHDICE[i];
			PHDICE[i] = 0;
		}
	}
}

void C_DICE::WorkAfterAct()
{
	Stage = 1;
	m_time = 0;
	actor.clear();
	gmRank.clear();
	C_DICE_HANDLE::AllReset();
	for(int i=0;i<MAX_PLAYERS;i++)
	{
		if(PDATA->SeatHaveRole(i))
		{
			PDATA->Players[i].Score += PDATA->Players[i].Increment;
			PDATA->Players[i].Increment = 0;
			playerIncrementDigit[i].setValue(0);
			playerScoreDigit[i].setValue(PDATA->Players[i].Score);
		}
	}
	if(PDATA->InDemo)
	{
		Stage = 0;
		vgSoundStop(sndPokerDice);
		sndbgPlayed = false;
		PDATA->MainGameStage = STAGE_SELECT_GAME;
	}
}

void C_DICE::Play()
{
	int i;
	if(!sndbgPlayed)
	{
		keys.clear();
		stop_all_sound();
		vgSoundPlay(sndPokerDice,PDATA->Vol*255/250);
		vgSoundPlay(sndStart,PDATA->Vol*255/100);
		sndbgPlayed = true;
	}
	SetUsableKeys();
	Draw1024X768Splitly(bg);
	FlagActors();
	FlagDrinkers();
	GAME::ShowSitRole();
	DisplaySitRoleName();
	DisplayPlayerScore();
	DisplayGameTime(10,30);
	DisplayFirstFlag();
	if(Stage != 0)
	{
		PlayDice();
	}
	if(Stage == 0)
	{
		if(SubGameStage == STAGE_NONE)
		{
			++m_time;
		}
		DisplaySurfaceCenterOnScreen(caption);
		DisplaySmallWatch(10 - m_time/20,585,267,false);
		if(m_time >= 200 || AnyOnePressEnter())
		{
			++Stage;
			if(m_time < 200)
			{
				vgSoundPlay(sndPress,PDATA->Vol*255/100);
			}
			m_time = 0;
		}
	}
	else if(Stage == 1)
	{
		if(needBonus && m_time == 0)
		{
			if(SubGameStage == STAGE_NONE)
			{
				PrevSubStage = SubGameStage;
				SubGameStage = STAGE_BONUS_GAME;
			}
		}
		else
		{
			if(m_time == 0)
			{
				for(i=0;i<MAX_PLAYERS;i++)
				{
					if(PHDICE[i])
					{
						PHDICE[i]->BeginToPlay();
					}
				}
				m_time = 1;
			}
			if(C_DICE_HANDLE::AllFinish() && GAME::IsCoinFlyingFinish())
			{
				--DrinkTimes;
				if(DrinkTimes >= 0)
				{
					SelectDrinkers();
					Stage = 2;
				}
				else
				{
					DrinkTimes = 1;
					if(rand()%100 < 30)
					{
						DrinkTimes = 2;
					}					
					SelectActors();
					Stage = 3;
				}
				C_DICE_HANDLE::AllReset();
				keys.clear();
			}
		}
	}
	else if(Stage == 2)
	{
		if(pdrink->Play(DrinkType))
		{
			vgSoundResume(sndPokerDice);
			DrinkType = DRINK_NONE;
			delete pdrink;
			pdrink = 0;
			WorkAfterAct();
		}
	}
	else if(Stage == 3)
	{
		if(PSMALLGAME->Play())
		{
			vgSoundResume(sndPokerDice);
			delete PSMALLGAME;
			PSMALLGAME = 0;
			WorkAfterAct();
		}
	}
}

C_DICE *C_DICE::GetSingleton()
{
	static C_DICE *PDICE = 0;
	return PDICE == 0 ? PDICE = new C_DICE() : PDICE;
}

bool C_DICE::IsCurrentPlayer(int who)
{
	return false;
}

void C_DICE::Reset()
{
	Stage = 0;
	m_time = 0;
	actor.clear();
	gmRank.clear();
	vgSoundStop(sndPokerDice);
	vgSoundStop(sndStart);
	sndbgPlayed = false;
	DrinkType = DRINK_NONE;
	C_DICE_HANDLE::AllReset();
	if(PSMALLGAME)
	{
		delete PSMALLGAME;
		PSMALLGAME = 0;
	}
	if(pdrink)
	{
		delete pdrink;
		pdrink = 0;
	}
	for(int i=0;i<MAX_PLAYERS;i++)
	{
		if(PDATA->SeatHaveRole(i))
		{
			PDATA->Players[i].Score += PDATA->Players[i].Increment;
			PDATA->Players[i].Increment = 0;
			playerIncrementDigit[i].setValue(0);
			playerScoreDigit[i].setValue(PDATA->Players[i].Score);
		}
	}
}

void C_DICE::SelectActors()
{
	int i;
	std::list<int> m,g;
	LIST_IT it;
	PSMALLGAME = CreateSmallGame(rand()%2);
	int mans  = PSMALLGAME->NumberOfManActors();
	int girls = PSMALLGAME->NumberOfGirlActors();
	
	for(i=0;i<MAX_PLAYERS;i++)
	{
		if(PDATA->CanAct(i))
		{
			if(i%2 == 0)
			{
				m.push_back(i);
			}
			else
			{
				g.push_back(i);
			}
		}
	}
		
	while(mans > 0 && !m.empty())
	{
		it = m.begin();
		advance(it,rand()%m.size());
		actor.insert(*it);
		m.remove(*it);
		--mans;
	}
	
	while(girls > 0 && !g.empty())
	{
		it = g.begin();
		advance(it,rand()%g.size());
		actor.insert(*it);
		g.remove(*it);
		--girls;
	}
	vgSoundPause(sndPokerDice);
}

void C_DICE::SelectDrinkers()
{
	int rx,numActors;
	int maxActors,minActors;
	int nPlayers = PDATA->NumberOfPlayers();
	if(nPlayers <= 2)
	{
		minActors = 1;
		maxActors = 1;
	}
	else if(nPlayers <= 5)
	{
		minActors = 1;
		maxActors = 2;
	}
	else if(nPlayers <= 7)
	{
		minActors = 2;
		maxActors = 3;
	}
	else
	{
		minActors = 2;
		maxActors = 4;
	}
	numActors = GetRandNumber(minActors,maxActors);
	MAP_IT it = gmRank.begin();
	while(numActors > 0 && it != gmRank.end())
	{
		if(PDATA->CanAct(it->second))
		{
			actor.insert(it->second);
			--numActors;
		}
		++it;
	}
	rx = rand()%60;
	if(rx < 20) 
	{
		DrinkType = DRINK_WHOLE;
	}
	else       
	{
		DrinkType = DRINK_HALF;
	}
	pdrink = new C_DRINK();
	vgSoundPause(sndPokerDice);
}
