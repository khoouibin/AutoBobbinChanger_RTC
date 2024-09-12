#ifndef _RTC_Profile_H_
#define	_RTC_Profile_H_
#include "usb_app.h"

typedef struct
{
    UCHAR_8 profile_01_01;
    CHAR_8 profile_01_02;
    UINT_16 profile_01_03;
    INT_16 profile_01_04;
    UINT_32 profile_01_05;
    INT_32 profile_01_06;
} RTC_Profile_01_t;

unsigned char RTC_Profile_Update(usb_msg_profile_t *profile_msg);
unsigned char RTC_Get_Profile_1(RTC_Profile_01_t *profile_01);
unsigned char RTC_Get_Profile(char profile_number, usb_msg_profile_t *profile_msg);

#endif

