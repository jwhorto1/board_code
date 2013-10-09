/*
 * wifly_drv.c
 *
 * Date: 9/22/2012
 * Revision:
 * Author: DC
 * Comments: PIC UART1 and GPIO driver routines for RN1xx PICTail on Explorer16
 *
 * Schematic Ref: "rn-171-pictail_rev2_sch.pdf" 09/18/201
 * WiFly UM Reference: "rn-wiflycr-um-1.0r.pdf" (covers v2.36-v2.45)
 *
 */

//#include <xc.h>  // JKW
#include "p24FJ128GA010.h"   //JKW from C:\Program Files (x86)\Microchip\mplabc30\v3.31\support\PIC24F\h
#include "HardwareProfile - PIC24FJ256GB110 PIM.h"   //JKW
#include <stdint.h>
#include <stdbool.h>
#include "wifly_drv.h"

#define BAUDRATE    9600    // baud rate setting
#define BACKSPACE   0x8     // ASCII backspace character code

// init the PIC I/O and UART (UART1, BAUDRATE, 8, N, 1, None )

int16_t  WiFlyMsgLen;  //JKW
char  WiFlyMsg[128];  //JKW
char *WiFlyMsg_ptr;  //JKW
char ftpDataIncoming; //JKW

void initWiFly(uint32_t Fcyc)
{
        // initialize wifly RESET input port pin
        // enable open drain configuration and drive high
        // required since module RESET has internal 100kOhm PU
        TRIS_RN_RESET = 0;
        OD_RN_RESET = 1;        // enable RF0 output as OD
        RN_RESET = 0;           // put module in reset

        RN_GPIO9 = 0;
        TRIS_RN_GPIO9 = 0;

        // enable open-drain config on GPIO5 and drive low
        // required since PICTail has 100k pull-up on GPIO5
        // as soon as the ALT GPIO function is enabled, this pin becomes input
        // and module will interpret as "connect to remote Host", unless you
        // drive it low.
        RN_GPIO5 = 0;           // OD driven so output is low
        OD_RN_GPIO5 = 1;        // enable RF7 output as OD
        TRIS_RN_GPIO5 = 0;

        // disable module
        RN_RESET = 0;

	// initialize PIC UART settings
        // Turn the UART off
        U1MODEbits.UARTEN = 0;
	U1STAbits.UTXEN = 0;

	// Disable U1 Interrupts
	IFS0bits.U1TXIF = 0;	          // Clear the Transmit Interrupt Flag
	IEC0bits.U1TXIE = 0;	          // Disable Transmit Interrupts
	IFS0bits.U1RXIF = 0;	          // Clear the Receive Interrupt Flag
	IEC0bits.U1RXIE = 0;	          // Disable Receive Interrupts

	// Configure U1MODE SFR
	U1MODEbits.UARTEN = 0;	        // Bit15 TX, RX DISABLED, ENABLE at end of func
	//U1MODEbits.notimplemented;	  // Bit14
	U1MODEbits.USIDL = 0;	          // Bit13 Continue in Idle
	U1MODEbits.IREN = 0;	          // Bit12 No IR translation
	U1MODEbits.RTSMD = 0;	          // Bit11 RTS Flow Ctrl Mode
	//U1MODEbits.notimplemented;	  // Bit10
	U1MODEbits.UEN = 0;		          // Bits8,9 TX,RX enabled
	U1MODEbits.WAKE = 0;	          // Bit7 No Wake up (since we don't sleep here)
	U1MODEbits.LPBACK = 0;	        // Bit6 No Loop Back
	U1MODEbits.ABAUD = 0;	          // Bit5 No Autobaud (would require sending '55')
	U1MODEbits.RXINV = 0;	          // Bit4 IdleState = 1  (for dsPIC)
	//U1MODEbits.BRGH = 1;	          // Bit3 16 clocks per bit period
        U1MODEbits.BRGH = 0;	          // Bit3 16 clocks per bit period
	U1MODEbits.PDSEL = 0;	          // Bits1,2 8bit, No Parity
	U1MODEbits.STSEL = 0;	          // Bit0 One Stop Bit

	// Load a value into the Baud Rate generator
	//U1BRG 	= ((Fcyc/BAUDRATE)/4)-1; // (BRGH=1 setting)
        U1BRG 	= ((Fcyc/BAUDRATE)/16)-1; // (BRGH=1 setting)

	// Configure U1STA SFR
	U1STAbits.UTXISEL1 = 0;	        // Bit15 Int when Char is transferred (1/2 config!)
	U1STAbits.UTXINV = 0;	          // Bit14 N/A, IRDA config
	U1STAbits.UTXISEL0 = 0;	        // Bit13 Other half of Bit15
	//U1STAbits.notimplemented = 0;	// Bit12
	U1STAbits.UTXBRK = 0;	          // Bit11 Disabled
	U1STAbits.UTXEN = 0;	          // Bit10 TX pins controlled by periph
	U1STAbits.UTXBF = 0;	          // Bit9 *Read Only Bit*
	U1STAbits.TRMT = 0;	            // Bit8 *Read Only bit*
	U1STAbits.URXISEL = 0;	        // Bits6,7 Int. on character recieved
	U1STAbits.ADDEN = 0;	          // Bit5 Address Detect Disabled
	U1STAbits.RIDLE = 0;	          // Bit4 *Read Only Bit*
	U1STAbits.PERR = 0;		          // Bit3 *Read Only Bit*
	U1STAbits.FERR = 0;		          // Bit2 *Read Only Bit*
	U1STAbits.OERR = 0;		          // Bit1 *Read Only Bit*
	U1STAbits.URXDA = 0;	          // Bit0 *Read Only Bit*

	// Set WiFly RTS
	// U1TRTS    = 0;                // make RTS output
	// U1RTS     = 0;                // set RTS default status (enable Xbee DO)

	// Set U1RX Interrupt Priority
        // ...default setting used (4)
        // Configure U1 Interrupt Flags (U1 RX is interrupt-driven)
        IFS0bits.U1TXIF = 0;	          // Clear the Transmit Interrupt Flag
        IEC0bits.U1TXIE = 0;	          // Disable Transmit Interrupts
        IFS0bits.U1RXIF = 0;	          // Clear the Receive Interrupt Flag
        IEC0bits.U1RXIE = 1;	          // Enable Receive Interrupts

        // ...And turn the UART on
        U1MODEbits.UARTEN = 1;
	U1STAbits.UTXEN = 1;

	// Enable the Module
        RN_RESET = 1;

} // initWiFly

