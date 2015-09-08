#ifndef NUMBER_DIGIT_H
#define NUMBER_DIGIT_H
class C_NUM_DIGIT
{
private:
	int ri;
	int delta;
	signed char buff[10];
	void updateBuff();
public:
	C_NUM_DIGIT();
	C_NUM_DIGIT(int n);
	C_NUM_DIGIT& operator+=(int x);
	C_NUM_DIGIT& operator-=(int x);
	void process();
	void setValue(int newV);
	bool isBuffZero();
	 int getValue()
	{
		return ri;
	}
};
#endif
