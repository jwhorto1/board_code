/*
 * switches.c
 * 
 * Date: 	9/21/2012
 * Revision:
 * Author: 	DC
 * Comments: 	Explorer 16 Input Switch Processing Functions
 * 
 * Dependencies:Delayms()
 *
 */

//#include <xc.h>  // JKW
#include "p24FJ128GA010.h"   //JKW from C:\Program Files (x86)\Microchip\mplabc30\v3.31\support\PIC24F\h
#include "HardwareProfile - PIC24FJ256GB110 PIM.h"   //JKW
#include <stdint.h>
#include <stdbool.h>
#include "switches.h"
#include "delay_ms.h"

// Variables used in this module

SWSTATE old_Sw;  // old state of switch inputs

// Initialize all Switches
void initAllSwitches(uint16_t max_debounce_time_ms)
{
    // Reset & Disable Change Notification Interrupts
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 0;
    
    // Initialize all switch inputs
    initSwitchS4(max_debounce_time_ms);
    initSwitchS5(max_debounce_time_ms);
    initSwitchS6(max_debounce_time_ms);
    initSwitchS3(max_debounce_time_ms);
}

// Initialize Switch S4

void initSwitchS4(uint16_t max_debounce_time_ms)
{
  TRIS_SW_S4 = 1;      // set as digital input
  old_Sw.Sw_bits.S4 = SwitchLevel(&SW_S4_PORT, SW_S4_PIN, max_debounce_time_ms); // save new value
  
}

// Initialize Switch S5

void initSwitchS5(uint16_t max_debounce_time_ms)
{
  TRIS_SW_S5 = 1;      // set as digital input
  old_Sw.Sw_bits.S5 = SwitchLevel(&SW_S5_PORT, SW_S5_PIN, max_debounce_time_ms); // save new value

}

// Initialize Switch S6

void initSwitchS6(uint16_t max_debounce_time_ms)
{
  TRIS_SW_S6 = 1;      // set as digital input
  old_Sw.Sw_bits.S6 = SwitchLevel(&SW_S6_PORT, SW_S6_PIN, max_debounce_time_ms); // save new value

}

// Initialize Switch S3

void initSwitchS3(uint16_t max_debounce_time_ms)
{
  TRIS_SW_S3 = 1;      // set as digital input
  old_Sw.Sw_bits.S3 = SwitchLevel(&SW_S3_PORT, SW_S3_PIN, max_debounce_time_ms); // save new value

}

// Has Switch S4 Lavel Changed ?

void SwitchS4IsPressed(uint16_t max_debounce_time_ms)
{
  // check if switch level has changed and if so, debounce/save the new level
  if(SW_S4 != old_Sw.Sw_bits.S4)
  {
    old_Sw.Sw_bits.S4 = SwitchLevel(&SW_S4_PORT, SW_S4_PIN, max_debounce_time_ms); // Debounce,Save new value

  }
}

// Has Switch S5 Lavel Changed ?

void SwitchS5IsPressed(uint16_t max_debounce_time_ms)
{
  // check if switch level has changed and if so, debounce/save the new level
  if(SW_S5 != old_Sw.Sw_bits.S5)
  {
    old_Sw.Sw_bits.S5 = SwitchLevel(&SW_S5_PORT, SW_S5_PIN, max_debounce_time_ms); // Debounce,Save new value

  }
}

// Has Switch S6 Lavel Changed ?

void SwitchS6IsPressed(uint16_t max_debounce_time_ms)
{
  // check if switch level has changed and if so, debounce/save the new level
  if(SW_S6 != old_Sw.Sw_bits.S6)
  {
    old_Sw.Sw_bits.S6 = SwitchLevel(&SW_S6_PORT, SW_S6_PIN, max_debounce_time_ms); // Debounce,Save new value

  }
}

// Has Switch S3 Lavel Changed ?

void SwitchS3IsPressed(uint16_t max_debounce_time_ms)
{
  // check if switch level has changed and if so, debounce/save the new level
  if(SW_S3 != old_Sw.Sw_bits.S3)
  {
    old_Sw.Sw_bits.S3 = SwitchLevel(&SW_S3_PORT, SW_S3_PIN, max_debounce_time_ms); // Debounce,Save new value

  }
}

// Provide Updated/Debounced current state of all input switches

void CheckSwitchIsPressed(SWSTATE *ptr, uint16_t max_debounce_time_ms)
{
    SwitchS4IsPressed(max_debounce_time_ms);
    SwitchS5IsPressed(max_debounce_time_ms);
    SwitchS6IsPressed(max_debounce_time_ms);
    SwitchS3IsPressed(max_debounce_time_ms);
    
    // update caller's player switch state structure
    ptr->Sw_all = old_Sw.Sw_all;
    
}

/*******************************************************************
* FUNCTION:     SwitchLevel() 
* INPUTS:       *sw = Pointer to I/O port register that is connected
*               to switch (e.g. &PORTD)
*               bit = bit number in port that is connected to switch
*               range = used to determine debounce time
* OUTPUTS:      TRUE (1) if switch/port pin debounced as "1"
*               FALSE (0) if switch/port pin debounced as "0"
* DESCRIPTION:  Debounces switch level.  The routine
*               will debounce the switch input to ensure that it 
*               is either high or low. See flow chart in TLS2130 class. This
*               implementation is modified to produce precise debounce delays.
*
* USAGE:        COUNT_BASE is #defined in Switches.h as 32768
*               Each additional tick in 'range' adds 1ms of bounce detection
*
*               Note: 
*               Requires Delayms.c and Delayms.h which use 4MHz Fcyc with
*               Timer 1 to implement 1ms delays using "Delayms(1);"
*           
*
* AUTHOR:       Dennis Cecic
* DATE:         23 JUN 2012
* REVISIONS:    
*******************************************************************/
uint16_t SwitchLevel(volatile unsigned int *sw, unsigned int bit, uint16_t range)
{
    uint16_t count = COUNT_BASE;
    uint16_t MAX_COUNT = count + range;
    uint16_t MIN_COUNT = count - range;
    
    // Toggle Test Pin (LED_D9)
    // LATAbits.LATA6 = 1;
    
    while ((MIN_COUNT <= count) && (count <= MAX_COUNT))
    {
        // sample the switch every 1mS
        Delayms(1);
        if (!(*sw & (1 << bit)))
        {
            count++;
            if (count > MAX_COUNT)
            {
                count = MAX_COUNT;
                // Toggle Test Pin
                // LATAbits.LATA6 = 0;
                return 0;
            }
        }
        else
        {
            count--;
            if (count < MIN_COUNT)
            {
                count = MIN_COUNT;
                // Toggle Test Pin
                // LATAbits.LATA6 = 0;
                return 1;
            }
        }
    }
    return 0;  //Should never reach this point - return should always happen within while loop
               //This was added to eliminate the warning:
               //warning: control reaches end of non-void function
}

 

     


