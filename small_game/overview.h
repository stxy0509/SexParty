#ifndef OVERVIEW_H
#define OVERVIEW_H
#include "smallgame.h"
namespace GAME
{
	class C_OVERVIEW : public C_SMALL_GAME
	{
	private:
		static int index;
		vgSURFACE *bg;
		void LoadResources();
		void UnloadResources();
	public:
		C_OVERVIEW();
		~C_OVERVIEW();
		bool Play();
		virtual int NumberOfManActors();
		virtual int NumberOfGirlActors();
	};
}
#endif
