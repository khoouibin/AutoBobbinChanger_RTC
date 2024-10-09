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

LECPA_Home_Parameters_t LECAP_30_Parameters={200,10,0};
LECPA_Home_Parameters_t LECAP_100_Parameters={100,25,0};
LECPA_Home_Status_t LECAP_30_HomeStatus = {-1,0,Routine_Idle,HomeState_Idle};
LECPA_Home_Status_t LECAP_100_HomeStatus= {-1,0,Routine_Idle,HomeState_Idle};

void RTC_LECPA_30_ServoCmd(IO_LECPA_ServoDef_t servo_cmd)
{
    char cmd = 0;
    cmd = (servo_cmd == SERVO_ON)?1:0;
    WriteValue_ByEntityName(OUTPUT_SVON_SERVO_ON_PIN9, cmd);
}

char Is_LECPA_30_HomeRoutine_Idle()
{
    char tmp;
    tmp = (LECAP_30_HomeStatus.home_routine == Routine_Idle) ? 0 : -1;
    return tmp;
}

char Get_LECPA_30_HomeRoutine()
{
    return (char)LECAP_30_HomeStatus.home_routine;
}
char Get_LECPA_30_HomeState()
{
    return (char)LECAP_30_HomeStatus.home_state;
}

char Set_LECPA_30_HomeStart()
{
    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_30_HOME_POLLING, LECAP_30_Parameters.polling_period_ms);
    LECAP_30_HomeStatus.home_routine = Routine_TaskStart;
    LECAP_30_HomeStatus.home_state = HomeState_Start;
    LECAP_30_HomeStatus.prepare_abort = 0;
    LECAP_30_HomeStatus.timeout_cnt = 0;    

    LECAP_30_Parameters.timeout_base_polling_period = LECAP_30_Parameters.timeout_sec * 1000 / LECAP_30_Parameters.polling_period_ms;
    IO_Entity_Mgr_Set_Entity(OUTPUT_RESET_CLRALARM_PIN8, 0);
    IO_Entity_Mgr_Set_Entity(OUTPUT_SVON_SERVO_ON_PIN9, 0);
    return 0;
}

char Set_LECPA_30_HomeReset()
{
    LECAP_30_HomeStatus.home_routine = Routine_Idle;
    LECAP_30_HomeStatus.home_state = HomeState_Idle;
    return 0;
}

char Set_LECPA_30_HomeAbort()
{
    if (LECAP_30_HomeStatus.home_routine == 0)
        return -1;
    LECAP_30_HomeStatus.prepare_abort = 1;
    return 0;
}

char LECPA_30_HomeRountineTask()
{
    usb_msg_home_parts_reply_t home_rountine_reply;
    LECPA_Home_Status_t *pLECPA30 = &LECAP_30_HomeStatus;
    home_rountine_reply.cmd_id_rep = RespPositive_HomeParts;
    home_rountine_reply.sub_func = SubFunc_home_LECPA_30_polling_reply;

    if (SysTimer_IsTimerExpiered(RTC_CONTROL_LECPA_30_HOME_POLLING) == 0)
        return 0;
    SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_LECPA_30_HOME_POLLING, LECAP_30_Parameters.polling_period_ms);

    IO_Entity_Mgr_Set_Entity(35, !IO_Entity_Mgr_Get_Entity(35));

    // negative state check, back to init, so that turn off home timer.
    if (pLECPA30->home_state < 0)
    {
        pLECPA30->home_state = HomeState_Idle;
        pLECPA30->home_routine = Routine_Idle;
        IO_Entity_Mgr_Set_Entity(35, 0);
        IO_Entity_Mgr_Set_Entity(OUTPUT_RESET_CLRALARM_PIN8, 0);
        IO_Entity_Mgr_Set_Entity(OUTPUT_SVON_SERVO_ON_PIN9, 0);
        return 0;
    }

    // time-out check
    pLECPA30->timeout_cnt += 1;
    if (pLECPA30->timeout_cnt > LECAP_30_Parameters.timeout_base_polling_period)
    {
        pLECPA30->home_routine = Routine_TaskEnd;
        pLECPA30->home_state = HomeState_Timeout;
    }

    // abort check
    if (pLECPA30->prepare_abort == 1)
    {
        // output pin release
        pLECPA30->home_routine = Routine_TaskEnd;
        pLECPA30->home_state = HomeState_Abort;
        IO_Entity_Mgr_Set_Entity(OUTPUT_RESET_CLRALARM_PIN8, 0);
        IO_Entity_Mgr_Set_Entity(OUTPUT_SVON_SERVO_ON_PIN9, 0);
    }


    switch (pLECPA30->home_routine)
    {
    case Routine_Idle:
        Nop();
        pLECPA30->home_routine+=1;
        break;

    case Routine_TaskStart:
        Nop();
        pLECPA30->home_routine+=1;
        break;

    case Routine_ALARM_ReadyCheck:
        Nop();
        pLECPA30->home_routine+=1;
        break;
    case Routine_SVON_CmdOn:
        IO_Entity_Mgr_Set_Entity(OUTPUT_SVON_SERVO_ON_PIN9, 1);
        pLECPA30->home_routine+=1;
        break;

    case Routine_SVRE_ReadyCheck:
        Nop();
        pLECPA30->home_routine+=1;
        break;

    case Routine_SETUP_CmdOn:
        Nop();
        pLECPA30->home_routine+=1;
        break;

    case Routine_SETUP_CmdOff:
        Nop();
        pLECPA30->home_routine+=1;
        break;

    case Routine_BUSY_Polling:
        Nop();
        pLECPA30->home_routine+=1;
        break;
    case Routine_BUSY_Finished:
        Nop();
        pLECPA30->home_routine+=1;
        break;
    case Routine_TaskEnd:
        Nop();
        break;
    }

    home_rountine_reply.home_routine = pLECPA30->home_routine;
    home_rountine_reply.home_state = pLECPA30->home_state;
    USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&home_rountine_reply, sizeof(home_rountine_reply));
    return 0;
}