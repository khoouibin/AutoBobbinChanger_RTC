#ifndef _BL_USB_H
#define _BL_USB_H

#include "usb_device.h"
#include "usb_device_hid.h"
#include "Ons_General.h"
#include "RTC_PulseControl.h"

#define TRANSACTION_SIZE 8 // row,Transaction
#define MSG_MAX_SIZE 64
#define MSG_DATA_SIZE 60
#define MSG_ENTITY_MAX_PACK_SIZE 30
#define MSG_ENTITY_TABLE_REPLY_SIZE 22
#define ADDR_MASK 0x00ffffff
#define USB_CMD_ID_OFFSET 0x40

typedef unsigned char *ptr_usb_msg_u8;
typedef unsigned char usb_msg_u8;
typedef struct
{
	bool Init;
	bool flag_ISR;
	bool Mutex; // check TxMutex evey 1ms.
	bool Stuck;
	char Ptr_buff;
	char Ptr_comp;
	unsigned char Buff[TRANSACTION_SIZE][MSG_MAX_SIZE]; //[row][column]
	unsigned char MsgSize[TRANSACTION_SIZE];
} USB_Transaction_State_t;

enum Protocol_Command
{
	Cmd_Echo = 0x00,
	Cmd_Reset = 0x01,
	Cmd_Profile = 0x02,
	Cmd_Log = 0x03,
	Cmd_EntityTable = 0x04,
	Cmd_EntityPack = 0x05,
	Cmd_Z_PulseGen = 0x06,
	Cmd_X_PulseGen = 0x07,
	Cmd_ControlModeSwitch = 0x10,
	Cmd_HomeParts = 0x11,
	Cmd_LECPA_100_Control = 0x12,
	Cmd_LECPA_30_Control = 0x13,
	Cmd_MAX,//0x3f
};

enum Protocol_PositiveResponse
{
	RespPositive_Echo = 0x40,
	RespPositive_Reset = 0x41,
	RespPositive_Profile = 0x42,
	RespPositive_Log = 0x43,
	RespPositive_EntityTable = 0x44,
	RespPositive_EntityPack = 0x45,
	RespPositive_Z_PulseGen = 0x46,
	RespPositive_X_PulseGen = 0x47,
	RespPositive_ControlModeSwitch = 0x50,
	RespPositive_HomeParts = 0x51,
	RespPositive_LECPA_100_Control = 0x52,
	RespPositive_LECPA_30_Control = 0x53,
};

enum Protocol_NegativeResponse
{
	RespNeg = 0x7f,
};

enum Protocol_Dummy
{
	Dummy_00 = 0x00,
	Dummy_ff = 0xff,
	Dummy_0000 = 0x0000,
	Dummy_ffff = 0xffff,
};

enum Echo_SubFunc
{
	SubFunc_55 = 0x55,
	SubFunc_AA = 0xAA,
};

enum Reset_SubFunc
{
	SubFunc_reset_mcu = 1,
	SubFunc_reset_usb = 2,
	SubFunc_reset_uart = 3,
	SubFunc_reset_max,
};

enum Profile_SubFunc
{
	SubFunc_profile_get = 1,
	SubFunc_profile_set = 2,
	SubFunc_profile_max,
};

enum Log_SubFunc
{
	SubFunc_log_level_get = 1,
	SubFunc_log_level_set = 2,
	SubFunc_log_msg_reply = 3,
	SubFunc_log_max,
};

enum EntityTable_SubFunc
{
	SubFunc_table_get_off = 0,
	SubFunc_table_get_instant = 1,
	SubFunc_table_get_period = 2,
	SubFunc_table_get_changed = 3,
	SubFunc_entitytable_max,
};

enum EntityPack_SubFunc
{
	SubFunc_pack_get = 1,
	SubFunc_pack_set = 2,
	SubFunc_entitypack_max,
};

