#ifndef BASEBALL_H_
#define BASEBALL_H_
#include <EagleSDK.h>
#include <vector>
#include <map>
#include "../game_common.h"
class C_UP_TIMER;
namespace GAME 
{
	class C_BOTTLE;
	class C_PLAYER;
	class C_BOTTLE_INFO;
	class C_EFFECT;
	class GT 
	{
	public:
		bool operator()(int a,int b)
		{
			return a > b;
		}
	};
	class C_BASEBALL
	{
	private:
		typedef std::multimap<int,int>::iterator MAP_IT;
		static int FlagPos[MAX_PLAYERS][2];
		static int Mark1Pos[MAX_PLAYERS][2];
		static int Mark2Pos[MAX_PLAYERS][2];
		static int Rank2Pos[MAX_PLAYERS][2];
		static int RankFramePos[MAX_PLAYERS][2];
		static int NamePos[MAX_PLAYERS][2];
		vgWAVE *sndBg;
		vgWAVE *sndBreak;
		vgWAVE *sndStart;
		vgWAVE *sndHit[2];
		vgSURFACE *bg[2];
		vgSURFACE *nr_left;
		vgSURFACE *nr;
		vgSURFACE *rank_left;
		vgSURFACE *rank;
		vgSURFACE *caption;
		int m_time;
		int Loaded;
		int Stage;
		int RankOrder[MAX_PLAYERS];
		bool sndPlayed;
		bool handleHit;
		C_UP_TIMER *PlayTime;
		C_BOTTLE *bottles[MAX_PLAYERS];
		C_EFFECT *effects[MAX_PLAYERS];
		C_PLAYER *ballplayers[MAX_PLAYERS];
		C_BOTTLE_INFO *infos[MAX_PLAYERS];
		C_BASEBALL();
		C_BASEBALL(const C_BASEBALL&);
		C_BASEBALL& operator=(const C_BASEBALL&);
		void LoadResources();
		void SelectActors();
		void DisplayPlayerMark();
		void UnloadResources();
		void DisplayRestTime();
		void DisplayRankOrder();
		void ComputeRankOrder();
		void DisplayActors();
		void DisplayName();
		void ShowActor(int n,int f);
		bool HitBottle(int index);
		int  PointToScore(int p);
	public:
		~C_BASEBALL();
		void Play();
		void Reset();
		bool IsCurrentPlayer(int who);
		static C_BASEBALL *GetSingleton();
	};
}
#endif
