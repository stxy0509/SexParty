#ifndef GAME_2_H
#define GAME_2_H
#include "smallgame.h"
namespace GAME
{
	class C_GAME2 : public C_SMALL_GAME
	{
	private:
		int nActions;
		int gameIndex;
		vgSURFACE *action[9];
		bool played;
		void LoadResources();
		void UnloadResources();
		static const int Speed = 10;
		static int TextRows[15];
		static int NumberOfActions[15];
	public:
		C_GAME2(int index);
		~C_GAME2();
		bool Play();
		virtual int NumberOfManActors();
		virtual int NumberOfGirlActors();
	};
}
#endif

