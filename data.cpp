#include "data.h"
#include "game_common.h"
#include "goldcoin.h"
#include "../../soft/EagleClass/error.h"
using GAME::freeGirl;
using GAME::freeMan;
using GAME::C_GOLD_COIN;
using GAME::AddPickedRole;
using GAME::MAX_PLAYERS;
using GAME::playerScoreDigit;
using GAME::playerIncrementDigit;
U8 *C_DATA::pOld = 0;
C_SAFE_WRITE_FLASH *C_DATA::flash = 0;
void C_DATA::Read()
{
	int ErrorCode;
	ErrorCode = flash->read();
	if(ErrorCode != C_SAFE_WRITE_FLASH::NO_ERROR)
	{
		ReportError("Read data error! Error Code is %d\n",ErrorCode);
	}
}

void C_DATA::Write()
{
	if(memcmp(pOld,this,sizeof(C_DATA)))
	{
		int ErrorCode;
		ErrorCode = flash->write();
		if(ErrorCode != C_SAFE_WRITE_FLASH::NO_ERROR)
		{
			ReportError("Write data error! Error Code is %d\n",ErrorCode);
		}
		memcpy(pOld,this,sizeof(C_DATA));
	}
}

C_DATA::C_DATA()
{
	flash = new C_SAFE_WRITE_FLASH;
	flash->add(this,sizeof(C_DATA),0);
	Read();
	if(pOld == 0)
	{
		pOld = new U8[sizeof(C_DATA)];
	}
	memcpy(pOld,this,sizeof(C_DATA));
	for(int i=0;i<6;i++)
	{
		freeMan.push_back(i);
		freeGirl.push_back(i);
	}
	
	if(InDemo)
	{
		ClearAllPlayer();
	}
	else
	{
		int i;
		if(MainGameStage == GAME::STAGE_END_GAME)
		{
			MainGameStage = GAME::STAGE_SELECT_GAME;
		}
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(SeatHaveRole(i))
			{
				AddPickedRole(Players[i].Index,i%2 == 0);
				if(Players[i].Increment > 0)
				{
					Players[i].Score += Players[i].Increment;
					Players[i].Increment = 0;
				}
			}
		}
	}
}

C_DATA::~C_DATA()
{
	delete flash;
}

C_DATA * C_DATA::GetSingleton()
{
	static C_DATA *PDATA = 0;
	return PDATA == 0 ? PDATA = new C_DATA() : PDATA;
}

void C_DATA::ClearAllPlayer()
{
	int i;
	for(i=0;i<MAX_PLAYERS;i++)
	{
		ClearPlayer(i);
	}
}

void C_DATA::ClearPlayer(int index)
{
	if(index >= 0 && index < MAX_PLAYERS)
	{
		Players[index].Index = -1;
		Players[index].Score =  0;
		Players[index].Increment = 0;
		Players[index].Deleting = false;
		GAME::playerScoreDigit[index].setValue(0);
		GAME::playerIncrementDigit[index].setValue(0);
	}
}

int C_DATA::NumberOfPlayers()
{
	int i,n = 0;
	for(i=0;i<MAX_PLAYERS;i++)
	{
		if(SeatHaveRole(i))
		{
			++n;
		}
	}
	return n;
}

int C_DATA::NumberOfGirls()
{
	int i,n = 0;
	for(i=1;i<MAX_PLAYERS;i += 2)
	{
		if(SeatHaveRole(i))
		{
			++n;
		}
	}
	return n;
}

int C_DATA::NumberOfMans()
{
	int i,n = 0;
	for(i=0;i<MAX_PLAYERS;i += 2)
	{
		if(SeatHaveRole(i))
		{
			++n;
		}
	}
	return n;
}

int C_DATA::NextPlayer(int n)
{
	for(int i=(n < 0 ? 0 : n + 1);i<MAX_PLAYERS;i++)
	{
		if(SeatHaveRole(i))
		{
			return i;
		}
	}
	return MAX_PLAYERS;
}

int C_DATA::NextSeat(int n)
{
	for(int i=(n < 0 ? 0 : n + 1);i<MAX_PLAYERS;i++)
	{
		if(!SeatHaveRole(i))
		{
			return i;
		}
	}
	return MAX_PLAYERS;
}

int C_DATA::NextMan(int n)
{
	for(int i=(n < 0 ? 0 : n + 2);i<MAX_PLAYERS;i += 2)
	{
		if(SeatHaveRole(i))
		{
			return i;
		}
	}
	return MAX_PLAYERS;
}

int C_DATA::NextGirl(int n)
{
	for(int i=(n < 0 ? 1 : n + 2);i<MAX_PLAYERS;i += 2)
	{
		if(SeatHaveRole(i))
		{
			return i;
		}
	}
	return MAX_PLAYERS;
}

int C_DATA::ChiefMan()
{
	if(SeatHaveRole(2))
	{
		return 2;
	}
	if(SeatHaveRole(4))
	{
		return 4;
	}
	return NextMan(-1);
}

bool C_DATA::SeatHaveRole(int n)
{
	int index = Players[n].Index;
	return index >= 0 && index < 6;
}

void C_DATA::DeleteRole(int who)
{
	if(!SeatHaveRole(who)) 
	{
		return;
	}
	GAME::SubPickedRole(Players[who].Index,who%2 == 0);
	ClearPlayer(who);
}

bool C_DATA::MustDecode1()
{
	return !ForSale && TotalCharge >= LockCharge;
}

bool C_DATA::MustDecode2()
{
	return !ForSale && TotalCharge >= 2*LockCharge;
}

bool C_DATA::CanAct(int w)
{
	return SeatHaveRole(w) && !Players[w].Deleting;
}
