#include "RTC_PulseControl.h"
#include "xc.h"
#include "usb_app.h"
#include "Compiler.h"
#include <stdio.h>
#include <stdbool.h>
#include <outcompare.h>

void OC2_SyncTmr3_Settings(enum Zrpm z_rpm);
void OC1_OC2_Cascade_Settings(enum Zrpm z_rpm);
char X_Jump_Settings();

static int z_rpm_idx = 0;

// z_pulse_update_buffer: update square waveform period and duty_on timing should be in isr,
// such that startup pwm after 1ms delay, 
// and update/off oc2 output after completed square waveform.
static Z_Pulse_msg_update_t z_pulse_update_buffer = {0, 0, 0, {0, 0, 0, 0}};
static X_Pulse_msg_update_t x_pulse_update_buffer = {0,0,0,0,0};
static X_Pulse_Jump_Para_t x_pulse_jump_para;

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

void OC4_SyncTmr4_Settings(OCx_sequence_t x_oc_value);
void OC3_OC4_Cascade_Settings(OCx_sequence_t x_oc_value);

void x_pulse_settings(OCx_sequence_t x_oc_value)
{
    if (x_oc_value.period.u16[1] > 0)
        OC3_OC4_Cascade_Settings(x_oc_value);
    else
        OC4_SyncTmr4_Settings(x_oc_value);
}

void OC4_SyncTmr4_Settings(OCx_sequence_t x_oc_value)
{
    int y, z;
    y = x_oc_value.dutyon.u16[0];
    z = x_oc_value.period.u16[0];
    X_PULSE_OC4_SYNC_TMR4_MACRO(y, z);
}

void OC3_OC4_Cascade_Settings(OCx_sequence_t x_oc_value)
{
    int w, x, y, z;
    w = x_oc_value.period.u16[1];
    x = x_oc_value.period.u16[0];
    y = x_oc_value.dutyon.u16[1];
    z = x_oc_value.dutyon.u16[0];
    X_PULSE_OC3_OC4_CASCADE_MACRO(w, x, y, z);
}

char OCx_CountDelay_Calculation(OCx_src_t *cn_ref, OCx_sequence_t *cn_seqence, int cn_seq_size, int *cn_seq_idx)
{
    OCx_sequence_t *ptr_dst = cn_seqence;
    int n, den;
    unsigned long num, cn_1, quo;
    for (n = 0; n < cn_seq_size; n++)
    {
        if (n < 2)
        {
            ptr_dst->period.u32 = cn_ref->cx[n].period.u32;
            ptr_dst->dutyon.u32 = cn_ref->cx[n].period.u32 >> 1;
        }
        else
        {
            cn_1 = (ptr_dst - 1)->period.u32;
            num = cn_1 + cn_1;
            den = (n << 2) + 1;
            quo = num / den;
            cn_1 = cn_1 - quo;
            ptr_dst->period.u32 = cn_1;
            ptr_dst->dutyon.u32 = cn_1 >> 1;
        }

        if ((ptr_dst->period.u32) < (cn_ref->cx_last.period.u32))
        {
            *cn_seq_idx = n;
            break;
        }
        ptr_dst += 1;
    }
    return 0;
}


char x_pulse_update_by_usb_msg(OCx_src_t* osx_src, unsigned short steps)
{
    if (x_pulse_update_buffer.x_pulse_update_mutex == 1)
        return -1;
    x_pulse_update_buffer.x_pulse_update_mutex = 1;
    x_pulse_update_buffer.x_steps = steps;
    memset(x_pulse_update_buffer.cx_seq, 0, sizeof(x_pulse_update_buffer.cx_seq));
    OCx_CountDelay_Calculation(osx_src, x_pulse_update_buffer.cx_seq, 64, &x_pulse_update_buffer.x_cx_seq_valid);
    return 0;
}

void OC4_SyncTmr4_by_value(int y, int z)
{
    X_PULSE_OC4_SYNC_TMR4_MACRO(y, z);
    x_pulse_update_buffer.x_pulse_gen_enable = 1;
}
void OC3_OC4_Cascade_by_value(int w, int x, int y, int z)
{
    X_PULSE_OC3_OC4_CASCADE_MACRO(w, x, y, z);
    x_pulse_update_buffer.x_pulse_gen_enable = 1;
}

void x_pulse_gen_by_value(int period_hiword, int period_loword, int dutyon_hiword, int dutyon_loword)
{
    if (period_hiword > 0)
    {
        OC3_OC4_Cascade_by_value(period_hiword, period_loword, dutyon_hiword, dutyon_loword);
    }
    else
    {
        OC4_SyncTmr4_by_value(dutyon_loword, period_loword);
    }
}

// void x_pulse_gen_by_update_buf()
// {
//     if (period_hiword > 0)
//     {
//         OC3_OC4_Cascade_by_value(period_hiword, period_loword, dutyon_hiword, dutyon_loword);
//     }
//     else
//     {
//         OC4_SyncTmr4_by_value(dutyon_loword, period_loword);
//     }
// }

