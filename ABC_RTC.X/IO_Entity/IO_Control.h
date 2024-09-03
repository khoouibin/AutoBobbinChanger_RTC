// ---------------------------------------------------------------------------
//  Filename: IO_Control.h
//  Created by: Roey Huberman
//  Date:  03/09/15
//  Modified by: Adam Lin
//  Date: 11/09/2015
//  Orisol (c) 
// ---------------------------------------------------------------------------

#ifndef _IO_CONTROL_H_
#define	_IO_CONTROL_H_

void IO_Init_Ports(void);						// Init the TRIS, ANSEL, LAT of all ports
void IO_Init_PPS(void);							// Re-map the IO Pins for the peripheral functions
int IO_Get( IO_Port_Id_t IO_Id );				// Get the Pin status according to the IO_ID
int IO_Set( IO_Port_Id_t IO_Id , int Val );	// Set the Pin state according to the IO_ID and Val
int IO_Toggle( IO_Port_Id_t IO_Id );
int IO_Entity_Mgr_Get_Entity( int Entity_Port_Or_Act_ID );
int IO_Entity_Mgr_Set_Entity( int Entity_Port_Or_Act_ID , int Value );

#endif	// _IO_CONTROL_H_