// send a character to the UART1 serial port
void putWiFly(int16_t c)
{
	while (U1STAbits.UTXBF);   // wait while Tx buffer full
	U1TXREG = c;
} // putWiFly


// wait for a new character to arrive to the UART2 serial port
// Oddly, I think this was setup for UART2.  I changed to UART1 Aug 21, 2013 JKW
// reverted back to UART2 serial port
char getWiFly(void)
{
    char temp;
    while(IFS0bits.U1RXIF == 0);	// wait for a new character to arrive
    temp = U1RXREG;
    IFS0bits.U1RXIF = 0;
    return temp;		// read the character from the receive buffer

//    while(IFS1bits.U2RXIF == 0);	// wait for a new character to arrive
//    temp = U2RXREG;
//    IFS1bits.U2RXIF = 0;
//    return temp;		// read the character from the receive buffer

}// getWiFly


// send a null terminated string to the UART1 serial port
void putsWiFly(char *s)
{
	while(*s)			// loop until *s == '\0' the  end of the string
		putWiFly(*s++);	// send the character and point to the next one
//    putWiFly('\r');       // terminate with a cr / line feed
} // putsWiFly


char *getsnWiFly(char *s, uint16_t len)
{
    char *p = s;            // copy the buffer pointer
    do{
        *s = getWiFly();       // wait for a new character
        //putWiFly(*s);         // echo character

        if ((*s==BACKSPACE)&&(s>p))
        {
            putWiFly(' ');     // overwrite the last character
            putWiFly( BACKSPACE);
            len++;
            s--;            // back the pointer
            continue;
        }
        if (*s=='\n')      // line feed, ignore it
            continue;
        if (*s=='\r')      // end of line, end loop
            break;
        s++;                // increment buffer pointer
        len--;
    } while (len>1 );      // until buffer full

    *s = '\0';              // null terminate the string

    return p;               // return buffer pointer
} // getsnWiFly

/*** Interrupt Service routine for UART1 RX ******************************/
void __attribute__ ((interrupt,no_auto_psv)) _U1RXInterrupt(void)
{
  IFS0bits.U1RXIF = 0;      // Clear the interrupt status of UART1 RX
  while(!U1STAbits.URXDA);


  while (U1STAbits.UTXBF);  // wait while Tx buffer full
  U2TXREG = U1RXREG;        // wifly Rx --> console Tx

  if (ftpDataIncoming == 1)
  {
      *WiFlyMsg_ptr = U1RXREG;
      WiFlyMsg_ptr++;
      if (WiFlyMsg_ptr >= &WiFlyMsg[0] + 127) {WiFlyMsg_ptr = &WiFlyMsg[0];}
      WiFlyMsgLen = WiFlyMsg_ptr - &WiFlyMsg[0];
  }

  if (U1RXREG == '[')
  {
      ftpDataIncoming = 1;
      WiFlyMsgLen = 0;
      WiFlyMsg_ptr = &WiFlyMsg[0];
  }
  if (U1RXREG == ']')
  {
      ftpDataIncoming = 0;
      *WiFlyMsg_ptr = '\n';
      WiFlyMsg_ptr++;
      *WiFlyMsg_ptr = 0x00;
      WiFlyMsg_ptr++;
      WiFlyMsgLen = WiFlyMsg_ptr - &WiFlyMsg[0];
  }

}



