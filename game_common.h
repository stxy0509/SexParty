#ifndef GAME_COMMON_H_
#define GAME_COMMON_H_
#include <EagleSDK.h>
#include <map>
#include <list>
#include <set>
#include "data.h"
#include "key.h"
#include "number_digit.h"
#include "consumetime.h"
class C_CHARGE;
class C_DOWN_TIMER;
class C_CONSUME_TIME;
struct SPLAYER 
{
	Player p;
	int num;
	bool isman;
	SPLAYER():num(0),isman(true)
	{}
};
bool Greater(const SPLAYER &a,const SPLAYER &b);
namespace GAME
{
	class C_DRINK;
	class C_SMALL_GAME;
	class C_GOLD_COIN;
	enum KEY_BIT
	{
		KEY_RIGHT    = 8,	//音量调节
		KEY_FUNCTION = 1,	//切换游戏
		KEY_ENTER0   = 7,
		KEY_ENTER1   = 4,
		KEY_ENTER2   = 5,
		KEY_ENTER3   = 10,
		KEY_ENTER4   = 11,
		KEY_ENTER5   = 3,
		KEY_ENTER6   = 2,
		KEY_ENTER7   = 15,
		KEY_ENTER8   = 14,
		KEY_ENTER9   = 13,
		KEY_LOCK     = 12,
		KEY_DOWN     = KEY_ENTER8,
		KEY_ENTER    = KEY_ENTER9,
		DOWN_LED     = 8,
		ENTER_LED    = 9,
	};
	enum
	{
		MAX_PLAYERS = 10,
		WARN_SECONDS = 600,
		JOIN_QUIT_DELAY = 5000,
	};
	enum GAME_STAGE
	{
		STAGE_SELECT_ROLE = 0,
		STAGE_SELECT_GAME,
		STAGE_POKER,
		STAGE_DICE_PK,
		STAGE_BASEBALL,
		STAGE_PRESENT_MANY,
		STAGE_END_GAME,
		STAGE_USER_FUNCTION,
		STAGE_SYSTEM,
		STAGE_CHARGE,
		STAGE_BONUS_GAME,
		STAGE_NONE
	};
	typedef std::multimap<int,int>::iterator MAP_IT;
	typedef std::set<int>::iterator SET_IT;
	typedef std::list<int>::iterator LIST_IT;

