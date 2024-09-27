#include "RTC_PulseControl.h"
#include "xc.h"
#include "usb_app.h"
#include "Compiler.h"
#include <stdio.h>
#include <stdbool.h>
#include <outcompare.h>

void OC2_SyncTmr3_Settings(enum Zrpm z_rpm);
void OC1_OC2_Cascade_Settings(enum Zrpm z_rpm);

static int z_rpm_idx = 0;

// z_pulse_update_buffer: update square waveform period and duty_on timing should be in isr,
// such that startup pwm after 1ms delay, 
// and update/off oc2 output after completed square waveform.
static Z_Pulse_msg_update_t z_pulse_update_buffer = {0, 0, 0, {0, 0, 0, 0}};

OC_1_2_CascadeDef_t OC_1_2_cascade_def[] FAR = {
    {100, {0x0001, 0x1940, 0x0000, 0x8ca0}},
    {200, {0x0000, 0x8ca0, 0x0000, 0x4650}},
    {300, {0x0000, 0x5dc0, 0x0000, 0x2ee0}},
    {400, {0x0000, 0x4650, 0x0000, 0x2328}},
    {500, {0x0000, 0x383f, 0x0000, 0x1c1f}},
    {600, {0x0000, 0x2ee0, 0x0000, 0x1770}},
    {700, {0x0000, 0x282d, 0x0000, 0x1416}},
    {800, {0x0000, 0x2328, 0x0000, 0x1194}},
    {900, {0x0000, 0x1f40, 0x0000, 0x0fa0}},
    {1000, {0x0000, 0x1c1f, 0x0000, 0x0e0f}},
    {1100, {0x0000, 0x1991, 0x0000, 0x0cc8}},
    {1200, {0x0000, 0x1770, 0x0000, 0x0bb8}},
    {1300, {0x0000, 0x15a2, 0x0000, 0x0ad1}},
    {1400, {0x0000, 0x1416, 0x0000, 0x0a0b}},
    {1500, {0x0000, 0x12c0, 0x0000, 0x0960}},
    {1600, {0x0000, 0x1194, 0x0000, 0x08ca}},
    {1700, {0x0000, 0x108b, 0x0000, 0x0845}},
    {1800, {0x0000, 0x0fa0, 0x0000, 0x07d0}},
    {1900, {0x0000, 0x0ecd, 0x0000, 0x0766}},
    {2000, {0x0000, 0x0e0f, 0x0000, 0x0707}},
    {2100, {0x0000, 0x0d64, 0x0000, 0x06b2}},
    {2200, {0x0000, 0x0cc8, 0x0000, 0x0664}},
    {2300, {0x0000, 0x0c3a, 0x0000, 0x061d}},
    {2400, {0x0000, 0x0bb8, 0x0000, 0x05dc}},
    {2500, {0x0000, 0x0b40, 0x0000, 0x05a0}},
    {2600, {0x0000, 0x0ad1, 0x0000, 0x0568}},
    {2700, {0x0000, 0x0a6a, 0x0000, 0x0535}},
    {2800, {0x0000, 0x0a0b, 0x0000, 0x0505}},
    {2900, {0x0000, 0x09b2, 0x0000, 0x04d9}},
    {3000, {0x0000, 0x0960, 0x0000, 0x04b0}},
    {3100, {0x0000, 0x0912, 0x0000, 0x0489}},
    {3200, {0x0000, 0x08ca, 0x0000, 0x0465}},
    {3300, {0x0000, 0x0885, 0x0000, 0x0442}},
    {3400, {0x0000, 0x0845, 0x0000, 0x0422}},
    {3500, {0x0000, 0x0809, 0x0000, 0x0404}},
    {3600, {0x0000, 0x07d0, 0x0000, 0x03e8}},
};

// oc1 no use
// void __attribute__((interrupt,no_auto_psv)) _OC1Interrupt(void)
// {
//     IFS0bits.OC1IF = 0;
//     LATHbits.LATH15 = !LATHbits.LATH15;
//     NOP20_MACRO();
//     //LATHbits.LATH15 = 0;
//     //pulse_counter+=1;

//     // if(pulse_counter >= 10)
//     // {
//     //     OC1_OFF_MACRO();
//     // }
// }

