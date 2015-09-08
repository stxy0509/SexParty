#include "uart.h"
#include <stdexcept>
const int C_UART::UartIntNumber[2] = 
{	
	INTNUM_UART0,INTNUM_UART1
};
//中断使能寄存器	
C_UART::PVUI C_UART::UIE[2] = 
{
	(PVUI)0xffe08004,(PVUI)0xffe08024
};
//状态寄存器
C_UART::PVUI C_UART::ULSR[2] = 
{
	(PVUI)0xffe08014,(PVUI)0xffe08034
};
//接收数据寄存器
C_UART::PVUI C_UART::URBR[2] = 
{
	(PVUI)0xffe08000,(PVUI)0xffe08020
};
//发送数据寄存器
C_UART::PVUI C_UART::UTHR[2] = 
{
	(PVUI)0xffe08000,(PVUI)0xffe08020
};

C_UART::C_UART(int ch):
ReceBuffLen(MinBuffLen),ReceDataLen(1),Channel(ch),
DataReady(false),HaveHeadFlag(false),HaveEndFlag(false),
Start(false),HaveConfigured(false),CurrPos(0)
{
	ReceBuff = new U8[MinBuffLen];
	memset(ReceBuff,0,ReceBuffLen);
}

C_UART::~C_UART()
{
	delete[] ReceBuff;
}

void C_UART::SetReceHeadFlag(U8 flag)
{
	ReceHeadFlag = flag;
	HaveHeadFlag = true;
}

void C_UART::SetReceEndFlag(U8 flag)
{
	ReceEndFlag = flag;
	HaveEndFlag = true;
}

void C_UART::SetReceDataLen(int len)
{
	if(len < 1) 
	{
		return;
	}
	ReceDataLen = len;
}

void C_UART::Configure(int rate,void (*p)())
{
	if(!IsvalidBitRate(rate) || p == NULL || HaveConfigured) 
	{
		return;
	}
	BitRate = rate;
	UartConfig(Channel,BitRate);
	if(Channel == 0)
	{
		FullDuplex = true;
	}
	else 
	{
		FullDuplex = false;
		//485 CONTROL LINE,半双工需要一个
		//开关来控制数据的接收和发送
		gpio_set_port_output(45);
	}
	SetInterruptPriority(UartIntNumber[Channel],1);
	vgSetInterrupt(UartIntNumber[Channel],p);
	EnableReceData();
	//enable data receive interrupt
    *(UIE[Channel]) = 0x01; 
	vgEnableInterrupt(UartIntNumber[Channel],true); 
	HaveConfigured = true;
}

void C_UART::CoolDownDelay() const
{
	int i,n;
	if(BitRate <= 9600)
	{
		n = 1600;
	}
	else if(BitRate <= 19200)
	{
		n = 2800;
	}
	else
	{
		n = 3600;
	}
	for(i=0;i<n;i++) ;
}

void C_UART::ReceiveDataPassively()
{
	U8  fifo[FifoLength],c;
	int i,rchars = 0;
	
	while((*ULSR[Channel]) & HAVE_DATA) 
	{
		fifo[rchars] = *URBR[Channel];
		++rchars;
	}
	CoolDownDelay();
	if(!HaveConfigured) 
	{
		return;
	}
	if(CurrPos + rchars >= ReceBuffLen)
	{
		SetReceBuffLen(ReceBuffLen*3/2);
	}
	i = 0;
	if(HaveHeadFlag && HaveEndFlag)
	{
		while(i < rchars)
		{
			c = fifo[i];
			i++;
			if(c == ReceHeadFlag) 
			{
				Start = true;
				ReceBuff[0] = c;
				CurrPos = 1;
			} 
			else if(Start) 
			{
				if(CurrPos < ReceBuffLen)
				{
					ReceBuff[CurrPos] = c;
					++CurrPos;
					if(c == ReceEndFlag) 
					{
						Start = false;
						DataReady = true;
						break;
					}
				}
			}
		}
	}
	else if(HaveHeadFlag)
	{
		while(i < rchars) 
		{
			c = fifo[i];
			i++;
			if(c == ReceHeadFlag) 
			{
				Start = true;
				ReceBuff[0] = c;
				CurrPos = 1;
			}
			else if(Start) 
			{
				if(CurrPos < ReceBuffLen) 
				{
					ReceBuff[CurrPos] = c;
					++CurrPos;
				}
			}
		}			
	}
	else if(HaveEndFlag)
	{
		while(i < rchars) 
		{
			c = fifo[i];
			i++;
			if(CurrPos < ReceBuffLen) 
			{
				ReceBuff[CurrPos] = c;
				++CurrPos;
			}
			if(c == ReceEndFlag) 
			{
				DataReady = true;
				break;
			}
		}
	}
	else
	{
		while(i < rchars) 
		{
			c = fifo[i];
			i++;
			if(CurrPos < ReceBuffLen) 
			{
				ReceBuff[CurrPos] = c;
				++CurrPos;
			}
		}
	}
	if(!DataReady)
	{
		if(rchars < TrigLength)
		{
			if(!HaveEndFlag)
			{
				DataReady = true;
			}
			else
			{
				CurrPos = 0;
				Start = false;
			}
		}
	}
	if(DataReady)
	{
		DealReceivedData();
		CurrPos = 0;
		Start = false;
		DataReady = false;
	}
}

