#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#ifndef DEBUG_H_
#define DEBUG_H_

void uart_putc(uint8_t c);
void uart_puts(char *s);
void uart_init();


void uart_write_P (const char *Buffer,...);
void uart_rawprint(uint8_t * data, uint16_t length);
#define uart_write(format, args...)   uart_write_P(PSTR(format) , ## args)
#define uart_writeln(format, args...)   uart_write_P(PSTR(format "\r\n") , ## args)


#endif /* DEBUG_H_ */



