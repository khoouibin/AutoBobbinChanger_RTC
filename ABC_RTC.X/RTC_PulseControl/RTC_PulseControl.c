#include "RTC_PulseControl.h"
#include "xc.h"
#include "usb_app.h"
#include "Compiler.h"
#include <stdio.h>
#include <stdbool.h>
#include <outcompare.h>
#include "Timers.h"
#include "RTC_Log.h"

void OC2_SyncTmr3_Settings(enum Zrpm z_rpm);
void OC1_OC2_Cascade_Settings(enum Zrpm z_rpm);
char X_Jump_Settings();
char X_Jump_Update_DelayCount();

static int z_rpm_idx = 0;

// z_pulse_update_buffer: update square waveform period and duty_on timing should be in isr,
// such that startup pwm after 1ms delay,
// and update/off oc2 output after completed square waveform.
static Z_Pulse_msg_update_t z_pulse_update_buffer = {0, 0, 0, {0, 0, 0, 0}};
//static Z_Pulse_filter_t z_pulse_filter = {-1,Zpfilter_Off};
static Z_Pulse_MA_t z_pulse_ma;
static UINT_32 u32_z_pulse_count = 0;
static UINT_32 u32_z_cycle_count = 0;

static X_Pulse_msg_update_t x_pulse_update_buffer = {0, 0, 0, 0, 0};
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

void z_pulse_ma_fillup(UINT_32 pulse_count)
{
    int i;
    u32_union_t *p_z_pulse_ma = &z_pulse_ma.ma_buf[0];
    z_pulse_ma.ma_idx = 0;
    z_pulse_ma.ma_bulk = 0;
    for (i = 0; i < 64; i++)
    {
        p_z_pulse_ma->u32 = pulse_count;
        z_pulse_ma.ma_bulk += pulse_count;
        p_z_pulse_ma += 1;
    }
}

void z_pulse_ma_init()
{
    z_pulse_ma_fillup((UINT_32) Z_SPR1600_5RPM);
    z_pulse_ma.state=FilterOff;
    z_pulse_ma.enable_off = 0;
}

void z_pulse_ma_debug()
{
    char log_msg[60];
    snprintf(log_msg, 60, "z_pulse_ma_debug mabulk:%llu",z_pulse_ma.ma_bulk);
    RTC_LogMsg(Debug_Lev, log_msg);
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

void z_pulse_counter_inc()
{
    usb_msg_z_pulse_gen_reply_t z_pulse_gen_reply;

    // char log_msg[60];
    u32_z_pulse_count += 1;
    if (u32_z_pulse_count >= Z_SPR1600)
    {
        u32_z_pulse_count = 0;
        u32_z_cycle_count += 1;

        z_pulse_gen_reply.cmd_id_rep = RespPositive_Z_PulseGen;
        z_pulse_gen_reply.sub_func = SubFunc_z_pulse_turns_reply;
        z_pulse_gen_reply.turns = 0xffff & ((unsigned short)u32_z_cycle_count);
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&z_pulse_gen_reply, 4);
        // snprintf(log_msg, 60, "z_cycle: %ld",u32_z_cycle_count);
        // RTC_LogMsg(Debug_Lev, log_msg);
    }
}

