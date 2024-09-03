// ---------------------------------------------------------------------------
//  Filename: Msg_Port.c
//  Created by: Nissim Avichail
//  Date:  06/07/15
//  Orisol (c)
// ---------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Definition.h"
#include "Ons_General.h"

#include "Timers.h"
#include "Ons_General.h"

#include "Msg_Prot.h"
#include "USBComm.h"

 
// -----------------
// Internal Define
// -----------------

// ---------------------
// internal Prototypes
// ---------------------
static unsigned long Msg_Prot_Checksum_calculator ( ONS_BYTE* Data , unsigned int Size );
static unsigned long Msg_Prot_Get_Msg_SN ( void );


//---------------------
// internal variables
// --------------------
static 	unsigned long			Msg_SN;	 			// Message S/N this number counter.
static 	Message_Header_t		Last_Msg_Hdr;

// ------------------------------
//  API Function Implementation
// ------------------------------
// ---------------------------------------------------------------------------
//  Function name		: Msg_Prot_Init
//
//  Purpose				: Module initialization
//
//  Inputs				:
//						:
//
//  Outputs				:
//
//  Returns				: Success or Error code
//
//  Description			: Module initialization
//
// ---------------------------------------------------------------------------
void Msg_Prot_Init (void)
{
	Msg_SN = 0;

	Last_Msg_Hdr.Id = NO_MESSAGE_ID;
}

// ---------------------------------------------------------------------------
//  Function name		: Msg_Prot_Send_Message
//
//  Purpose				: send message to USB
//
//  Inputs				: Msg_Buff - pointer to transmit message Buffer .
//						: Msg_Size - size of given message buffer
//
//  Outputs				:
//
//  Returns				: Success or Error code
//
//  Description			: Send message to USB
//
// ---------------------------------------------------------------------------
int Msg_Prot_Send_Message (ONS_BYTE* Mgs_Data_Buff, unsigned int Msg_Id, unsigned int Msg_Data_Size)
{
	Generic_Message_t tmp_message;
	int res;

	// update all message filed
	memcpy(tmp_message.Data,Mgs_Data_Buff,Msg_Data_Size);

	tmp_message.Message_Header.Id = Msg_Id;

	tmp_message.Message_Header.Serial_Number = Msg_Prot_Get_Msg_SN();

	tmp_message.Message_Header.Size = Msg_Data_Size + sizeof(tmp_message.Message_Header);   // the size

    tmp_message.Message_Header.Time_Stamp = SysTimer_GetTimeInMiliSeconds();

	res = USBComm_Send_Message ((USBComm_Buffer_t)&tmp_message , tmp_message.Message_Header.Size );

	return res;
}

// ---------------------------------------------------------------------------
//  Function name		: Msg_Prot_Receive_Next_Message_From_USB
//
//  Purpose				: get new message from USB RX buffer
//
//  Inputs				: Data_Buff - Buffer for received data.
//
//
//  Outputs				: Msg_Id    - message identification or NO_MESSAGE_ID if there is no message exist in buffer .
//						: New_Msg   -  the sender have to check this to determine if new message received
//						:			   1 - if message exist return 1
//						: 			   0 - No message received.
//
//  Returns				: Success or Error code
//
//  Description			: Get the new message from USB receive buffer
//						: update the buffer data, message id and new message fields,
//						: return success or relevant error code.
//
// ---------------------------------------------------------------------------
int Msg_Prot_Receive_Next_Message (Rcv_Msg_Data_t Data_Buff, unsigned int* Msg_Id, unsigned int* New_Msg)
{
	unsigned int 		msg_size;
	Generic_Message_t	generic_msg;
	Generic_Message_t*	msg_p;
 	int			res;

	msg_p = &generic_msg;
	res = USBComm_Peek_Message( (ONS_BYTE**)&msg_p, New_Msg, &msg_size );

	if (res != (int)USBCOMM_SUCCESS )
	{
		return (res);
	}

	if (!(*New_Msg))
	{
		*Msg_Id = NO_MESSAGE_ID;
		return (MSG_PROT_SUCCESS);
	}

	if (msg_size < sizeof(Message_Header_t))
	{
		return (MSG_PROT_ERROR_SIZE_MESSAGE);
	}

	//Extract data from USBComm received data buffer
	memcpy(&Last_Msg_Hdr,&(msg_p->Message_Header), sizeof(Last_Msg_Hdr));	// save last message header
	memcpy(Data_Buff, (msg_p->Data), MSG_PROT_GEN_MSG_MAX_DATA_SIZE);		// save message data to given Data Buffer
	res = USBComm_Remove_Rx_Message();										// we already copy the message so now we can remove it from Buffer
	*Msg_Id = Last_Msg_Hdr.Id;
	return res;
}

