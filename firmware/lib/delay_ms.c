/*
 * delayms.c
 *
 * Date: 9/21/2012
 * Revision:
 * Author: DC
 * Comments: Implement 1-65536 mS blocking delay using Timer 1 & 4MHz Fcyc
 *
 */

//#include <xc.h>  // JKW
#include "p24FJ128GA010.h"   //JKW from C:\Program Files (x86)\Microchip\mplabc30\v3.31\support\PIC24F\h
#include "HardwareProfile - PIC24FJ256GB110 PIM.h"   //JKW
#include <stdint.h>
#include <stdbool.h>
#include "delay_ms.h"

void initDelayms(void)
{
    // ...4MHz Fcyc assumed to already be enabled via config bit setting

    // T1 - Clk Source: Fosc/2 (Fcyc)
    T1CONbits.TCS = 0;
    // T1 - Prescale: 1:1 (4MHz / 250nS per tick)
    T1CONbits.TCKPS = 0;
    // Ideally, PR1 4000 (1mS) period
    // PR1 adjusted to account for sw delays intro'd in loop below at 8MHz
    PR1 = 3760;
    // Clear TMR1
    TMR1 = 0;
    // Reset T1IF
    IFS0bits.T1IF = 0;
    // T1 - Turn on
    T1CONbits.TON = 1;
    
}

void Delayms(uint16_t delay)
{
    // basic kernel is 1mS (32 ticks @ 31.25uS/tick)
    
    while(delay>0)
    {
        // Clear TMR1
        TMR1 = 0;
        // Reset T1IF
        IFS0bits.T1IF = 0;
        // Wait 1mS
        while(!IFS0bits.T1IF);
        delay--;
    }
    
}

