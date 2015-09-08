//function: I2C COMMUNICATION
#include "encrypt.h"
#include "i2c.h"
#include <libst/linkage.h>
//default I2c bus slave address for communication
U8 BusSlvAdr = 0xC4;

C_LINKAGE BOOL ENCRYPT::I2cSendByte(U8 c)
{
    BOOL retvalue = FALSE;
    if(!(*twi_stat & 0x00000020))//busy
    {
        //slave address + read and write control bit
        *twi_data = 0xC4;
        *twi_con |= msta_start;
        while(!(*twi_stat & 0x00000080))
            ;//transfer complete
        *twi_data = c;
        while(!(*twi_stat & 0x00000080))
            ;//data transfer
        if(!(*twi_stat & 0x00000001))
            retvalue = TRUE;
        *twi_con &= msta_stop;
    }
    return retvalue;
}

C_LINKAGE U8 ENCRYPT::I2c_SendStr(U8 sla,U8 *s,char no)
{
    unsigned char stat ;
    int i;

    stat = *twi_stat;
    // bus busy check
    while (stat & 0x20) stat = *twi_stat;
    *twi_data = sla; // Device Address<<1
    *twi_con= 0xB0;  // Master TX
    stat = *twi_stat;
    // check complete condition
    while( !(stat & 0x80) ) stat = *twi_stat;
    if(!(*twi_stat & 0x1) )
    {
        for(i = 0;i < no ;i++)
        {
            *twi_data = *(s++);
            stat = *twi_stat;
            while ( !(stat & 0x80) ) stat = *twi_stat;
        } // check complete condition
        *twi_con = 0x80 ; // Stop Condition, Disable Master Tx
        stat = *twi_stat;
        // bus busy check
        while (stat & 0x20) stat = *twi_stat;
    }
    else
    {
        *twi_con = 0x00;
        return 0xff;
    }
    *twi_con = 0x00;
    stat = *twi_stat;
    // check complete condition
    while (!(stat & 0x80)) stat = *twi_stat;
    return 0;
}

C_LINKAGE U8 ENCRYPT::I2c_RcvStr (U8 sla, U8 *s, int no)
{
    int i;
    unsigned char stat;
    unsigned long rdata;

    stat = *twi_stat;
    // bus busy check
    while(stat & 0x20) stat = *twi_stat;
    *twi_data = sla | 0x1; // Device Address(Read)
    *twi_con = 0xA0; // Master RX
    stat = *twi_stat;
    // check complete condition ´«ÍêÌø³ö
    while( !(stat & 0x80) ) stat = *twi_stat;
    rdata = *twi_data;
    stat = *twi_stat;
    while( !(stat & 0x80) ) stat = *twi_stat;
    if(!(*twi_stat & 0x1) )
    {
        for(i = 0;i < no ;i++)
        {
            rdata = *twi_data;
            stat = *twi_stat;
            while ( !(stat & 0x80) ) stat = *twi_stat;
            rdata &= 0x01FF;
            *s = rdata;
            s++;
        } 	// check complete condition
        *twi_con |= 0x08;
        stat = *twi_stat;
        while( !(stat & 0x80) ) stat = *twi_stat;
        rdata = *twi_data;
        stat = *twi_stat;
        while( !(stat & 0x80) ) stat = *twi_stat;
        stat = *twi_stat;
        while(stat & 0x20) stat = *twi_stat;
    }
    else
    {
        *twi_con = 0x00;
        return 0xff;
    }
    *twi_con = 0x00;
    stat = *twi_stat;
    // check complete condition
    while( !(stat & 0x80) ) stat = *twi_stat;
    return 0;
}

C_LINKAGE void ENCRYPT::twi_baudrate_set(U32 baud)
{
    *twi_br0 = ((vgGetSystemClock()/2)*7)/10000000 + 3 ;
    *twi_br1 = (U32)((float)(vgGetSystemClock()/2)/(2*baud) - (float)((*twi_br0+7)/2));
}

C_LINKAGE void ENCRYPT::Delay_1(unsigned long cnt)
{
    for (;cnt>0; cnt--);
}

C_LINKAGE void ENCRYPT::Reset8826()
{
    *R_PxOH1 |= (1 << 28);
    Delay(0x0027FFFD);
    *R_PxOL1 |= (1 << 28);
    Delay(0x0027FFFD);
    *R_PxOH1 |= (1 << 28);
    Delay(0x0027FFFD);
}

C_LINKAGE void ENCRYPT::I2c_Init()
{
    *R_PINMUX3 |= (0x03 << 24); //
    *R_PxOUT1 |= (1 << 28);	//set the GPIO60 as a output
    *R_PINMUX6 &= ~(0xf << 20);	//TWI_SDA, TWI_SCL
    Reset8826();
    twi_baudrate_set(100*1000);
}
