#include "RTC_LECPA_Control.h"
#include "xc.h"
#include "usb_app.h"
#include "Compiler.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include <stdio.h>
#include <stdbool.h>
#include <outcompare.h>
#include "Timers.h"
#include "RTC_Log.h"

LECPA_HomeTask_Vars_t LECAP_100_HomeTaskVars={10,1,8,1};//unit: sec
LECPA_Home_Parameters_t LECAP_100_HomeParas={50};
LECPA_Home_Status_t LECAP_100_HomeStatus = {-1,0,0,0,0,Routine_Idle,HomeState_Idle};

LECPA_Drive_Parameters_t LECAP_100_DriveParas= {-1,-1,-1,50};
LECPA_Drive_Status_t LECAP_100_DriveStatus = {-1,0,0,SERVO_OFF,Drive_NotReady,Drive_Command_Null};

LECPA_Home_Parameters_t LECAP_101_Parameters={100,25,0};
LECPA_Home_Status_t LECAP_101_HomeStatus= {-1,0,Routine_Idle,HomeState_Idle};

char Set_LECPA_100_Init()
{
    LECAP_100_DriveStatus.home_successful = -1;
    LECAP_100_DriveStatus.current_position = 0;
    LECAP_100_DriveStatus.position_command = 0;
    LECAP_100_DriveStatus.servo_status = SERVO_OFF;
    LECAP_100_DriveStatus.drive_state = Drive_NotReady;
    LECAP_100_DriveStatus.drive_command = Drive_Command_Null;
    LECAP_100_DriveParas.drive_task_polling_ms = 50;
    return 0;
}

char Is_LECPA_100_HomeRoutine_Idle()
{
    char tmp;
    tmp = (LECAP_100_HomeStatus.home_routine == Routine_Idle) ? 0 : -1;
    return tmp;
}

char Get_LECPA_100_HomeRoutine()
{
    return (char)LECAP_100_HomeStatus.home_routine;
}
char Get_LECPA_100_HomeState()
{
    return (char)LECAP_100_HomeStatus.home_state;
}

char Set_LECPA_100_HomeStart()
{
    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_HOME_POLLING, LECAP_100_HomeParas.home_task_polling_ms);
    LECAP_100_HomeStatus.home_routine = Routine_TaskStart;
    LECAP_100_HomeStatus.home_state = HomeState_Start;
    LECAP_100_HomeStatus.prepare_abort = 0;
    LECAP_100_HomeStatus.l32_home_task_timeout_cnt = 0;    
    LECAP_100_HomeStatus.l32_INPUTS_polling_cnt = 0;
    LECAP_100_HomeStatus.l32_BUSYOff_polling_cnt = 0;
    LECAP_100_HomeStatus.l32_SETON_polling_cnt = 0;
    
    LECAP_100_HomeParas.Wait_TIMEOUT_HomeTask = (unsigned long)LECAP_100_HomeTaskVars.u16_home_task_timeout_sec * 1000 / LECAP_100_HomeParas.home_task_polling_ms;
    LECAP_100_HomeParas.Wait_INPUTs_HomeTask = (unsigned long)LECAP_100_HomeTaskVars.u16_home_task_inputs_sec * 1000/LECAP_100_HomeParas.home_task_polling_ms;
    LECAP_100_HomeParas.Wait_BUSYOff_HomeTask = (unsigned long)LECAP_100_HomeTaskVars.u16_home_task_busy_off_sec* 1000/LECAP_100_HomeParas.home_task_polling_ms;
    LECAP_100_HomeParas.Wait_SETON_HomeTask =(unsigned long)LECAP_100_HomeTaskVars.u16_home_task_seton_sec* 1000/LECAP_100_HomeParas.home_task_polling_ms;

    IO_Entity_Mgr_Set_Entity(OUTPUT_RESET_CLRALARM_PIN8, 0);
    IO_Entity_Mgr_Set_Entity(OUTPUT_SVON_SERVO_ON_PIN9, 0);
    IO_Entity_Mgr_Set_Entity(OUTPUT_SETUP_HOME_PIN7, 0);
    return 0;
}

char Set_LECPA_100_HomeReset()
{
    LECAP_100_HomeStatus.home_routine = Routine_Idle;
    LECAP_100_HomeStatus.home_state = HomeState_Idle;
    return 0;
}

