#include "select_role.h"
#include "game_common.h"
#include "downtimer.h"
#include <libst.h>
using GAME::C_SELECT_ROLE;
int C_SELECT_ROLE::StandManPos[MAX_ROLES][2] = 
{
	{133,183},{251,185},{387,185},
	{485,191},{621,185},{733,171}
};
int C_SELECT_ROLE::StandGirlPos[MAX_ROLES][2] = 
{
	{123,171},{287,170},{400,170},
	{520,170},{627,170},{776,170}
};
int C_SELECT_ROLE::RoleNamePos[MAX_ROLES][2] = 
{
	{161,520},{300,520},{419,520},{532,520},{650,520},{788,520}
};
C_SELECT_ROLE::C_SELECT_ROLE():
sndPlayed(false),ResourceLoaded(false),FrameNumber(0),
Target(0),Timer(new C_DOWN_TIMER(PDATA->SelectRoleTime))
{
	LoadResources();
}

C_SELECT_ROLE::~C_SELECT_ROLE()
{
	UnloadResources();
}

void C_SELECT_ROLE::LoadResources()
{
	if(!ResourceLoaded)
	{
		load_bmp_files("picture/select_role/",bg,2);
		allman = vgLoadImageFile("picture/select_role/allman.bmp");
		allgirl = vgLoadImageFile("picture/select_role/allgirl.bmp");
		sndSelect = LoadWavFile("wav/role/select.wav");
		ResourceLoaded = true;
	}
}

void C_SELECT_ROLE::UnloadResources()
{
	if(ResourceLoaded)
	{
		ReleaseWave(&sndSelect);
		ReleaseSurface(&allgirl);
		ReleaseSurface(&allman);
		ReleaseSurface(bg,2);
		ResourceLoaded = false;
	}
}

C_SELECT_ROLE * C_SELECT_ROLE::GetSingleton()
{
	static C_SELECT_ROLE *PSELECT_ROLE = 0;
	return PSELECT_ROLE == 0 ? PSELECT_ROLE = new C_SELECT_ROLE() : PSELECT_ROLE;
}

void C_SELECT_ROLE::ShowSelectedRole()
{
	if(!sndPlayed)
	{
		vgSoundPlay(sndSelect,PDATA->Vol*255/100);		
		sndPlayed = true;
	}
	if(!vgSoundIsPlaying(sndBegin))
	{
		vgSoundPlay(sndBegin,PDATA->Vol*255/250);
	}
	Draw1024X768Splitly(bg);
	DisplayGameTime(10,30);
	if(Target == 0)
	{
		ShowSelectedMan();
	}
	else
	{
		ShowSelectedGirl();
	}
	GAME::ShowSitRole();
	ShowRemainedTime();
	DealSelectNormal();
	DisplaySitRoleName();
	if(SubGameStage == STAGE_NONE)
	{
		++FrameNumber;
	}
}

void C_SELECT_ROLE::Reset()
{
	Timer->Stop();
	Target = 0;
	FrameNumber = 0;
	vgSoundStop(sndSelect);
	vgSoundStop(sndBegin);
	sndPlayed = false;
}

void C_SELECT_ROLE::DealSelectNormal()
{
	int i,which;
	int nRoles = (Target == 0) ? PDATA->NumberOfMans() : PDATA->NumberOfGirls();
	if(!Timer->IsStart())
	{
		Timer->Start();
		keys.clear();
		if(Target == 0)
		{
			keys.insert(0);
			keys.insert(2);
			keys.insert(4);
			keys.insert(6);
			keys.insert(8);
			
		}
		else
		{
			keys.insert(1);
			keys.insert(3);
			keys.insert(5);
			keys.insert(7);
			keys.insert(9);
		}
	}
	if(SubGameStage == STAGE_NONE)
	{
		Timer->Resume();
	}
	else
	{
		Timer->Pause();
	}
	if(Timer->GetRestTime() > 0 && nRoles < MAX_PLAYERS/2)
	{
		if(!PDATA->InDemo)
		{
			for(i=Target;i<MAX_PLAYERS;i+=2)
			{
				DisplayPressAnimation(i);
				if(!PDATA->SeatHaveRole(i) && PKEY->PressKey(EnterOfPlayer(i)))
				{
					which = (Target == 0) ? GetFreeMan() : GetFreeGirl();
					vgSoundPlay(sndPress,PDATA->Vol*255/100);
					PDATA->Players[i].Index = which;
					AddPickedRole(which,i%2 == 0);
				}
			}
		}
		else
		{
			if(SubGameStage == STAGE_NONE)
			{
				int r = rand() % 150;
				if(r < 2)
				{
					for(i=Target;i<MAX_PLAYERS;i+=2)
					{
						if(!PDATA->SeatHaveRole(i))
						{
							which = (Target == 0) ? GetFreeMan() : GetFreeGirl();
							PDATA->Players[i].Index = which;
							AddPickedRole(which,i%2 == 0);
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		FrameNumber = 0;
		Target = 1 - Target;
		Timer->Stop();
		int nr = PDATA->InDemo ? 2 : 1;
		if(PDATA->NumberOfGirls() >= nr && 
		   PDATA->NumberOfMans()  >= nr)
		{
			Target = 0;
			sndPlayed = false;
			PDATA->MainGameStage = STAGE_SELECT_GAME;
		}
	}
}

void C_SELECT_ROLE::ShowSelectedMan()
{
	LIST_IT it;
	vgDrawSurface(allman,122,171,0);
	
	it = freeMan.begin();
	while(it != freeMan.end())
	{
		DisplaySelectRoleName(true,*it);
		++it;
	}
}

void C_SELECT_ROLE::ShowSelectedGirl()
{
	LIST_IT it;
	vgDrawSurface(allgirl,122,171,0);
	
	it = freeGirl.begin();
	while(it != freeGirl.end())
	{
		DisplaySelectRoleName(false,*it);
		++it;
	}
}

void C_SELECT_ROLE::DisplaySelectRoleName(bool isMan,int index)
{
	DisplayRoleName(isMan,index,1,
		RoleNamePos[index][0],
		RoleNamePos[index][1]);
	DisplayRoleName(isMan,index,3,
		RoleNamePos[index][0],
		RoleNamePos[index][1] + 30);
}
