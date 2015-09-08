#ifndef SMALL_GAME_H_
#define SMALL_GAME_H_
#include <list>
#include <EagleSDK.h>
std::list<int> operator+(const std::list<int>& a,const std::list<int>& b);
namespace GAME
{
	enum 
	{
		CHIEF_MAN = 0,
		CHIEF_GIRL,
		CHIEF_EITHER
	};
	struct TYPE_INDEX
	{
		int type;
		int index;
	};
	struct GAME_INFO
	{
		int man;
		int girl;
		int chief;
	};
	class C_SMALL_GAME
	{
	protected:
		int frameNumber;
		int time;
		int index1;
		int index2;
		int textRow;
		int soundt;
		vgWAVE *sndVoice;
		vgSURFACE *text;
		bool resLoaded;
		
		static bool listInit;
		static int manType;
		static int girlType;
		static std::list<int> gamesForMan[6];
		static std::list<int> gamesForGirl[6];
		static std::list<int> gamesForBoth[6];
		static GAME_INFO gameInfos1[13];
		static GAME_INFO gameInfos2[15];
		static GAME_INFO gameInfos3[16];
		static GAME_INFO gameInfos4[17];
		static GAME_INFO gameInfos5[9];
		static GAME_INFO gameInfos6[3];
		C_SMALL_GAME(const C_SMALL_GAME&);
		C_SMALL_GAME& operator=(const C_SMALL_GAME&);
		void DisplayText();
		void DisplayTextDown();
		void DisplayTextUp();
		static void InitGameList();
		static void ResetGameList(int type,int forwhat);
	public:
		C_SMALL_GAME();
		void LoadPictures();
		virtual ~C_SMALL_GAME();
		virtual bool Play();
		virtual int  NumberOfManActors();
		virtual int  NumberOfGirlActors();
		static TYPE_INDEX SelectGame(bool forMan,bool gift);
		static vgWAVE *sndWarm[2];
		static vgWAVE *sndMusic[2];
	};
}
#endif

