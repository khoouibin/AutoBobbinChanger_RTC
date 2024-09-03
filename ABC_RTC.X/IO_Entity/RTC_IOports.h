// ---------------------------------------------------------------------------
//  FileName: RTC_IOPorts.h
//  Created by: Adam Lin
//  Date: 12/06/2017 	
// 	swap ID for integration test => 12 & 76,	35 & 82,	71 & 83,	36 & 84 
//	swap ID for integration test => 58 & 60,    57 & 61
//  160502 add IOP_AIR_VALVE_ON_ID = 87
//  170509 add IOP_BC_DETECT_SENSOR ID = 97
//  Modified by: Nissim Avichail
//  Date: 12/11/18
//  Orisol (c)
// ---------------------------------------------------------------------------
#ifndef _RTC_IOPORTS_H_
#define _RTC_IOPORTS_H_

typedef enum
{
	IOP_UNDEFINED                   = -1 , 	// when no IOP defined
	
	IOP_YA_PISTON_ID                = 0,
	IOP_YA1_PISTON_ID               = 0,	//Orisew_Loader
	IOP_LDR_T_PISTON_DN_ID          = 1,
	IOP_YA2_PISTON_ID               = 1,	//Orisew_Loader, additional output for the 5-way 3-position valve
	IOP_CL_PISTON_ID                = 2,
	IOP_LDR_CL_PISTON_ID            = 2,	//Orisew_Loader
	IOP_XA_PISTON_ID                = 3,
	IOP_XA1_PISTON_ID               = 3,	//Orisew_Loader
	IOP_LDR_X_PISTON_RIGHT_ID       = 4,
	IOP_XA2_PISTON_ID               = 4,	//Orisew_Loader, additional output for the 5-way 3-position valve
	IOP_ZP_PISTON_ID                = 5,
	IOP_LDR_ZP_PISTON_ID            = 5,	//Orisew_Loader
	IOP_RIVET_MAGNET_ID             = 6,
    IOP_LOADER_SAFETY_DISABLE_ID    = 6,    //58 safety - changed for Israel testing = 6,	//IOP_ENABLE_LIGHT_CURTAIN_ID = 6,	//Orisew_Safeties
	IOP_RIVET_STOP_ID               = 7,
	IOP_LOADER_RED_LAMP_ID          = 7,	//Orisew_Loader
	IOP_XP_PISTON_ID                = 8,
    IOP_LDR_XP_PISTON_ID            = 8,        
	IOP_ZA_PISTON_ID                = 9,
	IOP_ZA1_PISTON_ID               = 9,
	IOP_THREAD_CONSUMPTION_ID       = 10,	//Spool_Counters
	IOP_LOCATING_SENSOR_ID          = 10,
	IOP_PALLET_COVER_OPEN_SENSOR_ID = 11,	//Orisew_Safeties
	IOP_BC_YO_SENSOR_ID             = 12,	//Bobbin_Changer

    IOP_STOP_SEW_BUTTON_ID          = 76,
    IOP_CUTTER_OUT_SENSOR_ID        = 12,
    IOP_SEW_PEDAL_ID                = 82,
    IOP_RIVET_ALARM_ID              = 35,
    IOP_LEFT_LOCK_PEDAL_ID          = 84,
    IOP_RIVET_IN_POSITION_ID        = 36,
    IOP_RIGHT_LOCK_PEDAL_ID         = 83,
    IOP_RIVET_READY_ID              = 71,
    IOP_R_HOME_SENSOR_ID            = 66,
    IOP_PF_HOME_SENSOR_ID           = 85,
    IOP_OLD_PF_HOME_SENSOR_ID       = 85,
    IOP_CLAMP_PF_DIRECTION_ID       = 57,
    IOP_CLAMP_PF_STEP_ID            = 58,
    IOP_CLAMP_PF_MOTOR_ON_ID        = 88,
    IOP_R_SERVO_ON_ID               = 59,
    IOP_R_STEP_ID                   = 60,
    IOP_R_DIRECTION_ID              = 61,
    IOP_THREAD_BREAK_INPUT_ID       = 96,
    IOP_BC_BD_SENSOR_ID             = 97,
                       
	IOP_PF_UP_SENSOR_ID             = 13,
	IOP_BOBBIN_COUNTER_ID           = 13,	//Spool_Counters
	IOP_WINDER_COUNTER_ID           = 14,	//Spool_Counters
	IOP_PUNCHER_DOWN_SENSOR_ID      = 15,	//Puncher
    IOP_COVER_SAFETY_KEY_ID         = 15,   // 14 safety - changed for Israel testing = 15,   //Orisew_Safeties
    IOP_UT_WELDER_DOWN_SENSOR_ID    = 15,   //Welder
    IOP_UT_WELDER_UP_SENSOR_ID      = 16,   //Welder
	IOP_SERVOS_EDM_ID               = 16,   // 63 safety - changed for Israel testing = 16,	//Orisew_Safeties
	IOP_PUNCHER_UP_SENSOR_ID        = 16,	//Puncher
    IOP_TABLE_DOWN_SENSOR_ID        = 16,   //TABLE_DOWN_SENSOR
	IOP_Z_INDEX_ID                  = 17,	//Z_Servo
	IOP_Z_CH_B_ID                   = 18,	//Z_Servo
	IOP_Z_CH_A_ID                   = 19,	//Z_Servo
	IOP_TENSION_CURRENT_POLARITY_ID = 20,	//Tension
	IOP_Z_SERVO_ON_ID               = 21,	//Z_Servo
	IOP_Z_DIRECTION_ID              = 22,	//Z_Servo
	IOP_Z_STEP_ID                   = 23,	//Z_Servo
	IOP_X_SERVO_ON_ID               = 24,	//Z_Servo
	IOP_Z_ALARM_ID                  = 25,	//Z_Servo
	IOP_X_STEP_ID                   = 26,	//X_Servo
	IOP_X_DIRECTION_ID              = 27,	//X_Servo
	IOP_X_ALARM_ID                  = 28,	//X_Servo
	IOP_LDR_ZU_SENSOR_ID            = 29,	//Orisew_Loader
	IOP_LDR_ZD_SENSOR_ID            = 30,	//Orisew_Loader
	IOP_LDR_XN_SENSOR_ID            = 31,	//Orisew_Loader
	IOP_LDR_XF_SENSOR_ID            = 32,	//Orisew_Loader
	IOP_PALLET_SENSOR_ID            = 33,	//Orisew_Loader
    	IOP_AUTO_SEW_SENSOR_ID          = 33,
	IOP_LOADER_SAFETY_KEY_ID        = 34,   // 29 safety - changed for Israel testing = 34,	//IOP_TECHNICIAN_KEY_INPUT_ID = 34,	//Orisew_Safeties
	IOP_BC_XC_SENSOR_ID             = 35,	//Bobbin_Changer
	IOP_BC_XH_SENSOR_ID             = 36,	//Bobbin_Changer
    	IOP_READY_LAMP_ID               = 37,	//Orisew_Loader
	IOP_SAFETY_LAMP_ID              = 38,
	IOP_ZA2_PISTON_ID               = 38,	//Orisew_Loader, additional output for the 5-way 3-position valve
	IOP_MOTOR_SSR_ON_ID             = 39,
	IOP_CLAMP_LEFT_LOCK_ID          = 40,
	IOP_COVER_SAFETY_DISABLE_ID     = 40,   // 38 safety - changed for Israel testing = 40, 	//IOP_ENABLE_STO = 40,		//Orisew_Safeties
	IOP_CLAMP_RIGHT_LOCK_ID         = 41,
	IOP_R_CLAMP_LOCK_ID             = 42,
	IOP_RESET_LOADER_ALARM_ID       = 42,   // 2 safety - changed for Israel testing  = 42, 	//Orisew_Safeties
	IOP_FLIP_FLOP_ID                = 43,
	IOP_RESET_SAFETY_RELAY_ID       = 43,   // 0 safety - changed for Israel testing = 43,	//IOP_RELEASE_SAFE_RELAY_ID	= 43,	//Orisew_Safeties
	IOP_BC_CX_CYLINDER_ID           = 44,	//Bobbin_Changer, the old name is IOP_BC_ARM_TURN_CYLINDER_ID
	//IOP_PRESSER_FOOT_ID           = 44,
	IOP_PF_LEVEL_HIGH_ID            = 45,
	IOP_BC_CY_CYLINDER_ID           = 45, 	//Bobbin_Changer
	IOP_PINCHER_ID                  = 46,
	IOP_RESET_EDM_ALARM_ID          = 46,   // IOP_RESET_EDM_ALARM_ID = 46,	//Orisew_Safeties
	IOP_PULLER_ID                   = 47,
	IOP_BC_JAW_CYLINDER_ID          = 47,	//Bobbin_Changer
	IOP_WIPER_ID                    = 48,
	IOP_CUTTER_ID                   = 49,
	IOP_NEEDLE_COOLER_ID            = 50,
	IOP_BC_CA_CYLINDER_ID           = 51,	//Bobbin_Changer, The old name is IOP_BC_CASSETE_CYLINDER_ID.
	IOP_PUNCHER_PISTON_UP_ID        = 52,
	IOP_LOADER_GREEN_LAMP_ID        = 52,   // 53 safety - changed for Israel testing = 52,	//Orisew_Loader
    	IOP_UT_WELDER_DEVICE_ON_ID      = 52,   //Welder
	IOP_RIVET_PISTON_DN_ID          = 53,	//Rivet
	IOP_Y_SERVO_ON_ID               = 54,	//Y_Servo
	IOP_Y_STEP_ID                   = 55,	//Y_Servo
	IOP_Y_DIRECTION_ID              = 56,	//Y_Servo
	//IOP_CLAMP_PF_DIRECTION_ID 	= 57,	// 160301 remarked
	//IOP_CLAMP_PF_STEP_ID          = 58,	// 160301 remarked	
	//IOP_R_SERVO_ON_ID             = 59,	// 160301 remarked
	//IOP_R_STEP_ID                 = 60,	// 160301 remarked
	//IOP_R_DIRECTION_ID            = 61,	// 160301 remarked
	IOP_PUNCHER_PISTON_DN_ID        = 62,	//Puncher	////should be removed after correct the  Activator_Def[] in Activator.c
	IOP_PUNCHER_PISTON_DOWN_ID      = 62,	//Puncher
	IOP_OPEN_COVER_PISTON_ID        = 62,	// 1 safety - changed for Israel testing = 62,	//Orisew_Safeties
    	IOP_UT_WELDER_PISTON_ID         = 62,   //Welder    
	IOP_R_ALARM_ID                  = 63,
	IOP_BC_CF_SENSOR_ID             = 63,	//Bobbin_Changer
	IOP_Y_ALARM_ID                  = 64,	//Y_Servo
	IOP_BOOST_BUTTON_ID             = 65,	//Boot Loader button
	IOP_BC_CB_SENSOR_ID             = 66,	//Bobbin_Changer
	//IOP_R_HOME_SENSOR_ID          = 66,	// 160301 remarked change to IOP_PF_HOME_SENSOR_ID for the Israel test
	IOP_Y_HOME_SENSOR_ID            = 67,	//Y_Servo
	IOP_X_HOME_SENSOR_ID            = 68,	//X_Servo
	IOP_AIR_FAN_SENSOR_ID           = 69,
	IOP_AIR_SENSOR_ID               = 70,
	IOP_BC_YI_SENSOR_ID             = 71,	//Bobbin_Changer
	IOP_YN_SENSOR_ID                = 72,	
	IOP_LDR_YN_SENSOR_ID            = 72,	//Orisew_Loader
	IOP_YF_SENSOR_ID                = 73,
	IOP_LDR_YF_SENSOR_ID            = 73,	//Orisew_Loader
	IOP_XL_SENSOR_ID                = 74,
	IOP_LDR_XL_SENSOR_ID            = 74,	//Orisew_Loader
	IOP_ZL_SENSOR_ID                = 75,
	IOP_LDR_ZL_SENSOR_ID            = 75,	//Orisew_Loader
	//IOP_STOP_SEW_BUTTON_ID        = 76,
	IOP_R_INDEX_ID                  = 77,
	IOP_Y_INDEX_ID                  = 78,	//Y_Servo
	IOP_LOADER_SAFETY_ALARM_ID      = 79,   // 74 safety - changed for Israel testing = 79, 	//Orisew_Safeties
	IOP_SAFETY_BUTTON_ID            = 79,
	IOP_X_INDEX_ID                  = 80,	//X_Servo
	IOP_READY_BUTTON_ID             = 81,	//Orisew
	//IOP_SEW_PEDAL_ID              = 82,
	//IOP_RIGHT_LOCK_PEDAL_ID       = 83,
	//IOP_LEFT_LOCK_PEDAL_ID        = 84
	//IOP_PF_HOME_SENSOR_ID         = 85
	IOP_TENSION_PWM_ID              = 86,	//Tension
	IOP_AIR_VALVE_ON_ID             = 87,
	//IOP_CLAMP_PF_MOTOR_ON_ID      = 88,
    IOP_LEVEL_SHIFT_IC_ID           = 89,
	IOP_RESET_THREAD_BREAK_ID       = 90,	//Thread_Break_Detect
    IOP_LOWER_COVER_CLOSED_ID       = 91, 	// 64 safety - changed for Israel testing = 91,  //Orisew_Safeties
    IOP_UPPER_COVER_CLOSED_ID       = 92, 	// 33 safety - changed for Israel testing = 92,  //Orisew_Safeties    
    IOP_UPPER_COVER_OPENED_ID       = 93, 	// 28 safety - changed for Israel testing = 93,  //Orisew_Safeties    
    IOP_COVER_OPEN_ALARM_ID         = 94, 	// 31 safety - changed for Israel testing = 94,  //Orisew_Safeties
	IOP_RESET_RFID_ID               = 95	//RFID
}
    IO_Port_Id_t;

