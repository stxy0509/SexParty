#include "bonus.h"
#include "game_common.h"
#include <libst.h>
using GAME::C_BONUS_GAME;
using GAME::PDATA;
using GAME::PKEY;
using GAME::watch_s;
using GAME::man;
using GAME::girl;
using GAME::PlayersEnter;
using GAME::DisplaySmallWatch;
using GAME::DisplayRoleName;
vgWAVE *C_BONUS_GAME::sndName = 0;
vgWAVE *C_BONUS_GAME::sndWine = 0;
vgWAVE *C_BONUS_GAME::sndBadLuck = 0;
vgWAVE *C_BONUS_GAME::sndGoodLuck = 0;
vgSURFACE *C_BONUS_GAME::bg = 0;
vgSURFACE *C_BONUS_GAME::text = 0;
vgSURFACE *C_BONUS_GAME::sad = 0;
vgSURFACE *C_BONUS_GAME::name = 0;
vgSURFACE *C_BONUS_GAME::happy = 0;
vgSURFACE *C_BONUS_GAME::picaward = 0;
vgSURFACE *C_BONUS_GAME::rollFrame = 0;
vgSURFACE *C_BONUS_GAME::flashFrame = 0;
vgSURFACE *C_BONUS_GAME::awardFrame = 0;
int C_BONUS_GAME::Minutes[5] = 
{
	0,0,5,10,15
};
C_BONUS_GAME::C_BONUS_GAME():
time(0),stage(0),award(0),rollSpeed(16),
awardx(0),currentx(0),sorted(false),
n8(0),n4(0)
{
	LoadResources();
	basex = (screenw - bg->w)/2;
	basey = (screenh - bg->h)/2;
}

C_BONUS_GAME::~C_BONUS_GAME()
{
	UnloadResources();
}

void C_BONUS_GAME::LoadResources()
{
	if(bg == 0)
	{
		bg = vgLoadImageFile("picture/bonus/bg.bmp");
		sad = LoadImageFile("picture/bonus/sad.bmp",255,255,255);
		text = LoadImageFile("picture/bonus/text.bmp",255,255,255);
		name = LoadImageFile("picture/bonus/name.bmp",0,255,0);
		happy = LoadImageFile("picture/bonus/happy.bmp",255,255,255);
		picaward = LoadImageFile("picture/bonus/award.bmp",0,255,0);
		rollFrame = LoadImageFile("picture/bonus/rf.bmp",255,0,0);
		awardFrame = LoadImageFile("picture/bonus/af.bmp",255,0,0);
		flashFrame = LoadImageFile("picture/bonus/ff.bmp",255,255,255);
		
		sndName = LoadWavFile("wav/bonus/name.wav");
		sndWine = LoadWavFile("wav/bonus/wine.wav");
		sndBadLuck = LoadWavFile("wav/bonus/bad.wav");
		sndGoodLuck = LoadWavFile("wav/bonus/good.wav");
	}
}

void C_BONUS_GAME::UnloadResources()
{
	if(bg != 0)
	{
		ReleaseWave(&sndGoodLuck);
		ReleaseWave(&sndBadLuck);
		ReleaseWave(&sndName);
		ReleaseSurface(&flashFrame);	
		ReleaseSurface(&awardFrame);
		ReleaseSurface(&rollFrame);
		ReleaseSurface(&picaward);
		ReleaseSurface(&happy);
		ReleaseSurface(&name);
		ReleaseSurface(&text);
		ReleaseSurface(&sad);
		ReleaseSurface(&bg);
	}
}

