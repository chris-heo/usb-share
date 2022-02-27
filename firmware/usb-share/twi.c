#include "twi.h"

/* I2C clock in Hz */
#define SCL_CLOCK  330000L

#define TW0_STATUS (TWSR0 & TW_STATUS_MASK)

void twi_master_init()
{
    TWSR0 = 0;                         /* no prescaler */
    TWBR0 = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */
      
    #if ((F_CPU/SCL_CLOCK) < 16)
    #error TWBR is below 0, I2C will not work properly.
    #elif ((((F_CPU/SCL_CLOCK)-16)/2) < 10)
    #warning TWBR is below 10, I2C might not work.
      
    #endif
    
}

bool twi_start(uint8_t address)
{
    uint8_t   twst;

    // send START condition
    TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    // wait until transmission completed
    while(!(TWCR0 & (1<<TWINT)));

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TW0_STATUS;
    if ( (twst != TW_START) && (twst != TW_REP_START)) return true;

    // send device address
    TWDR0 = address;
    TWCR0 = (1<<TWINT) | (1<<TWEN);

    // wail until transmission completed and ACK/NACK has been received
    while(!(TWCR0 & (1<<TWINT)));

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TW0_STATUS;
    if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return true;

    return false;
}

void twi_start_wait(uint8_t address)
{
    uint8_t   twst;


    while(true)
    {
        // send START condition
        TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
        
        // wait until transmission completed
        while(!(TWCR0 & (1<<TWINT)));
        
        // check value of TWI Status Register. Mask prescaler bits.
        twst = TW0_STATUS;
        if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
        
        // send device address
        TWDR0 = address;
        TWCR0 = (1<<TWINT) | (1<<TWEN);
        
        // wail until transmission completed
        while(!(TWCR0 & (1<<TWINT)));
        
        // check value of TWI Status Register. Mask prescaler bits.
        twst = TW0_STATUS;
        if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) )
        {
            /* device busy, send stop condition to terminate write operation */
            TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
            
            // wait until stop condition is executed and bus released
            while(TWCR0 & (1<<TWSTO));
            
            continue;
        }
        //if( twst != TW_MT_SLA_ACK) return 1;
        break;
    }

}

bool twi_rep_start(uint8_t address)
{
    return twi_start(address);
}

void twi_stop(void)
{
    /* send stop condition */
    TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
    
    // wait until stop condition is executed and bus released
    while(TWCR0 & (1<<TWSTO));

}

bool twi_write(uint8_t data)
{
    uint8_t twst;
    
    // send data to the previously addressed device
    TWDR0 = data;
    TWCR0 = (1<<TWINT) | (1<<TWEN);

    // wait until transmission completed
    while(!(TWCR0 & (1<<TWINT)));

    // check value of TWI Status Register. Mask prescaler bits
    twst = TW0_STATUS;
    if( twst != TW_MT_DATA_ACK) return true;
    return false;

}

uint8_t twi_readAck(void)
{
    TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while(!(TWCR0 & (1<<TWINT)));

    return TWDR0;

}

uint8_t twi_readNak(void)
{
    TWCR0 = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR0 & (1<<TWINT)));
    
    return TWDR0;

}