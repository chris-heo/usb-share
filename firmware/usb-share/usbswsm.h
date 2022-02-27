#ifndef USBSWSM_H_
#define USBSWSM_H_

#include <stdint.h>

#define USBSW_CHANNELS 2


typedef struct
{
    bool powerstate;
    bool powerstate_changed;
    bool selected;

    uint8_t transition_ticks;
}   usbportstate_t;

typedef struct
{
    bool auto_startup_first;
    bool auto_first_available;
    bool auto_change_connect;
    bool auto_change_disconnect;
    bool button_allow_select_disconnected;
    bool button_allow_disconnect_all;
}   usbswsm_config_t;

extern usbportstate_t usbchannels[USBSW_CHANNELS];


void usbswsm_init(void);
void usbswsm_tick();

#endif /* USBSWSM_H_ */
