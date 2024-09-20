// ---------------------------------------------------------------------------
//  Filename: IO_Entity_Mgr.c
//  Created by: Roey Huberman
//  Date:  14/10/15
//  Modified by: Nissim Avichail
//  Date: 12/11/18
//  Orisol (c) 
// ---------------------------------------------------------------------------

#include "Definition.h"
#include "Compiler.h"
#include "IO_Entity.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include "Ons_General.h"
#include "Timers.h"
#include "xc.h"
#include <stdio.h>
#include <stdbool.h>
#include "usb_app.h"
#define SetEntityTabVal(x) { gEntityTable[x / 8] |= (1 << (x % 8)); }
#define ClrEntityTabVal(x) { gEntityTable[x / 8] &= (1 << (x % 8)) ^ 0xff; }
#define ClrEntityCompMaskVal(x) { gEntityTable_CompMask[x / 8] &= (1 << (x % 8)) ^ 0xff; }
#define C_MIN_ENTITYTABLE_POLLING_ms 50
#define C_MAX_ENTITYTABLE_POLLING_ms 250

typedef struct
{
    char entity_table_reply_mode;
    unsigned char polling_period_ms;
} EntityTab_Reply_Mode_t;


typedef struct
{
    int Entity_PORT_OR_ACT_ID;
    IO_Entity_Type_t Entity_Type;
} Entity_Def_t;

