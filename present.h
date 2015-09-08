#ifndef PRESENT_H_
#define PRESENT_H_
#include <EagleSDK.h>
#include <vector>
class C_DOWN_TIMER;
class C_UP_TIMER;
namespace GAME 
{
	class C_PRESENT 
	{
	private:
		typedef std::vector<int>::iterator VIT;
		vgWAVE *sndBg;
		vgWAVE *sndStart;
		vgWAVE *sndGrab;
		vgWAVE *sndGrabBg;
		vgWAVE *sndSelect;
		vgWAVE *sndRoll;
		vgWAVE *sndNext;
		vgWAVE *sndCelebrate;
		vgWAVE *sndEncourage;
		
		vgSURFACE *bg[2];
		vgSURFACE *lattice[2];
		vgSURFACE *blue;
		vgSURFACE *gift;
		vgSURFACE *menu;
		vgSURFACE *coin;
		vgSURFACE *coin_left;
		vgSURFACE *nr1;
		vgSURFACE *picsend;
		vgSURFACE *value;
		vgSURFACE *caption1;
		vgSURFACE *caption2;
		
		vgSURFACE *sendD;
		vgSURFACE *recvD;
		vgSURFACE *sendL;
		vgSURFACE *recvL;
		
		int time;
		int stage;
		int lines;
		int points;
		int send;
		int recv;
		int picLoaded;
		int sndLoaded;
		int haveResult;
		int fastest;
		int sendScore;
		int demoCount;
		int award[9];
		int rate[8];
		int lineNum[8];
		int rollCol[3];
		int stopCol[3];
		int stopTime[3];
		int pressScore[5];
		int noPressCount[5];
		C_DOWN_TIMER *pTimer;
		C_DOWN_TIMER *fightTimer;
		std::vector<int> lineAward[8];
		std::vector<int> allAwards;
		bool waitFinish;
		bool drawFinish;
		bool gameFinish;
		bool sndbgPlayed;
		static int CoinPos[5][2];
		static int GiftPos[9][2];
		static int GiftRate[5];
		static int AwardFramePos[9][2];
		void Play();
		bool DrawAward();
		void RateOfAward();
		void LoadSounds();
		void UnloadSounds();
		void LoadPictures();
		void UnloadPictures();
		bool WaitToSelect();
		void DisplayAwardFrame(int n);
		void RequestAwardResult();
		bool DisplayAllAwardFrame();
		int  PointToScore(int p);
		void DisplayAward();
		void DisplayCoinScore(int n);
		void DealKeyPress();
		void DisplayAllCoin();
		void DisplaySendInfo();
		void SelectActors();
		
		void FlagWhoSend(int n);
		void FlagSendObject(int n);
		C_PRESENT();
		C_PRESENT(const C_PRESENT&);
		C_PRESENT& operator=(const C_PRESENT&);
	public:
		~C_PRESENT();
		void Reset();
		void Process();
		bool IsCurrentPlayer(int who);
		static C_PRESENT *GetSingleton();
	};
}
#endif
