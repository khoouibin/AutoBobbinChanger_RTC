// ---------------------------------------------------------------------------
//  Filename: Timers.c
//  Created by: Roey Huberman
//  Date:  07/07/15
//  Orisol (c) 
// ---------------------------------------------------------------------------

#include "Timers.h"
#include "RTC_Control.h"
#include "Ons_General.h"
#include "xc.h"
#include "usb_app.h"

// #include "usb.h"
// #include "usb_function_generic.h"
// #include "USBComm.h"
// #include "USBComm_Driver.h"
// #include "RTC_IOports.h"
// #include "IO_Control.h"

#define USBTimer_FAST_INTERVAL_TIME_IN_MS    1
#define USBTimer_SLOW_INTERVAL_TIME_IN_MS    3
#define USBTimer_DELAYED_INTERVAL_TIME_IN_MS 10

#define TIME_MAX_VALUE 4294967295
#define TIME_INIT_VALUE 0

static unsigned long Tickx1ms;
static unsigned char USBTimer_1msTick;
static unsigned char USBTimer_TimeMode;
static unsigned long Tickx1s_Flag;
static TimersHandler_t TimersHandler[RTC_CONTROL_TIMER_MAX] = {};

void Init_Timers(void)
{
    int index;
    IPC1bits.T2IP = 4;
    IEC0bits.T2IE = 0;
    IPC2bits.T3IP = 4;
    IPC7bits.T5IP = 3;
    IEC1bits.T5IE = 1;
    IPC11bits.T6IP = 3;
    IEC2bits.T6IE = 1;

    T5CON = 0;
    TMR5 = 0;
    PR5 = 0;

    IPC7bits.T5IP = 3;
    IEC1bits.T5IE = 1;

    // time tick - T9, clock source of time tick
    T9CON = 0x8010; // Prescale 1_8
    TMR9 = 0;
    PR9 = Timer9Tick;
    IPC13bits.T9IP = 4;
    IEC3bits.T9IE = 1;
    Tickx1ms = 0;
    USBTimer_1msTick = 0;
    USBTimer_TimeMode = USBTimer_SLOW_INTERVAL_TIME_IN_MS;
    for (index = 0; index < RTC_CONTROL_TIMER_MAX; index++)
    {
        TimersHandler[index].ended_ms = TIME_INIT_VALUE;
        TimersHandler[index].period_ms = TIME_INIT_VALUE;
        TimersHandler[index].carry_flag = TIME_INIT_VALUE;
    }
}

void __attribute__((interrupt, no_auto_psv)) _T9Interrupt(void)
{
    IFS3bits.T9IF = 0;
    TMR9 = 0;

    Tickx1ms++;
    Tickx1s_Flag++;
    USBTimer_1msTick++;

    BL_USB_Tx_1mISR_Set();

    /*
    if ( USBTimer_1msTick >= USBTimer_TimeMode )
    {
        USBComm_Get_Module_State( &usb_status );
        if ( usb_status ==  USBCOMM_READY_STATE )
        {
            USBComm_Rx_Int_Main();
            USBComm_Tx_Int_Main();
        }
        else if ( usb_status == USBCOMM_INIT_STATE )
        {
            if( !USBCheckFirstInit() )
            {
                USBComm_Set_Init_Complete();
            }
        }
        else
            USBTimer_TimeMode = USBTimer_DELAYED_INTERVAL_TIME_IN_MS;
        USBTimer_1msTick = 0;
    }*/

    // TMR9 = 0;
    // T9CONbits.TON = 1;
}