enum Z_PulseGen_SubFunc
{
	SubFunc_z_pulse_gen_off = 0,
	SubFunc_z_pulse_gen_rpm = 1,
	SubFunc_z_pulse_gen_pwm = 2,
	SubFunc_z_pulse_turns_reply = 11,
	SubFunc_z_pulse_gen_max,
};

enum X_PulseGen_SubFunc
{
	SubFunc_x_pulsemode_run_stop = 0,
	SubFunc_x_pulsemode_trapezoid = 1,
	SubFunc_x_pulsemode_max,
};

enum ControlModeSwitch_SubFunc
{
	SubFunc_controlmode_uninit = 0,
	SubFunc_controlmode_ready = 1,
	SubFunc_controlmode_home = 2,
	SubFunc_controlmode_diagnosis = 3,
	SubFunc_controlmode_max,
};

enum HomeParts_SubFunc
{
	SubFunc_home_WinderStepper = 0,
	SubFunc_home_LECPA_30 = 1,
	SubFunc_home_LECPA_100 = 2,
	SubFunc_home_WinderStepper_polling_reply = 10,
	SubFunc_home_LECPA_30_polling_reply = 11,
	SubFunc_home_LECPA_100_polling_reply = 12,
	SubFunc_home_max,
};

enum HomeParts_SubCmd
{
	SubCmd_Abort = 0,
	SubCmd_Start = 1,
	SubCmd_max,
};

enum LECPA_SubFunc
{
	SubFunc_LECPA_Mov_OrgPoint = 1, // without subcmd
	SubFunc_LECPA_Mov_MinPoint = 2, // without subcmd
	SubFunc_LECPA_Mov_MaxPoint = 3, // without subcmd
	SubFunc_LECPA_Mov_AnyPoint = 4, // with subcmd
	SubFunc_LECPA_Set_ServoOn = 5,	   // without subcmd
	SubFunc_LECPA_Set_ServoOff = 6,	   // without subcmd
	SubFunc_LECPA_Mov_OrgPoint_polling_reply = 11,
	SubFunc_LECPA_Mov_MinPoint_polling_reply = 12,
	SubFunc_LECPA_Mov_MaxPoint_polling_reply = 13,
	SubFunc_LECPA_Mov_AnyPoint_polling_reply = 14,
	SubFunc_LECPA_Set_ServoOn_polling_reply = 15,
	SubFunc_LECPA_Set_ServoOff_polling_reply = 16,
	SubFunc_LECPA_max,
};


enum Reponse_Code
{
	POSITIVE_CODE = 0x00,
	NRC_SIZE_ZERO = 0x81,
	NRC_SIZE_nZERO = 0x82,
	NRC_SIZE_EXCEED = 0x83,
	NRC_SUBFUNC_OUTRANGE = 0x84,
	NRC_DATA_OUTRANGE = 0x85,
	NRC_CMD_NOT_FOUND = 0x86,
	NRC_ILLEGAL_RTC_MODE = 0x87,
};

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char argv_0;
	unsigned char argv_1;
	unsigned char data[MSG_DATA_SIZE];
} USB_Task_msg_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	unsigned char argv_0;
	unsigned char argv_1;
	unsigned char data[MSG_DATA_SIZE];
} USB_TaskResp_msg_t;

typedef struct
{
	unsigned char resp_id;
	unsigned char cmd_id;
	unsigned char neg_code;
	unsigned char neg_argv;
	unsigned char data[MSG_DATA_SIZE];
} USB_NegResponse_msg_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char data[2];
} usb_msg_echo_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned short delay_time;
} usb_msg_reset_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char profile_number;
	unsigned char ignore;
	unsigned char data[MSG_DATA_SIZE];
} usb_msg_profile_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char set_level;
	unsigned char ignore;
	unsigned char data[MSG_DATA_SIZE];
} usb_msg_log_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	unsigned short log_counter;
	unsigned char data[MSG_DATA_SIZE];
} usb_msg_log_reply_t;

