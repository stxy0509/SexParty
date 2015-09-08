#ifndef PLAYER_H_
#define PLAYER_H_
#include <EagleSDK.h>
#include "bottle.h"
#include "../game_common.h"
class C_UP_TIMER;
namespace GAME
{
	class C_PLAYER
	{
	public:
		enum 
		{
			PLAYER_TYPES  = 2,
			BUSY_ACTIONS  = 6,
			RED_PLAYER    = 0,
			YELLOW_PLAYER = 1,
		};
		C_PLAYER(int pos);
		~C_PLAYER();
		void Play();
		void SitQuietly();
		void SetPlayerCome();
		void SetPlayerLeave();
		 int GetCurrentAction()
		{
			return CurrentAction;
		}
		void DisplayPlayerAction(int n);
		static void LoadResources();
		static void UnloadResources();
	private:
		static vgWAVE *sndStrike;
		static vgSURFACE *Actions[PLAYER_TYPES][DIRECTIONS];
		static int PlayerPos[MAX_PLAYERS][2];
		static int Loaded;
		C_UP_TIMER *timer;
		vgSURFACE *PlayAction;
		int  CurrentAction;
		int  time;
		int  position;
		bool exist;
		bool IsIdle;
		bool flip;
		C_PLAYER(const C_PLAYER&);
		C_PLAYER& operator=(const C_PLAYER&);
		void DecideToUseWhichPlayer(int pos);
	};
}
#endif
