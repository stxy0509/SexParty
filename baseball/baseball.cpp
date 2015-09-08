#include "baseball.h"
#include "bottle.h"
#include "player.h"
#include "bottleinfo.h"
#include "effect.h"
#include "../game.h"
#include "../uptimer.h"
#include "../game_common.h"
#include "../small_game/smallgame.h"
#include <libst.h>
using GAME::C_BASEBALL;
using GAME::C_BOTTLE;
using GAME::C_GAME;
using GAME::time_left;
using std::make_pair;
int C_BASEBALL::FlagPos[MAX_PLAYERS][2] = 
{
	{-26,249},{-26,384},{157,615},{291,615},{580,615},
	{719,615},{874,396},{874,261},{513,-25},{376,-25}
};
int C_BASEBALL::Mark1Pos[MAX_PLAYERS][2] = 
{
	{ 61,210},{ 61,501},{116,691},{407,691},{541,691},
	{832,691},{949,502},{949,211},{621, 61},{330, 61}
};
int C_BASEBALL::Mark2Pos[MAX_PLAYERS][2] = 
{
	{ 30,204},{ 30,495},{110,722},{401,722},{535,722},
	{826,722},{980,496},{980,205},{615, 30},{324, 30}
};
int C_BASEBALL::RankFramePos[MAX_PLAYERS][2] = 
{
	{ 85,196},{ 85,491},{101,634},{396,634},{526,634},
	{821,634},{890,491},{890,196},{615, 84},{315, 84}
};
int C_BASEBALL::Rank2Pos[MAX_PLAYERS][2] = 
{
	{ 87,231},{ 87,526},{136,638},{431,638},{561,638},
	{856,638},{893,493},{893,198},{617, 86},{317, 86}
};
int C_BASEBALL::NamePos[MAX_PLAYERS][2] = 
{
	{  0,282},{  0,422},{188,738},{327,738},{612,738},
	{750,738},{994,422},{994,282},{542,  0},{401,  0}
};
C_BASEBALL::C_BASEBALL():
m_time(0),Loaded(false),Stage(0),
sndPlayed(false),handleHit(true),
PlayTime(new C_UP_TIMER(PDATA->BaseBallTime))
{
	LoadResources();
	C_BOTTLE::LoadResources();
	C_PLAYER::LoadResources();
	C_UP_TIMER::AddTimer(PlayTime);
	for(int i=0;i<MAX_PLAYERS;i++)
	{
		infos[i] = 0;
		bottles[i] = 0;
		effects[i] = 0;
		ballplayers[i] = new C_PLAYER(i);
	}
}

C_BASEBALL::~C_BASEBALL()
{
	UnloadResources();
}

C_BASEBALL *C_BASEBALL::GetSingleton()
{
	static C_BASEBALL *PBASEBALL = 0;
	return PBASEBALL == 0 ? PBASEBALL = new C_BASEBALL() : PBASEBALL;
}

void C_BASEBALL::LoadResources()
{
	if(!Loaded)
	{
		load_bmp_files("picture/baseball/bg/",bg,2);
		nr = LoadImageFile("picture/baseball/nr.bmp",0,255,0);
		nr_left = LoadImageFile("picture/baseball/nr_left.bmp",0,255,0);
		rank = vgLoadImageFile("picture/baseball/rank.bmp");
		rank_left = vgLoadImageFile("picture/baseball/rank_left.bmp");
		caption = vgLoadImageFile("picture/baseball/caption.bmp");
		sndBg = LoadWavFile("wav/baseball/bg.wav",true);
		sndBreak = LoadWavFile("wav/baseball/break.wav");
		sndStart = LoadWavFile("wav/baseball/start.wav");
		load_wav_files("wav/baseball/hit",sndHit,2);
		Loaded = true;
	}
}

void C_BASEBALL::UnloadResources()
{
	if(Loaded)
	{
		ReleaseWave(sndHit,2);
		ReleaseWave(&sndStart);
		ReleaseWave(&sndBreak);
		ReleaseWave(&sndBg);
		ReleaseSurface(&rank_left);
		ReleaseSurface(&rank);
		ReleaseSurface(&nr_left);
		ReleaseSurface(&nr);
		ReleaseSurface(bg,2);
		Loaded = false;
	}
}

