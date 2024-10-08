#include "RTC_Control.h"
#include "xc.h"
#include "Timers.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include "IO_Entity.h"
#include "usb_app.h"
#include "RTC_Profile.h"
#include "RTC_Log.h"
#include "RTC_LECPA_Control.h"
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
void RTC_Control_Handler_Uninit(CommonMsg_Actions_t cmd, USB_Task_msg_t *task_msg);
void RTC_Control_Handler_Ready(CommonMsg_Actions_t cmd, USB_Task_msg_t *task_msg);
void RTC_Control_Handler_Home(CommonMsg_Actions_t cmd, USB_Task_msg_t *task_msg);
void RTC_Control_Handler_Diagnosis(CommonMsg_Actions_t cmd, USB_Task_msg_t *task_msg);
void RTC_Control_Wink_Entity_Debug(char *flag, unsigned long delay_time, int entity_name);

RTC_Control_State_t RTC_Control_Main(void)
{
    USB_Task_msg_t Task_msg;
    CommonMsg_Actions_t cmd_action;
    usb_msg_entitytable_reply_t entity_tab_reply;
    Refresh_EntityTableMask();
    // char log_msg[60];
    bool new_msg;
    control_state = RTC_CONTROL_STATE_UNINIT;
    while (control_state != RTC_CONTROL_RESET)
    {
        new_msg = USB_Msg_Parser(&Task_msg);
        if (new_msg == true)
        {
            cmd_action = RTC_Control_Hander_CommonMsg(&Task_msg);
            if (cmd_action == Action_ReplyCommonMsg)
            {
                USB_TxBulkBuffer_To_Bus();
                continue;
            }
        }
        else
        {
            cmd_action = Action_DoNothing;
        }

        if (control_state == RTC_CONTROL_STATE_UNINIT)
            RTC_Control_Handler_Uninit(cmd_action, &Task_msg);
        else if (control_state == RTC_CONTROL_STATE_READY)
            RTC_Control_Handler_Ready(cmd_action, &Task_msg);
        else if (control_state == RTC_CONTROL_STATE_HOME)
            RTC_Control_Handler_Home(cmd_action, &Task_msg);
        else if (control_state == RTC_CONTROL_STATE_DIAGNOSIS)
            RTC_Control_Handler_Diagnosis(cmd_action, &Task_msg);

        // 1ms isr.
        if (BL_USB_Tx_1mISR_Get() == 1)
        {
            BL_USB_Tx_1mISR_Clr();
            z_pulse_startup_by_tmr();
            x_pulse_startup_by_tmr();

            // to send usb_data every 1ms if bus available.
            USB_TxBulkBuffer_To_Bus();
        }

        if (reset_en == 1)
        {
            if (SysTimer_IsTimerExpiered(RTC_CONTROL_RESET_DELAY) == 1)
            {
                reset_en = -1;
                control_state = RTC_CONTROL_RESET;
                IO_Entity_Mgr_Set_Entity(IO_PUNCHER_PISTON_UP_ENTITY, 0);
            }
        }

        if (Get_GetEntityTableMode() == SubFunc_table_get_changed || Get_GetEntityTableMode() == SubFunc_table_get_period)
        {
            if (SysTimer_IsTimerExpiered(RTC_CONTROL_ENTITYTABLE_POLLING) == 1)
            {
                SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_ENTITYTABLE_POLLING, Get_GetEntityTableReplyPeriod());
                // snprintf(log_msg, 60, "entity table mode:%d",Get_GetEntityTableMode());
                // RTC_LogMsg(Debug_Lev, log_msg);
                entity_tab_reply.cmd_id_rep = RespPositive_EntityTable;
                entity_tab_reply.sub_func = Get_GetEntityTableMode();
                entity_tab_reply.reply_period = Get_GetEntityTableReplyPeriod();
                entity_tab_reply.table_size = 0;

                if (Get_GetEntityTableMode() == SubFunc_table_get_changed)
                {
                    if (Is_EntityTable_Changed(entity_tab_reply.data, &entity_tab_reply.table_size) == 1)
                    {
                        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&entity_tab_reply, sizeof(usb_msg_entitytable_t));
                    }
                }
                else
                {
                    Get_EntityTable(entity_tab_reply.data, &entity_tab_reply.table_size);
                    USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&entity_tab_reply, sizeof(usb_msg_entitytable_t));
                }
            }
        }
    }
    return control_state;
}