// ---------------------------------------------------------------------------	
// IO definition
// ---------------------------------------------------------------------------
//	PORT A
// ---------------------------------------------------------------------------
	#define  IOP_LDR_YA1_PISTON				LATAbits.LATA0		//Orisew Loader	

	#define  IOP_LDR_YA2_PISTON				LATAbits.LATA1		//Orisew Loader	

	
	#define  IOP_RESET_RFID					LATAbits.LATA2		//RFID reset
	#define  IOP_I2C_ASCL2					LATAbits.LATA2		//I2C2

	#define  IOP_I2C_ASDA2					LATAbits.LATA3		//I2C2
	
	#define  IOP_LDR_CL_PISTON				LATAbits.LATA4		//Orisew Loader	

	#define  IOP_LDR_XA1_PISTON 			LATAbits.LATA5		//Orisew Loader

	#define  IOP_LDR_XA2_PISTON 			LATAbits.LATA6		//Orisew Loader	

	#define  IOP_LDR_ZP_PISTON				LATAbits.LATA7		//Orisew Loader	
	
	#define	 IOP_RIVET_MAGNET				LATAbits.LATA9		//RIVET
	#define  IOP_LOADER_SAFETY_DISABLE      LATAbits.LATA9      //IOP_ENABLE_LIGHT_CURTAIN				//Orisew Safeties
	
	#define  IOP_RIVET_STOP					LATAbits.LATA10		//RIVET
	#define  IOP_LOADER_RED_LAMP			LATAbits.LATA10		//Orisew Loader
	
	#define  IOP_LDR_XP_PISTON				LATAbits.LATA14		//Orisew Loader
	
	#define  IOP_LDR_ZA1_PISTON				LATAbits.LATA15		//Orisew Loader
