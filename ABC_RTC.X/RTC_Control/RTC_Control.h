// ---------------------------------------------------------------------------
//  Filename: Timers.h
//  Created by: Roey Huberman
//  Date:  07/07/15
//  Orisol (c) 
// ---------------------------------------------------------------------------

#ifndef _RTC_Control_H_
#define	_RTC_Control_H_

#define AT_200h __attribute__((space(prog), address(0x200),section(".RTC_Version")))
#define AT_208h __attribute__((space(prog), address(0x208),section(".MsgProt_Version") ))
#define AT_210h __attribute__((space(prog), address(0x210),section(".BL_Version")))
#define AT_218h __attribute__((space(prog), address(0x218),section(".ParaNull")))
#define AT_220h __attribute__((space(prog), address(0x220),section(".Version_Msg")))

#define C_RTC_CONTROL_WINK_ms 450
#define C_MIN_RTC_CONTROL_RESET_DELAY_ms 5

typedef enum
{
    RTC_CONTROL_STATE_UNDEFINE = -1,
    RTC_CONTROL_STATE_UNINIT = 0,
    RTC_CONTROL_STATE_READY = 1,
    RTC_CONTROL_STATE_HOME = 2,
    RTC_CONTROL_STATE_DIAGNOSIS = 3,
    RTC_CONTROL_RESET = 99
} RTC_Control_State_t;

typedef enum
{
    PASS = 0,
    CONTINUE = 1,
} CommonMsg_Actions_t;

typedef enum
{
	RTC_CONTROL_WINK = 0,
    RTC_CONTROL_RESET_DELAY = 1,
    RTC_CONTROL_ENTITYTABLE_POLLING = 2,
	RTC_CONTROL_MAX,
} RTC_Control_Timer_type_t;

typedef enum
{
    CONTROL_FATAL_ERROR                            = -1,
	CONTROL_NO_EVENT                               = 0,
    CONTROL_PERFORM_RESET                          = 99     
} RTC_Control_Event_t;

RTC_Control_State_t RTC_Control_Main(void);


#endif

