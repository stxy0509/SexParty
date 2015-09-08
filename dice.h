#ifndef DICE_H_
#define DICE_H_
#include <map>
#include "game_common.h"
class C_DOWN_TIMER;
class C_UP_TIMER;
namespace GAME 
{
	class C_DICE_HANDLE;
	class C_DICE 
	{
	private:
		enum
		{
			DRINK_HALF = 0,
			DRINK_WHOLE,
			DRINK_NONE,
		};
		vgWAVE *sndBg;
		vgWAVE *sndStart;
		vgSURFACE *bg[2];
		vgSURFACE *cup[2];
		vgSURFACE *cup_left[2];
		vgSURFACE *caption;
		int Loaded;
		int Stage;
		int DrinkType;
		int DrinkTimes;
		int m_time;
		bool sndbgPlayed;
		C_DICE_HANDLE *PHDICE[MAX_PLAYERS];
		C_DICE();
		C_DICE(const C_DICE&);
		C_DICE& operator=(const C_DICE&);
		void LoadResources();
		void UnloadResources();
		void WorkAfterAct();
		void PlayDice();
		void SelectActors();
		void SelectDrinkers();
		void DisplayDrinker(int n);
		void FlagDrinkers();
		static int CupPos[MAX_PLAYERS][2];
	public:
		~C_DICE();
		void Play();
		void Reset();
		bool IsCurrentPlayer(int who);
		static C_DICE *GetSingleton();
	};
}
#endif

