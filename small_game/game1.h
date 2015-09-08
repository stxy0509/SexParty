#ifndef GAME_1_H
#define GAME_1_H
#include "smallgame.h"
namespace GAME
{
	class C_GAME1 : public C_SMALL_GAME
	{
	private:
		int nActions;
		int gameIndex;
		vgSURFACE *action[7];
		bool played;
		void LoadResources();
		void UnloadResources();
		static const int Speed = 10;
		static int TextRows[13];
		static int NumberOfActions[13];
	public:
		C_GAME1(int index);
		~C_GAME1();
		bool Play();
		virtual int NumberOfManActors();
		virtual int NumberOfGirlActors();
	};
}
#endif
