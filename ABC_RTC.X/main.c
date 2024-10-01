// ---------------------------------------------------------------------------
//  FileName: main.c
//  Created by: Adam Lin & Roey Huberman
//  Date: 12/10/2015
//  Orisol (c)
// ---------------------------------------------------------------------------
#include <stdio.h>

//Common includes
#include "Timers.h"
#include "GenericTypeDefs.h"
#include "Ons_General.h"
#include "RTC_IOports.h"
#include "IO_Control.h"
#include "IO_Entity.h"

#include "pps.h"
#include "RTC_Control.h"
#include "usb_app.h"
#include "RTC_PulseControl.h"
/** CONFIGURATION **************************************************/
// 150729 add Macros for Configuration Fuse Registers as below
//-----------------------------------------------------------------------------------------------------------------------------
// Section: Macros for Configuration Fuse Registers:
//-----------------------------------------------------------------------------------------------------------------------------
/* Invoke macros to set up  device configuration fuse registers.The fuses will
   select the oscillator source, power-up timers, watch-dog timers etc. The
   macros are defined within the device header files. The configuration fuse
   registers reside in Flash memory.
 */
// DSPIC33EP512MU810 Configuration Bit Settings
// 'C' source line config statements
// FGS
#pragma config GWRP = OFF           // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = OFF            // General Segment Code-Protect bit (General Segment Code protect is disabled)
#pragma config GSSK = OFF           // General Segment Key bits (General Segment Write Protection and Code Protection is Disabled)

// FOSCSEL
#pragma config FNOSC = FRC          // Initial Oscillator Source Selection Bits (Internal Fast RC (FRC))
#pragma config IESO = OFF           // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = XT          // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF       // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF        // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD       // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPOST = PS32768    // Watchdog Timer Postscaler Bits (1:32,768)
#pragma config WDTPRE = PR128       // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON          // PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF         // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128       // Power-on Reset Timer Value Select bits (128ms)
#pragma config BOREN = ON           // Brown-out Reset (BOR) Detection Enable bit (BOR is enabled)
#pragma config ALTI2C1 = OFF        // Alternate I2C pins for I2C1 (SDA1/SCK1 pins are selected as the I/O pins for I2C1)
#pragma config ALTI2C2 = OFF        // Alternate I2C pins for I2C2 (SDA2/SCK2 pins are selected as the I/O pins for I2C2)

// FICD
#pragma config ICS = PGD1           // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config RSTPRI = PF          // Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash)
#pragma config JTAGEN = OFF         // JTAG Enable bit (JTAG is disabled)

// FAS
#pragma config AWRP = OFF           // Auxiliary Segment Write-protect bit (Auxiliary program memory is not write-protected)
#pragma config APL = OFF            // Auxiliary Segment Code-protect bit (Aux Flash Code protect is disabled)
#pragma config APLK = OFF           // Auxiliary Segment Key bits (Aux Flash Write Protection and Code Protection is Disabled)
// 150729 add Macros for Configuration Fuse Registers as above

//local functions declaration
static void Initialize_System( void );
static void Initialize_Communication_Modules( void );
static void UART1_module_init(void);
static void UART2_module_init(void);

//static void uart_SendByte (uint8_t val);
//static void sec void sendUART2String(const char *data , int length);ndUART1String(const char *data , int length);
//static void sendUART2String(const char *data , int length);

RTC_Control_Event_t RTC_MSGMgr_Handle_Received_Messages( RTC_Control_State_t Control_State , ONS_BYTE* Rcv_Message );
//static RTC_Control_Error_t RTC_Control_Handle_Events( RTC_Control_Event_t Event_Val );
//static RTC_Control_Error_t RTC_Control_Handle_Init( void );

//local variables
//static ONS_BYTE                         Rcv_Message[ MSG_PROT_GEN_MSG_MAX_DATA_SIZE ];

char uart1RXBuffer[UART_BUFFER_SIZE];
char uart2RXBuffer[UART_BUFFER_SIZE];
char uart2GetPos[9] = {0x01,0x03,0x00,0x20,0x00,0x01,0x85,0xC0,'\0'};
int uart1RX_flag = 0;
int uart2RX_flag = 0;

#define RESET_MACRO()   \
    {                   \
        SET_CPU_IPL(7); \
        asm("RESET");   \
        asm("NOP");     \
        asm("NOP");     \
        asm("NOP");     \
    }

