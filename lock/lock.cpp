#include "lock.h"
#include <libst.h>
#include "../game_common.h"
using GAME::upArrow;
using GAME::lednr;
using GAME::PKEY;
using GAME::PDATA;
using GAME::screenw;
using GAME::screenh;
using GAME::sndPress;
vgSURFACE* C_LOCK::table = 0;
vgSURFACE* C_LOCK::line  = 0;  
vgSURFACE* C_LOCK::frame = 0; 
vgSURFACE* C_LOCK::quit = 0;
vgSURFACE* C_LOCK::error = 0;
vgSURFACE* C_LOCK::helpMsg = 0;
int C_LOCK::NumberPos[6][2] = 
{
	{145, 43},{240,101},{205,152},
	{205,207},{175,268},{167,330}
};
int C_LOCK::FramePos[10][2] = 
{
	{142,380},{168,380},{194,380},{220,380},{246,380},
	{272,380},{298,380},{324,380},{350,380},{376,380},
};

C_LOCK::C_LOCK():
digit(0),pos(0),time(0),chkcode(0),
stage(STAGE_INPUT),timeIndex(0),
computed(false)
{
	LoadResources();
	memset(pass,0,sizeof(pass));
	basex = (screenw - table->w)/2;
	basey = (screenh - table->h)/2;
	passdx = lednr->w/10;
}

C_LOCK::~C_LOCK()
{
	UnloadResources();
}

void C_LOCK::LoadResources()
{
	if(table == 0)
	{
		table = vgLoadImageFile("picture/lock/table.bmp");
		line = vgLoadImageFile("picture/lock/line.bmp");
		error = vgLoadImageFile("picture/lock/error.bmp");
		frame = LoadImageFile("picture/lock/frame.bmp",0,0,255);
		helpMsg = vgLoadImageFile("picture/keyhelp/lock/1.bmp");
		quit = LoadImageFile("picture/keyhelp/lock/2.bmp",0,0,255);
	}
}

void C_LOCK::UnloadResources()
{
	if(table != 0)
	{
		ReleaseSurface(&quit);
		ReleaseSurface(&helpMsg);
		ReleaseSurface(&frame);
		ReleaseSurface(&error);
		ReleaseSurface(&line);
		ReleaseSurface(&table);
	}
}

bool C_LOCK::Process()
{
	ComputePassword();
	vgClearScreen(80,80,80);
	vgDrawSurface(helpMsg,0,0,0);
	if(!PDATA->MustDecode1())
	{
		vgDrawSurface(upArrow,578, 0,0);
		vgDrawSurface(quit,   546,28,0);
	}
	vgDrawSurface(table,basex,basey,0);
	DisplayValues();
	if(stage == STAGE_INPUT)
	{
		InputPassword();
		if(!PDATA->MustDecode1())
		{
			if(PKEY->PressKey(GAME::KEY_ENTER8))
			{
				pos = 0;
				time = 0;
				computed = false;
				return true;
			}
		}
	}
	else if(stage == STAGE_PASS_WRONG)
	{
		vgDrawSurface(error,
			basex + 167,
			basey + 330,0);
		++time;
		if(time == 100)
		{
			pos = 0;
			time = 0;
			stage = STAGE_INPUT;
		}
	}
	else
	{
		pos = 0;
		time = 0;
		computed = false;
		stage = STAGE_INPUT;
		++PDATA->LockCount;
		PDATA->TotalIndex = 0;
		PDATA->TotalCharge = 0;
		PDATA->CurrentIndex = 0;
		PDATA->CurrentTotal = 0;
		if(timeIndex >= 2000)
		{
			PDATA->MachineNumber %= 100000;
			PDATA->MachineNumber += (timeIndex%1000)*100000;
		}
		else if(timeIndex >= 1000)
		{
			PDATA->LineNumber = (timeIndex%1000);
		}
		else if(timeIndex == 12)
		{
			PDATA->ForSale = 1;
		}
		else if(timeIndex == 11)
		{
			PDATA->ForSale = 0;
		}
		else if(timeIndex > 0 && timeIndex <= 10)
		{
			int lockTime[] = 
			{
				20,30,40,50,60,80,100,120,150,180
			};
			PDATA->LockCharge = lockTime[timeIndex - 1]*60;
		}
		memset(PDATA->CurrentCharge,0,sizeof(PDATA->CurrentCharge));
		memset(PDATA->HistoryCharge,0,sizeof(PDATA->HistoryCharge));
		PDATA->Write();
		return true;
	}
	return false;
}