// ---------------------------------------------------------------------------
//  Function name		: Msg_Prot_Get_Last_Msg_Hdr
//
//  Purpose				: send message to USB
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: Success or Error code
//
//  Description			: Send message to USB
//
// ---------------------------------------------------------------------------
void Msg_Prot_Get_Last_Msg_Hdr (int* Msg_Id,unsigned long* Msg_SN, int* Msg_Size, unsigned long* Msg_Time )
{
	*Msg_Id   = Last_Msg_Hdr.Id;
	*Msg_SN   = Last_Msg_Hdr.Serial_Number;
	*Msg_Size = Last_Msg_Hdr.Size;
	*Msg_Time = Last_Msg_Hdr.Time_Stamp;
}

//***********************************************************************************************************************************************************************************
//  TEST MESSAGES
//***********************************************************************************************************************************************************************************
//Host Test Message
// ---------------------------------------------------------------------------
//  Function name		: Msg_Prot_Pack_&_Tx_Host_Tst_Msg
//
//  Purpose				: pack & transmit host to RTC Test message
//
//  Inputs				:
//
//  Outputs				: Msg_Buff - data of the message.
//						: Size - the size of data.
//
//  Returns				: success or error code
//
//  Description			: update message header and data fields
//
// ---------------------------------------------------------------------------
int Msg_Prot_Pack_n_Tx_Host_Tst_Msg(int* Wait, int* Num_Of_Msg, int*  Delta_Time, int* Return_Size,int* Fill_Size,unsigned long* Tx_Time)
{
	Msg_Prot_Host_Tst_Msg_t  host_test_msg;
	int res;
	int index;
	USBComm_Buffer_t 	msg_p;
	int 				tmp_size;  //size of the Message info and the fill array (for checksum calculation)
    
	host_test_msg.Msg_Hdr.Id = MSG_PROT_TEST_USB_HOST_TO_RTC;
	host_test_msg.Msg_Hdr.Serial_Number = Msg_Prot_Get_Msg_SN();
	host_test_msg.Test_msg_info.Wait = *Wait;
	host_test_msg.Test_msg_info.Num_Of_Msg = *Num_Of_Msg;
	host_test_msg.Test_msg_info.Delta_Time = *Delta_Time;
	host_test_msg.Test_msg_info.Return_Size = *Return_Size;
	host_test_msg.Test_msg_info.SN = host_test_msg.Msg_Hdr.Serial_Number;
	host_test_msg.Test_msg_info.Fill_Size =  *Fill_Size;
	host_test_msg.Test_msg_info.Checksum = 0;

	//Verify fill Size (to avoid array overflow)
	if (*Fill_Size > TEST_HOST_FILL_SIZE)
	{
		host_test_msg.Test_msg_info.Fill_Size = TEST_HOST_FILL_SIZE;
	}

	if (*Fill_Size < 0)
	{
		host_test_msg.Test_msg_info.Fill_Size = 0;
	}

	//update Fill array
	for  (index = 0; index < host_test_msg.Test_msg_info.Fill_Size; index++)
	{
		host_test_msg.Fill[index] = 1 + index;
	}

	msg_p = (USBComm_Buffer_t)&host_test_msg;
	msg_p    = &msg_p[sizeof(host_test_msg.Msg_Hdr)];    //msg_p point to the start of host test message info
	tmp_size =  sizeof(host_test_msg.Test_msg_info) + host_test_msg.Test_msg_info.Fill_Size;  //size of the Message info and the fill array (for checksum calculation)
	host_test_msg.Test_msg_info.Checksum = Msg_Prot_Checksum_calculator(msg_p, tmp_size);      //calculate checksum on given message (when checksum field = 0)
	host_test_msg.Msg_Hdr.Size = sizeof(host_test_msg.Msg_Hdr)+ sizeof(host_test_msg.Test_msg_info) + host_test_msg.Test_msg_info.Fill_Size;    //the real size of message (general_message+test message+fill)

//#ifdef ONS_RTC
    host_test_msg.Msg_Hdr.Time_Stamp = SysTimer_GetTimeInMiliSeconds();
//#endif

	res = USBComm_Send_Message( (USBComm_Buffer_t)&host_test_msg , host_test_msg.Msg_Hdr.Size );
	return res;
}

 // ---------------------------------------------------------------------------
 //  Function name		: Msg_Prot_Unpack_Host_Test_Message
 //
 //  Purpose				: unpack test message
 //
 //  Inputs				: Msg_Buff - pointer to the received message (without the generic header) .
 //						: Msg_Size - the size of the received data.
 //
 //  Outputs				:Tst_info - all info from the header
 //
 //  Returns				: Success or Error code
 //
 //  Description			: Unpack message from Data_Buff validate the size
 //							: and the checksum. send message to log/to screen.
 //
 // ---------------------------------------------------------------------------
 int Msg_Prot_Unpack_Host_Test_Message(ONS_BYTE* Msg_Buff, Msg_Prot_Host_Tst_Msg_info_t* Tst_Info)
 {
	int	msg_size;
 	unsigned long calculated_checksum;
 	unsigned long tmp_checksum;
 	Msg_Prot_Host_Tst_Msg_info_t* msg_p;

	msg_p = (Msg_Prot_Host_Tst_Msg_info_t*)Msg_Buff;
	tmp_checksum = msg_p->Checksum;
	msg_size = msg_p->Fill_Size + sizeof(Msg_Prot_Host_Tst_Msg_info_t);
	msg_p->Checksum = 0;
 	memcpy(Tst_Info, Msg_Buff, sizeof(Msg_Prot_Host_Tst_Msg_info_t));

 	// validate checksum
 	calculated_checksum = Msg_Prot_Checksum_calculator(Msg_Buff, msg_size);      //calculate checksum on given message (when checksum field = 0)

 	if (tmp_checksum != calculated_checksum)
 	{
 		return  MSG_PROT_ERROR_CHECKSUM_MESSAGE;
 	}
 	return MSG_PROT_SUCCESS;
 }

 //RTC Test Message
 // ---------------------------------------------------------------------------
 //  Function name		: Msg_Prot_Pack_&_Tx_RTC_Tst_Msg
 //
 //  Purpose				: pack & transmit RTC to Host Test message
 //
 //  Inputs				:
 //
 //  Outputs				: Msg_Buff - data of the message.
 //							: Size - the size of fill array.
 //
 //  Returns				: success or error code
 //
 //  Description			: update message fields
 //
 // ---------------------------------------------------------------------------
  int Msg_Prot_Pack_n_Tx_RTC_Tst_Msg(unsigned long Origin_Msg_SN, int Return_Size)
 {
	 USBComm_Status_t Status;
	 Msg_Prot_RTC_Tst_Msg_t rtc_test_msg;
	 int index;
	 int res;
	 USBComm_Buffer_t 	msg_p;
	 int 				tmp_size;  //size of the Message info and the fill array (for checksum calculation)

     rtc_test_msg.Msg_Hdr.Id = MSG_PROT_TEST_USB_RTC_TO_HOST;
	 rtc_test_msg.Msg_Hdr.Serial_Number = Msg_Prot_Get_Msg_SN();

	 rtc_test_msg.Tst_msg_info.Origin_Msg_SN = Origin_Msg_SN;

	 USBComm_Get_Status(&Status);
	 rtc_test_msg.Tst_msg_info.Status.Total_Rcv_Lost_Msgs = Status.RX_Total_Lost_Msgs;
	 rtc_test_msg.Tst_msg_info.Status.Total_Rcv_Msg = Status.RX_Total_Msgs;
	 rtc_test_msg.Tst_msg_info.Status.Total_Rcv_data_size = Status.Rx_Total_Data_Size;
	 rtc_test_msg.Tst_msg_info.Status.Total_Tx_Lost_Msgs = Status.TX_Total_Buffer_Full_Msgs;
	 rtc_test_msg.Tst_msg_info.Status.Total_Rcv_data_size = Status.TX_Total_Msgs;
	 rtc_test_msg.Tst_msg_info.Fill_Size = Return_Size - sizeof(rtc_test_msg.Msg_Hdr) - sizeof(rtc_test_msg.Tst_msg_info);
     rtc_test_msg.Tst_msg_info.Busy_Time = 0;//dummy value in Host (RTC use Get_Total_Busy(1))
	 if (Return_Size > (int)(sizeof(rtc_test_msg)+sizeof(rtc_test_msg.Tst_msg_info)))
	 {
		 Return_Size = sizeof(rtc_test_msg);
		 rtc_test_msg.Tst_msg_info.Fill_Size = TEST_RTC_FILL_SIZE;
	 }

	 if (Return_Size < (int)(sizeof(rtc_test_msg)+sizeof(rtc_test_msg.Tst_msg_info)))
	 {
		 rtc_test_msg.Tst_msg_info.Fill_Size = 0;
	 }

	 //update Fill array
	 for  (index = 0; index < rtc_test_msg.Tst_msg_info.Fill_Size; index++)
	 {
		  rtc_test_msg.Fill[index] = 1 + index;
 	 }

	 msg_p = (USBComm_Buffer_t)&rtc_test_msg;
  	 msg_p    = &msg_p[sizeof(rtc_test_msg.Msg_Hdr)];    //msg_p point to the start of host test message info
  	 tmp_size =  sizeof(rtc_test_msg.Tst_msg_info) + rtc_test_msg.Tst_msg_info.Fill_Size;  		//size of the Message info and the fill array (for checksum calculation)
  	 rtc_test_msg.Tst_msg_info.Checksum = Msg_Prot_Checksum_calculator(msg_p, tmp_size);      	//calculate checksum on given message (when checksum field = 0)

  	 rtc_test_msg.Msg_Hdr.Size = Return_Size;
     
//#ifdef ONS_RTC
	 rtc_test_msg.Msg_Hdr.Time_Stamp = SysTimer_GetTimeInMiliSeconds();
//#endif

	res = USBComm_Send_Message( (USBComm_Buffer_t)&rtc_test_msg , rtc_test_msg.Msg_Hdr.Size );
   
	 return res;
 }

