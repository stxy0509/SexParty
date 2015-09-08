#include "game1.h"
#include "../game_common.h"
#include <libst.h>
using GAME::C_GAME1;
using GAME::PDATA;
int C_GAME1::TextRows[13] = 
{
	2,2,2,2,2,2,2,
	2,2,2,2,1,2
};
int C_GAME1::NumberOfActions[13] = 
{
	3,2,7,4,2,2,4,
	1,3,3,2,3,3
};
C_GAME1::C_GAME1(int index)
{
	played = false;
	gameIndex = index;
	index1 = rand()%2;
	index2 = rand()%2;
	textRow = TextRows[index];
	nActions = NumberOfActions[index];
}

C_GAME1::~C_GAME1()
{
	UnloadResources();
}

void C_GAME1::LoadResources()
{
	if(resLoaded)
	{
		return;
	}
	int i;
	for(i=0;i<7;i++)
	{
		action[i] = 0;
	}
	switch(gameIndex)
	{
		case 0 :
			action[0] = vgLoadImageFile("picture/small_game/1/1/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/1/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/1/1/3.jpg");
			text = LoadImageFile("picture/small_game/1/1/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/1/voice.wav");
			break;
		case 1 :
			action[0] = vgLoadImageFile("picture/small_game/1/2/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/2/2.jpg");
			text = LoadImageFile("picture/small_game/1/2/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/2/voice.wav");
			break;
		case 2:
			action[0] = vgLoadImageFile("picture/small_game/1/3/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/3/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/1/3/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/1/3/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/1/3/5.jpg");
			action[5] = vgLoadImageFile("picture/small_game/1/3/6.jpg");
			action[6] = vgLoadImageFile("picture/small_game/1/3/7.jpg");
			text = LoadImageFile("picture/small_game/1/3/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/3/voice.wav");
			break;
		case 3:
			action[0] = vgLoadImageFile("picture/small_game/1/4/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/4/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/1/4/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/1/4/4.jpg");
			text = LoadImageFile("picture/small_game/1/4/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/4/voice.wav");
			break;
		case 4:
			action[0] = vgLoadImageFile("picture/small_game/1/5/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/5/2.jpg");
			text = LoadImageFile("picture/small_game/1/5/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/5/voice.wav");
			break;
		case 5:
			action[0] = vgLoadImageFile("picture/small_game/1/6/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/6/2.jpg");
			text = LoadImageFile("picture/small_game/1/6/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/6/voice.wav");
			break;
		case 6:
			action[0] = vgLoadImageFile("picture/small_game/1/7/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/7/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/1/7/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/1/7/4.jpg");
			text = LoadImageFile("picture/small_game/1/7/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/7/voice.wav");
			break;
		case 7:
			action[0] = vgLoadImageFile("picture/small_game/1/8/1.jpg");
			text = LoadImageFile("picture/small_game/1/8/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/8/voice.wav");
			break;
		case 8:
			action[0] = vgLoadImageFile("picture/small_game/1/9/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/9/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/1/9/3.jpg");
			text = LoadImageFile("picture/small_game/1/9/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/9/voice.wav");
			break;
		case 9:
			action[0] = vgLoadImageFile("picture/small_game/1/10/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/10/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/1/10/3.jpg");
			text = LoadImageFile("picture/small_game/1/10/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/10/voice.wav");
			break;
		case 10:
			action[0] = vgLoadImageFile("picture/small_game/1/11/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/11/2.jpg");
			text = LoadImageFile("picture/small_game/1/11/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/11/voice.wav");
			break;
		case 11:
			action[0] = vgLoadImageFile("picture/small_game/1/12/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/12/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/1/12/3.jpg");
			text = LoadImageFile("picture/small_game/1/12/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/12/voice.wav");
			break;
		case 12:
			action[0] = vgLoadImageFile("picture/small_game/1/13/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/1/13/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/1/13/3.jpg");
			text = LoadImageFile("picture/small_game/1/13/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/1/13/voice.wav");
			break;
	}
	resLoaded = true;
}

void C_GAME1::UnloadResources()
{
	ReleaseSurface(&text);
	ReleaseSurface(action,7);
}

bool C_GAME1::Play()
{
	if(frameNumber == 0)
	{
		vgSoundPlay(sndMusic[index1],PDATA->Vol*255/250);	
	}
	LoadResources();
	switch(gameIndex)
	{
		case 0 :
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],392,181,0);
			break;
		case 1 :
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],320,181,0);
			break;
		case 2 :
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 3 :
			vgDrawSurface(window2,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],347,236,0);
			break;
		case 4 :
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 5 :
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 6 :
			vgDrawSurface(window1,256,162,0);
			if(frameNumber < nActions*Speed)
			{
				vgDrawSurface(action[frameNumber/Speed],403,254,0);
			}
			else if(frameNumber < (nActions + 5)*Speed)
			{
				vgDrawSurface(action[nActions - 1],403,254,0);
				if(frameNumber == (nActions + 5)*Speed - 1)
				{
					frameNumber = 0;
				}
			}
			break;
		case 7 :
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 8 :
			vgDrawSurface(action[0],256,162,0);
			if(frameNumber%(5*Speed) < Speed)
			{

			}
			else if(frameNumber%(5*Speed) < 2*Speed)
			{
				vgDrawSurface(action[1],377,279,0);
			}
			else
			{
				vgDrawSurface(action[2],377,279,0);
			}
			break;
		case 9 :
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 10:
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 11:
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 12:
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
	}
	
	if(!played)
	{
		vgSoundPlay(sndVoice,PDATA->Vol*255/100);
		played = true;
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

int C_GAME1::NumberOfManActors()
{
	return gameInfos1[gameIndex].man;
}

int C_GAME1::NumberOfGirlActors()
{
	return gameInfos1[gameIndex].girl;
}
