#ifndef GAME_5_H
#define GAME_5_H
#include "smallgame.h"
namespace GAME
{
	class C_GAME5 : public C_SMALL_GAME
	{
	private:
		int nActions;
		int gameIndex;
		vgSURFACE *action[6];
		bool played;
		void LoadResources();
		void UnloadResources();
		static const int Speed = 10;
		static int TextRows[9];
		static int NumberOfActions[9];
	public:
		C_GAME5(int index);
		~C_GAME5();
		bool Play();
		virtual int NumberOfManActors();
		virtual int NumberOfGirlActors();
	};
}
#endif