void C_BASEBALL::DisplayActors()
{
	static int Frame = 0;
	if(!actor.empty())
	{
		SET_IT it = actor.begin();
		while(it != actor.end())
		{
			ShowActor(*it,Frame);
			++it;
		}
		++Frame;
	}
	GAME::DisplayActNotice();
}

void C_BASEBALL::ShowActor(int n,int f)
{
	if(n < 0 || n >= MAX_PLAYERS) 
	{
		return;
	}
	ballplayers[n]->DisplayPlayerAction(6 + f/5%2);
}

void C_BASEBALL::Play()
{
	if(!sndPlayed)
	{
		keys.clear();
		stop_all_sound();
		vgSoundPlay(sndBg,PDATA->Vol*255/250);
		vgSoundPlay(sndStart,PDATA->Vol*255/100);
		sndPlayed = true;
	}
	Draw1024X768Splitly(bg);
	DisplayGameTime(10,30);
	DisplayPlayerMark();
	DisplayRankOrder();
	DisplayName();
	DisplayFirstFlag();
	if(Stage < 2)
	{
		SetUsableKeys();
	}
	for(int i=0;i<MAX_PLAYERS;i++)
	{
		if(PDATA->SeatHaveRole(i))
		{
			ballplayers[i]->SetPlayerCome();			
		}
		else
		{
			ballplayers[i]->SetPlayerLeave();
		}
	}
	if(Stage == 0)
	{
		for(int i=0;i<MAX_PLAYERS;i++)
		{
			ballplayers[i]->SitQuietly();
		}
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
		if(needBonus && !PlayTime->IsStart())
		{
			for(int i=0;i<MAX_PLAYERS;i++)
			{
				ballplayers[i]->SitQuietly();
			}
			if(SubGameStage == STAGE_NONE)
			{
				PrevSubStage = SubGameStage;
				SubGameStage = STAGE_BONUS_GAME;
			}
		}
		else
		{
			if(!PlayTime->IsStart())
			{
				PlayTime->Start();
			}
			DisplayRestTime();
			for(int i=0;i<MAX_PLAYERS;i++)
			{
				if(PDATA->SeatHaveRole(i))
				{
					ballplayers[i]->Play();
					if(handleHit && HitBottle(i))
					{
						if(rand()%100 < 20)
						{
							vgSoundPlay(sndHit[rand()%2],PDATA->Vol*255/100);
						}
						vgSoundPlay(sndBreak,PDATA->Vol*255/100);
						int v = PointToScore(bottles[i]->value);
						bottles[i]->SetBottleState(C_BOTTLE::BREAKED);
						infos[i] = new C_BOTTLE_INFO(v,i);
						effects[i] = new C_EFFECT(i);
						DealIncrement(v,i);
					}
					if(bottles[i] == 0)
					{
						bottles[i] = new C_BOTTLE(
							static_cast<BOTTLE_NAME>(
								GetRandNumber(BOTTLE_BEER1,BOTTLE_MAGIC)),i);
					}
					bottles[i]->Move();
					if(bottles[i]->GetBottleState() == C_BOTTLE::DEAD)
					{
						delete bottles[i];
						bottles[i] = 0;
					}
					if(infos[i])
					{
						infos[i]->Display();
						if(infos[i]->IsTimeOver())
						{
							delete infos[i];
							infos[i] = 0;
						}
					}
					if(effects[i])
					{
						effects[i]->Display();
						if(effects[i]->IsTimeOver())
						{
							delete effects[i];
							effects[i] = 0;
						}
					}
				}
				else
				{
					ballplayers[i]->Play();
				}
			}
			if(PDATA->InDemo)
			{
				if(PlayTime->GetElapsedTime() >= 10)
				{
					PlayTime->Stop();
					++Stage;
				}
			}
			else if(PlayTime->GetElapsedTime() >= PDATA->BaseBallTime)
			{
				handleHit = false;
				if(IsCoinFlyingFinish())
				{
					PlayTime->Stop();
					++Stage;
					handleHit = true;
				}
			}
		}
	}
	else if(Stage == 2)//演节目
	{
		++Stage;
		manRank.clear();
		girlRank.clear();
		for(int i=MAX_PLAYERS-1;i>=0;--i)
		{
			ballplayers[i]->SitQuietly();
			if(bottles[i]) 
			{
				delete bottles[i];
				bottles[i] = 0;
			}
			if(PDATA->SeatHaveRole(i))
			{
				if(i % 2 == 0)
				{
					manRank.insert(make_pair(PDATA->Players[i].Increment,i));
				}
				else
				{
					girlRank.insert(make_pair(PDATA->Players[i].Increment,i));
				}
			}
		}
		SelectActors();
	}
	else 
	{
		for(int i=0;i<MAX_PLAYERS;i++)
		{
			ballplayers[i]->SitQuietly();
		}
		if(PSMALLGAME->Play())
		{
			Stage = 1;
			vgSoundResume(sndBg);
			delete PSMALLGAME;
			PSMALLGAME = 0;
			actor.clear();
			for(int i=0;i<MAX_PLAYERS;i++)
			{
				PDATA->Players[i].Score += PDATA->Players[i].Increment;
				PDATA->Players[i].Increment = 0;
				playerIncrementDigit[i].setValue(0);
				playerScoreDigit[i].setValue(PDATA->Players[i].Score);
			}
			
			if(PDATA->InDemo)
			{
				Stage = 0;
				vgSoundStop(sndBg);
				sndPlayed = false;
				PDATA->MainGameStage = STAGE_SELECT_GAME;
			}
		}
	}
	DisplayActors();
	if(SubGameStage != STAGE_NONE)
	{
		PlayTime->Pause();
	}
	else
	{
		PlayTime->Resume();
	}
}

