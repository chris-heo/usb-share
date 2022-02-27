#include <avr/io.h>
#include <stdbool.h>
#include "simpleio.h"
#include "button.h"
#include "helper.h"

#define IO_BTN1 IOPIN(D, 4)
#define IO_BTN2 IOPIN(D, 5)

#define BUTTON_PRESSTIME_MAX 0xFFFF

button_t buttons[BUTTONS_COUNT] =
{
    { .portpin_reg = &PIND, .portpin_mask = (1<<4), },
    { .portpin_reg = &PIND, .portpin_mask = (1<<5), },
};

void button_init()
{
    DDRD &= ~(IOPIN_MASK(IO_BTN1) | IOPIN_MASK(IO_BTN2));
    //TODO: enable pull-ups?
    for(uint8_t i = 0; i < array_size(buttons); i++)
    {
        buttons[i].pressed = false;
        buttons[i].released = false;
        buttons[i].presstime = 0;
    }
}

void button_tick()
{
    for(uint8_t i = 0; i < array_size(buttons); i++)
    {
        button_t *btn = &buttons[i];
        
        bool state = false;
        if((*btn->portpin_reg & btn->portpin_mask) == 0)
            state = true;

        if(btn->laststate == false && state == true)
        {
            btn->presstime = 0;
            btn->pressed = true;
        }
        else if(btn->laststate == true && state == false)
        {
            btn->released = true;
        }
        if(state == true)
        {
            if(btn->presstime < BUTTON_PRESSTIME_MAX)
                btn->presstime++;
        }
        btn->laststate = state;

    }
}

bool button_state_current(uint8_t num)
{
    uint8_t val = 0;
    if(num == 1)
    {
        val = IOPIN_V(IO_BTN1);
    }
    else if(num == 2)
    {
        val = IOPIN_V(IO_BTN2);
    }

    if(val == 0)
        return true;
    return false;
}