// ---------------------------------------------------------------------------
//  Function name		: Msg_Prot_Unpack_RTC_Test_Message
//
//  Purpose				: unpack test message
//
//  Inputs				: Msg_Buff - pointer to the received message (without the generic header) .
//						: Msg_Size - the size of the received data.
//
//  Outputs				:Tst_info - all info from the header
//
//  Returns				: Success or Error code
//
//  Description			: Unpack message from Data_Buff validate the size
//						: and the checksum. send message to log/to screen.
//
// ---------------------------------------------------------------------------
int Msg_Prot_Unpack_RTC_Test_Message(ONS_BYTE* Msg_Buff, Msg_Prot_RTC_Tst_Hdr_t* Tst_Info)
{
	int 	msg_size;
	unsigned long calculated_checksum;
	Msg_Prot_RTC_Tst_Hdr_t* msg_p;
	unsigned long  tmp_checksum;

	msg_p = (Msg_Prot_RTC_Tst_Hdr_t*)Msg_Buff;
	msg_size = msg_p->Fill_Size + sizeof(Msg_Prot_RTC_Tst_Hdr_t);

	memcpy(Tst_Info, Msg_Buff, msg_size);

	// validate checksum
	tmp_checksum = msg_p->Checksum;

	calculated_checksum = Msg_Prot_Checksum_calculator(Msg_Buff, msg_size);      //calculate checksum on given message (when checksum field = 0)

	calculated_checksum -=  tmp_checksum;
	if (tmp_checksum != calculated_checksum)
	{
	//	Debug_Print_Error_Message_With_Code("Data checksum failed - Calculated checksum = ", calculated_checksum);
	//	return  MSG_PROT_ERROR_CHECKSUM_MESSAGE;
	}
	return MSG_PROT_SUCCESS;
}

// ---------------------------------------------------------------------------
//  Function name		: Msg_Prot_Checksum_calculator
//
//  Purpose				: calculate the data checksum
//
//  Inputs				:
//
//  Outputs				: Data - the data to calculate CS on.
//						: Size - the size of data.
//
//  Returns				: Checksum value
//
//  Description			: Get data and size, sum up the character value
//
// ---------------------------------------------------------------------------
static unsigned long Msg_Prot_Checksum_calculator(ONS_BYTE* Data, unsigned int Size)
{
	unsigned int index;
	unsigned long sum;
	sum = 0;

	for (index= 0; index < Size; index++)
	{
		sum += Data[index];
	}
	return sum;
}

// ---------------------------------------------------------------------------
//  Function name		: Msg_Prot_Get_Msg_SN
//
//  Purpose				: give the message serial number.
//
//  Inputs				:
//
//  Outputs				:
//
//  Returns				: Serial number
//
//  Description			: return the Serial number for Msg_Port messages
//
// ---------------------------------------------------------------------------
static unsigned long Msg_Prot_Get_Msg_SN(void)
{
	Msg_SN++;
	return Msg_SN;			// Message Index minimum value is 1
}