Entity_Def_t Entities_Def[IO_ENTITIES_MAX_NUM] FAR =
    {
        {IOP_X_SERVO_ON_ID, IO_OUTPUT},               // IO_X_SERVO_ON_ENTITY
        {IOP_X_STEP_ID, IO_OUTPUT},                   // IO_X_STEP_ENTITY
        {IOP_X_DIRECTION_ID, IO_OUTPUT},              // IO_X_DIRECTION_ENTITY
        {IOP_Y_SERVO_ON_ID, IO_OUTPUT},               // IO_Y_SERVO_ON_ENTITY
        {IOP_Y_STEP_ID, IO_OUTPUT},                   // IO_Y_STEP_ENTITY
        {IOP_Y_DIRECTION_ID, IO_OUTPUT},              // IO_Y_DIRECTION_ENTITY
        {IOP_Z_SERVO_ON_ID, IO_OUTPUT},               // IO_Z_SERVO_ON_ENTITY
        {IOP_Z_STEP_ID, IO_OUTPUT},                   // IO_Z_STEP_ENTITY
        {IOP_Z_DIRECTION_ID, IO_OUTPUT},              // IO_Z_DIRECTION_ENTITY
        {IOP_R_SERVO_ON_ID, IO_OUTPUT},               // IO_R_SERVO_ON_ENTITY  it is marked as R_SERVO_ON on the RTC, LATJ6
        {IOP_R_STEP_ID, IO_OUTPUT},                   // IO_R_STEP_ENTITY
        {IOP_R_DIRECTION_ID, IO_OUTPUT},              // IO_R_DIRECTION_ENTITY
        {IOP_CLAMP_PF_STEP_ID, IO_OUTPUT},            // IO_CLAMP_PF_STEP_ENTITY
        {IOP_CLAMP_PF_DIRECTION_ID, IO_OUTPUT},       // IO_CLAMP_PF_DIRECTION_ENTITY
        {IO_UNDEFINED, IO_OUTPUT},                    // IO_STEP_MOTOR_VREF_PWM_ENTITY
        {IOP_TENSION_PWM_ID, IO_OUTPUT},              // IO_TENSION_PWM_ENTITY
        {IOP_PF_UP_SENSOR_ID, IO_OUTPUT},             // IO_PRESSER_FOOT_ENTITY
        {IOP_PF_LEVEL_HIGH_ID, IO_OUTPUT},            // IO_PF_LEVEL_HIGH_ENTITY
        {IOP_CUTTER_ID, IO_OUTPUT},                   // IO_CUTTER_ENTITY
        {IOP_RESET_THREAD_BREAK_ID, IO_OUTPUT},       // IO_RESET_THREAD_BREAK_ENTITY
        {IOP_PUNCHER_PISTON_UP_ID, IO_OUTPUT},        // IO_PUNCHER_PISTON_UP_ENTITY
        {IOP_PUNCHER_PISTON_DN_ID, IO_OUTPUT},        // IO_PUNCHER_PISTON_DN_ENTITY
        {IOP_YA_PISTON_ID, IO_OUTPUT},                // IO_YA1_PISTON_ENTITY
        {IOP_LDR_T_PISTON_DN_ID, IO_OUTPUT},          // IO_LDR_T_PISTON_DN_ENTITY
        {IOP_XP_PISTON_ID, IO_OUTPUT},                // IO_XP_PISTON_ENTITY
        {IOP_ZA_PISTON_ID, IO_OUTPUT},                // IO_ZA1_PISTON_ENTITY
        {IOP_CL_PISTON_ID, IO_OUTPUT},                // IO_CL_PISTON_ENTITY
        {IOP_XA1_PISTON_ID, IO_OUTPUT},               // IO_XA1_PISTON_ENTITY
        {IOP_LDR_X_PISTON_RIGHT_ID, IO_OUTPUT},       // IO_LDR_X_PISTON_RIGHT_ENTITY
        {IOP_ZP_PISTON_ID, IO_OUTPUT},                // IO_ZP_PISTON_ENTITY
        {IOP_RIVET_MAGNET_ID, IO_OUTPUT},             // IO_RIVET_MAGNET_ENTITY
        {IOP_RIVET_STOP_ID, IO_OUTPUT},               // IO_RIVET_STOP_ENTITY
        {IOP_RIVET_PISTON_DN_ID, IO_OUTPUT},          // IO_RIVET_PISTON_DN_ENTITY
        {IOP_CLAMP_LEFT_LOCK_ID, IO_OUTPUT},          // IO_CLAMP_LEFT_LOCK_ENTITY
        {IOP_CLAMP_RIGHT_LOCK_ID, IO_OUTPUT},         // IO_CLAMP_RIGHT_LOCK_ENTITY
        {IOP_READY_LAMP_ID, IO_OUTPUT},               // IO_READY_LAMP_ENTITY
        {IOP_SAFETY_LAMP_ID, IO_OUTPUT},              // IO_SAFETY_LAMP_ENTITY
        {IOP_NEEDLE_COOLER_ID, IO_OUTPUT},            // IO_NEEDLE_COOLER_ENTITY
        {IOP_WIPER_ID, IO_OUTPUT},                    // IO_WIPER_ENTITY
        {IOP_PULLER_ID, IO_OUTPUT},                   // IO_PULLER_ENTITY
        {IOP_PINCHER_ID, IO_OUTPUT},                  // IO_PINCHER_ENTITY
        {IOP_FLIP_FLOP_ID, IO_OUTPUT},                // IO_FLIP_FLOP_ENTITY
        {IOP_R_CLAMP_LOCK_ID, IO_OUTPUT},             // IO_R_CLAMP_LOCK_ENTITY
        {IOP_MOTOR_SSR_ON_ID, IO_OUTPUT},             // IO_MOTOR_SSR_ON_ENTITY
        {IOP_AIR_VALVE_ON_ID, IO_OUTPUT},             // IO_AIR_VALVE_ON_ENTITY
        {IOP_RESET_LOADER_ALARM_ID, IO_OUTPUT},       // IO_RESET_LOADER_ALARM_ENTITY
        {IOP_LOADER_SAFETY_DISABLE_ID, IO_OUTPUT},    // IO_LOADER_SAFETY_DISABLE_ENTITY
        {IOP_LOADER_RED_LAMP_ID, IO_OUTPUT},          // IO_LOADER_RED_LAMP_ENTITY
        {IOP_LOADER_GREEN_LAMP_ID, IO_OUTPUT},        // IO_LOADER_GREEN_LAMP_ENTITY
        {IOP_RESET_SAFETY_RELAY_ID, IO_OUTPUT},       // IO_RESET_SAFETY_RELAY_ENTITY
        {IOP_OPEN_COVER_PISTON_ID, IO_OUTPUT},        // IO_OPEN_COVER_PISTON_ENTITY
        {IOP_RESET_EDM_ALARM_ID, IO_OUTPUT},          // IO_RESET_EDM_ALARM_ENTITY
        {IOP_COVER_SAFETY_DISABLE_ID, IO_OUTPUT},     // IO_COVER_SAFETY_DISABLE_ENTITY
        {IOP_BC_CX_CYLINDER_ID, IO_OUTPUT},           // IO_BC_CX_CYLINDER_ENTITY
        {IOP_BC_CY_CYLINDER_ID, IO_OUTPUT},           // IO_BC_CY_CYLINDER_ENTITY
        {IOP_BC_CA_CYLINDER_ID, IO_OUTPUT},           // IO_BC_CA_CYLINDER_ENTITY
        {IOP_BC_JAW_CYLINDER_ID, IO_OUTPUT},          // IO_BC_JAW_CYLINDER_ENTITY
        {IOP_TENSION_CURRENT_POLARITY_ID, IO_OUTPUT}, // IO_TENSION_CURRENT_POLARITY_ENTITY
        {IOP_LEVEL_SHIFT_IC_ID, IO_OUTPUT},           // IO_LEVEL_SHIFT_IC_ENTITY
        {IO_UNDEFINED, IO_OUTPUT},                    // IO_RS485_TX_ENTITY
        {IO_UNDEFINED, IO_OUTPUT},                    // IO_RS485_RX_ENTITY
        {IO_UNDEFINED, IO_OUTPUT},                    // IO_CAN2_RX_ENTITY
        {IO_UNDEFINED, IO_OUTPUT},                    // IO_CAN2_TX_ENTITY
        {IO_UNDEFINED, IO_OUTPUT},                    // IO_CAN1_RX_ENTITY
        {IO_UNDEFINED, IO_OUTPUT},                    // IO_CAN1_TX_ENTITY
        {IOP_CLAMP_PF_MOTOR_ON_ID, IO_OUTPUT},        // IO_CLAMP_PF_MOTOR_ON_ENTITY
        {IOP_RESET_RFID_ID, IO_OUTPUT},               // IO_RFID_ON_ENTITY - only for Israel RFID test - IOP_TENSION_PWM_ID
        {IO_UNDEFINED, IO_OUTPUT},                    // IO_RS232_RX_ENTITY
        {IO_UNDEFINED, IO_OUTPUT},                    // IO_RS232_TX_ENTITY
        {IOP_X_INDEX_ID, IO_INPUT},                   // IO_X_INDEX_ENTITY
        {IOP_X_HOME_SENSOR_ID, IO_INPUT},             // IO_X_HOME_SENSOR_ENTITY
        {IOP_X_ALARM_ID, IO_INPUT},                   // IO_X_ALARM_ENTITY
        {IOP_Y_INDEX_ID, IO_INPUT},                   // IO_Y_INDEX_ENTITY
        {IOP_Y_HOME_SENSOR_ID, IO_INPUT},             // IO_Y_HOME_SENSOR_ENTITY
        {IOP_Y_ALARM_ID, IO_INPUT},                   // IO_Y_ALARM_ENTITY
        {IOP_Z_INDEX_ID, IO_INPUT},                   // IO_Z_INDEX_ENTITY
        {IOP_Z_CH_A_ID, IO_INPUT},                    // IO_Z_CH_A_ENTITY
        {IOP_Z_CH_B_ID, IO_INPUT},                    // IO_Z_CH_B_ENTITY
        {IOP_Z_ALARM_ID, IO_INPUT},                   // IO_Z_ALARM_ENTITY
        {IOP_R_INDEX_ID, IO_INPUT},                   // IO_R_INDEX_ENTITY
        {IOP_R_HOME_SENSOR_ID, IO_INPUT},             // IO_R_HOME_SENSOR_ENTITY
        {IOP_R_ALARM_ID, IO_INPUT},                   // IO_R_ALARM_ENTITY
        {IOP_PF_HOME_SENSOR_ID, IO_INPUT},            // IO_PF_HOME_SENSOR_ENTITY
        {IO_UNDEFINED, IO_INPUT},                     // IO_Z_QEI_CNT_CMP_ENTITY
        {IOP_PF_UP_SENSOR_ID, IO_INPUT},              // IO_PF_UP_SENSOR_ENTITY
        {IOP_CUTTER_OUT_SENSOR_ID, IO_INPUT},         // IO_CUTTER_OUT_SENSOR_ENTITY
        {IOP_THREAD_BREAK_INPUT_ID, IO_INPUT},        // IO_THREAD_BREAK_INPUT_ENTITY
        {IOP_PUNCHER_UP_SENSOR_ID, IO_INPUT},         // IO_PUNCHER_UP_SENSOR_ENTITY
        {IOP_PUNCHER_DOWN_SENSOR_ID, IO_INPUT},       // IO_PUNCHER_DN_SENSOR_ENTITY
        {IOP_YN_SENSOR_ID, IO_INPUT},                 // IO_YN_SENSOR_ENTITY
        {IOP_YF_SENSOR_ID, IO_INPUT},                 // IO_YF_SENSOR_ENTITY
        {IOP_XL_SENSOR_ID, IO_INPUT},                 // IO_XL_SENSOR_ENTITY
        {IOP_ZL_SENSOR_ID, IO_INPUT},                 // IO_ZL_SENSOR_ENTITY
        {IOP_LDR_ZU_SENSOR_ID, IO_INPUT},             // IO_ZU_SENSOR_ENTITY
        {IOP_LDR_ZD_SENSOR_ID, IO_INPUT},             // IO_ZD_SENSOR_ENTITY
        {IOP_PALLET_SENSOR_ID, IO_INPUT},             // IO_PS_SENSOR_ENTITY
        {IOP_LDR_XN_SENSOR_ID, IO_INPUT},             // IO_XN_SENSOR_ENTITY
        {IOP_LDR_XF_SENSOR_ID, IO_INPUT},             // IO_XF_SENSOR_ENTITY
        {IOP_LOADER_SAFETY_KEY_ID, IO_INPUT},         // IO_LDR_SAFETY_KEY_ENTITY
        {IOP_RIVET_IN_POSITION_ID, IO_INPUT},         // IO_RIVET_IN_POSITION_ENTITY
        {IOP_RIVET_ALARM_ID, IO_INPUT},               // IO_RIVET_ALARM_ENTITY
        {IOP_RIVET_READY_ID, IO_INPUT},               // IO_RIVET_READY_ENTITY
        {IOP_LEFT_LOCK_PEDAL_ID, IO_INPUT},           // IO_LEFT_LOCK_PEDAL_ENTITY
        {IOP_RIGHT_LOCK_PEDAL_ID, IO_INPUT},          // IO_RIGHT_LOCK_PEDAL_ENTITY
        {IOP_SEW_PEDAL_ID, IO_INPUT},                 // IO_SEW_PEDAL_ENTITY
        {IOP_READY_BUTTON_ID, IO_INPUT},              // IO_READY_BUTTON_ENTITY
        {IOP_STOP_SEW_BUTTON_ID, IO_INPUT},           // IO_STOP_SEW_BUTTON_ENTITY
        {IOP_SAFETY_BUTTON_ID, IO_INPUT},             // IO_SAFETY_BUTTON_ENTITY
        {IOP_AIR_FAN_SENSOR_ID, IO_INPUT},            // IO_AIR_FAN_SENSOR_ENTITY
        {IOP_AUTO_SEW_SENSOR_ID, IO_INPUT},           // IO_AUTO_SEW_SENSOR_ENTITY
        {IOP_LOCATING_SENSOR_ID, IO_INPUT},           // IO_LOCATING_SENSOR_ENTITY
        {IOP_AIR_SENSOR_ID, IO_INPUT},                // IO_AIR_SENSOR_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_LDR_YA_PISTON_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_LDR_CL_PISTON_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_LDR_XA_PISTON_LEFT_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_LDR_ZP_PISTON_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_LDR_XP_PISTON_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_LDR_ZA_PISTON_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_PRESSER_FOOT_ACT_ENTITY -bad wiring
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_CUTTER_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_PUNCHER_PISTON_UP_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_RIVET_PISTON_DN_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_MOTOR_SSR_ON_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_CLAMP_LEFT_LOCK_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_CLAMP_RIGHT_LOCK_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_R_CLAMP_LOCK_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_PINCHER_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_PULLER_ACT_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_WIPER_ACT_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_READY_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_AIR_PRESSURE_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_PALLET_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_STOP_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_SEW_PEDAL_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_LOCK_PEDAL_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_LOCK_PEDAL_2_SENSOR_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_CLAMP1_3S_ACT_ENTITY
        {IOP_LOADER_SAFETY_ALARM_ID, IO_INPUT},       // IO_LOADER_SAFETY_ALARM_ENTITY
        {IOP_LOADER_SAFETY_KEY_ID, IO_INPUT},         // IO_LOADER_SAFETY_KEY_ENTITY
        {IOP_COVER_OPEN_ALARM_ID, IO_INPUT},          // IO_COVER_OPEN_ALARM_ENTITY
        {IOP_UPPER_COVER_CLOSED_ID, IO_INPUT},        // IO_UPPER_COVER_CLOSED_ENTITY
        {IOP_UPPER_COVER_OPENED_ID, IO_INPUT},        // IO_UPPER_COVER_OPENED_ENTITY
        {IOP_LOWER_COVER_CLOSED_ID, IO_INPUT},        // IO_LOWER_COVER_CLOSED_ENTITY
        {IOP_SERVOS_EDM_ID, IO_INPUT},                // IO_SERVOS_EDM_ENTITY
        {IOP_COVER_SAFETY_KEY_ID, IO_INPUT},          // IO_COVER_SAFETY_KEY_ENTITY
        {IOP_PALLET_COVER_OPEN_SENSOR_ID, IO_INPUT},  // IO_PALLET_COVER_OPEN_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_LOADER_SAFETY_KEY_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_COVER_SAFETY_KEY_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_PALLET_COVER_OPEN_SENSOR_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_OPEN_COVER_PISTON_ACT_ENTITY
        {IO_UNDEFINED, IO_INPUT},                     // IO_UNDEFINED_ENTITY
        {IOP_XA2_PISTON_ID, IO_OUTPUT},               // IO_XA2_PISTON_ENTITY
        {IOP_YA2_PISTON_ID, IO_OUTPUT},               // IO_YA2_PISTON_ENTITY
        {IOP_ZA2_PISTON_ID, IO_OUTPUT},               // IO_ZA2_PISTON_ENTITY
        {IOP_BC_XC_SENSOR_ID, IO_INPUT},              // IO_BC_XC_SENSOR_ENTITY
        {IOP_BC_YI_SENSOR_ID, IO_INPUT},              // IO_BC_YI_SENSOR_ENTITY
        {IOP_BC_XH_SENSOR_ID, IO_INPUT},              // IO_BC_XH_SENSOR_ENTITY
        {IOP_BC_YO_SENSOR_ID, IO_INPUT},              // IO_BC_YO_SENSOR_ENTITY
        {IOP_BC_BD_SENSOR_ID, IO_INPUT},              // IO_BC_BD_SENSOR_ENTITY
        {IOP_BC_CF_SENSOR_ID, IO_INPUT},              // IO_BC_CF_SENSOR_ENTITY
        {IOP_BC_CB_SENSOR_ID, IO_INPUT},              // IO_BC_CB_SENSOR_ENTITY
        {IOP_BOBBIN_COUNTER_ID, IO_INPUT},            // IO_BOBBIN_COUNTER_ENTITY
        {IOP_WINDER_COUNTER_ID, IO_INPUT},            // IO_WINDER_COUNTER_ENTITY
        {IOP_THREAD_CONSUMPTION_ID, IO_INPUT},        // IO_THREAD_CONSUMPTION_ENTITY
        {IOP_BOOST_BUTTON_ID, IO_INPUT},              // IO_BOOST_BUTTON_ENTITY
        {IOP_PALLET_SENSOR_ID, IO_INPUT},             // IO_PALLET_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_X_INDEX_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_Y_INDEX_SENSOR_ENTITY
        {IO_UNDEFINED, IO_SENSOR},                    // SEN_Z_INDEX_SENSOR_ENTITY
        {IOP_UT_WELDER_DOWN_SENSOR_ID, IO_INPUT},     // IO_UT_WELDER_DOWN_SENSOR_ENTITY
        {IOP_UT_WELDER_UP_SENSOR_ID, IO_INPUT},       // IO_UT_WELDER_UP_SENSOR_ENTITY
        {IOP_UT_WELDER_DEVICE_ON_ID, IO_OUTPUT},      // IO_UT_WELDER_DEVICE_ON_ENTITY
        {IOP_UT_WELDER_PISTON_ID, IO_OUTPUT},         // IO_UT_WELDER_PISTON_ENTITY
        {IO_UNDEFINED, IO_ACTIVATOR},                 // ACT_UT_WELDER_ACT_ENTITY
        {IOP_TABLE_DOWN_SENSOR_ID, IO_INPUT},         // IO_TABLE_DOWN_SENSOR_ENTITY
};

