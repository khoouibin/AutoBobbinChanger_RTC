// ---------------------------------------------------------------------------
//  Filename: IO_Control.c
//  Created by: Roey Huberman
//  Date:  03/09/15
//  Modified by:by: Adam Lin
//  Date: 11/09/2015
//  Orisol (c) 
// ---------------------------------------------------------------------------

#include <pps.h>
#include "Compiler.h"
#include "RTC_IODef.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include "IO_Entity.h"

void IO_Init_Ports(void)
{
	TRISA = TRISA_DIRECTION; // 0x0000
	LATA = LATA_INIT_VALUE;
	ANSELA = ANSELA_VALUE;
	TRISB = TRISB_DIRECTION; // 0xFFB1
	LATB = LATB_INIT_VALUE;
	ANSELB = ANSELB_VALUE;
	TRISC = TRISC_DIRECTION; // 0xF01C
	LATC = LATC_INIT_VALUE;
	ANSELC = ANSELC_VALUE;
	TRISD = TRISD_DIRECTION; // 0xFFC0;
	LATD = LATD_INIT_VALUE;
	ANSELD = ANSELD_VALUE;
	TRISE = TRISE_DIRECTION; // 0x0242
	LATE = LATE_INIT_VALUE;
	ANSELE = ANSELE_VALUE;
	TRISF = TRISF_DIRECTION; // 0x303F
	LATF = LATF_INIT_VALUE;
	TRISG = TRISG_DIRECTION; // 0x1282
	LATG = LATG_INIT_VALUE;
	ANSELG = ANSELG_VALUE;
	ODCGbits.ODCG15 = 1;	 // open drain output, TK
	LATGbits.LATG15 = 0;	 // output low to enable level shift IC, TK
	TRISH = TRISH_DIRECTION; // 0x0000
	LATH = LATH_INIT_VALUE;	 // All pins are off
	TRISJ = TRISJ_DIRECTION; // 0xEC00
	LATJ = LATJ_INIT_VALUE;
	TRISK = TRISK_DIRECTION; // 0xF803
	LATK = LATK_INIT_VALUE;
}

void IO_Init_PPS(void)
{
	PPSUnLock;
	iPPSOutput(OUT_PIN_PPS_RP84, OUT_FN_PPS_OC2); // Step Z 	// RE4/RP84  		//PIn 144
	iPPSInput(IN_FN_PPS_T6CK, IN_PIN_PPS_RPI32);  // Step Counter     // On New RTC uses the RB0/RPI32
	iPPSInput(IN_FN_PPS_QEI1, IN_PIN_PPS_RPI72);  // Index    // RD8/RPI72        //Pin 97
	iPPSInput(IN_FN_PPS_QEA1, IN_PIN_PPS_RPI77);  // Ch A     // RD13/RPI77      // Pin 114
	iPPSInput(IN_FN_PPS_QEB1, IN_PIN_PPS_RPI76);  // Ch B     // RD12/RPI76      // Pin 113
	iPPSInput(IN_FN_PPS_INT3, IN_PIN_PPS_RPI42);  // INT3     // RB10/RPI42
	iPPSInput(IN_FN_PPS_INT4, IN_PIN_PPS_RPI40);  // INT4     // RB8/RPI40
	PPSLock;
}

int IO_Get(IO_Port_Id_t IO_Id)
{
	return ((*(IO_Reg_Addr[IO_Ports_Table[IO_Id].Reg_ID])) & (IO_Ports_Table[IO_Id].IO_Mask)); // return 1 when sensor bit is 1
}

int IO_Set(IO_Port_Id_t IO_Id, int Val)
{
	if (Val) // set the bit to 1
	{
		(*(IO_Reg_Addr[IO_Ports_Table[IO_Id].Reg_ID])) |= IO_Ports_Table[IO_Id].IO_Mask;
	}
	else
	{
		(*(IO_Reg_Addr[IO_Ports_Table[IO_Id].Reg_ID])) &= (~IO_Ports_Table[IO_Id].IO_Mask);
	}
	return 0;
}

int IO_Toggle(IO_Port_Id_t IO_Id)
{
	if ((*(IO_Reg_Addr[IO_Ports_Table[IO_Id].Reg_ID])) & (IO_Ports_Table[IO_Id].IO_Mask)) // set the bit to 1
	{
		(*(IO_Reg_Addr[IO_Ports_Table[IO_Id].Reg_ID])) &= (~IO_Ports_Table[IO_Id].IO_Mask);
	}
	else
	{
		(*(IO_Reg_Addr[IO_Ports_Table[IO_Id].Reg_ID])) |= IO_Ports_Table[IO_Id].IO_Mask;
	}
	return 0;
}

char GetIO_ByEntityName(IO_Entity_Name_t entitynumber)
{
	IO_Port_t *PortAddr;
	unsigned int tmp;
	char bit_value;
	if (entitynumber>=IO_TABLE_MAX)
		return 0xff;
	if (Entities_IO_Port_Def[entitynumber].PortReg == UNDEFINED)
		return 0xff;
	PortAddr = IO_Reg_Addr[Entities_IO_Port_Def[entitynumber].PortReg];
    tmp = (*PortAddr) & (Entities_IO_Port_Def[entitynumber].PortBitMask);
	bit_value = (tmp > 0) ? 1 : 0;
    return bit_value;
}

char GetMask_ByEntityName(IO_Entity_Name_t entitynumber)
{
	char bit_value;
	if (entitynumber>=IO_TABLE_MAX)
		return 0xff;
	bit_value = Entities_IO_Port_Def[entitynumber].BitComp;
    return bit_value;
}

char WriteValue_ByEntityName(IO_Entity_Name_t entitynumber, unsigned char entity_value)
{
	if (entitynumber>=IO_TABLE_MAX)
		return 0xff;
	if (entity_value == 0)
		ClrValue_ByEntityName(entitynumber);
	else if (entity_value == 1)
		SetValue_ByEntityName(entitynumber);
	return 0;
}

char SetValue_ByEntityName(IO_Entity_Name_t entitynumber)
{
	IO_Port_t *PortAddr;
	if (entitynumber>=IO_TABLE_MAX)
		return 0xff;
	if (Entities_IO_Port_Def[entitynumber].PortReg == UNDEFINED)
		return 0xff;
	if (Entities_IO_Port_Def[entitynumber].PortBitType != IO_OUTPUT)
		return 0xff;
    PortAddr = IO_Reg_Addr[Entities_IO_Port_Def[entitynumber].PortReg];
    *PortAddr = *PortAddr | (Entities_IO_Port_Def[entitynumber].PortBitMask);
	return 0;
}

char ClrValue_ByEntityName(IO_Entity_Name_t entitynumber)
{
	IO_Port_t *PortAddr;
	if (entitynumber>=IO_TABLE_MAX)
		return 0xff;
	if (Entities_IO_Port_Def[entitynumber].PortReg == UNDEFINED)
		return 0xff;
	if (Entities_IO_Port_Def[entitynumber].PortBitType != IO_OUTPUT)
		return 0xff;
    PortAddr = IO_Reg_Addr[Entities_IO_Port_Def[entitynumber].PortReg];
    *PortAddr = (*PortAddr) & (~Entities_IO_Port_Def[entitynumber].PortBitMask);
	return 0;
}