#include "present.h"
#include "downtimer.h"
#include "game_common.h"
#include "small_game/smallgame.h"
#include <libst.h>
using GAME::C_PRESENT;
using GAME::nr;
using GAME::nr_left;
int C_PRESENT::CoinPos[5][2] = 
{
	{0,560},{476,761},{920,761},{1017,165},{305,0}
};
int C_PRESENT::GiftRate[5] = 
{
	40,60,90,150,250
};
int C_PRESENT::GiftPos[9][2] = 
{
	{283,211}, {454,211}, {624,211},
	{283,346}, {454,346}, {624,346},
	{283,480}, {454,480}, {624,480}
};
int C_PRESENT::AwardFramePos[9][2] = 
{
	{270,210},{441,210},{611,210},
	{270,345},{441,345},{611,345},
	{270,479},{441,479},{611,479}
};
C_PRESENT::C_PRESENT():
time(0),stage(0),lines(0),
points(0),send(-1),recv(-1),
picLoaded(false),sndLoaded(false),
haveResult(false),demoCount(0),award(),
pTimer(new C_DOWN_TIMER(25)),
fightTimer(new C_DOWN_TIMER(15)),
waitFinish(false),drawFinish(false),
gameFinish(false),sndbgPlayed(false)
{
	int i;
	LoadSounds();
	LoadPictures();
	for(i=0;i<3;i++)
	{
		rollCol[i] = i;
		stopCol[i] = -1;
		stopTime[i] = 0;
	}
	for(i=0;i<5;i++)
	{
		pressScore[i] = 10;
		noPressCount[i] = 0;
	}
	for(i=0;i<8;i++)
	{
		rate[i] = 0;
		lineNum[i] = -1;
		lineAward[i].reserve(3);
	}
	allAwards.reserve(9);
}

C_PRESENT::~C_PRESENT()
{
	UnloadPictures();
}

C_PRESENT *C_PRESENT::GetSingleton()
{
	static C_PRESENT *PPRESENT = 0;
	return PPRESENT == 0 ? PPRESENT = new C_PRESENT() : PPRESENT;
}

void C_PRESENT::LoadPictures()
{
	if(!picLoaded)
	{
		value = LoadImageFile("picture/present/value.bmp",255,255,255);
		load_bmp_files("picture/present/bg/",bg,2);
		load_bmp_files("picture/present/lattice/",lattice,2);
		blue = LoadImageFile("picture/present/blue.bmp",255,255,255);
		gift = LoadImageFile("picture/present/gift.bmp",0,0,255);
		menu = vgLoadImageFile("picture/present/menu.bmp");
		coin = vgLoadImageFile("picture/present/coin/down.bmp");
		coin_left = vgLoadImageFile("picture/present/coin/left.bmp");
		picsend = LoadImageFile("picture/present/sf.bmp",255,255,255);
		nr1 = LoadImageFile("picture/present/nr.bmp",0,255,0);
		
		sendD = LoadImageFile("picture/present/send.bmp",0,0,255);
		recvD = LoadImageFile("picture/present/recv.bmp",0,0,255);
		sendL = LoadImageFile("picture/present/send_left.bmp",0,0,255);
		recvL = LoadImageFile("picture/present/recv_left.bmp",0,0,255);
		sendD->alpha = 0xa0;
		sendL->alpha = 0xa0;
		AppendDrawMode(sendD,DRAWMODE_ALPHA);
		AppendDrawMode(sendL,DRAWMODE_ALPHA);
		
		caption1 = vgLoadImageFile("picture/present/caption1.bmp");
		caption2 = vgLoadImageFile("picture/present/caption2.bmp");
		picLoaded = true;
	}
}

void C_PRESENT::UnloadPictures()
{
	if(picLoaded)
	{
		ReleaseSurface(&recvL);
		ReleaseSurface(&sendL);
		ReleaseSurface(&recvD);
		ReleaseSurface(&sendD);
		ReleaseSurface(&nr1);
		ReleaseSurface(&picsend);
		ReleaseSurface(&coin_left);
		ReleaseSurface(&coin);
		ReleaseSurface(&menu);
		ReleaseSurface(&gift);
		ReleaseSurface(&blue);
		ReleaseSurface(lattice,2);	
		ReleaseSurface(bg,2);
		ReleaseSurface(&value);
		picLoaded = false;
	}
}