/*
    Interrupt list:
    1. USB1Interrupt:
    2. T9Interrupt: 1ms timer interrupt for rtc_control.
    3. OC1Interrupt: (un-use)
    4. OC1-OC2 with Tmr3 Interrupt: Z rpm pulse control.
    5. OC3-OC4 with Tmr4 Interrupt: X rpm pulse control.
*/
int main(void)
{
    RTC_Control_State_t Is_Exit_RTC_Control = RTC_CONTROL_STATE_UNDEFINE;
    Initialize_System();
    Initialize_Communication_Modules();

    // OCx_sequence_t x_cnt;
    // x_cnt.period.u16[1] = 0x0006;
    // x_cnt.period.u16[0] = 0xddd0;
    // x_cnt.dutyon.u16[1] = 0x0003;
    // x_cnt.dutyon.u16[0] = 0x6ee8;
    // x_pulse_settings(x_cnt);

    Is_Exit_RTC_Control = RTC_Control_Main();
    if (Is_Exit_RTC_Control == RTC_CONTROL_RESET)
    {
        RESET_MACRO();
    }
    return 0;
}

static void Initialize_System( void )
{
    PLLFBD = 58;                   		//M=60
    CLKDIVbits.PLLPOST = 0;             //N2=2
    CLKDIVbits.PLLPRE = 0;              //N1=2
    OSCTUN = 0;							// Tune FRC oscillator, if FRC is used

    RCONbits.SWDTEN = 0;                //Disable Watch Dog Timer

    // Clock switch to incorporate PLL
    __builtin_write_OSCCONH( 0x03 ); 			// Initiate Clock Switch to Primary
    // Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONL( OSCCON || 0x01 );  // Start clock switching
    while( OSCCONbits.COSC  != 0b011 );
    // Wait for Clock switch to occur
    while( OSCCONbits.LOCK != 1 );      //Wait for PLL to lock

    // Configuring the auxiliary PLL, since the primary
    // oscillator provides the source clock to the auxiliary
    // PLL, the auxiliary oscillator is disabled. Note that
    // the AUX PLL is enabled. The input 8MHz clock is divided
    // by 2, multiplied by 24 and then divided by 2. Wait till
    // the AUX PLL locks.

    ACLKCON3 = 0x24C1;
    ACLKDIV3 = 0x7;

    ACLKCON3bits.ENAPLL = 1;
    while(ACLKCON3bits.APLLCK != 1);

    REFOCONbits.ROON = 0;   // Reference oscillator output is enabled on REFCLK pin
    REFOCONbits.ROSSLP = 1;   // Reference oscillator output continues to run in Sleep
    REFOCONbits.ROSEL = 1;          // Use Oscillator crystal used as the reference clock
    REFOCONbits.RODIV = 0b1111;     // Reference clock divided by 32,768
    REFOCONbits.ROON = 1;   // Reference oscillator output is enabled on REFCLK pin
 
	IO_Init_Ports();			//151022 Adam added	
	IO_Init_PPS();				//151022 Adam added
}//end InitializeSystem

static void Initialize_Communication_Modules( void )
{
    ClearIntrflags();
    Init_Timers();
    UART1_module_init();
    UART2_module_init();
    USBDeviceInit();
    USBDeviceAttach();
}