CommonMsg_Actions_t RTC_Control_Hander_CommonMsg(USB_Task_msg_t *task_msg)
{
    CommonMsg_Actions_t res = Action_DoNothing;
    USB_TaskResp_msg_t task_resp;
    usb_msg_echo_t *p_echo_task;
    usb_msg_reset_t *p_reset_task;
    usb_msg_profile_t *p_profile_task;
    usb_msg_profile_t profile_msg;
    usb_msg_log_t *p_log_task;
    usb_msg_log_reply_t log_task_resp;
    usb_msg_entitytable_t *p_entity_tab_task;
    usb_msg_entity_pack_t *p_entity_pack_task;
    usb_msg_entitytable_reply_t entity_tab_reply;
    usb_msg_entity_pack_reply_t entity_pack_reply;

    unsigned short u16_tmp1; // u16_tmp2, u16_tmp3, u16_tmp4;
    unsigned char u8_tmp;
    char resp_msg[48];
    static unsigned int echo_cnt = 0;
    unsigned char i;

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
        res = Action_ReplyCommonMsg;
        break;

    case Cmd_Reset:
        p_reset_task = (usb_msg_reset_t *)task_msg;
        task_resp.cmd_id_rep = RespPositive_Reset;
        task_resp.sub_func = task_msg->sub_func;
        task_resp.data[0] = Dummy_00;
        task_resp.data[1] = Dummy_00;
        // reset_delay_ms = (p_reset_task->delay_time < C_MIN_RTC_CONTROL_RESET_DELAY_ms) ? C_MIN_RTC_CONTROL_RESET_DELAY_ms : p_reset_task->delay_time;
        memcpy(&u16_tmp1, &(p_reset_task->delay_time), sizeof(u16_tmp1));
        u16_tmp1 = (u16_tmp1 < C_MIN_RTC_CONTROL_RESET_DELAY_ms) ? C_MIN_RTC_CONTROL_RESET_DELAY_ms : u16_tmp1;
        reset_en = 1;
        IO_Entity_Mgr_Set_Entity(IO_PUNCHER_PISTON_UP_ENTITY, 1);
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_RESET_DELAY, (unsigned long)u16_tmp1);
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&task_resp, 4);
        res = Action_ReplyCommonMsg;
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
            RTC_Get_Profile(p_profile_task->profile_number, &profile_msg);
            profile_msg.cmd_id = RespPositive_Profile;
            profile_msg.sub_func = p_profile_task->sub_func;
            profile_msg.profile_number = p_profile_task->profile_number;
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&profile_msg, sizeof(usb_msg_profile_t));
        }
        res = Action_ReplyCommonMsg;
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
        res = Action_ReplyCommonMsg;
        break;

    case Cmd_EntityTable:
        p_entity_tab_task = (usb_msg_entitytable_t *)task_msg;
        if (p_entity_tab_task->sub_func == SubFunc_table_get_instant)
        {
            entity_tab_reply.cmd_id_rep = RespPositive_EntityTable;
            entity_tab_reply.sub_func = p_entity_tab_task->sub_func;
            entity_tab_reply.reply_period = Dummy_00;
            Set_GetEntityTableMode((enum EntityTable_SubFunc)p_entity_tab_task->sub_func);
            Get_EntityTable(entity_tab_reply.data, &entity_tab_reply.table_size);
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&entity_tab_reply, sizeof(usb_msg_entitytable_t));
        }
        else if (p_entity_tab_task->sub_func == SubFunc_table_get_changed)
        {
            entity_tab_reply.cmd_id_rep = RespPositive_EntityTable;
            entity_tab_reply.sub_func = p_entity_tab_task->sub_func;
            entity_tab_reply.reply_period = Dummy_00;
            entity_tab_reply.table_size = 0;
            SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_ENTITYTABLE_POLLING, Get_GetEntityTableReplyPeriod());
            Set_GetEntityTableMode((enum EntityTable_SubFunc)p_entity_tab_task->sub_func);
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&entity_tab_reply, 4);
        }
        else if (p_entity_tab_task->sub_func == SubFunc_table_get_period)
        {
            entity_tab_reply.cmd_id_rep = RespPositive_EntityTable;
            entity_tab_reply.sub_func = p_entity_tab_task->sub_func;
            entity_tab_reply.reply_period = p_entity_tab_task->reply_period;
            entity_tab_reply.table_size = 0;
            Set_GetEntityReplyPeriod(p_entity_tab_task->reply_period);
            SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_ENTITYTABLE_POLLING, Get_GetEntityTableReplyPeriod());
            Set_GetEntityTableMode((enum EntityTable_SubFunc)p_entity_tab_task->sub_func);
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&entity_tab_reply, 4);
        }
        else if (p_entity_tab_task->sub_func == SubFunc_table_get_off)
        {
            entity_tab_reply.cmd_id_rep = RespPositive_EntityTable;
            entity_tab_reply.sub_func = p_entity_tab_task->sub_func;
            entity_tab_reply.reply_period = Dummy_00;
            entity_tab_reply.table_size = 0;
            Set_GetEntityTableMode((enum EntityTable_SubFunc)p_entity_tab_task->sub_func);
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&entity_tab_reply, 4);
        }
        res = Action_ReplyCommonMsg;
        break;

    case Cmd_EntityPack:
        p_entity_pack_task = (usb_msg_entity_pack_t *)task_msg;
        if (p_entity_pack_task->sub_func == SubFunc_pack_get)
        {
            entity_pack_reply.cmd_id_rep = RespPositive_EntityPack;
            entity_pack_reply.sub_func = p_entity_pack_task->sub_func;
            entity_pack_reply.pack_size = p_entity_pack_task->pack_size;
            entity_pack_reply.argv = Dummy_00;
            for (i = 0; i < p_entity_pack_task->pack_size; i++)
            {
                entity_pack_reply.entity_pack[i].entity_name = p_entity_pack_task->entity_pack[i].entity_name;
                entity_pack_reply.entity_pack[i].entity_value = GetIO_ByEntityName(p_entity_pack_task->entity_pack[i].entity_name);
            }
            u8_tmp = p_entity_pack_task->pack_size << 1;
            u8_tmp += 4;
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&entity_pack_reply, u8_tmp);
        }
        else if (p_entity_pack_task->sub_func == SubFunc_pack_set)
        {
            entity_pack_reply.cmd_id_rep = RespPositive_EntityPack;
            entity_pack_reply.sub_func = p_entity_pack_task->sub_func;
            entity_pack_reply.pack_size = p_entity_pack_task->pack_size;
            entity_pack_reply.argv = Dummy_00;
            for (i = 0; i < p_entity_pack_task->pack_size; i++)
            {
                WriteValue_ByEntityName(p_entity_pack_task->entity_pack[i].entity_name,
                                        p_entity_pack_task->entity_pack[i].entity_value);
            }
            USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&entity_pack_reply, 4);
        }
        res = Action_ReplyCommonMsg;
        break;

    case Cmd_Z_PulseGen:
        res = Action_PulseGen_Z;
        break;

    case Cmd_X_PulseGen:
        res = Action_PulseGen_X;
        break;

    case Cmd_ControlModeSwitch:
        if (task_msg->sub_func == SubFunc_controlmode_uninit)
            res = Action_SwithMode_Uninit;
        else if (task_msg->sub_func == SubFunc_controlmode_ready)
            res = Action_SwithMode_Ready;
        else if (task_msg->sub_func == SubFunc_controlmode_home)
            res = Action_SwithMode_Home;
        else if (task_msg->sub_func == SubFunc_controlmode_diagnosis)
            res = Action_SwithMode_Diagnosis;
        break;

    case Cmd_HomeParts:
        res = Action_Home_Parts;
        break;

    default:
        res = Action_DoNothing;
    }
    return res;
}

