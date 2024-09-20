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
#include <IO_Entity.h>
#include "usb_app.h"
void IO_Init_Ports(void);                // Init the TRIS, ANSEL, LAT of all ports
void IO_Init_PPS(void);                  // Re-map the IO Pins for the peripheral functions
int IO_Get(IO_Port_Id_t IO_Id);          // Get the Pin status according to the IO_ID
int IO_Set(IO_Port_Id_t IO_Id, int Val); // Set the Pin state according to the IO_ID and Val
int IO_Toggle(IO_Port_Id_t IO_Id);
int IO_Entity_Mgr_Get_Entity(int Entity_Port_Or_Act_ID);
int IO_Entity_Mgr_Set_Entity(int Entity_Port_Or_Act_ID, int Value);

void Get_EntityTable(unsigned char *data, unsigned char *data_size);
char Is_EntityTable_Changed(unsigned char *data, unsigned char *data_size);

char GetIO_ByEntityName(IO_Entity_Name_t entitynumber);
void Refresh_EntityTableMask();
char GetMask_ByEntityName(IO_Entity_Name_t entitynumber);
char SetValue_ByEntityName(IO_Entity_Name_t entitynumber);
char ClrValue_ByEntityName(IO_Entity_Name_t entitynumber);

void Set_GetEntityTableMode(enum EntityTable_SubFunc mode);
char Get_GetEntityTableMode();
void Set_GetEntityReplyPeriod(unsigned char period_ms);
unsigned char Get_GetEntityTableReplyPeriod();

#endif