bool C_BASEBALL::IsCurrentPlayer(int who)
{
	return false;
}

bool C_BASEBALL::HitBottle(int index)
{
	return (bottles[index] &&
			bottles[index]->time >= 42 && 
			bottles[index]->time <= 54 &&
			ballplayers[index]->GetCurrentAction() >= 4 &&  
			bottles[index]->GetBottleState() == C_BOTTLE::FLYING);
}

void C_BASEBALL::Reset()
{
	Stage = 0;
	m_time = 0;
	sndPlayed = false;
	handleHit = true;
	PlayTime->Stop();
	manRank.clear();
	girlRank.clear();
	actor.clear();
	vgSoundStop(sndBg);
	vgSoundStop(sndBreak);
	vgSoundStop(sndStart);
	vgSoundStop(sndHit[0]);
	vgSoundStop(sndHit[1]);
	if(PSMALLGAME)
	{
		delete PSMALLGAME;
		PSMALLGAME = 0;
	}
	for(int i=0;i<MAX_PLAYERS;i++)
	{
		PDATA->Players[i].Score += PDATA->Players[i].Increment;
		PDATA->Players[i].Increment = 0;
		playerIncrementDigit[i].setValue(0);
		playerScoreDigit[i].setValue(PDATA->Players[i].Score);
		if(bottles[i]) 	
		{			
			delete bottles[i];
			bottles[i] = 0;
		}
		if(effects[i]) 	
		{
			delete effects[i];
			effects[i] = 0;
		}
		if(infos[i]) 	
		{
			delete infos[i];
			infos[i] = 0;
		}
	}
}

void C_BASEBALL::DisplayPlayerMark()
{
	int i;
	for(i=0;i<MAX_PLAYERS;i++)
	{
		if(!PDATA->SeatHaveRole(i))
		{
			continue;
		}
		switch(i)
		{
			case 0:
			case 1:
				display_num_vertical(nr_left,5,
					playerIncrementDigit[i].getValue(),
					Mark1Pos[i][0],Mark1Pos[i][1],1);
				display_num_vertical(nr_left,6,
					PDATA->Players[i].Score,
					Mark2Pos[i][0],Mark2Pos[i][1],1);
				break;
			case 2:
			case 3:
			case 4:
			case 5:
				display_num_horizontal(nr,5,
					playerIncrementDigit[i].getValue(),
					Mark1Pos[i][0],Mark1Pos[i][1],1);
				display_num_horizontal(nr,6,
					PDATA->Players[i].Score,
					Mark2Pos[i][0],Mark2Pos[i][1],1);
				break;
			case 6:
			case 7:
				display_num_vertical(nr_left,5,
					playerIncrementDigit[i].getValue(),
					Mark1Pos[i][0],Mark1Pos[i][1],1,true);
				display_num_vertical(nr_left,6,
					PDATA->Players[i].Score,
					Mark2Pos[i][0],Mark2Pos[i][1],1,true);
				break;
			case 8:
			case 9:
				display_num_horizontal(nr,5,
					playerIncrementDigit[i].getValue(),
					Mark1Pos[i][0],Mark1Pos[i][1],1,true);
				display_num_horizontal(nr,6,
					PDATA->Players[i].Score,
					Mark2Pos[i][0],Mark2Pos[i][1],1,true);
				break;	
		}
	}
}