void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void)
{
    IFS0bits.OC2IF = 0;
    Z_Pulse_MA_t *pZpma = &z_pulse_ma;
    Z_Pulse_msg_update_t *pZub = &z_pulse_update_buffer;
    u32_union_t u32_tmp1, u32_tmp2;

    if (pZpma->enable_off == 1)
    {
        z_pulse_gen_off();
        pZpma->enable_off = 0;
        pZpma->state = FilterOff;
        pZub->z_pulse_update_turn_off = 0;
        u32_z_cycle_count = 0;
        u32_z_pulse_count = 0;
        z_pulse_ma_fillup((UINT_32)Z_SPR1600_5RPM);
        return;
    }
    else
    {
        u32_tmp1.u32 = pZpma->ma_output.u32;
        u32_tmp2.u32 = pZpma->ma_output.u32 >> 1;
        z_pulse_gen_by_value(u32_tmp1.u16[1],
                             u32_tmp1.u16[0],
                             u32_tmp2.u16[1],
                             u32_tmp2.u16[0]);
    }
    z_pulse_counter_inc();
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    IFS0bits.T3IF = 0;
    TMR3 = 0;
    Z_Pulse_MA_t *pZpma = &z_pulse_ma;
    Z_Pulse_msg_update_t *pZub = &z_pulse_update_buffer;
    u32_union_t u32_tmp1, u32_tmp2;

    if (pZpma->enable_off == 1)
    {
        z_pulse_gen_off();
        pZpma->enable_off = 0;
        pZpma->state = FilterOff;
        pZub->z_pulse_update_turn_off = 0;
        u32_z_cycle_count = 0;
        u32_z_pulse_count = 0;
        z_pulse_ma_fillup((UINT_32)Z_SPR1600_5RPM);
        return;
    }
    else
    {
        u32_tmp1.u32 = pZpma->ma_output.u32;
        u32_tmp2.u32 = pZpma->ma_output.u32 >> 1;
        z_pulse_gen_by_value(u32_tmp1.u16[1],
                             u32_tmp1.u16[0],
                             u32_tmp2.u16[1],
                             u32_tmp2.u16[0]);
    }
    z_pulse_counter_inc();
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

char z_pulse_update_by_usb_msg(unsigned int w, unsigned int x, unsigned int y, unsigned int z)
{
    // if (z_pulse_update_buffer.z_pulse_update_mutex == 1)
    //     return -1;
    // z_pulse_update_buffer.z_pulse_update_mutex = 1;
    // z_pulse_update_buffer.z_pulse_update.period_hiword = w;
    // z_pulse_update_buffer.z_pulse_update.period_loword = x;
    // z_pulse_update_buffer.z_pulse_update.dutyon_hiword = y;
    // z_pulse_update_buffer.z_pulse_update.dutyon_loword = z;
    Z_Pulse_MA_t *pZpma = &z_pulse_ma;
    pZpma->ma_input.u16[1]=w;
    pZpma->ma_input.u16[0]=x;
    if (pZpma->state==FilterOff)
    {
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_Z_PULSE_FILTER, 50);
        pZpma->state = FirstRunning;
    }
    // z_pulse_filter.z_pulse_input.period_hiword =w;
    // z_pulse_filter.z_pulse_input.period_loword =x;
    return 0;
}

// to be cancelled.
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
// #define C_period_dummy 0xffff 
// #define Period_500RPM 45000
// #define Z_SPR1600_600RPM 3750
// #define Z_SPR1600_200RPM 11250
// #define Z_SPR1600_5RPM 450000
char Is_z_pulse_FilterTaskRunning()
{
    Z_Pulse_MA_t *pZpma = &z_pulse_ma;
    if (pZpma->state == FilterOff)
        return -1;
    else
        return 0;
}

u32_union_t z_pulse_MA_cal(u32_union_t input)
{
    Z_Pulse_MA_t *pZpma = &z_pulse_ma;

    // u32_union_t *p_z_pulse_ma = &z_pulse_ma.ma_buf[z_pulse_ma.ma_idx];
    long long bulk_tmp;
    // z_pulse_ma.ma_bulk += input.u32;
    // z_pulse_ma.ma_bulk -= p_z_pulse_ma->u32;
    // bulk_tmp = z_pulse_ma.ma_bulk;
    // bulk_tmp>>=6;
    // z_pulse_ma.ma_output.u32 = (UINT_32)bulk_tmp;
    // p_z_pulse_ma->u32 = input.u32;
    // z_pulse_ma.ma_idx += 1;
    // z_pulse_ma.ma_idx &= 0x3f;
    // return z_pulse_ma.ma_output;

    pZpma->ma_bulk += input.u32;
    pZpma->ma_bulk -= pZpma->ma_buf[pZpma->ma_idx].u32;
    bulk_tmp = pZpma->ma_bulk >> 6;
    pZpma->ma_output.u32 = (UINT_32)bulk_tmp;
    pZpma->ma_buf[pZpma->ma_idx].u32 = input.u32;
    pZpma->ma_idx += 1;
    pZpma->ma_idx &= 0x3f;
    return pZpma->ma_output;
}

