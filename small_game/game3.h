#ifndef GAME_3_H
#define GAME_3_H
#include "smallgame.h"
namespace GAME
{
	class C_GAME3 : public C_SMALL_GAME
	{
	private:
		int nActions;
		int gameIndex;
		vgSURFACE *action[11];
		bool played;
		void LoadResources();
		void UnloadResources();
		static const int Speed = 10;
		static int TextRows[16];
		static int NumberOfActions[16];
	public:
		C_GAME3(int index);
		~C_GAME3();
		bool Play();
		virtual int NumberOfManActors();
		virtual int NumberOfGirlActors();
	};
}
#endif
