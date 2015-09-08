#ifndef ADMIN_H_
#define ADMIN_H_
#include <EagleSDK.h>
class C_LOCK;
namespace GAME 
{
	class C_PARA;
	class C_QUERY_CHARGE;
	class C_ADMIN
	{
	private:
		static int fingerPos[4][2];
		enum ADMIN_ITEM 
		{
			TO_QUERY_CHARGE = 0,
			TO_SET_PARA,
			TO_LOCK,
			TO_EXIT
		};
		vgSURFACE *nr;
		vgSURFACE *menu;
		vgSURFACE *finger;
		vgSURFACE *message[2];
		C_PARA *PPARA;
		C_LOCK *PLOCK;
		C_QUERY_CHARGE *PQCHARGE;
		int item;
		bool loaded;
		bool inLock;
		bool inPara;
		bool inQueryCharge;
		C_ADMIN();
		C_ADMIN(const C_ADMIN&);
		C_ADMIN& operator=(const C_ADMIN&);
		void LoadResources();
		void UnloadResources();
	public:
		~C_ADMIN();
		bool Process();
		static C_ADMIN *GetSingleton();
	};
}
#endif
