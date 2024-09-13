#ifndef _RTC_Profile_H_
#define	_RTC_Profile_H_
#include "usb_app.h"

typedef ptr_usb_msg_u8 pU8;

typedef struct
{
    UCHAR_8 p01_01;
    CHAR_8 p01_02;
    UINT_16 p01_03;
    INT_16 p01_04;
    UINT_32 p01_05;
    INT_32 p01_06;
} RTC_Profile_01_t;

typedef struct
{
    UCHAR_8 p02_01;
    CHAR_8 p02_02;
    INT_16 p02_03;
    INT_16 p02_04;
    INT_32 p02_05;
    INT_32 p02_06;
    INT_32 p02_07;
    CHAR_8 p02_08;
    INT_32 p02_09;
    CHAR_8 p02_10;
    INT_32 p02_11;
} RTC_Profile_02_t;

unsigned char RTC_Profile_Update(usb_msg_profile_t *profile_msg);
unsigned char RTC_Get_Profile(char profile_number, usb_msg_profile_t *profile_msg);

#endif