void C_UART::TransData(U8 *data,int n) const
{
	if(!HaveConfigured || data == NULL || n < 1)
	{ 	
		return;
	}
	
	int i,totalSend = 0,fifoChars;
	int nFull = n/FifoLength;
	int nLeft = n%FifoLength;
	if(FullDuplex)
	{
		while(!(*ULSR[Channel] & FIFO_EMPTY)) ;
		for(i=0;i<nFull;i++)
		{
			fifoChars = 0;
			while(fifoChars < FifoLength)
			{
				*UTHR[Channel] = data[totalSend];
				++totalSend;
				++fifoChars;
			}
			if(i != nFull - 1 || nLeft > 0)
			{
				while(!(*ULSR[Channel] & FIFO_EMPTY)) ;
			}
		}
		if(nLeft > 0)
		{
			for(i=0;i<nLeft;i++)
			{
				*UTHR[Channel] = data[totalSend];
				++totalSend;
			}
		}
	}
	else
	{
		EnableTranData();
		for(i=0;i<nFull;i++)
		{
			fifoChars = 0;
			while(fifoChars < FifoLength)
			{
				*UTHR[Channel] = data[totalSend];
				++totalSend;
				++fifoChars;
			}
			while(!(*ULSR[Channel] & FIFO_EMPTY)) ;
		}
		if(nLeft > 0)
		{
			for(i=0;i<nLeft;i++)
			{
				*UTHR[Channel] = data[totalSend];
				++totalSend;
			}
			while(!(*ULSR[Channel] & FIFO_EMPTY)) ;
		}
		EnableReceData();
	}
}

void C_UART::EnableTranData() const
{
	if(!FullDuplex) 
	{
		*R_PxOH1 |= 0X00002000;
	}
}

void C_UART::EnableReceData() const
{
	if(!FullDuplex)
	{
		*R_PxOL1 |= 0X00002000; 
	}
}

void C_UART::DealReceivedData()
{
	if(IsReceivedDataValid())
	{	
		ReplyOrUse();
	}
}

void C_UART::SetReceBuffLen(int len)
{
	if(len > ReceBuffLen) 
	{
		U8 *oldP = ReceBuff;
		try 
		{
			ReceBuff = new U8[len];
			memcpy(ReceBuff,oldP,ReceBuffLen);
			ReceBuffLen = len;
			delete[] oldP;
		}
		catch(std::bad_alloc &e)
		{
			ReceBuff = oldP;
		}
	}
}

void C_UART::ReceiveDataActively(int len)
{
	if(!HaveConfigured || len < 1)
	{    
		return;
	}
	int MaxWait;
	MaxWait = len*10*1000/BitRate + 5;
	SetReceDataLen(len);
	Start = false;
	CurrPos = 0;
	LoopDelayMs(MaxWait);
}

/*
 * 1ms == 7600 for loop delay.
 */
void C_UART::WaitForDataCome()
{
	U8   readChar;
	int  i,n;
	bool receSomeData;
	//等待4帧的时间
	n = (4*10*7600*1000)/BitRate + 1;
	if(n < 3600)
	{
		n = 3600;
	}
	do 
	{
		receSomeData = false;
		for(i=0;i<n;i++) ;
		while((*ULSR[Channel]) & HAVE_DATA) 
		{
			readChar = *URBR[Channel];
			if(CurrPos < ReceBuffLen)
			{
				ReceBuff[CurrPos] = readChar;
				++CurrPos;
			}
			receSomeData = true;
		}
	}
	while(receSomeData);
}

bool C_UART::IsvalidBitRate(int rate) const
{
	return (rate == 57600  || rate == 38400 || rate == 19200 || rate == 115200 || 
			rate == 9600   || rate == 14400 || rate ==  4800 || rate == 2400   || 
			rate == 1200);
}

U8 C_UART::GetByte(unsigned int index)
{
	return ReceBuff[index]; 
}

