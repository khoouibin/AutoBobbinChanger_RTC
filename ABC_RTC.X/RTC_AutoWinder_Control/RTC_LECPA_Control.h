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
    OUTPUT_RESET_CLRALARM_PIN8 = IO_PUNCHER_PISTON_DN_ENTITY, // PUNCHER_PISTON_DOWNt
    OUTPUT_SVON_SERVO_ON_PIN9 = IO_R_SERVO_ON_ENTITY,         // R_SERVO_ONt
} IO_LECPA_Entity_t;

typedef enum
{
    LECPA_Home_Deny = -1,
    LECPA_Home_Start = 0,
    LECPA_Home_Wait = 1,
    LECPA_Home_End = 2,
} LECPA_Home_Status_t;

typedef enum
{
    LECPA_Init = 0,
    LECPA_ALARM_Ready,
    LECPA_SVRE_Ready,
    LECPA_SETUP_Command,
    LECPA_BUSY_Ready,
} LECPA_Home_Procedure_t;

typedef enum
{
    SERVO_OFF = 0,
    SERVO_ON = 1,
} IO_LECPA_ServoDef_t;

typedef struct
{
    LECPA_Home_Procedure_t HomeProcedure;
    char HomePollingTimerCount;
    char HomeEnable;
    char HomeResult;
} LECPA_Home_Status_t;

void RTC_LECPA_30_ServoCmd(IO_LECPA_ServoDef_t servo_cmd);
char Is_LECPA_30_HomeProcedure_Idle();




#endif
