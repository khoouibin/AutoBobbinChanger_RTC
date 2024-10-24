#ifndef _RTC_LECPA_Control_H_
#define _RTC_LECPA_Control_H_
#include "Ons_General.h"
#include "IO_Entity.h"

#define C_INPUT_ENTITY_ACTIVE 0
#define C_INPUT_ENTITY_INACTIVE 1

typedef enum
{
    INPUT_BUSY_PIN14 = IO_Y_HOME_SENSOR_ENTITY,  
    INPUT_SETON_PIN15 = IO_X_HOME_SENSOR_ENTITY,
    INPUT_SVRE_PIN17 = IO_AIR_SENSOR_ENTITY,
    INPUT_ALARM_PIN19 = IO_ZU_SENSOR_ENTITY,            
    OUTPUT_NP_DIR_PIN4 = IO_R_DIRECTION_ENTITY,           
    OUTPUT_PP_PULSE_PIN6 = IO_R_STEP_ENTITY,             
    OUTPUT_SETUP_HOME_PIN7 = IO_AIR_VALVE_ON_ENTITY,    
    OUTPUT_RESET_CLRALARM_PIN8 = IO_CLAMP_LEFT_LOCK_ENTITY,     
    OUTPUT_SVON_SERVO_ON_PIN9 = IO_X_SERVO_ON_ENTITY,
} IO_LECPA_Entity_t;

typedef enum
{
    Routine_Idle = 0,
    Routine_TaskStart = 1,
    Routine_RESET_Cmdoff = 2,
    Routine_ALARM_ReadyCheck = 3,
    Routine_SVON_CmdOn = 4,
    Routine_SVRE_ReadyCheck = 5,
    Routine_SETUP_CmdToggle = 6,
    Routine_BUSY_Polling = 7,
    Routine_SETON_Polling = 8,
    Routine_TaskEnd = 9,
} LECPA_Home_Routine_t;

typedef enum
{
    HomeState_Abort = -8,//250
    HomeState_Timeout =-7,//251
    HomeState_Fail = -6,//252
    HomeState_BusyPIN14NotOff = -5,
    HomeState_BusyPIN14NotTrigger = -4,//253
    HomeState_SvrePIN17NotReady = -3,//253
    HomeState_AlarmPIN19NotReady = -2,//254
    HomeState_Deny = -1,//255
    HomeState_Idle = 0,
    HomeState_Start = 1,
    HomeState_Wait = 2,
    HomeState_SETON = 3,
} LECPA_Home_State_t;

typedef enum
{
    Drive_NotReady = -1,
    Drive_Ready = 0,
    Drive_Moving_OrgPoint = 1,
    Drive_Moving_MinPoint = 2,
    Drive_Moving_MaxPoint = 3,
    Drive_Moving_AnyPoint = 4,
} LECPA_Drive_State_t;

typedef enum
{
    DriStage_Null = -1,
    DriStage_Init = 0,
    DriStage_SetDir=1,
    DriStage_SetPulse=2,
    DriStage_waitBUSY_On=3,
    DriStage_waitBUSY_Off=4,
    DriStage_Complete=5,
} LECPA_Drive_Stage_t;

typedef enum
{
    SERVO_OFF = 0,
    SERVO_ON = 1,
} LECPA_ServoDef_t;

typedef enum
{
    SERVO_OFF_EntityValue = 0,
    SERVO_ON_EntityValue = 1,
} IO_LECPA_ServoEntity_t;

typedef enum
{
    Drive_Command_Null = 0,
    Drive_Move_OrgPoint = 1,
    Drive_Move_MinPoint = 2,
    Drive_Move_MaxPoint = 3,
    Drive_Move_AnyPoint = 4,
    Drive_Servo_On = 5,
    Drive_Servo_Off = 6,
} LECPA_Drive_Command_t;

typedef struct
{
    char prepare_abort;
    unsigned long l32_home_task_timeout_cnt;
    unsigned long l32_INPUTS_polling_cnt;
    unsigned long l32_BUSYOff_polling_cnt;
    unsigned long l32_SETON_polling_cnt;
    LECPA_Home_Routine_t home_routine;
    LECPA_Home_State_t home_state;
} LECPA_Home_Status_t;

typedef struct
{
    unsigned long home_task_polling_ms;
    unsigned long Wait_TIMEOUT_HomeTask;    
    unsigned long Wait_INPUTs_HomeTask;    
    unsigned long Wait_BUSYOff_HomeTask;
    unsigned long Wait_SETON_HomeTask;
} LECPA_Home_Parameters_t;

typedef struct
{
    unsigned int u16_home_task_timeout_sec;
    unsigned int u16_home_task_inputs_sec;
    unsigned int u16_home_task_busy_off_sec;
    unsigned int u16_home_task_seton_sec;
} LECPA_HomeTask_Vars_t;

// lecpa-100 total = 15000pulse.
// assume shift = 7500
// min = -7500,
// max =  7499
// any : -7500 <= any <= 7499
typedef struct
{
    unsigned long wait_BUSY_ON_polling_ms;
    unsigned long wait_BUSY_OFF_polling_ms;
    unsigned long wait_task_start_polling_ms;
    unsigned long drive_task_polling_ms;
    signed short shift_pos; // shift position = 7500
    signed short max_point;
    signed short min_point;
    signed short org_point; // org_point should located between max_point and min_point.
} LECPA_Drive_Parameters_t;

typedef struct
{
    char home_successful; // if servo-off, home_successful set -1.
    signed short LECPA_inner_position;
    signed short LECPA_inner_command;
    signed short LECPA_inner_posdiff;
    signed short position_cmd; // position_cmd should between max_point and min_point.
    LECPA_ServoDef_t servo_status;
    LECPA_Drive_State_t drive_state;
    LECPA_Drive_Stage_t drive_stage;
    LECPA_Drive_Command_t drive_command;
} LECPA_Drive_Status_t;

char Set_LECPA_100_Init();

char RTC_LECPA_100_ServoCmd(LECPA_ServoDef_t servo_cmd);
char Is_LECPA_100_HomeRoutine_Idle();
char Get_LECPA_100_HomeRoutine();
char Get_LECPA_100_HomeState();

char Set_LECPA_100_HomeStart();
char Set_LECPA_100_HomeAbort();
char LECPA_100_HomeRountineTask();

char Get_LECPA_100_DriveState();
char Set_LECPA_100_DriveTask(LECPA_Drive_Command_t cmd, signed short val);
char Is_LECPA_100_DriveTaskRunning();
char LECPA_100_DriveRountineTask();
#endif
