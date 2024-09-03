#include "RTC_Control.h"
#include "xc.h"
#include "Timers.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include "IO_Entity.h"

static RTC_Control_State_t control_state = RTC_CONTROL_STATE_UNDEFINE;

void RTC_Control_Handler_Uninit();
void RTC_Control_Handler_Ready();
void RTC_Control_Handler_Home();
void RTC_Control_Handler_Diagnosis();

RTC_Control_State_t RTC_Control_Main(void)
{

    // check usb module IS_Ready,
    // ready => control_state = UNINITIAL
    // IsUSB_ModuleReady();
    control_state = RTC_CONTROL_STATE_UNINIT;

    // check Is usb message comming.
    Nop();

    while (control_state != RTC_CONTROL_RESET)
    {
        if (control_state == RTC_CONTROL_STATE_UNINIT)
            RTC_Control_Handler_Uninit();
        else if (control_state == RTC_CONTROL_STATE_READY)
            RTC_Control_Handler_Ready();
        else if (control_state == RTC_CONTROL_STATE_HOME)
            RTC_Control_Handler_Home();
        else if (control_state == RTC_CONTROL_STATE_DIAGNOSIS)
            RTC_Control_Handler_Diagnosis();
    }
    return control_state;
}

void RTC_Control_Handler_Uninit()
{
    static char led_wink_status = -1;
    int entity_val = -1;

    if (led_wink_status == -1)
    {
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_WINK, Const_RTC_CONTROL_WINK_ms);
        led_wink_status = 0;
    }
    else if (led_wink_status == 0)
    {
        if (SysTimer_IsTimerExpiered(RTC_CONTROL_WINK) == 1)
        {
            SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_WINK, Const_RTC_CONTROL_WINK_ms);
            entity_val = IO_Entity_Mgr_Get_Entity(IO_PUNCHER_PISTON_UP_ENTITY);
            if (entity_val == 0)
                IO_Entity_Mgr_Set_Entity(IO_PUNCHER_PISTON_UP_ENTITY, 1);
            else
                IO_Entity_Mgr_Set_Entity(IO_PUNCHER_PISTON_UP_ENTITY, 0);
        }
    }
}

void RTC_Control_Handler_Ready()
{
    Nop();
}

void RTC_Control_Handler_Home()
{
    Nop();
}

void RTC_Control_Handler_Diagnosis()
{
    Nop();
}
