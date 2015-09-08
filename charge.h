#ifndef CHARGE_H_
#define CHARGE_H_
#include <EagleSDK.h>
#include <set>
class C_CHARGE 
{
	public:
		bool charge();
		static C_CHARGE *GetSingleton();
		static void LoadResources();
		static void UnloadResources();
	private:
		C_CHARGE();
		~C_CHARGE();
		int amount;
		std::set<int> compKeys;
		static vgSURFACE *bg;
		static vgSURFACE *cnr;
		static vgSURFACE *warn;
		static vgSURFACE *message[3];
};
#endif
