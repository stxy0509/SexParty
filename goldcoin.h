#ifndef GOLD_COIN_H
#define GOLD_COIN_H
#include <EagleSDK.h>
#include "game_common.h"
namespace GAME
{
	class C_GOLD_COIN
	{
	private:
		 enum 
		 {
			CHANGE_SPEED = 5,
			CHANGE_TIME = 10
		};
		  int nArrive;
		  int x0;
		  int y0;
		  int amount;
		  int dt[7];
		float xv;
		float yv;
		static bool Loaded;
		static int CoinNumber[MAX_PLAYERS];
		static int StartPos[2];
		static int EndPos[MAX_PLAYERS][2];
		static vgSURFACE *flipCoin;
		static void LoadPictures();
		static void UnloadPictures();
		C_GOLD_COIN(const C_GOLD_COIN&);
		C_GOLD_COIN& operator=(const C_GOLD_COIN&);
	public:
		C_GOLD_COIN(int pos);
		C_GOLD_COIN(int from,int to);
		~C_GOLD_COIN();
		void Display();
		bool IsFlyingFinish()
		{
			return nArrive == amount;
		}
	};
}
#endif
