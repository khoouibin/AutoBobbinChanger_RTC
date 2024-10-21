#include "RTC_LECPA_Control.h"
#include "xc.h"
#include "usb_app.h"
#include "Compiler.h"
#include "IO_Control.h"
#include <stdio.h>
#include <stdbool.h>
#include <outcompare.h>
#include "Timers.h"
#include "RTC_Log.h"

LECPA_HomeTask_Vars_t LECAP_100_HomeTaskVars = {10, 1, 8, 1}; // unit: sec
LECPA_Home_Parameters_t LECAP_100_HomeParas = {50};
LECPA_Home_Status_t LECAP_100_HomeStatus = {-1, 0, 0, 0, 0, Routine_Idle, HomeState_Idle};

LECPA_Drive_Parameters_t LECAP_100_DriveParas = {50, 0, 0, 0, 0};
LECPA_Drive_Status_t LECAP_100_DriveStatus;

LECPA_Home_Parameters_t LECAP_101_Parameters = {100, 25, 0};
LECPA_Home_Status_t LECAP_101_HomeStatus = {-1, 0, Routine_Idle, HomeState_Idle};

char Set_LECPA_100_Init()
{
    LECAP_100_DriveStatus.home_successful = -1;
    LECAP_100_DriveStatus.LECPA_inner_position = -1;
    LECAP_100_DriveStatus.LECPA_inner_command = -1;
    LECAP_100_DriveStatus.LECPA_inner_posdiff = -1;
    LECAP_100_DriveStatus.position_cmd = -1;
    LECAP_100_DriveStatus.servo_status = SERVO_OFF;
    LECAP_100_DriveStatus.drive_state = Drive_NotReady;
    LECAP_100_DriveStatus.drive_command = Drive_Command_Null;
    LECAP_100_DriveStatus.drive_stage = DriStage_Init;

    LECAP_100_DriveParas.drive_task_polling_ms = 50;
    LECAP_100_DriveParas.wait_BUSY_ON_polling_ms = 1;
    LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms = 3;
    LECAP_100_DriveParas.wait_task_start_polling_ms = 1;
    LECAP_100_DriveParas.shift_pos = 7500;
    LECAP_100_DriveParas.max_point = 7499;
    LECAP_100_DriveParas.min_point = -7500;
    LECAP_100_DriveParas.org_point = 100;
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
    LECAP_100_HomeParas.Wait_INPUTs_HomeTask = (unsigned long)LECAP_100_HomeTaskVars.u16_home_task_inputs_sec * 1000 / LECAP_100_HomeParas.home_task_polling_ms;
    LECAP_100_HomeParas.Wait_BUSYOff_HomeTask = (unsigned long)LECAP_100_HomeTaskVars.u16_home_task_busy_off_sec * 1000 / LECAP_100_HomeParas.home_task_polling_ms;
    LECAP_100_HomeParas.Wait_SETON_HomeTask = (unsigned long)LECAP_100_HomeTaskVars.u16_home_task_seton_sec * 1000 / LECAP_100_HomeParas.home_task_polling_ms;

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
    LECPA_Drive_Status_t *pLECAP_100_DStatus = &LECAP_100_DriveStatus;

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
        pLECAP_100_DStatus->LECPA_inner_position = 0;
        pLECAP_100_DStatus->home_successful = 0;
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
        pLECAP_100_DStatus->home_successful = -1;
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

char Is_LECPA_100_SVRE_On()
{
    if (GetIO_ByEntityName(INPUT_SVRE_PIN17) == C_INPUT_ENTITY_ACTIVE)
        return 0;
    else
        return -1;
}

char Is_LECPA_100_BUSY_On()
{
    if (GetIO_ByEntityName(INPUT_BUSY_PIN14) == C_INPUT_ENTITY_ACTIVE)
        return 0;
    else
        return -1;
}

void Convert_LECPA_Drive_PositionCmd(int poscmd, int *ret_dir, int *ret_poscmd, int *ret_inner_cmd)
{
    LECPA_Drive_Status_t *pLECAP_100_DStatus = &LECAP_100_DriveStatus;
    LECPA_Drive_Parameters_t *pLECAP_100_DParas = &LECAP_100_DriveParas;
    int cmd_shift, cmd_tmp;
    cmd_shift = poscmd + pLECAP_100_DParas->shift_pos;
    cmd_tmp = cmd_shift - pLECAP_100_DStatus->LECPA_inner_position;
    if (cmd_tmp > 0)
    {
        *ret_dir = 1;
        *ret_poscmd = cmd_tmp;
    }
    else
    {
        *ret_dir = 0;
        *ret_poscmd = abs(cmd_tmp);
    }
    *ret_inner_cmd = cmd_tmp;
}

void Set_X_OCX_400rpm(OCx_src_t *val)
{
    OCx_src_t ocx;
    ocx.cx[0].period.u32 = 78750;
    ocx.cx[0].dutyon.u32 = 39375;
    ocx.cx[1].period.u32 = 32619;
    ocx.cx[1].dutyon.u32 = 16309;
    ocx.cx_last.period.u32 = 5625;
    ocx.cx_last.dutyon.u32 = 2812;
    *val = ocx;
}
void Set_X_OCX_1000rpm(OCx_src_t *val)
{
    OCx_src_t ocx;
    ocx.cx[0].period.u32 = 31500;
    ocx.cx[0].dutyon.u32 = 15750;
    ocx.cx[1].period.u32 = 13047;
    ocx.cx[1].dutyon.u32 = 6523;
    ocx.cx_last.period.u32 = 2250;
    ocx.cx_last.dutyon.u32 = 1125;
    *val = ocx;
}
void Set_X_OCX_3000rpm(OCx_src_t *val)
{
    OCx_src_t ocx;
    ocx.cx[0].period.u32 = 10500;
    ocx.cx[0].dutyon.u32 = 5250;
    ocx.cx[1].period.u32 = 4349;
    ocx.cx[1].dutyon.u32 = 2714;
    ocx.cx_last.period.u32 = 750;
    ocx.cx_last.dutyon.u32 = 375;
    *val = ocx;
}

char Set_LECPA_100_DriveTask(LECPA_Drive_Command_t cmd, signed short any_point_value)
{
    LECPA_Drive_Status_t *pLECAP_100_DStatus = &LECAP_100_DriveStatus;
    LECPA_Drive_Parameters_t *pLECAP_100_DParas = &LECAP_100_DriveParas;

    char log_msg[60];
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
        if (pLECAP_100_DStatus->home_successful == -1)
            break;
        pLECAP_100_DStatus->position_cmd = pLECAP_100_DParas->org_point;
        pLECAP_100_DStatus->drive_command = Drive_Move_OrgPoint;
        pLECAP_100_DStatus->drive_state = Drive_Moving_OrgPoint;
        pLECAP_100_DStatus->drive_stage = DriStage_Init;
        break;

    case Drive_Move_MinPoint:
        if (pLECAP_100_DStatus->home_successful == -1)
            break;
        pLECAP_100_DStatus->position_cmd = pLECAP_100_DParas->min_point;
        pLECAP_100_DStatus->drive_command = Drive_Move_MinPoint;
        pLECAP_100_DStatus->drive_state = Drive_Moving_MinPoint;
        pLECAP_100_DStatus->drive_stage = DriStage_Init;
        break;

    case Drive_Move_MaxPoint:
        if (pLECAP_100_DStatus->home_successful == -1)
            break;
        pLECAP_100_DStatus->position_cmd = pLECAP_100_DParas->max_point;
        pLECAP_100_DStatus->drive_command = Drive_Move_MaxPoint;
        pLECAP_100_DStatus->drive_state = Drive_Moving_MaxPoint;
        pLECAP_100_DStatus->drive_stage = DriStage_Init;
        break;

    case Drive_Move_AnyPoint:
        if (pLECAP_100_DStatus->home_successful == -1)
            break;
        pLECAP_100_DStatus->position_cmd = any_point_value;
        pLECAP_100_DStatus->drive_command = Drive_Move_AnyPoint;
        pLECAP_100_DStatus->drive_state = Drive_Moving_AnyPoint;
        pLECAP_100_DStatus->drive_stage = DriStage_Init;
        break;
    }

    if (pLECAP_100_DStatus->home_successful == -1)
    {
        snprintf(log_msg, 60, "LECAP_100 needs back to HomePoint to Start");
        RTC_LogMsg(Debug_Lev, log_msg);
    }

    if (pLECAP_100_DStatus->drive_command != Drive_Command_Null)
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_task_start_polling_ms);
    return 0;
}