bool C_BONUS_GAME::Play()
{
	if(!sorted)
	{
		int i;
		vgSoundPlay(GAME::sndBonus,PDATA->Vol*255/100);
		vgSoundPlay(sndName,PDATA->Vol*255/100);
		for(i=0;i<MAX_PLAYERS;i++)
		{
			pls[i].num = i;
			pls[i].isman = (i%2 == 0);
			pls[i].p = PDATA->Players[i];
			pls[i].p.Score += pls[i].p.Increment;
			pls[i].p.Increment = 0;
		}
		std::partial_sort(pls,pls + 1,pls + 10,Greater);
		tmpKeys.clear();
		tmpKeys.insert(pls[0].num);
		sorted = true;
	}
	vgDrawSurface(bg,basex,basey,0);
	DisplayBorder();
	vgDrawSurface(name,basex + 158,basey + 40,0);
	if(stage == 0)
	{
		DisplayAward(currentx);
		vgDrawSurface(rollFrame,basex +  70,basey + 320,0);
		vgDrawSurface(text,     basex + 140,basey + 240,0);
		DisplayRank();
		DisplayPressAnimation(pls[0].num);
		currentx += rollSpeed;
		currentx %= picaward->w;
		if(!PDATA->InDemo)
		{
			if(PKEY->PressKey(PlayersEnter[pls[0].num]) || time >= 600)
			{
				if(time < 600)
				{
					vgSoundPlay(sndPress,PDATA->Vol*255/100);
				}
				++stage;
			}
		}
		else if(time >= 150)
		{
			++stage;
		}
		if(stage == 1)
		{
			n8 = (128 - currentx % 128)/16;
			n4 = n8*2;
			time = 0;
			tmpKeys.clear();
			return false;
		}
	}
	else if(stage == 1)
	{
		DisplayAward(currentx);
		vgDrawSurface(rollFrame,basex + 70, basey + 320,0);
		vgDrawSurface(text,     basex + 140,basey + 240,0);
		DisplayRank();
		if(n8 > 0)
		{
			currentx += 8;
			currentx %= picaward->w;
			--n8;
		}
		else if(n4 > 0)
		{
			currentx += 4;
			currentx %= picaward->w;
			--n4;
			if(n4 == 0)
			{
				++stage;
				time = 0;
				awardx = currentx;
				award = XToAward(awardx);
				if(Minutes[award] == 0)
				{
					vgSoundPlay(sndWine,PDATA->Vol*255/100);
				}
				else
				{
					vgSoundPlay(sndGoodLuck,PDATA->Vol*255/100);
				}
			}
		}
	}
	else
	{
		DisplayAward(awardx);
		vgDrawSurface(rollFrame,basex + 70,basey + 320,0);
		DisplayRank();
		if(time%30 < 15)
		{
			vgDrawSurface(counterF,10,30,0);
			vgDrawSurface(awardFrame,basex + 198,basey + 320,0);
		}
		if(award < 2)
		{
			vgDrawSurface(sad,basex + 160,basey + 240,0);
			if(time == 30)
			{
				vgSoundPlay(sndBadLuck,PDATA->Vol*255/100);
			}
		}
		else
		{
			vgDrawSurface(happy,basex + 160,basey + 240,0);
			if(time == 30)
			{
				vgSoundPlay(GAME::sndAddTime,PDATA->Vol*255/100);
				if(PDATA->InDemo)
				{
					*PDEMOTIME += Minutes[award]*60;
				}
				else
				{
					*PGAMETIME += Minutes[award]*60;
				}
			}
		}
		if(time >= 300)
		{
			currentx = awardx;
			time = 0;
			stage = 0;
			sorted = false;
			rollSpeed = 16;
			vgSoundStop(GAME::sndBonus);
			vgSoundStop(GAME::sndAddTime);
			return true;
		}
	}
	++time;
	return false;
}
	
void C_BONUS_GAME::DisplayAward(int sx)
{
	sx %= picaward->w;
	if(sx <= picaward->w - 384)
	{
		DrawPartOfTexture(picaward,sx,0,384,picaward->h,
			basex +  70,basey + 320);
	}
	else
	{
		DrawPartOfTexture(picaward,sx,0,(picaward->w - sx),picaward->h,
			basex + 70,basey + 320);
		DrawPartOfTexture(picaward,0,0,
			(384 - picaward->w + sx),
			picaward->h,
			basex + 70 + (picaward->w - sx),
			basey + 320);
	}
}

void C_BONUS_GAME::DisplayRank()
{
	if(pls[0].p.Index >= 0)
	{
		display_num_horizontal(nr,2,
			1,
			basex + 145,
			basey + 155,1);
		GAME::DisplayRoleName(pls[0].isman,
			pls[0].p.Index,1,
			basex + 180,
			basey + 150);
		if(pls[0].isman)
		{
			DrawPartOfSurface(man,1,6,pls[0].p.Index,
				basex + 245,
				basey + 150 - 15);
		}
		else
		{
			DrawPartOfSurface(girl,1,6,pls[0].p.Index,
				basex + 245,
				basey + 150 - 15);
		}
		display_num_horizontal(nr,1,
			pls[0].p.Score,
			basex + 305,
			basey + 150,1);
	}
	if(time%40 < 20)
	{
		vgDrawSurface(flashFrame,basex + 145,basey + 135,0);
	}
}

int C_BONUS_GAME::XToAward(int x)
{
	if(x < 128)
	{
		return 4;
	}
	else if(x < 256)
	{
		return 1;
	}
	else if(x < 384)
	{
		return 2;
	}
	else if(x < 512)
	{
		return 1;
	}
	else if(x < 640)
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

