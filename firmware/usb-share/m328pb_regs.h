#ifndef M328PB_REGS_H_
#define M328PB_REGS_H_


typedef enum
{
    ADMUX_MUX_ADC0 = (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0),
    ADMUX_MUX_ADC1 = (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (1<<MUX0),
    ADMUX_MUX_ADC2 = (0<<MUX3) | (0<<MUX2) | (1<<MUX1) | (0<<MUX0),
    ADMUX_MUX_ADC3 = (0<<MUX3) | (0<<MUX2) | (1<<MUX1) | (1<<MUX0),
    ADMUX_MUX_ADC4 = (0<<MUX3) | (1<<MUX2) | (0<<MUX1) | (0<<MUX0),
    ADMUX_MUX_ADC5 = (0<<MUX3) | (1<<MUX2) | (0<<MUX1) | (1<<MUX0),
    ADMUX_MUX_ADC6 = (0<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0),
    ADMUX_MUX_ADC7 = (0<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0),

    ADMUX_MUX_TEMP = (1<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0),
    ADMUX_MUX_RESERVED1 = (1<<MUX3) | (0<<MUX2) | (0<<MUX1) | (1<<MUX0),
    ADMUX_MUX_RESERVED2 = (1<<MUX3) | (0<<MUX2) | (1<<MUX1) | (0<<MUX0),
    ADMUX_MUX_RESERVED3 = (1<<MUX3) | (0<<MUX2) | (1<<MUX1) | (1<<MUX0),
    ADMUX_MUX_RESERVED4 = (1<<MUX3) | (1<<MUX2) | (0<<MUX1) | (0<<MUX0),
    ADMUX_MUX_RESERVED5 = (1<<MUX3) | (1<<MUX2) | (0<<MUX1) | (1<<MUX0),
    ADMUX_MUX_VBG = (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0),
    ADMUX_MUX_GND = (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0),
}   ADMUX_MUX_t;

typedef enum
{
    ADCSRA_ADPS_DIV2A  = (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0),
    ADCSRA_ADPS_DIV2B  = (0<<ADPS2) | (0<<ADPS1) | (1<<ADPS0),
    ADCSRA_ADPS_DIV4   = (0<<ADPS2) | (1<<ADPS1) | (0<<ADPS0),
    ADCSRA_ADPS_DIV8   = (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0),
    ADCSRA_ADPS_DIV16  = (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0),
    ADCSRA_ADPS_DIV32  = (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0),
    ADCSRA_ADPS_DIV64  = (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0),
    ADCSRA_ADPS_DIV128 = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0),
}   ADCSRA_ADPS_t;

typedef enum
{
    ADCSRB_ADTS_FREERUN = (0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0),
    ADCSRB_ADTS_ANACOMP = (0<<ADTS2) | (0<<ADTS1) | (1<<ADTS0),
    ADCSRB_ADTS_EXTIRQ0 = (0<<ADTS2) | (1<<ADTS1) | (0<<ADTS0),
    ADCSRB_ADTS_TC0COMA = (0<<ADTS2) | (1<<ADTS1) | (1<<ADTS0),
    ADCSRB_ADTS_TC0OVF  = (1<<ADTS2) | (0<<ADTS1) | (0<<ADTS0),
    ADCSRB_ADTS_TC1COMB = (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0),
    ADCSRB_ADTS_TC1OVF  = (1<<ADTS2) | (1<<ADTS1) | (0<<ADTS0),
    ADCSRB_ADTS_TC1CAPT = (1<<ADTS2) | (1<<ADTS1) | (1<<ADTS0),
}   ADCSRB_ADTS_t;

