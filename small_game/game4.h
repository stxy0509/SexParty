#ifndef GAME_4_H
#define GAME_4_H
#include "smallgame.h"
namespace GAME
{
	class C_GAME4 : public C_SMALL_GAME
	{
	private:
		int nActions;
		int gameIndex;
		vgSURFACE *action[7];
		bool played;
		void LoadResources();
		void UnloadResources();
		static const int Speed = 10;
		static int TextRows[17];
		static int NumberOfActions[17];
	public:
		C_GAME4(int index);
		~C_GAME4();
		bool Play();
		virtual int NumberOfManActors();
		virtual int NumberOfGirlActors();
	};
}
#endif
