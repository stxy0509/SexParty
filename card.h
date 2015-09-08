#ifndef GAME_CARD_H
#define GAME_CARD_H
#include <EagleSDK.h>
#include <list>
#include "game_common.h"
namespace GAME
{
	class C_CARD
	{
	private:
		enum
		{
			NONE = 0,
			DISTRIBUTE,
			WAITTOOPEN,
			OPENING1,
			OPENING2,
			OPENED
		};
		int m_pos;
		int m_time;
		int m_dir;
		int m_state;
		int m_card1;
		int m_card2;
		U32 m_option;
		float m_x[2],m_y[2];
		float m_dx[2],m_dy[2];
		static vgSURFACE *pokers;
		static vgSURFACE *pokersL;
		static vgSURFACE *backCover;
		static vgSURFACE *backCoverL;
		static int InitialCardPos[2][2];
		static int ClockPos[MAX_PLAYERS][2];
		static int FinalCardPos[MAX_PLAYERS][2][2];
		int PointOfCard(int idx);
		void InitializePosition();
	public:
		C_CARD(int pos);
		~C_CARD();
		void Play();
		void Reset();
		bool IsStart();
		bool IsOpened();
		void BeginDistribute();
		void SetCards(int c1,int c2);
		static void LoadResources();
		static void UnloadResources();
	};
}
#endif
