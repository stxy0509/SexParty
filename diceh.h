#ifndef DICE_PROCESS_H
#define DICE_PROCESS_H
#include <EagleSDK.h>
#include "game_common.h"
class C_UP_TIMER;
namespace GAME
{
	class C_DICE_HANDLE
	{
	private:
		enum 
		{
			WAIT_TIME = 8,
			SPEED = 5,
		};
		
		int m_points;
		int m_pos;
		int m_score;
		int m_stage;
		int m_time;
		int m_p[3];
		int m_dir;
		int m_flip;
		int m_showScore;
		int m_dispTriple;
		C_UP_TIMER *ptimer;
		vgSURFACE *paction;
		vgSURFACE *ppoints;
		vgSURFACE *ptriple;
		bool begin;
		void GenerateResult();
		void DisplayTime();
		void DisplayPoints();
		void DisplayClosed();
		void DisplayOpening();
		void DisplayTriple();
		bool IsTriple();
		bool IsFinish();
		int  PointsToScore();
		static int playCount;
		static vgSURFACE *picTriple;
		static vgSURFACE *picTripleLeft;
		static vgSURFACE *picPoints;
		static vgSURFACE *picPointsLeft;
		static vgSURFACE *action;
		static vgSURFACE *actionLeft;
		static vgWAVE *sndRoll;
		static int TablePos[MAX_PLAYERS][2];
		static int DicePos[4][3][2];
		static int TriplePos[4][2];
		static int TimePos[4][2];
		static C_DICE_HANDLE *pdice[MAX_PLAYERS];
	public:
		C_DICE_HANDLE(int p);
		~C_DICE_HANDLE();
		void Play();
		void Reset();
		void BeginToPlay();
		static void LoadResources();
		static void UnloadResources();
		static bool AllFinish();
		static void AllReset();
	};
}
#endif
