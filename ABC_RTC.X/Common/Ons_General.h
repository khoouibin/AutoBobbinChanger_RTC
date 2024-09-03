// ---------------------------------------------------------------------------
//  Filename: ONS_General.h
//  Created by: Nissim Avichail
//  Date:  08/07/15
//  Orisol (c)
// ---------------------------------------------------------------------------


#ifndef _ONS_GEN_
#define _ONS_GEN_



#define ONS_TRUE   1		 
#define ONS_FALSE  0


typedef   unsigned int UINT_16;
typedef   int INT_16;
typedef   unsigned long int UINT_32;
typedef   long int INT_32;


typedef unsigned char ONS_BYTE;    
typedef unsigned long Time_Unit_t;	//unsigned long


typedef union
{
	unsigned int s16;
	struct
	{
		unsigned LB : 8;
		unsigned HB : 7;
		unsigned MSB : 1;
	} bytes;
} S16_t;

typedef union
{
	unsigned int u16;
	struct
	{
		unsigned LB : 8;
		unsigned HB : 8;
	} bytes;
} U16_t;

typedef union
{
	unsigned long u32;
    unsigned int word[2];
    unsigned char byte[4];
	struct
	{
		unsigned LLB : 8;
		unsigned LHB : 8;
		unsigned HLB : 8;
		unsigned HHB : 8;
	} bytes;
} U32_t;

typedef struct
{
	U32_t time_k;
	U32_t time_k_1;
	U32_t time_period; // unit: ms.
	char timer_overflow;
} Timer_t;


#endif