int IO_Entity_Mgr_Get_Entity(int Entity_Port_Or_Act_ID)
{
    int ret_val = IO_ERROR_TYPE;
    if (Entities_Def[Entity_Port_Or_Act_ID].Entity_PORT_OR_ACT_ID == IO_UNDEFINED)
    {
        return ret_val;
    }

    switch (Entities_Def[Entity_Port_Or_Act_ID].Entity_Type)
    {
    case IO_OUTPUT:
        ret_val = (IO_Get(Entities_Def[Entity_Port_Or_Act_ID].Entity_PORT_OR_ACT_ID) != 0);
        break;

    case IO_INPUT:
        ret_val = (IO_Get(Entities_Def[Entity_Port_Or_Act_ID].Entity_PORT_OR_ACT_ID) != 0);
        break;

    case IO_ACTIVATOR:
        Nop();
        // ABC without define Activator
        // ret_val = Get_Activator_State( Entities_Def[ Entity_Port_Or_Act_ID ].Entity_PORT_OR_ACT_ID );
        break;

    case IO_SENSOR:
        Nop();
        // ABC without define Activator
        // ret_val = SENS_Get_Sensor_State( Entities_Def[ Entity_Port_Or_Act_ID ].Entity_PORT_OR_ACT_ID );
        break;
    }
    return ret_val;
}

