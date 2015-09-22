#ifndef DATA_H_
#define DATA_H_
#include "../../soft/EagleClass/safe_rwflash.h"
#include "consumetime.h"
struct Player
{
	int Index;
	int Increment;
	int Score;
	int Deleting;
	Player():
	Index(-1),Increment(0),Score(0),Deleting(false)
	{
	}
};
struct C_DATA
{
private:
	static U8 *pOld;
	static C_SAFE_WRITE_FLASH *flash;
	C_DATA();
	C_DATA(const C_DATA&);
	C_DATA& operator=(const C_DATA&);
	void Read();
public:	
	int SelectRoleTime;
	int BaseBallTime;
	int FixPlayTime;
	int ExtraPlayTime;
	int DecideTime;
	int RemainedSeconds;
	int MainGameStage;
	int InDemo;
	int RunTime;
	int ForSale;
	int MachineNumber;
	int LineNumber;
	int LockCount;
	int Vol;
	int LockCharge;
	int TotalCharge;
	int CurrentTotal;
	int TotalIndex;
	int CurrentIndex;
	int CurrentCharge[30];
	int HistoryCharge[60];
	Player Players[10];
	C_CONSUME_TIME GameTime;
	int Year;
	int Month;
	int Day;
	U32 MinResources;
	~C_DATA();
	int NumberOfMans();
	int NumberOfGirls();
	int NumberOfPlayers();
	int NextPlayer(int n);
	int NextSeat(int n);
	int NextMan(int n);
	int NextGirl(int n);
	int ChiefMan();
	void Write();
	bool CanAct(int who);
	bool MustDecode1();
	bool MustDecode2();
	void ClearAllPlayer();
	void ClearPlayer(int index);
	bool SeatHaveRole(int n);
	void DeleteRole(int who);
	static C_DATA *GetSingleton();
};
#endif
