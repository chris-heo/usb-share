#ifndef BUTTON_H_
#define BUTTON_H_

#define BUTTONS_COUNT 2

typedef struct
{
    volatile uint8_t* portpin_reg;
    uint8_t portpin_mask;
    bool laststate;
    bool pressed;
    bool released;
    uint16_t presstime;
}   button_t;

extern button_t buttons[BUTTONS_COUNT];

void button_init();
void button_tick();
bool button_state_current(uint8_t num);

#endif /* BUTTON_H_ */