int IO_Entity_Mgr_Set_Entity(int Entity_Port_Or_Act_ID, int Value)
{
    int ret_val = IO_ERROR_TYPE;
    if (Entities_Def[Entity_Port_Or_Act_ID].Entity_PORT_OR_ACT_ID == IO_UNDEFINED)
    {
        return ret_val;
    }

    switch (Entities_Def[Entity_Port_Or_Act_ID].Entity_Type)
    {
    case IO_OUTPUT:
        if ((Value == 0) || (Value == 1)) // ignore incorrect values
        {
            ret_val = IO_Set(Entities_Def[Entity_Port_Or_Act_ID].Entity_PORT_OR_ACT_ID, Value);
        }
        break;

    case IO_INPUT:
    case IO_ACTIVATOR:
    case IO_SENSOR:
        Nop();
        break;
    }
    return ret_val;
}

#define ENTITY_TAB_SIZE 22
static unsigned char gEntityTable[ENTITY_TAB_SIZE];
static unsigned char gEntityTable_Backup[ENTITY_TAB_SIZE];
static unsigned char gEntityTable_CompMask[ENTITY_TAB_SIZE];
static EntityTab_Reply_Mode_t gEntityTabReplyMode ={0,0};

// static unsigned char gGetEntityTable_Mode = SubFunc_table_get_off;
// static unsigned char gGetEntityTable_PollingPeriod = SubFunc_table_get_off;

