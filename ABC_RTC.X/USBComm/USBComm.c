// ---------------------------------------------------------------------------
//  Filename: USBComm.c
//  Created by: Nissim Avichail
//  Date:  23/06/15
//  Orisol (c) 
// ---------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Ons_General.h"

#include "Timers.h"

#include "USBComm.h"
#include "USBComm_Driver.h"

// -----------------
// Internal Define
// -----------------

#define TX_THREAD_SLEEP_IN_MILLISECONDS 2  //5 Milliseconds
#define RX_THREAD_SLEEP_IN_MILLISECONDS 1
// ---------------------
// internal Prototypes
// ---------------------

static int		USBComm_Init_USB( void );
#define TX_MAX_RETRIES 3

static int  	USBComm_Init_Threads ( void );
static void 	USBComm_Init_Buffers (void );
static int  	USBComm_Kill_Threads ( void );
static int  	USBComm_Terminate_USB ( void );
static int  	USBComm_Bulk_Transmit_Messages ( void );
static int		USBComm_Bulk_Receive_Messages( void );
static void  	USBComm_Reset_Tx_Buffer ( void );
static void  	USBComm_Reset_Rx_Buffer ( void );

//---------------------
// internal variables
// --------------------
static	ONS_BYTE		USBComm_Rx_Buffer[USBCOMM_RX_BUFFER_SIZE][USBCOMM_MAX_RX_MESSAGE_SIZE];	// Buffer for incoming messages received from USB.
static	ONS_BYTE 		USBComm_Tx_Buffer[USBCOMM_TX_BUFFER_SIZE][USBCOMM_MAX_TX_MESSAGE_SIZE];	// Buffer for outgoing messages to transmit to USB.
static	USBComm_Status_t		Current_Status;									// Holds the current status of the Module.
static	int						Rx_Write_To_Buffer_Index;						// Index of the next free buffer entry.
static	int						Rx_Read_From_Buffer_Index;						// Index to the next message to read.
static	int						Tx_Write_To_Buffer_Index;						// Index of the next free buffer entry.
static	int						Tx_Read_From_Buffer_Index;						// Index to the next message to read.
static	int						Rx_Actual_Message_Size[USBCOMM_RX_BUFFER_SIZE];  //Hold the actual message size for each incoming message.
static	int						Tx_Actual_Message_Size[USBCOMM_TX_BUFFER_SIZE];  //Hold the actual message size for each outgoing message.
static 	ONS_BYTE				outgoing_message[USBCOMM_MAX_TX_MESSAGE_SIZE];
static  ONS_BYTE				incomming_message[USBCOMM_MAX_RX_MESSAGE_SIZE];

#ifdef ONS_HOST_LINUX

static pthread_t				USBComm_Init_Thread;							// The USB first initialization thread

