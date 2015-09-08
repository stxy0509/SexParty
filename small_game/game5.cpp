#include "game5.h"
#include "../game_common.h"
#include <libst.h>
using GAME::C_GAME5;
using GAME::PDATA;
int C_GAME5::TextRows[9] = 
{
	2,2,1,1,1,2,1,1,2
};

int C_GAME5::NumberOfActions[9] = 
{
	4,3,3,2,4,3,4,1,6
};

C_GAME5::C_GAME5(int index)
{
	played = false;
	gameIndex = index;
	index1 = rand()%2;
	index2 = rand()%2;
	textRow  = TextRows[index];
	nActions = NumberOfActions[index];
}

C_GAME5::~C_GAME5()
{
	UnloadResources();
}

void C_GAME5::LoadResources()
{
	if(resLoaded)
	{
		return;
	}
	int i;
	for(i=0;i<6;i++)
	{
		action[i] = 0;
	}
	switch(gameIndex)
	{
		case 0 :
			action[0] = vgLoadImageFile("picture/small_game/5/1/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/5/1/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/5/1/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/5/1/4.jpg");
			text = LoadImageFile("picture/small_game/5/1/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/5/1/voice.wav");
			break;
		case 1 :
			action[0] = vgLoadImageFile("picture/small_game/5/2/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/5/2/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/5/2/3.jpg");
			text = LoadImageFile("picture/small_game/5/2/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/5/2/voice.wav");
			break;
		case 2 :
			action[0] = vgLoadImageFile("picture/small_game/5/3/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/5/3/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/5/3/3.jpg");
			text = LoadImageFile("picture/small_game/5/3/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/5/3/voice.wav");
			break;
		case 3:
			action[0] = LoadImageFile("picture/small_game/5/4/1.bmp",0,0,255);
			action[1] = LoadImageFile("picture/small_game/5/4/2.bmp",0,0,255);
			text = LoadImageFile("picture/small_game/5/4/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/5/4/voice.wav");
			break;
		case 4:
			action[0] = vgLoadImageFile("picture/small_game/5/5/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/5/5/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/5/5/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/5/5/4.jpg");
			text = LoadImageFile("picture/small_game/5/5/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/5/5/voice.wav");
			break;
		case 5:
			action[0] = vgLoadImageFile("picture/small_game/5/6/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/5/6/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/5/6/3.jpg");
			text = LoadImageFile("picture/small_game/5/6/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/5/6/voice.wav");
			break;
		case 6:
			action[0] = LoadImageFile("picture/small_game/5/7/1.bmp",0,0,255);
			action[1] = LoadImageFile("picture/small_game/5/7/2.bmp",0,0,255);
			action[2] = vgLoadImageFile("picture/small_game/5/7/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/5/7/4.jpg");
			text = LoadImageFile("picture/small_game/5/7/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/5/7/voice.wav");
			break;
		case 7:
			action[0] = vgLoadImageFile("picture/small_game/5/8/bg.jpg");
			text = LoadImageFile("picture/small_game/5/8/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/5/8/voice.wav");
			break;
		case 8:
			action[0] = vgLoadImageFile("picture/small_game/5/9/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/5/9/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/5/9/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/5/9/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/5/9/5.jpg");
			action[5] = vgLoadImageFile("picture/small_game/5/9/6.jpg");
			text = LoadImageFile("picture/small_game/5/9/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/5/9/voice.wav");
			break;
	}
	resLoaded = true;
}

void C_GAME5::UnloadResources()
{
	ReleaseSurface(&text);
	ReleaseSurface(action,6);
}

bool C_GAME5::Play()
{
	if(frameNumber == 0)
	{
		vgSoundPlay(sndMusic[index1],PDATA->Vol*255/250);
	}
	LoadResources();
	switch(gameIndex)
	{
		case 0:
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],327,269,0);
			break;
		case 1:
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],327,269,0);
			break;
		case 2:
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 3:
			int T = frameNumber % 80;
			vgDrawSurface(window2,256,162,0);
			if(T < 50)
			{
				if(T < 34)
				{
					vgSetTexture(action[0]);
					vgDrawTexture(240,    316,action[0]->w + T - 34,action[0]->h,34 - T,0,0);
					vgSetTexture(action[1]);
					vgDrawTexture(562 - T,219,action[1]->w + T - 34,action[1]->h,0,     0,0);
				}
				else
				{
					vgDrawSurface(action[0],206 + T,316,0);
					vgDrawSurface(action[1],562 - T,219,0);
				}
			}
			else
			{
				vgDrawSurface(action[0],256,316,0);
				vgDrawSurface(action[1],512,219,0);
			}
			break;
		case 4:
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 5:
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],327,269,0);
			break;
		case 6:
			vgDrawSurface(window2,256,162,0);
			vgDrawSurface(action[3],404,191,0);
			if(frameNumber%(2*Speed) < Speed)
			{
				vgDrawSurface(action[2],524,238,0);
			}
			vgDrawSurface(action[frameNumber/Speed%2],274,378,0);
			vgDrawSurface(action[frameNumber/Speed%2],450,378,0);
			vgDrawSurface(action[frameNumber/Speed%2],623,378,0);
			break;
		case 7:
		case 8:
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

int C_GAME5::NumberOfManActors()
{
	return gameInfos5[gameIndex].man;
}

int C_GAME5::NumberOfGirlActors()
{
	return gameInfos5[gameIndex].girl;
}

