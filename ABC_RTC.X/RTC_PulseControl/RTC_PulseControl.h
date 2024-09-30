#ifndef _RTC_PulseControl_H_
#define _RTC_PulseControl_H_
#include "Ons_General.h"
#define NOP20_MACRO() \
    {                 \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
        asm("NOP");   \
    }

#define OC1_OFF_MACRO()      \
    {                        \
        OC1CON1 = 0;         \
        OC1CON2 = 0;         \
        OC1CON1bits.OCM = 0; \
    }
#define OC1_PWM_CENTRE_MACRO() \
    {                          \
        OC1CON1bits.OCM = 6;   \
    }
#define OC1_CONTINUOUS_PULSE_MACRO() \
    {                                \
        OC1CON1bits.OCM = 5;         \
    }
#define OC1_PULSE_DUTY_ON_COUNT(x) \
    {                              \
        OC1R = x;                  \
    }
#define OC1_PULSE_DUTY_OFF_COUNT(x) \
    {                               \
        OC1RS = x;                  \
    }

#define Z_PULSE_OFF_MACRO() \
    {                       \
        OC1CON1 = 0;        \
        OC1CON2 = 0;        \
        OC2CON1 = 0;        \
        OC2CON2 = 0;        \
        T3CON = 0;          \
    }

#define Z_PULSE_OC2_SYNC_TMR3_MACRO(Y, Z) \
    {                                     \
        OC1CON1 = 0;                      \
        OC1CON2 = 0;                      \
        OC2CON1 = 0;                      \
        OC2CON2 = 0;                      \
        T3CON = 0;                        \
        TMR3 = 0;                         \
        OC2CON1bits.OCTSEL = 0x07;        \
        OC2CON2bits.SYNCSEL = 0b1101;     \
        OC2CON2bits.OCTRIS = 0;           \
        OC2CON2bits.OC32 = 0;             \
        OC1CON2bits.OC32 = 0;             \
        OC2R = Y;                         \
        OC2RS = Z;                        \
        PR3 = Z;                          \
        OC2CON1bits.OCM = 7;              \
        IPC1bits.OC2IP = 0x06;            \
        IFS0bits.OC2IF = 0;               \
        IEC0bits.OC2IE = 0;               \
        IEC0bits.OC1IE = 0;               \
        IEC0bits.T3IE = 1;                \
        T3CON = 0x8000;                   \
    }

#define Z_PULSE_OC1_OC2_CASCADE_MACRO(W, X, Y, Z) \
    {                                             \
        OC1CON1 = 0;                              \
        OC1CON2 = 0;                              \
        OC2CON1 = 0;                              \
        OC2CON2 = 0;                              \
        T3CON = 0;                                \
        TMR3 = 0;                                 \
        OC2CON1bits.OCTSEL = 0x07;                \
        OC1CON1bits.OCTSEL = 0x07;                \
        OC2RS = W;                                \
        OC1RS = X;                                \
        OC2R = Y;                                 \
        OC1R = Z;                                 \
        OC2CON2bits.SYNCSEL = 0x1f;               \
        OC1CON2bits.SYNCSEL = 0x1f;               \
        OC2CON2bits.OCTRIS = 0;                   \
        OC1CON2bits.OCTRIS = 1;                   \
        OC2CON2bits.OC32 = 1;                     \
        OC1CON2bits.OC32 = 1;                     \
        OC2CON1bits.OCM = 7;                      \
        OC1CON1bits.OCM = 7;                      \
        IPC1bits.OC2IP = 0x06;                    \
        IFS0bits.OC2IF = 0;                       \
        IEC0bits.OC2IE = 1;                       \
        IEC0bits.OC1IE = 0;                       \
        IEC0bits.T3IE = 0;                        \
    }

#define X_PULSE_OC4_SYNC_TMR4_MACRO(Y, Z) \
    {                                     \
        OC3CON1 = 0;                      \
        OC3CON2 = 0;                      \
        OC4CON1 = 0;                      \
        OC4CON2 = 0;                      \
        T4CON = 0;                        \
        TMR4 = 0;                         \
        OC4CON1bits.OCTSEL = 0x07;        \
        OC4CON2bits.SYNCSEL = 0b1110;     \
        OC4CON2bits.OCTRIS = 0;           \
        OC4CON2bits.OC32 = 0;             \
        OC3CON2bits.OC32 = 0;             \
        OC4R = Y;                         \
        OC4RS = Z;                        \
        PR4 = Z;                          \
        OC4CON1bits.OCM = 7;              \
        IPC6bits.OC4IP = 0x06;            \
        IFS1bits.OC4IF = 0;               \
        IEC1bits.OC4IE = 0;               \
        IEC1bits.OC3IE = 0;               \
        IEC1bits.T4IE = 1;                \
        T4CON = 0x8000;                   \
    }