void C_PRESENT::LoadSounds()
{
	if(!sndLoaded)
	{
		sndBg = LoadWavFile("wav/present/bg.wav",true);
		sndRoll = LoadWavFile("wav/present/roll.wav",true);
		sndNext = LoadWavFile("wav/present/next.wav");
		sndSelect = LoadWavFile("wav/present/select.wav");
		
		sndStart = LoadWavFile("wav/present/start.wav");
		sndGrab = LoadWavFile("wav/present/grab.wav");
		sndGrabBg = LoadWavFile("wav/present/grab_bg.wav",true);
		sndEncourage = LoadWavFile("wav/present/press.wav");
		sndCelebrate = LoadWavFile("wav/present/celebrate.wav");
		sndLoaded = true;
	}
}

void C_PRESENT::UnloadSounds()
{
	if(sndLoaded)
	{
		ReleaseWave(&sndEncourage);
		ReleaseWave(&sndStart);
		ReleaseWave(&sndGrab);
		ReleaseWave(&sndSelect);
		ReleaseWave(&sndNext);
		ReleaseWave(&sndRoll);
		ReleaseWave(&sndBg);
		sndLoaded = false;
	}
}

bool C_PRESENT::IsCurrentPlayer(int who)
{
	if(stage < 2)
	{
		return 	who == send || who == recv;
	}
	else if(stage == 2)
	{
		return false;
	}
	else
	{
		return fastest == who;
	}
}

void C_PRESENT::DisplayCoinScore(int n)
{
	int i,x,y;
	x = CoinPos[(n - 1)/2][0];
	y = CoinPos[(n - 1)/2][1];
	switch(n)
	{
		case 1:
			for(i=0;i<pressScore[(n - 1)/2];i++)
			{
				vgDrawSurface(coin_left,x,y,0);
				x += coin_left->w;
			}
			display_num_vertical(nr_left,3,
				pressScore[(n - 1)/2],x + 20,y - 10,1);
			break;
		case 3:
		case 5:
			for(i=0;i<pressScore[(n - 1)/2];i++)
			{
				vgDrawSurface(coin,x,y,0);
				y -= coin->h;
			}
			display_num_horizontal(nr,3,
				pressScore[(n - 1)/2],x - 10,y - 20,1);
			break;
		case 7:
			for(i=0;i<pressScore[(n - 1)/2];i++)
			{
				vgDrawSurface(coin_left,x,y,0);
				x -= coin_left->w;
			}
			display_num_vertical(nr_left,3,
				pressScore[(n - 1)/2],x - 20,y + 10,1,true);
			break;
		case 9:
			for(i=0;i<pressScore[(n - 1)/2];i++)
			{
				vgDrawSurface(coin,x,y,0);
				y += coin->h;
			}
			display_num_horizontal(nr,3,
				pressScore[(n - 1)/2],x + 10,y + 20,1,true);
			break;
	}
}