// ---------------------------------------------------------------------------
//	PORT B
// ---------------------------------------------------------------------------
	#define  IOP_Z_STEP_COUNTER_IN			PORTBbits.RB0		//Z step counter input
	
	#define	 IOP_UPPER_COVER_CLOSED         PORTBbits.RB1		//IOP_UPPER_COVER_CLOSED_SENSOR  //Orisew safeties, 5V, need ext circuitry
	#define	 IOP_SPARE_RB1					PORTBbits.RB1

	#define  IOP_LOWER_COVER_CLOSED         PORTBbits.RB2     //IOP_LOWER_COVER_CLOSED_SENSOR	//Orisew safeties, 5V, need ext circuitry
	#define	 IOP_SPARE_RB2					PORTBbits.RB2
	
    #define	 IOP_PF_HOME_SENSOR				PORTBbits.RB3		//PF_Servo, 24V

	#define	 IOP_STOP_SEW_BUTTON			PORTBbits.RB4
	
	#define  IOP_R_INDEX					PORTBbits.RB5		//R_Servo, 5V, need re-assigned to RD4

	#define  IOP_ICSP_CLK					LATBbits.LATB6		//for programmer/debugger
	
	#define  IOP_ICSP_DAT					PORTBbits.RB7		//for programmer/debugger
	
	#define  IOP_Y_INDEX					PORTBbits.RB8		//Y_Servo, 5V, re-map RPI40 to INT4
	
	#define  IOP_LOADER_SAFETY_ALARM        PORTBbits.RB9       //IOP_CURTAIN_ACTIVATED					//Orisew safeties
	#define  IOP_SAFETY_BUTTON				PORTBbits.RB9
	
	#define  IOP_X_INDEX					PORTBbits.RB10		//X_Servo, 5V, re-map RPI42 to INT3
	
	#define  IOP_USB_VOLTAGE_SENSE			PORTBbits.RB11		//USB, reserve for USB Voltage A/D input
	
	#define  IOP_READY_BUTTON				PORTBbits.RB12		//Orisew Loader
	
    #define  IOP_SEW_PEDAL 					PORTBbits.RB13		//Pedal
	#define  IOP_RIGHT_LOCK_PEDAL 			PORTBbits.RB14		//Pedal
	#define  IOP_LEFT_LOCK_PEDAL 			PORTBbits.RB15		//Pedal

    