char Set_LECPA_100_HomeAbort()
{
    if (LECAP_100_HomeStatus.home_routine == 0)
        return -1;
    LECAP_100_HomeStatus.prepare_abort = 1;
    return 0;
}

char LECPA_100_HomeRountineTask()
{
    char log_msg[60];
    char s8_entity_value;
    usb_msg_home_parts_reply_t home_rountine_reply;
    LECPA_Home_Status_t *pLECPA100_Hstatus = &LECAP_100_HomeStatus;
    LECPA_Drive_Status_t *pLECAP_100_DStatus =&LECAP_100_DriveStatus;
    
    home_rountine_reply.cmd_id_rep = RespPositive_HomeParts;
    home_rountine_reply.sub_func = SubFunc_home_LECPA_100_polling_reply;
    if (SysTimer_IsTimerExpiered(RTC_CONTROL_LECPA_100_HOME_POLLING) == 0)
        return 0;
    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_HOME_POLLING, LECAP_100_HomeParas.home_task_polling_ms);

    if (pLECPA100_Hstatus->home_state < 0)
    {
        pLECPA100_Hstatus->home_state = HomeState_Idle;
        pLECPA100_Hstatus->home_routine = Routine_Idle;
        IO_Entity_Mgr_Set_Entity(OUTPUT_RESET_CLRALARM_PIN8, 0);
        IO_Entity_Mgr_Set_Entity(OUTPUT_SVON_SERVO_ON_PIN9, 0);
        return 0;
    }
    else if (pLECPA100_Hstatus->home_state == HomeState_SETON)
    {
        pLECPA100_Hstatus->home_routine = Routine_Idle;
        pLECAP_100_DStatus->drive_state = Drive_Ready;
        return 0;
    }

    // time-out check
    pLECPA100_Hstatus->l32_home_task_timeout_cnt += 1;
    if (pLECPA100_Hstatus->l32_home_task_timeout_cnt > LECAP_100_HomeParas.Wait_TIMEOUT_HomeTask)
    {
        pLECPA100_Hstatus->home_routine = Routine_TaskEnd;
        pLECPA100_Hstatus->home_state = HomeState_Timeout;
    }

    // abort check
    if (pLECPA100_Hstatus->prepare_abort == 1)
    {
        // output pin release
        pLECPA100_Hstatus->home_routine = Routine_TaskEnd;
        pLECPA100_Hstatus->home_state = HomeState_Abort;
        IO_Entity_Mgr_Set_Entity(OUTPUT_RESET_CLRALARM_PIN8, 0);
        IO_Entity_Mgr_Set_Entity(OUTPUT_SVON_SERVO_ON_PIN9, SERVO_OFF_EntityValue);
    }

    switch (pLECPA100_Hstatus->home_routine)
    {
    case Routine_Idle:
        Nop();
        pLECPA100_Hstatus->home_routine += 1;
        break;

    case Routine_TaskStart:
        Nop();
        IO_Entity_Mgr_Set_Entity(OUTPUT_RESET_CLRALARM_PIN8, 1);
        pLECPA100_Hstatus->home_routine += 1;
        pLECAP_100_DStatus->drive_state = Drive_NotReady;
        break;

    case Routine_RESET_Cmdoff:
        Nop();
        IO_Entity_Mgr_Set_Entity(OUTPUT_RESET_CLRALARM_PIN8, 0);
        pLECPA100_Hstatus->home_routine += 1;
        break;

    case Routine_ALARM_ReadyCheck:
        pLECPA100_Hstatus->l32_INPUTS_polling_cnt += 1;
        if (pLECPA100_Hstatus->l32_INPUTS_polling_cnt < LECAP_100_HomeParas.Wait_INPUTs_HomeTask)
        {
            s8_entity_value = GetIO_ByEntityName(INPUT_ALARM_PIN19);
            if (s8_entity_value == 0)
            {
                pLECPA100_Hstatus->l32_INPUTS_polling_cnt = 0;
                pLECPA100_Hstatus->home_routine += 1;
            }
        }
        else
        {
            pLECPA100_Hstatus->home_routine = Routine_TaskEnd;
            pLECPA100_Hstatus->home_state = HomeState_AlarmPIN19NotReady;
        }
        break;
    case Routine_SVON_CmdOn:
        IO_Entity_Mgr_Set_Entity(OUTPUT_SVON_SERVO_ON_PIN9, SERVO_ON_EntityValue);
        pLECPA100_Hstatus->home_routine += 1;
        break;

    case Routine_SVRE_ReadyCheck:
        Nop();
        pLECPA100_Hstatus->l32_INPUTS_polling_cnt += 1;
        if (pLECPA100_Hstatus->l32_INPUTS_polling_cnt < LECAP_100_HomeParas.Wait_INPUTs_HomeTask)
        {
            s8_entity_value = GetIO_ByEntityName(INPUT_SVRE_PIN17);
            if (s8_entity_value == 0)
            {
                pLECPA100_Hstatus->l32_INPUTS_polling_cnt = 0;
                pLECPA100_Hstatus->home_routine += 1;

                pLECAP_100_DStatus->servo_status = SERVO_ON;
            }
        }
        else
        {
            pLECPA100_Hstatus->home_routine = Routine_TaskEnd;
            pLECPA100_Hstatus->home_state = HomeState_SvrePIN17NotReady;
        }
        break;

    case Routine_SETUP_CmdToggle:
        pLECPA100_Hstatus->l32_INPUTS_polling_cnt += 1;
        if (pLECPA100_Hstatus->l32_INPUTS_polling_cnt < LECAP_100_HomeParas.Wait_INPUTs_HomeTask)
        {
            IO_Entity_Mgr_Set_Entity(OUTPUT_SETUP_HOME_PIN7, 1);

            s8_entity_value = GetIO_ByEntityName(INPUT_BUSY_PIN14);
            if (s8_entity_value == 0)
            {
                IO_Entity_Mgr_Set_Entity(OUTPUT_SETUP_HOME_PIN7, 0);
                pLECPA100_Hstatus->l32_INPUTS_polling_cnt = 0;
                pLECPA100_Hstatus->home_routine += 1;
            }
        }
        else
        {
            pLECPA100_Hstatus->home_routine = Routine_TaskEnd;
            pLECPA100_Hstatus->home_state = HomeState_BusyPIN14NotTrigger;
        }

        break;

    case Routine_BUSY_Polling:
        Nop();
        pLECPA100_Hstatus->l32_BUSYOff_polling_cnt += 1;
        if (pLECPA100_Hstatus->l32_BUSYOff_polling_cnt < LECAP_100_HomeParas.Wait_BUSYOff_HomeTask)
        {
            s8_entity_value = GetIO_ByEntityName(INPUT_BUSY_PIN14);
            if (s8_entity_value == 1)
            {
                pLECPA100_Hstatus->home_routine += 1;
            }
            pLECPA100_Hstatus->home_state = HomeState_Wait;
        }
        else
        {
            pLECPA100_Hstatus->home_routine = Routine_TaskEnd;
            pLECPA100_Hstatus->home_state = HomeState_BusyPIN14NotOff;
        }

        break;
    case Routine_SETON_Polling:
        pLECPA100_Hstatus->l32_SETON_polling_cnt += 1;
        if (pLECPA100_Hstatus->l32_SETON_polling_cnt < LECAP_100_HomeParas.Wait_SETON_HomeTask)
        {
            s8_entity_value = GetIO_ByEntityName(INPUT_SETON_PIN15);
            snprintf(log_msg, 60, "Routine_SETON_Polling: INPUT_SETON_PIN15 (%d) = :%d", INPUT_SETON_PIN15, s8_entity_value);
            RTC_LogMsg(Debug_Lev, log_msg);

            if (s8_entity_value == 0)
            {
                pLECPA100_Hstatus->home_state = HomeState_SETON;
                pLECPA100_Hstatus->home_routine += 1;
            }
        }
        else
        {
            pLECPA100_Hstatus->home_routine = Routine_TaskEnd;
            pLECPA100_Hstatus->home_state = HomeState_Fail;
        }
        break;
    case Routine_TaskEnd:
        Nop();
        break;
    }

    home_rountine_reply.home_routine = pLECPA100_Hstatus->home_routine;
    home_rountine_reply.home_state = pLECPA100_Hstatus->home_state;
    USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&home_rountine_reply, sizeof(home_rountine_reply));
    return 0;
}

