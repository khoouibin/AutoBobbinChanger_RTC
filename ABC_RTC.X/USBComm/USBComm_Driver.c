// ---------------------------------------------------------------------------
//  Filename: USBComm_Driver.c
//  Created by: Nissim Avichail
//  Date:  30/06/15
//  Orisol (c)
// ---------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <stdint.h>
#include "Ons_General.h"

#include "usb.h"
#include "usb_function_generic.h"

#include "USBComm.h"
#include "USBComm_Driver.h"

// -----------------
// Internal Define
// -----------------
#define RTC_USB_VID					0x04d8
#define RTC_USB_PID					0x0204
#define RTC_USB_EP_OUT				1
#define RTC_USB_EP_IN               129

#define INTRFACE_NUM				0

#define BULK_OUT_TIMEOUT			0		// 0 -No timeout [MS]
#define BULK_IN_TIMEOUT				0		// 0 -No timeout

#define Rx_THREAD_PRIORITY			99
#define Tx_THREAD_PRIORITY			99


//---------------------
// internal variables
// --------------------
static int USBCheckDeviceStatus ( void );
static int USBComm_Rtc_Err_Flag;                                        //USB event error flag
static int USBComm_Rx_Int_Flag;                                         //USB Interrupt receive flag
static int USBComm_Tx_Int_Flag;                                         //USB Interrupt send flag 
    
