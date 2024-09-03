// ---------------------------------------------------------------------------
//  Filename: USBComm_Driver.h
//  Created by: Nissim Avichail
//  Date:  30/06/15
//  Orisol (c)
// ---------------------------------------------------------------------------
#ifndef _USBCOMM_DRIVER_H_
#define _USBCOMM_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif
// ----------------
//   Prototypes
//------------------

void	USBComm_Driver_Set_Debug ( int Log_message_levels );
int 	USBComm_Driver_Init ( void );
int		USBComm_Driver_Open_USB_Device ( void );
int 	USBComm_Driver_Is_Kernel_Driver_Active ( void );
int 	USBComm_Driver_Detach_Kernel_Driver( void );
int 	USBComm_Driver_Release_Interface ( void );
int 	USBComm_Driver_Claim_Interface ( void );
void  	USBComm_Driver_Close_USB_Device ( void );
void  	USBComm_Driver_Exit ( void );
int 	USBComm_Driver_Bulk_Send_Message ( ONS_BYTE* outgoing_message , int message_size, int* actual );
int 	USBComm_Driver_Bulk_Receive_Message ( ONS_BYTE* incomming_message , int* actual );

int 	USBComm_Driver_Tx_Mutex_Lock ( void );
int 	USBComm_Driver_Rx_Mutex_Lock ( void );

void 	USBComm_Driver_Tx_Mutex_UNLock ( void );
void 	USBComm_Driver_Rx_Mutex_UNLock ( void );
int 	USBComm_Driver_Create_Tx_Thread ( void );
int 	USBComm_Driver_Create_Rx_Thread ( void );
int		USBComm_Driver_Tx_Thread_kill( void );
int		USBComm_Driver_Rx_Thread_kill( void );
void	USBComm_Driver_Sleep_Millisecond( unsigned int Time_Millisecond );

int     USBCheckFirstInit ( void );
#ifdef __cplusplus
}
#endif
#endif //_USBCOMM_DRIVER_H_









