#include "smallgame.h"
#include "../game_common.h"
#include <libst.h>
using GAME::PDATA;
using GAME::C_SMALL_GAME;
using GAME::GAME_INFO;
using GAME::TYPE_INDEX;
using GAME::CHIEF_MAN;
using GAME::CHIEF_GIRL;
using GAME::CHIEF_EITHER;
std::list<int> operator+(const std::list<int>& a,
						 const std::list<int>& b)
{
	std::list<int> sum(a);
	sum.insert(sum.begin(),b.begin(),b.end());
	return sum;
}

int C_SMALL_GAME::manType = 0;
int C_SMALL_GAME::girlType = 0;
bool C_SMALL_GAME::listInit = false;
vgWAVE *C_SMALL_GAME::sndMusic[2] = {0,};
vgWAVE *C_SMALL_GAME::sndWarm[2] = {0,};
std::list<int> C_SMALL_GAME::gamesForMan[6];
std::list<int> C_SMALL_GAME::gamesForGirl[6];
std::list<int> C_SMALL_GAME::gamesForBoth[6];
GAME_INFO C_SMALL_GAME::gameInfos1[13] = 
{
	{1,1,CHIEF_EITHER},	{1,1,CHIEF_MAN},
	{1,1,CHIEF_EITHER},	{1,1,CHIEF_MAN},
	{1,1,CHIEF_MAN},	{1,1,CHIEF_MAN},
	{1,1,CHIEF_EITHER},	{1,1,CHIEF_EITHER},
	{1,1,CHIEF_MAN},	{1,0,CHIEF_MAN},
	{1,1,CHIEF_GIRL},	{1,1,CHIEF_GIRL},
	{0,1,CHIEF_GIRL}
};
GAME_INFO C_SMALL_GAME::gameInfos2[15] = 
{
	{1,1,CHIEF_MAN},	{1,1,CHIEF_MAN},
	{1,1,CHIEF_MAN},	{1,1,CHIEF_EITHER},
	{1,1,CHIEF_EITHER},	{1,1,CHIEF_EITHER},
	{1,1,CHIEF_EITHER},	{1,2,CHIEF_MAN},
	{1,2,CHIEF_MAN},	{1,2,CHIEF_MAN},
	{1,1,CHIEF_GIRL},	{1,1,CHIEF_GIRL},
	{1,1,CHIEF_GIRL},	{1,1,CHIEF_GIRL},
	{2,1,CHIEF_GIRL}
};
GAME_INFO C_SMALL_GAME::gameInfos3[16] = 
{
	{1,1,CHIEF_MAN},	{1,1,CHIEF_EITHER},
	{1,1,CHIEF_MAN},	{1,1,CHIEF_MAN},
	{1,1,CHIEF_MAN},	{1,1,CHIEF_MAN},
	{1,1,CHIEF_MAN},	{1,1,CHIEF_EITHER},
	{1,2,CHIEF_MAN},	{1,2,CHIEF_MAN},
	{1,1,CHIEF_GIRL},	{1,1,CHIEF_GIRL},
	{1,1,CHIEF_GIRL},	{1,1,CHIEF_GIRL},
	{2,1,CHIEF_GIRL},	{2,1,CHIEF_GIRL}
};
GAME_INFO C_SMALL_GAME::gameInfos4[17] = 
{
	{1,1,CHIEF_MAN},	{1,1,CHIEF_MAN},
	{1,1,CHIEF_MAN},	{1,1,CHIEF_MAN},
	{1,1,CHIEF_MAN},	{1,1,CHIEF_MAN},
	{1,1,CHIEF_MAN},	{1,1,CHIEF_MAN},
	{1,2,CHIEF_MAN},	{1,2,CHIEF_MAN},
	{1,1,CHIEF_GIRL},	{1,1,CHIEF_GIRL},
	{1,1,CHIEF_GIRL},	{1,1,CHIEF_GIRL},
	{1,1,CHIEF_GIRL},	{2,1,CHIEF_GIRL},
	{2,1,CHIEF_GIRL}
};
GAME_INFO C_SMALL_GAME::gameInfos5[9] = 
{
	{1,1,CHIEF_EITHER}, {1,1,CHIEF_EITHER},
	{1,0,CHIEF_MAN},	{5,5,CHIEF_EITHER},
	{5,0,CHIEF_MAN},	{1,2,CHIEF_MAN},
	{0,1,CHIEF_GIRL},	{0,5,CHIEF_GIRL},
	{2,1,CHIEF_GIRL},
};
GAME_INFO C_SMALL_GAME::gameInfos6[3] = 
{
	{1,1,CHIEF_EITHER},	{1,2,CHIEF_MAN},
	{2,1,CHIEF_GIRL}
};
C_SMALL_GAME::C_SMALL_GAME():
frameNumber(0),time(0),index1(0),index2(0),
textRow(1),sndVoice(0),text(0),resLoaded(false)
{
	soundt = GetRandNumber(12,18)*10;
}