// ---------------------------------------------------------------------------
//	PORT C
// ---------------------------------------------------------------------------	
    #define	 IOP_BC_BD_SENSOR				PORTCbits.RC1		// Bobbin_Changer, RTC v.4, but the pin name is STEP_MOTOR_VREF_PWM

	//#define  IOP_STEP_MOTOR_VREF_PWM		LATCbits.LATC1		//Modify to 5V input with pull high resister in RTC v.4
	
	#define  IOP_THREAD_CONSUMPTION			PORTCbits.RC2		//Spool_Counters, remap RPI50 to T2 ext clock, 24V
	#define  IOP_LOCATING_SENSOR			PORTCbits.RC2
	
	#define  IOP_PALLET_COVER_OPEN_SENSOR	PORTCbits.RC3		//Orisew Safeties
	#define  IOP_AUTO_SEW_SENSOR			PORTCbits.RC3
	
	//#define  IOP_CUTTER_OUT_SENSOR 		PORTCbits.RC4
	#define  IOP_BC_YO_SENSOR	 			PORTCbits.RC4		//Bobbin_Changer, but the pin name is IOP_CUTTER_OUT_SENSOR
	
	#define  IOP_OSC1						PORTCbits.RC12		//Oscillator
	
	#define  IOP_BOBBIN_COUNTER				PORTCbits.RC13		//Spool_Counters, remap RPI61 to T4, 24V
	#define  IOP_PF_UP_SENSOR				PORTCbits.RC13	
	
	#define  IOP_WINDER_COUNTER				PORTCbits.RC14		//Spool_Counters, T1CLK , 24V
	
	#define  IOP_OSC2						PORTCbits.RC15		//Oscillator