char Get_EntityTable_Idx_Value(unsigned char entity_num)
{
    unsigned char tmp = gEntityTable[entity_num / 8] & (1 << (entity_num % 8));
    char bit_value = (tmp > 0) ? 1 : 0;
    return bit_value;
}

char Set_EntityTable_Idx_Value(unsigned char entity_num)
{
    gEntityTable[entity_num / 8] |= (1 << (entity_num % 8));
    return 0;
}

char Clr_EntityTable_Idx_Value(unsigned char entity_num)
{
    gEntityTable[entity_num / 8] &= (1 << (entity_num % 8)) ^ 0xff;
    return 0;
}

void Refresh_EntityTable_by_func()
{
    unsigned char i = 0;
    char entity_value;
    for (i = 0; i < IO_TABLE_MAX; i++)
    {
        entity_value = IO_Entity_Mgr_Get_Entity(i);
        (entity_value == 0) ? Clr_EntityTable_Idx_Value(i) : Set_EntityTable_Idx_Value(i);
    }
}

void Refresh_EntityTable_by_macro01()
{
    unsigned char i = 0;
    char entity_value;
    memset(gEntityTable, 0xff, ENTITY_TAB_SIZE);

    for (i = 0; i < IO_TABLE_MAX; i++)
    {
        entity_value = GetIO_ByEntityName(i);
        if (entity_value == 0)
            ClrEntityTabVal(i) else SetEntityTabVal(i)
    }
}

void Refresh_EntityTable_by_macro02()
{
    unsigned char i = 0;
    char entity_value;
    memset(gEntityTable, 0xff, ENTITY_TAB_SIZE);
    for (i = 0; i < IO_TABLE_MAX; i++)
    {
        entity_value = GetIO_ByEntityName(i);
        if (entity_value == 0)
            ClrEntityTabVal(i);
    }
}

