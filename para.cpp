#include "para.h"
#include "game_common.h"
#include <libst.h>
using GAME::C_PARA;
using GAME::PDATA;
using GAME::PKEY;
int C_PARA::ArrowPos[10][2] = 
{
	{280,195},{280,243},{280,286},{280,330},{280,377},
	{280,421},{280,468},{280,520},{280,565},{280,612}
};
int C_PARA::NumberPos[8][2] = 
{
	{580,196},{580,244},{580,288},{580,332},
	{580,378},{580,422},{580,468},{722,513},
};
int C_PARA::KeyPos[16][2] = 
{
	{  0,  0},{650,396},{440,574},{440,519},
	{440,279},{440,339},{  0,  0},{440,214},
	{650,456},{  0,  0},{440,396},{440,456},
	{650,519},{650,339},{650,279},{650,214},
};
C_PARA::C_PARA():
Loaded(false),CurrentItem(HARDWARE_TEST)
{
	GetOldValue();
	LoadResources();
}

C_PARA::~C_PARA()
{
	UnloadResources();
}

void C_PARA::LoadResources()
{
	if(!Loaded)
	{
		load_bmp_files("picture/para/bg/",bg,2);
		arrow = vgLoadImageFile("picture/para/arrow.bmp");
		box = vgLoadImageFile("picture/para/box.bmp");
		select = vgLoadImageFile("picture/para/select.bmp");
		nr = vgLoadImageFile("picture/para/nr.bmp");
		test = vgLoadImageFile("picture/para/test.bmp");
		Loaded = true;
	}
}

void C_PARA::UnloadResources()
{
	if(Loaded)
	{
		ReleaseSurface(&test);
		ReleaseSurface(&nr);
		ReleaseSurface(&arrow);
		ReleaseSurface(&box);
		ReleaseSurface(&select);
		ReleaseSurface(bg,2);
		Loaded = false;
	}
}

bool C_PARA::Process()
{
	if(!para[HARDWARE_TEST])
	{
		para[LINE_NUMBER]    = PDATA->LineNumber;
		para[MACHINE_NUMBER] = PDATA->MachineNumber;
		Draw1024X768Splitly(bg);
		DisplayArrow();	
	}
	else
	{
		vgClearScreen(54,54,54);
		vgDrawSurface(test,256,128,0);
	}
	DisplayParaValue();
	return DealKeyPress();
}

void C_PARA::DisplayArrow()
{
	vgDrawSurface(arrow,
		ArrowPos[CurrentItem][0],
		ArrowPos[CurrentItem][1],0);
}

void C_PARA::DisplayParaValue()
{
	int i,j;
	static int prevPress[MAX_PLAYERS] = {0,};
	if(para[HARDWARE_TEST])
	{
		for(i=0;i<13;i++)
		{
			if(i < 10)
			{
				j = PlayersEnter[i];
				if(PKEY->PressTime(j) > 0)
				{
					if(prevPress[i] == 0)
					{
						GAME::ToggleLed(i);
						prevPress[i] = 1;
					}
					vgDrawSurface(box,KeyPos[j][0],KeyPos[j][1],0);
				}
				else
				{
					prevPress[i] = 0;
				}
			}
			else if(i == 10)
			{
				if(PKEY->PressTime(KEY_FUNCTION) > 0)
				{
					vgDrawSurface(box,
						KeyPos[KEY_FUNCTION][0],
						KeyPos[KEY_FUNCTION][1],0);
				}
			}
			else if(i == 11)
			{
				if(PKEY->PressTime(KEY_RIGHT) > 0)
				{
					vgDrawSurface(box,KeyPos[KEY_RIGHT][0],KeyPos[KEY_RIGHT][1],0);
				}
			}
			else if(i == 12)
			{
				if(PKEY->PressKey(KEY_LOCK))
				{
					vgDrawSurface(box,KeyPos[KEY_LOCK][0],KeyPos[KEY_LOCK][1],0);
				}
			}
		}
	}
	else
	{
		for(i = LINE_NUMBER;i < LOAD_DEFAULTS;i++)
		{
			if(i != HARDWARE_TEST)
			{
				display_num_horizontal(nr,8,para[i],
					NumberPos[i][0],
					NumberPos[i][1],2);
			}
		}
		if(para[LOAD_DEFAULTS])
		{
			vgDrawSurface(select,
				NumberPos[LOAD_DEFAULTS][0],
				NumberPos[LOAD_DEFAULTS][1],0);
		}
	}
}

