#ifndef _RTC_Log_H_
#define	_RTC_Log_H_
#include "usb_app.h"

enum Log_Mask
{
    Critial_MASK = 0x01,
    Error_MASK = 0x03,
    Warning_MASK = 0x07,
    Info_MASK = 0x0f,
    Debug_MASK = 0x1f,
};

enum LogLev
{
    Critial_Lev = 1,
    Error_Lev = 2,
    Warning_Lev = 4,
    Info_Lev = 8,
    Debug_Lev = 16,
    LogLev_MAX
};
void RTC_LogLevel_Set(enum LogLev u8_value);
enum LogLev RTC_LogLevel_Get(void);
unsigned char RTC_LogMsg(enum LogLev level, uint8_t *log_msg);

#endif

