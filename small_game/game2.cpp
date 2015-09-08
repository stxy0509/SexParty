#include "game2.h"
#include <libst.h>
#include "../game_common.h"
#include <libst.h>
using GAME::C_GAME2;
using GAME::PDATA;
int C_GAME2::TextRows[15] = 
{
	3,3,2,1,1,1,1,3,
	2,2,2,2,2,1,2
};
int C_GAME2::NumberOfActions[15] = 
{
	4,3,7,4,9,3,3,2,
	1,4,2,4,5,2,2
};

C_GAME2::C_GAME2(int index)
{
	played = false;
	gameIndex = index;
	index1 = rand()%2;
	index2 = rand()%2;
	textRow = TextRows[index];
	nActions = NumberOfActions[index];
}

C_GAME2::~C_GAME2()
{
	UnloadResources();
}

void C_GAME2::LoadResources()
{
	if(resLoaded)
	{
		return;
	}
	int i;
	for(i=0;i<9;i++)
	{
		action[i] = 0;
	}
	switch(gameIndex)
	{
		case 0 :
			action[0] = vgLoadImageFile("picture/small_game/2/1/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/1/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/1/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/2/1/4.jpg");
			text = LoadImageFile("picture/small_game/2/1/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/1/voice.wav");
			break;
		case 1 :
			action[0] = vgLoadImageFile("picture/small_game/2/2/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/2/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/2/3.jpg");
			text = LoadImageFile("picture/small_game/2/2/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/2/voice.wav");
			break;
		case 2 :
			action[0] = vgLoadImageFile("picture/small_game/2/3/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/3/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/3/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/2/3/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/2/3/5.jpg");
			action[5] = vgLoadImageFile("picture/small_game/2/3/6.jpg");
			action[6] = vgLoadImageFile("picture/small_game/2/3/7.jpg");
			text = LoadImageFile("picture/small_game/2/3/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/3/voice.wav");
			break;
		case 3 :
			action[0] = vgLoadImageFile("picture/small_game/2/4/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/4/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/4/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/2/4/4.jpg");
			text = LoadImageFile("picture/small_game/2/4/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/4/voice.wav");
			break;
		case 4 :
			action[0] = vgLoadImageFile("picture/small_game/2/5/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/5/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/5/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/2/5/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/2/5/5.jpg");
			action[5] = LoadImageFile("picture/small_game/2/5/6.bmp",0,255,0);
			action[6] = LoadImageFile("picture/small_game/2/5/7.bmp",0,255,0);
			action[7] = LoadImageFile("picture/small_game/2/5/8.bmp",0,255,0);
			action[8] = LoadImageFile("picture/small_game/2/5/9.bmp",0,255,0);
			text = LoadImageFile("picture/small_game/2/5/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/5/voice.wav");
			break;
		case 5 :
			action[0] = vgLoadImageFile("picture/small_game/2/6/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/6/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/6/3.jpg");
			text = LoadImageFile("picture/small_game/2/6/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/6/voice.wav");
			break;
		case 6 :
			action[0] = vgLoadImageFile("picture/small_game/2/7/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/7/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/7/3.jpg");
			text = LoadImageFile("picture/small_game/2/7/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/7/voice.wav");
			break;
		case 7 :
			action[0] = vgLoadImageFile("picture/small_game/2/8/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/8/2.jpg");
			text = LoadImageFile("picture/small_game/2/8/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/8/voice.wav");
			break;
		case 8 :
			action[0] = vgLoadImageFile("picture/small_game/2/9/1.jpg");
			text = LoadImageFile("picture/small_game/2/9/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/9/voice.wav");
			break;
		case 9 :
			action[0] = vgLoadImageFile("picture/small_game/2/10/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/10/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/10/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/2/10/4.jpg");
			text = LoadImageFile("picture/small_game/2/10/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/10/voice.wav");
			break;
		case 10:
			action[0] = vgLoadImageFile("picture/small_game/2/11/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/11/2.jpg");
			text = LoadImageFile("picture/small_game/2/11/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/11/voice.wav");
			break;
		case 11:
			action[0] = vgLoadImageFile("picture/small_game/2/12/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/12/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/12/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/2/12/4.jpg");
			text = LoadImageFile("picture/small_game/2/12/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/12/voice.wav");
			break;
		case 12:
			action[0] = vgLoadImageFile("picture/small_game/2/13/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/13/2.jpg");
			action[2] = vgLoadImageFile("picture/small_game/2/13/3.jpg");
			action[3] = vgLoadImageFile("picture/small_game/2/13/4.jpg");
			action[4] = vgLoadImageFile("picture/small_game/2/13/5.jpg");
			text = LoadImageFile("picture/small_game/2/13/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/13/voice.wav");
			break;
		case 13:
			action[0] = vgLoadImageFile("picture/small_game/2/14/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/14/2.jpg");
			text = LoadImageFile("picture/small_game/2/14/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/14/voice.wav");
			break;
		case 14:
			action[0] = vgLoadImageFile("picture/small_game/2/15/1.jpg");
			action[1] = vgLoadImageFile("picture/small_game/2/15/2.jpg");
			text = LoadImageFile("picture/small_game/2/15/text.bmp",0,255,0);
			sndVoice = LoadWavFile("picture/small_game/2/15/voice.wav");
			break;
	}
	resLoaded = true;
}

