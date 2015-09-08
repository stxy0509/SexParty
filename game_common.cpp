#include "game_common.h"
#include "downtimer.h"
#include "goldcoin.h"
#include "small_game/game1.h"
#include "small_game/game2.h"
#include "small_game/game3.h"
#include "small_game/game4.h"
#include "small_game/game5.h"
#include "small_game/game6.h"
#include "small_game/overview.h"
#include <libst.h>
bool Greater(const SPLAYER& a,const SPLAYER& b)
{
	return a.p.Score > b.p.Score || 
		   (a.p.Score == b.p.Score && a.p.Index > b.p.Index);
}

namespace GAME
{
	C_KEY *PKEY  = 0;
	C_DATA *PDATA = 0;
	C_DRINK *pdrink = 0;
	C_DOWN_TIMER *minPlay = 0;
	C_DOWN_TIMER *extraPlay = 0;
	C_CONSUME_TIME *PGAMETIME = 0;
	C_CONSUME_TIME *PDEMOTIME = 0;
	C_SMALL_GAME *PSMALLGAME = 0;
	C_NUM_DIGIT playerScoreDigit[MAX_PLAYERS];
	C_NUM_DIGIT playerIncrementDigit[MAX_PLAYERS];
	C_GOLD_COIN *PCOIN[MAX_PLAYERS] = {0,};
	bool ledOn[12] = {false,};
	bool needBonus = false;
	int screenw = 1024;
	int screenh = 768;
	int playStage = 0;
	int ledTime = 0;
	int PrevSubStage = STAGE_NONE;
	int SubGameStage = STAGE_NONE;
	vgWAVE *sndCharge = 0;
	vgWAVE *sndPress = 0;
	vgWAVE *sndAdmin = 0;
	vgWAVE *sndUser = 0;
	vgWAVE *sndBonus = 0;
	vgWAVE *sndChange = 0;
	vgWAVE *sndAddTime = 0;
	vgWAVE *sndBegin = 0;
	vgWAVE *sndPokerDice = 0;
	vgSURFACE *window1 = 0;
	vgSURFACE *window2 = 0;
	vgSURFACE *man_down = 0;
	vgSURFACE *man_left = 0;
	vgSURFACE *girl_down = 0;
	vgSURFACE *girl_left = 0;
	vgSURFACE *chair_down = 0;
	vgSURFACE *chair_left = 0;
	vgSURFACE *arrow_left = 0;
	vgSURFACE *arrow_down = 0;
	vgSURFACE *time_left = 0;
	vgSURFACE *nr_left = 0;
	vgSURFACE *nr = 0;
	vgSURFACE *lednr = 0;
	vgSURFACE *demo = 0;
	vgSURFACE *outline = 0;
	vgSURFACE *fchair = 0;
	vgSURFACE *border[2] = {0,};
	vgSURFACE *fchair_left = 0;
	vgSURFACE *gameIcon[5] = {0,};
	vgSURFACE *gameName[5] = {0,};
	vgSURFACE *iconFrame = 0;
	vgSURFACE *counter = 0;
	vgSURFACE *counterF = 0;
	vgSURFACE *timebar = 0;
	vgSURFACE *watch = 0;
	vgSURFACE *watch_s = 0;
	vgSURFACE *lednr_s = 0;
	vgSURFACE *watch_s_left = 0;
	vgSURFACE *lednr_s_left = 0;
	vgSURFACE *tip[2] = {0,};
	vgSURFACE *girlname[6][2] = {{0,}};
	vgSURFACE *manname[6][2] = {{0,}};
	vgSURFACE *man = 0;
	vgSURFACE *girl = 0;
	vgSURFACE *scoref = 0;
	vgSURFACE *scorefLeft = 0;
	vgSURFACE *firstFlag = 0;
	vgSURFACE *firstFlagLeft = 0;
	vgSURFACE *upArrow = 0;
	vgSURFACE *join = 0;
	vgSURFACE *join_left = 0;
	vgSURFACE *gamenr = 0;
	vgSURFACE *tipNext = 0;
	vgSURFACE *pressAnimation[2] = {0,};
	vgSURFACE *pressAnimationL[2] = {0,};
	vgSURFACE *actNotice = 0;
	vgSURFACE *actNoticeL = 0;
	std::set<int> actor;
	std::set<int> tmpActors;
	std::set<int> keys;
	std::set<int> tmpKeys;
	std::list<int> freeMan;
	std::list<int> pickedMan;
	std::list<int> freeGirl;
	std::list<int> pickedGirl;
	std::multimap<int,int> manRank;
	std::multimap<int,int> girlRank;
	std::multimap<int,int> gmRank;
	SPLAYER pls[MAX_PLAYERS];
	int PlayersEnter[MAX_PLAYERS] = 
	{
		KEY_ENTER0,KEY_ENTER1,KEY_ENTER2,KEY_ENTER3,KEY_ENTER4,
		KEY_ENTER5,KEY_ENTER6,KEY_ENTER7,KEY_ENTER8,KEY_ENTER9
	};
	int SitRolePos[MAX_PLAYERS][2] = 
	{
		{  0,210},{-15,408},{124,625},{334,635},{569,625},
		{781,625},{923,409},{915,213},{530,  0},{364,-25}
	};
	int ArrowPos[MAX_PLAYERS][2] =
	{
		{115,260},{115,448},{174,528},{362,528},{618,528},
		{806,528},{804,447},{804,260},{580,133},{392,133}
	};
	int ChairPos[MAX_PLAYERS][2] =
	{
		{  0,207},{  0,395},{123,633},{311,633},{567,633},
		{755,633},{923,396},{923,208},{530,  0},{342,  0}
	};
	int ScorePos[MAX_PLAYERS][2] = 
	{
		{ 30,314},{ 30,503},{229,690},{416,690},{674,690},
		{863,690},{946,379},{946,192},{514, 30},{326, 30}
	};
	int SitRoleNamePos[MAX_PLAYERS][2] = 
	{
		{  0,250},{  0,439},{165,738},{352,738},{610,738},
		{799,738},{994,443},{994,256},{578,  0},{390,  0}
	};
	int FirstFlagPos[MAX_PLAYERS][2] = 
	{
		{100,188},{100,376},{102,574},{289,574},{547,574},
		{735,574},{861,503},{861,315},{639,130},{453,130}
	};
	int JoinMsgPos[MAX_PLAYERS][2] = 
	{
		{ 26,217},{ 26,403},{132,688},{320,688},{580,688},
		{768,688},{955,411},{955,220},{547, 40},{360, 40},
	};
	int PressAnimationPos[MAX_PLAYERS][2] = 
	{
		{100,223},{100,412},{137,517},{324,517},{583,517},
		{771,517},{804,411},{804,234},{549,131},{362,130}
	};
	int ActNoticePos[MAX_PLAYERS][2] = 
	{
		{ 46,226},{ 46,411},{131,631},{318,631},{575,631},
		{761,631},{920,417},{920,232},{548, 78},{360, 78},
	};
	void DisplayActNotice()
	{
		int i;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(IsActor(i))
			{
				if(i < 2)
				{
					vgDrawSurface(actNoticeL,ActNoticePos[i][0],ActNoticePos[i][1],0);
				}
				else if(i < 6)
				{
					vgDrawSurface(actNotice,ActNoticePos[i][0],ActNoticePos[i][1],0);
				}
				else if(i < 8)
				{
					vgDrawSurface(actNoticeL,ActNoticePos[i][0],ActNoticePos[i][1],
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
				}
				else
				{
					vgDrawSurface(actNotice,ActNoticePos[i][0],ActNoticePos[i][1],
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
				}
			}
		}
	}
	
	void ResetRole()
	{
		pickedGirl.clear();
		pickedMan.clear();
		freeGirl.clear();
		freeMan.clear();
		for(int i=0;i<6;i++)
		{
			freeGirl.push_back(i);
			freeMan.push_back(i);
		}
	}
	
	void DealScore(int v,int idx)
	{
		if(PDATA->SeatHaveRole(idx))
		{
			PDATA->Players[idx].Score += v;
			playerScoreDigit[idx] += v;
			if(PCOIN[idx] == 0)
			{
				PCOIN[idx] = new C_GOLD_COIN(idx);
			}
		}
	}
	
	void DealIncrement(int v,int idx)
	{
		if(PDATA->SeatHaveRole(idx))
		{
			PDATA->Players[idx].Increment += v;
			playerIncrementDigit[idx] += v;
			if(PCOIN[idx] == 0)
			{
				PCOIN[idx] = new C_GOLD_COIN(idx);
			}
		}
	}
	
	void InitNumberDigit()
	{
		int i;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			playerScoreDigit[i] = C_NUM_DIGIT(PDATA->Players[i].Score);
			playerIncrementDigit[i] = C_NUM_DIGIT(PDATA->Players[i].Increment);
		}
	}

