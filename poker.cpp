#include "poker.h"
#include "card.h"
#include "game_common.h"
#include <libst.h>
using GAME::C_POKER;
using GAME::PDATA;
std::set<int> C_POKER::freeCards;
vgWAVE *C_POKER::sndPut = 0;
vgWAVE *C_POKER::sndWash = 0;
vgWAVE *C_POKER::sndStart = 0;
vgSURFACE *C_POKER::bg[2] = {0,};
vgSURFACE *C_POKER::girl[3] = {0,};
vgSURFACE *C_POKER::action = 0;
vgSURFACE *C_POKER::caption = 0;
C_POKER::C_POKER():
m_time(0),m_stage(0),psmall(0),sndbgPlayed(false)
{
	LoadResources();
	ResetFreeCards();
	for(int i=0;i<MAX_PLAYERS;i++)
	{
		pcard[i] = 0;
	}
}

C_POKER::~C_POKER()
{
	
}

C_POKER *C_POKER::GetSingleton()
{
	static C_POKER *PK = 0;
	if(PK == 0) 
	{
		PK = new C_POKER();
	}
	return PK;
}

void C_POKER::LoadResources()
{
	if(action == 0)
	{
		sndStart = LoadWavFile("wav/poker/start.wav");
		sndWash = LoadWavFile("wav/poker/wash.wav",true);
		sndPut = LoadWavFile("wav/poker/put.wav");
		load_bmp_files("picture/poker/bg/",bg,2);
		action = LoadImageFile("picture/poker/action.bmp",0,0,255);
		caption = vgLoadImageFile("picture/poker/caption.bmp");
		load_bmp_files("picture/poker/girl/",girl,3);
	}
}

void C_POKER::UnloadResources()
{
	if(action != 0)
	{
		ReleaseSurface(&action);
		ReleaseSurface(bg,2);
		ReleaseSurface(girl,3);
		ReleaseWave(&sndStart);
	}
}

void C_POKER::PlayCard()
{
	int i;
	for(i=0;i<MAX_PLAYERS;i++)
	{
		if(pcard[i] == 0)
		{
			if(PDATA->SeatHaveRole(i))
			{
				pcard[i] = new C_CARD(i);
			}
		}
		else
		{
			if(!PDATA->SeatHaveRole(i))
			{
				delete pcard[i];
				pcard[i] = 0;
			}
			else
			{
				pcard[i]->Play();
			}
		}
	}
}

int C_POKER::SelectOneCard()
{
	int n;
	SET_IT it;
	it = freeCards.begin();
	std::advance(it,rand()%freeCards.size());
	n = *it;
	freeCards.erase(n);
	return n;
}

void C_POKER::ResetFreeCards()
{
	int i;
	freeCards.clear();
	for(i=0;i<52;i++)
	{
		freeCards.insert(i);
	}
}