// ---------------------------------------------------------------------------
//	PORT D
// ---------------------------------------------------------------------------
	#define  IOP_SPI1_CHIP_SELECT			LATDbits.LATD0		//SPI Flash	//RP64	//Connect to SST26VF064B
	
	#define  IOP_SPI1_DATA_OUT				LATDbits.LATD1		//SPI Flash	//RP65
	
	#define  IOP_RESET_THREAD_BREAK 		LATDbits.LATD2		//Thread_Break_Detect	//3V3 to D flip-flop SN74LVC1G175
	
	#define IOP_THREAD_BREAK_INPUT			PORTDbits.RD3		//Thread_Break_Detect	//3V3 from D flip-flop SN74LVC1G175
	
	#define  IOP_COVER_OPEN_ALARM       	PORTDbits.RD4		//RP68	//Orisew safeties, 5V, need ext circuitry
	
	#define  IOP_SPI1_CLOCK_OUTPUT			LATDbits.LATD5		//SPI Flash	//RP69
	
	#define  IOP_COVER_SAFETY_KEY			PORTDbits.RD6
	#define  IOP_PUNCHER_DOWN_SENSOR		PORTDbits.RD6
    	#define  IOP_UT_WELDER_DOWN_SENSOR		PORTDbits.RD6       //Welder

	#define  IOP_SERVOS_EDM					PORTDbits.RD7
	#define  IOP_PUNCHER_UP_SENSOR			PORTDbits.RD7
    	#define  IOP_UT_WELDER_UP_SENSOR		PORTDbits.RD7       //Welder
	
	#define  IOP_Z_INDEX					PORTDbits.RD8		//Z_Servo	//5V	//RPI72		//QEI
	
	#define  IOP_I2C_ASDA1					PORTDbits.RD9		//I2C1	//RPI73	
	#define  IOP_I2C_ASCL1					PORTDbits.RD10		//I2C1	//RPI74
	
	#define  IOP_SPARE_RD11					PORTDbits.RD11		//PPI75
	#define  IOP_UPPER_COVER_OPENED     	PORTDbits.RD11		//IOP_UPPER_COVER_OPENED_SENSOR   //Orisew_Safeties

	
	#define  IOP_Z_CH_B						PORTDbits.RD12		//Z_Servo	//5V	//RPI76		//QEI
	#define  IOP_Z_CH_A 					PORTDbits.RD13		//Z_Servo	//5V	//RPI77		//QEI
	
	#define  IOP_SPI1_DATA_IN				PORTDbits.RD14		//SPI_Flash	//RPI78
	
	#define  IOP_AIR_VALVE_ON				LATDbits.LATD15