char x_pulse_startup_by_tmr()
{
    if (x_pulse_update_buffer.x_pulse_gen_enable == 1)
        return 0;
    if (x_pulse_update_buffer.x_pulse_update_mutex == 1)
    {
        //x_pulse_update_buffer.z_pulse_update_mutex = 0;     wait steps decay to zero.
        // x_pulse_gen_by_value(x_pulse_update_buffer.cx_seq[0].period.u16[1],
        //                      x_pulse_update_buffer.cx_seq[0].period.u16[0],
        //                      x_pulse_update_buffer.cx_seq[0].dutyon.u16[1],
        //                      x_pulse_update_buffer.cx_seq[0].dutyon.u16[0]);
            X_Jump_Settings();

    }
    return 0;
}

void __attribute__((interrupt, no_auto_psv)) _OC4Interrupt(void)
{
    IFS1bits.OC4IF = 0;
    LATHbits.LATH15 = 1;
    NOP20_MACRO();
    LATHbits.LATH15 = 0;
    // if (Is_period_vary(&z_rpm_idx) == 0)
    // {
    //     z_pulse_gen_lookup_table(z_rpm_idx);
    //     LATHbits.LATH15 = 1;
    //     NOP20_MACRO();
    //     LATHbits.LATH15 = 0;
    // }
    // if (z_pulse_update_buffer.z_pulse_update_turn_off == 1)
    // {
    //     z_pulse_update_buffer.z_pulse_update_turn_off = 0;
    //     z_pulse_update_buffer.z_pulse_update_mutex = 0;
    //     z_pulse_gen_off();
    // }

    // if (z_pulse_update_buffer.z_pulse_update_mutex == 1)
    // {
    //     z_pulse_update_buffer.z_pulse_update_mutex = 0;
    //     z_pulse_gen_by_value(z_pulse_update_buffer.z_pulse_update.period_hiword,
    //                          z_pulse_update_buffer.z_pulse_update.period_loword,
    //                          z_pulse_update_buffer.z_pulse_update.dutyon_hiword,
    //                          z_pulse_update_buffer.z_pulse_update.dutyon_loword);
    // }
}

void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void)
{
    IFS1bits.T4IF = 0;
    TMR4 = 0;

    // if (Is_period_vary(&z_rpm_idx) == 0)
    // {
    //     z_pulse_gen_lookup_table(z_rpm_idx);
    //     LATHbits.LATH15 = 1;
    //     NOP20_MACRO();
    //     LATHbits.LATH15 = 0;
    // }
    // if (z_pulse_update_buffer.z_pulse_update_turn_off == 1)
    // {
    //     z_pulse_update_buffer.z_pulse_update_turn_off = 0;
    //     z_pulse_update_buffer.z_pulse_update_mutex = 0;
    //     z_pulse_gen_off();
    // }

    // if (z_pulse_update_buffer.z_pulse_update_mutex == 1)
    // {
    //     z_pulse_update_buffer.z_pulse_update_mutex = 0;
    //     z_pulse_gen_by_value(z_pulse_update_buffer.z_pulse_update.period_hiword,
    //                          z_pulse_update_buffer.z_pulse_update.period_loword,
    //                          z_pulse_update_buffer.z_pulse_update.dutyon_hiword,
    //                          z_pulse_update_buffer.z_pulse_update.dutyon_loword);
    // }
}

char X_Jump_Settings()
{
    int sum_accel_deaccel_steps = x_pulse_update_buffer.x_cx_seq_valid<<1;
    if (sum_accel_deaccel_steps > x_pulse_update_buffer.x_steps )
    {   // triangle, without max speed.
        x_pulse_jump_para.accel_steps = x_pulse_update_buffer.x_steps>>1;
        x_pulse_jump_para.deaccel_steps = x_pulse_update_buffer.x_steps - x_pulse_jump_para.accel_steps;
        x_pulse_jump_para.fixed_steps = 0;
    }
    else
    {   // trapezoid.
        x_pulse_jump_para.accel_steps = x_pulse_update_buffer.x_cx_seq_valid;
        x_pulse_jump_para.deaccel_steps = x_pulse_update_buffer.x_cx_seq_valid;
        x_pulse_jump_para.fixed_steps = x_pulse_update_buffer.x_steps - x_pulse_jump_para.accel_steps - x_pulse_jump_para.accel_steps;
    }
    x_pulse_jump_para.state = JmpAccel;
    x_pulse_jump_para.steps_counter = 0;
    return 0;
}