	extern C_KEY *PKEY;
	extern C_DATA *PDATA;
	extern C_DRINK *pdrink;
	extern C_DOWN_TIMER *minPlay;
	extern C_DOWN_TIMER *extraPlay;
	extern C_GOLD_COIN  *PCOIN[MAX_PLAYERS];
	extern C_SMALL_GAME *PSMALLGAME;	
	extern C_CONSUME_TIME *PGAMETIME;
	extern C_CONSUME_TIME *PDEMOTIME;
	extern int screenw;
	extern int screenh;
	extern int playStage;
	extern int SubGameStage;
	extern int PrevSubStage;
	extern int SitManPos[5][2];
	extern int SitGirlPos[5][2];
	extern int SitRolePos[MAX_PLAYERS][2];
	extern int ScorePos[MAX_PLAYERS][2];
	extern int PlayersEnter[MAX_PLAYERS];
	extern int ArrowPos[MAX_PLAYERS][2];
	extern int ChairPos[MAX_PLAYERS][2];
	extern int JoinMsgPos[MAX_PLAYERS][2];
	extern int ActNoticePos[MAX_PLAYERS][2];
	extern bool needBonus;
	extern bool ledOn[12];
	extern C_NUM_DIGIT playerScoreDigit[MAX_PLAYERS];
	extern C_NUM_DIGIT playerIncrementDigit[MAX_PLAYERS];
	extern vgWAVE *sndPress;
	extern vgWAVE *sndCharge;
	extern vgWAVE *sndAdmin;
	extern vgWAVE *sndUser; 
	extern vgWAVE *sndBonus; 
	extern vgWAVE *sndChange; 
	extern vgWAVE *sndAddTime; 
	extern vgWAVE *sndBegin; 
	extern vgWAVE *sndPokerDice; 
	extern vgSURFACE *border[2];
	extern vgSURFACE *window1;
	extern vgSURFACE *window2;
	extern vgSURFACE *man_left;
	extern vgSURFACE *man_down;
	extern vgSURFACE *girl_left;
	extern vgSURFACE *girl_down;
	extern vgSURFACE *arrow_down;
	extern vgSURFACE *arrow_left;
	extern vgSURFACE *chair_down;
	extern vgSURFACE *chair_right;
	extern vgSURFACE *time_left;
	extern vgSURFACE *nr_left;
	extern vgSURFACE *nr;
	extern vgSURFACE *lednr;
	extern vgSURFACE *fchair;
	extern vgSURFACE *fchair_left;
	extern vgSURFACE *gameIcon[5];
	extern vgSURFACE *gameName[5];
	extern vgSURFACE *iconFrame;
	extern vgSURFACE *counter;
	extern vgSURFACE *counterF;
	extern vgSURFACE *timebar;
	extern vgSURFACE *watch;
	extern vgSURFACE *watch_s;
	extern vgSURFACE *lednr_s;
	extern vgSURFACE *lednr_s_left;
	extern vgSURFACE *watch_s_left;
	extern vgSURFACE *tip[2];
	extern vgSURFACE *girlname[6][2];
	extern vgSURFACE *manname[6][2];
	extern vgSURFACE *man;
	extern vgSURFACE *girl;
	extern vgSURFACE *scoref;
	extern vgSURFACE *scorefLeft;
	extern vgSURFACE *firstFlag;
	extern vgSURFACE *firstFlagLeft;
	extern vgSURFACE *upArrow;
	extern vgSURFACE *join;
	extern vgSURFACE *join_left;
	extern vgSURFACE *gamenr;
	extern vgSURFACE *tipNext;
	extern vgSURFACE *pressAnimation[2];
	extern vgSURFACE *pressAnimationL[2];
	extern vgSURFACE *actNotice;
	extern vgSURFACE *actNoticeL;
	extern SPLAYER pls[MAX_PLAYERS];
	extern std::set<int> tmpActors;
	extern std::set<int> actor;
	extern std::set<int> keys;
	extern std::set<int> tmpKeys;
	extern std::multimap<int,int> manRank;
	extern std::multimap<int,int> girlRank;
	extern std::multimap<int,int> gmRank;
	extern std::list<int> freeMan;
	extern std::list<int> pickedMan;
	extern std::list<int> freeGirl;
	extern std::list<int> pickedGirl;
	extern  int GetFreeMan();
	extern  int GetFreeGirl();
	extern  int WhoJoinGame();
	extern  int WhoQuitGame();
	extern  int EnterOfPlayer(int n);
	extern  int GetFirstFreeRole(bool isMan);
	extern  int GetNextFreeRole(int index,bool isMan);
	extern void DisplayActNotice();
	extern void ResetGameTip();
	extern void ResetRole();
	extern void FlagActors();
	extern void DisplayBorder();
	extern void LoadPictureSound();
	extern void LoadResForAdmin();
	extern void ShowSitRole();
	extern void DisplayGameTime(int x,int y);
	extern void DisplayWatch(int n,int x,int y,bool flip = false);
	extern void DisplaySmallWatch(int n,int x,int y,bool flip = false);
	extern void DisplaySmallWatchLeft(int n,int x,int y,bool flip = false);
	extern void DisplayPlayerScore();
	extern void ArrowFocusOn(int n);
	extern void ChairFocusOn(int n);
	extern void ShowRemainedTime();
	extern void ControlLed();
	extern void AllLedOn();
	extern void TurnLedOn(int n);
	extern void TurnLedOff(int n);
	extern void ToggleLed(int n);
	extern void SetUsableKeys();
	extern bool DisplayTip();
	extern bool IsScoreUpdateFinish();
	extern bool IsCoinFlyingFinish();
	extern bool AnyOnePressEnter();
	extern bool AnyActorPressEnter();
	extern bool IsActor(int who);
	extern bool IsTmpActor(int who);
	extern bool IsInKeys(int k);
	extern bool IsTmpKey(int k);
	extern void InitNumberDigit();
	extern void LuckyStrikeOn(int n,int f);
	extern void DealScore(int v,int idx);
	extern void DealIncrement(int v,int idx);
	extern void AddPickedRole(int index,bool isMan);
	extern void SubPickedRole(int index,bool isMan);
	extern void DisplayRoleName(bool isMan,int index,int dir,int x,int y);
	extern void DisplaySitRoleName();
	extern void DisplayFirstFlag();
	extern void DisplayJoinMessage();
	extern void DisplayPressAnimation(int w);
	extern void DisplayTipNext();
	extern C_SMALL_GAME *CreateSmallGame(bool forman,bool gift = false);
}
#endif
