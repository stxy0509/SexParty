#ifndef GAME_POKER_H
#define GAME_POKER_H
#include <EagleSDK.h>
#include <set>
#include <vector>
#include "game_common.h"
#include "small_game/smallgame.h"
namespace GAME
{
	class C_CARD;
	class C_POKER
	{
	private:
		int m_time;
		int m_stage;
		C_SMALL_GAME *psmall;
		C_CARD *pcard[MAX_PLAYERS];
		bool sndbgPlayed;
		static vgSURFACE *bg[2];
		static vgSURFACE *girl[3];
		static vgSURFACE *action;
		static vgSURFACE *caption;
		static vgWAVE *sndBg;
		static vgWAVE *sndStart;
		static vgWAVE *sndWash;
		static vgWAVE *sndPut;
		static std::set<int> freeCards;
		void PlayCard();
		static void ResetFreeCards();
		static int SelectOneCard();
		C_POKER();
	public:
		~C_POKER();
		void Play();
		void Reset();
		void SelectActors();
		bool IsCurrentPlayer(int w);
		static void LoadResources();
		static void UnloadResources();
		static C_POKER *GetSingleton();
	};
}
#endif
