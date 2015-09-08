#ifndef LOCK_H
#define LOCK_H
#include <EagleSDK.h>
class C_LOCK
{
public:
	C_LOCK();
	~C_LOCK();
	bool Process();
	static void LoadResources();
	static void UnloadResources();
private:
	enum 
	{
		STAGE_INPUT = 0,
		STAGE_PASS_WRONG,
		STAGE_PASS_RIGHT,
		MAX_SYSTEM_PARA = 99000000
	};
	int digit;
	int pos;
	int time;
	int chkcode;
	int stage;
	int pass[8];
	int passdx;
	int basex;
	int basey;
	int timeIndex;
	U32 TempCheck[6];
	U32 TruePassword;
	bool computed;
	int CheckPassword();
	void ComputePassword();
	void DisplayValues();
	void InputPassword();
	static const U32 LockFlag = 20131202;
	static int NumberPos[6][2];
	static int FramePos[10][2];
	static vgSURFACE *table;
	static vgSURFACE *line;
	static vgSURFACE *frame;
	static vgSURFACE *error;
	static vgSURFACE *quit;
	static vgSURFACE *helpMsg;
};
#endif
