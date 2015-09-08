#include "game6.h"
#include "../game_common.h"
#include <libst.h>
using GAME::C_GAME6;
using GAME::PDATA;
int C_GAME6::TextRows[3] = {2,2,2};
int C_GAME6::NumberOfActions[3] = {5,5,5};
C_GAME6::C_GAME6(int index)
{
	played = false;
	gameIndex = index;
	index1 = rand()%2;
	index2 = rand()%2;
	textRow = TextRows[index];
	nActions = NumberOfActions[index];
}

C_GAME6::~C_GAME6()
{
	UnloadResources();
}

void C_GAME6::LoadResources()
{
	if(resLoaded)
	{
		return;
	}
	int i;
	for(i=0;i<5;i++)
	{
		action[i] = 0;
	}
	switch(gameIndex)
	{
		case 0 :
			action[0] = vgLoadImageFile("picture/small_game/6/1/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/6/1/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/6/1/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/6/1/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/6/1/5.jpg");
			text = LoadImageFile("picture/small_game/6/1/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/6/1/voice.wav");
			break;
		case 1 :
			action[0] = vgLoadImageFile("picture/small_game/6/2/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/6/2/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/6/2/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/6/2/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/6/2/5.jpg");
			text = LoadImageFile("picture/small_game/6/2/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/6/2/voice.wav");
			break;
		case 2 :
			action[0] = vgLoadImageFile("picture/small_game/6/3/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/6/3/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/6/3/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/6/3/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/6/3/5.jpg");
			text = LoadImageFile("picture/small_game/6/3/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/6/3/voice.wav");
			break;
	}
	resLoaded = true;
}

void C_GAME6::UnloadResources()
{
	ReleaseSurface(&text);
	ReleaseSurface(action,5);
}

bool C_GAME6::Play()
{
	if(frameNumber == 0)
	{
		vgSoundPlay(sndMusic[index1],PDATA->Vol*255/250);
	}
	LoadResources();
	switch(gameIndex)
	{
		case 0:
		case 1:
		case 2:
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
	}
	if(!played)
	{
		played = true;
		vgSoundPlay(sndVoice,PDATA->Vol*255/100);
	}
	if(soundt == time)
	{
		vgSoundPlay(sndWarm[index2],PDATA->Vol*255/100);
	}
	DisplayBorder();
	DisplayText();
	++time;
	++frameNumber;
	return DisplayTip();
}

int C_GAME6::NumberOfManActors()
{
	return gameInfos6[gameIndex].man;
}

int C_GAME6::NumberOfGirlActors()
{
	return gameInfos6[gameIndex].girl;
}
