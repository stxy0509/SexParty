#ifndef GAME_H_
#define GAME_H_
#include "game_common.h"
class C_CHARGE;
namespace GAME
{
	class C_SELECT_GAME;
	class C_SELECT_ROLE;
	class C_POKER;
	class C_DICE;
	class C_BASEBALL;
	class C_PRESENT;
	class C_USERFUNC;
	class C_ADMIN;
	class C_ROLE_TIP;
	class C_BONUS_GAME;
	class C_GAME
	{
	private:
		C_GAME();
		C_GAME(const C_GAME&);
		C_GAME& operator=(const C_GAME&);
		bool tuneVol;
		int loaded;
		int whojq;
		int joinquit;
		int tuneDelay;
		vgSURFACE *demoTip;
		vgSURFACE *warn;
		vgSURFACE *volbg;
		vgSURFACE *point;
		vgSURFACE *quit;
		vgSURFACE *quitmsg;
		vgSURFACE *quitmsg_left;
		
		vgSURFACE *userMsg[2];
		vgSURFACE *sysMsg[2];
		
		C_DICE  *PDICE;
		C_ADMIN *PADMIN;
		C_ROLE_TIP *PTIP;
		C_CHARGE   *PCHARGE;
		C_POKER    *PPOKER;
		C_BASEBALL *PBASEBALL;
		C_USERFUNC *PUSER;
		C_PRESENT  *PPRESENT;
		C_BONUS_GAME *PBONUS;
		C_SELECT_ROLE *PSELECT_ROLE;
		C_SELECT_GAME *PSELECT_GAME;
		void DisplayHelpMessage();
		void ResetPrevGame();
		bool CanQuit(int who);
		void DealMainGameStage();
		void DealSubGameStage();
		void EnterOrExitDemo();
		void DealScoreCoin();
		void DealSubGameRequest();
		void DealJoinQuit();
		void DisplayQuitMessage(int who,int sec);
	public:
		~C_GAME();
		void Run();
		void LoadPictures();
		void UnloadPictures();
		static C_GAME *GetSingleton();
	};
}
#endif