	void LoadResForAdmin()
	{
		upArrow = LoadImageFile("picture/keyhelp/arrow.bmp",0,0,255);
		lednr = vgLoadImageFile("picture/lednr.bmp");
		sndPress = LoadWavFile("wav/io/press.wav");
	}
	
	void LoadPictureSound()
	{
		int i,j;
		char buff[32];
		
		for(i=0;i<6;i++)
		{
			for(j=0;j<2;j++)
			{
				sprintf(buff,"picture/rolename/m%d%d.bmp",i + 1,j);
				manname[i][j] = vgLoadImageFile(buff);
				manname[i][j]->alpha = 160;
				vgSetDrawMode(manname[i][j],DRAWMODE_ALPHA);
				sprintf(buff,"picture/rolename/g%d%d.bmp",i + 1,j);
				girlname[i][j] = vgLoadImageFile(buff);
				girlname[i][j]->alpha = 160;
				vgSetDrawMode(girlname[i][j],DRAWMODE_ALPHA);
			}
		}
		window1 = vgLoadImageFile("picture/window1.bmp");
		window2 = vgLoadImageFile("picture/window2.bmp");
		girl = LoadImageFile("picture/girl.bmp",0,0,255);
		man = LoadImageFile("picture/man.bmp",  0,0,255);
		join = LoadImageFile("picture/tip/join.bmp",0,0,255);
		join_left = LoadImageFile("picture/tip/join_left.bmp",0,0,255);	
		watch = LoadImageFile("picture/watch.bmp",0,0,255);
		lednr_s = vgLoadImageFile("picture/lednr_s.bmp");
		watch_s = LoadImageFile("picture/watch_s.bmp",0,0,255);
		watch_s_left = LoadImageFile("picture/watch_s_left.bmp",0,0,255);
		lednr_s_left = vgLoadImageFile("picture/lednr_s_left.bmp");
		nr_left = LoadImageFile("picture/nr_left.bmp",0,186,255);
		iconFrame = LoadImageFile("picture/frame.bmp",0,  0,255);
		man_left = LoadImageFile("picture/select_role/man/left.bmp",0,255,0);
		man_down = LoadImageFile("picture/select_role/man/down.bmp",0,255,0);
		time_left = vgLoadImageFile("picture/timeleft.bmp");
		girl_left = LoadImageFile("picture/select_role/girl/left.bmp",0,0,255);
		girl_down = LoadImageFile("picture/select_role/girl/down.bmp",0,0,255);
		chair_left = LoadImageFile("picture/chair/left.bmp",0,0,0);
		chair_down = LoadImageFile("picture/chair/down.bmp",0,0,0);
		fchair = LoadImageFile("picture/flash_chair/down.bmp",0,0,0);
		fchair_left = LoadImageFile("picture/flash_chair/left.bmp",0,0,0);
		arrow_left = LoadImageFile("picture/arrow/left.bmp",0,255,0);
		arrow_down = LoadImageFile("picture/arrow/down.bmp",0,255,0);
		scoref = LoadImageFile("picture/sf.bmp",255,255,255);
		scorefLeft = LoadImageFile("picture/sf_left.bmp",255,255,255);
		scoref->alpha = 0xA0;
		scorefLeft->alpha = 0xA0;
		AppendDrawMode(scoref,DRAWMODE_ALPHA);
		AppendDrawMode(scorefLeft,DRAWMODE_ALPHA);
		firstFlag = LoadImageFile("picture/no1.bmp",0,0,255);
		firstFlagLeft = LoadImageFile("picture/no1_left.bmp",0,0,255);
		load_bmp_files("picture/b",border,2);
		load_bmp_files("picture/game_icon/",gameIcon,5);
		load_bmp_files("picture/name/",gameName,5);
		load_bmp_files("picture/small_game/tip/",tip,2,255,0,0);
		load_bmp_files("picture/tip/down/",pressAnimation, 2,0,0,255);
		load_bmp_files("picture/tip/left/",pressAnimationL,2,0,0,255);
		gamenr = LoadImageFile("picture/small_game/tip/nr.bmp",0,0,255);
		tipNext = LoadImageFile("picture/keyhelp/next.bmp",0,0,255);
		counter = vgLoadImageFile("picture/counter.bmp");
		counterF = LoadImageFile("picture/cf.bmp",0,0,0);
		timebar = vgLoadImageFile("picture/bar.bmp");
		actNotice = LoadImageFile("picture/actn.bmp",0,0,255);
		actNoticeL = LoadImageFile("picture/actnL.bmp",0,0,255);
		
		sndAdmin = LoadWavFile("wav/io/admin.wav");
		sndUser = LoadWavFile("wav/io/user.wav");
		sndCharge = LoadWavFile("wav/io/charge.wav");
		sndChange = LoadWavFile("wav/change.wav",true);
		sndBonus = LoadWavFile("wav/bonus.wav");
		sndAddTime = LoadWavFile("wav/addt.wav");
		sndPokerDice = LoadWavFile("wav/pd.wav",true);
		load_wav_files("wav/small/m",C_SMALL_GAME::sndMusic,2,true);
		load_wav_files("wav/small/dj",C_SMALL_GAME::sndWarm,2);
		minPlay = new C_DOWN_TIMER(PDATA->FixPlayTime);
		extraPlay = new C_DOWN_TIMER(PDATA->ExtraPlayTime);
	}
	
