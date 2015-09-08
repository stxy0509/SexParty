#ifndef DOWN_TIMER_H_
#define DOWN_TIMER_H_
#include <list>
class C_DOWN_TIMER 
{
private:
	bool CountDown;
	bool pause;
	int InitValue;
	int RemainedSeconds;
	C_DOWN_TIMER(const C_DOWN_TIMER&);
	C_DOWN_TIMER &operator=(const C_DOWN_TIMER&);
	static std::list<C_DOWN_TIMER *> DownTimer;
public:
	C_DOWN_TIMER(int value);
	~C_DOWN_TIMER();
	void Start();
	void Stop();
	void Pause();
	void Resume();
	void Reset();
	void DecTime();
	void SetNewTime(int value);
	bool IsStart() const;
	int GetRestTime() const;
	int GetElapsedTime() const;
	static void Push(C_DOWN_TIMER *atimer);
	static C_DOWN_TIMER *GetTopTimer();
};
#endif
