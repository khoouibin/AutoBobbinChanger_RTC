#include "RTC_Profile.h"
#include "xc.h"
#include "usb_app.h"
#include <stdio.h>
#include <stdbool.h>

static RTC_Profile_01_t local_profile_01 = {0,0,0,0,0,0};
unsigned char RTC_Profile_Update(usb_msg_profile_t *profile_msg)
{
    RTC_Profile_01_t* p_rtc_profile_01=(RTC_Profile_01_t*)profile_msg->data;

    switch (profile_msg->profile_number)
    {
    case 01:
        local_profile_01.profile_01_01 = p_rtc_profile_01->profile_01_01;
        local_profile_01.profile_01_02 = p_rtc_profile_01->profile_01_02;
        local_profile_01.profile_01_03 = p_rtc_profile_01->profile_01_03;
        local_profile_01.profile_01_04 = p_rtc_profile_01->profile_01_04;
        local_profile_01.profile_01_05 = p_rtc_profile_01->profile_01_05;
        local_profile_01.profile_01_06 = p_rtc_profile_01->profile_01_06;
        break;
    }

    return 0;
}

unsigned char RTC_Get_Profile_1(RTC_Profile_01_t *profile_01)
{
    memcpy(profile_01, (RTC_Profile_01_t *)&local_profile_01, 60);
    return 0;
}