static void UART1_module_init(void)
{
    U1MODEbits.UARTEN = 0;
    TRISGbits.TRISG12 = 1; // 485Rx
    TRISGbits.TRISG13 = 0; // 485Tx
    //TRISGbits.TRISG1 = 1; // 232Rx
    //TRISGbits.TRISG0 = 0; // 232Tx

    PPSUnLock;
    iPPSInput( IN_FN_PPS_U1RX , IN_PIN_PPS_RPI124 );         // 485Rx
    iPPSOutput( OUT_PIN_PPS_RP125 , OUT_FN_PPS_U1TX );      // 485Tx
    //iPPSInput( IN_FN_PPS_U1RX , IN_PIN_PPS_RP113 );       // 232Rx
    //iPPSOutput( OUT_PIN_PPS_RP112 , OUT_FN_PPS_U1TX );    // 232Rx
    PPSLock;

    U1MODEbits.USIDL  = 0;   // Bit13 Continue in Idle
    U1MODEbits.IREN   = 0;    // Bit12 No IR translation
    U1MODEbits.RTSMD  = 0;   // Bit11 Simplex Mode

    U1MODEbits.UEN    = 0;     // Bits8,9 TX,RX enabled, CTS,RTS not
    U1MODEbits.WAKE   = 0;    // Bit7 No Wake up (since we don't sleep here)
    U1MODEbits.LPBACK = 0;  // Bit6 No Loop Back
    U1MODEbits.ABAUD  = 0;   // Bit5 No Autobaud (would require sending '55')
    U1MODEbits.URXINV = 0;  // Bit4 IdleState = 1  (for dsPIC)
    U1MODEbits.BRGH   = 0;    // Bit3 16 clocks per bit period
    U1MODEbits.PDSEL  = 0;   // Bits1,2 8bit, No Parity
    U1MODEbits.STSEL  = 0;   // Bit0 One Stop Bit
    
    U1BRG = BRGVAL;
    // Load all values in for U1STA SFR
    U1STAbits.UTXISEL1 = 0; //Bit15 Int when Char is transferred (1/2 config!)
    U1STAbits.UTXINV   = 0; //Bit14 N/A, IRDA config
    U1STAbits.UTXISEL0 = 0; //Bit13 Other half of Bit15
    U1STAbits.UTXBRK   = 0;   //Bit11 Disabled
    U1STAbits.UTXEN    = 0;    //Bit10 TX pins controlled by periph
    U1STAbits.UTXBF    = 0;    //Bit9 *Read Only Bit*
    U1STAbits.TRMT     = 0; //Bit8 *Read Only bit*
    U1STAbits.URXISEL  = 0;  //Bits6,7 Int. on character recieved
    U1STAbits.ADDEN    = 0;    //Bit5 Address Detect Disabled
    U1STAbits.RIDLE    = 0;    //Bit4 *Read Only Bit*
    U1STAbits.PERR     = 0;     //Bit3 *Read Only Bit*
    U1STAbits.FERR     = 0;     //Bit2 *Read Only Bit*
    U1STAbits.OERR     = 0;     //Bit1 *Read Only Bit*
    U1STAbits.URXDA    = 0;    //Bit0 *Read Only Bit*
    //U1MODEbits.UARTEN  = 1;  // And turn the peripheral on

    IPC3bits.U1TXIP = 4;
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 0;
    IPC2bits.U1RXIP = 4;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    
    U1MODEbits.UARTEN = 1;//??UART
    U1STAbits.UTXEN    = 1;
}

static void UART2_module_init(void)
{
    U2MODEbits.UARTEN = 0;
    //TRISGbits.TRISG12 = 1; // 485Rx
    //TRISGbits.TRISG13 = 0; // 485Tx
    TRISGbits.TRISG1 = 1; // 232Rx
    TRISGbits.TRISG0 = 0; // 232Tx

    PPSUnLock;
    //iPPSInput( IN_FN_PPS_U1RX , IN_PIN_PPS_RPI124 );         // 485Rx
    //iPPSOutput( OUT_PIN_PPS_RP125 , OUT_FN_PPS_U1TX );      // 485Tx
    iPPSInput( IN_FN_PPS_U2RX , IN_PIN_PPS_RP113 );       // 232Rx
    iPPSOutput( OUT_PIN_PPS_RP112 , OUT_FN_PPS_U2TX );    // 232Rx
    PPSLock;

    U2MODEbits.USIDL  = 0;   // Bit13 Continue in Idle
    U2MODEbits.IREN   = 0;    // Bit12 No IR translation
    U2MODEbits.RTSMD  = 0;   // Bit11 Simplex Mode

    U2MODEbits.UEN    = 0;     // Bits8,9 TX,RX enabled, CTS,RTS not
    U2MODEbits.WAKE   = 0;    // Bit7 No Wake up (since we don't sleep here)
    U2MODEbits.LPBACK = 0;  // Bit6 No Loop Back
    U2MODEbits.ABAUD  = 0;   // Bit5 No Autobaud (would require sending '55')
    U2MODEbits.URXINV = 0;  // Bit4 IdleState = 1  (for dsPIC)
    U2MODEbits.BRGH   = 0;    // Bit3 16 clocks per bit period
    U2MODEbits.PDSEL  = 0;   // Bits1,2 8bit, No Parity
    U2MODEbits.STSEL  = 0;   // Bit0 One Stop Bit
    
    U2BRG = BRGVAL;
    // Load all values in for U1STA SFR
    U2STAbits.UTXISEL1 = 0; //Bit15 Int when Char is transferred (1/2 config!)
    U2STAbits.UTXINV   = 0; //Bit14 N/A, IRDA config
    U2STAbits.UTXISEL0 = 0; //Bit13 Other half of Bit15
    U2STAbits.UTXBRK   = 0;   //Bit11 Disabled
    U2STAbits.UTXEN    = 0;    //Bit10 TX pins controlled by periph
    U2STAbits.UTXBF    = 0;    //Bit9 *Read Only Bit*
    U2STAbits.TRMT     = 0; //Bit8 *Read Only bit*
    U2STAbits.URXISEL  = 0;  //Bits6,7 Int. on character recieved
    U2STAbits.ADDEN    = 0;    //Bit5 Address Detect Disabled
    U2STAbits.RIDLE    = 0;    //Bit4 *Read Only Bit*
    U2STAbits.PERR     = 0;     //Bit3 *Read Only Bit*
    U2STAbits.FERR     = 0;     //Bit2 *Read Only Bit*
    U2STAbits.OERR     = 0;     //Bit1 *Read Only Bit*
    U2STAbits.URXDA    = 0;    //Bit0 *Read Only Bit*
    //U2MODEbits.UARTEN  = 1;  // And turn the peripheral on

    IPC7bits.U2TXIP = 4;
    IFS1bits.U2TXIF = 0;
    IEC1bits.U2TXIE = 0;
    IPC7bits.U2RXIP = 4;
    IFS1bits.U2RXIF = 0;
    IEC1bits.U2RXIE = 1;
    
    U2MODEbits.UARTEN = 1;//EN_UART
    U2STAbits.UTXEN    = 1;
}

