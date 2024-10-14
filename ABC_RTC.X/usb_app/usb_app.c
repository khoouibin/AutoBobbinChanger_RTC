#include <xc.h>
#include <stdio.h>
#include <stdbool.h>

#include "usb_app.h"
#include "RTC_Log.h"
#include "Ons_General.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include "IO_Entity.h"
#include "RTC_Control.h"

unsigned char USB_RxBuf[MSG_MAX_SIZE];

USB_Transaction_State_t TxTransState = {0};
USB_Transaction_State_t RxTransState = {0};

volatile USB_HANDLE USBOutHandle;
volatile USB_HANDLE USBInHandle;
char debug_str[48];

void __attribute__((interrupt, no_auto_psv)) _USB1Interrupt(void)
{
    USBDeviceTasks();
    USB_RxBulkBuffer_Get_From_Bus();
}

void USB_DeviceInitialize(void)
{
    USBInHandle = 0;
    USBEnableEndpoint(CUSTOM_DEVICE_HID_EP, USB_IN_ENABLED | USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
    USBOutHandle = (volatile USB_HANDLE)HIDRxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t *)&USB_RxBuf[0], MSG_MAX_SIZE);
    USB_TransStateInit();
}

void USB_TransStateInit(void)
{
    TxTransState.Init = 1;
    TxTransState.Mutex = 0;
    TxTransState.Stuck = 0; // effective.
    TxTransState.Ptr_buff = -1;
    TxTransState.Ptr_comp = -1;

    RxTransState.Init = 1;
    RxTransState.Mutex = 0;
    RxTransState.Stuck = 0;
    RxTransState.Ptr_buff = -1;
    RxTransState.Ptr_comp = -1;

    LATHbits.LATH10 = 0;
}

char USB_RxBulkBuffer_Get_From_Bus(void)
{
    int i_RxLength = 0;
    if (USBGetDeviceState() < CONFIGURED_STATE)
    {
        return -1;
    }

    if (HIDRxHandleBusy(USBOutHandle) == false)
    {
        if (RxTransState.Ptr_buff == -1)
            RxTransState.Ptr_buff = 0;

        i_RxLength = USBHandleGetLength(USBOutHandle);
        USBOutHandle = HIDRxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t *)&USB_RxBuf[0], MSG_MAX_SIZE);

        memcpy(RxTransState.Buff[(unsigned char)RxTransState.Ptr_buff], USB_RxBuf, MSG_MAX_SIZE);
        RxTransState.MsgSize[(unsigned char)RxTransState.Ptr_buff] = i_RxLength;

        RxTransState.Ptr_buff++;
        RxTransState.Ptr_buff &= 0x07;

        if ((RxTransState.Ptr_buff ^ RxTransState.Ptr_comp) == 0)
            RxTransState.Stuck = 1;

        return 0;
    }

    // note:
    // 1. USBHandleGetLength must in front of HIDRxPacket.
    //    and HIDRxPacket leng(read out length) should be maximum.
    //    to access usb_handle once, that will changes state of usb_handle.
    //    to use usb_handle must very carefully.
    // 2. if receive message completed, the usb_handle will change state
    //    ( HIDRxHandleBusy=false ) and must to access data as soon as possible.
    //    then after access data, the usb_handle change state to busy
    //    ( HIDRxHandleBusy=busy ) untill to next next message interrupt.
    //    so that, cannot do this, that will cause program crash:
    //    while(1){
    //      if(HIDRxHandleBusy(USBOutHandle) == false)
    //          break;
    //    }
    //
    return 1;
}

char read_rxbuf(unsigned char *var)
{
    *var = USB_RxBuf[0];
    return 0;
}

char USB_Msg_To_TxBulkBuffer(ptr_usb_msg_u8 send_msg, unsigned char msg_size) // FirstIn
{
    char *ptr_TxBuff;
    ptr_TxBuff = &TxTransState.Ptr_buff;

    if (TxTransState.Ptr_buff == -1)
    {
        TxTransState.Ptr_buff = 0;
        TxTransState.Ptr_comp = 0;
    }

    memcpy(TxTransState.Buff[(unsigned char)TxTransState.Ptr_buff], send_msg, msg_size);
    TxTransState.MsgSize[(unsigned char)TxTransState.Ptr_buff] = msg_size;
    TxTransState.Ptr_buff++;
    TxTransState.Ptr_buff &= 0x07;

    if ((TxTransState.Ptr_buff ^ TxTransState.Ptr_comp) == 0)
        TxTransState.Stuck = 1;
    return 0;
}

