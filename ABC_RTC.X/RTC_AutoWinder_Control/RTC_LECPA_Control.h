#ifndef _RTC_LECPA_Control_H_
#define _RTC_LECPA_Control_H_
#include "Ons_General.h"
#include "IO_Entity.h"

typedef enum
{
    INPUT_BUSY_PIN14 = IO_YN_SENSOR_ENTITY,                   // LDR_YN_SENSOR-PORTKbits.RK12
    INPUT_SETON_PIN15 = IO_YF_SENSOR_ENTITY,                  // LDR_YF_SENSOR-PORTKbits.RK13
    INPUT_SVRE_PIN17 = IO_ZL_SENSOR_ENTITY,                   // LDR_ZL_SENSOR-PORTKbits.RK15
    INPUT_ALARM_PIN19 = IO_ZU_SENSOR_ENTITY,                  // LDR_ZU_SENSOR-PORTFbits.RF0
    OUTPUT_NP_DIR_PIN4 = IO_R_DIRECTION_ENTITY,               // R_DIRECTIONt-LATJbits.LATJ8
    OUTPUT_PP_PULSE_PIN6 = IO_R_STEP_ENTITY,                  // R_STEPt-LATJbits.LATJ7
    OUTPUT_SETUP_HOME_PIN7 = IO_PUNCHER_PISTON_UP_ENTITY,     // PUNCHER_PISTON_UPt
    OUTPUT_RESET_CLRALARM_PIN8 = IO_RIVET_MAGNET_ENTITY,        // D62,TP15,    J6-2
    OUTPUT_SVON_SERVO_ON_PIN9 = IO_AIR_VALVE_ON_ENTITY,         // none,        J6-1
} IO_LECPA_Entity_t;

typedef enum
{
    Routine_Idle = 0,
    Routine_TaskStart = 1,
    Routine_ALARM_ReadyCheck = 2,
    Routine_SVON_CmdOn = 3,
    Routine_SVRE_ReadyCheck = 4,
    Routine_SETUP_CmdOn = 5,
    Routine_SETUP_CmdOff = 6,
    Routine_BUSY_Polling = 7,
    Routine_BUSY_Finished = 8,
    Routine_TaskEnd = 9,
} LECPA_Home_Routine_t;

typedef enum
{
    HomeState_Abort = -4,
    HomeState_Timeout =-3,
    HomeState_Fail = -2,
    HomeState_Deny = -1,
    HomeState_Idle = 0,
    HomeState_Start = 1,
    HomeState_Wait = 2,
    HomeState_End = 3,
} LECPA_Home_State_t;

typedef enum
{
    SERVO_OFF = 0,
    SERVO_ON = 1,
} IO_LECPA_ServoDef_t;

typedef struct
{
    char prepare_abort;
    unsigned long timeout_cnt;
    LECPA_Home_Routine_t home_routine;
    LECPA_Home_State_t home_state;
} LECPA_Home_Status_t;

typedef struct
{
    unsigned long polling_period_ms;
    unsigned long timeout_sec;
    unsigned long timeout_base_polling_period;
} LECPA_Home_Parameters_t;

void RTC_LECPA_30_ServoCmd(IO_LECPA_ServoDef_t servo_cmd);
char Is_LECPA_30_HomeRoutine_Idle();
char Get_LECPA_30_HomeRoutine();
char Get_LECPA_30_HomeState();

char Set_LECPA_30_HomeStart();
char Set_LECPA_30_HomeAbort();
char LECPA_30_HomeRountineTask();

#endif