	bool IsInKeys(int k)
	{
		return keys.find(k) != keys.end();
	}
	
	bool IsTmpKey(int k)
	{
		return tmpKeys.find(k) != tmpKeys.end();
	}
	
	bool IsActor(int who)
	{
		return actor.find(who) != actor.end();
	}
	
	bool IsTmpActor(int who)
	{
		return tmpActors.find(who) != tmpActors.end();
	}
	
	void ResetGameTip()
	{
		playStage = 0;
		minPlay->Stop();
		extraPlay->Stop();
	}
	
	bool DisplayTip()
	{
		int rtime;
		if(playStage == 0)
		{
			if(!minPlay->IsStart())
			{
				minPlay->SetNewTime(PDATA->FixPlayTime);
				minPlay->Start();
			}
			rtime = minPlay->GetRestTime();
			vgDrawSurface(tip[0],205,158,0);
			vgDrawSurface(tip[0],787,158,
				DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			display_num_vertical(gamenr,3,
				rtime,
				211,163,1);
			display_num_vertical(gamenr,3,
				PDATA->ExtraPlayTime,
				211,389,1);
			
			display_num_vertical(gamenr,3,
				rtime,
				793,560,0,true);
			display_num_vertical(gamenr,3,
				PDATA->ExtraPlayTime,
				793,336,0,true);
			if(SubGameStage != STAGE_NONE)
			{
				minPlay->Pause();
			}
			else
			{
				minPlay->Resume();
			}
			if((!PDATA->InDemo && rtime == 0) || 
			   (PDATA->InDemo && minPlay->GetElapsedTime() >= 5))
			{
				++playStage;
				minPlay->Stop();
				extraPlay->SetNewTime(PDATA->ExtraPlayTime);
				extraPlay->Start();
			}
		}
		else
		{
			rtime = extraPlay->GetRestTime();
			vgDrawSurface(tip[1],205,158,0);
			vgDrawSurface(tip[1],787,158,
				DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			display_num_vertical(gamenr,3,
				rtime,
				211,164,1);
			display_num_vertical(gamenr,3,
				rtime,
				793,560,1,true);
			if(SubGameStage != STAGE_NONE)
			{
				extraPlay->Pause();
			}
			else
			{
				extraPlay->Resume();
			}
			if((!PDATA->InDemo && (rtime == 0 || AnyActorPressEnter())) ||
				(PDATA->InDemo && extraPlay->GetElapsedTime() >= 5))			
			{
				playStage = 0;
				extraPlay->Stop();
				return true;
			}
		}
		return false;
	}
	
	void DisplayBorder()
	{
		vgDrawSurface(border[0],237,145,0);
		vgDrawSurface(border[1],256,145,0);
		vgDrawSurface(border[0],768,145,DRAWOPTION_FLIPH);
		vgDrawSurface(border[1],256,606,DRAWOPTION_FLIPV);
	}
	
	void FlagActors()
	{
		if(!actor.empty())
		{
			SET_IT it;
			it = actor.begin();
			while(it != actor.end())
			{
				LuckyStrikeOn(*it,ledTime);
				++it;
			}
		}
	}
	
	void DisplayGameTime(int x,int y)
	{
		static U32 t = 0;
		int percent; 
		C_CONSUME_TIME *p = PDATA->InDemo ? PDEMOTIME : PGAMETIME;
		percent = p->Progress();
		vgDrawSurface(counter,x,y,0);
		if(percent >= 95 && t%20 < 10)
		{
		
		}
		else
		{
			DrawPartOfTexture(timebar,0,0,
				percent*timebar->w/100,timebar->h,
				x + 7,y + 6);
		}
		display_num_horizontal(lednr,
			2,p->GetHours(),
			x + 10,y + 26,1);
		display_num_horizontal(lednr,
			2,p->GetMinutes(),
			x + 72,y + 26,1);
		display_num_horizontal(lednr,
			2,p->GetSeconds(),
			x + 135,y + 26,1);
		++t;
	}
	
	void DisplayPlayerScore()
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
					vgDrawSurface(scorefLeft,ScorePos[i][0],ScorePos[i][1],0);
					display_num_vertical(nr_left,5,
						playerIncrementDigit[i].getValue(),
						ScorePos[i][0] + 28,ScorePos[i][1] + 2,1);
					display_num_vertical(nr_left,5,
						playerScoreDigit[i].getValue(),
						ScorePos[i][0] + 4,ScorePos[i][1] + 2,1);				
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					vgDrawSurface(scoref,ScorePos[i][0],ScorePos[i][1],0);
					display_num_horizontal(nr,5,
						playerIncrementDigit[i].getValue(),
						ScorePos[i][0] + 2,ScorePos[i][1] + 4,1);
					display_num_horizontal(nr,5,
						playerScoreDigit[i].getValue(),
						ScorePos[i][0] + 2,ScorePos[i][1] + 28,1);
					break;
				case 6:
				case 7:
					vgDrawSurface(scorefLeft,ScorePos[i][0],ScorePos[i][1],
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
					display_num_vertical(nr_left,5,
						playerIncrementDigit[i].getValue(),
						ScorePos[i][0] + 4,ScorePos[i][1] + 2,1,true);
					display_num_vertical(nr_left,5,
						playerScoreDigit[i].getValue(),
						ScorePos[i][0] + 28,ScorePos[i][1] + 2,1,true);
					break;
				case 8:
				case 9:
					vgDrawSurface(scoref,ScorePos[i][0],ScorePos[i][1],
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
					display_num_horizontal(nr,5,
						playerIncrementDigit[i].getValue(),
						ScorePos[i][0] + 2,ScorePos[i][1] + 28,1,true);
					display_num_horizontal(nr,5,
						playerScoreDigit[i].getValue(),
						ScorePos[i][0] + 2,ScorePos[i][1] + 4,1,true);
					break;	
			}
		}
	}
	
	void LuckyStrikeOn(int n,int f)
	{
		if(n < 0 || n >= MAX_PLAYERS) 
		{
			return;
		}
		if(n < 2)
		{
			DrawPartOfSurface(fchair_left,3,1,f/5%3,
				ChairPos[n][0],ChairPos[n][1] - 8);
		}
		else if(n < 6)
		{
			DrawPartOfSurface(fchair,1,3,f/5%3,
				ChairPos[n][0] - 8,ChairPos[n][1] - 7);
		}
		else if(n < 8)
		{
			DrawPartOfSurface(fchair_left,3,1,f/5%3,
				ChairPos[n][0] - 8,ChairPos[n][1] - 6,
				DRAWOPTION_FLIPH);
		}
		else
		{
			DrawPartOfSurface(fchair,1,3,f/5%3,
				ChairPos[n][0] - 8,ChairPos[n][1],
				DRAWOPTION_FLIPV);
		}
	}
	
	void ArrowFocusOn(int n)
	{
		static int Frame = 0;
		if(n < 0 || n >= MAX_PLAYERS) 
		{
			return;
		}
		int idx = Frame/10%4;
		if(idx == 3)
		{
			idx = 1;
		}
		if(n < 2)
		{
			DrawPartOfSurface(arrow_left,3,1,idx,
				ArrowPos[n][0],ArrowPos[n][1]);
		}
		else if(n < 6)
		{
			DrawPartOfSurface(arrow_down,1,3,idx,
				ArrowPos[n][0],ArrowPos[n][1]);
		}
		else if(n < 8)
		{
			DrawPartOfSurface(arrow_left,3,1,idx,
				ArrowPos[n][0],ArrowPos[n][1],
				DRAWOPTION_FLIPH);
		}
		else
		{
			DrawPartOfSurface(arrow_down,1,3,idx,
				ArrowPos[n][0],ArrowPos[n][1],
				DRAWOPTION_FLIPV);
		}
		++Frame;
	}
	
	void ChairFocusOn(int n)
	{
		static int Frame = 0;
		if(n < 0 || n >= MAX_PLAYERS || !actor.empty()) 
		{
			return;
		}
		if(Frame % 20 < 10)
		{
			if(n < 2)
			{
				vgDrawSurface(chair_left,ChairPos[n][0],ChairPos[n][1],0);
			}
			else if(n < 6)
			{
				vgDrawSurface(chair_down,ChairPos[n][0],ChairPos[n][1],0);
			}
			else if(n < 8)
			{
				vgDrawSurface(chair_left,ChairPos[n][0],ChairPos[n][1],
					DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			}
			else
			{
				vgDrawSurface(chair_down,ChairPos[n][0],ChairPos[n][1],
					DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			}
		}
		++Frame;
	}
	
	int EnterOfPlayer(int n)
	{
		return PlayersEnter[n];
	}
	
	void ShowSitRole()
	{
		int i,j;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(PDATA->SeatHaveRole(i))
			{
				j = PDATA->Players[i].Index;
				if(i % 2 == 0)
				{
					switch(i)
					{
						case 0:
						{
							DrawPartOfSurface(man_left,1,6,j,
								SitRolePos[i][0],SitRolePos[i][1],0);
							break;
						}
						case 6:
						{
							DrawPartOfSurface(man_left,1,6,j,
								SitRolePos[i][0],SitRolePos[i][1],DRAWOPTION_FLIPH);
							break;
						}
						case 2:
						case 4:
						{
							DrawPartOfSurface(man_down,1,6,j,
								SitRolePos[i][0],SitRolePos[i][1],0);
							break;
						}
						case 8:
						{
							DrawPartOfSurface(man_down,1,6,j,
								SitRolePos[i][0],SitRolePos[i][1],DRAWOPTION_FLIPV);
							break;
						}
					}
				}
				else
				{
					switch(i)
					{
						case 1:
						{
							DrawPartOfSurface(girl_left,1,6,j,
								SitRolePos[i][0],SitRolePos[i][1],0);
							break;
						}
						case 7:
						{
							DrawPartOfSurface(girl_left,1,6,j,
								SitRolePos[i][0],SitRolePos[i][1],DRAWOPTION_FLIPH);
							break;
						}
						case 3:
						case 5:
						{
							DrawPartOfSurface(girl_down,1,6,j,
								SitRolePos[i][0],SitRolePos[i][1],0);
							break;
						}
						case 9:
						{
							DrawPartOfSurface(girl_down,1,6,j,
								SitRolePos[i][0],SitRolePos[i][1],DRAWOPTION_FLIPV);
							break;
						}
					}
				}
			}
		}
		DisplayActNotice();
	}
	
	void ShowRemainedTime()
	{
		C_DOWN_TIMER *Timer;
		Timer = C_DOWN_TIMER::GetTopTimer();
		if(Timer)
		{
			int rt = Timer->GetRestTime();
			DisplayWatch(rt,865,92);
		}
	}
	
	int WhoJoinGame()
	{
		int who = PDATA->NextSeat(-1);
		while(who != MAX_PLAYERS)
		{
			if(PKEY->PressTime(PlayersEnter[who]) > JOIN_QUIT_DELAY)
			{
				PKEY->ZeroPressTime(PlayersEnter[who]);
				break;
			}
			who = PDATA->NextSeat(who);
		}
		return who;
	}

	int WhoQuitGame()
	{
		int who = PDATA->NextPlayer(-1);
		while(who != MAX_PLAYERS)
		{
			if(PKEY->PressTime(PlayersEnter[who]) >= 2000)
			{
				break;
			}
			who = PDATA->NextPlayer(who);
		}
		return who;
	}
	
	bool AnyOnePressEnter()
	{
		return (PKEY->PressKey(KEY_ENTER0) || PKEY->PressKey(KEY_ENTER1) ||
			    PKEY->PressKey(KEY_ENTER2) || PKEY->PressKey(KEY_ENTER3) ||
			    PKEY->PressKey(KEY_ENTER4) || PKEY->PressKey(KEY_ENTER5) ||
			    PKEY->PressKey(KEY_ENTER6) || PKEY->PressKey(KEY_ENTER7) ||
			    PKEY->PressKey(KEY_ENTER8) || PKEY->PressKey(KEY_ENTER9));
	}
		
	bool AnyActorPressEnter()
	{
		SET_IT it;
		for(it = actor.begin();it != actor.end();++it)
		{
			if(PKEY->PressKey(EnterOfPlayer(*it)))
			{
				return true;
			}
		}
		return false;
	}
	
	void TurnLedOn(int n)
	{
		if(!ledOn[n])
		{
			OutputHigh(n);
			ledOn[n] = true;
		}
	}
	
	void TurnLedOff(int n)
	{
		if(ledOn[n])
		{
			OutputLow(n);
			ledOn[n] = false;
		}
	}
	
	void ToggleLed(int n)
	{
		if(ledOn[n])
		{
			TurnLedOff(n);
		}
		else
		{
			TurnLedOn(n);
		}
	}
	
	void AllLedOn()
	{
		for(int i=0;i<10;++i)
		{
			TurnLedOn(i);
		}
	}
	
	void ControlLed()
	{
		int i;
		if(SubGameStage == STAGE_NONE && (PSMALLGAME || pdrink))
		{
			if(actor.empty())
			{
				for(i=0;i<MAX_PLAYERS;++i) 
				{
					TurnLedOff(i);
				}
			}
			else
			{
				for(i=0;i<MAX_PLAYERS;++i)
				{
					if(IsActor(i) && ledTime % 10 < 5)
					{
						TurnLedOn(i);
					}
					else
					{
						TurnLedOff(i);
					}
				}
				++ledTime;
			}
		}
		else
		{
			if(SubGameStage == STAGE_NONE)
			{
				if(keys.empty())
				{
					for(i=0;i<10;++i) 
					{
						TurnLedOff(i);
					}
				}
				else
				{	
					for(int i=0;i<10;++i)
					{
						if(IsInKeys(i))
						{
							TurnLedOn(i);
						}
						else
						{
							TurnLedOff(i);
						}
					}
				}
			}
			else
			{
				if(tmpKeys.empty())
				{
					for(i=0;i<10;++i) 
					{
						TurnLedOff(i);
					}
				}
				else
				{	
					for(int i=0;i<10;++i)
					{
						if(IsTmpKey(i))
						{
							TurnLedOn(i);
						}
						else
						{
							TurnLedOff(i);
						}
					}
				}
			}
		}
	}
	
	C_SMALL_GAME *CreateSmallGame(bool forman,bool gift)
	{
		GAME::TYPE_INDEX ti;
		ti = C_SMALL_GAME::SelectGame(forman,gift);
		switch(ti.type)
		{
			case 0:
				return new C_GAME1(ti.index);
			case 1:
				return new C_GAME2(ti.index);
			case 2:
				return new C_GAME3(ti.index);
			case 3:
				return new C_GAME4(ti.index);
			case 4:
				return new C_GAME5(ti.index);
			case 5:
				return new C_GAME6(ti.index);
			case 6:
				return new C_OVERVIEW();
			default:
				return 0;
		}
	}

	void AddPickedRole(int index,bool isMan)
	{
		if(isMan)
		{
			pickedMan.push_back(index);
			freeMan.remove(index);
		}
		else
		{
			pickedGirl.push_back(index);
			freeGirl.remove(index);
		}
	}

	void SubPickedRole(int index,bool isMan)
	{
		if(isMan)
		{
			pickedMan.remove(index);
			freeMan.push_back(index);
		}
		else
		{
			pickedGirl.remove(index);
			freeGirl.push_back(index);
		}
	}
	
	int GetFirstFreeRole(bool isMan)
	{
		return isMan ? freeMan.front() : freeGirl.front();
	}
	
	int GetFreeMan()
	{
		LIST_IT it = freeMan.begin();
		std::advance(it,rand()%freeMan.size());
		return *it;
	}
	
	int GetFreeGirl()
	{
		LIST_IT it = freeGirl.begin();
		std::advance(it,rand()%freeGirl.size());
		return *it;
	}
	
	int GetNextFreeRole(int index,bool isMan)
	{
		LIST_IT it;
		if(isMan)
		{
			it = find_if(freeMan.begin(),freeMan.end(),
					     bind2nd(std::greater<int>(),index));
			if(it == freeMan.end())
			{
				return freeMan.front();
			}
			else
			{
				return *it;
			}
		}
		else
		{
			it = find_if(freeGirl.begin(),freeGirl.end(),
					     bind2nd(std::greater<int>(),index));
			if(it == freeGirl.end())
			{
				return freeGirl.front();
			}
			else
			{
				return *it;
			}
		}
	}
	
	bool IsScoreUpdateFinish()
	{
		int i;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(!playerScoreDigit[i].isBuffZero() || 
			   !playerIncrementDigit[i].isBuffZero())
			{
				return false;
			}
		}
		return true;
	}
	
	bool IsCoinFlyingFinish()
	{
		int i;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(PCOIN[i] && !PCOIN[i]->IsFlyingFinish())
			{
				return false;
			}
		}
		return true;
	}
	
