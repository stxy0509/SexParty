#include "diceh.h"
#include <libst.h>
#include "uptimer.h"
#include "game_common.h"
using GAME::nr;
using GAME::nr_left;
using GAME::PDATA;
using GAME::PKEY;
using GAME::C_DICE_HANDLE;
int C_DICE_HANDLE::playCount = 0;
vgWAVE *C_DICE_HANDLE::sndRoll = 0;
vgSURFACE *C_DICE_HANDLE::action = 0;
vgSURFACE *C_DICE_HANDLE::actionLeft = 0;
vgSURFACE *C_DICE_HANDLE::picPoints = 0;
vgSURFACE *C_DICE_HANDLE::picPointsLeft = 0;
vgSURFACE *C_DICE_HANDLE::picTriple = 0;
vgSURFACE *C_DICE_HANDLE::picTripleLeft = 0;
C_DICE_HANDLE *C_DICE_HANDLE::pdice[MAX_PLAYERS] = {0,};
int C_DICE_HANDLE::TablePos[MAX_PLAYERS][2] = 
{
	{106,201},{106,377},{106,484},{309,484},{564,484},
	{755,484},{772,377},{772,201},{545,136},{331,136}
};
int C_DICE_HANDLE::TriplePos[4][2] = 
{
	{98,0},{0,30},{30,77},{77,98}
};
int C_DICE_HANDLE::TimePos[4][2] = 
{
	{  0,90},{90,125},{125,0},{0,  0}
};
int C_DICE_HANDLE::DicePos[4][3][2] = 
{
	{
		{56,21},{31, 6},{28,39},
	},
	{
		{21,55},{ 6, 80},{39,83},
	},
	{
		{55,93},{ 80,108},{83,75},
	},
	{
		{93,56},{108,31},{75,28}
	}
};

C_DICE_HANDLE::C_DICE_HANDLE(int p):
m_points(0),m_pos(p),m_score(0),
m_stage(0),m_time(0),m_showScore(false),
m_dispTriple(false),
ptimer(new C_UP_TIMER(WAIT_TIME)),
begin(false)
{
	C_UP_TIMER::AddTimer(ptimer);
	LoadResources();
	switch(m_pos)
	{
		case 0:
		case 1:
			m_dir = 0;
			m_flip = 0;
			ptriple = picTripleLeft;
			paction = actionLeft;
			ppoints = picPointsLeft;
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			m_dir = 1;
			m_flip = 0;
			ptriple = picTriple;
			paction = action;
			ppoints = picPoints;
			break;
		case 6:
		case 7:
			m_dir = 2;
			m_flip = DRAWOPTION_FLIPH | DRAWOPTION_FLIPV;
			ptriple = picTripleLeft;
			paction = actionLeft;
			ppoints = picPointsLeft;
			break;
		case 8:
		case 9:
			m_dir = 3;
			m_flip = DRAWOPTION_FLIPH | DRAWOPTION_FLIPV;
			ptriple = picTriple;
			paction = action;
			ppoints = picPoints;
			break;
	}
	pdice[m_pos] = this;
}

C_DICE_HANDLE::~C_DICE_HANDLE()
{
	C_UP_TIMER::DelTimer(ptimer);
	delete ptimer;
	pdice[m_pos] = 0;
}

void C_DICE_HANDLE::LoadResources()
{
	if(action == 0)
	{
		action = LoadImageFile("picture/dice/action.bmp",255,255,255);
		actionLeft = LoadImageFile("picture/dice/action_left.bmp",255,255,255);
		picTriple = LoadImageFile("picture/dice/bz.bmp",0,0,0);
		picTripleLeft = LoadImageFile("picture/dice/bz_left.bmp",0,0,0);
		picPoints = LoadImageFile("picture/dice/pt.bmp",0,0,255);
		picPointsLeft = LoadImageFile("picture/dice/pt_left.bmp",0,0,255);
		sndRoll = LoadWavFile("wav/dice/roll.wav",true);
	}
}

void C_DICE_HANDLE::UnloadResources()
{
	if(action != 0)
	{
		ReleaseSurface(&picPointsLeft);
		ReleaseSurface(&picPoints);
		ReleaseSurface(&picTripleLeft);
		ReleaseSurface(&picTriple);
		ReleaseSurface(&actionLeft);
		ReleaseSurface(&action);
	}
}