char z_pulse_MA_RountineTask()
{
    char log_msg[60];
    Z_Pulse_MA_t *pZpma = &z_pulse_ma;
    Z_Pulse_msg_update_t *pZub = &z_pulse_update_buffer;
    u32_union_t ma_out_tmp, u32_tmp;
    if (SysTimer_IsTimerExpiered(RTC_CONTROL_Z_PULSE_FILTER) == 0)
        return 0;

    if (pZpma->state == FirstRunning)
    {
        if (pZpma->ma_input.u32 > Z_SPR1600_200RPM)
        {
            z_pulse_ma_fillup(pZpma->ma_input.u32);
        }
        else
        {
            z_pulse_ma_fillup((UINT_32)Z_SPR1600_200RPM);
        }
        ma_out_tmp = z_pulse_MA_cal(pZpma->ma_input);
        u32_tmp.u32 = ma_out_tmp.u32 >> 1;
        z_pulse_gen_by_value(ma_out_tmp.u16[1],
                             ma_out_tmp.u16[0],
                             u32_tmp.u16[1],
                             u32_tmp.u16[0]);
        snprintf(log_msg, 60, "Z-ma(first-run):%lu", pZpma->ma_output.u32);
        RTC_LogMsg(Debug_Lev, log_msg);

        pZpma->state = ContinueRunning;
    }
    else if (pZpma->state == ContinueRunning)
    {
        if (pZub->z_pulse_update_turn_off == 1)
        {
            u32_tmp.u32 = (UINT_32)Z_SPR1600_180RPM;
            ma_out_tmp = z_pulse_MA_cal(u32_tmp);
            snprintf(log_msg, 60, "Z-ma(Z_SPR1600_180RPM):%lu", pZpma->ma_output.u32);
            RTC_LogMsg(Debug_Lev, log_msg);

            if ((pZpma->ma_output.u32) > Z_SPR1600_200RPM)
            {
                pZpma->enable_off = 1;
                snprintf(log_msg, 60, "Z-ma(ready_off):%lu", pZpma->ma_output.u32);
                RTC_LogMsg(Debug_Lev, log_msg);
            }
        }
        else
        {
            ma_out_tmp = z_pulse_MA_cal(pZpma->ma_input);
            snprintf(log_msg, 60, "Z-ma(running):%lu", pZpma->ma_output.u32);
            RTC_LogMsg(Debug_Lev, log_msg);
        }
    }

    if (pZpma->state == ContinueRunning)
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_Z_PULSE_FILTER, 50);
    return 0;
}

// char z_pulse_FilterRountineTask()
// {
//     Z_Pulse_filter_t *pZPF = &z_pulse_filter;
//     OCx_pulse_count_type_t width_tmp1,width_tmp2;
//     char log_msg[60];
//     if (SysTimer_IsTimerExpiered(RTC_CONTROL_Z_PULSE_FILTER) == 0)
//         return 0;
//     width_tmp1.u16[1] = pZPF->z_pulse_input.period_hiword;
//     width_tmp1.u16[0] = pZPF->z_pulse_input.period_loword;

//     if (z_pulse_update_buffer.z_pulse_gen_enable == 0)
//     {

//         if (width_tmp1.u32 > Z_SPR1600_200RPM)
//         {
//             pZPF->z_pulse_output.period_hiword = width_tmp1.u16[1];
//             pZPF->z_pulse_output.period_loword = width_tmp1.u16[0];
//             width_tmp1.u32 >>= 1;
//             pZPF->z_pulse_output.dutyon_hiword = width_tmp1.u16[1];
//             pZPF->z_pulse_output.dutyon_loword = width_tmp1.u16[0];

