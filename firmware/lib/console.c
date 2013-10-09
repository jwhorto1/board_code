/*
 * console.c
 * 
 * Date: 9/21/2012
 * Revision:
 * Author: DC
 * Comments: Basic UART 2 CONSOLE I/O for Explorer16
 *
 */

//#include <xc.h>  // JKW
#include "p24FJ128GA010.h"   //JKW from C:\Program Files (x86)\Microchip\mplabc30\v3.31\support\PIC24F\h
#include "HardwareProfile - PIC24FJ256GB110 PIM.h"   //JKW
#include <stdint.h>
#include <stdbool.h>
#include "console.h"

#define BAUDRATE    9600    // baud rate setting
#define BACKSPACE   0x8     // ASCII backspace character code

// init the serial port (UART2, BAUDRATE, 8, N, 1, None )

void initConsole(uint32_t Fcyc)
{
    // Turn the UART off
    U2MODEbits.UARTEN = 0;
    U2STAbits.UTXEN = 0;

    // Disable U2 Interrupts
    IFS1bits.U2TXIF = 0;	          // Clear the Transmit Interrupt Flag
    IEC1bits.U2TXIE = 0;	          // Disable Transmit Interrupts
    IFS1bits.U2RXIF = 0;	          // Clear the Receive Interrupt Flag
    IEC1bits.U2RXIE = 0;	          // Disable Receive Interrupts

    // Configure U2MODE SFR
    U2MODEbits.UARTEN = 0;	        // Bit15 TX, RX DISABLED, ENABLE at end of func
    //U2MODEbits.notimplemented;	  // Bit14
    U2MODEbits.USIDL = 0;	          // Bit13 Continue in Idle
    U2MODEbits.IREN = 0;	          // Bit12 No IR translation
    U2MODEbits.RTSMD = 0;	          // Bit11 RTS Flow Ctrl Mode
    //U2MODEbits.notimplemented;	  // Bit10
    U2MODEbits.UEN = 0;		          // Bits8,9 TX,RX enabled
    U2MODEbits.WAKE = 0;	          // Bit7 No Wake up (since we don't sleep here)
    U2MODEbits.LPBACK = 0;	        // Bit6 No Loop Back
    U2MODEbits.ABAUD = 0;	          // Bit5 No Autobaud (would require sending '55')
    U2MODEbits.RXINV = 0;	          // Bit4 IdleState = 1  (for dsPIC)
  //U2MODEbits.BRGH = 1;	          // Bit3 16 clocks per bit period
    U2MODEbits.BRGH = 0;	          // Bit3 16 clocks per bit period
    U2MODEbits.PDSEL = 0;	          // Bits1,2 8bit, No Parity
    U2MODEbits.STSEL = 0;	          // Bit0 One Stop Bit

    // Load a value into the Baud Rate generator
    //U2BRG = ((Fcyc/BAUDRATE)/4)-1; // (BRGH=1 setting)
    U2BRG = ((Fcyc/BAUDRATE)/16)-1; // (BRGH=1 setting)

    // Configure U2STA SFR
    U2STAbits.UTXISEL1 = 0;	        // Bit15 Int when Char is transferred (1/2 config!)
    U2STAbits.UTXINV = 0;	          // Bit14 N/A, IRDA config
    U2STAbits.UTXISEL0 = 0;	        // Bit13 Other half of Bit15
    //U2STAbits.notimplemented = 0;	// Bit12
    U2STAbits.UTXBRK = 0;	          // Bit11 Disabled
    U2STAbits.UTXEN = 0;	          // Bit10 TX pins controlled by periph
    U2STAbits.UTXBF = 0;	          // Bit9 *Read Only Bit*
    U2STAbits.TRMT = 0;	            // Bit8 *Read Only bit*
    U2STAbits.URXISEL = 0;	        // Bits6,7 Int. on character recieved
    U2STAbits.ADDEN = 0;	          // Bit5 Address Detect Disabled
    U2STAbits.RIDLE = 0;	          // Bit4 *Read Only Bit*
    U2STAbits.PERR = 0;		          // Bit3 *Read Only Bit*
    U2STAbits.FERR = 0;		          // Bit2 *Read Only Bit*
    U2STAbits.OERR = 0;		          // Bit1 *Read Only Bit*
    U2STAbits.URXDA = 0;	          // Bit0 *Read Only Bit*

    // Set U2RX Interrupt Priority
    // ...default setting used (4)
    // Configure U2 Interrupt Flags (U2 RX is interrupt-driven)
    IFS1bits.U2TXIF = 0;	          // Clear the Transmit Interrupt Flag
    IEC1bits.U2TXIE = 0;	          // Disable Transmit Interrupts
    IFS1bits.U2RXIF = 0;	          // Clear the Receive Interrupt Flag
    IEC1bits.U2RXIE = 1;	          // Enable Receive Interrupts
    
    // ...And turn the UART on
    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN = 1;

} // initConsole

// send a character to the UART2 serial port
void putConsole(int16_t c)
{
    while (U2STAbits.UTXBF);   // wait while Tx buffer full
    U2TXREG = c;
} // putConsole

// wait for a new character to arrive to the UART2 serial port
char getConsole(void)
{
    char temp;
    while(IFS1bits.U2RXIF == 0);	// wait for a new character to arrive
    temp = U2RXREG;
    IFS1bits.U2RXIF = 0;
    return temp;		// read the character from the receive buffer
}// getConsole


// send a null terminated string to the UART2 serial port
void putsConsole(char *s)
{
    while(*s)			// loop until *s == '\0' the  end of the string
        putConsole(*s++);	// send the character and point to the next one
//    putConsole('\r');       // terminate with a cr / line feed
} // putsConsole


char *getsnConsole(char *s, uint16_t len)
{
    char *p = s;            // copy the buffer pointer 
    do{
        *s = getConsole();       // wait for a new character
        putConsole(*s);         // echo character
        
        if ((*s==BACKSPACE)&&(s>p))
        {
            putConsole(' ');     // overwrite the last character
            putConsole( BACKSPACE);
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
} // getsn

/*** Interrupt Service routine for UART2 RX ******************************/
void __attribute__ ((interrupt,no_auto_psv)) _U2RXInterrupt(void)
{
  IFS1bits.U2RXIF = 0;      // Clear the interrupt status of UART2 RX
  while(!U2STAbits.URXDA);

  while (U2STAbits.UTXBF);  // wait while Tx buffer full
  U1TXREG = U2RXREG;        // console Rx --> wifly Tx
}