char Get_LECPA_100_DriveState()
{
    char tmp = 0x00;
    LECPA_Drive_Status_t *pLECAP_100_DStatus = &LECAP_100_DriveStatus;
    tmp = 0x0f & pLECAP_100_DStatus->drive_state;
    tmp = ((pLECAP_100_DStatus->servo_status << 4) & 0xf0) | tmp;
    return tmp;
}

char LECPA_Drive_InnerPositionUpdate(signed short *inner_cmd, signed short *inner_pos)
{
    signed short tmp1, tmp2;
    memcpy(&tmp1, inner_cmd, sizeof(tmp1));
    memcpy(&tmp2, inner_pos, sizeof(tmp2));
    tmp1 += tmp2;
    memcpy(inner_pos, &tmp1, sizeof(tmp1));
    return 0;
}

char LECPA_100_DriveRountineTask()
{
    char log_msg[60];
    usb_msg_lecpa_drive_cmd_reply_t drive_rountine_reply;
    LECPA_Drive_Status_t *pLECAP_100_DStatus = &LECAP_100_DriveStatus;
    int position_comp;
    static int output_dir, output_pulse;
    OCx_src_t LECPA_100_Spd;
    static LECPA_Drive_Command_t dricmd_tmp = Drive_Command_Null;
    static LECPA_Drive_Stage_t dristg_tmp = DriStage_Null;

    if (SysTimer_IsTimerExpiered(RTC_CONTROL_LECPA_100_DRIVE_POLLING) == 0)
        return 0;
    drive_rountine_reply.cmd_id_rep = RespPositive_LECPA_100_Control;
    drive_rountine_reply.argv_1 = Dummy_00;
    switch (pLECAP_100_DStatus->drive_command)
    {
    case Drive_Command_Null:
        drive_rountine_reply.sub_func = Dummy_ff;
        break;

    case Drive_Servo_On:
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
        drive_rountine_reply.sub_func = SubFunc_LECPA_Set_ServoOn_polling_reply;
        RTC_LECPA_100_ServoCmd(SERVO_ON);
        if (Is_LECPA_100_SVRE_On() == 0)
        {
            pLECAP_100_DStatus->servo_status = SERVO_ON;
            pLECAP_100_DStatus->drive_command = Drive_Command_Null;
        }
        break;

    case Drive_Servo_Off:
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING,LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
        drive_rountine_reply.sub_func = SubFunc_LECPA_Set_ServoOff_polling_reply;
        RTC_LECPA_100_ServoCmd(SERVO_OFF);
        if (Is_LECPA_100_SVRE_On() == -1)
        {
            pLECAP_100_DStatus->home_successful = -1;
            pLECAP_100_DStatus->servo_status = SERVO_OFF;
            pLECAP_100_DStatus->drive_command = Drive_Command_Null;
        }
        break;

    case Drive_Move_OrgPoint:
        drive_rountine_reply.sub_func = SubFunc_LECPA_Mov_OrgPoint_polling_reply;
        switch (pLECAP_100_DStatus->drive_stage)
        {
        case DriStage_Null:
            Nop();
            break;

        case DriStage_Init:
            Convert_LECPA_Drive_PositionCmd(pLECAP_100_DStatus->position_cmd, &output_dir, &output_pulse, &position_comp);
            memcpy(&(pLECAP_100_DStatus->LECPA_inner_command), &(position_comp), sizeof(position_comp));
            pLECAP_100_DStatus->drive_stage = (position_comp != 0) ? DriStage_SetDir : DriStage_Complete;
            Set_X_OCX_400rpm(&LECPA_100_Spd);
            snprintf(log_msg, 60, "p_cmd:%d,dir:%d,pulse:%d,comp:%d",
                     pLECAP_100_DStatus->position_cmd, output_dir, output_pulse, position_comp);
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_SetDir:
            WriteValue_ByEntityName(IO_X_DIRECTION_ENTITY, output_dir);
            pLECAP_100_DStatus->drive_stage = DriStage_SetPulse;
            snprintf(log_msg, 60, "Stage:DriStage_SetDir");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_SetPulse:
            x_pulse_update_by_usb_msg(&LECPA_100_Spd, output_pulse);
            pLECAP_100_DStatus->drive_stage = DriStage_waitBUSY_On;
            break;

        case DriStage_waitBUSY_On:
            if (Is_LECPA_100_BUSY_On() ==0)
            {
                pLECAP_100_DStatus->drive_stage = DriStage_waitBUSY_Off;
            }
            snprintf(log_msg, 60, "Stage:DriStage_waitBUSY_On");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_waitBUSY_Off:
            if (Is_LECPA_100_BUSY_On() == -1)
            {
                pLECAP_100_DStatus->drive_stage = DriStage_Complete;
                LECPA_Drive_InnerPositionUpdate(&(pLECAP_100_DStatus->LECPA_inner_command), &(pLECAP_100_DStatus->LECPA_inner_position));
            }
            snprintf(log_msg, 60, "DriStage_waitBUSY_Off");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_Complete:
            snprintf(log_msg, 60, "DriStage_Complete-OrgPoint");
            RTC_LogMsg(Debug_Lev, log_msg);
            pLECAP_100_DStatus->drive_command = Drive_Command_Null;
            pLECAP_100_DStatus->drive_state = Drive_Ready;
            break;
        }

        if (pLECAP_100_DStatus->drive_state != Drive_Ready)
        {
            if (pLECAP_100_DStatus->drive_stage < DriStage_waitBUSY_On)
                SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
            else
            {
                if (pLECAP_100_DStatus->drive_stage == DriStage_waitBUSY_On)
                    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_ON_polling_ms);
                else
                    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
            }
        }
        break;

    case Drive_Move_MinPoint:
        drive_rountine_reply.sub_func = SubFunc_LECPA_Mov_MinPoint_polling_reply;
        switch (pLECAP_100_DStatus->drive_stage)
        {
        case DriStage_Null:
            Nop();
            break;

        case DriStage_Init:
            Convert_LECPA_Drive_PositionCmd(pLECAP_100_DStatus->position_cmd, &output_dir, &output_pulse, &position_comp);
            memcpy(&(pLECAP_100_DStatus->LECPA_inner_command), &(position_comp), sizeof(position_comp));
            pLECAP_100_DStatus->drive_stage = (position_comp != 0) ? DriStage_SetDir : DriStage_Complete;
            Set_X_OCX_1000rpm(&LECPA_100_Spd);
            snprintf(log_msg, 60, "p_cmd:%d,dir:%d,pulse:%d,comp:%d",
                     pLECAP_100_DStatus->position_cmd, output_dir, output_pulse, position_comp);
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_SetDir:
            WriteValue_ByEntityName(IO_X_DIRECTION_ENTITY, output_dir);
            pLECAP_100_DStatus->drive_stage = DriStage_SetPulse;
            snprintf(log_msg, 60, "Stage:DriStage_SetDir");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_SetPulse:
            x_pulse_update_by_usb_msg(&LECPA_100_Spd, output_pulse);
            pLECAP_100_DStatus->drive_stage = DriStage_waitBUSY_On;
            break;

        case DriStage_waitBUSY_On:
            if (Is_LECPA_100_BUSY_On() ==0)
            {
                pLECAP_100_DStatus->drive_stage = DriStage_waitBUSY_Off;
            }
            snprintf(log_msg, 60, "Stage:DriStage_waitBUSY_On");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_waitBUSY_Off:
            if (Is_LECPA_100_BUSY_On() == -1)
            {
                pLECAP_100_DStatus->drive_stage = DriStage_Complete;
                LECPA_Drive_InnerPositionUpdate(&(pLECAP_100_DStatus->LECPA_inner_command), &(pLECAP_100_DStatus->LECPA_inner_position));
            }
            snprintf(log_msg, 60, "DriStage_waitBUSY_Off");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_Complete:
            snprintf(log_msg, 60, "DriStage_Complete-MinPoint");
            RTC_LogMsg(Debug_Lev, log_msg);
            pLECAP_100_DStatus->drive_command = Drive_Command_Null;
            pLECAP_100_DStatus->drive_state = Drive_Ready;
            break;
        }

        if (pLECAP_100_DStatus->drive_state != Drive_Ready)
        {
            if (pLECAP_100_DStatus->drive_stage < DriStage_waitBUSY_On)
                SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
            else
            {
                if (pLECAP_100_DStatus->drive_stage == DriStage_waitBUSY_On)
                    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_ON_polling_ms);
                else
                    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
            }
        }
        break;

    case Drive_Move_MaxPoint:
        drive_rountine_reply.sub_func = SubFunc_LECPA_Mov_MaxPoint_polling_reply;
        switch (pLECAP_100_DStatus->drive_stage)
        {
        case DriStage_Null:
            Nop();
            break;

        case DriStage_Init:
            Convert_LECPA_Drive_PositionCmd(pLECAP_100_DStatus->position_cmd, &output_dir, &output_pulse, &position_comp);
            memcpy(&(pLECAP_100_DStatus->LECPA_inner_command), &(position_comp), sizeof(position_comp));
            pLECAP_100_DStatus->drive_stage = (position_comp != 0) ? DriStage_SetDir : DriStage_Complete;
            Set_X_OCX_3000rpm(&LECPA_100_Spd);
            snprintf(log_msg, 60, "p_cmd:%d,dir:%d,pulse:%d,comp:%d",
                     pLECAP_100_DStatus->position_cmd, output_dir, output_pulse, position_comp);
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_SetDir:
            WriteValue_ByEntityName(IO_X_DIRECTION_ENTITY, output_dir);
            pLECAP_100_DStatus->drive_stage = DriStage_SetPulse;
            snprintf(log_msg, 60, "Stage:DriStage_SetDir");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_SetPulse:
            x_pulse_update_by_usb_msg(&LECPA_100_Spd, output_pulse);
            pLECAP_100_DStatus->drive_stage = DriStage_waitBUSY_On;
            break;

        case DriStage_waitBUSY_On:
            if (Is_LECPA_100_BUSY_On() ==0)
            {
                pLECAP_100_DStatus->drive_stage = DriStage_waitBUSY_Off;
            }
            snprintf(log_msg, 60, "Stage:DriStage_waitBUSY_On");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_waitBUSY_Off:
            if (Is_LECPA_100_BUSY_On() == -1)
            {
                pLECAP_100_DStatus->drive_stage = DriStage_Complete;
                LECPA_Drive_InnerPositionUpdate(&(pLECAP_100_DStatus->LECPA_inner_command), &(pLECAP_100_DStatus->LECPA_inner_position));
            }
            snprintf(log_msg, 60, "DriStage_waitBUSY_Off");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_Complete:
            snprintf(log_msg, 60, "DriStage_Complete-MaxPoint");
            RTC_LogMsg(Debug_Lev, log_msg);
            pLECAP_100_DStatus->drive_command = Drive_Command_Null;
            pLECAP_100_DStatus->drive_state = Drive_Ready;
            break;
        }

        if (pLECAP_100_DStatus->drive_state != Drive_Ready)
        {
            if (pLECAP_100_DStatus->drive_stage < DriStage_waitBUSY_On)
                SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
            else
            {
                if (pLECAP_100_DStatus->drive_stage == DriStage_waitBUSY_On)
                    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_ON_polling_ms);
                else
                    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
            }
        }
        break;

    case Drive_Move_AnyPoint:
        drive_rountine_reply.sub_func = SubFunc_LECPA_Mov_AnyPoint_polling_reply;    
        switch (pLECAP_100_DStatus->drive_stage)
        {
        case DriStage_Null:
            Nop();
            break;

        case DriStage_Init:
            Convert_LECPA_Drive_PositionCmd(pLECAP_100_DStatus->position_cmd, &output_dir, &output_pulse, &position_comp);
            memcpy(&(pLECAP_100_DStatus->LECPA_inner_command), &(position_comp), sizeof(position_comp));
            pLECAP_100_DStatus->drive_stage = (position_comp != 0) ? DriStage_SetDir : DriStage_Complete;
            Set_X_OCX_3000rpm(&LECPA_100_Spd);
            snprintf(log_msg, 60, "p_cmd:%d,dir:%d,pulse:%d,comp:%d",
                     pLECAP_100_DStatus->position_cmd, output_dir, output_pulse, position_comp);
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_SetDir:
            WriteValue_ByEntityName(IO_X_DIRECTION_ENTITY, output_dir);
            pLECAP_100_DStatus->drive_stage = DriStage_SetPulse;
            snprintf(log_msg, 60, "Stage:DriStage_SetDir");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_SetPulse:
            x_pulse_update_by_usb_msg(&LECPA_100_Spd, output_pulse);
            pLECAP_100_DStatus->drive_stage = DriStage_waitBUSY_On;
            break;

        case DriStage_waitBUSY_On:
            if (Is_LECPA_100_BUSY_On() ==0)
            {
                pLECAP_100_DStatus->drive_stage = DriStage_waitBUSY_Off;
            }
            snprintf(log_msg, 60, "Stage:DriStage_waitBUSY_On");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_waitBUSY_Off:
            if (Is_LECPA_100_BUSY_On() == -1)
            {
                pLECAP_100_DStatus->drive_stage = DriStage_Complete;
                LECPA_Drive_InnerPositionUpdate(&(pLECAP_100_DStatus->LECPA_inner_command), &(pLECAP_100_DStatus->LECPA_inner_position));
            }
            snprintf(log_msg, 60, "DriStage_waitBUSY_Off");
            RTC_LogMsg(Debug_Lev, log_msg);
            break;

        case DriStage_Complete:
            snprintf(log_msg, 60, "DriStage_Complete-AnyPoint");
            RTC_LogMsg(Debug_Lev, log_msg);
            pLECAP_100_DStatus->drive_command = Drive_Command_Null;
            pLECAP_100_DStatus->drive_state = Drive_Ready;
            break;
        }

        if (pLECAP_100_DStatus->drive_state != Drive_Ready)
        {
            if (pLECAP_100_DStatus->drive_stage < DriStage_waitBUSY_On)
                SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING,LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
            else
            {
                if (pLECAP_100_DStatus->drive_stage == DriStage_waitBUSY_On)
                    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_ON_polling_ms);
                else
                    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_100_DRIVE_POLLING, LECAP_100_DriveParas.wait_BUSY_OFF_polling_ms);
            }
        }

        break;
    }

    //dricmd_tmp = Drive_Command_Null;
    //static LECPA_Drive_Stage_t dristg_tmp = DriStage_Null;
    if (dricmd_tmp ^ pLECAP_100_DStatus->drive_command == 1)
    {
        drive_rountine_reply.drive_state = Get_LECPA_100_DriveState();
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&drive_rountine_reply, sizeof(drive_rountine_reply));
    }

    return 0;
}
