#include "game4.h"
#include "../game_common.h"
#include <libst.h>
using GAME::C_GAME4;
using GAME::PDATA;
int C_GAME4::TextRows[17] = 
{
	2,2,2,1,2,1,2,2,2,
	2,2,2,2,2,2,2,2
};

int C_GAME4::NumberOfActions[17] = 
{
	2,5,6,4,5,7,2,5,5,
	7,2,6,7,2,5,5,7
};

C_GAME4::C_GAME4(int index)
{
	played = false;
	gameIndex = index;
	index1 = rand()%2;
	index2 = rand()%2;
	textRow = TextRows[index];
	nActions = NumberOfActions[index];
}

C_GAME4::~C_GAME4()
{
	UnloadResources();
}

void C_GAME4::LoadResources()
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
			action[0] = vgLoadImageFile("picture/small_game/4/1/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/1/2.jpg");
			text = LoadImageFile("picture/small_game/4/1/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/1/voice.wav");
			break;
		case 1 :
			action[0] = vgLoadImageFile("picture/small_game/4/2/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/2/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/2/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/2/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/2/5.jpg");
			text = LoadImageFile("picture/small_game/4/2/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/2/voice.wav");
			break;
		case 2 :
			action[0] = vgLoadImageFile("picture/small_game/4/3/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/3/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/3/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/3/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/3/5.jpg");
			action[5] = vgLoadImageFile("picture/small_game/4/3/6.jpg");
			text = LoadImageFile("picture/small_game/4/3/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/3/voice.wav");
			break;
		case 3 :
			action[0] = vgLoadImageFile("picture/small_game/4/4/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/4/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/4/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/4/4.jpg");
			text = LoadImageFile("picture/small_game/4/4/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/4/voice.wav");
			break;
		case 4 :
			action[0] = vgLoadImageFile("picture/small_game/4/5/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/5/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/5/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/5/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/5/5.jpg");
			text = LoadImageFile("picture/small_game/4/5/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/5/voice.wav");
			break;
		case 5 :
			action[0] = vgLoadImageFile("picture/small_game/4/6/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/6/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/6/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/6/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/6/5.jpg");
			action[5] = vgLoadImageFile("picture/small_game/4/6/6.jpg");
			action[6] = vgLoadImageFile("picture/small_game/4/6/7.jpg");
			text = LoadImageFile("picture/small_game/4/6/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/6/voice.wav");
			break;
		case 6 :
			action[0] = vgLoadImageFile("picture/small_game/4/7/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/7/2.jpg");
			text = LoadImageFile("picture/small_game/4/7/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/7/voice.wav");
			break;
		case 7 :
			action[0] = vgLoadImageFile("picture/small_game/4/8/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/8/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/8/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/8/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/8/5.jpg");
			text = LoadImageFile("picture/small_game/4/8/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/8/voice.wav");
			break;
		case 8 :
			action[0] = vgLoadImageFile("picture/small_game/4/9/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/9/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/9/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/9/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/9/5.jpg");
			text = LoadImageFile("picture/small_game/4/9/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/9/voice.wav");
			break;
		case 9 :
			action[0] = vgLoadImageFile("picture/small_game/4/10/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/10/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/10/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/10/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/10/5.jpg");
			action[5] = vgLoadImageFile("picture/small_game/4/10/6.jpg");
			action[6] = vgLoadImageFile("picture/small_game/4/10/7.jpg");
			text = LoadImageFile("picture/small_game/4/10/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/10/voice.wav");
			break;
		case 10:
			action[0] = vgLoadImageFile("picture/small_game/4/11/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/11/2.jpg");
			text = LoadImageFile("picture/small_game/4/11/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/11/voice.wav");
			break;
		case 11:
			action[0] = vgLoadImageFile("picture/small_game/4/12/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/12/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/12/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/12/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/12/5.jpg");
			action[5] = vgLoadImageFile("picture/small_game/4/12/6.jpg");
			text = LoadImageFile("picture/small_game/4/12/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/12/voice.wav");
			break;
		case 12:
			action[0] = vgLoadImageFile("picture/small_game/4/13/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/13/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/13/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/13/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/13/5.jpg");
			action[5] = vgLoadImageFile("picture/small_game/4/13/6.jpg");
			action[6] = vgLoadImageFile("picture/small_game/4/13/7.jpg");
			text = LoadImageFile("picture/small_game/4/13/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/13/voice.wav");
			break;
		case 13:
			action[0] = vgLoadImageFile("picture/small_game/4/14/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/14/2.jpg");
			text = LoadImageFile("picture/small_game/4/14/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/14/voice.wav");
			break;
		case 14:
			action[0] = vgLoadImageFile("picture/small_game/4/15/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/15/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/15/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/15/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/15/5.jpg");
			text = LoadImageFile("picture/small_game/4/15/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/15/voice.wav");
			break;
		case 15:
			action[0] = vgLoadImageFile("picture/small_game/4/16/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/16/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/16/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/16/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/16/5.jpg");
			text = LoadImageFile("picture/small_game/4/16/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/16/voice.wav");
			break;
		case 16:
			action[0] = vgLoadImageFile("picture/small_game/4/17/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/4/17/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/4/17/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/4/17/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/4/17/5.jpg");
			action[5] = vgLoadImageFile("picture/small_game/4/17/6.jpg");
			action[6] = vgLoadImageFile("picture/small_game/4/17/7.jpg");
			text = LoadImageFile("picture/small_game/4/17/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/4/17/voice.wav");
			break;
	}
	resLoaded = true;
}

void C_GAME4::UnloadResources()
{
	ReleaseSurface(&text);
	ReleaseSurface(action,7);
}

bool C_GAME4::Play()
{
	if(frameNumber == 0)
	{
		vgSoundPlay(sndMusic[index1],PDATA->Vol*255/250);
	}
	LoadResources();
	vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
	DisplayBorder();
	DisplayText();
	if(!played)
	{
		played = true;
		vgSoundPlay(sndVoice,PDATA->Vol*255/100);
	}
	if(soundt == time)
	{
		vgSoundPlay(sndWarm[index2],PDATA->Vol*255/100);
	}
	++time;
	++frameNumber;
	return DisplayTip();
}

int C_GAME4::NumberOfManActors()
{
	return gameInfos4[gameIndex].man;
}

int C_GAME4::NumberOfGirlActors()
{
	return gameInfos4[gameIndex].girl;
}

