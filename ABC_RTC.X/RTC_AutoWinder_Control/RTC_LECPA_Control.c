#include "RTC_LECPA_Control.h"
#include "xc.h"
#include "usb_app.h"
#include "Compiler.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include <stdio.h>
#include <stdbool.h>
#include <outcompare.h>

LECPA_Home_Status_t LECAP_30_HomeStatus = {-1,-1,-1,-1};
LECPA_Home_Status_t LECAP_100_HomeStatus= {-1,-1,-1,-1};

void RTC_LECPA_30_ServoCmd(IO_LECPA_ServoDef_t servo_cmd)
{
    char cmd = 0;
    cmd = (servo_cmd == SERVO_ON)?1:0;
    WriteValue_ByEntityName(OUTPUT_SVON_SERVO_ON_PIN9, cmd);
}

char Is_LECPA_30_HomeProcedure_Idle()
{
    if ((LECAP_30_HomeStatus.HomeEnable == -1) || (LECAP_30_HomeStatus.HomeEnable == 0))
        return 0;
    else
        return -1;
}

char RTC_LECPA_30_Homing()
{

    
    return 0;
}