typedef enum
{
    TCCR1A_WGM_NORMAL     = (0<<WGM11) | (0<<WGM10),
    TCCR1A_WGM_PWM_PC8    = (0<<WGM11) | (1<<WGM10),
    TCCR1A_WGM_PWM_PC9    = (1<<WGM11) | (0<<WGM10),
    TCCR1A_WGM_PWM_PC10   = (1<<WGM11) | (1<<WGM10),
    TCCR1A_WGM_CTC_OCR    = (0<<WGM11) | (0<<WGM10),
    TCCR1A_WGM_FPWM8      = (0<<WGM11) | (1<<WGM10),
    TCCR1A_WGM_FPWM9      = (1<<WGM11) | (0<<WGM10),
    TCCR1A_WGM_FPWM10     = (1<<WGM11) | (1<<WGM10),
    TCCR1A_WGM_PWMPFC_ICR = (0<<WGM11) | (0<<WGM10),
    TCCR1A_WGM_PWMPFC_OCR = (0<<WGM11) | (1<<WGM10),
    TCCR1A_WGM_PWMPC_ICR  = (1<<WGM11) | (0<<WGM10),
    TCCR1A_WGM_PWMPC_OCR  = (1<<WGM11) | (1<<WGM10),
    TCCR1A_WGM_CTC_ICR    = (0<<WGM11) | (0<<WGM10),
    TCCR1A_WGM_RESERVED   = (0<<WGM11) | (1<<WGM10),
    TCCR1A_WGM_FPWM_ICR   = (1<<WGM11) | (0<<WGM10),
    TCCR1A_WGM_FPWM_OCR   = (1<<WGM11) | (1<<WGM10),
}   TCCR1A_WGM_t;

typedef enum
{
    TCCR1B_WGM_NORMAL     = (0<<WGM13) | (0<<WGM12),
    TCCR1B_WGM_PWM_PC8    = (0<<WGM13) | (0<<WGM12),
    TCCR1B_WGM_PWM_PC9    = (0<<WGM13) | (0<<WGM12),
    TCCR1B_WGM_PWM_PC10   = (0<<WGM13) | (0<<WGM12),
    TCCR1B_WGM_CTC_OCR    = (0<<WGM13) | (1<<WGM12),
    TCCR1B_WGM_FPWM8      = (0<<WGM13) | (1<<WGM12),
    TCCR1B_WGM_FPWM9      = (0<<WGM13) | (1<<WGM12),
    TCCR1B_WGM_FPWM10     = (0<<WGM13) | (1<<WGM12),
    TCCR1B_WGM_PWMPFC_ICR = (1<<WGM13) | (0<<WGM12),
    TCCR1B_WGM_PWMPFC_OCR = (1<<WGM13) | (0<<WGM12),
    TCCR1B_WGM_PWMPC_ICR  = (1<<WGM13) | (0<<WGM12),
    TCCR1B_WGM_PWMPC_OCR  = (1<<WGM13) | (0<<WGM12),
    TCCR1B_WGM_CTC_ICR    = (1<<WGM13) | (1<<WGM12),
    TCCR1B_WGM_RESERVED   = (1<<WGM13) | (1<<WGM12),
    TCCR1B_WGM_FPWM_ICR   = (1<<WGM13) | (1<<WGM12),
    TCCR1B_WGM_FPWM_OCR   = (1<<WGM13) | (1<<WGM12),
}   TCCR1B_WGM_t;

typedef enum
{
    TCCR1B_CS_STOP = (0<<CS12) | (0<<CS11) | (0<<CS10),
    TCCR1B_CS_DIV1 = (0<<CS12) | (0<<CS11) | (1<<CS10),
    TCCR1B_CS_DIV8 = (0<<CS12) | (1<<CS11) | (0<<CS10),
    TCCR1B_CS_DIV64 = (0<<CS12) | (1<<CS11) | (1<<CS10),
    TCCR1B_CS_DIV256 = (1<<CS12) | (0<<CS11) | (0<<CS10),
    TCCR1B_CS_DIV1024 = (1<<CS12) | (0<<CS11) | (1<<CS10),
    TCCR1B_CS_EXT_T1F = (1<<CS12) | (1<<CS11) | (0<<CS10),
    TCCR1B_CS_EXT_T1R = (1<<CS12) | (1<<CS11) | (1<<CS10),
}   TCCR1B_CS_t;

#endif /* M328PB_REGS_H_ */