void C_DICE_HANDLE::DisplayTriple()
{
	vgDrawSurface(ptriple,
		TablePos[m_pos][0] + TriplePos[m_dir][0],
		TablePos[m_pos][1] + TriplePos[m_dir][1],
		m_flip);
}

void C_DICE_HANDLE::DisplayClosed()
{
	DrawPartOfSurface(paction,5,6,0,
		TablePos[m_pos][0],TablePos[m_pos][1],m_flip);
}

void C_DICE_HANDLE::DisplayOpening()
{
	if(m_time < 26*SPEED)
	{
		DrawPartOfSurface(paction,5,6,m_time/SPEED,
			TablePos[m_pos][0],TablePos[m_pos][1],m_flip);
		if(m_time == 24*SPEED)
		{
			m_showScore = true;
			DealIncrement(PointsToScore(),m_pos);
			gmRank.insert(std::make_pair(m_points,m_pos));
		}
	}
	else
	{
		DrawPartOfSurface(paction,5,6,25,
			TablePos[m_pos][0],TablePos[m_pos][1],m_flip);
	}
	if(m_time >= 24*SPEED)
	{
		if(m_time == 24*SPEED)
		{
			if(playCount > 0)
			{
				--playCount;
				if(playCount == 0)
				{
					vgSoundStop(sndRoll);
				}
			}
		}
		DisplayPoints();
		if(m_time == 35*SPEED && IsTriple())
		{
			m_dispTriple = true;
			DealIncrement(2*PointsToScore(),m_pos);
		}
		if(m_dispTriple)
		{
			DisplayTriple();
		}
	}
}

void C_DICE_HANDLE::Reset()
{
	m_time = 0;
	m_stage = 0;
	ptimer->Stop();
	m_showScore = false;
	m_dispTriple = false;
	if(playCount > 0)
	{
		--playCount;
		if(playCount == 0)
		{
			vgSoundStop(sndRoll);
		}
	}
	begin = false;
}

void C_DICE_HANDLE::Play()
{
	if(!begin)
	{
		return;
	}
	if(m_stage == 0)
	{
		if(!ptimer->IsStart())
		{
			ptimer->Start();
		}
		DisplayClosed();
		DisplayTime();
		if(ptimer->GetElapsedTime() >= WAIT_TIME || 
           PKEY->PressKey(EnterOfPlayer(m_pos)))
		{
			if(ptimer->GetElapsedTime() < WAIT_TIME)
			{
				vgSoundPlay(sndPress,PDATA->Vol*255/100);
			}
			GenerateResult();
			++m_stage;
			m_time = 0;
			++playCount;
			if(!vgSoundIsPlaying(sndRoll))
			{
				vgSoundPlay(sndRoll,PDATA->Vol*255/100);
			}
		}
	}
	else if(m_stage == 1)
	{
		DisplayOpening();
	}
	if(SubGameStage == STAGE_NONE)
	{
		ptimer->Resume();
		++m_time;
	}
	else
	{
		ptimer->Pause();
	}
}

