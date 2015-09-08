#include "downtimer.h"
#include <EagleSDK.h>
std::list<C_DOWN_TIMER *> C_DOWN_TIMER::DownTimer;
C_DOWN_TIMER::C_DOWN_TIMER(int value):
CountDown(false),InitValue(value),RemainedSeconds(value)
{
	
}

C_DOWN_TIMER::~C_DOWN_TIMER()
{
	DownTimer.remove(this);
}

void C_DOWN_TIMER::Start()
{
	pause = false;
	CountDown = true;
	RemainedSeconds = InitValue;
	Push(this);
}

void C_DOWN_TIMER::Stop()
{
	pause = false;
	CountDown = false;
	DownTimer.remove(this);
}

void C_DOWN_TIMER::Pause()
{
	pause = true;
}

void C_DOWN_TIMER::Resume()
{
	pause = false;
}

bool C_DOWN_TIMER::IsStart() const
{
	return CountDown;
}

void C_DOWN_TIMER::Reset()
{
	RemainedSeconds = InitValue;
}

void C_DOWN_TIMER::Push(C_DOWN_TIMER *atimer)
{
	if(atimer)
	{
		DownTimer.remove(atimer);
		DownTimer.push_back(atimer);
	}
}

void C_DOWN_TIMER::DecTime()
{
	if(CountDown && !pause && RemainedSeconds > 0)
	{
		--RemainedSeconds;
	}
}

C_DOWN_TIMER *C_DOWN_TIMER::GetTopTimer()
{
	return DownTimer.empty() ? 0 : DownTimer.back();
}

int C_DOWN_TIMER::GetRestTime() const
{
	return RemainedSeconds;
}

int C_DOWN_TIMER::GetElapsedTime() const
{
	return (InitValue - RemainedSeconds);
}

void C_DOWN_TIMER::SetNewTime(int value)
{
	if(InitValue != value)
	{
		InitValue = value;
	}
}
