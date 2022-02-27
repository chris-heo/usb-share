#ifndef PAC193X_H_
#define PAC193X_H_

typedef struct  
{
    uint8_t channel;
    uint8_t shunt_mR;
    uint16_t vbus;
    uint16_t vsense;
}   pac1934x_channel_s;

typedef enum
{
    pac193x_addrGND    = 0b0010000,
    pac193x_addr499    = 0b0010001,
    pac193x_addr806    = 0b0010010,
    pac193x_addr1270   = 0b0010011,
    pac193x_addr2050   = 0b0010100,
    pac193x_addr3240   = 0b0010101,
    pac193x_addr5230   = 0b0010110,
    pac193x_addr8450   = 0b0010111,
    pac193x_addr13300  = 0b0011000,
    pac193x_addr21500  = 0b0011001,
    pac193x_addr34000  = 0b0011010,
    pac193x_addr54900  = 0b0011011,
    pac193x_addr88700  = 0b0011100,
    pac193x_addr140000 = 0b0011101,
    pac193x_addr226000 = 0b0011110,
    pac193x_addrVDD    = 0b0011111,
}   pac193x_addr_e;

typedef enum
{
    pac193x_reg_REFRESH         = 0x00, // 1 byte(s), W
    pac193x_reg_CTRL            = 0x01, // 1 byte(s), R/W
    pac193x_reg_ACC_COUNT       = 0x02, // 3 byte(s), R
    pac193x_reg_VPOWER1_ACC     = 0x03, // 6 byte(s), R
    pac193x_reg_VPOWER2_ACC     = 0x04, // 6 byte(s), R
    pac193x_reg_VPOWER3_ACC     = 0x05, // 6 byte(s), R
    pac193x_reg_VPOWER4_ACC     = 0x06, // 6 byte(s), R
    pac193x_reg_VBUS1           = 0x07, // 2 byte(s), R
    pac193x_reg_VBUS2           = 0x08, // 2 byte(s), R
    pac193x_reg_VBUS3           = 0x09, // 2 byte(s), R
    pac193x_reg_VBUS4           = 0x0A, // 2 byte(s), R
    pac193x_reg_VSENSE1         = 0x0B, // 2 byte(s), R
    pac193x_reg_VSENSE2         = 0x0C, // 2 byte(s), R
    pac193x_reg_VSENSE3         = 0x0D, // 2 byte(s), R
    pac193x_reg_VSENSE4         = 0x0E, // 2 byte(s), R
    pac193x_reg_VBUS1_AVG       = 0x0F, // 2 byte(s), R
    pac193x_reg_VBUS2_AVG       = 0x10, // 2 byte(s), R
    pac193x_reg_VBUS3_AVG       = 0x11, // 2 byte(s), R
    pac193x_reg_VBUS4_AVG       = 0x12, // 2 byte(s), R
    pac193x_reg_VSENSE1_AVG     = 0x13, // 2 byte(s), R
    pac193x_reg_VSENSE2_AVG     = 0x14, // 2 byte(s), R
    pac193x_reg_VSENSE3_AVG     = 0x15, // 2 byte(s), R
    pac193x_reg_VSENSE4_AVG     = 0x16, // 2 byte(s), R
    pac193x_reg_VPOWER1         = 0x17, // 4 byte(s), R
    pac193x_reg_VPOWER2         = 0x18, // 4 byte(s), R
    pac193x_reg_VPOWER3         = 0x19, // 4 byte(s), R
    pac193x_reg_VPOWER4         = 0x1A, // 4 byte(s), R
    pac193x_reg_CHANNEL_DIS     = 0x1C, // 1 byte(s), R/W
    pac193x_reg_NEG_PWR         = 0x1D, // 1 byte(s), R/W
    pac193x_reg_REFRESH_G       = 0x1E, // 1 byte(s), W
    pac193x_reg_REFRESH_V       = 0x1F, // 1 byte(s), W
    pac193x_reg_SLOW            = 0x20, // 1 byte(s), R/W
    pac193x_reg_CTRL_ACT        = 0x21, // 1 byte(s), R
    pac193x_reg_CHANNEL_DIS_ACT = 0x22, // 1 byte(s), R
    pac193x_reg_NEG_PWR_ACT     = 0x23, // 1 byte(s), R
    pac193x_reg_CTRL_LAT        = 0x24, // 1 byte(s), R
    pac193x_reg_CHANNEL_DIS_LAT = 0x25, // 1 byte(s), R
    pac193x_reg_NEG_PWR_LAT     = 0x26, // 1 byte(s), R
    pac193x_reg_PID             = 0xFD, // 1 byte(s), R
    pac193x_reg_MID             = 0xFE, // 1 byte(s), R
    pac193x_reg_REV             = 0xFF, // 1 byte(s), R
}   pac193x_reg_e;