void C_BASEBALL::DisplayRestTime()
{
	int TimeLeft = PDATA->BaseBallTime - PlayTime->GetElapsedTime();
	if(TimeLeft < 0)
	{
		TimeLeft = 0;
	}
	DisplayWatch(TimeLeft,865,92);
}

void C_BASEBALL::DisplayRankOrder()
{
	ComputeRankOrder();
	for(int i=0;i<MAX_PLAYERS;i++)
	{
		if(RankOrder[i] != 0)
		{
			switch(i)
			{
				case 0:
				case 1:
					vgDrawSurface(rank_left,RankFramePos[i][0],
						RankFramePos[i][1],0);
					display_num_vertical(nr_left,3,RankOrder[i],
						Rank2Pos[i][0],Rank2Pos[i][1],1);
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					vgDrawSurface(rank,RankFramePos[i][0],
						RankFramePos[i][1],0);
					display_num_horizontal(nr,3,RankOrder[i],
						Rank2Pos[i][0],Rank2Pos[i][1],1);
					break;
				case 6:
				case 7:
					vgDrawSurface(rank_left,RankFramePos[i][0],
						RankFramePos[i][1],
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
					display_num_vertical(nr_left,3,RankOrder[i],
						Rank2Pos[i][0],Rank2Pos[i][1],1,true);
					break;
				case 8:
				case 9:
					vgDrawSurface(rank,RankFramePos[i][0],
						RankFramePos[i][1],
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
					display_num_horizontal(nr,3,RankOrder[i],
						Rank2Pos[i][0],Rank2Pos[i][1],1,true);
					break;
			}
		}
	}
}

void C_BASEBALL::ComputeRankOrder()
{
	int i;
	std::multimap<int,int,GT> order;
	MAP_IT it;
	for(i=0;i<MAX_PLAYERS;i++)
	{
		RankOrder[i] = 0;
		if(PDATA->SeatHaveRole(i))
		{
			order.insert(make_pair(PDATA->Players[i].Increment,i));
		}
	}
	i = 1;
	it = order.begin();
	while(it != order.end())
	{
		RankOrder[it->second] = i;
		++i;
		++it;
	}
}

int C_BASEBALL::PointToScore(int p)
{
	return p*10;
}

void C_BASEBALL::SelectActors()
{
	//选择男士和女士中的得分高的表演节目
	int i,mans,girls;
	int manscore,girlscore;
	std::multimap<int,int,GT> orderMan;
	std::multimap<int,int,GT> orderGirl;
	std::multimap<int,int,GT>::iterator mapit;
	
	for(i=0;i<MAX_PLAYERS;i+=2)
	{
		if(PDATA->CanAct(i))
		{
			orderMan.insert(make_pair(PDATA->Players[i].Increment,i));
		}
	}
	manscore = orderMan.begin()->first;
	for(i=1;i<MAX_PLAYERS;i+=2)
	{
		if(PDATA->CanAct(i))
		{
			orderGirl.insert(make_pair(PDATA->Players[i].Increment,i));
		}
	}
	girlscore = orderGirl.begin()->first;
	PSMALLGAME = CreateSmallGame(manscore > girlscore);
	mans  = PSMALLGAME->NumberOfManActors();
	girls = PSMALLGAME->NumberOfGirlActors();
	
	mapit = orderMan.begin();
	while(mapit != orderMan.end() && mans > 0)
	{
		actor.insert(mapit->second);
		++mapit;
		--mans;
	}
	
	mapit = orderGirl.begin();
	while(mapit != orderGirl.end() && girls > 0)
	{
		actor.insert(mapit->second);
		++mapit;
		--girls;
	}
	vgSoundPause(sndBg);
}

void C_BASEBALL::DisplayName()
{
	int i,idx;
	int dir[MAX_PLAYERS] = 
	{
		0,0,1,1,1,1,2,2,3,3
	};
	for(i=0;i<MAX_PLAYERS;i++)
	{
		if(PDATA->SeatHaveRole(i))
		{
			idx = PDATA->Players[i].Index;
			DisplayRoleName(i%2 == 0,idx,dir[i],
				NamePos[i][0],NamePos[i][1]);
		}
	}
}
