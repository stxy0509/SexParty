#ifndef UP_TIMER_H_
#define UP_TIMER_H_
#include <list>
class C_UP_TIMER 
{
private:
	typedef std::list<C_UP_TIMER *>::iterator TIMER_IT;
	bool StartCount;
	bool pause;
	int  ElapsedTime;
	const int MaxTime;
	C_UP_TIMER(const C_UP_TIMER&);
	C_UP_TIMER &operator=(const C_UP_TIMER&);
	void IncTime();
	static std::list<C_UP_TIMER *> UpTimer;
public:
	C_UP_TIMER(int value);
	~C_UP_TIMER();
	void Start();
	void Stop();
	void Pause();
	void Resume();
	bool IsStart() const;
	int  GetElapsedTime() const;
	static void AddTimer(C_UP_TIMER *pTimer);
	static void DelTimer(C_UP_TIMER *pTimer);
	static void DealAllTimer();
};
#endif