void RTC_Control_Handler_Uninit(CommonMsg_Actions_t cmd, USB_Task_msg_t *task_msg)
{
    char log_msg[60];
    static char led_wink_status = -1;
    usb_msg_control_mode_switch_t *p_mode_switch_task = (usb_msg_control_mode_switch_t *)task_msg;
    usb_msg_control_mode_switch_reply_t mode_switch_reply;

    RTC_Control_Wink_Entity_Debug(&led_wink_status,C_RTC_CONTROL_WINK_Uninit_ms, IO_ZA2_PISTON_ENTITY);

    switch (cmd)
    {
    case Action_DoNothing:
    case Action_ReplyCommonMsg:
        Nop();
        break;

    case Action_Home_Parts:
        snprintf(log_msg, 60, "Action Home illegal, rtc mode:%d", control_state);
        RTC_LogMsg(Debug_Lev, log_msg);
        break;

    case Action_PulseGen_Z:
    case Action_PulseGen_X:
        snprintf(log_msg, 60, "Action PulseGen illegal, rtc mode:%d", control_state);
        RTC_LogMsg(Debug_Lev, log_msg);
        break;

    case Action_SwithMode_Uninit:

        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_NoChanges;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Ready:
        control_state = RTC_CONTROL_STATE_READY;
        led_wink_status = -1;
        IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);

        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_Success;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Home:
        control_state = RTC_CONTROL_STATE_HOME;
        led_wink_status = -1;
        IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_Success;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Diagnosis:
        control_state = RTC_CONTROL_STATE_DIAGNOSIS;
        led_wink_status = -1;
        IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_Success;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;
    }
}

