#ifndef BOTTLE_H_
#define BOTTLE_H_
#include <EagleSDK.h>
#include "../game_common.h"
namespace GAME
{
	enum BOTTLE_NAME 
	{
		BOTTLE_BEER1 = 0,
		BOTTLE_BEER2,
		BOTTLE_SPIRIT,
		BOTTLE_VODKA,
		BOTTLE_MAOTAI,
		BOTTLE_XO1,
		BOTTLE_XO2,
		BOTTLE_MAGIC,
		BOTTLE_NR
	};
	enum 
	{
		DIRECTIONS = 2,
		DOWN  = 0,
		RIGHT = 1,
	};
	class C_BOTTLE
	{
	public:
		enum BOTTLE_STATE 
		{
			FLYING = 0,
			BREAKED,
			DEAD
		};
		enum 
		{
			FLY_ACTIONS   = 5,
			BREAK_ACTIONS = 4,
		};
		void Move();
		BOTTLE_STATE GetBottleState() const
		{
			return state;
		}
		void SetBottleState(BOTTLE_STATE s)
		{
			if(state != s)
			{
				state = s;
				time = 0;
			}
		}
		static void LoadResources();
		static void UnloadResources();
		C_BOTTLE(BOTTLE_NAME name,int to);
		~C_BOTTLE();
		int time;
		int value;
	private:
		static int Loaded;
		static vgSURFACE *Fly[BOTTLE_NR][DIRECTIONS][FLY_ACTIONS];
		static vgSURFACE *BottleBreak[BOTTLE_NR][DIRECTIONS][BREAK_ACTIONS];
		static const int StartPosition[BOTTLE_NR][DIRECTIONS][2];
		static const int EndPosition[BOTTLE_NR][MAX_PLAYERS][2];
		vgSURFACE *fly[FLY_ACTIONS];
		vgSURFACE *bottlebreak[BREAK_ACTIONS];
		int x0,y0;
		int x, y;
		int w, h;
		float dx,dy;
		bool  flip;
		BOTTLE_STATE state;
		C_BOTTLE(const C_BOTTLE&);
		C_BOTTLE& operator=(const C_BOTTLE&);	
		bool OutOfScreen();
		void DecideFlyBreakPictures(BOTTLE_NAME,int);
	};
}
#endif