void C_POKER::Play()
{
	int i;
	SET_IT it;
	
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
	PlayCard();
	FlagActors();
	ShowSitRole();
	DisplayGameTime(10,30);
	DisplaySitRoleName();
	DisplayPlayerScore();
	DisplayFirstFlag();
	if(m_stage == 0) 		//显示说明
	{
		DisplaySurfaceCenterOnScreen(caption);
		DisplaySmallWatch(10 - m_time/20,585,267,false);
		if(m_time >= 200 || AnyOnePressEnter())
		{
			if(m_time < 200)
			{
				vgSoundPlay(sndPress,PDATA->Vol*255/100);
			}
			++m_stage;
			m_time = 0;
		}
	}
	else if(m_stage == 1)	//发牌
	{
		if(needBonus && m_time == 1)
		{
			if(SubGameStage == STAGE_NONE)
			{
				PrevSubStage = SubGameStage;
				SubGameStage = STAGE_BONUS_GAME;
			}
		}
		else
		{
			int speed = 5;
			int an = m_time/speed%30;
			int w = action->w/5;
			int h = action->h/6;
			vgRECT rect1,rect2;
			
			if(m_time < 25*speed)
			{
				vgDrawSurface(girl[m_time/4%3],406,168,0);
			}
			if(m_time == 1)
			{
				vgSoundPlay(sndWash,PDATA->Vol*255/100);
			}
			rect1.x = an%5*w;
			rect1.y = an/5*h;
			rect1.w = w;
			rect1.h = h;
			
			rect2.x = 380;
			rect2.y = 422;
			rect2.w = 264;
			rect2.h = 170;
			vgDrawStretchSurface(action,&rect1,&rect2,0);
			if(m_time == 25*speed)
			{
				ResetFreeCards();
				SelectActors();
				vgSoundStop(sndWash);
				vgSoundPlay(sndPut,PDATA->Vol*255/100);
				for(i=0;i<MAX_PLAYERS;i++)
				{
					if(pcard[i])
					{
						pcard[i]->BeginDistribute();
						if(IsTmpActor(i))
						{
							pcard[i]->SetCards(SelectOneCard(),53);
						}
						else
						{
							pcard[i]->SetCards(SelectOneCard(),SelectOneCard());
						}
					}
				}
			}
			if(m_time >= 30*speed - 1)
			{
				++m_stage;
				vgSoundStop(sndPut);
				m_time = 0;
			}
		}
	}
	else if(m_stage == 2) 	//等待牌全部打开
	{
		bool allOpened = true;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(pcard[i] != 0)
			{
				if(pcard[i]->IsOpened() || !pcard[i]->IsStart())
				{
					continue;
				}
				else
				{
					allOpened = false;
					break;
				}
			}
		}
		if(allOpened && IsCoinFlyingFinish())
		{
			++m_stage;
			m_time = 0;
			actor.swap(tmpActors);
			PSMALLGAME = psmall;
			for(i=0;i<MAX_PLAYERS;i++)
			{
				if(pcard[i])
				{
					pcard[i]->Reset();
				}
			}
			vgSoundPause(sndPokerDice);
		}
	}
	else 					//表演节目
	{
		if(PSMALLGAME->Play())
		{
			vgSoundResume(sndPokerDice);
			delete PSMALLGAME;
			PSMALLGAME = 0;
			psmall = 0;
			m_stage = 1;
			m_time = 0;
			actor.clear();
			tmpActors.clear();
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
				m_stage = 0;
				vgSoundStop(sndPokerDice);
				vgSoundStop(sndStart);
				sndbgPlayed = false;
				PDATA->MainGameStage = STAGE_SELECT_GAME;
				return;
			}
		}
	}
	if(SubGameStage == STAGE_NONE)
	{
		++m_time;
	}
}

void C_POKER::SelectActors()
{
	int i;
	std::list<int> m,g;
	LIST_IT it;
	
	psmall = CreateSmallGame(rand()%2);
	int mans  = psmall->NumberOfManActors();
	int girls = psmall->NumberOfGirlActors();
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
		tmpActors.insert(*it);
		m.remove(*it);
		--mans;
	}
	
	while(girls > 0 && !g.empty())
	{
		it = g.begin();
		advance(it,rand()%g.size());
		tmpActors.insert(*it);
		g.remove(*it);
		--girls;
	}
}

void C_POKER::Reset()
{
	m_stage = 0;
	m_time = 0;
	actor.clear();
	tmpActors.clear();
	vgSoundStop(sndPokerDice);
	vgSoundStop(sndWash);
	vgSoundStop(sndPut);
	vgSoundStop(sndStart);
	sndbgPlayed = false;
	if(psmall)
	{
		delete psmall;
		psmall = 0;
		PSMALLGAME = 0;
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
		if(pcard[i])
		{
			pcard[i]->Reset();
		}
	}
}

bool C_POKER::IsCurrentPlayer(int w)
{
	return false;
}

