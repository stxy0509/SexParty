#ifndef START_INFO_H_
#define START_INFO_H_
#include <EagleSDK.h>
#include "data.h"
class C_START_INFO 
{
private:
	static int time;
	static vgWAVE *sndGameOver;
	static vgWAVE *sndWelcome;
	static vgSURFACE *bar;
	static vgSURFACE *rankNr;
	static vgSURFACE *tip;
	static vgSURFACE *note;
	static vgSURFACE *frame;
	static vgSURFACE *frame_opt;
	static vgSURFACE *fire;
	static vgSURFACE *title;
	static vgSURFACE *list;
	static vgSURFACE *cup[3];
	static vgSURFACE *logo[2];
	static bool sorted;
	static bool Loaded;
	static U32 Max;
	static int stage;
	static int gameStage;
public:
	static vgSURFACE *bg[2];
	static void DirectEnd()		
	{
		stage = 1;
	}
	static void	ConfirmEnd(int s)	
	{
		stage = 0;
		gameStage = s;
	}
	static void DisplayRank();
	static void DisplayFire();
	static void Start();
	static void End();
	static void Init();
	static void Exit();
	static void Reset();
};
#endif
