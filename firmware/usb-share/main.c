#include <avr/io.h>
#include <stdbool.h>
#include <avr/fuse.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "simpleio.h"
#include "led.h"
#include "adc.h"
#include "button.h"
#include "usbswitch.h"
#include "uart.h"
#include "m328pb_regs.h"
#include "usbswsm.h"
#include "twi.h"
#include "ssd1306.h"
#include "pac193x.h"

void systick_init()
{
    TCCR1A = (0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | TCCR1A_WGM_CTC_OCR;
    TCCR1B = (0<<ICNC1) | (0<<ICES1) | TCCR1B_WGM_CTC_OCR | TCCR1B_CS_DIV64;
    OCR1A = 375-1; // 2ms
}

usbswitch_state_t usbsw_oldstate;

void printstate(usbswitch_state_t state)
{
    switch(state)
    {
        case usbswitch_state_disconnected: uart_write("dis"); break;
        case usbswitch_state_connected: uart_write("con"); break;
        case usbswitch_state_seq_connect: uart_write("sc"); break;
        case usbswitch_state_seq_disconnect: uart_write("sd"); break;
        case usbswitch_state_seqpowersettle: uart_write("sps"); break;
    }
}



int main(void)
{
    usbswitch_init();
    usbsw_oldstate = usbswitch_state;
    button_init();

    led_init();
    uart_init();

    uart_writeln("===== Starting system =====");
    adc_init();
    IOPIN_O(IOPIN(B, 3));
    systick_init();
    led_brightness(64);
    usbswsm_init();

    twi_master_init();
    
    _delay_ms(50);

    sei();

    pac193x_ctrl_s ctrl = 
    {
        .alert_overflow_en = false,
        .alert_dataready = false,
        .alert_pin_en = false,
        .singleshot_en = false,
        .sleepmode_en = false,
        .samplerate = pac1934x_ctrl_samplerate_64,
    };
    pac193x_ctrl_set(pac193x_addrGND, ctrl);

    while (1)
    {
        
        if(TIFR1 & (1<<OCF1A))
        {
            TIFR1 |= (1<<OCF1A);
            
            usbswsm_tick();
        }
    }
}

