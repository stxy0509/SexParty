#ifndef ROLE_TIP_H
#define ROLE_TIP_H
#include <EagleSDK.h>
namespace GAME
{
	class C_ROLE_TIP
	{
	private:
		static vgSURFACE *tip[4];
		static bool loaded;
		vgSURFACE *oneTip;
		int time;
		C_ROLE_TIP(const C_ROLE_TIP&);
		C_ROLE_TIP& operator=(const C_ROLE_TIP&);
	public:
		enum
		{
			MAN_JOIN = 0,
			GIRL_JOIN,
			MAN_QUIT,
			GIRL_QUIT
		};
		C_ROLE_TIP(int type);
		~C_ROLE_TIP();
		bool Display();
		static void LoadPictures();
		static void UnloadPictures();
	};
}
#endif