char X_Jump_Get_DelayCount_Value()
{
    int period_hiword, period_loword, dutyon_hiword, dutyon_loword;
    switch (x_pulse_jump_para.state)
    {
    case JmpAccel:
        if (x_pulse_jump_para.steps_counter < x_pulse_jump_para.accel_steps)
        {
            period_hiword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.steps_counter].period.u16[1];
            period_loword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.steps_counter].period.u16[0];
            dutyon_hiword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.steps_counter].dutyon.u16[1];
            dutyon_loword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.steps_counter].dutyon.u16[0];
            x_pulse_jump_para.steps_counter += 1;
        }
        else if (x_pulse_jump_para.steps_counter == x_pulse_jump_para.accel_steps)
        {
            period_hiword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].period.u16[1];
            period_loword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].period.u16[0];
            dutyon_hiword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].dutyon.u16[1];
            dutyon_loword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].dutyon.u16[0];
            x_pulse_jump_para.steps_counter = 0;
            x_pulse_jump_para.state = (x_pulse_jump_para.fixed_steps == 0) ? JmpDeaccel : JmpFixed;
        }
        break;

    case JmpDeaccel:

    break;

    case JmpFixed:
        if (x_pulse_jump_para.steps_counter < x_pulse_jump_para.fixed_steps)
        {
            period_hiword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].period.u16[1];
            period_loword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].period.u16[0];
            dutyon_hiword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].dutyon.u16[1];
            dutyon_loword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].dutyon.u16[0];
            x_pulse_jump_para.steps_counter += 1;
        }
        else
        {
            period_hiword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].period.u16[1];
            period_loword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].period.u16[0];
            dutyon_hiword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].dutyon.u16[1];
            dutyon_loword = x_pulse_update_buffer.cx_seq[x_pulse_jump_para.accel_steps].dutyon.u16[0];
            x_pulse_jump_para.steps_counter = 0;
            x_pulse_jump_para.state = JmpDeaccel;
        }
    break;
    }
    
    return 0;
}

// unsigned short int  XYJ_Next_X_Dt(  )
// {
	
// 	unsigned short int dt ;

	

// 	switch (XJump_Status.State)
// 	{
// 	case STATE_ACC:

// 		if (XJump_Status.Steps_in_State != 0)		// first step- special case- load calculated dt
// 		{
// 			XJump_Status.Dt = XJump_Param.ds_div_acc_l / (XJump_Status.Acc_Time/(100 / 2));  // dt = ds / ac / T
// 			if (XJump_Param.ds_div_acc_h)
// 				XJump_Status.Dt <<= XJump_Param.ds_div_acc_h;
// 		}

// 		XJump_Status.Acc_Time += (XJump_Status.Dt >> 1 ) ;	 // divide time by 2 ( keep in int range )
// 		XJump_Status.Steps_in_State++;


// 		// check if reached last step in Acc period

// 		if (XJump_Status.Steps_in_State == XJump_Param.Acc_Steps)
// 		{
			
// 			XJump_Status.State = STATE_FIXED;
// 			XJump_Status.Acc_Time -= (XJump_Status.Dt >> 1) ;		// 'cancel' last addition before first deacc calculation
// 			if (XJump_Param.Fixed_Steps == 0)		// triangle
// 				XJump_Status.State = STATE_DEACC;
// 			XJump_Status.Steps_in_State = 0;
			
// 		}
		
// 		dt = XJump_Status.Dt - XYJ_Profile.ISR_Time ; // subtract the ISR time delay. Assuming positive result!
		
// 		break;


// 	case STATE_FIXED:

// 		XJump_Status.Steps_in_State++;
// 		if (XJump_Status.Steps_in_State == XJump_Param.Fixed_Steps)
// 		{
// 			XJump_Status.State = STATE_DEACC;	// next step - switch to Deacc period
// 			XJump_Status.Steps_in_State = 0;
// 		}
		
// 		dt = XJump_Status.Dt - XYJ_Profile.ISR_Time_No_Calc ; // subtract the ISR time delay. Assuming positive result!
		
// 		break;


// 	case STATE_DEACC:

// 		if (XJump_Status.Steps_in_State >= XJump_Param.Deacc_Steps)
// 			return (0); // no more steps

// 		// protection against (Acc_time/50) getting zero but it should never happen ( note overflow situation )

// 		if (XJump_Status.Acc_Time < 100 / 2)
// 			XJump_Status.Acc_Time = 100 / 2;


// 		XJump_Status.Dt = XJump_Param.ds_div_acc_l / (XJump_Status.Acc_Time / (100 / 2));  // dt = ds / ac / T
// 		if (XJump_Param.ds_div_acc_h)  
// 			XJump_Status.Dt <<= XJump_Param.ds_div_acc_h ;
// 		XJump_Status.Acc_Time -= (XJump_Status.Dt >> 1 ) ;
		
// 		XJump_Status.Steps_in_State++;
		
// 		dt = XJump_Status.Dt - XYJ_Profile.ISR_Time ; // subtract the ISR time delay. Assuming positive result!

// 		break;

// 	}

// 	if (dt > XYJ_Profile. Max_DT_Value )		//checking against maximal dt value
// 		dt = XYJ_Profile. Max_DT_Value;

// 	return ( dt );

// }