//             pZPF->z_pulse_backup.period_hiword = width_tmp1.u16[1];
//             pZPF->z_pulse_backup.period_loword = width_tmp1.u16[0];

//             z_pulse_gen_by_value(pZPF->z_pulse_output.period_hiword,
//                                  pZPF->z_pulse_output.period_loword,
//                                  pZPF->z_pulse_output.dutyon_hiword,
//                                  pZPF->z_pulse_output.dutyon_loword);

//             // snprintf(log_msg, 60, "Ztask(1)- %d,%d,%d,%d,",
//             //          pZPF->z_pulse_output.period_hiword,
//             //          pZPF->z_pulse_output.period_loword,
//             //          pZPF->z_pulse_output.dutyon_hiword,
//             //          pZPF->z_pulse_output.dutyon_loword);
//             // RTC_LogMsg(Debug_Lev, log_msg);
//         }
//         else
//         {
//             width_tmp1.u32 = Z_SPR1600_200RPM;
//             pZPF->z_pulse_output.period_hiword = width_tmp1.u16[1];
//             pZPF->z_pulse_output.period_loword = width_tmp1.u16[0];
//             width_tmp1.u32 >>= 1;
//             pZPF->z_pulse_output.dutyon_hiword = width_tmp1.u16[1];
//             pZPF->z_pulse_output.dutyon_loword = width_tmp1.u16[0];
//             pZPF->z_pulse_backup.period_hiword = width_tmp1.u16[1];
//             pZPF->z_pulse_backup.period_loword = width_tmp1.u16[0];

//             z_pulse_gen_by_value(pZPF->z_pulse_output.period_hiword,
//                                  pZPF->z_pulse_output.period_loword,
//                                  pZPF->z_pulse_output.dutyon_hiword,
//                                  pZPF->z_pulse_output.dutyon_loword);
//             // snprintf(log_msg, 60, "Ztask(2)- %d,%d,%d,%d,",
//             //          pZPF->z_pulse_output.period_hiword,
//             //          pZPF->z_pulse_output.period_loword,
//             //          pZPF->z_pulse_output.dutyon_hiword,
//             //          pZPF->z_pulse_output.dutyon_loword);
//             // RTC_LogMsg(Debug_Lev, log_msg);
//         }
//     }
//     else
//     {
//         width_tmp2.u16[1] = pZPF->z_pulse_output.period_hiword;
//         width_tmp2.u16[0] = pZPF->z_pulse_output.period_loword;
        
//         if (z_pulse_update_buffer.z_pulse_update_turn_off == 1)
//         {
//             width_tmp2.u32 += Z_SPR1600_DEACCEL;
//             if (width_tmp2.u32 > Z_SPR1600_200RPM)
//             {
//                 pZPF->filter_ready_off = 1; 
//                 snprintf(log_msg, 60, "Ztask(off)");
//                 RTC_LogMsg(Debug_Lev, log_msg);
//             }
//             else
//             {
//                 pZPF->filter_ready_off = 0;

