#include "query.h"
#include "game_common.h"
#include <libst.h>
using GAME::C_QUERY_CHARGE;
int C_QUERY_CHARGE::ScorePos[3][2] = 
{
	{128,266},{485,266},{802,266}
};
int C_QUERY_CHARGE::SNPos[3][2] = 
{
	{ 33,266},{385,266},{710,266}
};
C_QUERY_CHARGE::C_QUERY_CHARGE():
loaded(false),currentItem(0),
currentPage(0),startPage(0)
{
	LoadResources();
}

C_QUERY_CHARGE::~C_QUERY_CHARGE()
{
	UnloadResources();
}

void C_QUERY_CHARGE::LoadResources()
{
	if(!loaded)
	{
		load_bmp_files("picture/query/bg/",bg,2);
		nr1 = vgLoadImageFile("picture/query/nr1.bmp");
		nr2 = vgLoadImageFile("picture/query/nr2.bmp");
		nr3 = vgLoadImageFile("picture/query/nr3.bmp");
		loaded = true;
	}
}

void C_QUERY_CHARGE::UnloadResources()
{
	if(loaded)
	{
		ReleaseSurface(&nr3);
		ReleaseSurface(&nr2);
		ReleaseSurface(&nr1);
		ReleaseSurface(bg,2);
		loaded = false;
	}
}

void C_QUERY_CHARGE::DisplayCurrentCharge()
{
	if(currentPage != 0)
	{
		return;
	}
	int i,j,k,m,r;
	k = 0;
	for(i = k;i < ENTRIES_PER_PAGE;i++)
	{
		r = PDATA->CurrentIndex - 1 - i;
		if(r < 0)
		{
			r += 30;
		}
		j = i - k;
		if(j < ENTRIES_PER_PAGE/3)
		{
			display_num_horizontal(nr1,3,i + 1,
				SNPos[0][0],SNPos[0][1] + j * Dy,1);
			display_num_horizontal(nr3,9,PDATA->CurrentCharge[r],
				ScorePos[0][0],ScorePos[0][1] + j * Dy,1);
		}
		else if(j < 2*ENTRIES_PER_PAGE/3)
		{
			m = j - ENTRIES_PER_PAGE/3;
			display_num_horizontal(nr1,3,i + 1,
				SNPos[1][0],SNPos[1][1] + m * Dy,1);
			display_num_horizontal(nr3,9,PDATA->CurrentCharge[r],
				ScorePos[1][0],ScorePos[1][1] + m * Dy,1);
		}
		else
		{
			m = j - 2*ENTRIES_PER_PAGE/3;
			display_num_horizontal(nr1,3,i + 1,
				SNPos[2][0],SNPos[2][1] + m * Dy,1);
			display_num_horizontal(nr3,9,PDATA->CurrentCharge[r],
				ScorePos[2][0],ScorePos[2][1] + m * Dy,1);
		}
	}
}

void C_QUERY_CHARGE::DisplayHistoryCharge()
{
	if(currentPage == 0)
	{
		return;
	}
	int i,j,k,m,r;
	k = (currentPage - 1)*ENTRIES_PER_PAGE;
	for(i = k;i < (k + ENTRIES_PER_PAGE) && i < 60;i++)
	{
		r = PDATA->TotalIndex - 1 - i;
		if(r < 0)
		{
			r += 60;
		}
		j = i - k;
		if(j < ENTRIES_PER_PAGE/3)
		{
			display_num_horizontal(nr1,3,i + 1,
				SNPos[0][0],SNPos[0][1] + j * Dy,1);
			display_num_horizontal(nr2,9,PDATA->HistoryCharge[r],
				ScorePos[0][0],ScorePos[0][1] + j * Dy,1);
		}
		else if(j < 2*ENTRIES_PER_PAGE/3)
		{
			m = j - ENTRIES_PER_PAGE/3;
			display_num_horizontal(nr1,3,i + 1,
				SNPos[1][0],SNPos[1][1] + m * Dy,1);
			display_num_horizontal(nr2,9,PDATA->HistoryCharge[r],
				ScorePos[1][0],ScorePos[1][1] + m * Dy,1);
		}
		else
		{
			m = j - 2*ENTRIES_PER_PAGE/3;
			display_num_horizontal(nr1,3,i + 1,
				SNPos[2][0],SNPos[2][1] + m * Dy,1);
			display_num_horizontal(nr2,9,PDATA->HistoryCharge[r],
				ScorePos[2][0],ScorePos[2][1] + m * Dy,1);
		}
	}
}

bool C_QUERY_CHARGE::DealKeyPress()
{
	if(PKEY->PressKey(KEY_RIGHT))
	{
		++currentPage;
		vgSoundPlay(sndPress,PDATA->Vol*255/100);
		if(currentPage*ENTRIES_PER_PAGE >= 90)
		{
			currentPage = startPage;
		}
	}
	else if(PKEY->PressKey(KEY_DOWN))
	{
		vgSoundPlay(sndPress,PDATA->Vol*255/100);
		if(PDATA->CurrentTotal > 0)
		{
			PDATA->HistoryCharge[PDATA->TotalIndex] = PDATA->CurrentTotal;
			++PDATA->TotalIndex;
			PDATA->TotalIndex %= 60;
			PDATA->CurrentTotal = 0;
			PDATA->CurrentIndex = 0;
			memset(PDATA->CurrentCharge,0,sizeof(PDATA->CurrentCharge));
		}
	}
	else if(PKEY->PressKey(KEY_ENTER))
	{
		vgSoundPlay(sndPress,PDATA->Vol*255/100);
		return true;
	}
	return false;
}

bool C_QUERY_CHARGE::Process()
{
	if(startPage == 0 && PDATA->CurrentTotal == 0)
	{
		startPage = 1;
		currentPage = 1;
	}
	if(startPage == 1 && PDATA->CurrentTotal > 0)
	{
		startPage = 0;
		currentPage = 0;
	}
	Draw1024X768Splitly(bg);
	display_num_horizontal(nr1,2,(currentPage + 1),690,152,1);
	display_num_horizontal(nr1,6,PDATA->TotalCharge, 32,158,1);
	DisplayCurrentCharge();
	DisplayHistoryCharge();
	return DealKeyPress();
}

