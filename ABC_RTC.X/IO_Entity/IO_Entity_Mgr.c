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
        Nop();
        break;

    case IO_ACTIVATOR:
        if (Value != -1)
        {
            Nop();
            // ret_val = Set_Activator( Entities_Def[ Entity_Port_Or_Act_ID ].Entity_PORT_OR_ACT_ID , Value , SysTimer_GetTimeInMiliSeconds() );
        }
        break;

    case IO_SENSOR:
        if ((Value == 0) || (Value == 1)) // ignore incorrect values
        {
            // SENS_Enable_Disable_Sensor( Entities_Def[ Entity_Port_Or_Act_ID ].Entity_PORT_OR_ACT_ID , Value );
            ret_val = 0;
        }
        break;
    }
    return ret_val;
}

// int IO_Entity_Mgr_Convert_Host_Cmd(int Entity_Port_Or_Act_ID, int Value)
// {
//     int command_val;

//     command_val = -1; // to ignore illegal command

//     if (Entities_Def[Entity_Port_Or_Act_ID].Entity_Type == IO_ACTIVATOR)
//     {
//         switch (Entity_Port_Or_Act_ID)
//         {
//         // update
//         case ACT_LDR_YA_PISTON_ACT_ENTITY:

//             switch (Value)
//             {
//             case IO_ENTITY_ARM_TO_NEAR_CMD:

//                 command_val = YA_PISTON_NEAR_ACT_CMD;

//                 break;

//             case IO_ENTITY_ARM_TO_FAR_CMD:

//                 command_val = YA_PISTON_FAR_ACT_CMD;

//                 break;

//             case IO_ENTITY_ACT_FREEZE_CMD:

//                 command_val = YA_PISTON_FREEZE_ACT_CMD;

//                 break;

//             case IO_ENTITY_ACT_UNFREEZE_CMD:

//                 command_val = YA_PISTON_UNFREEZE_ACT_CMD;

//                 break;
//             }

//             break;

//         case ACT_LDR_CL_PISTON_ACT_ENTITY:

//             if (Value == IO_ENTITY_CLAMP_TO_UNLOCK_CMD)
//                 command_val = CL_PISTON_UNLOCK_ACT_CMD;
//             else if (Value == IO_ENTITY_CLAMP_TO_LOCK_CMD)
//                 command_val = CL_PISTON_LOCK_ACT_CMD;

//             break;

//         case ACT_LDR_XA_PISTON_LEFT_ACT_ENTITY:

//             switch (Value)
//             {
//             case IO_ENTITY_ARM_TO_NEAR_CMD:

//                 command_val = XA_PISTON_NEAR_ACT_CMD;

//                 break;

//             case IO_ENTITY_ARM_TO_FAR_CMD:

//                 command_val = XA_PISTON_FAR_ACT_CMD;

//                 break;

//             case IO_ENTITY_ACT_FREEZE_CMD:

//                 command_val = XA_PISTON_FREEZE_ACT_CMD;

//                 break;

//             case IO_ENTITY_ACT_UNFREEZE_CMD:

//                 command_val = XA_PISTON_UNFREEZE_ACT_CMD;

//                 break;
//             }

//             break;

//         case ACT_LDR_ZP_PISTON_ACT_ENTITY:

//             if (Value == IO_ENTITY_PINS_TO_LOCK_CMD)
//                 command_val = ZP_PISTON_LOCK_ACT_CMD;
//             else if (Value == IO_ENTITY_PINS_TO_UNLOCK_CMD)
//                 command_val = ZP_PISTON_UNLOCK_ACT_CMD;

//             break;

//         case ACT_LDR_XP_PISTON_ACT_ENTITY:

//             if (Value == IO_ENTITY_PINS_TO_LOCK_CMD)
//                 command_val = XP_PISTON_LOCK_ACT_CMD;
//             else if (Value == IO_ENTITY_PINS_TO_UNLOCK_CMD)
//                 command_val = XP_PISTON_UNLOCK_ACT_CMD;

//             break;

//         case ACT_LDR_ZA_PISTON_ACT_ENTITY:

//             switch (Value)
//             {
//             case IO_ENTITY_ARM_TO_UP_CMD:

//                 command_val = ZA_PISTON_UP_ACT_CMD;

//                 break;

//             case IO_ENTITY_ARM_TO_DOWN_CMD:

//                 command_val = ZA_PISTON_DN_ACT_CMD;

//                 break;

//             case IO_ENTITY_ACT_FREEZE_CMD:

//                 command_val = ZA_PISTON_FREEZE_ACT_CMD;

//                 break;

//             case IO_ENTITY_ACT_UNFREEZE_CMD:

//                 command_val = ZA_PISTON_UNFREEZE_ACT_CMD;

//                 break;
//             }

//             break;

//         case ACT_CLAMP_LEFT_LOCK_ACT_ENTITY:

//             if (Value == IO_ENTITY_CLAMP_TO_LOCK_CMD)
//                 command_val = CLAMP_LEFT_LOCK_ACT_CMD;
//             else if (Value == IO_ENTITY_CLAMP_TO_UNLOCK_CMD)
//                 command_val = CLAMP_LEFT_UNLOCK_ACT_CMD;

//             break;

//         case ACT_CLAMP_RIGHT_LOCK_ACT_ENTITY:

//             if (Value == IO_ENTITY_CLAMP_TO_LOCK_CMD)
//                 command_val = CLAMP_RIGHT_LOCK_ACT_CMD;
//             else if (Value == IO_ENTITY_CLAMP_TO_UNLOCK_CMD)
//                 command_val = CLAMP_RIGHT_UNLOCK_ACT_CMD;

//             break;

//         case ACT_CLAMP1_3S_ACT_ENTITY:

//             if (Value == IO_ENTITY_CLAMP_TO_LOCK_CMD)
//                 command_val = CLAMP1_S3_CLOSE_CMD;
//             else if (Value == IO_ENTITY_CLAMP_TO_MID_CMD)
//                 command_val = CLAMP1_S3_MID_CMD;
//             else if (Value == IO_ENTITY_CLAMP_TO_UNLOCK_CMD)
//                 command_val = CLAMP1_S3_OPEN_CMD;

//             break;

//         case ACT_OPEN_COVER_PISTON_ACT_ENTITY:

//             if (Value == IO_ENTITY_UPPER_COVER_CLOSE_CMD)
//                 command_val = UPPER_COVER_CLOSE_ACT_CMD;
//             else if (Value == IO_ENTITY_UPPER_COVER_OPEN_CMD)
//                 command_val = UPPER_COVER_OPEN_ACT_CMD;

//             break;

//         case ACT_UT_WELDER_ACT_ENTITY:

//             if (Value == IO_ENTITY_UT_WELDER_UP_CMD)
//                 command_val = UT_WELDER_UP_ACT_CMD;
//             else if (Value == IO_ENTITY_UT_WELDER_DOWN_CMD)
//                 command_val = UT_WELDER_DOWN_ACT_CMD;
//             break;

//         default:

//             command_val = Value;

//             break;
//         }
//     }
//     else
//     {
//         command_val = Value;
//     }

//     return command_val;
// }