//                 pZPF->z_pulse_output.period_hiword = width_tmp2.u16[1];
//                 pZPF->z_pulse_output.period_loword = width_tmp2.u16[0];
//                 width_tmp2.u32 >>= 1;
//                 pZPF->z_pulse_output.dutyon_hiword = width_tmp2.u16[1];
//                 pZPF->z_pulse_output.dutyon_loword = width_tmp2.u16[0];
//                 z_pulse_gen_by_value(pZPF->z_pulse_output.period_hiword,
//                                      pZPF->z_pulse_output.period_loword,
//                                      pZPF->z_pulse_output.dutyon_hiword,
//                                      pZPF->z_pulse_output.dutyon_loword);
//                 // snprintf(log_msg, 60, "Ztask(prepare-off) %d,%d,%d,%d,",
//                 //          pZPF->z_pulse_output.period_hiword,
//                 //          pZPF->z_pulse_output.period_loword,
//                 //          pZPF->z_pulse_output.dutyon_hiword,
//                 //          pZPF->z_pulse_output.dutyon_loword);
//                 // RTC_LogMsg(Debug_Lev, log_msg);
//             }
//         }
//         else
//         {
//             if (width_tmp1.u32 < width_tmp2.u32) // input higher than output
//             {
//                 width_tmp2.u32 -= Z_SPR1600_ACCEL;
//                 if (width_tmp2.u32 < width_tmp1.u32)
//                     width_tmp2.u32 = width_tmp1.u32;
//             }
//             else
//             {
//                 width_tmp2.u32 += Z_SPR1600_DEACCEL;
//                 if (width_tmp2.u32 > width_tmp1.u32)
//                     width_tmp2.u32 = width_tmp1.u32;
//                 if ((width_tmp1.u32 > Z_SPR1600_200RPM) && (width_tmp2.u32 > Z_SPR1600_200RPM))
//                 {
//                     width_tmp2.u32 = width_tmp1.u32;
//                 }
//             }
//             pZPF->z_pulse_output.period_hiword =width_tmp2.u16[1];
//             pZPF->z_pulse_output.period_loword =width_tmp2.u16[0];
//             width_tmp2.u32 >>= 1;
//             pZPF->z_pulse_output.dutyon_hiword = width_tmp2.u16[1];
//             pZPF->z_pulse_output.dutyon_loword = width_tmp2.u16[0];
//             z_pulse_gen_by_value(pZPF->z_pulse_output.period_hiword,
//                                  pZPF->z_pulse_output.period_loword,
//                                  pZPF->z_pulse_output.dutyon_hiword,
//                                  pZPF->z_pulse_output.dutyon_loword);
//             // snprintf(log_msg, 60, "Ztask(3)- %d,%d,%d,%d,",
//             //          pZPF->z_pulse_output.period_hiword,
//             //          pZPF->z_pulse_output.period_loword,
//             //          pZPF->z_pulse_output.dutyon_hiword,
//             //          pZPF->z_pulse_output.dutyon_loword);
//             // RTC_LogMsg(Debug_Lev, log_msg);
//         }
//     }


//     if( pZPF->z_pulse_filter_state == Zpfilter_ContinueRunning)
//         SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_Z_PULSE_FILTER,50); 
//     return 0;
// }

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
            ptr_dst->period.u32 = cn_ref->cx_last.period.u32;
            ptr_dst->dutyon.u32 = cn_ref->cx_last.period.u32 >> 1;
            *cn_seq_idx = n;
            break;
        }
        ptr_dst += 1;
    }
    return 0;
}

char x_pulse_update_by_usb_msg(OCx_src_t *osx_src, unsigned short steps)
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

char x_pulse_startup_by_tmr()
{
    if (x_pulse_update_buffer.x_pulse_gen_enable == 1)
        return 0;
    if (x_pulse_update_buffer.x_pulse_update_mutex == 1)
    {
        X_Jump_Settings();
        X_Jump_Update_DelayCount();
    }
    return 0;
}
void x_pulse_gen_off()
{
    X_PULSE_OFF_MACRO();
    x_pulse_update_buffer.x_pulse_gen_enable = 0;
}

void __attribute__((interrupt, no_auto_psv)) _OC4Interrupt(void)
{
    IFS1bits.OC4IF = 0;
    X_Jump_Update_DelayCount();
}

void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void)
{
    IFS1bits.T4IF = 0;
    TMR4 = 0;
    X_Jump_Update_DelayCount();
}

char X_Jump_Settings()
{
    int sum_ramp_steps = x_pulse_update_buffer.x_cx_seq_valid << 1;
    if (sum_ramp_steps > x_pulse_update_buffer.x_steps)
    { // triangle, without max speed.
        x_pulse_jump_para.accel_steps = x_pulse_update_buffer.x_steps >> 1;
        x_pulse_jump_para.deaccel_steps = x_pulse_update_buffer.x_steps - x_pulse_jump_para.accel_steps;
        x_pulse_jump_para.fixed_steps = 0;
    }
    else
    { // trapezoid.
        x_pulse_jump_para.accel_steps = x_pulse_update_buffer.x_cx_seq_valid;
        x_pulse_jump_para.deaccel_steps = x_pulse_update_buffer.x_cx_seq_valid;
        x_pulse_jump_para.fixed_steps = x_pulse_update_buffer.x_steps - x_pulse_jump_para.accel_steps - x_pulse_jump_para.accel_steps;
    }
    x_pulse_jump_para.state = JmpAccel;
    x_pulse_jump_para.steps_counter = 0;
    return 0;
}



