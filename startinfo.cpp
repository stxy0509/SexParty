#include "startinfo.h"
#include "game_common.h"
#include <libst.h>
using GAME::pls;
using GAME::PKEY;
using GAME::PDATA;
using GAME::EnterOfPlayer;
using GAME::man;
using GAME::nr;
using GAME::girl;
using GAME::keys;
using GAME::sndBegin;
using GAME::sndPress;
using GAME::SubGameStage;
using GAME::STAGE_END_GAME;
using GAME::STAGE_SELECT_ROLE;
using GAME::SubPickedRole;
using GAME::STAGE_NONE;
using GAME::MAX_PLAYERS;
using GAME::DisplayPressAnimation;
using GAME::IsInKeys;
U32 C_START_INFO::Max = 0;
int C_START_INFO::time = 0;
int C_START_INFO::stage = 0;
int C_START_INFO::gameStage = 0;
vgSURFACE *C_START_INFO::bg[2] = {0,};
vgSURFACE *C_START_INFO::logo[2] = {0,};
vgSURFACE *C_START_INFO::cup[3] = {0,};
vgSURFACE *C_START_INFO::bar = 0;
vgSURFACE *C_START_INFO::rankNr = 0;
vgSURFACE *C_START_INFO::tip = 0;
vgSURFACE *C_START_INFO::note = 0;
vgSURFACE *C_START_INFO::frame = 0;
vgSURFACE *C_START_INFO::frame_opt = 0;
vgSURFACE *C_START_INFO::fire = 0;
vgSURFACE *C_START_INFO::list = 0;
vgSURFACE *C_START_INFO::title = 0;
vgWAVE *C_START_INFO::sndGameOver = 0;
vgWAVE *C_START_INFO::sndWelcome = 0;
bool C_START_INFO::Loaded = false;
bool C_START_INFO::sorted = false;
void C_START_INFO::Init()
{
	if(!Loaded)
	{
		load_bmp_files("picture/sign/logo/",logo,2);
		Max = vgGetFreeTextureMemSize() + GetFreeSoundMemorySize();
		vgSetInterrupt(INTNUM_TIMER2,Start);
		vgSetTimer(2, 100);
		vgEnableInterrupt(INTNUM_TIMER2,true);
		
		load_bmp_files("picture/sign/",bg,2);
		nr = LoadImageFile("picture/nr.bmp",0,186,255);
		bar = vgLoadImageFile("picture/sign/start/bar.bmp");
		frame = LoadImageFile("picture/sign/start/frame.bmp",255,255,255);
		rankNr = LoadImageFile("picture/sign/end/nr.bmp",0,186,255);
		tip = vgLoadImageFile("picture/sign/end/tip.bmp");
		note = vgLoadImageFile("picture/sign/end/note.bmp");
		fire = vgLoadImageFile("picture/sign/end/fire.bmp");
		vgSetDrawMode(fire,DRAWMODE_ALPHA | DRAWMODE_TEXTURE);
		frame_opt = LoadImageFile("picture/sign/end/frame.bmp",54,54,54);
		cup[0] = LoadImageFile("picture/sign/end/gold.bmp",0,255,0);
		cup[1] = LoadImageFile("picture/sign/end/silver.bmp",0,255,0);
		cup[2] = LoadImageFile("picture/sign/end/copper.bmp",0,255,0);
		list = vgLoadImageFile("picture/sign/end/list.bmp");
		title = vgLoadImageFile("picture/sign/end/title.bmp");
		sndGameOver = LoadWavFile("wav/over.wav",true);
		sndWelcome = LoadWavFile("wav/welcome.wav");
		sndBegin = LoadWavFile("wav/begin.wav",true);
		vgSoundPlay(sndBegin,PDATA->Vol*255/200);
		Loaded = true;
	}
}

void C_START_INFO::Exit()
{
	U32 Min;
	Min = vgGetFreeTextureMemSize() + GetFreeSoundMemorySize();
	if(PDATA->MinResources != Min)
	{
		PDATA->MinResources = Min;
		PDATA->Write();
	}
	vgEnableInterrupt(INTNUM_TIMER2,false);
	ReleaseSurface(logo,2);
	if(PDATA->MainGameStage == STAGE_END_GAME)
	{
		vgSoundStop(sndBegin);
	}
}

void C_START_INFO::Start()
{
	U32 current = vgGetFreeTextureMemSize() + GetFreeSoundMemorySize();
	int i,percent = 100ULL*(Max - current)/(Max - PDATA->MinResources);
	if(percent > 100)
	{
		percent = 100;
	}
	if(percent < 20)
	{
		Draw1024X768Splitly(logo);
		vgASyncFlip();
	}
	else
	{
		extern const char *version;
		Draw1024X768Splitly(bg);
		vgDrawSurface(frame,289,605,0);
		DrawPartOfTexture(bar,0,0,percent*bar->w/100,bar->h,289,605);
		display_num_horizontal(nr,3,percent,482,670,1);
		for(i=0;i<4;i++)
		{
			display_num_horizontal(nr,1,version[i] - '0',950 + 12*i,750,1);
		}
		vgASyncFlip();
		static bool played = false;
		if(percent >= 50 && !played)
		{
			vgSoundPlay(sndWelcome,PDATA->Vol*255/100);
			played = true;
		}
	}
}

