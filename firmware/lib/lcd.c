/*
 * lcd.c
 * 
 * Date: 9/21/2012
 * Revision:
 * Author: DC
 * Comments: Explorer 16 LCD Routines
 * 
 * (Based upon LDJ "Flying the PIC24 SE" (LCD.c) with updates)
 *
 */

//#include <xc.h>  // JKW
#include "p24FJ128GA010.h"   //JKW from C:\Program Files (x86)\Microchip\mplabc30\v3.31\support\PIC24F\h
#include "HardwareProfile - PIC24FJ256GB110 PIM.h"   //JKW
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "delay_ms.h"

void initLCD( void)
{
    // PMP initialization 
    PMCON = 0x83BF;             // Enable the PMP, long waits
    PMMODE = 0x3FF;             // Master Mode 1
    PMAEN = 0x0001;             // PMA0 enabled
    
    // wait for >30ms
    Delayms(30);
    
    //initiate the HD44780 display 8-bit init sequence

    // "Function Set"
    PMADDR = LCDCMD;            // command register
    PMDATA = 0b00111000;        // 8-bit interface, 2 lines,5x7
    Delayms(1);
    
    // "Display ON/OFF Control"
    PMDATA = 0b00001100;        // disp ON, cursor off, blink off
    Delayms(1);
    
    // "Clear Display"
    PMDATA = 0b00000001;        // clear display
    Delayms(2);
    
    // "Entry Mode Set"
    PMDATA = 0b00000110;        // increment cursor, no shift
    Delayms(2);

} // initLCD


char readLCD( uint16_t addr)
{
    uint16_t dummy;
    while( PMMODEbits.BUSY);    // wait for PMP to be available
    PMADDR = addr;              // select the command address
    dummy = PMDATA;             // initiate a dummy read cycle
    while( PMMODEbits.BUSY);    // wait for PMP to be available
    return( PMDATA);            // read the status register
    
} // readLCD


void writeLCD( uint16_t addr, char c)
{
    while( busyLCD());
    while( PMMODEbits.BUSY);    // wait for PMP to be available
    PMADDR = addr;
    PMDATA = c;
} // writeLCD
    

void putsLCD(char *s)
{
    int row, col;

    row = 1;
    col = 1;

    while(*s)
    {
        switch (*s)
        {
        case '\n':          // point to next line
            switch(row)
            {
                case 1:
                   setLCDC(0x40); // point to 2nd line
                   row++;
                   break;
                case 2:
                   setLCDC(0x00); // point to 1st line
                   row = 1;
                   break;
                default:
                   row = 1;
                   break;
            }
            break;

        default:            // print character
            putLCD(*s);
            col++;
            break;
        } //switch
        s++;
    } //while
} //putsLCD