/*
// UART send Byte
static void uart_SendByte (uint8_t val)
{
    while(U1STAbits.UTXBF == 1);    //wait for an empty position in the buffer
    U1TXREG = val;
    while(!U1STAbits.TRMT);
}
*/

// UART1 send String
//static void sendUART1String(const char *data , int length)
//{
//    int i;
//    for(i=0; i<length; i++)
//    {
//        while(U1STAbits.UTXBF == 1);    //wait for an empty position in the buffer
//        U1TXREG = *data;
//        while(!U1STAbits.TRMT);
//        data++;
//    }
//}
//
//// UART2 send String
//static void sendUART2String(const char *data , int length)
//{
//    int i;
//    for(i=0; i<length; i++)
//    {
//        while(U2STAbits.UTXBF == 1);    //wait for an empty position in the buffer
//        U2TXREG = *data;
//        while(!U2STAbits.TRMT);
//        data++;
//    }
//}

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
    static int index = 0;
    while (U1STAbits.URXDA && index < UART_BUFFER_SIZE)
    {
        char receivedChar = U1RXREG;

        uart1RXBuffer[index++] = receivedChar;
        Delay1usX(180);     //1byte 180us for 57600
        /*
        if (receivedChar == '\r')
        {
            IO_Toggle( IOP_XA1_PISTON_ID );
            uart1RXBuffer[index - 1] = '\0'; 
            uart1RX_flag = 1;
            //sendUARTString(uartRXBuffer);
            //Clear Buffler
            //memset(uartRXBuffer, 0, UART_BUFFER_SIZE);
            index = 0;
        }*/
    }
    IO_Toggle( IOP_XA1_PISTON_ID );
    //uart1RXBuffer[index - 1] = '\0';
    uart1RX_flag = 1;
    index = 0;
    IFS0bits.U1RXIF = 0;
}

void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void)
{
    static int index = 0;
    while (U2STAbits.URXDA && index < UART_BUFFER_SIZE)
    {
        char receivedChar = U2RXREG;

        uart2RXBuffer[index++] = receivedChar;
        Delay1usX(180);     //1byte 180us for 57600
        /*
        if (receivedChar == '\r')
        {
            IO_Toggle( IOP_XA2_PISTON_ID );
            uart2RXBuffer[index - 1] = '\0'; 
            uart2RX_flag = 1;
            //sendUARTString(uartRXBuffer);
            //Clear Buffler
            //memset(uartRXBuffer, 0, UART_BUFFER_SIZE);
            index = 0;
        }*/
    }
    IO_Toggle( IOP_XA2_PISTON_ID );
    //uart2RXBuffer[index - 1] = '\0';
    uart2RX_flag = 1;
    index = 0;
    IFS1bits.U2RXIF = 0;
}