char RTC_LECPA_100_ServoCmd(LECPA_ServoDef_t servo_cmd)
{
    char cmd = 0;
    char res = 0;
    if ((LECAP_100_DriveStatus.drive_state == Drive_NotReady) || (LECAP_100_DriveStatus.drive_state == Drive_Ready))
    {
        cmd = (servo_cmd == SERVO_ON) ? SERVO_ON_EntityValue : SERVO_OFF_EntityValue;
        WriteValue_ByEntityName(OUTPUT_SVON_SERVO_ON_PIN9, cmd);
    }
    else
    {

        res = -1;
    }

    return res;
}

char Is_LECPA_100_DriveTaskRunning()
{
    if (LECAP_100_DriveStatus.drive_command == Drive_Command_Null)
        return -1;
    else
        return 0;
}

char Set_LECPA_100_DriveTask(LECPA_Drive_Command_t cmd)
{
    LECPA_Drive_Status_t *pLECAP_100_DStatus = &LECAP_100_DriveStatus;

    if (pLECAP_100_DStatus->drive_state != Drive_Ready)
        return -1;
    if (pLECAP_100_DStatus->drive_command != Drive_Command_Null)
        return -2;

    switch (cmd)
    {
        case Drive_Command_Null:
            Nop();
            break;
    case Drive_Servo_On:
        
        pLECAP_100_DStatus->drive_command = Drive_Servo_On;
        break;

    case Drive_Servo_Off:    
        pLECAP_100_DStatus->drive_command = Drive_Servo_Off;
        break;
        
    case Drive_Move_OrgPoint:
        pLECAP_100_DStatus->drive_command = Drive_Move_OrgPoint;
        pLECAP_100_DStatus->drive_state = Drive_Moving_OrgPoint;
        break;

    case Drive_Move_MinPoint:
        pLECAP_100_DStatus->drive_command = Drive_Move_MinPoint;
        pLECAP_100_DStatus->drive_state = Drive_Moving_MinPoint;
        break;

    case Drive_Move_MaxPoint:
        pLECAP_100_DStatus->drive_command = Drive_Move_MaxPoint;
        pLECAP_100_DStatus->drive_state = Drive_Moving_MaxPoint;
        break;

    case Drive_Move_AnyPoint:
        pLECAP_100_DStatus->drive_command = Drive_Move_AnyPoint;
        pLECAP_100_DStatus->drive_state = Drive_Moving_AnyPoint;
        break;
    }

    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.drive_task_polling_ms);
    return 0;
}

