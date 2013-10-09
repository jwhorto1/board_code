/*
 * Module:      main.c
 *
 * Project:     wifly_pass_thru_demo_exp16_p24
 * Date:        09/24/2012
 * Revision:
 * Author:      DC
 * Comments:    Project enables WiFly PICTail configuration on Explorer16
 *              (PIC24FJ128GA010) using TeraTerm or other terminal emulator.
 *
 *              Switch S4 can be used to trigger factory setting restoral.
 *
 *              Terminal settings: 9600, 8, N, 1, No Flow Control
 *
 * Schematic:   "rn-171-pictail_rev2_sch.pdf" 09/18/2012
 *
 */

/** INCLUDES ******************************************************************/

#include <xc.h>  // JKW
//#include <p24fxxxx.h>
//#include "p24FJ128GA010.h"   //JKW from C:\Program Files (x86)\Microchip\mplabc30\v3.31\support\PIC24F\h
//#include "HardwareProfile - PIC24FJ256GB110 PIM.h"   //JKW
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "delay_ms.h"
#include "indicators.h"
#include "switches.h"
#include "lcd.h"
#include "console.h"
#include "wifly_drv.h"


/*** CONFIGURATION ************************************************************/

// Key Settings:
// OSCILLATOR: Primary Oscillator (XT Crystal) (8MHz --> Fcy=4MHz)
// JTAG PORT: Disabled
// WATCHDOG TIMER: Disabled
// DEBUG/PGM PINS: PGC2/PGD2 (EMUC2/EMUD2)

// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
//#define POSCMOD = XT
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)
/** VARIABLES *****************************************************************/

char  OutString[128];               //console output buffer used by sprintf()
const char Version[] = "00.01";
const char CompileDate[] =__DATE__;
const char CompileTime[] =__TIME__;

/** LOCAL #defines ************************************************************/

// System clock frequency definitions - valid after reset and initialization.
// Must be supported by a combination of config bit settings as well as run-time
// initialization.

#define SYS_CLOCK_FREQ          8000000             // clock generator output
#define CPU_CLOCK_FREQ          SYS_CLOCK_FREQ/2    // Fcyc
#define PB_CLOCK_FREQ           SYS_CLOCK_FREQ/2    // Peripheral Bus clock
#define SOSC_CLOCK_FREQ         32768               // SOSC XTAL frequency

// Switch debounce interval (ms)

#define SWITCH_DEBOUNCE_MS      20

/** LOCAL PROTOTYPES ********************************************************/

void InitializeSystem(void);    // Initialize hardware and global variables

/** main() ********************************************************************/

int main(void)
{
    InitializeSystem();

    sprintf(OutString, "WiFly Pass_thru\nDemo");
    putsLCD(OutString);
    while(1)
    {
        // check if S4 pressed to trigger WiFly factory restoral
        if(0 == SwitchLevel(&SW_S4_PORT, SW_S4_PIN, SWITCH_DEBOUNCE_MS))
        {
            sprintf(OutString,"\r\n\r\ninitiating WiFly factory reset...\r\n\r\n");
            putsConsole(OutString);
            // drive GPIO9 high for 10ms before resetting the module
            RN_GPIO9 = 1;
            Delayms(1000);
            // Assert RESET for 10ms
            RN_RESET = 0;
            Delayms(100);
            RN_RESET = 1;
            Delayms(100);
            // toggle GPIO9 5 times, delaying at least 1 sec between toggles
            RN_GPIO9 = 0;
            Delayms(1200);
            RN_GPIO9 = 1;
            Delayms(1200);

            RN_GPIO9 = 0;
            Delayms(1200);
            RN_GPIO9 = 1;
            Delayms(1200);

            RN_GPIO9 = 0;
            Delayms(1200);
            RN_GPIO9 = 1;
            Delayms(1200);

            RN_GPIO9 = 0;
            Delayms(1200);
            RN_GPIO9 = 1;
            Delayms(1200);

            RN_GPIO9 = 0;
            Delayms(1200);

            // now toggle RESET to reboot the module and apply factory default settings
            RN_RESET = 0;
            Delayms(100);
            RN_RESET = 1;
            // wait for user to release S4
            while(0 == SwitchLevel(&SW_S4_PORT, SW_S4_PIN, SWITCH_DEBOUNCE_MS));
        }
    }

} // main

/******************************************************************************
 * Function:        void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine takes care of all of the system
 *                  initialization that is required.
 *
 * Note:
 *
 *****************************************************************************/
void InitializeSystem(void)
{

    initDelayms();                  // Delayms() must be initialized first
    initAllIndicators();            // Init all Explorer 16 LEDs
    initAllSwitches(SWITCH_DEBOUNCE_MS);  // Init all Explorer 16 switches
    initLCD();                      // Init LCD

    //Initialize Debug Console UART interface
    initConsole(PB_CLOCK_FREQ);
    clrscrConsole();
    homeConsole();
    putsConsole("wifly_pass_thru_demo_exp16_p24...\r\n");
    sprintf(OutString,"version: %s\r\n", Version);
    putsConsole(OutString);
    sprintf(OutString,"build date: %s\r\n", CompileDate);
    putsConsole(OutString);
    sprintf(OutString,"build time: %s\r\n\r\n", CompileTime);
    putsConsole(OutString);
    sprintf(OutString,"use a terminal emulator to communicate directly with the module...\r\n\r\n");
    putsConsole(OutString);
    sprintf(OutString,"(optional) press S4 on Explorer16 to reset WiFly to factory defaults...\r\n\r\n");
    putsConsole(OutString);
    initWiFly(PB_CLOCK_FREQ);
}

