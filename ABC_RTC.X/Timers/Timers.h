// ---------------------------------------------------------------------------
//  Filename: Timers.h
//  Created by: Roey Huberman
//  Date:  07/07/15
//  Orisol (c)
// ---------------------------------------------------------------------------

#ifndef _TIMERS_H_
#define _TIMERS_H_

#define Fcy 60000000L
#define Delay_1uS_Cnt (Fcy / 1000000)
#define Delay_1mS_Cnt (Fcy / 1000)
#define TCY_CNT_PER_LOOP 15 // insted of 6 fix by adam
#define Timer9TogglesPerSec 1000
#define Timer9Tick ((Fcy / 8) / Timer9TogglesPerSec)

#define BAUDRATE 57600 // 115200 too fast, will have error bit.
#define BRGVAL ((Fcy / BAUDRATE) / 16) - 1
#define UART_BUFFER_SIZE 16
#include "RTC_Control.h"

typedef struct
{
	unsigned long ended_ms;
	unsigned long period_ms;
	char carry_flag;
} TimersHandler_t;

// static unsigned long Timers_Handles[ RTC_CONTROL_TIMER_MAX ];
typedef enum
{
	USBTimer_Interrupt_Fast_Speed = 0,
	USBTimer_Interrupt_Slow_Speed = 1,
	USBTimer_Interrupt_Delayed_Speed = 2
} USBTimer_Interrupt_Mode_State_t;

typedef enum
{
	USBTimer_Rx = 0,
	USBTimer_Tx = 1
} USBTimer_Direction_t;

void Init_Timers(void);
void USBTimer_SetInterruptTimeMode(USBTimer_Interrupt_Mode_State_t Mode, USBTimer_Direction_t Direction);
unsigned long SysTimer_GetTimeInMiliSeconds(void);
void SysTimer_SetTimerInMiliSeconds(RTC_Control_Timer_type_t Timer_Handle, unsigned long Delta_Time);
int SysTimer_IsTimerExpiered(RTC_Control_Timer_type_t Timer_Handle);
void Delay1usX(unsigned int count);
void ClearIntrflags(void);
char GetTimerPeriod(unsigned long t_k, unsigned long *t_k_1, unsigned long t_period);

#endif