char USB_TxBulkBuffer_To_Bus(void) // FirstOut
{
    char tx_ptr = TxTransState.Ptr_comp;

    if (TxTransState.Init != 1)
        return -1;

    if ((TxTransState.Ptr_buff ^ TxTransState.Ptr_comp) == 0)
        return 1;

    if (BL_USB_TxMutex_Get() == 1)
        return 2; // usb data transmit, need to wait.

    if (TxTransState.Stuck == 1)
    {
        LATHbits.LATH10 = 1;
        return -2; // host usb stuck. need reset usb.
    }
    if (!HIDTxHandleBusy(USBInHandle))
    {
        USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t *)&TxTransState.Buff[(unsigned char)tx_ptr][0], TxTransState.MsgSize[(unsigned char)tx_ptr]);
        // BL_USB_TxMutex_Set();
        TxTransState.Ptr_comp++;
        TxTransState.Ptr_comp &= 0x07;
    }
    return 0;
}

void BL_USB_Tx_1mISR_Set(void)
{
    TxTransState.flag_ISR = 1;
}

void BL_USB_Tx_1mISR_Clr(void)
{
    TxTransState.flag_ISR = 0;
}

bool BL_USB_Tx_1mISR_Get(void)
{
    return TxTransState.flag_ISR;
}

void BL_USB_TxMutex_Set(void)
{
    TxTransState.Mutex = 1;
}

void BL_USB_TxMutex_Clr(void)
{ // cannot do this function, lead to checksum error.
    if (TxTransState.Mutex == 1)
    {
        TxTransState.Mutex = 0;
        TxTransState.Ptr_comp++;
        TxTransState.Ptr_comp &= 0x07;
    }
}

bool BL_USB_TxMutex_Get(void)
{
    return TxTransState.Mutex;
}

void BL_USB_Rx_4mISR_Set(void)
{
    static unsigned char inc_4ms = 0;

    inc_4ms++;
    inc_4ms &= 1;
    if (inc_4ms == 0)
        RxTransState.flag_ISR = 1;
}

void BL_USB_Rx_4mISR_Clr(void)
{
    RxTransState.flag_ISR = 0;
}

bool BL_USB_Rx_4mISR_Get(void)
{
    return RxTransState.flag_ISR;
}

/*
void BL_USB_RxMutex_Set(void)
{
    RxTransState.Mutex=1;
}

void BL_USB_RxMutex_Clr(void)
{
    Nop();
}

bool BL_USB_RxMutex_Get(void)
{
    return RxTransState.Mutex;
}
*/
char USB_Msg_From_RxBuffer(usb_msg_u8 *msg_cmd, unsigned char *msg_size)
{
    char res = -1;
    char *ptr_RxBuff;
    ptr_RxBuff = &RxTransState.Ptr_comp;

    if (RxTransState.Init != 1)
    {
        res = -1;
        return res;
    }
    if ((RxTransState.Ptr_buff ^ RxTransState.Ptr_comp) == 0)
    {
        res = 1;
        return res;
    }
    if (RxTransState.Ptr_buff != -1 && RxTransState.Ptr_comp == -1)
    {
        RxTransState.Ptr_comp = 0;
        res = -2;
        return res;
    }
	memcpy(msg_cmd, (ptr_usb_msg_u8)RxTransState.Buff[(unsigned char)*ptr_RxBuff], MSG_MAX_SIZE);
    *msg_size = RxTransState.MsgSize[(unsigned char)*ptr_RxBuff];
    RxTransState.Ptr_comp++;
    RxTransState.Ptr_comp &= 0x07;
    res = 0;
    return res;
}

bool USB_Msg_Parser(USB_Task_msg_t *task_msg)
{
    char neg_msg[60];
    usb_msg_u8 msg[MSG_MAX_SIZE];
    USB_Task_msg_t *p_taskmsg;
    unsigned char msg_length;
    bool b_new_msg = false;
    unsigned char nrc_res;
    char msg_res;
    p_taskmsg = (USB_Task_msg_t *)msg;
    msg_res = USB_Msg_From_RxBuffer(msg, &msg_length);
    if (msg_res == 0)
    {
        nrc_res = Is_USB_Msg_NegResponse(p_taskmsg);
        if (nrc_res == POSITIVE_CODE)
        {
            memcpy(task_msg, (usb_msg_u8 *)msg, MSG_MAX_SIZE);
            b_new_msg = true;
        }
        else if (nrc_res == NRC_SUBFUNC_OUTRANGE)
        {
            snprintf(neg_msg, 60, "error message:%s", "subfunction out of range");
            USB_NegResp(p_taskmsg->cmd_id, nrc_res, neg_msg);
        }
        else if (nrc_res == NRC_SIZE_EXCEED)
        {
            snprintf(neg_msg, 60, "error message:%s", "_size out of range");
            USB_NegResp(p_taskmsg->cmd_id, nrc_res, neg_msg);
        }
        else if (nrc_res == NRC_ILLEGAL_RTC_MODE)
        {
            snprintf(neg_msg, 60, "error message:%s", "command not suit for rtc mode");
            USB_NegResp(p_taskmsg->cmd_id, nrc_res, neg_msg);
        }
        else
        {
            snprintf(neg_msg, 60, "error message:%s", "unknown command message");
            USB_NegResp(p_taskmsg->cmd_id, nrc_res, neg_msg);
        }
    }
    return b_new_msg;
}

