#ifndef _RTC_Log_H_
#define	_RTC_Log_H_
#include "usb_app.h"

#define AT_20000h __attribute__((space(prog), address(0x20000),section(".RTC_LogTab")))

enum LogMask
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

typedef enum {
    level_critial =Critial_Lev,
    level_error =Error_Lev, 
    level_warning =Warning_Lev, 
    level_info =Info_Lev,
    level_debug =Debug_Lev
} log_level;

typedef enum {
    mask_critial =Critial_MASK,
    mask_error =Error_MASK,
    mask_warning=Warning_MASK,
    mask_info=Info_MASK,
    mask_debug=Debug_MASK,
} log_mask;

typedef struct
{
    enum LogLev u8_level;
    enum LogMask u8_mask;
} enum_log;

typedef struct
{
    enum LogLev u8_level;
    char* log_header;
    char header_size;
} enum_header;


void RTC_LogLevel_Set(enum LogLev u8_value);
enum LogLev RTC_LogLevel_Get(void);
char RTC_LogMsg(enum LogLev level, char *log_msg);

char RTC_GetLevelMask(enum_log *table, enum LogLev log_level);
char GetLevelMask_by_inner_level(enum LogLev value);

char *RTC_GetLevelHeader(enum_header *table, enum LogLev log_level,char* header_size);
char *GetLevelHeader_by_inner_level(enum LogLev value, char* header_size);
#endif