// Is_period_vary() just for testing
// in order to check period varing transient if OCx pin output spike.
int Is_period_vary(int *spd_idx)
{
    int res = 0;
    static int period_vary_cnt = 0;
    static int b_up_counting = 1;
    period_vary_cnt += 1;
    period_vary_cnt &= 0x1ff;
    if (period_vary_cnt == 0)
    {
        if (b_up_counting == 1)
        {
            *spd_idx += 1;
            if (*spd_idx >= z_rpm_max)
            {
                b_up_counting = 0;
                *spd_idx = z_rpm_max - 1;
            }
        }
        else
        {
            *spd_idx -= 1;
            if (*spd_idx == 0)
                b_up_counting = 1;
        }
    }
    else
        res = -1;
    return res;
}

void z_pulse_gen_off()
{
    Z_PULSE_OFF_MACRO();
    z_pulse_update_buffer.z_pulse_gen_enable = 0;
}

void OC2_SyncTmr3_by_value(int y, int z)
{
    Z_PULSE_OC2_SYNC_TMR3_MACRO(y, z);
    z_pulse_update_buffer.z_pulse_gen_enable = 1;
}
void OC1_OC2_Cascade_by_value(int w, int x, int y, int z)
{
    Z_PULSE_OC1_OC2_CASCADE_MACRO(w, x, y, z);
    z_pulse_update_buffer.z_pulse_gen_enable = 1;
}

void z_pulse_gen_by_value(int period_hiword, int period_loword, int dutyon_hiword, int dutyon_loword)
{
    if (period_hiword > 0)
    {
        OC1_OC2_Cascade_by_value(period_hiword, period_loword, dutyon_hiword, dutyon_loword);
    }
    else
    {
        OC2_SyncTmr3_by_value(dutyon_loword, period_loword);
    }
}
void z_pulse_gen_lookup_table(enum Zrpm rpm_value)
{
    z_rpm_idx = rpm_value;
    int tab_hiword = OC_1_2_cascade_def[z_rpm_idx].z_pulse_cnts.period_hiword;
    if (tab_hiword > 0)
        OC1_OC2_Cascade_Settings(z_rpm_idx);
    else
        OC2_SyncTmr3_Settings(z_rpm_idx);
}

void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void)
{
    IFS0bits.OC2IF = 0;

    // if (Is_period_vary(&z_rpm_idx) == 0)
    // {
    //     z_pulse_gen_lookup_table(z_rpm_idx);
    //     LATHbits.LATH15 = 1;
    //     NOP20_MACRO();
    //     LATHbits.LATH15 = 0;
    // }
    if (z_pulse_update_buffer.z_pulse_update_turn_off == 1)
    {
        z_pulse_update_buffer.z_pulse_update_turn_off = 0;
        z_pulse_update_buffer.z_pulse_update_mutex = 0;
        z_pulse_gen_off();
    }

    if (z_pulse_update_buffer.z_pulse_update_mutex == 1)
    {
        z_pulse_update_buffer.z_pulse_update_mutex = 0;
        z_pulse_gen_by_value(z_pulse_update_buffer.z_pulse_update.period_hiword,
                             z_pulse_update_buffer.z_pulse_update.period_loword,
                             z_pulse_update_buffer.z_pulse_update.dutyon_hiword,
                             z_pulse_update_buffer.z_pulse_update.dutyon_loword);
    }
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    IFS0bits.T3IF = 0;
    TMR3 = 0;

    // if (Is_period_vary(&z_rpm_idx) == 0)
    // {
    //     z_pulse_gen_lookup_table(z_rpm_idx);
    //     LATHbits.LATH15 = 1;
    //     NOP20_MACRO();
    //     LATHbits.LATH15 = 0;
    // }
    if (z_pulse_update_buffer.z_pulse_update_turn_off == 1)
    {
        z_pulse_update_buffer.z_pulse_update_turn_off = 0;
        z_pulse_update_buffer.z_pulse_update_mutex = 0;
        z_pulse_gen_off();
    }

    if (z_pulse_update_buffer.z_pulse_update_mutex == 1)
    {
        z_pulse_update_buffer.z_pulse_update_mutex = 0;
        z_pulse_gen_by_value(z_pulse_update_buffer.z_pulse_update.period_hiword,
                             z_pulse_update_buffer.z_pulse_update.period_loword,
                             z_pulse_update_buffer.z_pulse_update.dutyon_hiword,
                             z_pulse_update_buffer.z_pulse_update.dutyon_loword);
    }
}

