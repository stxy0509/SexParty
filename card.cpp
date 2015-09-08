#include "card.h"
#include "game_common.h"
#include <libst.h>
using GAME::PKEY;
using GAME::PDATA;
using GAME::EnterOfPlayer;
using GAME::C_CARD;
vgSURFACE *C_CARD::pokers = 0;
vgSURFACE *C_CARD::pokersL = 0;
vgSURFACE *C_CARD::backCover = 0;
vgSURFACE *C_CARD::backCoverL = 0;
int C_CARD::InitialCardPos[2][2] = 
{
	{476,422},{462,436}
};
int C_CARD::ClockPos[MAX_PLAYERS][2] = 
{
	{100,279},{100,463},
	{189,573},{376,573},{637,573},{822,573},
	{860,426},{860,238},
	{562,131},{372,131},
};
int C_CARD::FinalCardPos[MAX_PLAYERS][2][2] = 
{
	{{101,207},{101,280}},{{101,387},{101,460}},
	{{152,533},{225,533}},{{313,533},{386,533}},
	{{556,533},{629,533}},{{723,533},{796,533}},
	
	{{823,461},{823,388}},{{823,291},{823,218}},
	
	{{610,133},{537,133}},{{420,133},{347,133}},
};
C_CARD::C_CARD(int pos):
m_pos(pos),m_time(0),m_state(NONE),
m_card1(0),m_card2(0)
{
	LoadResources();
	InitializePosition();
	int flyingTime = 25;
	m_dx[0] = (FinalCardPos[pos][0][0] - m_x[0])/flyingTime;
	m_dy[0] = (FinalCardPos[pos][0][1] - m_y[0])/flyingTime;
	m_dx[1] = (FinalCardPos[pos][1][0] - m_x[1])/flyingTime;
	m_dy[1] = (FinalCardPos[pos][1][1] - m_y[1])/flyingTime;
}

C_CARD::~C_CARD()
{
	
}

void C_CARD::InitializePosition()
{
	if(m_pos < 2)
	{
		m_dir = 0;
		m_option = 0;
		m_x[0] = InitialCardPos[1][0];
		m_y[0] = InitialCardPos[1][1];
		m_x[1] = InitialCardPos[1][0];
		m_y[1] = InitialCardPos[1][1];
	}
	else if(m_pos < 6)
	{
		m_dir = 1;
		m_option = 0;
		m_x[0] = InitialCardPos[0][0];
		m_y[0] = InitialCardPos[0][1];
		m_x[1] = InitialCardPos[0][0];
		m_y[1] = InitialCardPos[0][1];
	}
	else if(m_pos < 8)
	{
		m_dir = 2;
		m_option = DRAWOPTION_FLIPH | DRAWOPTION_FLIPV;
		m_x[0] = InitialCardPos[1][0];
		m_y[0] = InitialCardPos[1][1];
		m_x[1] = InitialCardPos[1][0];
		m_y[1] = InitialCardPos[1][1];
	}
	else
	{
		m_dir = 3;
		m_option = DRAWOPTION_FLIPH | DRAWOPTION_FLIPV;
		m_x[0] = InitialCardPos[0][0];
		m_y[0] = InitialCardPos[0][1];
		m_x[1] = InitialCardPos[0][0];
		m_y[1] = InitialCardPos[0][1];
	}
}

void C_CARD::LoadResources()
{
	if(pokers == 0)
	{
		pokers = vgLoadImageFile("picture/poker/poker.bmp");
		pokersL = vgLoadImageFile("picture/poker/poker1.bmp");
		backCover = vgLoadImageFile("picture/poker/backcover.bmp");
		backCoverL = vgLoadImageFile("picture/poker/backcover1.bmp");
	}
}

void C_CARD::UnloadResources()
{
	if(pokers != 0)
	{
		ReleaseSurface(&pokers);
		ReleaseSurface(&backCover);
		ReleaseSurface(&pokersL);
		ReleaseSurface(&backCoverL);
	}
}

void C_CARD::BeginDistribute()
{
	m_state = DISTRIBUTE;
	m_time = 0;
}

