#include "goldcoin.h"
#include "game_common.h"
#include <libst.h>
using GAME::C_GOLD_COIN;
bool C_GOLD_COIN::Loaded = false;
vgSURFACE *C_GOLD_COIN::flipCoin = 0;
int C_GOLD_COIN::StartPos[2] = {128,76};
int C_GOLD_COIN::EndPos[MAX_PLAYERS][2] = 
{
	{ 38,263},{ 37,451},{179,692},{367,693},{623,692},
	{811,692},{949,458},{949,270},{592, 36},{404, 36},
};
int C_GOLD_COIN::CoinNumber[MAX_PLAYERS] = 
{
	2,3,4,5,6,7,6,5,4,3
};
C_GOLD_COIN::C_GOLD_COIN(int pos)
{
	LoadPictures();
	nArrive = 0;
	amount = CoinNumber[pos];
	x0 = StartPos[0];
	y0 = StartPos[1];
	xv = 1.0*(EndPos[pos][0] - x0)/(CHANGE_TIME*amount);
	yv = 1.0*(EndPos[pos][1] - y0)/(CHANGE_TIME*amount);
	for(int i=0;i<amount;i++)
	{
		dt[i] = -CHANGE_TIME*i;
	}
}

C_GOLD_COIN::C_GOLD_COIN(int from,int to)
{
	LoadPictures();
	nArrive = 0;
	amount = 7;
	x0 = EndPos[from][0];
	y0 = EndPos[from][1];
	xv = 1.0*(EndPos[to][0] - x0)/(CHANGE_TIME*amount);
	yv = 1.0*(EndPos[to][1] - y0)/(CHANGE_TIME*amount);
	for(int i=0;i<amount;i++)
	{
		dt[i] = -CHANGE_TIME*i;
	}
}

C_GOLD_COIN::~C_GOLD_COIN()
{
	
}

void C_GOLD_COIN::LoadPictures()
{
	if(!Loaded)
	{
		flipCoin = LoadImageFile("picture/flipcoin.bmp",0,255,0);
		Loaded = true;
	}
}

void C_GOLD_COIN::UnloadPictures()
{
	if(Loaded)
	{
		ReleaseSurface(&flipCoin);
		Loaded = false;
	}
}

void C_GOLD_COIN::Display()
{
	int i;
	for(i=nArrive;i<amount;i++)
	{
		if(dt[i] >= 0)
		{
			DrawPartOfSurface(flipCoin,3,3,dt[i]/CHANGE_SPEED%7,
				static_cast<int>(x0 + xv*dt[i]),
				static_cast<int>(y0 + yv*dt[i]));
		}
		++dt[i];
		if(dt[i] == amount * CHANGE_TIME)
		{
			dt[i] = 0;
			++nArrive;
		}
	}
}
