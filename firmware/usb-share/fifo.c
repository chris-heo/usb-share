#include "fifo.h"

void FIFO16_gets(FIFO16_t fifo, char* lin)
{
    char c;
    while(FIFO_available(fifo))
    {
        c = FIFO16_read(fifo);
        if(c < 0x20) continue; // ignore control characters
        *lin = c;
        lin++;
    }
    *lin = '\0';
}

/*
uint8_t FIFO16_peek_w(FIFO16_t fifo, uint8_t offset)
{	
    if(offset <= fifo._write)
    {
        offset = fifo._write - offset;
        
    }
    else
    {
        
        offset = 16 + fifo._write - offset;
        
        
    }
    return fifo._buffer[offset];
}
*/