void C_LOCK::DisplayValues()
{
	int i;
	display_num_horizontal(lednr,1,
		PDATA->LineNumber,
		basex + NumberPos[0][0],
		basey + NumberPos[0][1],1);
	display_num_horizontal(lednr,1,
		PDATA->TotalCharge,
		basex + NumberPos[1][0],
		basey + NumberPos[1][1],1);
	display_num_horizontal(lednr,1,
		PDATA->MachineNumber,
		basex + NumberPos[2][0],
		basey + NumberPos[2][1],1);
	display_num_horizontal(lednr,1,
		PDATA->LockCount,
		basex + NumberPos[3][0],
		basey + NumberPos[3][1],1);
	display_num_horizontal(lednr,1,
		chkcode,
		basex + NumberPos[4][0],
		basey + NumberPos[4][1],1);
	for(i=0;i<pos;i++)
	{
		display_num_horizontal(lednr,
			1,pass[i],
			basex + NumberPos[5][0] + i*passdx,
			basey + NumberPos[5][1],1);
	}
	if(pos < 8)
	{
		vgDrawSurface(line,
			basex + NumberPos[5][0] + pos*passdx,
			basey + NumberPos[5][1] + lednr->h,
			0);
	}
	else
	{
		vgDrawSurface(line,
			basex + NumberPos[5][0] + 7*passdx,
			basey + NumberPos[5][1] + lednr->h,
			0);
	}
	vgDrawSurface(frame,
		basex + FramePos[digit][0],
		basey + FramePos[digit][1],0);	
}

void C_LOCK::InputPassword()
{
	if(pos < 8)
	{
		if(PKEY->PressKey(GAME::KEY_FUNCTION))
		{
			--digit;
			if(digit < 0)
			{
				digit = 9;
			}
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
		}
		if(PKEY->PressKey(GAME::KEY_RIGHT))
		{
			++digit;
			if(digit > 9)
			{
				digit = 0;
			}
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
		}
		if(PKEY->PressKey(GAME::KEY_ENTER))
		{
			pass[pos] = digit;
			++pos;
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
		}
	}
	else
	{
		if(PKEY->PressKey(GAME::KEY_ENTER))
		{
			vgSoundPlay(sndPress,PDATA->Vol*255/100);
			timeIndex = CheckPassword();
			if(timeIndex >= 0)
			{
				stage = STAGE_PASS_RIGHT;
			}
			else
			{
				stage = STAGE_PASS_WRONG;
			}
		}
	}
}

void C_LOCK::ComputePassword()
{
	if(computed) return;
    U32 temp;
	
	temp = abs(PDATA->TotalCharge);
	TempCheck[0] = temp << 5;
	if(PDATA->TotalCharge < 0)
	{
		TempCheck[0] >>= 1;
	}
	
	temp = abs(PDATA->TotalCharge);
	TempCheck[1] = temp << (GetNumberOfOneBit(&temp,4)%5);
	if(PDATA->TotalCharge < 0)
	{
		TempCheck[1] >>= 1;
	}
	
	temp = PDATA->MachineNumber;
	TempCheck[2] = temp << (GetNumberOfOneBit(&temp,4)%5);
	
	temp = PDATA->LineNumber;
	TempCheck[3] = temp << (GetNumberOfOneBit(&temp,4)%5);
	
	temp = PDATA->LockCount;
	TempCheck[4] = (temp + LockFlag) << (GetNumberOfOneBit(&temp,4)%5);
	
	TempCheck[5]  = TempCheck[4] ^ TempCheck[0];
	TempCheck[5] ^= TempCheck[1];
	TempCheck[5] ^= TempCheck[2];
	TempCheck[5] ^= TempCheck[3];
	
	chkcode = ((TempCheck[5] >> 16) ^ (TempCheck[5] & 0xffff))%10000;
	TruePassword = TempCheck[5] % MAX_SYSTEM_PARA;
}

int C_LOCK::CheckPassword()
{
	int n,r;
	U32 i,InputPass = 0,base = 10000000;
	
	for(i=0;i<8;i++)
	{
		InputPass += pass[i]*base;
		base /= 10;
	}
	if(InputPass >= TruePassword)
	{
		r = (InputPass - TruePassword)%chkcode;
		n = (InputPass - TruePassword)/chkcode;
		if(r != 0 || n >= 13)
		{
			return -1;
		}
		else
		{			
			return n;
		}
	}
	else if(InputPass > TruePassword - chkcode - 2000 &&
			InputPass <= TruePassword - chkcode)
	{
		return 1000 + (TruePassword - chkcode - InputPass);
	}
	else
	{
		return -1;
	}
}