void RTC_Control_Handler_Ready(CommonMsg_Actions_t cmd, USB_Task_msg_t *task_msg)
{
    char log_msg[60];
    unsigned short u16_tmp1, u16_tmp2, u16_tmp3, u16_tmp4;

    usb_msg_control_mode_switch_t *p_mode_switch_task = (usb_msg_control_mode_switch_t *)task_msg;
    usb_msg_control_mode_switch_reply_t mode_switch_reply;
    usb_msg_z_pulse_gen_t *p_z_pulse_gen_task;
    usb_msg_z_pulse_gen_reply_t z_pulse_gen_reply;
    usb_msg_x_pulse_gen_t *p_x_pulse_gen_task;
    usb_msg_x_pulse_gen_reply_t x_pulse_gen_reply;
    OCx_src_t ocx_scr;

    static char led_wink_status = -1;
    RTC_Control_Wink_Entity_Debug(&led_wink_status,C_RTC_CONTROL_WINK_Ready_ms, IO_ZA2_PISTON_ENTITY);


    switch (cmd)
    {
    case Action_DoNothing:
    case Action_ReplyCommonMsg:
        Nop();
        break;
    case Action_PulseGen_Z:
        p_z_pulse_gen_task = (usb_msg_z_pulse_gen_t *)task_msg;
        if (p_z_pulse_gen_task->sub_func == SubFunc_z_pulse_gen_off)
        {
            z_pulse_off_by_usb_msg();
        }
        else if (p_z_pulse_gen_task->sub_func == SubFunc_z_pulse_gen_pwm)
        {
            memcpy(&u16_tmp1, &(p_z_pulse_gen_task->z_pwm_value.period_hiword), sizeof(u16_tmp1));
            memcpy(&u16_tmp2, &(p_z_pulse_gen_task->z_pwm_value.period_loword), sizeof(u16_tmp1));
            memcpy(&u16_tmp3, &(p_z_pulse_gen_task->z_pwm_value.dutyon_hiword), sizeof(u16_tmp1));
            memcpy(&u16_tmp4, &(p_z_pulse_gen_task->z_pwm_value.dutyon_loword), sizeof(u16_tmp1));
            z_pulse_update_by_usb_msg(u16_tmp1, u16_tmp2, u16_tmp3, u16_tmp4);
        }
        z_pulse_gen_reply.cmd_id_rep = RespPositive_Z_PulseGen;
        z_pulse_gen_reply.sub_func = p_z_pulse_gen_task->sub_func;
        z_pulse_gen_reply.argv_0 = Dummy_00;
        z_pulse_gen_reply.argv_1 = Dummy_00;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&z_pulse_gen_reply, 4);
        break;
    case Action_PulseGen_X:
        p_x_pulse_gen_task = (usb_msg_x_pulse_gen_t *)task_msg;
        if (p_x_pulse_gen_task->sub_func == SubFunc_x_pulsemode_run_stop)
        {
            Nop(); // not yet implement
        }
        else if (p_x_pulse_gen_task->sub_func == SubFunc_x_pulsemode_trapezoid)
        {
            // OCx_src_t *p_ocx_scr = (OCx_src_t *)&p_x_pulse_gen_task->x_sequence;
            memcpy(&u16_tmp1, &(p_x_pulse_gen_task->steps), sizeof(u16_tmp1));
            memcpy(&ocx_scr, (OCx_src_t *)&p_x_pulse_gen_task->x_sequence, sizeof(OCx_src_t));
            x_pulse_update_by_usb_msg(&ocx_scr, u16_tmp1);
        }
        x_pulse_gen_reply.cmd_id_rep = RespPositive_X_PulseGen;
        x_pulse_gen_reply.sub_func = p_x_pulse_gen_task->sub_func;
        x_pulse_gen_reply.argv_0 = Dummy_00;
        x_pulse_gen_reply.argv_1 = Dummy_00;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&x_pulse_gen_reply, 4);
        break;

    case Action_Home_Parts:
        snprintf(log_msg, 60, "Action Home illegal, rtc mode:%d", control_state);
        RTC_LogMsg(Debug_Lev, log_msg);
        break;

    case Action_SwithMode_Uninit:
        control_state = RTC_CONTROL_STATE_UNINIT;
        led_wink_status = -1;
        IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_Success;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Ready:
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_NoChanges;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Home:
        control_state = RTC_CONTROL_STATE_HOME;
        led_wink_status = -1;
        IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_Success;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Diagnosis:
        control_state = RTC_CONTROL_STATE_DIAGNOSIS;
        led_wink_status = -1;
        IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_Success;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;
    }
}