void C_DICE_HANDLE::GenerateResult()
{
	int rn = rand()%120;
	m_points = rand()%16 + 3;
	switch(m_points)
	{
		case 3: 
			m_p[0] = 1;m_p[1] = 1;m_p[2] = 1;
			break;
		case 4: 
			m_p[0] = 1;m_p[1] = 1;m_p[2] = 2;
			break;
		case 5: 
			if(rn < 60)
			{
				m_p[0] = 1;m_p[1] = 1;m_p[2] = 3;
			}
			else
			{
				m_p[0] = 1;m_p[1] = 2;m_p[2] = 2;
			}
			break;
		case 6: 
			if(rn < 40)
			{
				m_p[0] = 1;m_p[1] = 1;m_p[2] = 4;
			}
			else if(rn < 80)
			{
				m_p[0] = 1;m_p[1] = 2;m_p[2] = 3;
			}
			else
			{
				m_p[0] = 2;m_p[1] = 2;m_p[2] = 2;
			}
			break;
		case 7: 
			if(rn < 30)
			{
				m_p[0] = 1;m_p[1] = 1;m_p[2] = 5;
			}
			else if(rn < 60)
			{
				m_p[0] = 1;m_p[1] = 2;m_p[2] = 4;
			}
			else if(rn < 90)
			{
				m_p[0] = 1;m_p[1] = 3;m_p[2] = 3;
			}
			else
			{
				m_p[0] = 2;m_p[1] = 2;m_p[2] = 3;
			}
			break;		
		case 8: 
			if(rn < 24)
			{
				m_p[0] = 1;m_p[1] = 1;m_p[2] = 6;
			}
			else if(rn < 48)
			{
				m_p[0] = 1;m_p[1] = 2;m_p[2] = 5;
			}
			else if(rn < 72)
			{
				m_p[0] = 1;m_p[1] = 3;m_p[2] = 4;
			}
			else if(rn < 96)
			{
				m_p[0] = 2;m_p[1] = 2;m_p[2] = 4;
			}
			else
			{
				m_p[0] = 2;m_p[1] = 3;m_p[2] = 3;
			}
			break;
			
		case 9:	
			if(rn < 20)
			{
				m_p[0] = 1;m_p[1] = 2;m_p[2] = 6;
			}
			else if(rn < 40)
			{
				m_p[0] = 1;m_p[1] = 3;m_p[2] = 5;
			}
			else if(rn < 60)
			{
				m_p[0] = 1;m_p[1] = 4;m_p[2] = 4;
			}
			else if(rn < 80)
			{
				m_p[0] = 2;m_p[1] = 2;m_p[2] = 5;
			}
			else if(rn < 100)
			{
				m_p[0] = 2;m_p[1] = 3;m_p[2] = 4;
			}
			else
			{
				m_p[0] = 3;m_p[1] = 3;m_p[2] = 3;
			}
			break;
		case 10:
			if(rn < 20)
			{
				m_p[0] = 1;m_p[1] = 3;m_p[2] = 6;
			}
			else if(rn < 40)
			{
				m_p[0] = 1;m_p[1] = 4;m_p[2] = 5;
			}
			else if(rn < 60)
			{
				m_p[0] = 2;m_p[1] = 2;m_p[2] = 6;
			}
			else if(rn < 80)
			{
				m_p[0] = 2;m_p[1] = 3;m_p[2] = 5;
			}
			else if(rn < 100)
			{
				m_p[0] = 2;m_p[1] = 4;m_p[2] = 4;
			}
			else
			{
				m_p[0] = 3;m_p[1] = 3;m_p[2] = 4;
			}
			break;
		case 11:
			if(rn < 20)
			{
				m_p[0] = 1;m_p[1] = 4;m_p[2] = 6;
			}
			else if(rn < 40)
			{
				m_p[0] = 1;m_p[1] = 5;m_p[2] = 5;
			}
			else if(rn < 60)
			{
				m_p[0] = 2;m_p[1] = 3;m_p[2] = 6;
			}
			else if(rn < 80)
			{
				m_p[0] = 2;m_p[1] = 4;m_p[2] = 5;
			}
			else if(rn < 100)
			{
				m_p[0] = 3;m_p[1] = 3;m_p[2] = 5;
			}
			else
			{
				m_p[0] = 3;m_p[1] = 4;m_p[2] = 4;
			}
			break;
		case 12:
			if(rn < 20)
			{
				m_p[0] = 1;m_p[1] = 5;m_p[2] = 6;
			}
			else if(rn < 40)
			{
				m_p[0] = 2;m_p[1] = 4;m_p[2] = 6;
			}
			else if(rn < 60)
			{
				m_p[0] = 2;m_p[1] = 5;m_p[2] = 5;
			}
			else if(rn < 80)
			{
				m_p[0] = 3;m_p[1] = 3;m_p[2] = 6;
			}
			else if(rn < 100)
			{
				m_p[0] = 3;m_p[1] = 4;m_p[2] = 5;
			}
			else
			{
				m_p[0] = 4;m_p[1] = 4;m_p[2] = 4;
			}
			break;
		case 13:
			if(rn < 24)
			{
				m_p[0] = 1;m_p[1] = 6;m_p[2] = 6;
			}
			else if(rn < 48)
			{
				m_p[0] = 2;m_p[1] = 5;m_p[2] = 6;
			}
			else if(rn < 72)
			{
				m_p[0] = 3;m_p[1] = 4;m_p[2] = 6;
			}
			else if(rn < 96)
			{
				m_p[0] = 3;m_p[1] = 5;m_p[2] = 5;
			}
			else
			{
				m_p[0] = 4;m_p[1] = 4;m_p[2] = 5;
			}
			break;
		case 14:
			if(rn < 30)
			{
				m_p[0] = 2;m_p[1] = 6;m_p[2] = 6;
			}
			else if(rn < 60)
			{
				m_p[0] = 3;m_p[1] = 5;m_p[2] = 6;
			}
			else if(rn < 90)
			{
				m_p[0] = 4;m_p[1] = 4;m_p[2] = 6;
			}
			else
			{
				m_p[0] = 4;m_p[1] = 5;m_p[2] = 5;
			}
			break;
		case 15:
			if(rn < 40)
			{
				m_p[0] = 3;m_p[1] = 6;m_p[2] = 6;
			}
			else if(rn < 80)
			{
				m_p[0] = 4;m_p[1] = 5;m_p[2] = 6;
			}
			else
			{
				m_p[0] = 5;m_p[1] = 5;m_p[2] = 5;
			}
			break;
		case 16:
			if(rn < 60)
			{
				m_p[0] = 4;m_p[1] = 6;m_p[2] = 6;
			}
			else
			{
				m_p[0] = 5;m_p[1] = 5;m_p[2] = 6;
			}
			break;
		case 17:
			m_p[0] = 5;m_p[1] = 6;m_p[2] = 6;
			break;
		case 18:
			m_p[0] = 6;m_p[1] = 6;m_p[2] = 6;
			break;
	}
}