void Refresh_EntityTable_by_macro03()
{
    memset(gEntityTable, 0xff, ENTITY_TAB_SIZE);
    if (GetIO_ByEntityName(0) == 0)
        ClrEntityTabVal(0);
    if (GetIO_ByEntityName(1) == 0)
        ClrEntityTabVal(1);
    if (GetIO_ByEntityName(2) == 0)
        ClrEntityTabVal(2);
    if (GetIO_ByEntityName(3) == 0)
        ClrEntityTabVal(3);
    if (GetIO_ByEntityName(4) == 0)
        ClrEntityTabVal(4);
    if (GetIO_ByEntityName(5) == 0)
        ClrEntityTabVal(5)
    if (GetIO_ByEntityName(6) == 0)
        ClrEntityTabVal(6);
    if (GetIO_ByEntityName(7) == 0)
        ClrEntityTabVal(7);
    if (GetIO_ByEntityName(8) == 0)
        ClrEntityTabVal(8);
    if (GetIO_ByEntityName(9) == 0)
        ClrEntityTabVal(9);
    if (GetIO_ByEntityName(10) == 0)
        ClrEntityTabVal(10);
    if (GetIO_ByEntityName(11) == 0)
        ClrEntityTabVal(11);
    if (GetIO_ByEntityName(12) == 0)
        ClrEntityTabVal(12);
    if (GetIO_ByEntityName(13) == 0)
        ClrEntityTabVal(13);
    if (GetIO_ByEntityName(14) == 0)
        ClrEntityTabVal(14);
    if (GetIO_ByEntityName(15) == 0)
        ClrEntityTabVal(15);
    if (GetIO_ByEntityName(16) == 0)
        ClrEntityTabVal(16);
    if (GetIO_ByEntityName(17) == 0)
        ClrEntityTabVal(17);
    if (GetIO_ByEntityName(18) == 0)
        ClrEntityTabVal(18);
    if (GetIO_ByEntityName(19) == 0)
        ClrEntityTabVal(19);
    if (GetIO_ByEntityName(20) == 0)
        ClrEntityTabVal(20);
    if (GetIO_ByEntityName(21) == 0)
        ClrEntityTabVal(21);
    if (GetIO_ByEntityName(22) == 0)
        ClrEntityTabVal(22);
    if (GetIO_ByEntityName(23) == 0)
        ClrEntityTabVal(23);
    if (GetIO_ByEntityName(24) == 0)
        ClrEntityTabVal(24);
    if (GetIO_ByEntityName(25) == 0)
        ClrEntityTabVal(25);
    if (GetIO_ByEntityName(26) == 0)
        ClrEntityTabVal(26);
    if (GetIO_ByEntityName(27) == 0)
        ClrEntityTabVal(27);
    if (GetIO_ByEntityName(28) == 0)
        ClrEntityTabVal(28);
    if (GetIO_ByEntityName(29) == 0)
        ClrEntityTabVal(29);
    if (GetIO_ByEntityName(30) == 0)
        ClrEntityTabVal(30);
    if (GetIO_ByEntityName(31) == 0)
        ClrEntityTabVal(31);
    if (GetIO_ByEntityName(32) == 0)
        ClrEntityTabVal(32);
    if (GetIO_ByEntityName(33) == 0)
        ClrEntityTabVal(33);
    if (GetIO_ByEntityName(34) == 0)
        ClrEntityTabVal(34);
    if (GetIO_ByEntityName(35) == 0)
        ClrEntityTabVal(35);
    if (GetIO_ByEntityName(36) == 0)
        ClrEntityTabVal(36);
    if (GetIO_ByEntityName(37) == 0)
        ClrEntityTabVal(37);
    if (GetIO_ByEntityName(38) == 0)
        ClrEntityTabVal(38);
    if (GetIO_ByEntityName(39) == 0)
        ClrEntityTabVal(39);
    if (GetIO_ByEntityName(40) == 0)
        ClrEntityTabVal(40);
    if (GetIO_ByEntityName(41) == 0)
        ClrEntityTabVal(41);
    if (GetIO_ByEntityName(42) == 0)
        ClrEntityTabVal(42);
    if (GetIO_ByEntityName(43) == 0)
        ClrEntityTabVal(43);
    if (GetIO_ByEntityName(44) == 0)
        ClrEntityTabVal(44);
    if (GetIO_ByEntityName(45) == 0)
        ClrEntityTabVal(45);
    if (GetIO_ByEntityName(46) == 0)
        ClrEntityTabVal(46);
    if (GetIO_ByEntityName(47) == 0)
        ClrEntityTabVal(47);
    if (GetIO_ByEntityName(48) == 0)
        ClrEntityTabVal(48);
    if (GetIO_ByEntityName(49) == 0)
        ClrEntityTabVal(49);
    if (GetIO_ByEntityName(50) == 0)
        ClrEntityTabVal(50);
    if (GetIO_ByEntityName(51) == 0)
        ClrEntityTabVal(51);
    if (GetIO_ByEntityName(52) == 0)
        ClrEntityTabVal(52);
    if (GetIO_ByEntityName(53) == 0)
        ClrEntityTabVal(53);
    if (GetIO_ByEntityName(54) == 0)
        ClrEntityTabVal(54);
    if (GetIO_ByEntityName(55) == 0)
        ClrEntityTabVal(55);
    if (GetIO_ByEntityName(56) == 0)
        ClrEntityTabVal(56);
    if (GetIO_ByEntityName(57) == 0)
        ClrEntityTabVal(57);
    if (GetIO_ByEntityName(58) == 0)
        ClrEntityTabVal(58);
    if (GetIO_ByEntityName(59) == 0)
        ClrEntityTabVal(59);
    if (GetIO_ByEntityName(60) == 0)
        ClrEntityTabVal(60);
    if (GetIO_ByEntityName(61) == 0)
        ClrEntityTabVal(61);
    if (GetIO_ByEntityName(62) == 0)
        ClrEntityTabVal(62);
    if (GetIO_ByEntityName(63) == 0)
        ClrEntityTabVal(63);
    if (GetIO_ByEntityName(64) == 0)
        ClrEntityTabVal(64);
    if (GetIO_ByEntityName(65) == 0)
        ClrEntityTabVal(65);
    if (GetIO_ByEntityName(66) == 0)
        ClrEntityTabVal(66);
    if (GetIO_ByEntityName(67) == 0)
        ClrEntityTabVal(67);
    if (GetIO_ByEntityName(68) == 0)
        ClrEntityTabVal(68);
    if (GetIO_ByEntityName(69) == 0)
        ClrEntityTabVal(69);
    if (GetIO_ByEntityName(70) == 0)
        ClrEntityTabVal(70);
    if (GetIO_ByEntityName(71) == 0)
        ClrEntityTabVal(71);
    if (GetIO_ByEntityName(72) == 0)
        ClrEntityTabVal(72);
    if (GetIO_ByEntityName(73) == 0)
        ClrEntityTabVal(73);
    if (GetIO_ByEntityName(74) == 0)
        ClrEntityTabVal(74);
    if (GetIO_ByEntityName(75) == 0)
        ClrEntityTabVal(75);
    if (GetIO_ByEntityName(76) == 0)
        ClrEntityTabVal(76);
    if (GetIO_ByEntityName(77) == 0)
        ClrEntityTabVal(77);
    if (GetIO_ByEntityName(78) == 0)
        ClrEntityTabVal(78);
    if (GetIO_ByEntityName(79) == 0)
        ClrEntityTabVal(79);
    if (GetIO_ByEntityName(80) == 0)
        ClrEntityTabVal(80);
    if (GetIO_ByEntityName(81) == 0)
        ClrEntityTabVal(81);
    if (GetIO_ByEntityName(82) == 0)
        ClrEntityTabVal(82);
    if (GetIO_ByEntityName(83) == 0)
        ClrEntityTabVal(83);
    if (GetIO_ByEntityName(84) == 0)
        ClrEntityTabVal(84);
    if (GetIO_ByEntityName(85) == 0)
        ClrEntityTabVal(85);
    if (GetIO_ByEntityName(86) == 0)
        ClrEntityTabVal(86);
    if (GetIO_ByEntityName(87) == 0)
        ClrEntityTabVal(87);
    if (GetIO_ByEntityName(88) == 0)
        ClrEntityTabVal(88);
    if (GetIO_ByEntityName(89) == 0)
        ClrEntityTabVal(89);
    if (GetIO_ByEntityName(90) == 0)
        ClrEntityTabVal(90);
    if (GetIO_ByEntityName(91) == 0)
        ClrEntityTabVal(91);
    if (GetIO_ByEntityName(92) == 0)
        ClrEntityTabVal(92);
    if (GetIO_ByEntityName(93) == 0)
        ClrEntityTabVal(93);
    if (GetIO_ByEntityName(94) == 0)
        ClrEntityTabVal(94);
    if (GetIO_ByEntityName(95) == 0)
        ClrEntityTabVal(95);
    if (GetIO_ByEntityName(96) == 0)
        ClrEntityTabVal(96);
    if (GetIO_ByEntityName(97) == 0)
        ClrEntityTabVal(97);
    if (GetIO_ByEntityName(98) == 0)
        ClrEntityTabVal(98);
    if (GetIO_ByEntityName(99) == 0)
        ClrEntityTabVal(99);
    if (GetIO_ByEntityName(100) == 0)
        ClrEntityTabVal(100);
    if (GetIO_ByEntityName(101) == 0)
        ClrEntityTabVal(101);
    if (GetIO_ByEntityName(102) == 0)
        ClrEntityTabVal(102);
    if (GetIO_ByEntityName(103) == 0)
        ClrEntityTabVal(103);
    if (GetIO_ByEntityName(104) == 0)
        ClrEntityTabVal(104);
    if (GetIO_ByEntityName(105) == 0)
        ClrEntityTabVal(105);
    if (GetIO_ByEntityName(106) == 0)
        ClrEntityTabVal(106);
    if (GetIO_ByEntityName(107) == 0)
        ClrEntityTabVal(107);
    if (GetIO_ByEntityName(108) == 0)
        ClrEntityTabVal(108);
    if (GetIO_ByEntityName(109) == 0)
        ClrEntityTabVal(109);
    if (GetIO_ByEntityName(110) == 0)
        ClrEntityTabVal(110);
    if (GetIO_ByEntityName(111) == 0)
        ClrEntityTabVal(111);
    if (GetIO_ByEntityName(112) == 0)
        ClrEntityTabVal(112);
    if (GetIO_ByEntityName(113) == 0)
        ClrEntityTabVal(113);
    if (GetIO_ByEntityName(114) == 0)
        ClrEntityTabVal(114);
    if (GetIO_ByEntityName(115) == 0)
        ClrEntityTabVal(115);
    if (GetIO_ByEntityName(116) == 0)
        ClrEntityTabVal(116);
    if (GetIO_ByEntityName(117) == 0)
        ClrEntityTabVal(117);
    if (GetIO_ByEntityName(118) == 0)
        ClrEntityTabVal(118);
    if (GetIO_ByEntityName(119) == 0)
        ClrEntityTabVal(119);
    if (GetIO_ByEntityName(120) == 0)
        ClrEntityTabVal(120);
    if (GetIO_ByEntityName(121) == 0)
        ClrEntityTabVal(121);
    if (GetIO_ByEntityName(122) == 0)
        ClrEntityTabVal(122);
    if (GetIO_ByEntityName(123) == 0)
        ClrEntityTabVal(123);
    if (GetIO_ByEntityName(124) == 0)
        ClrEntityTabVal(124);
    if (GetIO_ByEntityName(125) == 0)
        ClrEntityTabVal(125);
    if (GetIO_ByEntityName(126) == 0)
        ClrEntityTabVal(126);
    if (GetIO_ByEntityName(127) == 0)
        ClrEntityTabVal(127);
    if (GetIO_ByEntityName(128) == 0)
        ClrEntityTabVal(128);
    if (GetIO_ByEntityName(129) == 0)
        ClrEntityTabVal(129);
    if (GetIO_ByEntityName(130) == 0)
        ClrEntityTabVal(130);
    if (GetIO_ByEntityName(131) == 0)
        ClrEntityTabVal(131);
    if (GetIO_ByEntityName(132) == 0)
        ClrEntityTabVal(132);
    if (GetIO_ByEntityName(133) == 0)
        ClrEntityTabVal(133);
    if (GetIO_ByEntityName(134) == 0)
        ClrEntityTabVal(134);
    if (GetIO_ByEntityName(135) == 0)
        ClrEntityTabVal(135);
    if (GetIO_ByEntityName(136) == 0)
        ClrEntityTabVal(136);
    if (GetIO_ByEntityName(137) == 0)
        ClrEntityTabVal(137);
    if (GetIO_ByEntityName(138) == 0)
        ClrEntityTabVal(138);
    if (GetIO_ByEntityName(139) == 0)
        ClrEntityTabVal(139);
    if (GetIO_ByEntityName(140) == 0)
        ClrEntityTabVal(140);
    if (GetIO_ByEntityName(141) == 0)
        ClrEntityTabVal(141);
    if (GetIO_ByEntityName(142) == 0)
        ClrEntityTabVal(142);
    if (GetIO_ByEntityName(143) == 0)
        ClrEntityTabVal(143);
    if (GetIO_ByEntityName(144) == 0)
        ClrEntityTabVal(144);
    if (GetIO_ByEntityName(145) == 0)
        ClrEntityTabVal(145);
    if (GetIO_ByEntityName(146) == 0)
        ClrEntityTabVal(146);
    if (GetIO_ByEntityName(147) == 0)
        ClrEntityTabVal(147);
    if (GetIO_ByEntityName(148) == 0)
        ClrEntityTabVal(148);
    if (GetIO_ByEntityName(149) == 0)
        ClrEntityTabVal(149);
    if (GetIO_ByEntityName(150) == 0)
        ClrEntityTabVal(150);
    if (GetIO_ByEntityName(151) == 0)
        ClrEntityTabVal(151);
    if (GetIO_ByEntityName(152) == 0)
        ClrEntityTabVal(152);
    if (GetIO_ByEntityName(153) == 0)
        ClrEntityTabVal(153);
    if (GetIO_ByEntityName(154) == 0)
        ClrEntityTabVal(154);
    if (GetIO_ByEntityName(155) == 0)
        ClrEntityTabVal(155);
    if (GetIO_ByEntityName(156) == 0)
        ClrEntityTabVal(156);
    if (GetIO_ByEntityName(157) == 0)
        ClrEntityTabVal(157);
    if (GetIO_ByEntityName(158) == 0)
        ClrEntityTabVal(158);
    if (GetIO_ByEntityName(159) == 0)
        ClrEntityTabVal(159);
    if (GetIO_ByEntityName(160) == 0)
        ClrEntityTabVal(160);
    if (GetIO_ByEntityName(161) == 0)
        ClrEntityTabVal(161);
    if (GetIO_ByEntityName(162) == 0)
        ClrEntityTabVal(162);
    if (GetIO_ByEntityName(163) == 0)
        ClrEntityTabVal(163);
    if (GetIO_ByEntityName(164) == 0)
        ClrEntityTabVal(164);
    if (GetIO_ByEntityName(165) == 0)
        ClrEntityTabVal(165);
    if (GetIO_ByEntityName(166) == 0)
        ClrEntityTabVal(166);
    if (GetIO_ByEntityName(167) == 0)
        ClrEntityTabVal(167);
    if (GetIO_ByEntityName(168) == 0)
        ClrEntityTabVal(168);
    if (GetIO_ByEntityName(169) == 0)
        ClrEntityTabVal(169);
    if (GetIO_ByEntityName(170) == 0)
        ClrEntityTabVal(170);
    if (GetIO_ByEntityName(171) == 0)
        ClrEntityTabVal(171);
    if (GetIO_ByEntityName(172) == 0)
        ClrEntityTabVal(172);
    if (GetIO_ByEntityName(173) == 0)
        ClrEntityTabVal(173);
    if (GetIO_ByEntityName(174) == 0)
        ClrEntityTabVal(174);
}

