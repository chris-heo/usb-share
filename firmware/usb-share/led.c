#include <avr/io.h>
#include "simpleio.h"
#include "led.h"

#define IO_LED1R IOPIN(B, 0)
#define IO_LED1G IOPIN(D, 7)
#define IO_LED2R IOPIN(B, 2)
#define IO_LED2G IOPIN(B, 1)
#define IO_LED_PWM IOPIN(D, 6)

void led_init()
{
    DDRB |= IOPIN_MASK(IO_LED1R) | IOPIN_MASK(IO_LED2R) | IOPIN_MASK(IO_LED2G);
    DDRD |= IOPIN_MASK(IO_LED1G) | IOPIN_MASK(IO_LED_PWM);

    TCCR0A = (1<<COM0A1) | (0<<COM0A0) | (1<<WGM01) | (1<<WGM00); // Fast PWM
    TCCR0B = (0<<FOC0A) | (0<<FOC0B) | (0<<WGM02) | (0<<CS02) | (1<<CS01) | (0<<CS00); // clkdiv = 8
}

void led_brightness(uint8_t brightness)
{
    OCR0A = brightness;
}

void led_setcolor(uint8_t num, color_t color)
{
    if(num == 1)
    {
        if(color == color_off)
        {
            IOPIN_L(IO_LED1R);
            IOPIN_L(IO_LED1G);
        }
        else if(color == color_red)
        {
            IOPIN_H(IO_LED1R);
            IOPIN_L(IO_LED1G);
        }
        else if(color == color_green)
        {
            IOPIN_L(IO_LED1R);
            IOPIN_H(IO_LED1G);
        }
        else if(color == color_orange)
        {
            IOPIN_H(IO_LED1R);
            IOPIN_H(IO_LED1G);
        }
    }
    else if(num == 2)
    {
        if(color == color_off)
        {
            IOPIN_L(IO_LED2R);
            IOPIN_L(IO_LED2G);
        }
        else if(color == color_red)
        {
            IOPIN_H(IO_LED2R);
            IOPIN_L(IO_LED2G);
        }
        else if(color == color_green)
        {
            IOPIN_L(IO_LED2R);
            IOPIN_H(IO_LED2G);
        }
        else if(color == color_orange)
        {
            IOPIN_H(IO_LED2R);
            IOPIN_H(IO_LED2G);
        }
    }

}