unsigned char Is_USB_Msg_NegResponse(USB_Task_msg_t *task_msg)
{
    unsigned char res_code = NRC_CMD_NOT_FOUND;
    unsigned char i;
    usb_msg_log_t *p_log_task=(usb_msg_log_t *)task_msg;
    usb_msg_entity_pack_t *p_entity_pack_task =(usb_msg_entity_pack_t*)task_msg;

    for (i = 0; i < Cmd_MAX; i++)
    {
        if (i == task_msg->cmd_id)
        {
            res_code = POSITIVE_CODE;
            break;
        }
    }
    if (res_code == NRC_CMD_NOT_FOUND)
        return res_code;

    switch (task_msg->cmd_id)
    {
    case Cmd_Echo:
        res_code = (task_msg->sub_func == SubFunc_55 || task_msg->sub_func == SubFunc_AA)
                       ? POSITIVE_CODE
                       : NRC_SUBFUNC_OUTRANGE;
        break;

    case Cmd_Reset:
        res_code = NRC_SUBFUNC_OUTRANGE;
        for (i = 0; i < SubFunc_reset_max; i++)
        {
            if (i == task_msg->sub_func)
            {
                res_code = POSITIVE_CODE;
                break;
            }
        }
        break;

    case Cmd_Profile:
        res_code = NRC_SUBFUNC_OUTRANGE;
        for (i = 0; i < SubFunc_profile_max; i++)
        {
            if (i == task_msg->sub_func)
            {
                res_code = POSITIVE_CODE;
                break;
            }
        }
        break;

    case Cmd_Log:
        res_code = NRC_SUBFUNC_OUTRANGE;
        for (i = 0; i < SubFunc_log_max - 1; i++)
        {
            if (i == p_log_task->sub_func)
            {
                res_code = POSITIVE_CODE;
                break;
            }
        }
        if (p_log_task->sub_func == SubFunc_log_level_set)
        {
            res_code = NRC_DATA_OUTRANGE;
            for (i = 0; i < LogLev_MAX; i++)
            {
                if (i == p_log_task->set_level)
                {
                    res_code = POSITIVE_CODE;
                    break;
                }
            }
        }
        else
        {
            res_code = POSITIVE_CODE;
        }
        break;

    case Cmd_EntityTable:
        res_code = NRC_SUBFUNC_OUTRANGE;
        for (i = 0; i < SubFunc_entitytable_max; i++)
        {
            if (i == task_msg->sub_func)
            {
                res_code = POSITIVE_CODE;
                break;
            }
        }
        break;

    case Cmd_EntityPack:
        res_code = NRC_SUBFUNC_OUTRANGE;
        for (i = 0; i < SubFunc_entitypack_max; i++)
        {
            if (i == task_msg->sub_func)
            {
                res_code = POSITIVE_CODE;
                break;
            }
        }
        res_code = NRC_SIZE_EXCEED;
        if (p_entity_pack_task->pack_size <= MSG_ENTITY_MAX_PACK_SIZE)
            res_code = POSITIVE_CODE;
        break;

    case Cmd_Z_PulseGen:
        res_code = NRC_SUBFUNC_OUTRANGE;
        for (i = 0; i < SubFunc_z_pulse_gen_max; i++)
        {
            if (i == task_msg->sub_func)
            {
                res_code = POSITIVE_CODE;
                break;
            }
        }
        break;

    case Cmd_X_PulseGen:
        res_code = NRC_SUBFUNC_OUTRANGE;
        for (i = 0; i < SubFunc_x_pulsemode_max; i++)
        {
            if (i == task_msg->sub_func)
            {
                res_code = POSITIVE_CODE;
                break;
            }
        }
        break;

    case Cmd_ControlModeSwitch:
        res_code = NRC_SUBFUNC_OUTRANGE;
        for (i = 0; i < SubFunc_controlmode_max; i++)
        {
            if (i == task_msg->sub_func)
            {
                res_code = POSITIVE_CODE;
                break;
            }
        }
        break;

    case Cmd_HomeParts:
        res_code = NRC_SUBFUNC_OUTRANGE;
        for (i = 0; i < SubFunc_home_max; i++)
        {
            if (i == task_msg->sub_func)
            {
                res_code = POSITIVE_CODE;
                break;
            }
        }
        res_code = NRC_ILLEGAL_RTC_MODE;
        if ( Get_RTC_Control_State() == RTC_CONTROL_STATE_HOME)
            res_code = POSITIVE_CODE;
        break;
    }
    return res_code;
}

char USB_NegResp(unsigned char cmd_id, unsigned char neg_code, char* strmsg)
{
    USB_NegResponse_msg_t neg_resp;
    memset(neg_resp.data, 0, 60);
    neg_resp.resp_id = RespNeg;
    neg_resp.cmd_id = cmd_id;    
    neg_resp.neg_code = neg_code;
    neg_resp.neg_argv = Dummy_ff;
    memcpy(neg_resp.data, strmsg, 60);
    USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&neg_resp, 64);
    return 0;
}