void C_CARD::Play()
{
	if(m_state == NONE)
	{
		
	}
	else if(m_state == DISTRIBUTE)
	{
		m_x[0] += m_dx[0];
		m_y[0] += m_dy[0];
		
		m_x[1] += m_dx[1];
		m_y[1] += m_dy[1];
		if(m_dir == 0 || m_dir == 2)
		{
			vgDrawSurface(backCoverL,m_x[0],m_y[0],m_option);
			vgDrawSurface(backCoverL,m_x[1],m_y[1],m_option);
		}
		else
		{
			vgDrawSurface(backCover,m_x[0],m_y[0],m_option);
			vgDrawSurface(backCover,m_x[1],m_y[1],m_option);
		}
		if(m_time == 25)
		{
			m_state = WAITTOOPEN;
			m_time = 0;
		}
	}
	else if(m_state == WAITTOOPEN)
	{
		if(m_dir == 0 || m_dir == 2)
		{
			vgDrawSurface(backCoverL,m_x[0],m_y[0],m_option);
			vgDrawSurface(backCoverL,m_x[1],m_y[1],m_option);
		}
		else
		{
			vgDrawSurface(backCover,m_x[0],m_y[0],m_option);
			vgDrawSurface(backCover,m_x[1],m_y[1],m_option);
		}
		
		int timeLeft = 8 - m_time/20;
		switch(m_dir)
		{
			case 0:
				DisplaySmallWatchLeft(timeLeft,
					ClockPos[m_pos][0],
					ClockPos[m_pos][1]);
				break;		
			case 1:
				DisplaySmallWatch(timeLeft,
					ClockPos[m_pos][0],
					ClockPos[m_pos][1]);
				break;
			case 2:
				DisplaySmallWatchLeft(timeLeft,
					ClockPos[m_pos][0],
					ClockPos[m_pos][1],true);
				break;
			case 3:
				DisplaySmallWatch(timeLeft,
					ClockPos[m_pos][0],
					ClockPos[m_pos][1],true);
				break;
		}
		if(timeLeft == 0 || PKEY->PressKey(EnterOfPlayer(m_pos)))
		{
			m_state = OPENING1;
			m_time = 0;
			if(timeLeft != 0)
			{
				vgSoundPlay(sndPress,PDATA->Vol*255/100);
			}
		}
	}
	else if(m_state == OPENING1)
	{
		int dt = 2;
		int sw,sh;
		float zoom;
		vgRECT rect1,rect2;
		
		if(m_time < 9*dt)
		{
			zoom = 1.0 - m_time/(9.0*dt);
			if(m_dir == 0 || m_dir == 2)
			{
				sw = backCoverL->w;
				sh = backCoverL->h;
				rect2.x = m_x[0];
				rect2.y = m_y[0] + (1 - zoom)/2*sh;
				rect2.w = sw;
				rect2.h = sh*zoom;
				vgDrawStretchSurface(backCoverL,0,&rect2,m_option);
				vgDrawSurface(backCoverL,m_x[1],m_y[1],m_option);
			}
			else
			{
				sw = backCover->w;
				sh = backCover->h;
				rect2.x = m_x[0] + (1 - zoom)/2*sw;
				rect2.y = m_y[0];
				rect2.w = sw*zoom;
				rect2.h = sh;
				vgDrawStretchSurface(backCover,0,&rect2,m_option);
				vgDrawSurface(backCover,m_x[1],m_y[1],m_option);
			}
		}
		else
		{
			zoom = (m_time - 9*dt)/(9.0*dt);
			if(m_dir == 0 || m_dir == 2)
			{
				sw = pokersL->w/4;
				sh = pokersL->h/14;
				rect1.x = m_card1%4*sw;
				rect1.y = m_card1/4*sh;
				rect1.w = sw;
				rect1.h = sh;
				
				rect2.x = m_x[0];
				rect2.y = m_y[0] + (1 - zoom)/2*sh;
				rect2.w = sw;
				rect2.h = sh*zoom;
				vgDrawStretchSurface(pokersL,&rect1,&rect2,m_option);
				vgDrawSurface(backCoverL,m_x[1],m_y[1],m_option);
			}
			else
			{
				sw = pokers->w/14;
				sh = pokers->h/4;
				rect1.x = m_card1%14*sw;
				rect1.y = m_card1/14*sh;
				rect1.w = sw;
				rect1.h = sh;
				
				rect2.w = sw*zoom;
				rect2.h = sh;
				rect2.x = m_x[0] + (1 - zoom)/2*sw;
				rect2.y = m_y[0];
				vgDrawStretchSurface(pokers,&rect1,&rect2,m_option);
				vgDrawSurface(backCover,m_x[1],m_y[1],m_option);
			}
			if(m_time == 18*dt)
			{
				m_time = 0;
				m_state = OPENING2;
			}
		}
	}
	else if(m_state == OPENING2)
	{
		int dt = 2;
		int sw,sh;
		float zoom;
		vgRECT rect1,rect2;
		
		if(m_time < 9*dt)
		{
			zoom = 1.0 - m_time/(9.0*dt);
			if(m_dir == 0 || m_dir == 2)
			{
				sw = backCoverL->w;
				sh = backCoverL->h;
				rect2.x = m_x[1];
				rect2.y = m_y[1] + (1 - zoom)/2*sh;
				rect2.w = sw;
				rect2.h = sh*zoom;
				DrawPartOfSurface(pokersL,14,4,m_card1,m_x[0],m_y[0],m_option);
				vgDrawStretchSurface(backCoverL,0,&rect2,m_option);
			}
			else
			{
				sw = backCover->w;
				sh = backCover->h;
				rect2.x = m_x[1] + (1 - zoom)/2*sw;
				rect2.y = m_y[1];
				rect2.w = sw*zoom;
				rect2.h = sh;
				DrawPartOfSurface(pokers,4,14,m_card1,m_x[0],m_y[0],m_option);
				vgDrawStretchSurface(backCover,0,&rect2,m_option);
			}
		}
		else
		{
			zoom = (m_time - 9*dt)/(9.0*dt);
			if(m_dir == 0 || m_dir == 2)
			{
				sw = pokersL->w/4;
				sh = pokersL->h/14;
				rect1.x = m_card2%4*sw;
				rect1.y = m_card2/4*sh;
				rect1.w = sw;
				rect1.h = sh;
				
				rect2.x = m_x[1];
				rect2.y = m_y[1] + (1 - zoom)/2*sh;
				rect2.w = sw;
				rect2.h = sh*zoom;
				DrawPartOfSurface(pokersL,14,4,m_card1,m_x[0],m_y[0],m_option);
				vgDrawStretchSurface(pokersL,&rect1,&rect2,m_option);
			}
			else
			{
				sw = pokers->w/14;
				sh = pokers->h/4;
				rect1.x = m_card2%14*sw;
				rect1.y = m_card2/14*sh;
				rect1.w = sw;
				rect1.h = sh;
				
				rect2.w = sw*zoom;
				rect2.h = sh;
				rect2.x = m_x[1] + (1 - zoom)/2*sw;
				rect2.y = m_y[1];
				DrawPartOfSurface(pokers,4,14,m_card1,m_x[0],m_y[0],m_option);
				vgDrawStretchSurface(pokers,&rect1,&rect2,m_option);
			}
			if(m_time == 18*dt)
			{
				m_time = 0;
				m_state = OPENED;
			}
		}
	}
	else if(m_state == OPENED)
	{
		if(m_dir == 0 || m_dir == 2)
		{
			DrawPartOfSurface(pokersL,14,4,m_card1,m_x[0],m_y[0],m_option);
			DrawPartOfSurface(pokersL,14,4,m_card2,m_x[1],m_y[1],m_option);
		}
		else
		{
			DrawPartOfSurface(pokers,4,14,m_card1,m_x[0],m_y[0],m_option);
			DrawPartOfSurface(pokers,4,14,m_card2,m_x[1],m_y[1],m_option);
		}
		if(m_time == 10)
		{
			int v = PointOfCard(m_card1) + PointOfCard(m_card2);
			if(m_card1%13 == m_card2%13 && m_card1 < 52 && m_card2 < 52)
			{
				v += v;
			}
			DealIncrement(v,m_pos);
		}
	}
	if(SubGameStage == STAGE_NONE)
	{
		++m_time;
	}
}

int C_CARD::PointOfCard(int idx)
{
	if(idx < 52)
	{
		return (idx%13 + 1)*10;
	}
	else if(idx == 52)
	{
		return 200;
	}
	else if(idx == 53)
	{
		return 300;
	}
}

void C_CARD::SetCards(int c1,int c2)
{
	if(c1 < c2)
	{
		m_card1 = c1;
		m_card2 = c2;
	}
	else
	{
		m_card1 = c2;
		m_card2 = c1;
	}
}

bool C_CARD::IsStart()
{
	return m_state != NONE;
}

bool C_CARD::IsOpened()
{
	return m_state >= OPENED && m_time > 10;
}

void C_CARD::Reset()
{
	m_time = 0;
	m_state = NONE;
	InitializePosition();
}