void RTC_Control_Handler_Home(CommonMsg_Actions_t cmd, USB_Task_msg_t *task_msg)
{
    usb_msg_control_mode_switch_t *p_mode_switch_task = (usb_msg_control_mode_switch_t *)task_msg;
    usb_msg_control_mode_switch_reply_t mode_switch_reply;
    usb_msg_home_parts_t *p_home_parts_task = (usb_msg_home_parts_t *)task_msg;
    usb_msg_home_parts_reply_t home_parts_reply;

    char log_msg[60];
    static char led_wink_status = -1;
    RTC_Control_Wink_Entity_Debug(&led_wink_status, C_RTC_CONTROL_WINK_Home_ms, IO_ZA2_PISTON_ENTITY);

    switch (cmd)
    {
    case Action_DoNothing:
    case Action_ReplyCommonMsg:
        Nop();
        break;
    case Action_PulseGen_Z:
    case Action_PulseGen_X:
        snprintf(log_msg, 60, "Action PulseGen illegal, rtc mode:%d", control_state);
        RTC_LogMsg(Debug_Lev, log_msg);
        break;
    case Action_SwithMode_Uninit:
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        if (Is_LECPA_30_HomeProcedure_Idle() == -1)
        {
            mode_switch_reply.switch_status = SwitchMode_Fail;
        }
        else
        {
            led_wink_status = -1;
            IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);

            control_state = RTC_CONTROL_STATE_UNINIT;
            mode_switch_reply.switch_status = SwitchMode_Success;
        }
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Ready:
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        if (Is_LECPA_30_HomeProcedure_Idle() == -1)
        {
            mode_switch_reply.switch_status = SwitchMode_Fail;
        }
        else
        {
            led_wink_status = -1;
            IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);

            control_state = RTC_CONTROL_STATE_READY;
            mode_switch_reply.switch_status = SwitchMode_Success;
        }
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Home:
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_NoChanges;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Diagnosis:
        control_state = RTC_CONTROL_STATE_DIAGNOSIS;
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        if (Is_LECPA_30_HomeProcedure_Idle() == -1)
        {
            mode_switch_reply.switch_status = SwitchMode_Fail;
        }
        else
        {
            led_wink_status = -1;
            IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);
            control_state = RTC_CONTROL_STATE_DIAGNOSIS;
            mode_switch_reply.switch_status = SwitchMode_Success;
        }
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_Home_Parts:
        home_parts_reply.cmd_id_rep = RespPositive_HomeParts;
        home_parts_reply.sub_func = p_home_parts_task->sub_func;

        if (p_home_parts_task->sub_func == SubFunc_home_WinderStepper)
        {
            Nop(); // not yet implement
        }
        else if (p_home_parts_task->sub_func == SubFunc_home_LECPA_100)
        {
            Nop(); // not yet implement
        }
        else if (p_home_parts_task->sub_func == SubFunc_home_LECPA_30)
        {
            if (Is_LECPA_30_HomeProcedure_Idle() == -1)
            {
                home_parts_reply.home_procedure = LECPA_Init;
                home_parts_reply.home_status = LECPA_Home_Deny;
            }
            else
            {
                home_parts_reply.home_procedure = LECPA_Init;
                home_parts_reply.home_status = LECPA_Home_Start;

                // create a timer for homing.
            }
        }
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&home_parts_reply, 4);
        break;
    }    
}

