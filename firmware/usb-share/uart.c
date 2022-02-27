#include "uart.h"
#include "fifo.h"
#include "simpleio.h"
#include <stdbool.h>

volatile FIFO64_t uart_debug_fifo;
volatile bool uart_debug_running = false;

ISR(USART1_TX_vect)
{
    if(FIFO_available(uart_debug_fifo))
    {
        UDR1 = FIFO64_read(uart_debug_fifo);
    }
    else
    {
        uart_debug_running = false;
    }
}

void uart_init() {
    FIFO_init(uart_debug_fifo);
    //FIFO64_write(uart_debug_fifo, 0x00);
    //FIFO64_read(uart_debug_fifo);
    UBRR1 = 12; //115200 Bd @ 12 MHz
    UCSR1A |= (1<<U2X1);
    UCSR1B = (1<<TXCIE1) | (1<<TXEN1) | (0<<UCSZ12);
    //UCSR0B = (0<<TXCIE0) | (1<<TXEN0) | (0<<UCSZ02);
    UCSR1C = (0<<UMSEL11) | (0<<UMSEL10) | (0<<UPM11) | (0<<UPM10) | (0<<USBS1) | (1<<UCSZ11) | (1<<UCSZ10);
}

inline void uart_putc(uint8_t c) {
    while(!FIFO64_free(uart_debug_fifo));
    
    FIFO64_write(uart_debug_fifo, c)
    
    if(uart_debug_running == false)
    {
        uart_debug_running = true;
        UDR1 = FIFO64_read(uart_debug_fifo);
    }
}

void uart_puts(char *s) {
    while(*s)
    {
        uart_putc(*s++);
    }
}

void uart_write_P (const char *Buffer,...)
{
    va_list ap;
    va_start (ap, Buffer);	
    
    int format_flag;
    char str_buffer[10];
    char str_null_buffer[10];
    char move = 0;
    char Base = 0;
    char isSigned = 0;
    int tmp = 0;
    char by;
    char *ptr;
        
    //Ausgabe der Zeichen
    for(;;)
    {
        by = pgm_read_byte(Buffer++);
        if(by==0) break; // end of format string
        if (by == '%')
        {
            by = pgm_read_byte(Buffer++);
            if (isdigit(by)>0)
            {
                str_null_buffer[0] = by;
                str_null_buffer[1] = '\0';
                move = atoi(str_null_buffer);
                by = pgm_read_byte(Buffer++);
            }
            
            switch (by)
            {
                case 's':
                    ptr = va_arg(ap,char *);
                    while(*ptr) { uart_putc(*ptr++); }
                    break;
                case '%':
                    uart_putc('%');
                    break;
                case 'b':
                    Base = 2;
                    goto ConversionLoop;
                case 'c':
                    //Int to char
                    format_flag = va_arg(ap,int);
                    uart_putc (format_flag++);
                    break;
                case 'i':
                    isSigned = 1;
                    Base = 10;
                    goto ConversionLoop;
                case 'u':
                    Base = 10;
                    goto ConversionLoop;
                case 'o':
                    Base = 8;
                    goto ConversionLoop;
                case 'x':
                    Base = 16;
                    //****************************
                    ConversionLoop:
                    //****************************
                    if(isSigned == 1) {
                        itoa(va_arg(ap,int),str_buffer,Base);
                    } else {
                        utoa(va_arg(ap,unsigned int),str_buffer,Base);
                    }
                    int b=0;
                    while (str_buffer[b++] != 0){};
                    b--;
                    if (b<move)
                        {
                        move -=b;
                        for (tmp = 0;tmp<move;tmp++)
                            {
                            str_null_buffer[tmp] = '0';
                            }
                        //tmp ++;
                        str_null_buffer[tmp] = '\0';
                        strcat(str_null_buffer,str_buffer);
                        strcpy(str_buffer,str_null_buffer);
                        }
                    uart_puts (str_buffer);
                    move = 0;
                    break;
            }
        }
        else
        {
            uart_putc ( by );
        }
    }
    va_end(ap);
}

void uart_rawprint(uint8_t * data, uint16_t length)
{
    for(uint16_t i = 0; i < length; i++)
    {
        uart_write("%2x", data[i]);
    }
}