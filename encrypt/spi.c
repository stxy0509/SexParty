#include <FS8806.h>
#include <spi.h>
#include <libst/linkage.h>

C_LINKAGE void Spi_Init(BYTE mode,WORD rate)
{
    mode = mode;
    rate = rate;
}

C_LINKAGE void Spi_WriteData( BYTE *buffer, BYTE Len)
{
    buffer = buffer;
    Len = Len;
}

C_LINKAGE void Spi_ReadData( BYTE *buffer, BYTE Len)
{
    buffer = buffer;
    Len = Len;
}
