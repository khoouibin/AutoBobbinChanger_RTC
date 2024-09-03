// ---------------------------------------------------------------------------
//  Filename: RTC_IODef.h
//  Created by: Roey Huberman
//  Date:  06/02/17
//  Modified by: Adam Lin/Min Liao
//  Date: 09/05/17
//  Modified by: Nissim Avichail
//  Date: 12/11/18
//  Orisol (c) 
// ---------------------------------------------------------------------------

#ifndef _RTC_IODEF_H_
#define _RTC_IODEF_H_

#define TRISJ_DIRECTION 0xFC00 // bit12 is input

#define LATA_INIT_VALUE 0x0000
#define LATB_INIT_VALUE 0xFF3F // changed
#define LATC_INIT_VALUE 0xFFFF // 170509 changed for RTC v.4
#define LATD_INIT_VALUE 0x7FDC // changed
#define LATE_INIT_VALUE 0xFE42 // changed
#define LATF_INIT_VALUE 0xFEFF // changed
#define LATG_INIT_VALUE 0x3FFF // changed
#define LATH_INIT_VALUE 0x1870 // 170522 change for BC integration
#define LATJ_INIT_VALUE 0x0000
#define LATK_INIT_VALUE 0xFFFF

#define TRISA_DIRECTION 0x0000 // All pins are output
#define ANSELA_VALUE 0x0000	   // Select no pin as the Analog input
#define TRISB_DIRECTION 0xFFBF // 0b 1111 1111 1011 1111;
#define ANSELB_VALUE 0x0000	   // Select no pin as the Analog input
#define TRISC_DIRECTION 0xF01C // 0b 1111 0000 0001 1100;
#define ANSELC_VALUE 0x0000	   // Select no pin as the Analog input
#define TRISD_DIRECTION 0x79D8 // 0b 0111 1001 1101 1000;  //0106
#define ANSELD_VALUE 0x0000	   // Select no pin as the Analog input
#define TRISE_DIRECTION 0x0242 //	0b 0000 0010 0100 0010
#define ANSELE_VALUE 0x0002	   // Select RE1 pin is Analog input
#define TRISF_DIRECTION 0x303F // 0b 0011 0000 0011 1111
#define TRISG_DIRECTION 0x1282 // 0b 0001 0010 1000 0010
#define ANSELG_VALUE 0x0000	   // Select no pin as the Analog input
#define TRISH_DIRECTION 0x0000 // All pins are output
#define TRISK_DIRECTION 0xF803 // 0b 1111 1000 0000 0011

typedef enum
{
	BIT0_MASK = 0x0001,
	BIT1_MASK = 0x0002,
	BIT2_MASK = 0x0004,
	BIT3_MASK = 0x0008,
	BIT4_MASK = 0x0010,
	BIT5_MASK = 0x0020,
	BIT6_MASK = 0x0040,
	BIT7_MASK = 0x0080,
	BIT8_MASK = 0x0100,
	BIT9_MASK = 0x0200,
	BIT10_MASK = 0x0400,
	BIT11_MASK = 0x0800,
	BIT12_MASK = 0x1000,
	BIT13_MASK = 0x2000,
	BIT14_MASK = 0x4000,
	BIT15_MASK = 0x8000,
} PORT_BIT_t;

#define MAX_PORTS 20		//0116
//#define MAX_PORTS 14

typedef enum
{
	LATA_ID = 0,
	LATB_ID = 1,
	LATC_ID = 2,
	LATD_ID = 3,
	LATE_ID = 4,
	LATF_ID = 5,
	LATG_ID = 6,
	LATH_ID = 7,
	LATJ_ID = 8,
	LATK_ID = 9,
	PORTA_ID = 10,
	PORTB_ID = 11,
	PORTC_ID = 12,
	PORTD_ID = 13,
	PORTE_ID = 14,
	PORTF_ID = 15,
	PORTG_ID = 16,
	PORTH_ID = 17,
	PORTJ_ID = 18,
	PORTK_ID = 19,
} PORT_REG_t;

