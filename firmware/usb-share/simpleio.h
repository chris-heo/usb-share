#ifndef SIMPLEIO_H_
#define SIMPLEIO_H_

#include <avr/io.h>

//usage: #define MyPin   IOPIN(B, 2)

#define IOPIN(port, pin) port, pin

#define __IOPIN_PORT(port, pin) PORT##port
#define __IOPIN_DDR(port, pin) DDR##port
#define __IOPIN_PIN(port, pin) PIN##port
#define __IOPIN_MASK(port, pin) (1<<pin)

#define __IOPIN_O(port, pin) do { DDR##port |=  (1<<pin); } while(0)
#define __IOPIN_I(port, pin) do { DDR##port &= (uint8_t)~(1<<pin); } while(0)

#define __IOPIN_H(port, pin) do { PORT##port |=  (1<<pin); } while(0)
#define __IOPIN_L(port, pin) do { PORT##port &= (uint8_t)~(1<<pin); } while(0)
#define __IOPIN_T(port, pin) do { PORT##port ^=  (1<<pin); } while(0)

#define __IOPIN_V(port, pin) (PIN##port & (1<<pin))

#define IOPIN_PORT(x) __IOPIN_PORT(x, 0)
#define IOPIN_DDR(x)  __IOPIN_DDR(x, 0)
#define IOPIN_PIN(x)  __IOPIN_PIN(x, 0)

// returns the mask of the pin within the register
#define IOPIN_MASK(x) __IOPIN_MASK(x)

// configures the pin as output
#define IOPIN_O(x) __IOPIN_O(x)

// configures the pin as input
#define IOPIN_I(x) __IOPIN_I(x)

// sets the pin to high or enables pull-up when configured as input
#define IOPIN_H(x) __IOPIN_H(x)

// sets the pin to low or disables pull-up when configured as input
#define IOPIN_L(x) __IOPIN_L(x)

// toggles the pin (not efficient on modern AVRs
#define IOPIN_T(x) __IOPIN_T(x)

// returns the value of the pin's input
#define IOPIN_V(x) __IOPIN_V(x)

#endif /* SIMPLEIO_H_ */