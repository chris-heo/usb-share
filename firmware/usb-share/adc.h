#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void adc_init();
uint16_t adc_getvoltage(uint8_t index);

#endif /* ADC_H_ */
