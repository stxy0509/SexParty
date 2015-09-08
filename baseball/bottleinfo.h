#ifndef BOTTLE_INFO_H_
#define BOTTLE_INFO_H_
#include <EagleSDK.h>
#include "../game_common.h"
namespace GAME
{
	class C_BOTTLE_INFO
	{
	private:
		static vgSURFACE *Number[2];
		static vgSURFACE *Plus[2];
		static int PlusPos[MAX_PLAYERS][2];
		static int Loaded;
		static void LoadResources();
		static void UnloadResources();
		vgSURFACE *number;
		vgSURFACE *plus;
		int time;
		int x,y;
		int dx,dy;
		int position;
		int value;
		bool flip;
		C_BOTTLE_INFO(const C_BOTTLE_INFO&);
		C_BOTTLE_INFO& operator=(const C_BOTTLE_INFO&);
	public:
		C_BOTTLE_INFO(int n,int pos);
		~C_BOTTLE_INFO();
		void Display();
		bool IsTimeOver()
		{
			return time >= 40;
		}
	};
}
#endif