#define X_PULSE_OC3_OC4_CASCADE_MACRO(W, X, Y, Z) \
    {                                             \
        OC3CON1 = 0;                              \
        OC3CON2 = 0;                              \
        OC4CON1 = 0;                              \
        OC4CON2 = 0;                              \
        T4CON = 0;                                \
        TMR4 = 0;                                 \
        OC4CON1bits.OCTSEL = 0x07;                \
        OC3CON1bits.OCTSEL = 0x07;                \
        OC4RS = W;                                \
        OC3RS = X;                                \
        OC4R = Y;                                 \
        OC3R = Z;                                 \
        OC4CON2bits.SYNCSEL = 0x1f;               \
        OC3CON2bits.SYNCSEL = 0x1f;               \
        OC4CON2bits.OCTRIS = 0;                   \
        OC3CON2bits.OCTRIS = 1;                   \
        OC4CON2bits.OC32 = 1;                     \
        OC3CON2bits.OC32 = 1;                     \
        OC4CON1bits.OCM = 7;                      \
        OC3CON1bits.OCM = 7;                      \
        IPC6bits.OC4IP = 0x06;                    \
        IFS1bits.OC4IF = 0;                       \
        IEC1bits.OC4IE = 1;                       \
        IEC1bits.OC3IE = 0;                       \
        IEC1bits.T4IE = 0;                        \
    }

enum Zrpm
{
    z_100rpm = 0,
    z_200rpm = 1,
    z_300rpm = 2,
    z_400rpm = 3,
    z_500rpm = 4,
    z_600rpm = 5,
    z_700rpm = 6,
    z_800rpm = 7,
    z_900rpm = 8,
    z_1000rpm = 9,
    z_1100rpm = 10,
    z_1200rpm = 11,
    z_1300rpm = 12,
    z_1400rpm = 13,
    z_1500rpm = 14,
    z_1600rpm = 15,
    z_1700rpm = 16,
    z_1800rpm = 17,
    z_1900rpm = 18,
    z_2000rpm = 19,
    z_2100rpm = 20,
    z_2200rpm = 21,
    z_2300rpm = 22,
    z_2400rpm = 23,
    z_2500rpm = 24,
    z_2600rpm = 25,
    z_2700rpm = 26,
    z_2800rpm = 27,
    z_2900rpm = 28,
    z_3000rpm = 29,
    z_3100rpm = 30,
    z_3200rpm = 31,
    z_3300rpm = 32,
    z_3400rpm = 33,
    z_3500rpm = 34,
    z_3600rpm = 35,
    z_rpm_max,
};

typedef struct
{
    unsigned short period_hiword;
    unsigned short period_loword;
    unsigned short dutyon_hiword;
    unsigned short dutyon_loword;
} z_pulse_width_modulation_t;

typedef struct
{
    enum Zrpm z_rpm_enum;
} z_pulse_rpm_enum_t;

typedef struct
{
    char z_pulse_gen_enable;
    char z_pulse_update_mutex;
    char z_pulse_update_turn_off;
    z_pulse_width_modulation_t z_pulse_update;
} Z_Pulse_msg_update_t;

typedef struct
{
    unsigned short z_rpm_value;
    z_pulse_width_modulation_t z_pulse_cnts;
} OC_1_2_CascadeDef_t;

typedef union
{
	UINT_32 u32;
	UINT_16 u16[2];
    UCHAR_8 u8[4];
}
OCx_pulse_count_type_t;

typedef struct
{
    OCx_pulse_count_type_t period;
    OCx_pulse_count_type_t dutyon;
} OCx_pulse_width_t;

typedef struct
{
    OCx_pulse_width_t c_0;
    OCx_pulse_width_t c_1;
    OCx_pulse_width_t c_last;
} OCx_pulse_width_cal_input_t;

void z_pulse_gen_lookup_table(enum Zrpm rpm_value);
char z_pulse_off_by_usb_msg();
char z_pulse_update_by_usb_msg(int w, int x, int y, int z);
char z_pulse_startup_by_tmr();

void x_pulse_settings(OCx_pulse_width_t x_pwm);
//char OCx_CountDelay_Calculation(OCx_pulse_width_cal_input_t argvs, OCx_pulse_width_t *Cn[]);
char OCx_CountDelay_Calculation(OCx_pulse_width_t *cn_ref, OCx_pulse_width_t *cn_seqence);

#endif
