#include "RTC_Control.h"
#include "xc.h"
#include "Timers.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include "IO_Entity.h"
#include "usb_app.h"
#include "RTC_Profile.h"
#include "RTC_Log.h"
#include <stdio.h>
#include <stdbool.h>

const int AT_200h RTC_version[4] = {0, 3, 2};
const int AT_208h MsgProt_ver[4] = {0, 3, 2};
const int AT_210h BL_version[4] = {0, 1, 2};
const int AT_218h null00[4];
const char AT_220h rtc_version_message[48] = {"auto bobbin changer real-time controller"};
static RTC_Control_State_t control_state = RTC_CONTROL_STATE_UNDEFINE;
static char reset_en = -1;

CommonMsg_Actions_t RTC_Control_Hander_CommonMsg(USB_Task_msg_t *task_msg);
void RTC_Control_Handler_Uninit();
void RTC_Control_Handler_Ready();
void RTC_Control_Handler_Home();
void RTC_Control_Handler_Diagnosis();

RTC_Control_State_t RTC_Control_Main(void)
{
    USB_Task_msg_t Task_msg;
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

        if (reset_en == 1)
        {
            if (SysTimer_IsTimerExpiered(RTC_CONTROL_RESET_DELAY) == 1)
            {
                reset_en = -1;
                control_state = RTC_CONTROL_RESET;
            }
        }
    }
    return control_state;
}

CommonMsg_Actions_t RTC_Control_Hander_CommonMsg(USB_Task_msg_t *task_msg)
{
    CommonMsg_Actions_t res = PASS;
    USB_TaskResp_msg_t task_resp;
    usb_msg_echo_t *p_echo_task;
    usb_msg_reset_t *p_reset_task;
    usb_msg_profile_t *p_profile_task;
    usb_msg_profile_t profile_msg;
    usb_msg_log_t *p_log_task;
    usb_msg_log_reply_t log_task_resp;
    usb_msg_entitytable_t *p_entity_tab_task;
    //usb_msg_entity_pack_t *p_entity_pack_task;
    usb_msg_entitytable_reply_t entity_tab_reply;

    unsigned long reset_delay_cnt;
    char resp_msg[48];
    static unsigned int echo_cnt = 0;
    switch (task_msg->cmd_id)
    {
    case Cmd_Echo:
        p_echo_task = (usb_msg_echo_t *)task_msg;
        task_resp.cmd_id_rep = RespPositive_Echo;
        task_resp.sub_func = task_msg->sub_func;
        snprintf(resp_msg, 48, "sub_func:0x%02x, echo count:%d", p_echo_task->sub_func, echo_cnt);
        memcpy(task_resp.data, resp_msg, sizeof(resp_msg));
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&task_resp, 32);
        echo_cnt += 1;
        res = CONTINUE;
        break;

    case Cmd_Reset:
        p_reset_task = (usb_msg_reset_t *)task_msg;
        task_resp.cmd_id_rep = RespPositive_Reset;
        task_resp.sub_func = task_msg->sub_func;
        task_resp.data[0] = 0;
        task_resp.data[1] = 0;
        reset_delay_cnt = (p_reset_task->delay_time < C_MIN_RTC_CONTROL_RESET_DELAY_ms) ? C_MIN_RTC_CONTROL_RESET_DELAY_ms : p_reset_task->delay_time;
        reset_en = 1;
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_RESET_DELAY, reset_delay_cnt);
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&task_resp, sizeof(usb_msg_reset_t));
        res = CONTINUE;
        break;

    case Cmd_Profile:
        p_profile_task = (usb_msg_profile_t *)task_msg;
        if (p_profile_task->sub_func == SubFunc_profile_set)
        {
            RTC_Profile_Update(p_profile_task);
            task_resp.cmd_id_rep = RespPositive_Profile;
            task_resp.sub_func = task_msg->sub_func;
            task_resp.data[0] = p_profile_task->profile_number;
            task_resp.data[1] = 0;
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&task_resp, 4);
        }
        else if (p_profile_task->sub_func == SubFunc_profile_get)
        {
            RTC_Get_Profile(p_profile_task->profile_number,&profile_msg);
            profile_msg.cmd_id = RespPositive_Profile;
            profile_msg.sub_func = p_profile_task->sub_func;
            profile_msg.profile_number = p_profile_task->profile_number;
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&profile_msg, sizeof(usb_msg_profile_t));
        }
        res = CONTINUE;
        break;

    case Cmd_Log:
        p_log_task = (usb_msg_log_t *)task_msg;
        if (p_log_task->sub_func == SubFunc_log_level_set)
        {
            RTC_LogLevel_Set((enum LogLev)p_log_task->set_level);
            log_task_resp.cmd_id_rep = RespPositive_Log;
            log_task_resp.sub_func = p_log_task->sub_func;
            log_task_resp.log_counter = 0xffff;
            memset(log_task_resp.data, 0xff, sizeof(log_task_resp.data));
            log_task_resp.data[0] = (unsigned char)RTC_LogLevel_Get();
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&log_task_resp, 8);
        }
        else if (p_log_task->sub_func == SubFunc_log_level_get)
        {
            log_task_resp.cmd_id_rep = RespPositive_Log;
            log_task_resp.sub_func = p_log_task->sub_func;
            log_task_resp.log_counter = 0xffff;
            memset(log_task_resp.data, 0xff, sizeof(log_task_resp.data));
            log_task_resp.data[0] = (unsigned char)RTC_LogLevel_Get();
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&log_task_resp, 8);
        }
        res = CONTINUE;
        break;

    case Cmd_EntityTable:
        p_entity_tab_task = (usb_msg_entitytable_t *)task_msg;
        if (p_entity_tab_task->sub_func == SubFunc_table_get_instant)
        {
            entity_tab_reply.cmd_id_rep = RespPositive_EntityTable;
            entity_tab_reply.sub_func = p_entity_tab_task->sub_func;
            entity_tab_reply.reply_period = Dummy_00;
            memset(entity_tab_reply.data, 0, sizeof(entity_tab_reply.data));
            LATHbits.LATH15 = 1;
            Get_EntityTable(entity_tab_reply.data, &entity_tab_reply.table_size);
            LATHbits.LATH15 = 0;
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&entity_tab_reply, sizeof(usb_msg_entitytable_t));
        }
        res = CONTINUE;
        break;

    default:
        res = PASS;
    }
    return res;
}

