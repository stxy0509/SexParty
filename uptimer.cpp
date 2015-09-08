#include "uptimer.h"
#include "game_common.h"
using GAME::PDATA;
std::list<C_UP_TIMER *> C_UP_TIMER::UpTimer;
C_UP_TIMER::C_UP_TIMER(int value):
StartCount(false),pause(false),
ElapsedTime(0),MaxTime(value)
{
	
}

C_UP_TIMER::~C_UP_TIMER()
{
	UpTimer.remove(this);
}

void C_UP_TIMER::Start()
{
	pause = false;
	StartCount = true;
	ElapsedTime = 0;
}

void C_UP_TIMER::Pause()
{
	pause = true;
}

void C_UP_TIMER::Resume()
{
	pause = false;
}

void C_UP_TIMER::Stop()
{
	StartCount = false;
	ElapsedTime = 0;
	pause = false;
}

void C_UP_TIMER::IncTime()
{
	if(StartCount && !pause && ElapsedTime < MaxTime)
	{
		++ElapsedTime;
	}
}

bool C_UP_TIMER::IsStart() const
{
	return StartCount;
}

int C_UP_TIMER::GetElapsedTime() const
{
	return ElapsedTime;
}

void C_UP_TIMER::AddTimer(C_UP_TIMER *pTimer)
{
	if(pTimer)
	{
		UpTimer.remove(pTimer);
		UpTimer.push_back(pTimer);
	}
}

void C_UP_TIMER::DelTimer(C_UP_TIMER *pTimer)
{
	UpTimer.remove(pTimer);
}

void C_UP_TIMER::DealAllTimer()
{
	TIMER_IT it = UpTimer.begin();
	while(it != UpTimer.end())
	{
		(*it)->IncTime();
		++it;
	}
}