char EntityTable_Compare()
{
   char res = 0;
   unsigned char i;
   unsigned char tmp, cmp_res;
   Refresh_EntityTable_by_macro03();
   for (i = 0; i < ENTITY_TAB_SIZE; i++)
   {
       tmp = gEntityTable[i] ^ gEntityTable_Backup[i];
       cmp_res = tmp & gEntityTable_CompMask[i];
       if (cmp_res != 0)
       {
           res = 1;
           break;
       }
   }
   asm("NOP");
   asm("NOP");

   memcpy(gEntityTable_Backup, gEntityTable, sizeof(gEntityTable));
   asm("NOP");
   asm("NOP");
   return res;
}

// e.g. IO_PUNCHER_PISTON_UP_ENTITY = 20
//      IO_LOADER_GREEN_LAMP_ENTITY = 48
//      IO_UT_WELDER_DEVICE_ON_ENTITY = 171
// three entity name mapping to LATH15,
// so states of bit 20, bit48, bit171 are the same.
//	IOP_PUNCHER_PISTON_UP_ID        = 52,
//	IOP_LOADER_GREEN_LAMP_ID        = 52,
//  IOP_UT_WELDER_DEVICE_ON_ID      = 52,

// escape: 
// (1) 380usec -> Refresh_EntityTable_by_func
// (2) 277usec -> Refresh_EntityTable_by_macro01
// (3) 232usec -> Refresh_EntityTable_by_macro02
// (3) 190usec -> Refresh_EntityTable_by_macro03
void Get_EntityTable(unsigned char *data, unsigned char *data_size)
{
   Refresh_EntityTable_by_macro03();
   memcpy(data, gEntityTable, sizeof(gEntityTable));
   *data_size = sizeof(gEntityTable);
}

