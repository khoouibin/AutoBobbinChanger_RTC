#include "RTC_Profile.h"
#include "xc.h"
#include "usb_app.h"
#include <stdio.h>
#include <stdbool.h>

static RTC_Profile_01_t local_profile_01 = {120,-10,2300,-2000,1147123456,-1147987456};
unsigned char RTC_Profile_Update(usb_msg_profile_t *profile_msg)
{
    switch (profile_msg->profile_number)
    {
    case 01:
        memcpy(&local_profile_01.profile_01_01, profile_msg->data + 0, sizeof(local_profile_01.profile_01_01));
        memcpy(&local_profile_01.profile_01_02, profile_msg->data + sizeof(local_profile_01.profile_01_01), sizeof(local_profile_01.profile_01_02));
        memcpy(&local_profile_01.profile_01_03, profile_msg->data + sizeof(local_profile_01.profile_01_01) + sizeof(local_profile_01.profile_01_02), sizeof(local_profile_01.profile_01_03));
        memcpy(&local_profile_01.profile_01_04, profile_msg->data + sizeof(local_profile_01.profile_01_01) + sizeof(local_profile_01.profile_01_02) + sizeof(local_profile_01.profile_01_03), sizeof(local_profile_01.profile_01_04));
        memcpy(&local_profile_01.profile_01_05, profile_msg->data + sizeof(local_profile_01.profile_01_01) + sizeof(local_profile_01.profile_01_02) + sizeof(local_profile_01.profile_01_03) + sizeof(local_profile_01.profile_01_04), sizeof(local_profile_01.profile_01_05));
        memcpy(&local_profile_01.profile_01_06, profile_msg->data + sizeof(local_profile_01.profile_01_01) + sizeof(local_profile_01.profile_01_02) + sizeof(local_profile_01.profile_01_03) + sizeof(local_profile_01.profile_01_04) + sizeof(local_profile_01.profile_01_05), sizeof(local_profile_01.profile_01_06));
        break;
    }
    return 0;
}

unsigned char RTC_Get_Profile_1(RTC_Profile_01_t *profile_01)
{
    memcpy(profile_01, (RTC_Profile_01_t *)&local_profile_01, 60);
    return 0;
}

unsigned char RTC_Get_Profile(char profile_number, usb_msg_profile_t *profile_msg)
{
    switch (profile_number)
    {
    case 01:
        // memcpy(profile_msg->data, (RTC_Profile_01_t *)&local_profile_01, sizeof(RTC_Profile_01_t));
        memcpy(profile_msg->data + 0, (ptr_usb_msg_u8)&local_profile_01.profile_01_01, sizeof(local_profile_01.profile_01_01));
        memcpy(profile_msg->data + sizeof(local_profile_01.profile_01_01), (ptr_usb_msg_u8)&local_profile_01.profile_01_02, sizeof(local_profile_01.profile_01_02));
        memcpy(profile_msg->data + sizeof(local_profile_01.profile_01_01) + sizeof(local_profile_01.profile_01_02), (ptr_usb_msg_u8)&local_profile_01.profile_01_03, sizeof(local_profile_01.profile_01_03));
        memcpy(profile_msg->data + sizeof(local_profile_01.profile_01_01) + sizeof(local_profile_01.profile_01_02) + sizeof(local_profile_01.profile_01_03), (ptr_usb_msg_u8)&local_profile_01.profile_01_04, sizeof(local_profile_01.profile_01_04));
        memcpy(profile_msg->data + sizeof(local_profile_01.profile_01_01) + sizeof(local_profile_01.profile_01_02) + sizeof(local_profile_01.profile_01_03) + sizeof(local_profile_01.profile_01_04), (ptr_usb_msg_u8)&local_profile_01.profile_01_05, sizeof(local_profile_01.profile_01_05));
        memcpy(profile_msg->data + sizeof(local_profile_01.profile_01_01) + sizeof(local_profile_01.profile_01_02) + sizeof(local_profile_01.profile_01_03) + sizeof(local_profile_01.profile_01_04) + sizeof(local_profile_01.profile_01_05), (ptr_usb_msg_u8)&local_profile_01.profile_01_06, sizeof(local_profile_01.profile_01_06));
        break;
    }
    return 0;
}