char X_Jump_Update_DelayCount()
{
    char log_msg[60];

    int period_hiword, period_loword, dutyon_hiword, dutyon_loword;
    int tb_idx;
    switch (x_pulse_jump_para.state)
    {
    case JmpAccel:
        tb_idx = x_pulse_jump_para.steps_counter;
        tb_idx = (tb_idx < x_pulse_update_buffer.x_cx_seq_valid - 1) ? tb_idx : x_pulse_update_buffer.x_cx_seq_valid - 1;
        period_hiword = x_pulse_update_buffer.cx_seq[tb_idx].period.u16[1];
        period_loword = x_pulse_update_buffer.cx_seq[tb_idx].period.u16[0];
        dutyon_hiword = x_pulse_update_buffer.cx_seq[tb_idx].dutyon.u16[1];
        dutyon_loword = x_pulse_update_buffer.cx_seq[tb_idx].dutyon.u16[0];

        x_pulse_jump_para.steps_counter += 1;
        if (x_pulse_jump_para.steps_counter >= x_pulse_jump_para.accel_steps)
        {
            x_pulse_jump_para.steps_counter = 0;
            x_pulse_jump_para.state = (x_pulse_jump_para.fixed_steps == 0) ? JmpDeaccel : JmpFixed;
        }
        break;

    case JmpFixed:
        tb_idx = x_pulse_update_buffer.x_cx_seq_valid - 1;
        period_hiword = x_pulse_update_buffer.cx_seq[tb_idx].period.u16[1];
        period_loword = x_pulse_update_buffer.cx_seq[tb_idx].period.u16[0];
        dutyon_hiword = x_pulse_update_buffer.cx_seq[tb_idx].dutyon.u16[1];
        dutyon_loword = x_pulse_update_buffer.cx_seq[tb_idx].dutyon.u16[0];
        x_pulse_jump_para.steps_counter += 1;
        if (x_pulse_jump_para.steps_counter >= x_pulse_jump_para.fixed_steps)
        {
            x_pulse_jump_para.steps_counter = 0;
            x_pulse_jump_para.state = JmpDeaccel;
        }
        break;
    case JmpDeaccel:
        tb_idx = x_pulse_jump_para.deaccel_steps - x_pulse_jump_para.steps_counter - 2;
        tb_idx = (tb_idx < 0) ? 0 : tb_idx;
        period_hiword = x_pulse_update_buffer.cx_seq[tb_idx].period.u16[1];
        period_loword = x_pulse_update_buffer.cx_seq[tb_idx].period.u16[0];
        dutyon_hiword = x_pulse_update_buffer.cx_seq[tb_idx].dutyon.u16[1];
        dutyon_loword = x_pulse_update_buffer.cx_seq[tb_idx].dutyon.u16[0];
        x_pulse_jump_para.steps_counter += 1;
        if (x_pulse_jump_para.steps_counter >= x_pulse_jump_para.deaccel_steps)
        {
            x_pulse_jump_para.steps_counter = 0;
            x_pulse_jump_para.state = JmpTerminate;
        }
        break;
    case JmpTerminate:
        x_pulse_update_buffer.x_pulse_update_mutex = 0;
        x_pulse_gen_off();
        snprintf(log_msg, 60, "x-JmpTerminate");
        RTC_LogMsg(Debug_Lev, log_msg);
        break;
    }

    if (x_pulse_jump_para.state != JmpTerminate)
    {
        x_pulse_gen_by_value(period_hiword, period_loword, dutyon_hiword, dutyon_loword);
    }
    return 0;
}