#include "key.h"
#include <libst.h>
#include <EagleSDK.h>
C_KEY::C_KEY():
Key(0),ValidPress(20),LongPress(500),LongPressInterval(150)
{
	int i;
	for(i=0;i<MAX_KEYS;i++)
	{
		KeyPressTime[i] = 0;
	}
}

C_KEY *C_KEY::GetSingleton()
{
	static C_KEY *PKEY = 0;
	return PKEY == 0 ? PKEY = new C_KEY() : PKEY;
}

bool C_KEY::IsLockKey(int bit)
{
	return std::find(LockKeys.begin(),LockKeys.end(),bit) != LockKeys.end();
}

bool C_KEY::PressKey(int bit)
{
	if(bit < 0 || bit >= MAX_KEYS) 
	{
		return false;
	}
	bool pressed = IS_BIT_SET(Key,bit);
	if(!IsLockKey(bit))
	{
		CLEAR_BIT(Key,bit);
	}
	return pressed;
}


void C_KEY::AddLongPressKeys(int n,...)
{
	int i,bit;
	va_list argp;
	va_start(argp,n);
	for(i=0;i<n;i++) 
	{
		bit = va_arg(argp,int);
		if(bit < 0 || bit >= MAX_KEYS)
		{
			continue;
		}
		LongPressKeys.push_back(bit);
	}
	va_end(argp);
}

void C_KEY::AddLockKeys(int n)
{
	if(n < 0 || n >= MAX_KEYS) 
	{
		return;
	}
	LockKeys.push_back(n);
}

void C_KEY::AddPressReleaseKeys(int n,...)
{
	int i,bit;
	va_list argp;

	va_start(argp,n);
	for(i=0;i<n;i++) 
	{
		bit = va_arg(argp,int);
		if(bit < 0 || bit >= MAX_KEYS)
		{
			continue;
		}
		PressReleaseKeys.push_back(bit);
	}
	va_end(argp);
}

/* 
 * For Eagle Boards ,this function must be called in
 * 1 ms Timer Interrupt Service Routine,otherwise,the 
 * class will not work correctly!
 */
void C_KEY::DealMsKeys()
{
	U32 k = getInputStatus();
	KEY_IT it;
	if(!LockKeys.empty()) 
	{
		it = LockKeys.begin();
		while(it != LockKeys.end()) 
		{
			if(!IS_BIT_SET(k,*it)) 	
			{
				SET_BIT(Key,*it);
			}
			else  
			{
				CLEAR_BIT(Key,*it);
			}
			++it;
		}
	}
	if(!PressReleaseKeys.empty())
	{
		it = PressReleaseKeys.begin();	
		while(it != PressReleaseKeys.end()) 
		{
			if(!IS_BIT_SET(k,*it)) 
			{
				++KeyPressTime[*it];
				if(KeyPressTime[*it] == ValidPress) 
				{
					SET_BIT(Key,*it);
				}
			} 
			else 
			{
				KeyPressTime[*it] = 0;
				CLEAR_BIT(Key,*it);
			}
			++it;
		}
	}
	if(!LongPressKeys.empty())
	{
		it = LongPressKeys.begin();
		while(it != LongPressKeys.end()) 
		{
			if(!IS_BIT_SET(k,*it)) 
			{
				++KeyPressTime[*it];
				if(KeyPressTime[*it] == ValidPress ||
				   KeyPressTime[*it] == LongPress) 
				{				
					SET_BIT(Key,*it);
				}
				else if(KeyPressTime[*it] == LongPress + LongPressInterval) 
				{
					KeyPressTime[*it] = LongPress;
					SET_BIT(Key,*it);
				}
			} 
			else 
			{
				KeyPressTime[*it] = 0;
				CLEAR_BIT(Key,*it);
			}
			++it;
		}
	}		
}

int C_KEY::PressTime(int n)
{
	return (n < 0 || n >= MAX_KEYS) ? 0 : KeyPressTime[n];
}

void C_KEY::ZeroPressTime(int n)
{
	if(n >= 0 && n < MAX_KEYS)
	{
		KeyPressTime[n] = 0;
	}
}
