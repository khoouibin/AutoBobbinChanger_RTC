// ---------------------------------------------------------------------------
//  Filename: Msg_Prot.h
//  Created by: Nissim Avichail
//  Date:  01/07/15
//  Orisol (c)
// ---------------------------------------------------------------------------

#ifndef _MSG_PROT_H_
#define _MSG_PROT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define 	MSG_PROT_GEN_MSG_MAX_DATA_SIZE      48      //Max Message size - header size
    
#define 	TEST_HOST_FILL_SIZE					32		// = general message data size - host test message header size

#define		TEST_RTC_FILL_SIZE					20		//general message data size - RTC test message header size
    
#define		MSG_PROT_RTC_ERROR_DATA_SIZE			30
    
//-----------------------------------
// Message Identification definition
//-----------------------------------

#define			NO_MESSAGE_ID							0
#define			MSG_PROT_TEST_USB_HOST_TO_RTC			1
#define			MSG_PROT_TEST_USB_RTC_TO_HOST			2
#define         MSG_PROT_PING_PONG_MSG					3
    
#define			MSG_PROT_RTC_RESET_CMD					79

//RTC State machine
//#define RTC_IN_READY_STATE			0
//#define RTC_IN_RUN_STATE			1
#define MSG_PROT_RTC_DEFAULT_STITCH_ID		(-1)

//Msg_Port Module Error Code Definition
#define MSG_PROT_SUCCESS					0
#define MSG_PROT_ERROR_GET_MESSAGE			1
#define MSG_PROT_ERROR_SIZE_MESSAGE			2
#define MSG_PROT_ERROR_CHECKSUM_MESSAGE   	3
#define MSG_PROT_UNKNOWN_STATE_ERROR        4
#define MSG_PROT_FULL_STC_BUFF_ERROR        5
#define MSG_PROT_UNKNOWN_EVENT_ERROR        6
#define MSG_PROT_RUN_ERROR                  7
#define MSG_PROT_SH_ERROR					8
#define MSG_PROT_PF_APF_ERROR               9
#define MSG_PROT_SERVO_CLAMP_ERROR          10
//Msg_Port Module Error Code Definition


typedef enum
{
	RTC_INIT_SUCCESSFULLY		=	0,
	RTC_INIT_FAIL				=	1
}Msg_Prot_RTC_Init_Status_t;

//RTC State machine
typedef enum
{
	RTC_IN_READY_STATE				= 0,
	RTC_IN_RUN_STATE				= 1,
	RTC_HOMING_IN_READY_STATE		= 2,
	RTC_HOMING_IN_Z_PHASE			= 3,
    RTC_HOMING_IN_APF_PHASE			= 4,        
	RTC_HOMING_IN_XY_PHASE_1		= 5,
	RTC_HOMING_IN_XY_PHASE_2		= 6,
	RTC_DIAG_ACT_IN_READY_STATE		= 7,
	RTC_DIAG_ACT_IN_LOCK_AT_INDEX	= 8,
	RTC_DIAG_ACT_IN_X_PHASE			= 9,
	RTC_DIAG_ACT_IN_Y_PHASE			= 10
}RTC_State_t;

// Message structure
typedef struct
{
	UINT_16  	Id;						// Message ID according to the Global USB Message list.
	INT_16	  	Size;					// Total size of message include the header size.
	UINT_32		Serial_Number;			// Message S/N this number is  for each message sent.
	UINT_32   	Time_Stamp;				// Current system time in millisecond.
}Message_Header_t;

typedef struct
{
	Message_Header_t	Message_Header;								// Generic message header
	ONS_BYTE			Data[MSG_PROT_GEN_MSG_MAX_DATA_SIZE];		// Generic Data array (maximum rx data message size)
}Generic_Message_t;

typedef ONS_BYTE Rcv_Msg_Data_t[MSG_PROT_GEN_MSG_MAX_DATA_SIZE];

