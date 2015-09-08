#ifndef KEY_H_
#define KEY_H_
#include <vector>
#include <algorithm>
class C_KEY
{
private:
	typedef std::vector<int>::iterator KEY_IT;
	static const int MAX_KEYS = 16;
	int Key;
	int KeyPressTime[MAX_KEYS];
	const int ValidPress;
	const int LongPress;
	const int LongPressInterval;
	std::vector<int> LongPressKeys;   //����ģʽ�İ���
	std::vector<int> PressReleaseKeys;//�����ͷ�ģʽ�İ���
	std::vector<int> LockKeys;        //��ģʽ����
	C_KEY();
public:
	void AddLockKeys(int n);
	void AddLongPressKeys(int n,...);
	void AddPressReleaseKeys(int n,...);
	void DealMsKeys();
	bool IsLockKey(int bit);
	bool PressKey(int n);
	int  PressTime(int n);
	void ZeroPressTime(int n);
	static C_KEY *GetSingleton();
};
#endif