C_SMALL_GAME::~C_SMALL_GAME()
{
	vgSoundStop(sndMusic[index1]);
	vgSoundStop(sndWarm[index2]);
	ReleaseWave(&sndVoice);
	ResetGameTip();
}

bool C_SMALL_GAME::Play()
{
	return false;
}

void GAME::C_SMALL_GAME::DisplayText()
{
	DisplayTextDown();
	DisplayTextUp();
}

void C_SMALL_GAME::DisplayTextDown()
{
	vgDrawSurface(text,256,162,0);
}

void C_SMALL_GAME::DisplayTextUp()
{
	vgDrawSurface(text,256,606 - text->h,DRAWOPTION_FLIPH | DRAWOPTION_FLIPV);
}

void C_SMALL_GAME::InitGameList()
{
	if(!listInit)
	{
		int i;
		for(i=0;i<6;i++)
		{
			ResetGameList(i,0);
			ResetGameList(i,1);
			ResetGameList(i,2);
		}
		listInit = true;
	}
}

TYPE_INDEX C_SMALL_GAME::SelectGame(bool forman,bool gift)
{
	static size_t demoCount = 0;
	LIST_IT it;
	bool found,valid;
	GAME_INFO *pinfo;
	TYPE_INDEX result;
	size_t ngames,index;
	std::list<int> games;
	
	InitGameList();
	if(PDATA->InDemo && ++demoCount%2 == 0)
	{
		result.type = 6;
		result.index = 0;
		return result;
	}
	int &type = forman ? manType : girlType;
	std::list<int> (&gamesMG)[6] = forman ? gamesForMan : gamesForGirl;
		
	found = false;
	valid = false;
	while(!found)
	{
		switch(type)
		{
			case 0: 
				pinfo = gameInfos1;
				break;
			case 1:	
				pinfo = gameInfos2;
				break;
			case 2:	
				pinfo = gameInfos3;
				break;
			case 3:	
				pinfo = gameInfos4;
				break;
			case 4:	
				pinfo = gameInfos5;
				break;
			case 5:	
				pinfo = gameInfos6;
				break;
		}
		games = gamesMG[type] + gamesForBoth[type];
		if(games.empty())
		{
			++type;
			if(type > 5)
			{
				int i;
				type = 0;
				for(i=0;i<6;i++)
				{
					ResetGameList(i,forman);
					ResetGameList(i,2);
				}
			}
			continue;
		}
		it = games.begin();
		ngames = games.size();
		std::advance(it,rand()%ngames);
		index = *it;
		valid = PDATA->NumberOfMans()  >= pinfo[index].man &&
				PDATA->NumberOfGirls() >= pinfo[index].girl;
		if(gift)
		{
			valid = valid &&
					pinfo[index].man  > 0 && 
					pinfo[index].man  < 2 && 
					pinfo[index].girl > 0 &&
					pinfo[index].girl < 3;
		}
		if(valid)
		{
			result.type  = type;
			result.index = index;
			found = true;
		}
		gamesMG[type].remove(index);
		gamesForBoth[type].remove(index);
	}
	++type;
	if(type > 5)
	{
		type = 0;
	}
	return result;
}

void C_SMALL_GAME::ResetGameList(int type,int forwhat)
{
	int i;
	GAME_INFO *pinfo;
	int nrGame[6] = {13,15,16,17,9,3};
	int nr = nrGame[type];
	switch(type)
	{
		case 0:
			pinfo = gameInfos1;
			break;
		case 1:	
			pinfo = gameInfos2;
			break;
		case 2:	
			pinfo = gameInfos3;
			break;
		case 3:	
			pinfo = gameInfos4;
			break;
		case 4:	
			pinfo = gameInfos5;
			break;
		case 5:	
			pinfo = gameInfos6;
			break;
	}
	for(i=0;i<nr;i++)
	{
		switch(pinfo[i].chief)
		{
			case CHIEF_GIRL:
				if(forwhat == 0)
				{
					gamesForGirl[type].push_back(i);
				}
				break;
			case CHIEF_MAN:
				if(forwhat == 1)
				{
					gamesForMan[type].push_back(i);
				}
				break;
			case CHIEF_EITHER:
				if(forwhat == 2)
				{
					gamesForBoth[type].push_back(i);
				}
				break;
		}
	}
}

int C_SMALL_GAME::NumberOfManActors()
{
	return 1;
}

int C_SMALL_GAME::NumberOfGirlActors()
{
	return 1;
}


