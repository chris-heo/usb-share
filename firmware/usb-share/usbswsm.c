#include <stdbool.h>
#include "usbswsm.h"
#include "led.h"
#include "adc.h"
#include "button.h"
#include "usbswitch.h"
#include "uart.h"
#include "helper.h"

// active & inactive voltages in mV
// FIXME: use raw ADC values
#define USBSENSE_THRS_ACTIVE 4500
#define USBSENSE_THRS_INACTIVE 3000

// active & inactive durations in ticks for transition
#define USBSENSE_TIME_ACTIVE 10
#define USBSENSE_TIME_INACTIVE 5

// button presstimes
#define BUTTON_PRESSTIME_SHRT 20
#define BUTTON_PRESSTIME_LONG 500

// button longpress functions
#define BUTTON_LONGPRESS_DISCONNECT_ALL 0


usbswsm_config_t usbswsm_config = 
{
    .auto_startup_first = true,
    .auto_first_available = true,
    .auto_change_connect = true,
    .auto_change_disconnect = true,
    .button_allow_select_disconnected = true,
    .button_allow_disconnect_all = true,
};

usbportstate_t usbchannels[USBSW_CHANNELS];

bool usbswsm_firstblood = true;

void usbswsm_init(void)
{
    for(uint8_t i = 0; i < USBSW_CHANNELS; i++)
    {
        usbchannels[i].powerstate = false;
        usbchannels[i].powerstate_changed = false;
        usbchannels[i].transition_ticks = 0;
    }
    usbswsm_firstblood = true;
}

void vbus_state_tick(void)
{
    for(uint8_t i = 0; i < USBSW_CHANNELS; i++)
    {
        uint16_t voltage = adc_getvoltage(i);

        if(usbchannels[i].powerstate == false)
        {
            if(voltage >= USBSENSE_THRS_ACTIVE)
            {
                usbchannels[i].transition_ticks++;
                if(usbchannels[i].transition_ticks > USBSENSE_TIME_ACTIVE)
                {
                    usbchannels[i].powerstate = true;
                    usbchannels[i].powerstate_changed = true;
                    usbchannels[i].transition_ticks = 0;
                }
            }
            else
            {
                usbchannels[i].transition_ticks = 0;
            }
            
        }
        else //usbchannels[i].state == true
        {
            if(voltage <= USBSENSE_THRS_INACTIVE)
            {
                usbchannels[i].transition_ticks++;
                if(usbchannels[i].transition_ticks > USBSENSE_TIME_INACTIVE)
                {
                    usbchannels[i].powerstate = false;
                    usbchannels[i].powerstate_changed = true;
                    usbchannels[i].transition_ticks = 0;
                }
            }
            else
            {
                usbchannels[i].transition_ticks = 0;
            }
        }
    }
}

void leds_update()
{
    for(uint8_t i = 0; i < 2; i++)
    {
        bool powered = false;
        if(usbchannels[i].powerstate == true)
            powered = true;

        bool selected = false;
        if(usbswitch_state == usbswitch_state_connected && usbswitch_selection == (i + 1))
            selected = true;

        color_t color = color_off;
        if(powered)
        {
            if(selected)
                color = color_green;
            else
                color = color_red;
        }
        else
        {
            if(selected)
            color = color_orange;
        }
        led_setcolor(i + 1, color);
    }
}


void usbswsm_tick()
{
    vbus_state_tick();
    button_tick();
    usbswitch_tick();

    for(uint8_t i = 0; i < array_size(buttons); i++)
    {
        button_t* btn = &buttons[i];

        if(btn->released == true)
        {
            btn->released = false;

            if(btn->presstime >= BUTTON_PRESSTIME_SHRT && btn->presstime < BUTTON_PRESSTIME_LONG)
            {
                //button was pressed and released as shortpress
                if(usbswsm_config.button_allow_select_disconnected == true || 
                    (usbswsm_config.button_allow_select_disconnected == false && usbchannels[i].powerstate == true))
                {
                    uart_writeln("change channel -> %u", (i + 1));
                    usbswitch_set(i + 1);
                }
                else
                {
                    uart_writeln("change channel -> %u NOT ALLOWED!", (i + 1));
                }
            }
        }
        else if(btn->pressed && btn->presstime > BUTTON_PRESSTIME_LONG)
        {
            btn->pressed = false;
            
            if(i == BUTTON_LONGPRESS_DISCONNECT_ALL && usbswsm_config.button_allow_disconnect_all)
            {
                uart_writeln("disconnect all");
                usbswitch_set(0);
            }
        }
    }

    for(uint8_t i = 0; i < array_size(usbchannels); i++)
    {
        usbportstate_t* ups = &usbchannels[i];

        if(ups->powerstate_changed == true)
        {
            ups->powerstate_changed = false;

            if(ups->powerstate == true && usbswsm_config.auto_change_connect)
            {
                uart_writeln("autochange conn channel -> %u", (i + 1));
                usbswitch_set(i + 1);
            }
            else if(ups->powerstate == false && usbswsm_config.auto_change_disconnect)
            {
                // find other connected channel
                bool found = false;
                for(uint8_t j = 0; j < array_size(usbchannels); j++)
                {
                    if(usbchannels[j].powerstate == true)
                    {
                        found = true;
                        uart_writeln("autochange disconn channel -> %u", (j + 1));
                        usbswitch_set(j + 1);
                        break;
                    }
                }
                if(found == false)
                {
                    uart_writeln("autochange disconn channel failed");
                }
            }
            else if(ups->powerstate == true && usbswsm_config.auto_first_available)
            {
                uint8_t cnt = 0;
                for(uint8_t j = 0; j < array_size(usbchannels); j++)
                {
                    if(usbchannels[j].powerstate == true)
                        cnt++;
                }
                if(cnt == 1)
                {
                    uart_writeln("autoconn first avail -> %u", i + 1);
                    usbswitch_set(i + 1);
                }
            }
        }
        else
        {
            //no change but "first blood"
            if(usbswsm_config.auto_startup_first == true && usbswsm_firstblood == true && ups->powerstate == true)
            {
                usbswsm_firstblood = false;
                uart_writeln("startup channel -> %u", (i + 1));
                usbswitch_set(i + 1);
            }
        }

        //FIXME: optimize
        if(usbswitch_state == usbswitch_state_connected && usbswitch_selection == (i + 1))
        {
            ups->selected = true;
        }
        else
        {
            ups->selected = false;
        }
    }

    leds_update();
}
