#ifndef USBSWITCH_H_
#define USBSWITCH_H_

#include <stdbool.h>

typedef enum
{
    usbswitch_state_disconnected = 0, // disconnected (steady state)
    usbswitch_state_connected = 1, // connected (steady state)
    usbswitch_state_seq_connect = 2, // sequence to connect
    usbswitch_state_seq_disconnect = 3, // sequence to disconnect
    usbswitch_state_seqpowersettle = 4, // sequence to settle power (after disconnecting)

}   usbswitch_state_t;

extern usbswitch_state_t usbswitch_state;
extern uint8_t usbswitch_selection;

void usbswitch_init();
bool usbswitch_tick();
void usbswitch_set(uint8_t num);

#endif /* USBSWITCH_H_ */
