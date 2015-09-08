#include "consumetime.h"
#include "game_common.h"
using GAME::needBonus;
C_CONSUME_TIME::C_CONSUME_TIME(int sec):
m_totalSeconds(sec),m_uptime(0),
m_start(false),m_pause(false)
{
	UpdateHMS();
}

C_CONSUME_TIME::~C_CONSUME_TIME()
{
	
}

bool C_CONSUME_TIME::TimeOver() const
{
	return m_totalSeconds == 0;
}

int C_CONSUME_TIME::GetHours() const
{
	return m_hours;
}

int C_CONSUME_TIME::GetMinutes() const
{
	return m_minutes;
}

int C_CONSUME_TIME::GetSeconds() const
{
	return m_seconds;
}

int C_CONSUME_TIME::GetTotalSeconds() const
{
	return m_totalSeconds;
}

void C_CONSUME_TIME::Start()
{
	m_start = true;
	m_pause = false;
}

void C_CONSUME_TIME::Pause()
{
	if(m_start)
	{
		m_pause = true;
	}
}

void C_CONSUME_TIME::Resume()
{
	if(m_pause)
	{
		m_pause = false;
	}
}

void C_CONSUME_TIME::SetTotalSeconds(int sec)
{
	m_totalSeconds = sec;
	m_start = true;
	m_pause = false;
	m_uptime = 0;
	UpdateHMS();
}

/* call this function in 1 second interrupt */
void C_CONSUME_TIME::CountTime()
{
	if(m_start && !m_pause)
	{
		if(m_totalSeconds > 0)
		{
			++m_uptime;
			--m_totalSeconds;
			UpdateHMS();
			if(!needBonus && Progress() == 100)
			{
				needBonus = true;
			}
		}
	}
}

void C_CONSUME_TIME::UpdateHMS()
{
	m_hours = m_totalSeconds/3600;
	m_minutes = m_totalSeconds%3600/60;
	m_seconds = m_totalSeconds%3600%60;
}

C_CONSUME_TIME C_CONSUME_TIME::operator+(int sec)
{
	C_CONSUME_TIME t(m_totalSeconds + sec);
	return t;
}
	
C_CONSUME_TIME& C_CONSUME_TIME::operator+=(int sec)
{
	if(sec != 0)
	{
		m_totalSeconds += sec;
		UpdateHMS();
	}
	return *this;
}

#define FIRST_BONUS  (20*60)
#define SECOND_BONUS (30*60)
int C_CONSUME_TIME::Progress()
{
	if(needBonus)
	{
		return 100;
	}
	if((m_uptime - FIRST_BONUS)%(SECOND_BONUS) == 0)
	{
		return 100;
	}
	else if(m_uptime < FIRST_BONUS)
	{
		return m_uptime*100/FIRST_BONUS;
	}
	else
	{
		return (m_uptime - FIRST_BONUS)%(SECOND_BONUS)*100/(SECOND_BONUS);
	}
}

