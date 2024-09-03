// ---------------------------------------------------------------------------
//  FileName: USBComm.h
//  Created by: Nissim Avichail
//  Date: 23/06/2015 
//  Orisol (c)
// ---------------------------------------------------------------------------

#ifndef _USBCOMM_H_
#define _USBCOMM_H_

#ifdef __cplusplus
extern "C" {
#endif


#define USBCOMM_VERSION				"USBCOMM  0.0.2"

#define USBCOMM_MAX_TX_MESSAGE_SIZE  64		//The maximum size for transmit massage in ONS_BYTEs.
#define USBCOMM_TX_BUFFER_SIZE       5		//The maximum size for transmit buffer.
#define USBCOMM_MAX_RX_MESSAGE_SIZE  64		//The maximum size for receive massage in ONS_BYTEs.
#define USBCOMM_RX_BUFFER_SIZE       5		//The maximum size for receive buffer.

//USBComm Module Error Code Definition
typedef enum
{
	 USBCOMM_SUCCESS						=	0,
	 USBCOMM_ERROR_BUFFER_SIZE				=	1,
	 USBCOMM_ERROR_BUFFER_FULL				=	2,
	 USBCOMM_ERROR_MESSAGE_SIZE				=	3,
	 USBCOMM_ERROR_MODULE_IN_ERROR_STATE	=	4,
	 USBCOMM_ERROR_CAN_NOT_OPEN_DEVICE  	=	5,
	 USBCOMM_ERROR_RETRY       				=	6,
	 USBCOMM_ERROR_MODULE_NOT_READY 	    =	7
}
	USBComm_Error_Code_t;

#define USBCOMM_ERROR_CONNECTION_FAIL  0xDEAD


typedef enum
{
	 USBCOMM_INIT_STATE					=	0,
	 USBCOMM_READY_STATE				=	1,
	 USBCOMM_ERROR_STATE				=	2
}
	USBComm_State_code_t;


typedef ONS_BYTE*	USBComm_Buffer_t;

typedef struct
{
	int				RX_Buffer_Full;					// flag to indicate buffer is full.
	unsigned long	RX_Current_Lost_Msgs;			// number of messages lost currently.
	unsigned long	RX_Total_Lost_Msgs;				// total number of messages lost since last session started
	unsigned long	RX_Total_Msgs;					// total number of messages received from USB (including the lost ones).
	unsigned long	Rx_Total_Data_Size;				// total size of received data in ONS_BYTEs
	unsigned long	Rx_Num_Of_Retry;				// total number of time entered  to the retry phase in Rx.
	int				USBComm_Module_State;			// flag to indicate module one of this modes (USBComm_State_code_t): INIT,READY,ERROR
	int				TX_Buffer_Epmty;				// flag to indicate buffer is Empty.
	unsigned long	TX_Current_Buffer_Full_Msgs;	// number of Tx buffer full error currently.
	unsigned long	TX_Total_Buffer_Full_Msgs;		// total number of Tx buffer full error since last session started
	unsigned long	TX_Total_Msgs;					// total number of messages transmitted via USB.
	unsigned long	Tx_Total_Data_Size;				// total size of transmitted data in ONS_BYTEs
	unsigned long	Tx_Total_Buffer_empty;			// total number of accessing empty buffer
	unsigned long	Tx_Num_Of_Retry;				// total number of time entered  to the retry phase in Tx.

}
	USBComm_Status_t;

// ----------------
//   Prototypes
//------------------

int  USBComm_Init 				( void );
int  USBComm_Termination 		( void );
int  USBComm_Send_Message 		( USBComm_Buffer_t Message , int Message_Size );
int  USBComm_Is_Tx_Buffer_Full 	( void );
int  USBComm_Get_Message 		( USBComm_Buffer_t Message , int Buffer_Size , int* New_Message_Received, int* Message_Size );
int  USBComm_Peek_Message		( USBComm_Buffer_t* Message , unsigned int* New_Message_Received, unsigned int* Message_Size );
int  USBComm_Is_Rx_Buffer_Empty ( void );
void USBComm_Get_Status 		( USBComm_Status_t* Status );
int  USBComm_Remove_Rx_Message  (void);
void USBComm_Get_Module_State	( int* Module_State );
void		USBComm_Set_Init_Complete	( void );
const char* USBComm_Get_Version			( void );

void  USBComm_Rx_Int_Main ( void );
void  USBComm_Tx_Int_Main ( void );

#ifdef __cplusplus
}
#endif

#endif //_USBCOMM_H_
