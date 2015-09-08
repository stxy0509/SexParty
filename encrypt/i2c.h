#ifndef _I2C_H_
#define _I2C_H_
#include <vgtypedef.h>
namespace ENCRYPT
{
	C_LINKAGE 
	{
		BOOL I2cSendByte(U8 c);
		U8   I2c_SendStr(U8 sla,U8 *s,char no);
		U8   I2c_RcvStr (U8 sla, U8 *s, int no);
		void twi_baudrate_set(U32 baud);
		void Delay_1(unsigned long cnt);
		void Reset8826();
		void I2c_Init();
	}
	#define twi_con			((volatile unsigned int*)0xFFE0ac00)
	#define twi_addr		((volatile unsigned int*)0xFFE0ac08)
	#define twi_stat		((volatile unsigned int*)0xFFE0ac04)
	#define twi_br0			((volatile unsigned int*)0xFFE0ac10)
	#define twi_br1			((volatile unsigned int*)0xFFE0ac14)
	#define twi_data		((volatile unsigned int*)0xFFE0ac0c)
	#define msta_start		(1 << 5)
	#define msta_stop		0xffffffdf
	#define repst_start		(1 << 2)
}
extern U8  BusSlvAdr;
#endif
