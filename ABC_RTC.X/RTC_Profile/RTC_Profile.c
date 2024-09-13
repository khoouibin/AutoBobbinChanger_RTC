#include "RTC_Profile.h"
#include "xc.h"
#include "usb_app.h"
#include <stdio.h>
#include <stdbool.h>

static RTC_Profile_01_t l_prof_01 = {120,-10,2300,-2000,1147123456,-1147987456};
static RTC_Profile_02_t l_prof_02 = {255,-100,9876,-9876,123456789,-123456789,234567890,-100,-3456789,-1,741852};

unsigned char RTC_Profile_Update(usb_msg_profile_t *profile_msg)
{
    switch (profile_msg->profile_number)
    {
    case 01:
        memcpy(&l_prof_01.p01_01, profile_msg->data, sizeof(l_prof_01.p01_01));
        memcpy(&l_prof_01.p01_02, profile_msg->data + sizeof(l_prof_01.p01_01), sizeof(l_prof_01.p01_02));
        memcpy(&l_prof_01.p01_03, profile_msg->data + sizeof(l_prof_01.p01_01) + sizeof(l_prof_01.p01_02), sizeof(l_prof_01.p01_03));
        memcpy(&l_prof_01.p01_04, profile_msg->data + sizeof(l_prof_01.p01_01) + sizeof(l_prof_01.p01_02) + sizeof(l_prof_01.p01_03), sizeof(l_prof_01.p01_04));
        memcpy(&l_prof_01.p01_05, profile_msg->data + sizeof(l_prof_01.p01_01) + sizeof(l_prof_01.p01_02) + sizeof(l_prof_01.p01_03) + sizeof(l_prof_01.p01_04), sizeof(l_prof_01.p01_05));
        memcpy(&l_prof_01.p01_06, profile_msg->data + sizeof(l_prof_01.p01_01) + sizeof(l_prof_01.p01_02) + sizeof(l_prof_01.p01_03) + sizeof(l_prof_01.p01_04) + sizeof(l_prof_01.p01_05), sizeof(l_prof_01.p01_06));
        break;

    case 02:
        memcpy(&l_prof_02.p02_01, profile_msg->data, sizeof(l_prof_02.p02_01));
        memcpy(&l_prof_02.p02_02, profile_msg->data + sizeof(l_prof_02.p02_01), sizeof(l_prof_02.p02_02));
        memcpy(&l_prof_02.p02_03, profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02), sizeof(l_prof_02.p02_03));
        memcpy(&l_prof_02.p02_04, profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03), sizeof(l_prof_02.p02_04));
        memcpy(&l_prof_02.p02_05, profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04), sizeof(l_prof_02.p02_05));
        memcpy(&l_prof_02.p02_06, profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05), sizeof(l_prof_02.p02_06));
        memcpy(&l_prof_02.p02_07, profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06), sizeof(l_prof_02.p02_07));
        memcpy(&l_prof_02.p02_08, profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06)+ sizeof(l_prof_02.p02_07), sizeof(l_prof_02.p02_08));
        memcpy(&l_prof_02.p02_09, profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06)+ sizeof(l_prof_02.p02_07)+ sizeof(l_prof_02.p02_08), sizeof(l_prof_02.p02_09));
        memcpy(&l_prof_02.p02_10, profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06)+ sizeof(l_prof_02.p02_07)+ sizeof(l_prof_02.p02_08)+ sizeof(l_prof_02.p02_09), sizeof(l_prof_02.p02_10));
        memcpy(&l_prof_02.p02_11, profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06)+ sizeof(l_prof_02.p02_07)+ sizeof(l_prof_02.p02_08)+ sizeof(l_prof_02.p02_09)+ sizeof(l_prof_02.p02_10), sizeof(l_prof_02.p02_11));
        break;
    }
    return 0;
}

unsigned char RTC_Get_Profile(char profile_number, usb_msg_profile_t *profile_msg)
{
    switch (profile_number)
    {
    case 01:
        memcpy(profile_msg->data + 0, (pU8)&l_prof_01.p01_01, sizeof(l_prof_01.p01_01));
        memcpy(profile_msg->data + sizeof(l_prof_01.p01_01), (pU8)&l_prof_01.p01_02, sizeof(l_prof_01.p01_02));
        memcpy(profile_msg->data + sizeof(l_prof_01.p01_01) + sizeof(l_prof_01.p01_02), (pU8)&l_prof_01.p01_03, sizeof(l_prof_01.p01_03));
        memcpy(profile_msg->data + sizeof(l_prof_01.p01_01) + sizeof(l_prof_01.p01_02) + sizeof(l_prof_01.p01_03), (ptr_usb_msg_u8)&l_prof_01.p01_04, sizeof(l_prof_01.p01_04));
        memcpy(profile_msg->data + sizeof(l_prof_01.p01_01) + sizeof(l_prof_01.p01_02) + sizeof(l_prof_01.p01_03) + sizeof(l_prof_01.p01_04), (ptr_usb_msg_u8)&l_prof_01.p01_05, sizeof(l_prof_01.p01_05));
        memcpy(profile_msg->data + sizeof(l_prof_01.p01_01) + sizeof(l_prof_01.p01_02) + sizeof(l_prof_01.p01_03) + sizeof(l_prof_01.p01_04) + sizeof(l_prof_01.p01_05), (ptr_usb_msg_u8)&l_prof_01.p01_06, sizeof(l_prof_01.p01_06));
        break;
    case 02:
        memcpy(profile_msg->data, (pU8)&l_prof_02.p02_01, sizeof(l_prof_02.p02_01));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01), (pU8)&l_prof_02.p02_02, sizeof(l_prof_02.p02_02));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02), (pU8)&l_prof_02.p02_03, sizeof(l_prof_02.p02_03));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03), (pU8)&l_prof_02.p02_04, sizeof(l_prof_02.p02_04));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04), (pU8)&l_prof_02.p02_05, sizeof(l_prof_02.p02_05));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05), (pU8)&l_prof_02.p02_06, sizeof(l_prof_02.p02_06));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06), (pU8)&l_prof_02.p02_07, sizeof(l_prof_02.p02_07));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06)+ sizeof(l_prof_02.p02_07), (pU8)&l_prof_02.p02_08, sizeof(l_prof_02.p02_08));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06)+ sizeof(l_prof_02.p02_07)+ sizeof(l_prof_02.p02_08), (pU8)&l_prof_02.p02_09, sizeof(l_prof_02.p02_09));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06)+ sizeof(l_prof_02.p02_07)+ sizeof(l_prof_02.p02_08)+ sizeof(l_prof_02.p02_09), (pU8)&l_prof_02.p02_10, sizeof(l_prof_02.p02_10));
        memcpy(profile_msg->data + sizeof(l_prof_02.p02_01) + sizeof(l_prof_02.p02_02) + sizeof(l_prof_02.p02_03) + sizeof(l_prof_02.p02_04) + sizeof(l_prof_02.p02_05)+ sizeof(l_prof_02.p02_06)+ sizeof(l_prof_02.p02_07)+ sizeof(l_prof_02.p02_08)+ sizeof(l_prof_02.p02_09)+ sizeof(l_prof_02.p02_10), (pU8)&l_prof_02.p02_11, sizeof(l_prof_02.p02_11));
        break;
    }
    return 0;
}