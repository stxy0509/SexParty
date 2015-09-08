#ifndef PARA_H_
#define PARA_H_
#include "game_common.h"
namespace GAME 
{
	class C_PARA
	{
	private:
		enum 
		{
			LINE_NUMBER     = 0,
			MACHINE_NUMBER     ,
			HARDWARE_TEST      ,
			FIX_PLAY           ,
			EXTRA_PLAY         ,
			SELECT_ROLE_TIME   ,
			BASEBALL_PLAY_TIME ,
			LOAD_DEFAULTS      ,
			SAVE_AND_EXIT      ,
			DISCARD_AND_EXIT   ,
		};
		vgSURFACE *bg[2];
		vgSURFACE *nr;
		vgSURFACE *box;
		vgSURFACE *select;
		vgSURFACE *arrow;
		vgSURFACE *test;
		vgSURFACE *set;
		int Loaded;
		int para[8];
		int CurrentItem;
		void DoSave();
		void GetOldValue();
		void LoadResources();
		void UnloadResources();
		bool DealKeyPress();
		void DisplayArrow();
		void DisplayParaValue();
		C_PARA(const C_PARA&);
		C_PARA& operator=(const C_PARA&);
		static int ArrowPos[10][2];
		static int NumberPos[8][2];
		static int KeyPos[16][2];
	public:
		C_PARA();
		~C_PARA();
		bool Process();
	};
}
#endif
