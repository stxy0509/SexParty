#ifndef BONUS_GAME_H
#define BONUS_GAME_H
#include <EagleSDK.h>
namespace GAME
{
	class C_BONUS_GAME
	{
	public:
		C_BONUS_GAME();
		~C_BONUS_GAME();
		bool Play();
		static void LoadResources();
		static void UnloadResources();
	private:
		int time;
		int stage;
		int basex;
		int basey;
		int award;
		int rollSpeed;
		int awardx;
		int currentx;
		int sorted;
		int n8,n4;
		int XToAward(int x);
		void DisplayAward(int sx);
		void DisplayRank();
		static vgSURFACE *bg;
		static vgSURFACE *text;
		static vgSURFACE *name;
		static vgSURFACE *happy;
		static vgSURFACE *sad;
		static vgSURFACE *flashFrame;
		static vgSURFACE *rollFrame;
		static vgSURFACE *awardFrame;
		static vgSURFACE *picaward;
		static vgWAVE *sndName;
		static vgWAVE *sndWine;
		static vgWAVE *sndBadLuck;
		static vgWAVE *sndGoodLuck;
		static int Minutes[5];
	};
}
#endif