void RTC_Control_Handler_Uninit()
{
    static char led_wink_status = -1;
    int entity_val = -1;
    
    if (led_wink_status == -1)
    {
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_WINK, C_RTC_CONTROL_WINK_ms);
        led_wink_status = 0;
    }
    else if (led_wink_status == 0 && reset_en == -1)
    {
        if (SysTimer_IsTimerExpiered(RTC_CONTROL_WINK) == 1)
        { 
            SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_WINK, C_RTC_CONTROL_WINK_ms);
            // entity_val = IO_Entity_Mgr_Get_Entity(IO_PUNCHER_PISTON_UP_ENTITY);
            // if (entity_val == 0)
            //     IO_Entity_Mgr_Set_Entity(IO_PUNCHER_PISTON_UP_ENTITY, 1);
            // else
            //     IO_Entity_Mgr_Set_Entity(IO_PUNCHER_PISTON_UP_ENTITY, 0);

            //snprintf(log_msg, 60, "IO_PUNCHER_PISTON_UP_ENTITY:%d",(!entity_val));
            
            //snprintf(log_msg, 60, "USBGetDeviceState:%d",USBGetDeviceState());
            //RTC_LogMsg(Debug_Lev, log_msg);
        }
    }
    else if (led_wink_status == 0 && reset_en == 1)
    {
        if (SysTimer_IsTimerExpiered(RTC_CONTROL_WINK) == 1)
        {
            SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_WINK, C_RTC_CONTROL_WINK_ms/4);
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
