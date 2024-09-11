#ifndef _BL_USB_H
#define _BL_USB_H

#include "usb_device.h"
#include "usb_device_hid.h"
#include "Ons_General.h"

#define TRANSACTION_SIZE 8 // row,Transaction
#define MSG_MAX_SIZE 64
#define MSG_DATA_SIZE 62
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
	Cmd_MAX,
};

enum Protocol_PositiveResponse
{
	RespPositive_Echo = 0x40,
	RespPositive_Reset = 0x41,
	RespPositive_Profile = 0x42,	
};

enum Protocol_NegativeResponse
{
	RespNeg = 0x7f,
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
	SubFunc_profile_read = 1,
	SubFunc_profile_write = 2,
	SubFunc_profile_max,
};

enum Reponse_Code
{
	POSITIVE_CODE = 0x00,
	NRC_SIZE_ZERO = 0x81,
	NRC_SIZE_nZERO = 0x82,
	NRC_SIZE_EXCEED = 0x83,
	NRC_ADDR_OUTRANGE = 0x84,
	NRC_DATA_OUTRANGE = 0x85,
	NRC_CMD_NOT_FOUND = 0x86,
};

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char data[MSG_DATA_SIZE];
} USB_Task_msg_t;

typedef struct
{
	unsigned char cmd_id_rep;
	unsigned char sub_func;
	unsigned char data[MSG_DATA_SIZE];
} USB_TaskResp_msg_t;

typedef struct
{
	unsigned char resp_id;
	unsigned char cmd_id;
	unsigned char neg_code;
	unsigned char ignore;
	unsigned char data[60];
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
	unsigned short  delay_time;
} usb_msg_reset_t;

typedef struct
{
	unsigned char cmd_id;
	unsigned char sub_func;
	unsigned char profile_number;
	unsigned char ignore;
	unsigned char data[60];
} usb_msg_profile_t;

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
