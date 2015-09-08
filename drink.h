#ifndef DRINK_H_
#define DRINK_H_
#include <EagleSDK.h>
namespace GAME
{
	class C_DRINK
	{
	private:
		int which;
		int picNum;
		int amount;
		int soundt;
		int FrameNumber;
		vgWAVE *sndMusic;
		vgWAVE *sndFg;
		static bool Loaded;
		static vgSURFACE *text[2];
		static vgSURFACE *action[2];
		void LoadPictures();
		void UnloadPictures();
		C_DRINK(const C_DRINK&);
		C_DRINK& operator=(const C_DRINK&);
	public:
		C_DRINK();
		~C_DRINK();
		bool Play(int HowMany);
	};
}
#endif