//0x00: REFRESH COMMAND
#define PAC193X_OVF_bp 0
#define PAC193X_ALERT_OVF_bp 1
#define PAC193X_ALERT_CC_bp 2
#define PAC193X_ALERT_PIN_bp 3
#define PAC193X_SING_bp 4
#define PAC193X_SLEEP_bp 5
#define PAC193X_SR0_bp 6
#define PAC193X_SR1_bp 7

typedef enum
{
    pac1934x_ctrl_samplerate_1024 = 0,
    pac1934x_ctrl_samplerate_256  = 1,
    pac1934x_ctrl_samplerate_64   = 2,
    pac1934x_ctrl_samplerate_8    = 3,
}   pac1934x_ctrl_samplerate_e;

typedef union
{
    struct  
    {
        bool alert_accumulator_overflow: 1; // R-0
        bool alert_overflow_en: 1; // RW-0
        bool alert_dataready: 1; // RW-0
        bool alert_pin_en: 1; // RW-0
        bool singleshot_en: 1; // RW-0
        bool sleepmode_en: 1; // RW-0
        pac1934x_ctrl_samplerate_e samplerate: 2; // RW-0
    };
    uint8_t data;
}   pac193x_ctrl_s;

//0x1C: CHANNEL_DIS AND SMBUS
#define PAC193X_NOSKIP_bp 1
#define PAC193X_BYTECOUNT_bp 2
#define PAC193X_TIMEOUT_bp 3
#define PAC193X_CH4_OFF_bp 4
#define PAC193X_CH3_OFF_bp 5
#define PAC193X_CH2_OFF_bp 6
#define PAC193X_CH1_OFF_bp 7

//0x1D: NEG_PWR
#define PAC193X_CH4_BIDV_bp 0
#define PAC193X_CH3_BIDV_bp 1
#define PAC193X_CH2_BIDV_bp 2
#define PAC193X_CH1_BIDV_bp 3
#define PAC193X_CH4_BIDI_bp 4
#define PAC193X_CH3_BIDI_bp 5
#define PAC193X_CH2_BIDI_bp 6
#define PAC193X_CH1_BIDI_bp 7

//0x20: SLOW
#define PAC193X_POR_bp 0
#define PAC193X_R_V_FALL_bp 1
#define PAC193X_R_FALL_bp 2
#define PAC193X_R_V_RISE_bp 3
#define PAC193X_R_RISE_bp 4
#define PAC193X_SLOW_HL_bp 5
#define PAC193X_SLOW_LH_bp 6
#define PAC193X_SLOW_bp 7

bool pac193x_ctrl_set(uint8_t i2c_addr, pac193x_ctrl_s data);


void pac193x_refresh(uint8_t i2c_addr, bool reset_accu);
bool pac193x_vbus_getraw(uint8_t i2c_addr, uint8_t channel, bool avg, uint16_t* val);
bool pac193x_vsense_getraw(uint8_t i2c_addr, uint8_t channel, bool avg, uint16_t* val);
bool pac193x_vbus_getse_mV(uint8_t i2c_addr, uint8_t channel, bool avg, uint16_t* val);
bool pac193x_current_getse_mA(uint8_t i2c_addr, uint8_t channel, bool avg, uint8_t rsense, uint16_t* val);
#endif /* PAC193X_H_ */