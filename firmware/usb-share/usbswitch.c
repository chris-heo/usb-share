#include <avr/io.h>
#include <util/delay.h>
#include "simpleio.h"
#include "usbswitch.h"

#define IO_MUX_OE IOPIN(C, 2)
#define IO_MUX_S IOPIN(C, 3)

#define IO_USB1_EN IOPIN(E, 3)
#define IO_USB2_EN IOPIN(E, 2)

#define USBSWITCH_DELAY_CONN 20
#define USBSWITCH_DELAY_DISC 30
#define USBSWITCH_DELAY_PWRS 50

usbswitch_state_t usbswitch_state;
uint8_t usbswitch_selection;
uint8_t usbswitch_ticks;

void usbswitch_setpower(uint8_t num);
void usbswitch_setmux(uint8_t num);


void usbswitch_init()
{
    DDRC |= IOPIN_MASK(IO_MUX_OE) | IOPIN_MASK(IO_MUX_S);
    PORTC = (PORTC & ~(IOPIN_MASK(IO_MUX_OE) | IOPIN_MASK(IO_MUX_S))) | IOPIN_MASK(IO_MUX_OE);

    DDRE |= IOPIN_MASK(IO_USB1_EN) | IOPIN_MASK(IO_USB2_EN);
    PORTE &= ~(IOPIN_MASK(IO_USB1_EN) | IOPIN_MASK(IO_USB2_EN));

    usbswitch_state = usbswitch_state_disconnected;
}

inline void usbswitch_setstate(usbswitch_state_t newstate)
{
    if(newstate == usbswitch_state_seqpowersettle)
        usbswitch_ticks = USBSWITCH_DELAY_PWRS;
    else if(newstate == usbswitch_state_seq_connect)
        usbswitch_ticks = USBSWITCH_DELAY_CONN;
    else if(newstate == usbswitch_state_seq_disconnect)
        usbswitch_ticks = USBSWITCH_DELAY_DISC;

    usbswitch_state = newstate;
}

bool usbswitch_tick()
{
    if(usbswitch_state == usbswitch_state_disconnected || usbswitch_state == usbswitch_state_connected)
    {
        // FIXME: checking usbswitch_ticks should also work
        // nothing to do, return false
        return false;
    }

    if(usbswitch_ticks > 0)
    {
        // still waiting for the transition to finish, decrement ticks and return true
        usbswitch_ticks--;
        return true;
    }

    // usbswitch_ticks is 0
    if(usbswitch_state == usbswitch_state_seq_disconnect)
    {
        // task was to disconnect data, now let's cut power and settle
        usbswitch_setpower(0);
        usbswitch_setstate(usbswitch_state_seqpowersettle);
        return true; // still busy
    }
    else if(usbswitch_state == usbswitch_state_seq_connect)
    {
        // power is connected, now connect the corresponding data channel
        usbswitch_setmux(usbswitch_selection);
        //and we're done
        usbswitch_setstate(usbswitch_state_connected);
        return false;
    }
    else if(usbswitch_state == usbswitch_state_seqpowersettle)
    {
        // power has settled (disconnected), what to do next?
        if(usbswitch_selection == 0)
        {
            // task was just disconnect, we're done here.
            usbswitch_setstate(usbswitch_state_disconnected);
            return false;
        }
        // connect the new channel, so enable power connection and run the on sequence
        usbswitch_setpower(usbswitch_selection);
        usbswitch_setstate(usbswitch_state_seq_connect);
        return true;
    }
    return true;
}

void usbswitch_set(uint8_t num)
{
    if(num == usbswitch_selection)
        return;
        //FIXME: ensure that the currently selected channel is active?

    // FIXME: what to do when a transition is ongoing?

    if(usbswitch_selection == 0 && num != 0)
    {
        // transition from disconnected to any channel
        // first connect power, then data
        usbswitch_setpower(num);
        // next step is to connect data
        usbswitch_selection = num;
        usbswitch_setstate(usbswitch_state_seq_connect);
    }
    else
    {
        // transition from any channel to another channel
        // first disconnect data, then power
        usbswitch_setmux(0);
        usbswitch_selection = num;
        usbswitch_setstate(usbswitch_state_seq_disconnect);

        // after the off-sequence has finished let off power state settle.
        // after that, connect to new channel
    }
}

void usbswitch_setmux(uint8_t num)
{
    IOPIN_H(IO_MUX_OE);
    if(num == 1)
    {
        IOPIN_L(IO_MUX_S);
        IOPIN_L(IO_MUX_OE);
    }
    else if(num == 2)
    {
        IOPIN_H(IO_MUX_S);
        IOPIN_L(IO_MUX_OE);
    }
}

void usbswitch_setpower(uint8_t num)
{
    uint8_t tmp = (PORTE & ~(IOPIN_MASK(IO_USB1_EN) | IOPIN_MASK(IO_USB2_EN)));
    
    if(num == 1)
    {
        tmp |= IOPIN_MASK(IO_USB1_EN);
    }
    else if(num == 2)
    {
        tmp |= IOPIN_MASK(IO_USB2_EN);
    }
    PORTE = tmp;
}