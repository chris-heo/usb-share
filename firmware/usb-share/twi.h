#include <avr/io.h>
#include <stdbool.h>
#include <util/twi.h>

#ifndef TWI_H_
#define TWI_H_

#define TWI_READ 1
#define TWI_WRITE 0

void twi_master_init();
bool twi_start(uint8_t address);
void twi_start_wait(uint8_t address);
bool twi_rep_start(uint8_t address);
void twi_stop(void);
bool twi_write(uint8_t data);
uint8_t twi_readAck(void);
uint8_t twi_readNak(void);


#endif /* TWI_H_ */
