#include "twi.h"
#include "pac193x.h"
#include "uart.h"

bool twi_writereg8(uint8_t i2c_addr, uint8_t reg, uint8_t val)
{
    uint8_t status = twi_start((i2c_addr << 1) | TWI_WRITE);
    if(status) goto failed;

    twi_write(reg);
    twi_write(val);

failed:
    twi_stop();
    return status;
    
}

bool twi_writereg16BE(uint8_t i2c_addr, uint8_t reg, uint16_t* val)
{
    uint8_t status = twi_start((i2c_addr << 1) | TWI_WRITE);
    if(status) goto failed;

    twi_write(reg);

    status = twi_start((i2c_addr << 1) | TWI_READ);
    if(status) goto failed;

    uint16_t tmp = twi_readAck() << 8;
    tmp |= twi_readNak();

    *val = tmp;

    failed:
    twi_stop();
    return status;
}

bool pac193x_ctrl_set(uint8_t i2c_addr, pac193x_ctrl_s data)
{
    return twi_writereg8(i2c_addr, pac193x_reg_CTRL, data.data);
}

void pac193x_refresh(uint8_t i2c_addr, bool reset_accu)
{
    twi_start((i2c_addr << 1) | TWI_WRITE);
    twi_write(reset_accu ? pac193x_reg_REFRESH : pac193x_reg_REFRESH_V);
    twi_stop();
}


bool pac193x_vbus_getraw(uint8_t i2c_addr, uint8_t channel, bool avg, uint16_t* val)
{
    uint8_t reg = (avg == true ? pac193x_reg_VBUS1_AVG : pac193x_reg_VBUS1) + channel;

    return twi_writereg16BE(i2c_addr, reg, val);
}

bool pac193x_vsense_getraw(uint8_t i2c_addr, uint8_t channel, bool avg, uint16_t* val)
{
    uint8_t reg = (avg == true ? pac193x_reg_VSENSE1_AVG : pac193x_reg_VSENSE1) + channel;

    return twi_writereg16BE(i2c_addr, reg, val);
}


bool pac193x_vbus_getse_mV(uint8_t i2c_addr, uint8_t channel, bool avg, uint16_t* val)
{
    uint16_t tmp;
    if(pac193x_vbus_getraw(i2c_addr, channel, avg, &tmp))
        return true;

    uint32_t tmp2 = tmp + 1;
    tmp2 = (tmp2 * 125) >> 8;

    *val = (uint16_t)tmp2;
    return false;
}

bool pac193x_current_getse_mA(uint8_t i2c_addr, uint8_t channel, bool avg, uint8_t rsense, uint16_t* val)
{
    if(rsense <= 2)
        return true;

    uint16_t vsense = 0;
    if(pac193x_vsense_getraw(i2c_addr, channel, avg, &vsense))
        return true;

    uint32_t tmp = 100000 / rsense; //FIXME: proper rounding
    tmp *= vsense;
    tmp = tmp / ((uint32_t)1<<16); //FIXME: proper rounding
    *val = (uint16_t)tmp;

    return false;
}
