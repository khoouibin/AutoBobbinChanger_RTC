#include "RTC_Log.h"
#include "xc.h"
#include "usb_app.h"
#include <stdio.h>
#include <stdbool.h>

static unsigned short log_counter = {0};
static enum LogLev u8_LogLevel = {Debug_Lev};

enum LogLev RTC_LogLevel_Get(void)
{
    return u8_LogLevel;
}

void RTC_LogLevel_Set(enum LogLev u8_value)
{
    u8_LogLevel = u8_value;
}

unsigned char RTC_LogMsg(enum LogLev u8_value, uint8_t *log_msg)
{

    log_counter += 1;
    return 0;
}
