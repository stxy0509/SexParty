#ifndef QUERY_CHARGE_H_
#define QUERY_CHARGE_H_
#include <EagleSDK.h>
namespace GAME
{
	class C_QUERY_CHARGE
	{
	private:
		enum
		{
			ENTRIES_PER_PAGE = 30,
		};
		vgSURFACE *bg[2];
		vgSURFACE *nr1;
		vgSURFACE *nr2;
		vgSURFACE *nr3;
		int loaded;
		int currentItem;
		int currentPage;
		int startPage;
		static const int Dy = 47;
		static int ScorePos[3][2];
		static int SNPos[3][2];
		void LoadResources();
		void UnloadResources();
		void DisplayArrow();
		bool DealKeyPress();
		void DisplayCurrentCharge();
		void DisplayHistoryCharge();
		C_QUERY_CHARGE(const C_QUERY_CHARGE&);
		C_QUERY_CHARGE& operator=(const C_QUERY_CHARGE&);
	public:
		C_QUERY_CHARGE();
		~C_QUERY_CHARGE();
		bool Process();
	};
};
#endif
