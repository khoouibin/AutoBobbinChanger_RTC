#include "RTC_Control.h"
#include "xc.h"
#include "Timers.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include "IO_Entity.h"
#include "usb_app.h"
#include <stdio.h>
#include <stdbool.h>

static RTC_Control_State_t control_state = RTC_CONTROL_STATE_UNDEFINE;

CommonMsg_Actions_t RTC_Control_Hander_CommonMsg(USB_Task_msg_t* task_msg);
void RTC_Control_Handler_Uninit();
void RTC_Control_Handler_Ready();
void RTC_Control_Handler_Home();
void RTC_Control_Handler_Diagnosis();

RTC_Control_State_t RTC_Control_Main(void)
{
    USB_Task_msg_t Task_msg;
    // USB_Task_msg_t* pTask_msg;
    // pTask_msg = &Task_msg;
    // USB_msg_tx_t resp_msg;
    CommonMsg_Actions_t res_CommonMsg;
    bool new_msg;
    control_state = RTC_CONTROL_STATE_UNINIT;
    while (control_state != RTC_CONTROL_RESET)
    {
        new_msg = USB_Msg_Parser(&Task_msg);
        if (new_msg == true)
        {
            res_CommonMsg = RTC_Control_Hander_CommonMsg(&Task_msg);
            if (res_CommonMsg == CONTINUE)
            {
                USB_TxBulkBuffer_To_Bus();
                continue;
            }
        }

         if (control_state == RTC_CONTROL_STATE_UNINIT)
             RTC_Control_Handler_Uninit();
         else if (control_state == RTC_CONTROL_STATE_READY)
             RTC_Control_Handler_Ready();
         else if (control_state == RTC_CONTROL_STATE_HOME)
             RTC_Control_Handler_Home();
         else if (control_state == RTC_CONTROL_STATE_DIAGNOSIS)
             RTC_Control_Handler_Diagnosis();

        if (BL_USB_Tx_1mISR_Get() == 1)
        {
            BL_USB_Tx_1mISR_Clr();
            // to send usb_data every 1ms if bus available.
            USB_TxBulkBuffer_To_Bus();
        }
    }
    return control_state;
}

CommonMsg_Actions_t RTC_Control_Hander_CommonMsg(USB_Task_msg_t* task_msg)
{
    CommonMsg_Actions_t res = PASS;
    USB_TaskResp_msg_t task_resp;
    char resp_msg[48];
    static unsigned int echo_cnt = 0;

    if( task_msg->cmd_id == Cmd_Echo)
    {
        task_resp.cmd_id_rep = RespPos_Echo;
        task_resp.sub_func = task_msg->sub_func;
        snprintf(resp_msg, 48, "echo count:%d",echo_cnt);
        memcpy( task_resp.data,resp_msg, sizeof(resp_msg));
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&task_resp, 32);
        echo_cnt+=1;
        res = CONTINUE;
    }
    else
        res = PASS;
    return res;
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
