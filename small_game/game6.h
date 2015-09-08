#ifndef GAME_6_H
#define GAME_6_H
#include "smallgame.h"
namespace GAME
{
	class C_GAME6 : public C_SMALL_GAME
	{
	private:
		int nActions;
		int gameIndex;
		vgSURFACE *action[5];
		bool played;
		void LoadResources();
		void UnloadResources();
		static const int Speed = 10;
		static int TextRows[3];
		static int NumberOfActions[3];
	public:
		C_GAME6(int index);
		~C_GAME6();
		bool Play();
		virtual int NumberOfManActors();
		virtual int NumberOfGirlActors();
	};
}
#endif