bool C_PARA::DealKeyPress()
{
	using GAME::PKEY;
	if( para[HARDWARE_TEST] &&
		PKEY->PressTime(KEY_FUNCTION) > 80)
	{
		PKEY->ZeroPressTime(KEY_RIGHT);
		para[HARDWARE_TEST] = 0;
		return false;
	}
	if(para[HARDWARE_TEST])
	{
		return false;
	}
	if(PKEY->PressKey(KEY_DOWN))
	{
		vgSoundPlay(sndPress,PDATA->Vol*255/100);
		++CurrentItem;
		if(CurrentItem > DISCARD_AND_EXIT)
		{
			CurrentItem = HARDWARE_TEST;
		}
	}
	else if(PKEY->PressKey(KEY_RIGHT))
	{
		vgSoundPlay(sndPress,PDATA->Vol*255/100);
		switch(CurrentItem)
		{
			case HARDWARE_TEST:
			{
				para[CurrentItem] = 1;
				break;
			}
			case FIX_PLAY:
			{
				para[CurrentItem] += 5;
				if(para[CurrentItem] > 30)
				{
					para[CurrentItem] = 10;
				}
				break;
			}
			case EXTRA_PLAY:
			{
				para[CurrentItem] += 20;
				if(para[CurrentItem] > 240)
				{
					para[CurrentItem] = 120;
				}
				break;
			}
			case BASEBALL_PLAY_TIME:
			{
				para[CurrentItem] += 10;
				if(para[CurrentItem] > 120)
				{
					para[CurrentItem] = 20;
				}
				break;
			}
			case SELECT_ROLE_TIME:
			{
				++para[CurrentItem];
				if(para[CurrentItem] > 15)
				{
					para[CurrentItem] = 5;
				}
				break;
			}
			case LOAD_DEFAULTS:
			{
				para[CurrentItem] = !para[CurrentItem];
				if(para[CurrentItem])
				{
					para[FIX_PLAY] = 20;
					para[EXTRA_PLAY] = 180;
					para[SELECT_ROLE_TIME] = 15;
					para[BASEBALL_PLAY_TIME] = 30;
				}
				break;
			}
		}
	}
	else if(PKEY->PressKey(KEY_ENTER))
	{
		vgSoundPlay(sndPress,PDATA->Vol*255/100);
		switch(CurrentItem)
		{
			case HARDWARE_TEST:
				para[CurrentItem] = 1;
				break;
			case SAVE_AND_EXIT:
				DoSave();
			case DISCARD_AND_EXIT:
				para[LOAD_DEFAULTS] = false;
				return true;
		}
	}
	return false;
}

void C_PARA::GetOldValue()
{
	para[LINE_NUMBER]        = PDATA->LineNumber;
	para[MACHINE_NUMBER]     = PDATA->MachineNumber;
	para[HARDWARE_TEST]      = 0;
	para[FIX_PLAY]           = PDATA->FixPlayTime;
	para[EXTRA_PLAY]         = PDATA->ExtraPlayTime;
	para[SELECT_ROLE_TIME]   = PDATA->SelectRoleTime;
	para[BASEBALL_PLAY_TIME] = PDATA->BaseBallTime;
	para[LOAD_DEFAULTS]      = false;
}

void C_PARA::DoSave()
{
	PDATA->FixPlayTime     = para[FIX_PLAY];             
	PDATA->ExtraPlayTime   = para[EXTRA_PLAY];             
	PDATA->SelectRoleTime  = para[SELECT_ROLE_TIME];        
	PDATA->BaseBallTime    = para[BASEBALL_PLAY_TIME];
	PDATA->Write();
}