void C_PRESENT::DisplaySendInfo()
{
	//---------------Up-------------------------
	vgDrawSurface(picsend,353,204,0);
	DrawPartOfSurface(gift,5,1,
		sendScore/100,558,158);
	vgDrawSurface(value,557,286,0);
	display_num_horizontal(nr,3,sendScore,
		607,289,1);
	//--------------Down------------------------
	vgDrawSurface(picsend,476,496,DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
	DrawPartOfSurface(gift,5,1,
		sendScore/100,358,474);
	vgDrawSurface(value,357,444,DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
	display_num_horizontal(nr,3,sendScore,
		367,447,1,true);
}

void C_PRESENT::Reset()
{
	int i;
	time = 0;
	stage = 0;
	points = 0;
	fastest = -1;
	send = -1;
	demoCount = 0;
	for(i=0;i<3;i++)
	{
		rollCol[i] = i;
		stopCol[i] = -1;
		stopTime[i] = 0;
	}
	for(i=0;i<5;i++)
	{
		pressScore[i] = 10;
		noPressCount[i] = 0;
	}
	waitFinish = false;
	drawFinish = false;
	gameFinish = false;
	haveResult = false;
	sndbgPlayed = false;
	vgSoundStop(sndBg);
	vgSoundStop(sndNext);
	vgSoundStop(sndRoll);
	vgSoundStop(sndSelect);
	vgSoundStop(sndStart);
	vgSoundStop(sndGrab);
	vgSoundStop(sndGrabBg);
	vgSoundStop(sndCelebrate);
	pTimer->Stop();
	fightTimer->Stop();
	actor.clear();
	if(PSMALLGAME) 
	{	
		delete PSMALLGAME;
		PSMALLGAME = 0;
	}
	for(i=0;i<MAX_PLAYERS;i++)
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

void C_PRESENT::DisplayAward()
{
	for(int i=0;i<9;i++)
	{
		DrawPartOfSurface(gift,5,1,award[i],
			GiftPos[i][0],GiftPos[i][1]);
	}
}

bool C_PRESENT::WaitToSelect()
{
	if(pTimer->GetRestTime() > 0)
	{
		if(!PDATA->InDemo)
		{
			if(SubGameStage == STAGE_NONE)
			{
				int rt = (118 - time%100)/20;
				int TimePos[MAX_PLAYERS][2] = 
				{
					{180,239},{180,435},{180,497},{359,497},{612,497},
					{790,497},{786,436},{786,242},{583,213},{396,213}
				};
				if(send%2 == 0)
				{
					if(time%100 == 99 || PKEY->PressKey(KEY_FUNCTION))
					{
						time = 0;
						vgSoundPlay(sndNext,PDATA->Vol*255/100);
						recv = PDATA->NextGirl(recv);
						if(recv == MAX_PLAYERS)
						{
							recv = PDATA->NextGirl(-1);
						}
					}
				}
				else
				{
					if(time%100 == 99 || PKEY->PressKey(KEY_FUNCTION))
					{
						time = 0;
						vgSoundPlay(sndNext,PDATA->Vol*255/100);
						recv = PDATA->NextMan(recv);
						if(recv == MAX_PLAYERS)
						{
							recv = PDATA->NextMan(-1);
						}
					}
				}
				if(recv < 2)
				{
					DisplaySmallWatchLeft(rt,
						TimePos[recv][0],
						TimePos[recv][1]);
				}
				else if(recv < 6)
				{
					DisplaySmallWatch(rt,
						TimePos[recv][0],
						TimePos[recv][1]);
				}
				else if(recv < 8)
				{
					DisplaySmallWatchLeft(rt,
						TimePos[recv][0],
						TimePos[recv][1],true);
				}
				else
				{
					DisplaySmallWatch(rt,
						TimePos[recv][0],
						TimePos[recv][1],true);
				}
				if(PKEY->PressKey(PlayersEnter[send]))
				{
					vgSoundPlay(sndSelect,PDATA->Vol*255/100);
					return true;
				}
			}
		}
		else
		{
			int x = rand()%150;
			if(x < 4)
			{
				if(send%2 == 0)
				{
					recv = PDATA->NextGirl(recv);
					if(recv == MAX_PLAYERS)
					{
						recv = PDATA->NextGirl(-1);
					}
					if(rand()%100 < 30)
					{
						return true;
					}
				}
				else
				{
					recv = PDATA->NextMan(recv);
					if(recv == MAX_PLAYERS)
					{
						recv = PDATA->NextMan(-1);
					}
					if(rand()%100 < 30)
					{
						return true;
					}
				}
			}
		}
	}
	else
	{
		int i,rn,nMG;
		if(send%2 == 0)
		{
			nMG = PDATA->NumberOfGirls();
			rn = rand()%nMG;
			i = 0;
			recv = -1;
			do
			{
				recv = PDATA->NextGirl(recv);
				if(i == rn)
				{
					break;
				}
				++i;
			}while(i < nMG);
		}
		else
		{
			nMG = PDATA->NumberOfMans();
			rn = rand()%nMG;
			i = 0;
			recv = -1;
			do
			{
				recv = PDATA->NextMan(recv);
				if(i == rn)
				{
					break;
				}
				++i;
			}while(i < nMG);
		}
		return true;
	}
	return false;
}

void C_PRESENT::DisplayAllCoin()
{
	int i;
	for(i=PDATA->NextGirl(-1);i<MAX_PLAYERS;i=PDATA->NextGirl(i))
	{
		DisplayCoinScore(i);
	}
}

void C_PRESENT::RequestAwardResult()
{
	int i,pool[2];
	pool[0] = rand()%5;
	pool[1] = rand()%5;
	for(i=0;i<9;i++)
	{
		award[i] = pool[rand()%2];
	}
}

bool C_PRESENT::DisplayAllAwardFrame()
{
	DisplayAward();
	if(lines == 0)
	{
		if(time == 1 && SubGameStage == STAGE_NONE)
		{
			points = PointToScore(10);
			DealIncrement(points,recv);
		}
		return time >= 40;
	}
	else
	{
		int i = time/20;
		if(i < lines)
		{
			VIT it = lineAward[i].begin();
			while(it != lineAward[i].end())
			{
				DisplayAwardFrame(*it);
				++it;
			}
			
			if(time == 20*i + 1 && SubGameStage == STAGE_NONE)
			{
				points += PointToScore(rate[i]);
				if(i == lines - 1)
				{
					DealIncrement(points,recv);
				}
			}
		}
		else
		{
			if(time%14 < 7)
			{
				VIT it = allAwards.begin();
				while(it != allAwards.end())
				{
					DisplayAwardFrame(*it);
					++it;
				}
			}
		}
		return time >= 20*(lines + 2);
	}
}

void C_PRESENT::RateOfAward()
{
	int i,j,use[9] = {0,};
	j = 0;
	lines = 0;
	allAwards.clear();
	for(i=0;i<8;i++)
	{
		rate[i] = 0;
		lineNum[i] = -1;
		lineAward[i].clear();
	}
	for(i=0;i<3;i++) 	//三行中奖情况
	{
		if((award[3*i + 1] == award[3*i]) && (award[3*i + 1] == award[3*i + 2]))
		{
			lineNum[j] = i;
			rate[j] = GiftRate[award[3*i + 1]];
			lineAward[j].push_back(3*i);
			lineAward[j].push_back(3*i + 1);
			lineAward[j].push_back(3*i + 2);
			++use[3*i];
			++use[3*i + 1];
			++use[3*i + 2];			
			++j;
			++lines;
		}
	}
	for(i=0;i<3;i++)	//三列中奖情况
	{
		if((award[i + 3] == award[i]) && (award[i + 3] == award[i + 6]))
		{
			lineNum[j] = i + 3;
			rate[j] = GiftRate[award[i]];
			lineAward[j].push_back(i);
			lineAward[j].push_back(i + 3);
			lineAward[j].push_back(i + 6);
			
			++use[i];
			++use[i + 3];
			++use[i + 6];
			++j;
			++lines;
		}
	}
	if((award[4] == award[0]) && (award[4] == award[8]))
	{
		lineNum[j] = 6;
		rate[j] = GiftRate[award[4]];
		lineAward[j].push_back(4);
		lineAward[j].push_back(0);
		lineAward[j].push_back(8);
		++use[0];
		++use[4];
		++use[8];
		++j;
		++lines;
	}
	if((award[4] == award[2]) && (award[4] == award[6]))
	{
		lineNum[j] = 7;
		rate[j] = GiftRate[award[4]];
		lineAward[j].push_back(4);
		lineAward[j].push_back(2);
		lineAward[j].push_back(6);
		++use[2];
		++use[4];
		++use[6];
		++j;
		++lines;
	}
	for(i=0;i<9;i++)
	{
		if(use[i] > 0)
		{
			allAwards.push_back(i);
		}
	}
}

bool C_PRESENT::DrawAward()
{
	int i,interT = 3;
	if(!haveResult)
	{
		RequestAwardResult();
		RateOfAward();
		haveResult = true;
		vgSoundPlay(sndRoll,PDATA->Vol*255/100);
	}
	if(stopTime[2] < interT*7)
	{
		int t,a,b,c;
		for(i=0;i<3;i++)
		{
			if(rollCol[i] >= 0)
			{
				DrawSurfaceForRollVertically(gift,5,1,5,3,
					14 - (award[i] + 3 + time)%15,
					GiftPos[i][0],GiftPos[i][1]);
				DrawSurfaceForRollVertically(gift,5,1,5,3,
					14 - (award[i + 3] + 6 + time)%15,
					GiftPos[i + 3][0],GiftPos[i + 3][1]);
				DrawSurfaceForRollVertically(gift,5,1,5,3,
					14 - (award[i + 6] + 9 + time)%15,
					GiftPos[i + 6][0],GiftPos[i + 6][1]);
				if(time == 39 * (i + 1))
				{
					stopCol[i] = i;
					rollCol[i] = -1;
				}
			}
		}
		for(i=0;i<3;i++)
		{
			if(stopCol[i] < 0) continue;
			t = stopTime[i];
			a = (award[i]    )*128;
			b = (award[i + 3])*128;
			c = (award[i + 6])*128;
			if(t < interT*7)
			{
				if(t < interT)
				{
					DrawSurfaceForRollVertically(gift,5,1,5,128,a,
						GiftPos[i][0],GiftPos[i][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,3,14 - (award[i] + 6 + t/1)%15,
						GiftPos[i + 3][0],GiftPos[i + 3][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,3,14 - (award[i] + 9 + t/1)%15,
						GiftPos[i + 6][0],GiftPos[i + 6][1]);
				}
				else if(t < interT*2)
				{
					DrawSurfaceForRollVertically(gift,5,1,5,128,(a + 18)%(5*128),
						GiftPos[i][0],GiftPos[i][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,3,14 - (award[i] + 6 + t/1)%15,
						GiftPos[i + 3][0],GiftPos[i + 3][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,3,14 - (award[i] + 9 + t/1)%15,
						GiftPos[i + 6][0],GiftPos[i + 6][1]);
				}
				else if(t < interT*3)
				{
					DrawSurfaceForRollVertically(gift,5,1,5,128,(a + 12)%(5*128),
						GiftPos[i][0],GiftPos[i][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,128,b,
						GiftPos[i + 3][0],GiftPos[i + 3][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,3,14 - (award[i] + 9 + t/1)%15,
						GiftPos[i + 6][0],GiftPos[i + 6][1]);
				}
				else if(t < interT*4)
				{
					DrawSurfaceForRollVertically(gift,5,1,5,128,award[i]*128,
						GiftPos[i][0],GiftPos[i][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,128,(b + 18)%(5*128),
						GiftPos[i + 3][0],GiftPos[i + 3][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,3,14 - (award[i] + 9 + t/1)%15,
						GiftPos[i + 6][0],GiftPos[i + 6][1]);
				}
				else if(t < interT*5)
				{
					DrawSurfaceForRollVertically(gift,5,1,5,128,award[i]*128,
						GiftPos[i][0],GiftPos[i][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,128,(b + 12)%(5*128),
						GiftPos[i + 3][0],GiftPos[i + 3][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,128,c,
						GiftPos[i + 6][0],GiftPos[i + 6][1]);
				}
				else if(t < interT*6)
				{
					DrawSurfaceForRollVertically(gift,5,1,5,128,award[i]*128,
						GiftPos[i][0],GiftPos[i][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,128,award[i + 3]*128,
						GiftPos[i + 3][0],GiftPos[i + 3][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,128,(c + 18)%(5*128),
						GiftPos[i + 6][0],GiftPos[i + 6][1]);
				}
				else
				{
					DrawSurfaceForRollVertically(gift,5,1,5,128,award[i]*128,
						GiftPos[i][0],GiftPos[i][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,128,award[i + 3]*128,
						GiftPos[i + 3][0],GiftPos[i + 3][1]);
					DrawSurfaceForRollVertically(gift,5,1,5,128,award[i + 6]*128,
						GiftPos[i + 6][0],GiftPos[i + 6][1]);
				}
				++stopTime[i];
			}
			else
			{
				DrawPartOfSurface(gift,5,1,award[i],    GiftPos[i][0],    GiftPos[i][1]);
				DrawPartOfSurface(gift,5,1,award[i + 3],GiftPos[i + 3][0],GiftPos[i + 3][1]);
				DrawPartOfSurface(gift,5,1,award[i + 6],GiftPos[i + 6][0],GiftPos[i + 6][1]);
			}
		}
		if(stopTime[2] == interT*7)
		{
			time = 0;
			vgSoundStop(sndRoll);
		}
	}
	else if(DisplayAllAwardFrame())
	{
		time = 0;
		for(i=0;i<3;i++)
		{
			rollCol[i] = i;
			stopCol[i] = -1;
			stopTime[i] = 0;
		}
		return true;
	}
	return false;
}

void C_PRESENT::DisplayAwardFrame(int n)
{
	vgDrawSurface(blue,AwardFramePos[n][0],AwardFramePos[n][1],0);
}

int C_PRESENT::PointToScore(int p)
{
	return p;
}

void C_PRESENT::SelectActors()
{
	int i;
	LIST_IT it;
	std::list<int> m,g;
	
	PSMALLGAME = CreateSmallGame(send%2 == 0,true);
	int mans  = PSMALLGAME->NumberOfManActors();
	int girls = PSMALLGAME->NumberOfGirlActors();
	
	if(send%2 == 0)
	{
		actor.insert(send);
		--mans;
		
		actor.insert(recv);
		--girls;
		for(i=0;i<MAX_PLAYERS;i+=2)
		{
			if(i != send && PDATA->CanAct(i))
			{
				m.push_back(i);
			}
		}
		for(i=1;i<MAX_PLAYERS;i+=2)
		{
			if(i != recv && PDATA->CanAct(i))
			{
				g.push_back(i);
			}
		}
	}
	else
	{
		actor.insert(send);
		--girls;
		
		actor.insert(recv);
		--mans;
		for(i=0;i<MAX_PLAYERS;i+=2)
		{
			if(i != recv && PDATA->CanAct(i))
			{
				m.push_back(i);
			}
		}
		for(i=1;i<MAX_PLAYERS;i+=2)
		{
			if(i != send && PDATA->CanAct(i))
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
	vgSoundPause(sndBg);
}

void C_PRESENT::FlagWhoSend(int n)
{
	int SendWordPos[MAX_PLAYERS][2] = 
	{
		{116,226},{116,411},{131,561},{318,561},{575,561},
		{761,561},{850,417},{850,232},{548,148},{360,148},
	};
	if(n < 0 || n >= MAX_PLAYERS) 
	{
		return;
	}
	if(n < 2)
	{
		vgDrawSurface(sendL,SendWordPos[n][0],SendWordPos[n][1],0);
	}
	else if(n < 6)
	{
		vgDrawSurface(sendD,SendWordPos[n][0],SendWordPos[n][1],0);
	}
	else if(n < 8)
	{
		vgDrawSurface(sendL,SendWordPos[n][0],SendWordPos[n][1],
			DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
	}
	else
	{
		vgDrawSurface(sendD,SendWordPos[n][0],SendWordPos[n][1],
			DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
	}
}

void C_PRESENT::FlagSendObject(int n)
{
	int SendWordPos[MAX_PLAYERS][2] = 
	{
		{116,226},{116,411},{131,561},{318,561},{575,561},
		{761,561},{850,417},{850,232},{548,148},{360,148},
	};
	if(n < 0 || n >= MAX_PLAYERS)
	{
		return;
	}
	if(n < 2)
	{
		vgDrawSurface(recvL,SendWordPos[n][0],SendWordPos[n][1],0);
	}
	else if(n < 6)
	{
		vgDrawSurface(recvD,SendWordPos[n][0],SendWordPos[n][1],0);
	}
	else if(n < 8)
	{
		vgDrawSurface(recvL,SendWordPos[n][0],SendWordPos[n][1],
			DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
	}
	else
	{
		vgDrawSurface(recvD,SendWordPos[n][0],SendWordPos[n][1],
			DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
	}
}
	
void C_PRESENT::Process()
{
	if(!sndbgPlayed)
	{
		keys.clear();
		stop_all_sound();
		vgSoundPlay(sndBg,PDATA->Vol*255/250);
		vgSoundPlay(sndStart,PDATA->Vol*255/100);
		sndbgPlayed = true;
	}
	if(send < 0)
	{
		send = PDATA->NextPlayer(-1);
		if(send%2 == 0)
		{
			recv = PDATA->NextGirl(-1);
		}
		else
		{
			recv = PDATA->NextMan(-1);
		}
		keys.insert(send);
	}
	Draw1024X768Splitly(bg);
	if(stage < 2)
	{
		vgDrawSurface(lattice[0],242,139,0);
		vgDrawSurface(lattice[1],754,139,0);
		FlagActors();
		if(SubGameStage == STAGE_NONE)
		{
			DisplayPressAnimation(send);
		}
		FlagWhoSend(send);
		ChairFocusOn(recv);
		FlagSendObject(recv);
		display_num_horizontal(nr1,1,points,523,159,1);
	}
	DisplayFirstFlag();
	if(stage == 0)
	{
		DisplayAward();
		DisplaySurfaceCenterOnScreen(caption1);
		DisplaySmallWatch(10 - time/20,585,267,false);
		if(time >= 200 || AnyOnePressEnter())
		{
			++stage;
			if(time < 200)
			{
				vgSoundPlay(sndPress,PDATA->Vol*255/100);
			}
			time = 0;
		}
	}
	else if(stage == 1)
	{
		if(needBonus && !waitFinish && !pTimer->IsStart())
		{
			DisplayAward();
			if(SubGameStage == STAGE_NONE)
			{
				PrevSubStage = SubGameStage;
				SubGameStage = STAGE_BONUS_GAME;
			}
		}
		else
		{
			if(!waitFinish)
			{
				if(!pTimer->IsStart())	
				{
					pTimer->Start();
				}
				DisplayAward();
				ShowRemainedTime();
				waitFinish = WaitToSelect();
				LuckyStrikeOn(recv,time);
				if(waitFinish)
				{
					time = 0;
					pTimer->Stop();
				}
			}
			else if(!PSMALLGAME)
			{
				LuckyStrikeOn(recv,time);
				if(!drawFinish)
				{
					drawFinish = DrawAward();
				}
				else 
				{
					DisplayAward();
					if(IsCoinFlyingFinish())
					{
						SelectActors();
					}
				}
			}
			else
			{
				if(PSMALLGAME->Play())
				{
					vgSoundResume(sndBg);
					haveResult = false;
					waitFinish = false;
					drawFinish = false;
					points = 0;
					actor.clear();
					delete PSMALLGAME;
					PSMALLGAME = 0;
					keys.erase(send);
					send = PDATA->NextPlayer(send);
					if(PDATA->InDemo)
					{
						++demoCount;
						if(demoCount == 2)
						{
							demoCount = 0;
							send = MAX_PLAYERS;
						}
					}
					if(send == MAX_PLAYERS)
					{
						time = 0;
						stage = 2;
						send = PDATA->NextPlayer(-1);
						keys.clear();
						for(int i=1;i<MAX_PLAYERS;i+=2)
						{
							if(PDATA->SeatHaveRole(i))
							{
								keys.insert(i);
							}
						}
					}
					else
					{
						keys.insert(send);
					}
					if(send%2 == 0)
					{
						recv = PDATA->NextGirl(-1);
					}
					else
					{
						recv = PDATA->NextMan(-1);
					}
				}
			}
		}
	}
	else if(stage == 2)
	{
		if(needBonus && time == 1)
		{
			if(SubGameStage == STAGE_NONE)
			{
				PrevSubStage = SubGameStage;
				SubGameStage = STAGE_BONUS_GAME;
			}
		}
		else
		{
			if(time < 120)
			{
				DisplaySurfaceCenterOnScreen(caption2);
				DisplaySmallWatch(6 - time/20,585,267,false);
			}
			if(time >= 120 || AnyOnePressEnter())
			{
				time = 0;
				vgSoundPlay(sndGrab,PDATA->Vol*255/100);
				++stage;
				fightTimer->Start();
				fastest = PDATA->NextGirl(-1);
			}
		}
	}
	else if(stage == 3)
	{
		if(time == 30)
		{
			vgSoundPlay(sndGrabBg,PDATA->Vol*255/100);
		}
		if(time%60 == 20)
		{
			vgSoundPlay(sndEncourage,PDATA->Vol*255/100);
		}
		if(fightTimer->GetRestTime() > 0 && pressScore[(fastest - 1)/2] < 100)
		{
			if(SubGameStage == STAGE_NONE)
			{				
				DealKeyPress();
				if(time%10 == 0)
				{
					sendScore = GetRandNumber(8,49)*10;
				}
			}
			ShowRemainedTime();
			DisplayAllCoin();
			LuckyStrikeOn(fastest,time);
			DisplaySendInfo();
		}
		else
		{
			++stage;
			time = 0;
			fightTimer->Stop();
			sendScore = GetRandNumber(8,49)*10;
			vgSoundStop(sndGrabBg);
			vgSoundPlay(sndCelebrate,PDATA->Vol*255/100);
			DealIncrement(sendScore,fastest);
		}
	}
	else
	{
		if(PCOIN[fastest] || time < 150)
		{
			DisplayAllCoin();
			DisplaySendInfo();
		}
		else
		{
			time = 0;
			stage = 1;
			send = -1;
			fastest = -1;
			keys.clear();
			for(int i=0;i<5;i++)
			{
				pressScore[i] = 10;
				noPressCount[i] = 0;
			}
			vgSoundStop(sndCelebrate);
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
				stage = 0;
				vgSoundStop(sndBg);
				sndbgPlayed = false;
				PDEMOTIME->SetTotalSeconds(3600);
				for(int i=0;i<MAX_PLAYERS;i++)
				{
					PDATA->Players[i].Score = 0;
					PDATA->Players[i].Increment = 0;
					playerScoreDigit[i].setValue(0);
					playerIncrementDigit[i].setValue(0);
				}
				PDATA->MainGameStage = STAGE_SELECT_GAME;
			}
		}
	}
	ShowSitRole();
	DisplaySitRoleName();
	DisplayGameTime(10,30);
	DisplayPlayerScore();
	if(SubGameStage != STAGE_NONE)
	{
		pTimer->Pause();
		fightTimer->Pause();
	}
	else
	{
		pTimer->Resume();
		fightTimer->Resume();
		++time;
	}
}

void C_PRESENT::DealKeyPress()
{
	int i,j;
	if(fastest == -1)
	{
		fastest = PDATA->NextGirl(-1);
	}
	for(i=PDATA->NextGirl(-1);i<MAX_PLAYERS;i=PDATA->NextGirl(i))
	{
		int rx = rand()%100;
		j = (i - 1)/2;
		if(PKEY->PressKey(PlayersEnter[i]) || (PDATA->InDemo && rx < 4))
		{
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
			noPressCount[j] = 0;
			++pressScore[j];
		}
		else
		{
			++noPressCount[j];
			if(noPressCount[j] == 30)
			{
				noPressCount[j] = 0;
				if(pressScore[j] > 10)
				{
					--pressScore[j];
				}
			}
		}
	}
	for(i=PDATA->NextGirl(-1);i<MAX_PLAYERS;i=PDATA->NextGirl(i))
	{
		j = (i - 1)/2;
		if(pressScore[j] > pressScore[(fastest - 1)/2])
		{
			fastest = i;
		}
	}
}
