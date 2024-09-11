#ifndef _RTC_Profile_H_
#define	_RTC_Profile_H_
#include "usb_app.h"

typedef struct
{
    unsigned char profile_01_01;
    signed char profile_01_02;
    unsigned short profile_01_03;
    signed short profile_01_04;
    unsigned long profile_01_05;
    signed long profile_01_06;
} RTC_Profile_01_t;

unsigned char RTC_Profile_Update(usb_msg_profile_t *profile_msg);
unsigned char RTC_Get_Profile_1(RTC_Profile_01_t *profile_01);

#endif

