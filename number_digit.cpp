#include "number_digit.h"
#include <cstring>
#include <cstdlib>
#include "game_common.h"
using GAME::MAX_PLAYERS;
C_NUM_DIGIT::C_NUM_DIGIT():
ri(0),delta(0),buff()
{
	
}

C_NUM_DIGIT::C_NUM_DIGIT(int n):
ri(n),delta(0),buff()
{
	
}

C_NUM_DIGIT& C_NUM_DIGIT::operator+=(int x)
{
	delta += x;
	updateBuff();
	return *this;
}

C_NUM_DIGIT& C_NUM_DIGIT::operator-=(int x)
{
	delta -= x;
	updateBuff();
	return *this;
}

void C_NUM_DIGIT::setValue(int newV)
{
	ri = newV;
	delta = 0;
	memset(buff,0,sizeof(buff));
}

void C_NUM_DIGIT::updateBuff()
{
	int i,x = abs(delta);
	for(i=0;i<10;i++)
	{
		buff[i] = x%10;
		x /= 10;
	}
}

void C_NUM_DIGIT::process()
{
	if(delta)
	{
		int i,base = 1;
		for(i=0;i<MAX_PLAYERS;i++)
		{
			if(buff[i] == 0)
			{
				base *= 10;
				continue;
			}
			else
			{
				--buff[i];
				if(delta > 0)	
				{
					ri += base;
					delta -= base;
				}
				else
				{
					ri -= base;
					delta += base;
				}
				break;
			}
		}
	}
}

bool C_NUM_DIGIT::isBuffZero()
{
	return delta == 0;
}