USB_HANDLE USBComm_GenericInHandle;                                     //USB send handle.  Must be initialized to 0 at startup.
USB_HANDLE USBComm_GenericOutHandle;                                    //USB receive handle.  Must be initialized to 0 at startup.
unsigned char OUTPacket[USBGEN_EP_SIZE] ;                               //USB input messages local buffer for receive messages

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Set_Debug
//
//  Purpose				: Initialization of the USBComm module
//
//  Inputs				: Log_message_levels
//
//  Outputs				:
//						:
//  Returns				:
//
//  Description			: Host -  Set libusb Debug level
//						: LIBUSB_LOG_LEVEL_NONE		(0) : no messages ever printed by the library (default)
//						: LIBUSB_LOG_LEVEL_ERROR	(1) : error messages are printed to stderr
//						: LIBUSB_LOG_LEVEL_WARNING	(2) : warning and error messages are printed to stderr
//						: LIBUSB_LOG_LEVEL_INFO		(3) : informational messages are printed to stdout, warning and error messages are printed to stderr
//						: LIBUSB_LOG_LEVEL_DEBUG	(4) : debug and informational messages are printed to stdout, warnings and errors to stderr)
//
// ---------------------------------------------------------------------------
void USBComm_Driver_Set_Debug(int Log_message_levels)
{
	return;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Init
//
//  Purpose				: Initialize USB Library.
//
//  Inputs				: USBComm_Rx_Buffer_Adder - pointer to the Rx Buffer in USBComm
//
//  Outputs				:
//						:
//  Returns				: 0 on success, or a Error code on failure (Host - LIBUSB_ERROR)
//
//  Description			: Host -  initialize the libusb
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Init( void )
{
    USBComm_Rtc_Err_Flag = 0;
    USBComm_Rx_Int_Flag = 0;
    USBComm_Tx_Int_Flag = 0;
    USBComm_GenericInHandle = 0;
    USBComm_GenericOutHandle = 0;
    USBDeviceInit();
    USBDeviceAttach();
    return USBCOMM_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Open_USB_Device
//
//  Purpose				: finding the device handler.
//
//  Inputs				:
//
//  Outputs				:
//						:
//  Returns				: USBCOMM_SUCCESS of USBCOMM_ERROR_CAN_NOT_OPEN_DEVICE
//
//  Description			: Host - Convenience function for finding a device
//						:		 with a particular idVendor/idProduct combination
//                        RTC - This function attaches the device.
// ---------------------------------------------------------------------------
int USBComm_Driver_Open_USB_Device()
{
    return USBCOMM_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Is_Kernel_Driver_Active
//
//  Purpose				: Determine if a kernel driver is active on an interface.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: 0 on success, 1 if active, or a Error code on failure (Host - LIBUSB_ERROR)
//
//  Description			: Host - Convenience function for finding a device
//						:		 If a kernel driver is active, you cannot claim the interface,
//						:		 and libusb will be unable to perform I/O.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Is_Kernel_Driver_Active(void)
{
    return USBCOMM_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Detach_Kernel_Driver
//
//  Purpose				: Detach a kernel driver from an interface.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: 0 on success, or a Error code on failure (Host - LIBUSB_ERROR)
//
//  Description			: Host - Detach a kernel driver from an interface.
//						:		 If successful, you will then be able to claim the interface and perform I/O.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Detach_Kernel_Driver(void)
{
    return USBCOMM_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Release_Interface
//
//  Purpose				: Release an interface previously claimed with libusb_claim_interface().
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: 0 on success, or a Error code on failure (Host - LIBUSB_ERROR)
//
//  Description			: Host - Release an interface previously claimed with libusb_claim_interface().
//						:		 You should release all claimed interfaces before closing a device handle.
//						:		 This is a blocking function.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Release_Interface ( void )
{
	return USBCOMM_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Claim_Interface
//
//  Purpose				: Claim an interface on a given device handle.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: 0 on success, or a Error code on failure (Host - LIBUSB_ERROR)
//
//  Description			: Host - Claim an interface on a given device handle.
//						:		 You must claim the interface you wish to use before you can perform I/O on any of its endpoints.
//						:		 This is a non-blocking function.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Claim_Interface(void)
{
    return USBCOMM_SUCCESS;
}


// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Close_USB_Device
//
//  Purpose				: Close a device handle.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				:
//
//  Description			: Host - Close a device handle.
//						:		 Should be called on all open handles before your application exits.
//						:		 Internally, this function destroys the reference that was added by libusb_open() on the given device.
//						:		 This is a non-blocking function; no requests are sent over the bus.
//
// ---------------------------------------------------------------------------
void USBComm_Driver_Close_USB_Device (void)
{
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Exit
//
//  Purpose				: uninitialized USB library.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				:
//
//  Description			: Host - Deinitialize libusb.
//						:		 Should be called after closing all open devices and before your application terminates.
//                        RTC - Detach USB device
// ---------------------------------------------------------------------------
void USBComm_Driver_Exit(void)
{
    USBSoftDetach();
}



// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Bulk_Send_Message
//
//  Purpose				: send message to USB in bulk mode.
//
//  Inputs				:
//
//  Outputs				: outgoing_message 	- pointer to the message to send
//						: message_size		- the size of the message to send
//						: actual 		   	- the actual size of that sent successfully
//
//  Returns				: 0 on success, or a Error code on failure (Host - LIBUSB_ERROR)
//
//  Description			: send message to USB EP (OUT direction) in bulk mode.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Bulk_Send_Message(ONS_BYTE* outgoing_message, int message_size, int* actual)
{
    if( !USBCheckDeviceStatus ( ) )
    {
        if( !USBHandleBusy ( USBComm_GenericInHandle ) )
        {    
            if( !USBComm_Tx_Int_Flag )
            {
                USBComm_GenericInHandle = USBGenWrite ( USBGEN_EP_NUM , outgoing_message , message_size );         
                *actual = USBHandleGetLength ( USBComm_GenericInHandle );
                return USBCOMM_SUCCESS;
            }
        }
        return USBCOMM_ERROR_RETRY;
    }
    return USBCOMM_ERROR_MODULE_IN_ERROR_STATE;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Bulk_Receive_Message
//
//  Purpose				: receive message from USB in bulk mode.
//
//  Inputs				:
//
//  Outputs				: incomming_message 	- pointer to the  received message
//						: actual 		   		- the actual size of that sent successfully
//
//  Returns				: 0 on success, or a Error code on failure (Host - LIBUSB_ERROR)
//
//  Description			: send message to USB EP (OUT direction) in bulk mode.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Bulk_Receive_Message(ONS_BYTE* incomming_message, int* actual)
{
     if( !USBCheckDeviceStatus ( ) )
    {
        if( !USBHandleBusy ( USBComm_GenericOutHandle ) )
        {
            if ( !USBComm_Rx_Int_Flag )
            {
                *actual = USBHandleGetLength ( USBComm_GenericOutHandle );
                memcpy( incomming_message , OUTPacket , *actual );
                USBComm_GenericOutHandle = USBGenRead ( USBGEN_EP_NUM , ( BYTE* ) OUTPacket , USBGEN_EP_SIZE );
                return USBCOMM_SUCCESS;
            }
        }
        return USBCOMM_ERROR_RETRY;
    }
    return USBCOMM_ERROR_MODULE_IN_ERROR_STATE;
}


// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Tx_Mutex_Lock
//
//  Purpose				: lock Tx mutex.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				:
//
//  Description			: lock Tx mutex/Interrupt Flag.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Tx_Mutex_Lock(void)
{
    //mutex already locked
    if( USBComm_Tx_Int_Flag )
        return ONS_FALSE;
    
    USBComm_Tx_Int_Flag = 1;
    return ONS_TRUE;
}


// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Rx_Mutex_Lock
//
//  Purpose				: lock Rx mutex.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				:
//
//  Description			: lock Rx mutex/Interrupt Flag.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Rx_Mutex_Lock(void)
{
    if( USBComm_Rx_Int_Flag )
        return ONS_FALSE;
    USBComm_Rx_Int_Flag = 1;
    return ONS_TRUE;
}


// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Tx_Mutex_UNLock
//
//  Purpose				: unlock Tx mutex.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				:
//
//  Description			: unlock Tx mutex/Interrupt Flag.
//
// ---------------------------------------------------------------------------
void USBComm_Driver_Tx_Mutex_UNLock(void)
{
    USBComm_Tx_Int_Flag = 0;
}


// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Rx_Mutex_UNLock
//
//  Purpose				: unlock Rx mutex.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				:
//
//  Description			: unlock Rx mutex/Interrupt Flag.
//
// ---------------------------------------------------------------------------
void USBComm_Driver_Rx_Mutex_UNLock(void)
{
    USBComm_Rx_Int_Flag = 0;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Create_Tx_Thread
//
//  Purpose				: create Tx thread.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: 0 for success or pthread error code
//
//  Description			: create Tx thread.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Create_Tx_Thread (void)
{
    return USBCOMM_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Create_Rx_Thread
//
//  Purpose				: create Rx thread.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: 0 for success or  error code
//
//  Description			: create Rx thread.
//
// ---------------------------------------------------------------------------
int USBComm_Driver_Create_Rx_Thread (void)
{
    return USBCOMM_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Tx_Thread_kill
//
//  Purpose				: create Tx thread.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: 0 for success or  error code
//
//  Description			: create Tx thread.
//
// ---------------------------------------------------------------------------
int	USBComm_Driver_Tx_Thread_kill(void)
{
    return USBCOMM_SUCCESS;
}
// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Rx_Thread_kill
//
//  Purpose				: create Rx thread.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: 0 for success or  error code
//
//  Description			: create Rx thread.
//
// ---------------------------------------------------------------------------
int	USBComm_Driver_Rx_Thread_kill(void)
{
    return USBCOMM_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: USBComm_Driver_Sleep_Millisecond
//
//  Purpose				: sleep for time in milliseconds.
//
//  Inputs				: sleep time in Millisecond
//
//  Outputs				:
//
//  Returns				:
//
//  Description			: sleep for time in milliseconds.
//
// ---------------------------------------------------------------------------
void	USBComm_Driver_Sleep_Millisecond(unsigned int Time_Millisecond)
{
}

// ---------------------------------------------------------------------------
//  Function name		: USBCBInitEP
//
//  Purpose				: This function is called when the device becomes initialized.
//
//  Inputs				: 
//
//  Outputs				:
//
//  Returns				:
//
//  Description			: This callback function should initialize the endpoints 
//                        for the device's usage according to the current configuration.
//
// ---------------------------------------------------------------------------
void USBCBInitEP( void )
{
    //Enable the application endpoints
    USBEnableEndpoint( USBGEN_EP_NUM , USB_OUT_ENABLED | USB_IN_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP );
    //Arm the application OUT endpoint, so it can receive a packet from the host
    USBComm_GenericOutHandle = USBGenRead( USBGEN_EP_NUM ,(BYTE*) OUTPacket , USBGEN_EP_SIZE);
}

// ---------------------------------------------------------------------------
//  Function name		: USER_USB_CALLBACK_EVENT_HANDLER
//
//  Purpose				: This function is called from the USB stack to
//                        notify a user application that a USB event occured.
//
//  Inputs				: int event - the type of event
//                        void *pdata - pointer to the event data
//                        WORD size - size of the event data
//
//  Outputs				:
//
//  Returns				: BOOL True/False
//
//  Description			:  This callback is in interrupt context
//                         when the USB_INTERRUPT option is selected.
//
// ---------------------------------------------------------------------------
BOOL USER_USB_CALLBACK_EVENT_HANDLER ( int event , void *pdata , WORD size )
{
    switch( event )
    {
        case EVENT_TRANSFER:
            break;
        case EVENT_SOF:
            break;
        case EVENT_SUSPEND:
            break;
        case EVENT_RESUME:
            break;
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            break;
        case EVENT_EP0_REQUEST:
            break;
        case EVENT_BUS_ERROR:
        case EVENT_TRANSFER_TERMINATED:
            USBComm_Rtc_Err_Flag = 1;
            break;
        default:
            break;
    }      
    return TRUE; 
}

// ---------------------------------------------------------------------------
//  Function name		: USBCheckDeviceStatus
//
//  Purpose				: This function is called before each interrupt send / receive
//                        routine to check the device state.     
//
//  Inputs				: 
//
//  Outputs				: 
//
//  Returns				: Success/Failure
//
//  Description			: This function check the device state.
//
// ---------------------------------------------------------------------------
int USBCheckDeviceStatus ( void )
{
    if ( ( !( USBDeviceState & CONFIGURED_STATE ) ) || ( USBSuspendControl == 1 ) || ( USBComm_Rtc_Err_Flag == 1 ) )
    {
        return USBCOMM_ERROR_MODULE_IN_ERROR_STATE;
    }
    return USBCOMM_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: USBCheckFirstInit
//
//  Purpose				: This function is called to check first initialization of USB device.  
//
//  Inputs				: 
//
//  Outputs				: 
//
//  Returns				: Success/Failure
//
//  Description			: This function check the device state.
//
// ---------------------------------------------------------------------------
int USBCheckFirstInit ( void )
{
    if ( ( !( USBDeviceState & CONFIGURED_STATE ) ) || ( USBSuspendControl == 1 ) )
    {
        return USBCOMM_ERROR_MODULE_NOT_READY;
    }
    return USBCOMM_SUCCESS;
}