void C_DICE_HANDLE::DisplayTime()
{
	int timeLeft = WAIT_TIME - ptimer->GetElapsedTime();
	switch(m_dir)
	{
		case 0:
			DisplaySmallWatchLeft(timeLeft,
				TablePos[m_pos][0] + TimePos[m_dir][0],
				TablePos[m_pos][1] + TimePos[m_dir][1]);
			break;		
		case 1:
			DisplaySmallWatch(timeLeft,
				TablePos[m_pos][0] + TimePos[m_dir][0],
				TablePos[m_pos][1] + TimePos[m_dir][1]);
			break;
		case 2:
			DisplaySmallWatchLeft(timeLeft,
				TablePos[m_pos][0] + TimePos[m_dir][0],
				TablePos[m_pos][1] + TimePos[m_dir][1],true);
			break;
		case 3:
			DisplaySmallWatch(timeLeft,
				TablePos[m_pos][0] + TimePos[m_dir][0],
				TablePos[m_pos][1] + TimePos[m_dir][1],true);
			break;
	}
}

int C_DICE_HANDLE::PointsToScore()
{
	return m_points*10;
}

void C_DICE_HANDLE::DisplayPoints()
{
	DrawPartOfSurface(ppoints,1,6,m_p[0] - 1,
		TablePos[m_pos][0] + DicePos[m_dir][0][0],
		TablePos[m_pos][1] + DicePos[m_dir][0][1],
		m_flip);
	DrawPartOfSurface(ppoints,1,6,m_p[1] - 1,
		TablePos[m_pos][0] + DicePos[m_dir][1][0],
		TablePos[m_pos][1] + DicePos[m_dir][1][1],
		m_flip);
	DrawPartOfSurface(ppoints,1,6,m_p[2] - 1,
		TablePos[m_pos][0] + DicePos[m_dir][2][0],
		TablePos[m_pos][1] + DicePos[m_dir][2][1],
		m_flip);
}

bool C_DICE_HANDLE::AllFinish()
{
	int i;
	for(i=0;i<MAX_PLAYERS;i++)
	{
		if(pdice[i] && (!pdice[i]->IsFinish()))
		{
			return false;
		}
	}
	return true;
}

void C_DICE_HANDLE::AllReset()
{
	int i;
	for(i=0;i<MAX_PLAYERS;i++)
	{
		if(pdice[i] != 0)
		{
			pdice[i]->Reset();
		}
	}
}

bool C_DICE_HANDLE::IsTriple()
{
	return m_p[0] == m_p[1] && m_p[1] == m_p[2];
}

bool C_DICE_HANDLE::IsFinish()
{
	return !begin || (m_stage == 1 && m_time >= 45*SPEED);
}

void C_DICE_HANDLE::BeginToPlay()
{
	begin = true;
}
