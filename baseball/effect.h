#ifndef EFFECT_H_
#define EFFECT_H_
#include <EagleSDK.h>
#include "../game_common.h"
namespace GAME
{
	class C_EFFECT
	{
	private:
		static vgSURFACE *Effect[2];
		static int Pos[MAX_PLAYERS][2];
		static int Loaded;
		static void LoadResources();
		static void UnloadResources();
		vgSURFACE *effect;
		int time;
		int position;
		bool flip;
		C_EFFECT(const C_EFFECT&);
		C_EFFECT& operator=(const C_EFFECT&);
	public:
		C_EFFECT(int pos);
		~C_EFFECT();
		void Display();
		bool IsTimeOver()
		{
			return time >= 10;
		}
	};
}
#endif
