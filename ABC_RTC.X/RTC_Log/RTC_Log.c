#include "RTC_Log.h"
#include "xc.h"
#include "usb_app.h"
#include <stdio.h>
#include <stdbool.h>

static unsigned short log_counter = {0};
static enum LogLev u8_LogLevel = {Debug_Lev};
static enum_log log_level_tab[] =
    {
        {level_critial, mask_critial},
        {level_error, mask_error},
        {level_warning, mask_warning},
        {level_info, mask_info},
        {level_debug, mask_debug},
};

static enum_header log_header_tab[] =
    {
        {level_critial, (char *)"[crit]", 5}, // 5size
        {level_error, (char *)"[err]", 5},
        {level_warning, (char *)"[warn]", 6},
        {level_info, (char *)"[info]", 6},
        {level_debug, (char *)"[debug]", 7},
};

enum LogLev RTC_LogLevel_Get(void)
{
    return u8_LogLevel;
}

void RTC_LogLevel_Set(enum LogLev u8_value)
{
    u8_LogLevel = u8_value;
}

char RTC_LogMsg(enum LogLev u8_value, char *log_msg)
{
    usb_msg_log_reply_t log_reply;
    char log_mask;
    char header_size;
    char *log_header = GetLevelHeader_by_inner_level(u8_LogLevel, &header_size);
    log_reply.cmd_id_rep = RespPositive_Log;
    log_reply.sub_func = SubFunc_log_msg_reply;
    log_reply.log_counter = log_counter;
    log_mask = GetLevelMask_by_inner_level(u8_LogLevel);
    if (log_mask && u8_value > 0)
    {
        memcpy(log_reply.data, log_header, header_size);
        memcpy(log_reply.data + header_size, log_msg, sizeof(log_reply.data) - header_size);
        USB_Msg_To_TxBulkBuffer((ptr_usb_msg_u8)&log_reply, sizeof(log_reply));
        log_counter += 1;
    }
    return 0;
}

char RTC_GetLevelMask(enum_log *table, enum LogLev log_level)
{
    while (table->u8_mask != 0)
    {
        if (table->u8_level == (char)log_level)
            return (char)table->u8_mask;
        table++;
    }
    return -1;
}

char GetLevelMask_by_inner_level(enum LogLev value)
{
    return RTC_GetLevelMask(log_level_tab, value);
}

char *RTC_GetLevelHeader(enum_header *table, enum LogLev log_level, char *header_size)
{
    while (table->log_header != 0)
    {
        if (table->u8_level == (char)log_level)
        {
            *header_size = table->header_size;
            return table->log_header;
        }
        table++;
    }
    return "";
}

char *GetLevelHeader_by_inner_level(enum LogLev value, char *header_size)
{
    return RTC_GetLevelHeader(log_header_tab, value, header_size);
}
