#include "game3.h"
#include "../game_common.h"
#include <libst.h>
using GAME::C_GAME3;
using GAME::PDATA;
int C_GAME3::TextRows[16] = 
{
	2,3,2,2,2,2,1,1,
	2,2,1,2,2,2,2,2
};
int C_GAME3::NumberOfActions[16] = 
{
	 4,5,2,2,5,3,2,1,
	11,3,4,4,2,3,3,1
};
C_GAME3::C_GAME3(int index)
{
	played = false;
	gameIndex = index;
	index1 = rand()%2;
	index2 = rand()%2;
	textRow = TextRows[index];
	nActions = NumberOfActions[index];
}

C_GAME3::~C_GAME3()
{
	UnloadResources();
}

void C_GAME3::LoadResources()
{
	if(resLoaded)
	{
		return;
	}
	int i;
	for(i=0;i<11;i++)
	{
		action[i] = 0;
	}
	switch(gameIndex)
	{
		case 0 :
			action[0] = vgLoadImageFile("picture/small_game/3/1/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/1/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/3/1/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/3/1/4.jpg");
			text = LoadImageFile("picture/small_game/3/1/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/1/voice.wav");
			break;
		case 1 :
			action[0] = vgLoadImageFile("picture/small_game/3/2/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/2/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/3/2/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/3/2/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/3/2/5.jpg");
			text = LoadImageFile("picture/small_game/3/2/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/2/voice.wav");
			break;
		case 2 :
			action[0] = vgLoadImageFile("picture/small_game/3/3/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/3/2.jpg");
			text = LoadImageFile("picture/small_game/3/3/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/3/voice.wav");
			break;
		case 3 :
			action[0] = vgLoadImageFile("picture/small_game/3/4/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/4/2.jpg");
			text = LoadImageFile("picture/small_game/3/4/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/4/voice.wav");
			break;
		case 4 :
			action[0] = vgLoadImageFile("picture/small_game/3/5/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/5/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/3/5/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/3/5/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/3/5/5.jpg");
			text = LoadImageFile("picture/small_game/3/5/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/5/voice.wav");
			break;
		case 5 :
			action[0] = vgLoadImageFile("picture/small_game/3/6/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/6/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/3/6/3.jpg");
			text = LoadImageFile("picture/small_game/3/6/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/6/voice.wav");
			break;
		case 6 :
			action[0] = vgLoadImageFile("picture/small_game/3/7/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/7/2.jpg");
			text = LoadImageFile("picture/small_game/3/7/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/7/voice.wav");
			break;
		case 7 :
			action[0] = vgLoadImageFile("picture/small_game/3/8/1.jpg");
			text = LoadImageFile("picture/small_game/3/8/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/8/voice.wav");
			break;
		case 8 :
			action[0 ] = vgLoadImageFile("picture/small_game/3/9/1.jpg");
			action[1 ] = vgLoadImageFile("picture/small_game/3/9/2.jpg");
			action[2 ] = vgLoadImageFile("picture/small_game/3/9/3.jpg");
			action[3 ] = vgLoadImageFile("picture/small_game/3/9/4.jpg");
			action[4 ] = vgLoadImageFile("picture/small_game/3/9/5.jpg");
			action[5 ] = vgLoadImageFile("picture/small_game/3/9/6.jpg");
			action[6 ] = vgLoadImageFile("picture/small_game/3/9/7.jpg");
			action[7 ] = vgLoadImageFile("picture/small_game/3/9/8.jpg");
			action[8 ] = vgLoadImageFile("picture/small_game/3/9/9.jpg");
			action[9 ] = vgLoadImageFile("picture/small_game/3/9/10.jpg");
			action[10] = vgLoadImageFile("picture/small_game/3/9/11.jpg");
			text = LoadImageFile("picture/small_game/3/9/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/9/voice.wav");
			break;
		case 9 :
			action[0 ] = vgLoadImageFile("picture/small_game/3/10/1.jpg");
			action[1 ] = vgLoadImageFile("picture/small_game/3/10/2.jpg");
			action[2 ] = vgLoadImageFile("picture/small_game/3/10/3.jpg");
			text = LoadImageFile("picture/small_game/3/10/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/10/voice.wav");
			break;
		case 10:
			action[0] = vgLoadImageFile("picture/small_game/3/11/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/11/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/3/11/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/3/11/4.jpg");
			text = LoadImageFile("picture/small_game/3/11/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/11/voice.wav");
			break;
		case 11:
			action[0] = vgLoadImageFile("picture/small_game/3/12/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/12/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/3/12/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/3/12/4.jpg");
			text = LoadImageFile("picture/small_game/3/12/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/12/voice.wav");
			break;
		case 12:
			action[0] = vgLoadImageFile("picture/small_game/3/13/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/13/2.jpg");
			text = LoadImageFile("picture/small_game/3/13/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/13/voice.wav");
			break;
		case 13:
			action[0] = vgLoadImageFile("picture/small_game/3/14/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/14/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/3/14/3.jpg");
			text = LoadImageFile("picture/small_game/3/14/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/14/voice.wav");
			break;
		case 14:
			action[0] = vgLoadImageFile("picture/small_game/3/15/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/3/15/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/3/15/3.jpg");
			text = LoadImageFile("picture/small_game/3/15/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/15/voice.wav");
			break;
		case 15:
			action[0] = vgLoadImageFile("picture/small_game/3/16/1.jpg");
			text = LoadImageFile("picture/small_game/3/16/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/3/16/voice.wav");
			break;
	}
	resLoaded = true;
}

void C_GAME3::UnloadResources()
{
	ReleaseSurface(&text);
	ReleaseSurface(action,11);
}

bool C_GAME3::Play()
{
	if(frameNumber == 0)
	{
		vgSoundPlay(sndMusic[index1],PDATA->Vol*255/250);
	}
	LoadResources();
	switch(gameIndex)
	{
		case 0 :
			vgDrawSurface(window2,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],435,208,0);
			break;
		case 1 :
			vgDrawSurface(action[0],256,162,0);
			vgDrawSurface(action[1 + frameNumber/Speed%2],329,317,0);
			vgDrawSurface(action[3 + frameNumber/Speed%2],667,322,0);
			break;
		case 6 :
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],332,268,0);
			break;
		case 7 :
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],326,178,0);
			break;
		case 8 :
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%10],269,239,0);
			vgDrawSurface(action[10],269,360,0);
			break;
		case 9 :
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],331,262,0);
			break;
		case 10:
			vgDrawSurface(window2,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],440,219,0);
			break;
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
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

int C_GAME3::NumberOfManActors()
{
	return gameInfos3[gameIndex].man;
}

int C_GAME3::NumberOfGirlActors()
{
	return gameInfos3[gameIndex].girl;
}

