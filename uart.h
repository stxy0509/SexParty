#ifndef _C_UART_H_
#define _C_UART_H_
#include <libst.h>
class C_UART 
{
private:
	enum REG_FLAG
	{
		HAVE_DATA = 1,
		ERROR_OVERRUN = 2,
		ERROR_PARITY = 4,
		ERROR_FRAME = 8,
		ERROR_BREAK = 16,
		THR_EMPTY = 32,
		FIFO_EMPTY = 64,
		ERROR_FIFO = 128,
	};
	typedef volatile unsigned int* PVUI;
	void EnableReceData() const;
	void EnableTranData() const;
	void CoolDownDelay() const;
	void WaitForDataCome();
	bool IsvalidBitRate(int rate) const;
	U8  *ReceBuff;
	int  BitRate;
	int  ReceBuffLen;
	int  ReceDataLen;
	U8   ReceHeadFlag;
	U8   ReceEndFlag;
	U8   Channel;
	bool DataReady;
	bool FullDuplex;
	bool HaveHeadFlag;
	bool HaveEndFlag;
	bool Start;
	bool HaveConfigured;
	
	static PVUI ULSR[2];
	static PVUI UIE[2];
	static PVUI URBR[2];
	static PVUI UTHR[2];
	static const int UartIntNumber[2];
	static const int MinBuffLen = 32;
	static const int FifoLength = 16;
	static const int TrigLength = 8;
protected:
	C_UART(int ch);
	int CurrPos;
public:
	virtual ~C_UART();
	U8   GetByte(unsigned int index);
	void ReceiveDataPassively();
	void ReceiveDataActively(int len);
	void TransData(U8 *data,int n) const;
	void SetReceHeadFlag(U8 flag);
	void SetReceEndFlag(U8 flag);
	void SetReceBuffLen(int len);
	void DealReceivedData();
	void Configure(int rate,void (*p)());
	void SetReceDataLen(int len);
	virtual void ReplyOrUse() = 0;
	virtual bool IsReceivedDataValid() = 0;
};
#endif
