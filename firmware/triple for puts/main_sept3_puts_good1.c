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

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "delay_ms.h"
#include "indicators.h"
#include "switches.h"
#include "lcd.h"
#include "console.h"
#include "wifly_drv.h"

#include <string.h>


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

// GLOBALS
//char    UART1_Active;
//char    InString[128];
//int     InStringLength;
//int     OldInStringLength;

//char   length;
//char   ones_char;
//char   tens_char;

//int16_t  WiFlyMsgLen;  //JKW
//char  WiFlyMsg[128];  //JKW
//char *WiFlyMsg_ptr;  //JKW
//char ftpDataIncoming; //JKW

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
 unsigned char jloop;
 unsigned int iloop;

 char AddrString[30];
 char ConstString[256];


    InitializeSystem();

 unsigned char   temp, temp1, temp2, temp3, temp4;
 unsigned char   ones_char, tens_char, huns_char;
 int16_t   change_cnt;
    sprintf(OutString, "WiFly Pass_thru\nDemo");
    putsLCD(OutString);
    // JKW starts here
  Delayms(5000);
    sprintf(OutString,"$$$");
    putsWiFly(OutString);
    putsConsole(OutString);
  Delayms(5000);
    sprintf(OutString,"set comm open [\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
    sprintf(OutString,"set wlan ssid Arthur$and$Matts$Internet\r\n");
//  sprintf(OutString,"set wlan ssid Hogwarts\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
    sprintf(OutString,"set wlan phrase hot7blue\r\n");
//  sprintf(OutString,"set wlan phrase pooKums496\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
    sprintf(OutString,"set ftp timer 80\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
    sprintf(OutString,"set comm close ]\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
    sprintf(OutString,"set ftp dir /jackwaterman\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
    sprintf(OutString,"set ftp user waterallusers\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
    sprintf(OutString,"set ftp pass aRkeO7755123!\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
//User=jaycen     is now waterallusers
//Pass=jWes232740 is now aRkeO7755123!
//Dir=../../      is now /jackwaterman
    Delayms(1000);
    sprintf(OutString,"save\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
    sprintf(OutString,"get everything\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(200);
    sprintf(OutString, "Sept4 1:00PM    \nputs only       ");  // columns 41, 59
    putsConsole(OutString);
  Delayms(200);
   setLCDC(0x00); // point to 1st line
  Delayms(200);
    putsLCD(OutString);
  Delayms(2000);
    sprintf(OutString, "\r\n");
    putsConsole(OutString);

  change_cnt = 79;
//  sprintf(ConstString,"A9ABE296E34713800CC11D0D1E0FB197B36810D14F32085E4E50BFB685EC755E26C88F63A105284B0AC627FA90EF3B294D6D549C0A0D88E814B5A2C17794B1EBD1A064868BB377DFD5CD80218D73D3F0A18B642EFAFACEDDBA6E2694D27F3E7EE3B04A57D6CBBE452343D9428133C5209D763B2F776628529B94173B3568");  // 1009


  for (iloop = 0; iloop <=999; iloop++)
    {
  Delayms(500);  // worked well with 5000
//  sprintf(AddrString,"ftp put jack___000.txt\r\n");
    sprintf(AddrString,"ftp put arthur_000.txt\r\n");
    temp1 = (iloop  % 10);
    ones_char = temp1 + 0x30;
    temp2 = ((iloop - temp1) % 100)/10;
    tens_char = temp2 + 0x30;
    temp3 = (iloop - temp2*10 - temp1)/100;
    huns_char = temp3 + 0x30;
    AddrString[15] = huns_char;
    AddrString[16] = tens_char;
    AddrString[17] = ones_char;
  Delayms(500);  // worked well with 5000
    putsConsole(AddrString);
  Delayms(500);
    putsWiFly(AddrString);

  sprintf(ConstString,"A9ABE296E34713800CC11D0D1E0FB197B36810D14F32085E4E50BFB685EC755E26C88F63A105284B0AC627FA90EF3B294D6D549C0A0D88E814B5A2C17794B1EBD1A064868BB377DFD5CD80218D73D3F0A18B642EFAFACEDDBA6E2694D27F3E7EE3B04A57D6CBBE452343D9428133C5209D763B2F776628529B94173B3568");  // 1009
  temp4 = ConstString[change_cnt] - 0x01;
  if (temp4 < 0x30)
    {
		temp4 = 0x30;
		change_cnt++;
		if (change_cnt == 251) {change_cnt = 0;}
      temp4 = ConstString[change_cnt] - 0x01;
	}
  if ((temp4 < 0x41) && (temp4 > 0x39)) temp4 = 0x39;
  ConstString[change_cnt] = temp4;
  change_cnt++;
  if (change_cnt == 251) {change_cnt = 0;}

//    Delayms(1000);  // extra one second delay to allow periods to show up


    temp = 0x00;
//    for (jloop = 0; jloop <4; jloop++)
//    {
//      while (temp != '.')
//      {
//          temp = U1RXREG;
//          Delayms(10);
//      }
//      temp = 0x00;
//    }
    while (U1RXREG != '1')  // [ = 0x5B, ] = 0x5D
    {
      putConsole(U1RXREG);
  Delayms(200);
    }

    putsConsole(ConstString);
  Delayms(500);
    putsWiFly(ConstString);

//  Delayms(7000);  // works OK when ftp_timeout = 40, 1 every 12 sec
//  Delayms(21000);// works OK when ftp_timeout = 40, 1 every 24 sec
  Delayms(17000);  // works OK when ftp_timeout = 40, 1 every 12 sec

  sprintf(ConstString,"$$$");
    putsConsole(ConstString);
  Delayms(500);
    putsWiFly(ConstString);
  Delayms(500);


  setLCDC(0x00); // point to 1st line
  Delayms(200);
    sprintf(OutString, "iloop =         \nLoop to 1000    ");
    temp1 = (iloop  % 10);
    ones_char = temp1 + 0x30;
//    temp2 = (iloop - temp1)/10;
    temp2 = ((iloop - temp1) % 100)/10;
    tens_char = temp2 + 0x30;
    temp3 = (iloop - temp2*10 - temp1)/100;
    huns_char = temp3 + 0x30;
    OutString[8] = huns_char;
    OutString[9] = tens_char;
    OutString[10] = ones_char;
    putsLCD(OutString);

  Delayms(1000);  // worked well with 10000
    }

// JKW ends here

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

//    set ftp address 198.175.253.161
//    set ftp dir ./public
//    set ftp user roving
//    set ftp pass Pass123
//    ftp update wifly7-236.img
// Sparkfun electronics
//"Helping Connect the World...One Machine at a Time" - GSM, CDMA, Wi-Fi, Ethernet, Bluetooth, ZigBee & GPS


