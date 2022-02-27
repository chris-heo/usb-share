#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "m328pb_regs.h"

#define adc_start() do { ADCSRA |= (1<<ADSC); } while(0)

#define array_length(arr) (sizeof(arr) / sizeof(typeof(*arr)))

const __flash uint8_t adc_scanchannels[] = 
{
    ADMUX_MUX_ADC0, ADMUX_MUX_ADC1,
};

volatile uint16_t adc_values[array_length(adc_scanchannels)];
volatile uint8_t adc_currchan = 0;

ISR(ADC_vect)
{
    adc_values[adc_currchan] = ADC;
    adc_currchan++;
    if(adc_currchan >= array_length(adc_scanchannels))
    {
        adc_currchan = 0;
    }
    ADMUX = (ADMUX & ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0))) | adc_scanchannels[adc_currchan];
    adc_start();
}

void adc_init()
{
    ADMUX = (1<<REFS1) | (1<<REFS0) | (0<<ADLAR) | ADMUX_MUX_ADC0;
    ADCSRA = (1<<ADEN) | (0<<ADSC) | (0<<ADIF) | (1<<ADIE) | ADCSRA_ADPS_DIV64;
    ADCSRB = (0<<ACME) | ADCSRB_ADTS_FREERUN;
    DIDR0 = (1<<ADC1D) | (1<<ADC0D);
    adc_currchan = 0;
    adc_start();
}

uint16_t adc_getraw(uint8_t index)
{
    return adc_values[index];
}

uint16_t adc_getvoltage(uint8_t index)
{
    //FIXME: don't use magic numbers
    uint32_t foo = ((uint32_t)adc_values[index] * 1100) / 1024;
    return (foo * (180+47)) / 47;
}
