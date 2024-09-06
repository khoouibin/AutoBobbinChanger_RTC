#ifndef _BL_USB_H
#define _BL_USB_H

#include "usb_device.h"
#include "usb_device_hid.h"
#include "Ons_General.h"

#define TRANSACTION_SIZE 8 // row,Transaction
#define MESSAGE_MAX 64     // column
#define MSG_DATA_SIZE 62
#define ADDR_MASK 0x00ffffff
#define BL_USB_CMD_ID_OFFSET 0x40
#define CONFIG_ADDR_HLB 0xF8
#define CONFIG_ADDR_LLB_MIN 0x04
#define CONFIG_ADDR_LLB_MAX 0x12

#define FLASH_ADDR_GEN_START 0x000000
#define FLASH_ADDR_GEN_END   0x055800
#define FLASH_ADDR_AUX_START 0x7FC000
#define FLASH_ADDR_AUX_END   0x800000

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
	unsigned char Buff[TRANSACTION_SIZE][MESSAGE_MAX]; //[row][column]
	unsigned char MsgSize[TRANSACTION_SIZE];
} USB_Transaction_State_t;

enum Aux_Bootloader_Cmd
{
	AuxBL_NOP = 0x00,
	AuxBL_Negative_Response = 0x7f,
};
// protocol:
//                               byte0  byte1 byte2 byte3     byte4      byte5.....
//   Host->Client(cmd):          cmd_id AddrL AddrM AddrH     size       data[n]
//   Client->Host(feedback):     offset AddrL AddrM AddrH     size       data[n]
// 1.AuxBL_NOP                   0x00   0xrr  0xss  0xtt      0
//   feedback                    0x40   0xrr  0xss  0xtt+0x40 0
// 2.AuxBL_Read_SegmentChecksum  0x01   0xff  0xff  0xff      1          0/1    0:general segment, 1:auxiliary segment.
//   feedback                    0x41   0x00  0x00  0x00      2          chksumL  chksumH
// 3.AuxBL_Clear_GenSeg_Flash    0x02   0xff  0xff  0xff      0
//   feedback                    0x42   0x00  0x00  0x00      0
// 4.AuxBL_Read_PIC_FlashMemory  0x05   0xrr  0xss  0xtt      16~32
//   feedback                    0x45   0xrr  0xss  0xtt      16~32      data[0]...data[size-1]
// 5.AuxBL_Write_PIC_FlashMemory 0x06   0xuu  0xvv  0xww      16~32      data[0]...data[size-1]
//   feedback                    0x46   0x00  0x00  0x00      write_size fbk_chksum
// 6.AuxBL_Read_ONS_VersionMsg   0x07   0xii  0xjj  0xkk      1          0/1    0:version number(18bytes), 1:version note(48bytes), addr:memory_offset.
//   feedback                    0x47   0xii  0xjj  0xkk      18~48      data[n] 
// 7.AuxBL_Read_PIC_Config       0x0a   0xii  0xjj  0xkk      1
//   feedback                    0x4a   0xii  0xjj  0xkk      1          data[0]
// 8.AuxBL_Write_PIC_Config      0x0b   0xii  0xjj  0xkk      1          data[0]
//   feedback                    0x4b   0x00  0x00  0x00      0
// 9.AuxBL_Reset_PIC             0x10   0xff  0xff  0xff      0
//   feedback                    0x50   0x00  0x00  0x00      0

enum USB_Task_NRC
{
	AuxBL_POSITIVE = 0x00,
	AuxBL_NRC_SIZE_ZERO = 0x81,
	AuxBL_NRC_SIZE_nZERO = 0x82,
	AuxBL_NRC_SIZE_EXCEED = 0x83,
	AuxBL_NRC_ADDR_OUTRANGE = 0x84,
	AuxBL_NRC_DATA_OUTRANGE = 0x85,
	AuxBL_NRC_CMD_NOT_FOUND = 0x86,
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
{  // id - 28
	U16_t RTC_Base;
	U16_t RTC_Major;
	U16_t RTC_Minor;
	U16_t MsgProt_Base;
	U16_t MsgProt_Major;
	U16_t MsgProt_Minor;
	U16_t BL_Base;
	U16_t BL_Major;
	U16_t BL_Minor;
}VersionInfo_t;

void USB_DeviceInitialize(void);
void USB_TransStateInit(void);

char BL_USB_PushRx_Data(void);

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

// void BL_USB_RxMutex_Set(void);
// void BL_USB_RxMutex_Clr(void);
// bool BL_USB_RxMutex_Get(void);
bool USB_Msg_Parser(USB_Task_msg_t* task_msg);
unsigned char Is_USB_Msg_NegResponse(USB_Task_msg_t* task_msg);
//char USB_NegResp(unsigned char cmd_fbk, unsigned char neg_code);
char USB_NegResp(unsigned char cmd_id, unsigned char neg_code, char* strmsg);

char Cal_Segment_Checksum(unsigned char select_seg, unsigned int* checksum);

int BL_Set_ConfigReg_RSTPRI(unsigned long addr, unsigned char config_reg);
int BL_Get_ConfigReg_RSTPRI(unsigned long *addr, unsigned char *config_reg);
int BL_Set_ConfigReg_GSS(unsigned long addr, unsigned char config_reg);
int BL_Get_ConfigReg_GSS(unsigned long *addr, unsigned char *config_reg);

#endif
