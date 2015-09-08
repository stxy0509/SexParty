#ifndef SELECT_ROLE_H_
#define SELECT_ROLE_H_
#include <EagleSDK.h>
class C_DOWN_TIMER;
namespace GAME
{
	class C_SELECT_ROLE 
	{
	private:
		enum 
		{
			MAX_ROLES = 6,
			MAX_ROLE_INDEX = 5,
			CHANGE_SPEED = 100,
		};
		vgWAVE *sndSelect;
		vgWAVE *sndBackground;
		vgSURFACE *bg[2];
		vgSURFACE *allman;
		vgSURFACE *allgirl;
		bool sndPlayed;
		int ResourceLoaded;
		int FrameNumber;
		int Target;
		C_DOWN_TIMER *Timer;
		static int StandManPos[MAX_ROLES][2];
		static int StandGirlPos[MAX_ROLES][2];
		static int RoleNamePos[MAX_ROLES][2];
		C_SELECT_ROLE();
		C_SELECT_ROLE(const C_SELECT_ROLE &);
		C_SELECT_ROLE &operator=(const C_SELECT_ROLE &);
		void LoadResources();
		void UnloadResources();
		void DealSelectNormal();
		void ShowSelectedMan();
		void ShowSelectedGirl();
		void DisplaySelectRoleName(bool isMan,int index);
		void DisplaySelectTime(bool isMan);
	public:
		~C_SELECT_ROLE();
		void Reset();
		void ShowSelectedRole();
		static C_SELECT_ROLE *GetSingleton();
	};
}
#endif
