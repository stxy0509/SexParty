#ifndef USER_H_
#define USER_H_
#include <EagleSDK.h>
namespace GAME 
{
	class C_POKER;
	class C_DICE;
	class C_BASEBALL;
	class C_PRESENT;
	class C_USERFUNC
	{
	private:
		struct NameStage 
		{
			int namei;
			int stage;
			NameStage():namei(0),stage(0){}
			NameStage(int ni,int s):namei(ni),stage(s){}
		};
		int Loaded;
		int time;
		bool sndPlayed;
		vgWAVE *sndQuit;
		vgWAVE *sndWait;
		vgSURFACE *caption;
		C_POKER    *PPOKER;
		C_DICE     *PDICE;
		C_BASEBALL *PBASEBALL;
		C_PRESENT  *PPRESENT;
		NameStage  nameStage[5];
		void LoadResources();
		void UnloadResources();
		void DisplayFreeGameName();
		void FillNameStage();
		void SwitchGame(int newGame);
		C_USERFUNC();
		C_USERFUNC(const C_USERFUNC &);
		C_USERFUNC& operator=(const C_USERFUNC &);
		static int IconPos[5][2];
	public:
		~C_USERFUNC();
		void Process();
		static C_USERFUNC *GetSingleton();
	};
}
#endif