#endif
#ifdef ONS_WIN
static	HANDLE USBComm_Init_Thread;
static  dw_USBComm_Init_Thread;
#endif // ONS_WIN

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Init
//
//  Purpose				: Initialization of the UUSBCommmodule
//
//  Inputs				: 
//
//  Outputs				: 
//						:
//  Returns				: Success or Error code
//
//  Description			: Activates the Module initialization sequence. 
//						: Host : call the USB initialization thread,
//						: transmit and receive threads initialization 
//						: and resets transmit and receive buffers.
//						: Note : All the initialization process error codes
//						: are critical errors
//
// ---------------------------------------------------------------------------
int  USBComm_Init ( void )
{
	int res = USBCOMM_SUCCESS;

	//Set the status to default values
	Current_Status.RX_Buffer_Full				= ONS_FALSE;
	Current_Status.RX_Current_Lost_Msgs			= 0;		
	Current_Status.RX_Total_Lost_Msgs			= 0;		
	Current_Status.RX_Total_Msgs				= 0;			 
	Current_Status.Rx_Total_Data_Size			= 0;				 
	Current_Status.USBComm_Module_State			= USBCOMM_INIT_STATE;
	Current_Status.TX_Buffer_Epmty				= ONS_TRUE;
	Current_Status.TX_Current_Buffer_Full_Msgs	= 0;
	Current_Status.TX_Total_Buffer_Full_Msgs	= 0;		
	Current_Status.TX_Total_Msgs				= 0;	
	Current_Status.Tx_Total_Data_Size			= 0;
	Current_Status.Tx_Num_Of_Retry				= 0;
	Current_Status.Rx_Num_Of_Retry				= 0;

	USBComm_Init_Buffers();

	res = USBComm_Init_Threads();
	if (res != USBCOMM_SUCCESS)
	{
		//TBD -define critical errors in libusb and in usbcom
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;		//failed to initialize the threads, set the status to Error mode.
		return res;
	}

	res = USBComm_Init_USB();
	if (res != USBCOMM_SUCCESS)
	{
		//TBD -define critical errors in libusb and in usbcom
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;		// USB initialization failed, set the status to Error mode.
		return res;
	}

	return res;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Termination
//
//  Purpose				: Handles the termination of the USBComm module.
//
//  Inputs				: 
//
//  Outputs				: 
//						
//  Returns				: Success or Error code
//
//  Description			: Releases the USB interface, close the USB device handler 
//						: and exit the libusb, resets transmit and receive buffers 
//						: and kill transmit and receive threads.
//						: Note : All the termination process error codes 
//						: are critical errors
// ---------------------------------------------------------------------------
int USBComm_Termination ( void )
{
	int res = USBCOMM_SUCCESS;

	res = USBComm_Kill_Threads();
	if (res != USBCOMM_SUCCESS)
	{
		//TBD -define critical errors in libusb and in usbcom
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;		//failed to kill the threads, set the status to Error mode.
		return res;
	}
	res = USBComm_Terminate_USB();
	if (res != USBCOMM_SUCCESS)
	{
		//TBD -define critical errors in libusb and in usbcom
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;		//failed to initialize the threads, set the status to Error mode.
		return res;
	}
	return res;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Send_Message
//
//  Purpose				: Handles the sending of a message
//
//  Inputs				: Message - pointer to TX message
//						: Message_Size - the TX message size 
//
//  Outputs				: 
//						
//  Returns				: Success or Error code
//
//  Description			: Verify that the transmit message size not exceed maximum 
//						: and that the buffer not full.
//						: After verification the message inserted into the the buffer
//						: the USBCome_Tx_Thread responsible for sending 
//						: the message through the USB bus.  
//
// ---------------------------------------------------------------------------
int USBComm_Send_Message (USBComm_Buffer_t 	Message ,
						 int 				Message_Size )
{
    int                 retries_counter;

    retries_counter = 0;
    
	// Verification stage
	//TBD -define critical errors in libusb and in usbcom
	if (Current_Status.USBComm_Module_State ==  USBCOMM_ERROR_STATE)
	{
		return USBCOMM_ERROR_MODULE_IN_ERROR_STATE;
	}

	if (Current_Status.USBComm_Module_State ==  USBCOMM_INIT_STATE)
	{
		return USBCOMM_ERROR_MODULE_NOT_READY;
	}

	if ( Message_Size > USBCOMM_MAX_TX_MESSAGE_SIZE)
	{
		return USBCOMM_ERROR_MESSAGE_SIZE;
	}

    while( !USBComm_Driver_Tx_Mutex_Lock() ) // lock Tx mutex
    {
        retries_counter++;
        
        if( retries_counter >= TX_MAX_RETRIES )
            return USBCOMM_ERROR_RETRY;
    }
    
	if (USBComm_Is_Tx_Buffer_Full())
	{
        Current_Status.TX_Total_Buffer_Full_Msgs++;
		Current_Status.TX_Current_Buffer_Full_Msgs++;
		USBComm_Driver_Tx_Mutex_UNLock();		// Unlock Tx mute
		return USBCOMM_ERROR_BUFFER_FULL;
	}

	Current_Status.TX_Current_Buffer_Full_Msgs = 0;

	memcpy ( USBComm_Tx_Buffer[Tx_Write_To_Buffer_Index] , Message , Message_Size);			//Access Buffer
	Tx_Actual_Message_Size[Tx_Write_To_Buffer_Index] = Message_Size;
	Current_Status.TX_Buffer_Epmty = ONS_FALSE;
	Tx_Write_To_Buffer_Index = ( Tx_Write_To_Buffer_Index + 1 ) % USBCOMM_TX_BUFFER_SIZE;	//Update cyclic Index


	USBComm_Driver_Tx_Mutex_UNLock();		// Unlock Tx mutex

	return USBCOMM_SUCCESS;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Is_Tx_Buffer_Full
//
//  Purpose				: Check if the transmit buffer full
//
//  Inputs				: 
//
//  Outputs				: 
//					
//  Returns				: ONS_TRUE or ONS_FALSE
//
//  Description			: Check if the transmit buffer full
//						: Note: before using the USBComm_Send_Message() function it is not mendetory to use this function,
//                      : the USBComm_Get_Message() check the buffer verify that the buffer not empty.
//
// ---------------------------------------------------------------------------
int USBComm_Is_Tx_Buffer_Full ( void )
{
	int res = ONS_FALSE;

	if (( Tx_Read_From_Buffer_Index == Tx_Write_To_Buffer_Index ) //check if read and write set to the same index
		&& ( Current_Status.TX_Buffer_Epmty == ONS_FALSE ))			  //and the buffer not empty

		res = ONS_TRUE;

	return res;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Get_Message
//
//  Purpose				: Get the received message. 
//
//  Inputs				: Buffer_Size  - size the area the give for the message
//
//  Outputs				: Message      					- the RX message pointer
//						: New_Message_Received			- ONS_TRUE if new message receive
//						: Message_Size - the real size if the incoming message  
//						 
//  Returns				: Success or Error code
//
//  Description			: Check if the there is a message in the Rx Buffer
//						: and copy it according to the given message size, 
//						: it also send the real size of the message
//
// ---------------------------------------------------------------------------
int USBComm_Get_Message ( USBComm_Buffer_t Message ,
						 int 			 Buffer_Size ,
						 int* 			 New_Message_Received,
						 int* 			 Message_Size )
{

	*New_Message_Received = ONS_FALSE;

	// Verification stage
	if (Current_Status.USBComm_Module_State ==  USBCOMM_ERROR_STATE)
	{
		return USBCOMM_ERROR_MODULE_IN_ERROR_STATE;
	}

	if (Current_Status.USBComm_Module_State ==  USBCOMM_INIT_STATE)
	{
		return USBCOMM_ERROR_MODULE_NOT_READY;
	}

#ifdef ONS_PC_PLAT
	USBComm_Driver_Rx_Mutex_Lock();		// lock Rx mutex
#else
	if( !USBComm_Driver_Rx_Mutex_Lock())		// lock Rx mutex
        return USBCOMM_SUCCESS;
#endif
	if (USBComm_Is_Rx_Buffer_Empty())
	{
		USBComm_Driver_Rx_Mutex_UNLock();		// unlock Rx mutex
		return USBCOMM_SUCCESS;						//No new message
	}

	*Message_Size = Rx_Actual_Message_Size[Rx_Read_From_Buffer_Index];
	if (*Message_Size > Buffer_Size)
	{
		USBComm_Driver_Rx_Mutex_UNLock();		// unlock Rx mutex
		return USBCOMM_ERROR_BUFFER_SIZE;
	}
	memcpy ( Message , USBComm_Rx_Buffer[Rx_Read_From_Buffer_Index] , *Message_Size);
	Rx_Actual_Message_Size[Rx_Read_From_Buffer_Index] = 0;	
	Rx_Read_From_Buffer_Index = (Rx_Read_From_Buffer_Index + 1) % USBCOMM_RX_BUFFER_SIZE;  //Update cyclic Index

	USBComm_Driver_Rx_Mutex_UNLock();		// unlock Rx mutex

	*New_Message_Received = ONS_TRUE;
	Current_Status.RX_Buffer_Full = ONS_FALSE;
	Current_Status.RX_Current_Lost_Msgs = 0;
	return USBCOMM_SUCCESS;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Peek_Message
//
//  Purpose				: Give an option to look at the Rx message in the buffer 
//
//  Inputs				: Message_View - pointer
//
//  Outputs				: Message_Size  - the new state of the component
//						:
//  Returns				: Success or Error code
//
//  Description			: Gives the pointer to next message from Rx Buffer without deleting it.
//						: The message stays in the buffer.
//						: The caller can only look at here(read only).
//
// ---------------------------------------------------------------------------
int USBComm_Peek_Message ( USBComm_Buffer_t* 	Message ,
						   unsigned int* 	 	New_Message_Received,
						   unsigned int* 	 	Message_Size )
{

	*New_Message_Received = ONS_FALSE;

	// Verification stage
	//TBD -define critical errors in libusb and in usbcom
	if (Current_Status.USBComm_Module_State ==  USBCOMM_ERROR_STATE)
	{
		return USBCOMM_ERROR_MODULE_IN_ERROR_STATE;
	}

	if (Current_Status.USBComm_Module_State ==  USBCOMM_INIT_STATE)
	{
		return USBCOMM_ERROR_MODULE_NOT_READY;
	}
#ifdef ONS_PC_PLAT
	USBComm_Driver_Rx_Mutex_Lock();		// lock Rx mutex
#else
	if ( !USBComm_Driver_Rx_Mutex_Lock() )		// lock Rx mutex
        return USBCOMM_SUCCESS;
#endif

	if (USBComm_Is_Rx_Buffer_Empty())
	{
		USBComm_Driver_Rx_Mutex_UNLock();		// unlock Rx mutex
		return USBCOMM_SUCCESS;						//No new message
	}

	*Message = (USBComm_Buffer_t)(USBComm_Rx_Buffer[Rx_Read_From_Buffer_Index]);


	*Message_Size = Rx_Actual_Message_Size[Rx_Read_From_Buffer_Index];
	if (*Message_Size > 0 )
	{
		*New_Message_Received = ONS_TRUE;
	}
#ifdef ONS_PC_PLAT   
	else
	{
		Log_Write_String_Msg(__FILE__, __func__, __LINE__, -1 , (char*)*Message);
	}
#endif
	USBComm_Driver_Rx_Mutex_UNLock();		// unlock Rx mutex


	return USBCOMM_SUCCESS;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Remove_Rx_Message
//
//  Purpose				: remove received message from buffer (if we already used it)
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: ONS_TRUE or ONS_FALSE
//
//  Description			: Check if the receiving buffer empty
//						: Note: before using the USBComm_Get_Message() function it is not mandatory to use this function,
//                      : the USBComm_Get_Message() check the buffer verify that the buffer not empty.
//
// ---------------------------------------------------------------------------
int USBComm_Remove_Rx_Message (void)
{
	if (Current_Status.USBComm_Module_State ==  USBCOMM_ERROR_STATE)
	{
		return USBCOMM_ERROR_MODULE_IN_ERROR_STATE;
	}

	if (Current_Status.USBComm_Module_State ==  USBCOMM_INIT_STATE)
	{
		return USBCOMM_ERROR_MODULE_NOT_READY;
	}

#ifdef ONS_PC_PLAT
	USBComm_Driver_Rx_Mutex_Lock();		// lock Rx mutex
#else
	if( !USBComm_Driver_Rx_Mutex_Lock() )		// lock Rx mutex
        return USBCOMM_SUCCESS;
#endif
	if (USBComm_Is_Rx_Buffer_Empty())
	{
		USBComm_Driver_Rx_Mutex_UNLock();		// unlock Rx mutex
		return USBCOMM_SUCCESS;						//No new message
	}

	Rx_Actual_Message_Size[Rx_Read_From_Buffer_Index] = 0;
	Rx_Read_From_Buffer_Index = (Rx_Read_From_Buffer_Index + 1) % USBCOMM_RX_BUFFER_SIZE;  //Update cyclic Index

	USBComm_Driver_Rx_Mutex_UNLock();		// unlock Rx mutex

	Current_Status.RX_Buffer_Full = ONS_FALSE;
	Current_Status.RX_Current_Lost_Msgs = 0;
	return USBCOMM_SUCCESS;
}


// --------------------------------------------------------------------------
//  Function name		: USBComm_Is_Rx_Buffer_Empty
//
//  Purpose				: Check if the receiving buffer empty
//
//  Inputs				: 
//
//  Outputs				: 
//
//  Returns				: ONS_TRUE or ONS_FALSE
//
//  Description			: Check if the receiving buffer empty
//						: Note: before using the USBComm_Get_Message() function it is not mandatory to use this function,
//                      : the USBComm_Get_Message() check the buffer verify that the buffer not empty.
//
// ---------------------------------------------------------------------------
int USBComm_Is_Rx_Buffer_Empty ( void )
{
	int res = ONS_FALSE;

	if (( Rx_Read_From_Buffer_Index == Rx_Write_To_Buffer_Index ) //check if read and write set to the same index
		&& ( Current_Status.RX_Buffer_Full == ONS_FALSE ))		   //and the buffer not empty

		res = ONS_TRUE;

	return res;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Get_Status
//
//  Purpose				: to give the module status
//
//  Inputs				: 
//
//  Outputs				: module status
//
//  Returns				: 
//
//  Description			: gives the module status structure
//
// ---------------------------------------------------------------------------
void USBComm_Get_Status( USBComm_Status_t *Status )
{
	*Status = Current_Status;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Get_Module_State
//
//  Purpose				: to give the module  state
//
//  Inputs				:
//
//  Outputs				: Module state
//
//  Returns				:
//
//  Description			: gives the module  state
//
// ---------------------------------------------------------------------------
void USBComm_Get_Module_State( int* Module_State )
{
	*Module_State = Current_Status.USBComm_Module_State;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Set_Init_Complete
//
//  Purpose				: Set the module state to ready
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				:
//
//  Description			: Set the module state to ready
//
// ---------------------------------------------------------------------------
void USBComm_Set_Init_Complete	( void )
{
	Current_Status.USBComm_Module_State = USBCOMM_READY_STATE;
}


// --------------------------------------------------------------------------
//  Function name		: USBComm_Init_USB
//
//  Purpose				: Initialize the USB library.
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: 
//
//  Description			: initialize the USB library, set the debug level, 
//						: detach the kernel drive of the device if needed, open the device port and get the device handler, using libusb library. 
//						: open the device port and get the device handler. 
//						: Check the current EP for given PID and VID
// ---------------------------------------------------------------------------
static int USBComm_Init_USB(void)
{
	int res;
    int rv = 0;

	res = USBComm_Driver_Init();									//initialize the library for the session we just declared
	if (res !=  0)
	{
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;
		return rv;
	}

	USBComm_Driver_Set_Debug(3);									//set debug level to 3, as suggested in the documentation

	res = USBComm_Driver_Open_USB_Device();
	if (res != USBCOMM_SUCCESS)
	{
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;
		return rv;
	}

	res = USBComm_Driver_Is_Kernel_Driver_Active();
	if (res == 1)													//find out if kernel driver is attached
		res = USBComm_Driver_Detach_Kernel_Driver();			    //detach kernel driver

	if (res != 0)
	{
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;
		return rv;
	}

	res = USBComm_Driver_Claim_Interface();					 //claim interface 0 (the first) of device (mine had just 1)

	if (res != 0)
	{
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;
		return rv;
	}
	return rv;	
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Init_Threads
//
//  Purpose				: Initialize the USB receive thread and USB transmit thread.
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: USBCOMM_SUCCESS or  error code
//
//  Description			: Initialize and activate receive thread and USB transmit thread.
//
// ---------------------------------------------------------------------------
static int USBComm_Init_Threads ( void )
{
	int res ;

	res = USBComm_Driver_Create_Rx_Thread ();
	if (res != 0)
	{
		return res;
	}

	res = USBComm_Driver_Create_Tx_Thread ();
	if (res != 0)
	{
		if (USBComm_Driver_Rx_Thread_kill() != 0)
		{
		}
		return res;
	}

	return USBCOMM_SUCCESS;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Init_Buffers
//
//  Purpose				: Initialize the receive and transmit buffer.
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: 
//
//  Description			: Initialize the receive and transmit buffer
//						: the head and tail pointer set to the start of the buffer.
//
// ---------------------------------------------------------------------------
static void USBComm_Init_Buffers  ( void )
{
	USBComm_Reset_Rx_Buffer();
	USBComm_Reset_Tx_Buffer();
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Kill_Threads
//
//  Purpose				: Terminate the receive and transmit buffer.
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: 
//
//  Description			: Terminate the receive and transmit buffer.
//
// ---------------------------------------------------------------------------
static int USBComm_Kill_Threads ( void )
{
	int res;

	res = USBComm_Driver_Rx_Thread_kill();
	if (res != 0)
	{
#ifdef ONS_PC_PLAT
		Debug_Print_Error_Message_With_Code("Error while killing USBComm_Rx_Thread ", res );
#endif
		return res;
	}

	res = USBComm_Driver_Tx_Thread_kill();
	if (res != 0)
	{
#ifdef ONS_PC_PLAT
		Debug_Print_Error_Message_With_Code("Error while killing USBComm_Tx_Thread ", res );
#endif 
		return res;
	}

	return USBCOMM_SUCCESS;
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Terminate_USB
//
//  Purpose				: Reset the Rx Buffer.
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: 
//
//  Description			: Reset the Rx Buffer, the head and tail pointer 
//						: set to the start of the Buffer.
//
// ---------------------------------------------------------------------------
static int USBComm_Terminate_USB(void)
{
	int res;

	res = USBComm_Driver_Release_Interface();	//release the claimed interface
	if (res != 0) 
	{
#ifdef ONS_PC_PLAT
		Debug_Print_Error_Message_With_Code( "Error while releasing the interface ", res );
#endif
		return res;
	}
#ifndef ONS_IP_WIN
	USBComm_Driver_Claim_Interface();		//close the USB device
#endif
	USBComm_Driver_Close_USB_Device ();		// exit USB library

	return USBCOMM_SUCCESS;


}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Bulk_Transmit_Messages
//
//  Purpose				: Send massage to the USB bus.
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: 
//
//  Description			: Send Tx message from the TX buffer, if exist, to the USB bus
//						: in bulk mode.
//
// ---------------------------------------------------------------------------
static int USBComm_Bulk_Transmit_Messages(void)
{
	int		message_size;
	int		res;
	int		actual;

//	TMP_Into_Transmit++;
	//TBD -define critical errors in libusb and in usbcom
	if (Current_Status.USBComm_Module_State ==  USBCOMM_ERROR_STATE)
	{
		return USBCOMM_ERROR_MODULE_IN_ERROR_STATE;
	}

	if (Current_Status.USBComm_Module_State ==  USBCOMM_INIT_STATE)
	{
		return USBCOMM_ERROR_MODULE_NOT_READY;
	}
#ifdef ONS_PC_PLAT
	USBComm_Driver_Tx_Mutex_Lock();		// lock Tx mutex
#else
	if( !USBComm_Driver_Tx_Mutex_Lock() )		// lock Tx mutex
        return USBCOMM_SUCCESS;
#endif
	memcpy(outgoing_message,USBComm_Tx_Buffer[Tx_Read_From_Buffer_Index], Tx_Actual_Message_Size[Tx_Read_From_Buffer_Index]);

	message_size = Tx_Actual_Message_Size[Tx_Read_From_Buffer_Index];

	USBComm_Driver_Tx_Mutex_UNLock();			// unlock Tx mutex

	//Tmp_b_send_msg_
	//Transmit Message to USB bus 
	res = USBComm_Driver_Bulk_Send_Message (outgoing_message, message_size, &actual);

	if (res == USBCOMM_ERROR_RETRY)
	{
		Current_Status.Tx_Num_Of_Retry++;
		return res;
	}
#ifdef ONS_PC_PLAT   
	USBComm_Driver_Tx_Mutex_Lock();		// lock Tx mutex - update the buffer index only if no retry needed
#else
	if( !USBComm_Driver_Tx_Mutex_Lock())		// lock Tx mutex - update the buffer index only if no retry needed
        return res;
#endif
	Tx_Read_From_Buffer_Index = (Tx_Read_From_Buffer_Index + 1) % USBCOMM_TX_BUFFER_SIZE;		//Update cyclic Index
	if (Tx_Read_From_Buffer_Index == Tx_Write_To_Buffer_Index)
	{
		Current_Status.TX_Buffer_Epmty = ONS_TRUE;
	}

	USBComm_Driver_Tx_Mutex_UNLock();			// unlock Tx mutex

	if (res != 0)
	{
#ifdef ONS_PC_PLAT          
		Debug_Print_Error_Message_With_Code("Error in bulk out transition ", res );
		Log_Write_Msg(__FILE__, __func__, __LINE__, res, Tx_Read_From_Buffer_Index, Current_Status.TX_Total_Msgs, 0,0,0,0);
#endif
		//TBD -define critical errors in libusb and in usbcom
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;
		return res;
	}

	if (actual != message_size)
	{
		res = USBCOMM_ERROR_MESSAGE_SIZE;
#ifdef ONS_PC_PLAT  		
		Debug_Print_Error_Message_With_Code("Error in actual size of transmitted message ", res );
		Log_Write_Msg(__FILE__, __func__, __LINE__, res, Tx_Read_From_Buffer_Index, Current_Status.TX_Total_Msgs, message_size,actual,0,0);
#endif
	}
	else
	{
		res =USBCOMM_SUCCESS;
	}

	Current_Status.TX_Total_Msgs++;
	Current_Status.Tx_Total_Data_Size += actual;
	return res;

}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Bulk_Recive_Messages
//
//  Purpose				: Send massage to the USB bus.
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: 
//
//  Description			: Receive Rx message from USB bus in bulk mode
//						: and insert it to the RX buffer.
//
// ---------------------------------------------------------------------------
static int USBComm_Bulk_Receive_Messages( void )
{
	int		res;
	int		actual;

	//TBD -define critical errors in libusb and in usbcom
	if (Current_Status.USBComm_Module_State ==  USBCOMM_ERROR_STATE)
	{
		return USBCOMM_ERROR_MODULE_IN_ERROR_STATE;
	}

	if (Current_Status.USBComm_Module_State ==  USBCOMM_INIT_STATE)
	{
		return USBCOMM_ERROR_MODULE_NOT_READY;
	}


	if (Current_Status.RX_Buffer_Full == ONS_TRUE)
	{
		Current_Status.RX_Current_Lost_Msgs++;
		Current_Status.RX_Total_Lost_Msgs++;
		Current_Status.RX_Total_Msgs++;
		//return USBCOMM_SUCCESS;

		return USBCOMM_ERROR_BUFFER_FULL;
	}

	//Receive Message from USB bus

	res = USBComm_Driver_Bulk_Receive_Message (incomming_message, &actual);

    if (res == USBCOMM_ERROR_RETRY)
    {
    	Current_Status.Rx_Num_Of_Retry++;
    	return res;
    }

	if (res != 0)
	{
		Current_Status.RX_Current_Lost_Msgs++;
		Current_Status.RX_Total_Lost_Msgs++;
		Current_Status.RX_Total_Msgs++;
		//TBD -define critical errors in libusb and in usbcom
		Current_Status.USBComm_Module_State = USBCOMM_ERROR_STATE;		//critical error - abort USB receive transmit operation
#ifdef ONS_PC_PLAT
		Debug_Print_Error_Message_With_Code("Error in BULK IN direction ", res );
#endif
		return res;
	}

	Current_Status.RX_Total_Msgs++;
	Current_Status.Rx_Total_Data_Size += actual;

	//update buffer
#ifdef ONS_PC_PLAT
	USBComm_Driver_Rx_Mutex_Lock();		// lock Rx mutex
#else
	if( !USBComm_Driver_Rx_Mutex_Lock() )		// lock Rx mutex
        return res;
#endif
	Rx_Actual_Message_Size[Rx_Write_To_Buffer_Index] = actual;
	memcpy(USBComm_Rx_Buffer[Rx_Write_To_Buffer_Index], incomming_message, Rx_Actual_Message_Size[Rx_Write_To_Buffer_Index]);
	Rx_Write_To_Buffer_Index = (Rx_Write_To_Buffer_Index + 1) % USBCOMM_RX_BUFFER_SIZE;  		//Update cyclic Index

	if (Rx_Read_From_Buffer_Index == Rx_Write_To_Buffer_Index)
	{
		Current_Status.RX_Buffer_Full = ONS_TRUE;
	}

	USBComm_Driver_Rx_Mutex_UNLock();			// unlock Rx mutex

	return USBCOMM_SUCCESS;

}


// --------------------------------------------------------------------------
//  Function name		: USBComm_Reset_Tx_Buffer
//
//  Purpose				: Reset the Tx Buffer
//
//  Inputs				: 
//
//  Outputs				: 
//
//  Returns				: 
//
//  Description			: Reset the Tx Buffer, the head and tail pointer 
//						: set to the start of the Buffer.
//
// ---------------------------------------------------------------------------
static void USBComm_Reset_Tx_Buffer (void)
{
	USBComm_Driver_Tx_Mutex_Lock();		// lock Tx mutex

	Tx_Read_From_Buffer_Index = 0;
	Tx_Write_To_Buffer_Index = 0;
	Current_Status.TX_Buffer_Epmty = ONS_TRUE;

	USBComm_Driver_Tx_Mutex_UNLock();		// unlock Tx mutex

}



// --------------------------------------------------------------------------
//  Function name		: USBComm_Reset_Rx_Buffer
//
//  Purpose				: Reset the Rx Buffer.
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: 
//
//  Description			: Reset the Rx Buffer, the head and tail pointer 
//						: set to the start of the Buffer.
//
// ---------------------------------------------------------------------------
static void USBComm_Reset_Rx_Buffer (void)
{
    int i;
	USBComm_Driver_Rx_Mutex_Lock();		// lock Rx Mutex

	Rx_Read_From_Buffer_Index = 0;
	Rx_Write_To_Buffer_Index = 0;
	Current_Status.RX_Buffer_Full = ONS_FALSE;
	for ( i = 0; i < USBCOMM_RX_BUFFER_SIZE ; i++ )
			Rx_Actual_Message_Size[i] = 0;

	USBComm_Driver_Rx_Mutex_UNLock();		// unlock Rx Mutex

}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Rx_Int_Main
//
//  Purpose				: read periodically from the USB bus
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: 
//
//  Description			: read periodically from the USB bus
//
// ---------------------------------------------------------------------------
void  USBComm_Rx_Int_Main ( void )
{
	int res;
	res = USBComm_Bulk_Receive_Messages();
    if( !res )
    {
        USBTimer_SetInterruptTimeMode ( USBTimer_Interrupt_Fast_Speed , USBTimer_Rx );
    }
    else
    {
        USBTimer_SetInterruptTimeMode ( USBTimer_Interrupt_Slow_Speed , USBTimer_Rx);
    }
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Tx_Int_Main
//
//  Purpose				: write periodically to the USB bus
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: 
//
//  Description			: write periodically from the USB bus
//
// ---------------------------------------------------------------------------
void  USBComm_Tx_Int_Main ( void )
{
    int res;
    
    if(!Current_Status.TX_Buffer_Epmty)
    {
        res = USBComm_Bulk_Transmit_Messages();
        USBTimer_SetInterruptTimeMode ( USBTimer_Interrupt_Fast_Speed , USBTimer_Tx );
    }
    else
    {
        Current_Status.Tx_Total_Buffer_empty++;
        USBTimer_SetInterruptTimeMode ( USBTimer_Interrupt_Slow_Speed , USBTimer_Tx );
    }
}

// --------------------------------------------------------------------------
//  Function name		: USBComm_Get_Version
//
//  Purpose				: get the module version
//
//  Inputs				: 
//
//  Outputs				: 
//						 
//  Returns				: version
//
//  Description			: get the module version
//
// ---------------------------------------------------------------------------
const char* USBComm_Get_Version (void)
{
	return USBCOMM_VERSION ;
}