	void DisplayWatch(int n,int x,int y,bool flip)
	{
		if(!flip)
		{
			vgDrawSurface(watch,x,y,0);
			display_num_horizontal(lednr,2,
				n,x + 15,y + 30,1);
		}
		else
		{
			vgDrawSurface(watch,x,y,
				DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			display_num_horizontal(lednr,2,
				n,x + 15,y + 30,1,true);
		}
	}
	
	void DisplaySmallWatch(int n,int x,int y,bool flip)
	{
		if(!flip)
		{
			vgDrawSurface(watch_s,x,y,0);
			display_num_horizontal(lednr_s,2,
				n,x + 15,y + 28,1);
		}
		else
		{
			vgDrawSurface(watch_s,x,y,
				DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			display_num_horizontal(lednr_s,2,
				n,x + 15,y + 20,1,true);
		}
	}
	
	void DisplaySmallWatchLeft(int n,int x,int y,bool flip)
	{
		if(!flip)
		{
			vgDrawSurface(watch_s_left,x,y,0);
			display_num_vertical(lednr_s_left,
				2,n,x + 20,y + 15,1);
		}
		else
		{
			vgDrawSurface(watch_s_left,x,y,
				DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
			display_num_vertical(lednr_s_left,
				2,n,x + 28,y + 14,1,true);
		}
	}
	
	void DisplayRoleName(bool isMan,int index,int dir,int x,int y)
	{
		if(isMan)
		{
			switch(dir)
			{
				case 0:
					vgDrawSurface(manname[index][1],x,y,0);
					break;
				case 1:
					vgDrawSurface(manname[index][0],x,y,0);
					break;
				case 2:
					vgDrawSurface(manname[index][1],x,y,
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
					break;
				case 3:
					vgDrawSurface(manname[index][0],x,y,
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
					break;
			}
		}
		else
		{
			switch(dir)
			{
				case 0:
					vgDrawSurface(girlname[index][1],x,y,0);
					break;
				case 1:
					vgDrawSurface(girlname[index][0],x,y,0);
					break;
				case 2:
					vgDrawSurface(girlname[index][1],x,y,
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
					break;
				case 3:
					vgDrawSurface(girlname[index][0],x,y,
						DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
					break;
			}
		}
	}
	
	void DisplaySitRoleName()
	{
		int i,j;
		int dir[MAX_PLAYERS] = 
		{
			0,0,1,1,1,1,2,2,3,3
		};
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(PDATA->SeatHaveRole(i))
			{
				j = PDATA->Players[i].Index;
				DisplayRoleName(i%2 == 0,j,dir[i],
					SitRoleNamePos[i][0],
					SitRoleNamePos[i][1]);
			}
		}
	}
	
	void DisplayFirstFlag()
	{
		int i,max = 0,maxs = 0;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if((PDATA->Players[i].Score + PDATA->Players[i].Increment) > maxs)
			{
				max = i;
				maxs = PDATA->Players[i].Score + PDATA->Players[i].Increment;
			}
		}
		switch(max)
		{
			case 0:
			case 1:
				vgDrawSurface(firstFlagLeft,FirstFlagPos[max][0],FirstFlagPos[max][1],0);
				break;
			case 2:
			case 3:
			case 4:
			case 5:
				vgDrawSurface(firstFlag,FirstFlagPos[max][0],FirstFlagPos[max][1],0);
				break;
			case 6:
			case 7:
				vgDrawSurface(firstFlagLeft,FirstFlagPos[max][0],FirstFlagPos[max][1],
					DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
				break;
			case 8:
			case 9:
				vgDrawSurface(firstFlag,FirstFlagPos[max][0],FirstFlagPos[max][1],
					DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
				break;
		}
	}
	
	void DisplayJoinMessage()
	{
		int i;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(!PDATA->SeatHaveRole(i))
			{
				switch(i)
				{
					case 0:
					case 1:
						vgDrawSurface(join_left,JoinMsgPos[i][0],JoinMsgPos[i][1],0);
						break;
					case 2:
					case 3:
					case 4:
					case 5:
						vgDrawSurface(join,JoinMsgPos[i][0],JoinMsgPos[i][1],0);
						break;
					case 6:
					case 7:
						vgDrawSurface(join_left,JoinMsgPos[i][0],JoinMsgPos[i][1],
							DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
						break;
					case 8:
					case 9:
						vgDrawSurface(join,JoinMsgPos[i][0],JoinMsgPos[i][1],
							DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
						break;
				}
			}
		}
	}
	
	void SetUsableKeys()
	{
		int i;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(PDATA->SeatHaveRole(i))
			{
				if(!IsInKeys(i))
				{
					keys.insert(i);
				}
			}
			else if(IsInKeys(i))
			{
				keys.erase(i);
			}
		}
	}
	
	void DisplayPressAnimation(int p)
	{
		static U32 f = 0;
		vgSURFACE **s = pressAnimation;
		U32 option = 0;
		
		if(p < 2)
		{
			s = pressAnimationL;
		}
		else if(p < 6)
		{
			
		}
		else if(p < 8)
		{
			s = pressAnimationL;
			option = DRAWOPTION_FLIPH | DRAWOPTION_FLIPV;
		}
		else
		{
			option = DRAWOPTION_FLIPH | DRAWOPTION_FLIPV;
		}
		vgDrawSurface(s[f/4%2],
			PressAnimationPos[p][0],
			PressAnimationPos[p][1],
			option);
		++f;
	}
	
	void DisplayTipNext()
	{
		vgDrawSurface(upArrow,785, 0,0);
		vgDrawSurface(tipNext,760,28,0);
	}
}