// ---------------------------------------------------------------------------
//	PORT E
// ---------------------------------------------------------------------------
	#define  IOP_TENSION_PWM				LATEbits.LATE0		//Tension	//PWM1L
	#define  IOP_CH_PEDAL_HEIGHT			PORTEbits.RE1		//Pedal		//AN25	//ADC
	#define  IOP_Z_SERVO_ON					LATEbits.LATE2		//Z_Servo
	#define  IOP_Z_DIRECTION				LATEbits.LATE3		//Z_Servo	//5V
	#define  IOP_Z_STEP						LATEbits.LATE4		//Z_Servo	//5V
	#define  IOP_X_SERVO_ON					LATEbits.LATE5		//Z_Servo
	#define  IOP_Z_ALARM					PORTEbits.RE6		//Z_Servo	//24V
	#define  IOP_X_STEP						LATEbits.LATE7		//X_Servo	//5V
	#define  IOP_X_DIRECTION				LATEbits.LATE8		//X_Servo	//5V
	#define  IOP_X_ALARM					PORTEbits.RE9		//X_Servo	//24V
// ---------------------------------------------------------------------------
//	PORT F
// ---------------------------------------------------------------------------
	#define  IOP_LDR_ZU_SENSOR				PORTFbits.RF0		//Orisew_Loader
	
	#define  IOP_LDR_ZD_SENSOR				PORTFbits.RF1		//Orisew_Loader
	
	#define  IOP_LDR_XN_SENSOR				PORTFbits.RF2		//Orisew_Loader
	
	#define  IOP_LDR_XF_SENSOR				PORTFbits.RF3		//Orisew_Loader
	#define  IOP_XF_SENSOR					PORTFbits.RF3
	#define  IOP_XR_SENSOR					PORTFbits.RF3		//should modify schematic LDR_XR_SENSOR to LDR_XF_SENSOR
	
	#define  IOP_PALLET_SENSOR				PORTFbits.RF4		//Orisew_Loader
	
	#define  IOP_LOADER_SAFETY_KEY          PORTFbits.RF5      //IOP_TECHNICIAN_KEY_INPUT				//Orisew_Safety
	#define  IOP_LDR_SF_SENSOR				PORTFbits.RF5
	
	#define  IOP_TENSION_URRENT_POLARITY	LATFbits.LATF8		//Tension	//5V
	
	#define  IOP_RIVET_ALARM				PORTFbits.RF12		//Rivet
	#define  IOP_BC_XC_SENSOR				PORTFbits.RF12		//Bobbin_Changer	//170626 bug fix
	
	#define	 IOP_RIVET_IN_POSITION			PORTFbits.RF13		//Rivet
	#define	 IOP_BC_XH_SENSOR				PORTFbits.RF13		//Bobbin_Changer