void RTC_Control_Handler_Diagnosis(CommonMsg_Actions_t cmd, USB_Task_msg_t *task_msg)
{
    usb_msg_control_mode_switch_t *p_mode_switch_task = (usb_msg_control_mode_switch_t *)task_msg;
    usb_msg_control_mode_switch_reply_t mode_switch_reply;
    static char led_wink_status = -1;
    char log_msg[60];
    
    RTC_Control_Wink_Entity_Debug(&led_wink_status,C_RTC_CONTROL_WINK_Diagnosis_ms, IO_ZA2_PISTON_ENTITY);

    switch (cmd)
    {
    case Action_DoNothing:
    case Action_ReplyCommonMsg:
        Nop();
        break;

    case Action_Home_Parts:
        snprintf(log_msg, 60, "Action Home illegal, rtc mode:%d", control_state);
        RTC_LogMsg(Debug_Lev, log_msg);
        break;

    case Action_PulseGen_Z:
    case Action_PulseGen_X:
        snprintf(log_msg, 60, "Action PulseGen illegal, rtc mode:%d", control_state);
        RTC_LogMsg(Debug_Lev, log_msg);
        break;

    case Action_SwithMode_Uninit:
        led_wink_status = -1;
        IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);
        control_state = RTC_CONTROL_STATE_UNINIT;
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_Success;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Ready:
        led_wink_status = -1;
        IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);
        control_state = RTC_CONTROL_STATE_READY;
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_Success;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Home:
        led_wink_status = -1;
        IO_Entity_Mgr_Set_Entity(IO_ZA2_PISTON_ENTITY, 0);
        control_state = RTC_CONTROL_STATE_HOME;
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_Success;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;

    case Action_SwithMode_Diagnosis:
        mode_switch_reply.cmd_id_rep = RespPositive_ControlModeSwitch;
        mode_switch_reply.sub_func = p_mode_switch_task->sub_func;
        mode_switch_reply.control_status = (char)control_state;
        mode_switch_reply.switch_status = SwitchMode_NoChanges;
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&mode_switch_reply, 4);
        break;
    }
}

void RTC_Control_Wink_Entity_Debug(char *flag, unsigned long delay_time, int entity_name)
{
    if (*flag == -1)
    {
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_WINK, delay_time);
        *flag = 0;
        return;
    }
    if (SysTimer_IsTimerExpiered(RTC_CONTROL_WINK) == 1)
    {
        SysTimer_SetTimerInMiliSeconds(RTC_CONTROL_WINK, delay_time);
        IO_Entity_Mgr_Set_Entity(entity_name, !IO_Entity_Mgr_Get_Entity(entity_name));
    }
}