// date: 2024, 0926.
// because Z rpm control range 100(or less) ~ 3600 rpm
// the OCxTMR to exceed 0xffff
// (1) trying using tmr3 sync mode, with tmr3-prescaler1_8,
//     --> the t_period is correct, but t_duty_on is incorrect(without prescaler)
//
// (2) trying using oc1 oc2 cascade mode with Peripheral clock
//     --> the OC2RS >=1, fine, but OC2RS=0, pulse-gen fault.
//
// (3) solution: with wide-range pulse generator must combine (1) (2).
//
// (4) OC2_SyncTmr3_Settings:
//     t_duty_on positive-edge(start) depends on OC2R,
//     t_duty_on negative-edge(end) depends on OC2RS,
//     t_period depends tmr3-PR3
// and OC1_OC2_Cascade_Settings:
//     t_duty_on positive-edge(start) depends on OC2R:OC1R,
//     t_duty_on negative-edge(end) depends on t_period,
//     t_period depends OC2RS:OC1RS
// so that, OC2_SyncTmr3 OC2R(start) equal to OC2R:OC1R
//                       OC2RS(end) equal to period
// to prevent the signal sequence with seam.
void OC2_SyncTmr3_Settings(enum Zrpm z_rpm)
{
    int y, z;
    y = OC_1_2_cascade_def[z_rpm].z_pulse_cnts.dutyon_loword;
    z = OC_1_2_cascade_def[z_rpm].z_pulse_cnts.period_loword;
    Z_PULSE_OC2_SYNC_TMR3_MACRO(y, z);
}

void OC1_OC2_Cascade_Settings(enum Zrpm z_rpm)
{
    int w, x, y, z;
    w = OC_1_2_cascade_def[z_rpm].z_pulse_cnts.period_hiword;
    x = OC_1_2_cascade_def[z_rpm].z_pulse_cnts.period_loword;
    y = OC_1_2_cascade_def[z_rpm].z_pulse_cnts.dutyon_hiword;
    z = OC_1_2_cascade_def[z_rpm].z_pulse_cnts.dutyon_loword;
    Z_PULSE_OC1_OC2_CASCADE_MACRO(w, x, y, z);
}

char z_pulse_off_by_usb_msg()
{
    char res = 0;
    if (z_pulse_update_buffer.z_pulse_gen_enable == 1)
    {
        z_pulse_update_buffer.z_pulse_update_turn_off = 1;
        z_pulse_update_buffer.z_pulse_update.period_hiword = 0;
        z_pulse_update_buffer.z_pulse_update.period_loword = 0;
        z_pulse_update_buffer.z_pulse_update.dutyon_hiword = 0;
        z_pulse_update_buffer.z_pulse_update.dutyon_loword = 0;
    }
    else
        res = -1;
    return res;
}

char z_pulse_update_by_usb_msg(int w, int x, int y, int z)
{
    if (z_pulse_update_buffer.z_pulse_update_mutex == 1)
        return -1;
    z_pulse_update_buffer.z_pulse_update_mutex = 1;
    z_pulse_update_buffer.z_pulse_update.period_hiword = w;
    z_pulse_update_buffer.z_pulse_update.period_loword = x;
    z_pulse_update_buffer.z_pulse_update.dutyon_hiword = y;
    z_pulse_update_buffer.z_pulse_update.dutyon_loword = z;
    return 0;
}

char z_pulse_startup_by_tmr()
{
    if (z_pulse_update_buffer.z_pulse_gen_enable == 1)
        return 0;
    if (z_pulse_update_buffer.z_pulse_update_mutex == 1)
    {
        z_pulse_update_buffer.z_pulse_update_mutex = 0;
        z_pulse_gen_by_value(z_pulse_update_buffer.z_pulse_update.period_hiword,
                             z_pulse_update_buffer.z_pulse_update.period_loword,
                             z_pulse_update_buffer.z_pulse_update.dutyon_hiword,
                             z_pulse_update_buffer.z_pulse_update.dutyon_loword);
    }
    return 0;
}