char Is_EntityTable_Changed(unsigned char *data, unsigned char *data_size)
{
   char res = 0;
   if (EntityTable_Compare() == 1)
   {
       memcpy(data, gEntityTable, sizeof(gEntityTable));
       *data_size = sizeof(gEntityTable);
       res = 1;
   }
   return res;
}

void Refresh_EntityTableMask()
{
    memset(gEntityTable_CompMask, 0xff, ENTITY_TAB_SIZE);
    unsigned char i, mask_value;
    for (i = 0; i < IO_TABLE_MAX; i++)
    {
        mask_value = GetMask_ByEntityName(i);
        if (mask_value == 0)
            ClrEntityCompMaskVal(i);
    }
}

void Set_GetEntityTableMode(enum EntityTable_SubFunc mode)
{
    if (mode < SubFunc_entitytable_max)
    {
        gEntityTabReplyMode.entity_table_reply_mode = (char)mode;
    }

    if (gEntityTabReplyMode.entity_table_reply_mode == SubFunc_table_get_changed)
    {
        Set_GetEntityReplyPeriod(C_MIN_ENTITYTABLE_POLLING_ms);
    }
}

char Get_GetEntityTableMode()
{
   return gEntityTabReplyMode.entity_table_reply_mode;
}

void Set_GetEntityReplyPeriod(unsigned char period_ms)
{
    unsigned char tmp;

    if (period_ms < C_MIN_ENTITYTABLE_POLLING_ms)
        tmp = C_MIN_ENTITYTABLE_POLLING_ms;
    else if (period_ms > C_MAX_ENTITYTABLE_POLLING_ms)
        tmp = C_MAX_ENTITYTABLE_POLLING_ms;
    else
        tmp = period_ms;

    gEntityTabReplyMode.polling_period_ms = tmp;
}

unsigned char Get_GetEntityTableReplyPeriod()
{
   return gEntityTabReplyMode.polling_period_ms;
}
