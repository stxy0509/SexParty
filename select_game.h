#ifndef SELECT_GAME_H_
#define SELECT_GAME_H_
#include <EagleSDK.h>
#include <set>
namespace GAME
{
	class C_SELECT_GAME 
	{
	public:
		enum GAME_NAME
		{
			POKER = 0,
			DICE_PK,
			BASEBALL,
			BIG_PRESENT,
			GAME_NONE,
		};
		
		~C_SELECT_GAME();
		void Reset();
		void ShowSelectedGame();
		static C_SELECT_GAME *GetSingleton();
	private:
		static int FramePos[4][2];
		static int TextPos[4][2];
		vgWAVE *sndSelect;
		vgWAVE *sndToCharge;
		vgSURFACE *bg[2];
		vgSURFACE *text;
		vgSURFACE *frame;
		vgSURFACE *frame2;
		vgSURFACE *option1;
		vgSURFACE *option2;
		vgSURFACE *helpText;
		bool sndPlayed;
		int stage;
		int SelectedGame;
		int Loaded;
		int FrameNumber;
		int demoGame;
		int chargeOrQuit;
		C_SELECT_GAME();
		C_SELECT_GAME(const C_SELECT_GAME &);
		C_SELECT_GAME &operator=(const C_SELECT_GAME &);
		void LoadResources();
		void UnloadResources();
		void SelectGame();
	};
}
#endif
