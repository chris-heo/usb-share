#ifndef LED_H_
#define LED_H_

#include <stdint.h>

typedef enum
{
    color_off = 0,
    color_red = 1,
    color_green = 2,
    color_orange = 3,

}   color_t;

void led_init();
void led_brightness(uint8_t brightness);
void led_setcolor(uint8_t num, color_t color);

#endif /* LED_H_ */