void C_START_INFO::End()
{
	if(!vgSoundIsPlaying(sndGameOver))
	{
		vgSoundPlay(sndGameOver,PDATA->Vol*255/100);
	}
	Draw1024X768Splitly(bg);
	if(stage == 0)
	{
		int opt = time/100%2;
		int framePos[2][2] = 
		{
			{37,104},{37,147}
		};
		int timePos[2][2] = 
		{
			{217,110},{217,153}
		};
		int firstMan = PDATA->ChiefMan();
		if(keys.size() != 1 || !IsInKeys(firstMan))
		{
			keys.clear();
			keys.insert(firstMan);
		}
		vgDrawSurface(note,384,256,0);
		vgDrawSurface(frame_opt,
			384 + framePos[opt][0],
			256 + framePos[opt][1],0);
		display_num_horizontal(nr,1,
			(118 - time%100)/20,
			384 + timePos[opt][0],
			256 + timePos[opt][1],0);
		DisplayPressAnimation(firstMan);
		if(PKEY->PressKey(EnterOfPlayer(firstMan)))
		{
			time = 0;
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
			if(opt == 0)
			{
				SubGameStage = GAME::STAGE_USER_FUNCTION;
				PDATA->MainGameStage = gameStage;
			}
			else
			{
				stage = 1;
			}
		}
	}
	else if(stage == 1)
	{
		if(!sorted)
		{
			int i;
			GAME::PGAMETIME->SetTotalSeconds(0);
			for(i=0;i<MAX_PLAYERS;i++)
			{
				pls[i].p = PDATA->Players[i];
				pls[i].num = i;
				pls[i].isman = (i%2 == 0);
				PDATA->DeleteRole(i);
			}
			std::sort(pls,pls + MAX_PLAYERS,Greater);
			sorted = true;
			keys.clear();
		}
		DisplayRank();
		DisplayFire();
		if(time >= 300)
		{
			time = 0;
			stage = 2;
			sorted = false;
		}
	}
	else
	{
		vgDrawSurface(tip,260,512,0);
		if(time >= 200 && SubGameStage == STAGE_NONE)
		{
			time = 0;
			stage = 0;
			sorted = false;
			GAME::needBonus = false;
			vgSoundStop(sndGameOver);
			PDATA->InDemo = true;
			PDATA->MainGameStage = STAGE_SELECT_ROLE;
			GAME::PDEMOTIME->SetTotalSeconds(3600);
			PDATA->Write();
		}
	}
	++time;
}

void C_START_INFO::Reset()
{
	time = 0;
	stage = 0;
	sorted = false;
	vgSoundStop(sndGameOver);
}

void C_START_INFO::DisplayRank()
{
	vgDrawSurface(title,330,20,0);
	vgDrawSurface(list, 330,84,0);
	vgDrawSurface(list, 330,284,0);
	vgDrawSurface(list, 330,484,0);
	for(int i=0;i<MAX_PLAYERS;i++)
	{
		if(pls[i].p.Index >= 0)
		{
			display_num_horizontal(rankNr,2,
				i + 1,330,82 + 60*i,1);
			GAME::DisplayRoleName(pls[i].isman,
				pls[i].p.Index,1,
				380,84 + 60*i);
			if(pls[i].isman)
			{
				DrawPartOfSurface(man,1,6,pls[i].p.Index,
					455,84 + 60*i - 15);
			}
			else
			{
				DrawPartOfSurface(girl,1,6,pls[i].p.Index,
					455,84 + 60*i - 15);
			}
			display_num_horizontal(rankNr,1,
				pls[i].p.Score,
				515,82 + 60*i,0);
			if(i < 3)
			{
				vgDrawSurface(cup[i],615,84 + 60*i,0);
			}
		}
	}
}

void C_START_INFO::DisplayFire()
{
	int idx1 = time/5%32;
	int idx2 = (idx1 + 8)%32;
	int idx3 = (idx2 + 8)%32;
	int idx4 = (idx3 + 8)%32;
	vgSetTexture(fire);
	vgSetAlpha(0,0,0,DGBLEND_SRCCOLOR,
			   0,0,0,DGBLEND_INVSRCCOLOR);
	vgDrawTexture(283,220,128,220,128*(idx1%8),220*(idx1/8),0);
	vgDrawTexture(481,134,128,220,128*(idx2%8),220*(idx2/8),0);
	vgDrawTexture(708,  4,128,220,128*(idx3%8),220*(idx3/8),0);
	vgDrawTexture(443,430,128,220,128*(idx4%8),220*(idx4/8),0);
}