typedef typeof(LATA) IO_Port_t;		// Adam-- Can remove ? 
//typedef typeof(LATA) IO_Port;

IO_Port_t *IO_Reg_Addr[MAX_PORTS] = {
	&LATA,
	&LATB,
	&LATC,
	&LATD,
	&LATE,
	&LATF,
	&LATG,
	&LATH,
	&LATJ,
	&LATK,
	&PORTA,
	&PORTB,
	&PORTC,
	&PORTD,
	&PORTE,
	&PORTF,
	&PORTG,
	&PORTH,
	&PORTJ,
	&PORTK,
};

typedef struct
{
	PORT_REG_t Reg_ID;
	PORT_BIT_t IO_Mask;
} IO_Port_Def_t;

#define IOP_ID_NUM 98	//Correct and add IOP_RESET_THREAD_BREAK   ID = 90, add IOP_BC_DETECT_SENSOR ID = 98
static IO_Port_Def_t IO_Ports_Table[IOP_ID_NUM] = {
	{LATA_ID, BIT0_MASK},  //  LDR_YA1_PISTON,         ID = 0
	{LATA_ID, BIT1_MASK},  //  LDR_YA2_PISTON,         ID = 1
	{LATA_ID, BIT4_MASK},  //  LDR_CL_PISTON,			ID = 2
	{LATA_ID, BIT5_MASK},  //  LDR_XA1_PISTON,         ID = 3
	{LATA_ID, BIT6_MASK},  //  LDR_XA2_PISTON,         ID = 4
	{LATA_ID, BIT7_MASK},  //  LDR_ZP_PISTON,			ID = 5
	{LATA_ID, BIT9_MASK},  //	RIVET_MAGNET,           ID = 6
	{LATA_ID, BIT10_MASK}, //	RIVET_STOP,             ID = 7
	{LATA_ID, BIT14_MASK}, //  LDR_XP_PISTON,			ID = 8
	{LATA_ID, BIT15_MASK}, //  LDR_ZA1_PISTON,			ID = 9

	{PORTC_ID, BIT2_MASK},	//	LOCATING_SENSOR,        ID = 10
	{PORTC_ID, BIT3_MASK},	// 	AUTO_SEW_SENSOR,        ID = 11
	{PORTC_ID, BIT4_MASK},	// 	CUTTER_OUT_SENSOR,  	ID = 12
	{PORTC_ID, BIT13_MASK}, //	PF_UP_SENSOR,           ID = 13
	{PORTC_ID, BIT14_MASK}, //	WINDER_COUNTER,         ID = 14		// use RC14/T1CK as WCNT

	{PORTD_ID, BIT6_MASK},	// 	PUNCHER_DOWN_SENSOR,	ID = 15     //UT_WELDER_DOWN_SENSOR
	{PORTD_ID, BIT7_MASK},	// 	PUNCHER_UP_SENSOR,  	ID = 16     //UT_WELDER_UP_SENSOR
	{PORTD_ID, BIT8_MASK},	//	Z_INDEX,            	ID = 17
	{PORTD_ID, BIT12_MASK}, //  Z_CH_B,         		ID = 18
	{PORTD_ID, BIT13_MASK}, // 	Z_CH_A,     			ID = 19

	{LATF_ID, BIT8_MASK}, // TENSION_CURRENT_POLARITYID = 20
	//{ LATD_ID , BIT15_MASK } ,	//	Z_QEI_CNT_CMP,  		ID = 20

	{LATE_ID, BIT2_MASK},  //	Z_SERVO_ON,             ID = 21
	{LATE_ID, BIT3_MASK},  //	Z_DIRECTION,        	ID = 22
	{LATE_ID, BIT4_MASK},  //	Z_STEP,                 ID = 23
	{LATE_ID, BIT5_MASK},  //  X_SERVO_ON,             ID = 24
	{PORTE_ID, BIT6_MASK}, //  Z_ALARM,            	ID = 25
	{LATE_ID, BIT7_MASK},  //	X_STEP,                 ID = 26
	{LATE_ID, BIT8_MASK},  //	X_DIRECTION,        	ID = 27
	{PORTE_ID, BIT9_MASK}, //	X_ALARM,            	ID = 28

	{PORTF_ID, BIT0_MASK},	//	LDR_ZU_SENSOR,      	ID = 29
	{PORTF_ID, BIT1_MASK},	//  LDR_ZD_SENSOR,      	ID = 30
	{PORTF_ID, BIT2_MASK},	//	LDR_XN_SENSOR,      	ID = 31
	{PORTF_ID, BIT3_MASK},	//	LDR_XF_SENSOR,      	ID = 32
	{PORTF_ID, BIT4_MASK},	//	PALLET_SENSOR,          ID = 33
	{PORTF_ID, BIT5_MASK},	//	TECHNICIAN_KEY_INPUT    ID = 34
	{PORTF_ID, BIT12_MASK}, //	RIVET_ALARM,            ID = 35
	{PORTF_ID, BIT13_MASK}, //	RIVET_PALLET_IN_POSITION,	ID = 36

	{LATH_ID, BIT0_MASK},  //	READY_LAMP,             ID = 37
	{LATH_ID, BIT1_MASK},  //	LDR_ZA2_PISTON,         ID = 38
	{LATH_ID, BIT2_MASK},  //	MOTOR_SSR_ON,           ID = 39
	{LATH_ID, BIT3_MASK},  //	CLAMP_LEFT_LOCK,        ID = 40
	{LATH_ID, BIT4_MASK},  //  CLAMP_RIGHT_LOCK,       ID = 41
	{LATH_ID, BIT5_MASK},  //	R_CLAMP_LOCK,           ID = 42
	{LATH_ID, BIT6_MASK},  // 	FLIP_FLOP,              ID = 43
	{LATH_ID, BIT7_MASK},  // 	PRESSER_FOOT,           ID = 44
	{LATH_ID, BIT8_MASK},  //	PF_LEVEL_HIGH,          ID = 45
	{LATH_ID, BIT9_MASK},  //  PINCHER,                ID = 46
	{LATH_ID, BIT10_MASK}, //	PULLER,                 ID = 47
	{LATH_ID, BIT11_MASK}, //	WIPER,                  ID = 48
	{LATH_ID, BIT12_MASK}, //	CUTTER,                 ID = 49
	{LATH_ID, BIT13_MASK}, //  NEEDLE_COOLER,          ID = 50
	{LATH_ID, BIT14_MASK}, //	IOP_BC_CASSETE_CYLINDER ID = 51
	{LATH_ID, BIT15_MASK}, //	PUNCHER_PISTON_UP,      ID = 52      //UT_WELDER_DEVICE_ON

	{LATJ_ID, BIT0_MASK},	//	RIVET_PISTON_DOWN,      ID = 53
	{LATJ_ID, BIT1_MASK},	//	Y_SERVO_ON,             ID = 54
	{LATJ_ID, BIT2_MASK},	//	Y_STEP,                 ID = 55
	{LATJ_ID, BIT3_MASK},	//	Y_DIRECTION,            ID = 56
	{LATJ_ID, BIT4_MASK},	//	CLAMP_PF_DIRECTION,     ID = 57
	{LATJ_ID, BIT5_MASK},	//	CLAMP_PF_STEP,          ID = 58
	{LATJ_ID, BIT6_MASK},	//	R_SERVO_ON,             ID = 59
	{LATJ_ID, BIT7_MASK},	//	R_STEP,                 ID = 60
	{LATJ_ID, BIT8_MASK},	//	R_DIRECTION,            ID = 61
	{LATJ_ID, BIT9_MASK},	//	PUNCHER_PISTON_DOWN,	ID = 62     //UT_WELDER_PISTON
	{PORTJ_ID, BIT10_MASK}, //	R_ALARM,                ID = 63
	{PORTJ_ID, BIT11_MASK}, //	Y_ALARM,                ID = 64
	{PORTJ_ID, BIT12_MASK}, //	BOOST_BUTTON,           ID = 65
	//{ LATJ_ID , BIT12_MASK } ,		//	IOP_SPARE_LATJ12,		ID = 65		//wrong assigment, reuse for spare output
	//{ PORTJ_ID , BIT12_MASK } ,		//	IOP_PF_Home_SENSOR,		ID = 65
	{PORTJ_ID, BIT13_MASK}, //	R_HOME_SENSOR,          ID = 66
	{PORTJ_ID, BIT14_MASK}, //	Y_HOME_SENSOR,          ID = 67
	{PORTJ_ID, BIT15_MASK}, //	X_HOME_SENSOR,          ID = 68

	{PORTK_ID, BIT0_MASK},	//	AIR_FAN_SENSOR,     	ID = 69
	{PORTK_ID, BIT1_MASK},	//	AIR_SENSOR,             ID = 70
	{PORTK_ID, BIT11_MASK}, //	RIVET_READY,            ID = 71
	{PORTK_ID, BIT12_MASK}, //	LDR_YN_SENSOR,          ID = 72
	{PORTK_ID, BIT13_MASK}, //	LDR_YF_SENSOR,          ID = 73
	{PORTK_ID, BIT14_MASK}, //	LDR_XL_SENSOR,  		ID = 74
	{PORTK_ID, BIT15_MASK}, //	LDR_ZL_SENSOR,          ID = 75

	{PORTB_ID, BIT4_MASK},	//	STOP_SEW_BUTTON,        ID = 76
	{PORTB_ID, BIT5_MASK},	//	R_INDEX,                ID = 77
	{PORTB_ID, BIT8_MASK},	//	Y_INDEX,                ID = 78
	{PORTB_ID, BIT9_MASK},	//	SAFETY_BUTTON,          ID = 79
	{PORTB_ID, BIT10_MASK}, //	X_INDEX,                ID = 80
	{PORTB_ID, BIT12_MASK}, //	READY_BUTTON,           ID = 81
	{PORTB_ID, BIT13_MASK}, //	PEDAL_START,			ID = 82
	{PORTB_ID, BIT14_MASK}, //	RIGHT_LOCK_PEDAL,       ID = 83
	{PORTB_ID, BIT15_MASK}, //	LEFT_LOCK_PEDAL,        ID = 84
	{PORTB_ID, BIT3_MASK},	//	PH_HOME_SENSOR,         ID = 85
	{LATE_ID, BIT0_MASK},	//	TENSION_PWM,            ID = 86
	{LATD_ID, BIT15_MASK},	//	IOP_AIR_VALVE_ON,       ID = 87
	{LATG_ID, BIT14_MASK},	//	CLAMP_PF_MOTOR_ON,      ID = 88
	{LATG_ID, BIT15_MASK},	// 	PGOE(LEVEL_SHIFT_IC_EN) ID = 89
	{LATD_ID, BIT2_MASK},	// 	RESET_THREAD_BREAK      ID = 90
	{PORTB_ID, BIT2_MASK},	// 	LOWER_COVER_CLOSED      ID = 91
	{PORTB_ID, BIT1_MASK},	// 	UPPER_COVER_CLOSED      ID = 92
	{PORTD_ID, BIT11_MASK}, // 	UPPER_COVER_OPENED      ID = 93
	{PORTD_ID, BIT4_MASK},	// 	COVER_OPEN_ALARM        ID = 94
	{LATA_ID, BIT2_MASK},	// 	RESET_RFID  			ID = 95
	{PORTD_ID, BIT3_MASK},	//  THREAD_BREAK_INPUT     	ID = 96
	{PORTC_ID, BIT1_MASK}	//	BC_DETECT_SENSOR        ID = 97
};

#endif	// _RTC_IODEF_H_