// ---------------------------------------------------------------------------
//  Function name		: USBTimer_SetInterruptTimeMode
//
//  Purpose				: This function set the time for the USB interrupt  
//
//  Inputs				: USBTimer_Interrupt_Mode_State_t - Fast , slow or delayed
//
//  Outputs				: 
//
//  Returns				: 
//
//  Description			: Set USB interrupt time.
//
// ---------------------------------------------------------------------------
void USBTimer_SetInterruptTimeMode( USBTimer_Interrupt_Mode_State_t Mode , USBTimer_Direction_t Direction )
{
    static char rx_mode = USBTimer_SLOW_INTERVAL_TIME_IN_MS;
    static char tx_mode = USBTimer_SLOW_INTERVAL_TIME_IN_MS;
    unsigned char interval_time;
    
    switch ( Mode )
    {
        case USBTimer_Interrupt_Fast_Speed:	
            interval_time = USBTimer_FAST_INTERVAL_TIME_IN_MS;
            break;
        case USBTimer_Interrupt_Slow_Speed:
            interval_time = USBTimer_SLOW_INTERVAL_TIME_IN_MS;
            break;
        case USBTimer_Interrupt_Delayed_Speed:
            interval_time = USBTimer_DELAYED_INTERVAL_TIME_IN_MS;
            break;
    }
    
    if( Direction == USBTimer_Rx )
    {
        rx_mode = interval_time;
    }
    else if( Direction == USBTimer_Tx )
    {
        tx_mode = interval_time;
    }
    else
        return;
    
    if( rx_mode == USBTimer_DELAYED_INTERVAL_TIME_IN_MS || tx_mode == USBTimer_DELAYED_INTERVAL_TIME_IN_MS )
    {
        USBTimer_TimeMode = USBTimer_DELAYED_INTERVAL_TIME_IN_MS;
    }
    else if ( rx_mode < tx_mode )
    {
        USBTimer_TimeMode = rx_mode;
    }
    else
    {
        USBTimer_TimeMode = tx_mode;
    }
}

unsigned long SysTimer_GetTimeInMiliSeconds(void)
{
    return Tickx1ms;
}

void SysTimer_SetTimerInMiliSeconds(RTC_Control_Timer_type_t Timer_Handle, unsigned long Delta_Time)
{
    unsigned long current_time;
    U32_t time_tmp;
    current_time = SysTimer_GetTimeInMiliSeconds();
    TimersHandler[Timer_Handle].period_ms = Delta_Time;
    if ((current_time + Delta_Time) > current_time)
    {
        TimersHandler[Timer_Handle].ended_ms = current_time + Delta_Time;
        TimersHandler[Timer_Handle].carry_flag = 0;
    }
    else
    { // overflow
        time_tmp.u32 = TIME_MAX_VALUE - current_time;
        time_tmp.u32 += Delta_Time;
        TimersHandler[Timer_Handle].ended_ms = time_tmp.u32;
        TimersHandler[Timer_Handle].carry_flag = 1;
    }
}

//  Returns				: 1 - timer expired
//                        0 - timer did not expired
int SysTimer_IsTimerExpiered(RTC_Control_Timer_type_t Timer_Handle)
{
    unsigned long current_time;
    U32_t time_tmp;
    current_time = SysTimer_GetTimeInMiliSeconds();
    if (TimersHandler[Timer_Handle].carry_flag == 0)
    {
        if (current_time >= TimersHandler[Timer_Handle].ended_ms)
            return 1;
    }
    else
    {
        time_tmp.u32 = TIME_MAX_VALUE - current_time;
        time_tmp.u32 += TimersHandler[Timer_Handle].period_ms;
        if (time_tmp.u32 >= TimersHandler[Timer_Handle].ended_ms)
            return 1;
    }
    return 0;
}

void Delay1usX( unsigned int count)
{
    unsigned int j1,j2;

    for(j1=0; j1<count; j1++)
        for (j2=0; j2<Delay_1uS_Cnt; j2+=TCY_CNT_PER_LOOP)
                asm("clrwdt");
			
}

void ClearIntrflags( void )
{
    IFS0 = 0;
    IFS1 = 0;
    IFS2 = 0;
    IFS3 = 0;
    IFS4 = 0;    
}

char GetTimerPeriod(unsigned long t_k, unsigned long *t_k_1, unsigned long t_period)
{
    char res = -1;
    U32_t time_diff;
    U32_t time_k_1;
    time_diff.u32 = 0;
    time_k_1.u32 = *t_k_1;
    if (t_k < time_k_1.u32)
    {
        time_diff.u32 = 0 - time_diff.u32;
        time_diff.u32 = time_diff.u32 - time_k_1.u32;
        time_diff.u32 = time_diff.u32 + t_k;
    }
    else
    {
        time_diff.u32 = t_k - time_k_1.u32;
    }

    if (time_diff.u32 >= t_period)
    {
        time_k_1.u32 = t_k;
        *t_k_1 = time_k_1.u32;
        res = 0;
    }
    return res;
}