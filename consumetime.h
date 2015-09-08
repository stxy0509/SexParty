#ifndef CONSUME_TIME
#define CONSUME_TIME
class C_CONSUME_TIME
{
private:
	int m_hours;
	int m_minutes;
	int m_seconds;
	int m_totalSeconds;
	int m_uptime;
	bool m_start;
	bool m_pause;
	void UpdateHMS();
public:
	C_CONSUME_TIME(int sec = 0);
	~C_CONSUME_TIME();
	int GetHours() const;
	int GetMinutes() const;
	int GetSeconds() const;
	int GetTotalSeconds() const;
	int Progress();
	bool TimeOver()const;
	void Start();
	void Pause();
	void Resume();
	void CountTime();
	void SetTotalSeconds(int sec);
	C_CONSUME_TIME operator+(int sec);
	C_CONSUME_TIME& operator+=(int sec);
};
#endif