typedef struct
{
	INT_16		Wait;				// Waiting time until start of Message execution (in millisecond)
	INT_16		Num_Of_Msg;			// Number of message RTC will send to host as replay
	INT_16		Delta_Time;			// Time delay between messages (in millisecond)
	INT_16		Return_Size;	 	// Size of RTC replay message data
	INT_16	 	Fill_Size;			// data size in Data_Element
	INT_16		Dummy;				// for packing use
	UINT_32		SN;					// Serial Number (same as in header)
	UINT_32		Checksum;			// Checksum calculation include all fields exclude the checksum itself.
}Msg_Prot_Host_Tst_Msg_info_t;


typedef struct
{
		Message_Header_t			  Msg_Hdr;						// Generic message type
		Msg_Prot_Host_Tst_Msg_info_t  Test_msg_info;				// Host to RTC test Message info
		ONS_BYTE				      Fill[TEST_HOST_FILL_SIZE];		// buffer for Message Data (maximum size)
}Msg_Prot_Host_Tst_Msg_t;

typedef struct
{
	UINT_32		Total_Rcv_Msg;				// total Number of received messages
	UINT_32		Total_Rcv_data_size;		// total size of data received
	UINT_32		Total_Rcv_Lost_Msgs;		// total number of messages received with error
	UINT_32		Total_Tx_Lost_Msgs;		    // total number of messages Tx with error
	UINT_32		Total_Tx_Msgs;		        // total number of messages Tx
}Msg_Prot_Test_Msg_Status_t;

typedef struct
{
	UINT_32						Origin_Msg_SN;		//contain the S/N received from the host (the same on that this message generated from)
	Msg_Prot_Test_Msg_Status_t	Status;				// the status data from RTC
	INT_16 						Fill_Size;			// data size in Data_Element
	INT_16 						Dummy;			    // dummy
    UINT_32                     Busy_Time;          //Busy loop time
	UINT_32						Checksum;			// calculated data checksum
}Msg_Prot_RTC_Tst_Hdr_t;

typedef struct
{
	Message_Header_t			Msg_Hdr;
	Msg_Prot_RTC_Tst_Hdr_t  	Tst_msg_info;				// Host to RTC test Message info
	ONS_BYTE				   	Fill[TEST_RTC_FILL_SIZE];	// buffer for Message Data (maximum size)

}Msg_Prot_RTC_Tst_Msg_t;
 
typedef enum
{
	HOST_PING						= 0,
	RTC_PING						= 1
}Ping_Initiator_t;   
    
//------------------
//   Prototypes
//------------------

// Init Message
void	Msg_Prot_Init (void);

// Send Message
int		Msg_Prot_Send_Message (ONS_BYTE* Mgs_Data_Buff, unsigned int Msg_Id, unsigned int Msg_Data_Size);

// Receive Message
int		Msg_Prot_Receive_Next_Message (Rcv_Msg_Data_t Data_Buff, unsigned int* Msg_Id, unsigned int* New_Msg);
void	Msg_Prot_Get_Last_Msg_Hdr (int* Msg_Id,unsigned long* Msg_SN, int* Msg_Size, unsigned long* Msg_Time );

// For Testing
int 	Msg_Prot_Pack_n_Tx_Host_Tst_Msg(int* Wait, int* Num_Of_Msg, int*  Delta_Time, int* Return_Size,int* Fill_Size,unsigned long* Tx_Time);
int 	Msg_Prot_Unpack_Host_Test_Message(ONS_BYTE* Msg_Buff, Msg_Prot_Host_Tst_Msg_info_t* Tst_Info);

int 	Msg_Prot_Pack_n_Tx_RTC_Tst_Msg(unsigned long Origin_Msg_SN, int Return_Size);
int 	Msg_Prot_Unpack_RTC_Test_Message(ONS_BYTE* Msg_Buff, Msg_Prot_RTC_Tst_Hdr_t* Tst_Info);

#ifdef __cplusplus
}
#endif
#endif