char LECPA_100_DriveRountineTask()
{
    char log_msg[60];
    char s8_entity_value;
    usb_msg_lecpa_drive_cmd_reply_t drive_rountine_reply;
    LECPA_Drive_Status_t *pLECAP_100_DStatus =&LECAP_100_DriveStatus;

    if (SysTimer_IsTimerExpiered(RTC_CONTROL_LECPA_100_HOME_POLLING) == 0)
        return 0;
    
    snprintf(log_msg, 60, "LECPA_100_DriveTask");
    RTC_LogMsg(Debug_Lev, log_msg);
            
    switch (pLECAP_100_DStatus->drive_command)
    {
    case Drive_Command_Null:
        drive_rountine_reply.sub_func = Dummy_ff;
        break;

    case Drive_Servo_On:
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_HOME_POLLING, LECAP_100_HomeParas.home_task_polling_ms);
        drive_rountine_reply.sub_func = SubFunc_ServoOn_polling_reply;
        break;
        
    case Drive_Servo_Off:
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_HOME_POLLING, LECAP_100_HomeParas.home_task_polling_ms);
        drive_rountine_reply.sub_func = SubFunc_ServoOff_polling_reply;
        break;        
            
            
    case Drive_Move_OrgPoint:
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_HOME_POLLING, LECAP_100_HomeParas.home_task_polling_ms);
        drive_rountine_reply.sub_func = SubFunc_Move_OrgPoint_polling_reply;

        break;

    case Drive_Move_MinPoint:
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_HOME_POLLING, LECAP_100_HomeParas.home_task_polling_ms);
        drive_rountine_reply.sub_func = SubFunc_Move_MinPoint_polling_reply;

        break;

    case Drive_Move_MaxPoint:
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_HOME_POLLING, LECAP_100_HomeParas.home_task_polling_ms);
        drive_rountine_reply.sub_func = SubFunc_Move_MaxPoint_polling_reply;

        break;

    case Drive_Move_AnyPoint:
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_HOME_POLLING, LECAP_100_HomeParas.home_task_polling_ms);
        drive_rountine_reply.sub_func = SubFunc_Move_AnyPoint_polling_reply;

        break;
    }
   
    USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&drive_rountine_reply, sizeof(drive_rountine_reply));
    return 0;
}