void C_GAME2::UnloadResources()
{
	ReleaseSurface(&text);
	ReleaseSurface(action,9);
}

bool C_GAME2::Play()
{
	if(frameNumber == 0)
	{
		vgSoundPlay(sndMusic[index1],PDATA->Vol*255/250);
	}
	LoadResources();
	switch(gameIndex)
	{
		case 0:
			int index = frameNumber/Speed%nActions;
			vgDrawSurface(window1,256,162,0);
			if(index < 3)
			{
				vgDrawSurface(action[index],279,198,0);
			}
			else
			{
				DrawPartOfSurface(action[3],2,3,index - 3,
					396,198,0);
			}
			break;
		case 1:
			vgDrawSurface(window2,256,162,0);
			vgDrawSurface(action[nActions - 1],391,184,0);
			vgDrawSurface(action[frameNumber/Speed%(nActions - 1)],464,405,0);
			break;
		case 2:
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],256,289,0);
			break;
		case 3:
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],371,257,0);
			break;
		case 4:
		{
			int i;
			int ManPos[5][2] = 
			{
				{257,279},{261,281},{266,283},{272,285},{280,292}
			};
			int GirlPos[5][2] = 
			{
				{547,286},{541,290},{541,293},{531,295},{523,299}
			};
			
			vgDrawSurface(window1,256,162,0);
			for(i=0;i<5;i++)
			{
				if(frameNumber%(5*6*Speed) < (i+1)*6*Speed)
				{
					break;
				}
			}
			vgDrawSurface(action[i],474,162,0);
			vgDrawSurface(action[7 + frameNumber/Speed%2], ManPos[i][0], ManPos[i][1],0);
			vgDrawSurface(action[5 + frameNumber/Speed%2],GirlPos[i][0],GirlPos[i][1],0);
			break;
		}
		case 5 :
			vgDrawSurface(window2,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],349,240,0);
			break;
		case 6 :
			vgDrawSurface(window2,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],350,255,0);
			break;
		case 7 :
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 8 :
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 9 :
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 10:
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 11:
			vgDrawSurface(window2,256,162,0);
			vgDrawSurface(action[3],410,216,0);
			if(frameNumber%(2*Speed) < Speed)
			{
				vgDrawSurface(action[2],519,261,0);
			}
			vgDrawSurface(action[frameNumber/Speed%2],481,398,0);
			break;
		case 12:
			vgDrawSurface(window1,256,162,0);
			vgDrawSurface(action[frameNumber/Speed%nActions],442,257,0);
			break;
		case 13:
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
		case 14:
			vgDrawSurface(action[frameNumber/Speed%nActions],256,162,0);
			break;
	}
	if(frameNumber == 1)
	{
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

int C_GAME2::NumberOfManActors()
{
	return gameInfos2[gameIndex].man;
}

int C_GAME2::NumberOfGirlActors()
{
	return gameInfos2[gameIndex].girl;
}