// template:
// SubFunc_table_get_instant: 0x04, 0x01, 0x00, 22;
// reply:                     0x44, 0x01, 0x00, 22, ...(data = 22 bytes)
// SubFunc_table_get_period:  0x04, 0x02, msec, 22;
// reply:                     0x44, 0x02, msec, 22, ...(data = 22 bytes)
// SubFunc_table_get_changed: 0x04, 0x03, 0x00, 22;
// reply:                     0x44, 0x03, 0x00, 22, ...(data = 22 bytes)
// SubFunc_table_get_off:     0x04, 0x04, 0x00, 0;
// reply:                     0x44, 0x04, 0x00, 0,

// SubFunc_pack_get:          0x05, 0x01, 0x00, entity_pack_size;
// reply:                     0x45, 0x01, 0x00, entity_pack_size, ...(data = n entity_pack)
// SubFunc_pack_set:          0x05, 0x02, 0x00, entity_pack_size, ...(data = n entity_pack);
// reply:                     0x45, 0x02, 0x00, 0;

typedef struct
{
	unsigned char entity_name;
	unsigned char entity_value;
} ioentity_pack_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char reply_period;
	unsigned char table_size;
	unsigned char data[MSG_ENTITY_TABLE_REPLY_SIZE];
} usb_msg_entitytable_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	unsigned char reply_period;
	unsigned char table_size;
	unsigned char data[MSG_ENTITY_TABLE_REPLY_SIZE];
} usb_msg_entitytable_reply_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char pack_size;
	unsigned char argv;
	ioentity_pack_t entity_pack[MSG_ENTITY_MAX_PACK_SIZE];
} usb_msg_entity_pack_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	unsigned char pack_size;
	unsigned char argv;
	ioentity_pack_t entity_pack[MSG_ENTITY_MAX_PACK_SIZE];
} usb_msg_entity_pack_reply_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char argv_0;
	unsigned char argv_1;
	z_pulse_width_modulation_t z_pwm_value;
	z_pulse_rpm_enum_t z_rpm_value;
} usb_msg_z_pulse_gen_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	unsigned short turns;
} usb_msg_z_pulse_gen_reply_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned short steps;
	OCx_src_t x_sequence;
} usb_msg_x_pulse_gen_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	unsigned char argv_0;
	unsigned char argv_1;
} usb_msg_x_pulse_gen_reply_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char argv_0;
	unsigned char argv_1;
} usb_msg_control_mode_switch_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	unsigned char control_status;
	unsigned char switch_status;
} usb_msg_control_mode_switch_reply_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char sub_cmd;
	unsigned char argv_1;
} usb_msg_home_parts_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	unsigned char home_routine;
	unsigned char home_state;
} usb_msg_home_parts_reply_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	signed short position_cmd;
	OCx_src_t x_sequence;
} usb_msg_lecpa_drive_cmd_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	char drive_state;
	char drive_stage;
} usb_msg_lecpa_drive_cmd_reply_t;

void USB_DeviceInitialize(void);
void USB_TransStateInit(void);

char USB_Msg_From_RxBuffer(usb_msg_u8 *msg_cmd, unsigned char *msg_size);
char USB_Msg_To_TxBulkBuffer(ptr_usb_msg_u8 send_msg, unsigned char msg_size);
char USB_TxBulkBuffer_To_Bus(void);
char USB_RxBulkBuffer_Get_From_Bus(void);

void BL_USB_Tx_1mISR_Set(void);
void BL_USB_Tx_1mISR_Clr(void);
bool BL_USB_Tx_1mISR_Get(void);

void BL_USB_Rx_4mISR_Set(void);
void BL_USB_Rx_4mISR_Clr(void);
bool BL_USB_Rx_4mISR_Get(void);

void BL_USB_TxMutex_Set(void);
void BL_USB_TxMutex_Clr(void);
bool BL_USB_TxMutex_Get(void);

bool USB_Msg_Parser(USB_Task_msg_t* task_msg);
unsigned char Is_USB_Msg_NegResponse(USB_Task_msg_t* task_msg);
char USB_NegResp(unsigned char cmd_id, unsigned char neg_code, char* strmsg);

#endif