// ---------------------------------------------------------------------------
//	PORT G
// ---------------------------------------------------------------------------
	#define  IOP_RS232_Tx					LATGbits.LATG0		//RP112		//RS232
	
	#define  IOP_RS232_Rx					PORTGbits.RG1		//RP113		//RS232
	
	#define  IOP_USB_D_Plus					LATGbits.LATG2		//USB		//D+			
	
	#define  IOP_USB_D_Minus 				LATGbits.LATG3		//USB		//D-

	#define  IOP_CAN1_TX					LATGbits.LATG6		//RP118		//CAN1
	
	#define  IOP_CAN1_RX					PORTGbits.RG7		//RPI119	//CAN1
	
	#define  IOP_CAN2_TX					LATGbits.LATG8		//RP120		//CAN2
	
	#define  IOP_CAN2_RX					PORTGbits.RG9		//RPI121	//CAN2
	
	#define  IOP_RS485_RX					PORTGbits.RG12		//RP124		//RS485
	
	#define  IOP_RS485_TX					LATGbits.LATG13		//RP125		//RS485

    #define  IOP_CLAMP_PF_MOTOR_ON			LATGbits.LATG14
	
	#define  IOP_PGOE               		LATGbits.LATG15		//IOP_LEVEL_SHIFT_IC
// ---------------------------------------------------------------------------
//	PORT H
// ---------------------------------------------------------------------------
	#define  IOP_READY_LAMP					LATHbits.LATH0		//Orisew Loader
	
	#define  IOP_LDR_ZA2_PISTON				LATHbits.LATH1		//Orisew Loader
	
	#define  IOP_MOTOR_SSR_ON				LATHbits.LATH2
	
	#define  IOP_CLAMP_LEFT_LOCK			LATHbits.LATH3		//Clamp											//Set '1' for On
	#define  IOP_COVER_SAFETY_DISABLE       LATHbits.LATH3      //IOP_ENABLE_STO	//Orisew Safeties, enable STO bypass	//Set '1' for On
	
	#define  IOP_CLAMP_RIGHT_LOCK			LATHbits.LATH4		//Clamp											//Set '0' for On
	
	#define  IOP_R_CLAMP_LOCK				LATHbits.LATH5		//Clamp											//Set '0' for On
	#define  IOP_RESET_LOADER_ALARM         LATHbits.LATH5      //IOP_CLEAR_CURTAIN_ALARM	//Orisew Safeties	//Set '0' for On

	#define  IOP_FLIP_FLOP					LATHbits.LATH6		//Flip flop										//Set '0' for On
	#define  IOP_RESET_SAFETY_RELAY         LATHbits.LATH6      //IOP_RELEASE_SAFE_RELAY	//Orisew Safeties	//Set '0' for On
	
	#define  IOP_PRESSER_FOOT				LATHbits.LATH7
	#define  IOP_BC_CX_CYLINDER				LATHbits.LATH7		//Bobbin_Changer
	
	#define  IOP_PF_LEVEL_HIGH				LATHbits.LATH8
	#define  IOP_BC_CY_CYLINDER				LATHbits.LATH8		//Bobbin_Changer
	
	#define  IOP_PINCHER					LATHbits.LATH9	
	#define  IOP_RESET_EDM_ALARM            LATHbits.LATH9     //IOP_RESET_EDM_ALARM				//Orisew Safeties
	
	#define  IOP_PULLER						LATHbits.LATH10
	#define  IOP_BC_JAW_CYLINDER			LATHbits.LATH10		//Bobbin_Changer

	
	#define  IOP_WIPER						LATHbits.LATH11		//Wiper
	
	#define  IOP_CUTTER						LATHbits.LATH12		//Cutter
	
	#define  IOP_NEEDLE_COOLER				LATHbits.LATH13		//Needle coller
	
	#define  IOP_SPARE_LATH14				LATHbits.LATH14
	#define  IOP_BC_CA_CYLINDER				LATHbits.LATH14		//BC

	#define  IOP_PUNCHER_PISTON_UP			LATHbits.LATH15
   	#define  IOP_UT_WELDER_DEVICE_ON        LATHbits.LATH15     //Welder
	#define  IOP_LOADER_GREEN_LAMP			LATHbits.LATH15		//Orisew Safeties
// ---------------------------------------------------------------------------
//	PORT J
// ---------------------------------------------------------------------------
	#define  IOP_RIVET_PISTON_DN			LATJbits.LATJ0
	
	#define  IOP_Y_SERVO_ON					LATJbits.LATJ1		//Y_Servo
	
	#define  IOP_Y_STEP						LATJbits.LATJ2		//Y_Servo	//5V
	
	#define  IOP_Y_DIRECTION				LATJbits.LATJ3		//Y_Servo	//5V

    #define  IOP_R_SERVO_ON					LATJbits.LATJ6		//R_Servo
	#define  IOP_R_STEP						LATJbits.LATJ7		//R_Servo	//5V
	#define  IOP_R_DIRECTION				LATJbits.LATJ8		//R_Servo	//5V
    #define  IOP_CLAMP_PF_DIRECTION			LATJbits.LATJ4		//PF_Servo	//5V
	#define  IOP_CLAMP_PF_STEP				LATJbits.LATJ5		//PF_Servo	//5V
	
	#define  IOP_PUNCHER_PISTON_DN			LATJbits.LATJ9		//Puncher
    	#define  IOP_UT_WELDER_PISTON		LATJbits.LATJ9		//Welder
	#define  IOP_OPEN_COVER_PISTON          LATJbits.LATJ9      //IOP_ACTIVATE_COVER_PISTON				//Orisew Safeties

	#define  IOP_R_ALARM  					PORTJbits.RJ10		//R_Servo
	#define  IOP_BC_CF_SENSOR   			PORTJbits.RJ10		//Bobbin_Changer

	#define  IOP_Y_ALARM					PORTJbits.RJ11		//Y_Servo

    #define BOOST_BUTTON					PORTJbits.RJ12		//Boot loader button
	
	#define  IOP_R_HOME_SENSOR				PORTJbits.RJ13		//R_Servo
	#define  IOP_BC_CB_SENSOR				PORTJbits.RJ13		//Bobbin_Changer

	#define  IOP_Y_HOME_SENSOR				PORTJbits.RJ14		//Y_Servo
	
	#define  IOP_X_HOME_SENSOR				PORTJbits.RJ15		//X_Servo
// ---------------------------------------------------------------------------
//	PORT K
// ---------------------------------------------------------------------------
	#define  IOP_AIR_FAN_SENSOR				PORTKbits.RK0 
	
	#define  IOP_AIR_SENSOR					PORTKbits.RK1
	
	#define  IOP_RIVET_READY				PORTKbits.RK11		//Rivet
	#define  IOP_BC_YI_SENSOR				PORTKbits.RK11		//Bobbin_Changer
	
	#define  IOP_LDR_YN_SENSOR				PORTKbits.RK12		//Orisew_Loader
	
	#define  IOP_LDR_YF_SENSOR				PORTKbits.RK13		//Orisew_Loader
	
	#define  IOP_LDR_XL_SENSOR				PORTKbits.RK14		//Orisew_Loader
	
	#define  IOP_LDR_ZL_SENSOR				PORTKbits.RK15		//Orisew_Loader
// ---------------------------------------------------------------------------

//Define the IO polarity  
	#define IOP_X_HOME_SENSOR_ON	0 					// 0 is ON ans 1 means off
	#define IOP_Y_HOME_SENSOR_ON	0 					// 0 is ON ans 1 means off

#endif      // _RTC_IOPORTS_H_ 
