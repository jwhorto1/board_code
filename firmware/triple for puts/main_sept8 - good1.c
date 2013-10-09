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

// Setup configuration bits
// JTAG/Code Protect/Write Protect/Clip-on Emulation mode
// Watchdog Timer/ICD pins select
//_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2)

// Disable CLK switch and CLK monitor, OSCO or Fosc/2, HS oscillator,
// Primary oscillator
//_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)

// GLOBALS
unsigned char   TenSecondTick;
unsigned char	TimeSecBCD;
unsigned char	TimeMinBCD;
unsigned char	TimeHourBCD;
unsigned char	TimeDayOfWeekBCD;
unsigned char	TimeDayBCD;
unsigned char	TimeMonthBCD;
unsigned char	TimeYearBCD;

unsigned char	TimeSecBinary;
unsigned char	TimeMinBinary;
unsigned char	TimeHourBinary;
unsigned char	TimeDayOfWeekBinary;
unsigned char	TimeDayBinary;
unsigned char	TimeMonthBinary;
unsigned char	TimeYearBinary;


unsigned char GreenLEDState;
unsigned char RedLEDState;

unsigned char TimeMinLeftOver;
unsigned int  kickout_seconds;
unsigned int  time_mark1;
unsigned int  time_mark2;
unsigned char TimeHourBinarySave;

// Union to access rtcc registers
typedef union tagRTCC {
	struct {
		unsigned char sec;
		unsigned char min;
		unsigned char hr;
		unsigned char wkd;
		unsigned char day;
		unsigned char mth;
		unsigned char yr;
	};
	struct {
		unsigned int prt00;
		unsigned int prt01;
		unsigned int prt10;
		unsigned int prt11;
	};
} RTCC;

RTCC _time;
RTCC _time_chk;
RTCC _alarm;

// macro
#define mRTCCDec2Bin(Dec) (10*(Dec>>4)+(Dec&0x0f))
#define mRTCCBin2Dec(Bin) (((Bin/10)<<4)|(Bin%10))


void RTCCgrab(void);
void RTCCInit(void);
void RTCCSet(void);
void RTCCALMSet(void);
void RTCCUnlock(void);
void RTCCSetBinSec(unsigned char );
void RTCCSetBinMin(unsigned char );
void RTCCSetBinHour(unsigned char );
void RTCCCalculateWeekDay(void);
void RTCCSetBinDay(unsigned char );
void RTCCSetBinMonth(unsigned char );
void RTCCSetBinYear(unsigned char );




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

     ODCAbits.ODA6 		= 0;  	// This is for the Explorer-16 Demo Board
     TRISAbits.TRISA6 	= 0;		// This is for the Explorer-16 Demo Board
 //  ODCCbits.ODC2 		= 0;
 //  TRISCbits.TRISC2 	= 0;

 unsigned long i;

 	GreenLEDState = 0;
 	RedLEDState = 0;

        RTCCInit();		// initialization includes set the time and date

        TimeMinBinary = 0;
 	while (TimeMinBinary < 3)
        {
	if (TenSecondTick == 1)
	{
        RTCCgrab();
        TimeHourBinary = mRTCCDec2Bin(TimeHourBCD);
        TimeMinBinary = mRTCCDec2Bin(TimeMinBCD);
        TimeSecBinary = mRTCCDec2Bin(TimeSecBCD);
        TimeDayOfWeekBinary = mRTCCDec2Bin(TimeDayOfWeekBCD);
        TimeDayBinary = mRTCCDec2Bin(TimeDayBCD);
        TimeMonthBinary = mRTCCDec2Bin(TimeMonthBCD);
        TimeYearBinary = mRTCCDec2Bin(TimeYearBCD);


    sprintf(OutString, "  :  :          \n  /  /   20     ");  // columns 41, 59
//  sprintf(OutString, "Sept4 1:00PM    \nputs only       ");  // columns 41, 59
    temp1 = (TimeHourBinary  % 10);
    ones_char = temp1 + 0x30;
    temp2 = (TimeHourBinary - temp1)/10;
    tens_char = temp2 + 0x30;
    OutString[0] = tens_char;
    OutString[1] = ones_char;

    temp1 = (TimeMinBinary  % 10);
    ones_char = temp1 + 0x30;
    temp2 = (TimeMinBinary - temp1)/10;
    tens_char = temp2 + 0x30;
    OutString[3] = tens_char;
    OutString[4] = ones_char;

    temp1 = (TimeSecBinary  % 10);
    ones_char = temp1 + 0x30;
    temp2 = (TimeSecBinary - temp1)/10;
    tens_char = temp2 + 0x30;
    OutString[6] = tens_char;
    OutString[7] = ones_char;

    temp1 = (TimeDayOfWeekBinary  % 10);
    ones_char = temp1 + 0x30;
    temp2 = (TimeDayOfWeekBinary - temp1)/10;
    tens_char = temp2 + 0x30;
    OutString[17] = tens_char;
    OutString[18] = ones_char;

    temp1 = (TimeDayBinary  % 10);
    ones_char = temp1 + 0x30;
    temp2 = (TimeDayBinary - temp1)/10;
    tens_char = temp2 + 0x30;
    OutString[20] = tens_char;
    OutString[21] = ones_char;

    temp1 = (TimeMonthBinary  % 10);
    ones_char = temp1 + 0x30;
    temp2 = (TimeMonthBinary - temp1)/10;
    tens_char = temp2 + 0x30;
    OutString[23] = tens_char;
    OutString[24] = ones_char;

    temp1 = (TimeYearBinary  % 10);
    ones_char = temp1 + 0x30;
    temp2 = (TimeYearBinary - temp1)/10;
    tens_char = temp2 + 0x30;
    OutString[28] = tens_char;
    OutString[29] = ones_char;

    Delayms(200);
    setLCDC(0x00); // point to 1st line
    Delayms(2000);
    putsLCD(OutString);
    TenSecondTick = 0;
} // if (TenSecondTick == 1)
} // while(TimeMinBinary < 4)

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
//  sprintf(OutString,"set wlan ssid AMI\r\n");
//  sprintf(OutString,"set wlan ssid Arthur$and$Matts$Internet\r\n");
    sprintf(OutString,"set wlan ssid Hogwarts\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
//    sprintf(OutString,"set wlan phrase hot7blue\r\n");
    sprintf(OutString,"set wlan phrase pooKums496\r\n");
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
//  Delayms(1000);
//    sprintf(OutString,"get everything\r\n");
//    putsConsole(OutString);
//    putsWiFly(OutString);
  Delayms(200);
    sprintf(OutString, "Sept7 8:00AM    \nputs only       ");  // columns 41, 59
    putsConsole(OutString);
  Delayms(200);
   setLCDC(0x00); // point to 1st line
  Delayms(200);
    putsLCD(OutString);
  Delayms(2000);
    sprintf(OutString, "\r\n");
    putsConsole(OutString);

  change_cnt = 0;
//  sprintf(ConstString,"A9ABE296E34713800CC11D0D1E0FB197B36810D14F32085E4E50BFB685EC755E26C88F63A105284B0AC627FA90EF3B294D6D549C0A0D88E814B5A2C17794B1EBD1A064868BB377DFD5CD80218D73D3F0A18B642EFAFACEDDBA6E2694D27F3E7EE3B04A57D6CBBE452343D9428133C5209D763B2F776628529B94173B3568");  // 1009


  for (iloop = 0; iloop <=999; iloop++)
    {
  Delayms(500);  // worked well with 5000
    sprintf(AddrString,"ftp put sept9__000.txt\r\n");
//  sprintf(AddrString,"ftp put arthur_000.txt\r\n");
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

    temp = 0x00;
    kickout_seconds = 0;  // kicks out if takes longer than 2 minutes
    RTCCgrab();
    TimeHourBinary = mRTCCDec2Bin(TimeHourBCD);
    TimeMinBinary = mRTCCDec2Bin(TimeMinBCD);
    TimeSecBinary = mRTCCDec2Bin(TimeSecBCD);
    time_mark1 = TimeHourBinary*3600 + TimeMinBinary*60+ TimeSecBinary;
    TimeHourBinarySave = TimeHourBinary;

    while ( (U1RXREG != '1') && (kickout_seconds < 60) )  // [ = 0x5B, ] = 0x5D
    {
        putConsole(U1RXREG);
    	RTCCgrab();
    	TimeHourBinary = mRTCCDec2Bin(TimeHourBCD);
    	TimeMinBinary = mRTCCDec2Bin(TimeMinBCD);
    	TimeSecBinary = mRTCCDec2Bin(TimeSecBCD);
    	time_mark2 = TimeHourBinary*3600 + TimeMinBinary*60+ TimeSecBinary;
    	if (TimeHourBinary < TimeHourBinarySave) time_mark2 = time_mark2 + 24*3600;
    	kickout_seconds = time_mark2 - time_mark1;
        Delayms(200);
    }
    if (kickout_seconds < 60)
    {
    putsConsole(ConstString);
  Delayms(500);
    putsWiFly(ConstString);
//  Delayms(7000);  // works OK when ftp_timeout = 40, 1 every 12 sec
//  Delayms(21000);// works OK when ftp_timeout = 40, 1 every 24 sec
  Delayms(12000);  // works OK when ftp_timeout = 40, 1 every 16 sec
    }
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



/*********************************************************************
 * Function: RTCCProcessEvents
 *
 * Preconditions: RTCCInit must be called before.
 *
 * Overview: The function grabs the current time from the RTCC and
 * translate it into strings.
 *
 * Input: None.
 *
 * Output: It update time and date strings  _time_str, _date_str,
 * and _time, _time_chk structures.
 *
 ********************************************************************/
void RTCCgrab(void)
{
	// Process time object only if time is not being set

		// Grab the time
		RCFGCALbits.RTCPTR = 3;
		_time.prt11 = RTCVAL;
		_time.prt10 = RTCVAL;
		_time.prt01 = RTCVAL;
		_time.prt00 = RTCVAL;

		// Grab the time again
		RCFGCALbits.RTCPTR = 3;
		_time_chk.prt11 = RTCVAL;
		_time_chk.prt10 = RTCVAL;
		_time_chk.prt01 = RTCVAL;
		_time_chk.prt00 = RTCVAL;

		// Verify there is no roll-over
		if ((_time.prt00 == _time_chk.prt00) &&
			(_time.prt01 == _time_chk.prt01) &&
			(_time.prt10 == _time_chk.prt10) &&
			(_time.prt11 == _time_chk.prt11))
			{
			// Here, you can watch structure _time,
			//	which has the data from RTCC registers.

			// TO DO: do something as you like here.

			TimeSecBCD = _time_chk.sec;
			TimeMinBCD = _time_chk.min;
			TimeHourBCD = _time_chk.hr;
			TimeDayOfWeekBCD = _time_chk.wkd;
			TimeDayBCD = _time_chk.day;
			TimeMonthBCD = _time_chk.mth;
			TimeYearBCD = _time_chk.yr;
			}

		else {TimeHourBCD = 37;} // Return a bad value, TimeHourBinary = 25

}






/*********************************************************************
 * Function: RTCCInit
 *
 * Preconditions: RTCCInit must be called before.
 *
 * Overview: Enable the oscillator for the RTCC
 *
 * Input: None.
 *
 * Output: None.
 ********************************************************************/
void RTCCInit(void)
{
    // Enables the LP OSC for RTCC operation
	asm("mov #OSCCON,W1");	// move address of OSCCON to W1
	asm("mov.b #0x02, W0");	// move 8-bit literal to W0, 16-bit.
	asm("mov.b #0x46, W2");	// unlock byte 1 for OSCCONL(low byte)
	asm("mov.b #0x57, W3");	// unlock byte 2 for OSCCONL(low byte)
							// move 8-bit of Wn to OSCCON register
	asm("mov.b W2, [W1]");	// write unlock byte 1
	asm("mov.b W3, [W1]");	// write unlock byte 2
	asm("mov.b W0, [W1]");	// enable SOSCEN

    // Unlock sequence must take place for RTCEN to be written
	RCFGCAL	= 0x0000;
    RTCCUnlock();
    RCFGCALbits.RTCEN = 1;	// bit15

    //RTCC pin pad conected to RTCC second clock
	PADCFG1bits.RTSECSEL = 1;
	RCFGCALbits.RTCOE = 1;		//RTCC Output Enable bit

	/* Enable the RTCC interrupt*/
	IFS3bits.RTCIF = 0;		//clear the RTCC interrupt flag
	IEC3bits.RTCIE = 1;		//enable the RTCC interrupt

    // TO DO: Write the time and date to RTCC as follow.
	_time_chk.sec = mRTCCBin2Dec(0);
	_time_chk.min = mRTCCBin2Dec(1);  // 40 minutes
	_time_chk.hr =  mRTCCBin2Dec(2);   //7PM
	_time_chk.wkd = 3;
	_time_chk.day = mRTCCBin2Dec(4);
	_time_chk.mth = mRTCCBin2Dec(5);
	_time_chk.yr = mRTCCBin2Dec(13);
	RTCCCalculateWeekDay();	// To calculate and confirm the weekday

	// Set it after you change the time and date.
	RTCCSet();

	// Set alarm
	_alarm.sec	= 0x01;
	_alarm.min	= 0x01;
	_alarm.hr	= 0x01;
	_alarm.wkd	= 0x01;
	_alarm.day	= 0x01;
	_alarm.mth	= 0x01;
	RTCCALMSet();
}

/*********************************************************************
 * Function: RTCCSet
 *
 * Preconditions: None.
 *
 * Overview:
 * The function upload time and date from _time_chk into clock.
 *
 * Input: _time_chk - structure containing time and date.
 *
 * Output: None.
 *
 ********************************************************************/
void RTCCSet(void)
{
	RTCCUnlock();				// Unlock the RTCC

	RCFGCALbits.RTCPTR = 3;		// Set the time
	RTCVAL = _time_chk.prt11;	// set year
	RTCVAL = _time_chk.prt10;	// set month:day
	RTCVAL = _time_chk.prt01;	// set week:hour
	RTCVAL = _time_chk.prt00;	// set min:sec

	RCFGCALbits.RTCWREN = 0;	// Lock the RTCC

	// Here, you can watch the RTCC register,
	// 	the new time and date has been updated.
}

/*********************************************************************
 * Function: RTCCALMSet
 *
 * Preconditions: None.
 *
 * Overview:
 * The function upload time and date from _alarm into RTCC alarm.
 *
 * Input: _alarm - structure containing time and date.
 *
 * Output: None.
 *
 ********************************************************************/
void RTCCALMSet(void)
{
	RTCCUnlock();				// Unlock the RTCC
	while(RCFGCALbits.RTCSYNC==1);		//wait for RTCSYNC bit to become 0

	ALCFGRPTbits.ALRMEN		= 0;		//disable alarm to update it
	ALCFGRPTbits.ALRMPTR	= 2;  	 	//Point to Month/Day register
	ALRMVAL = _alarm.prt10;				//load month & day
	ALRMVAL = _alarm.prt01;				//load weekday & hour
	ALRMVAL = _alarm.prt00;				//load minute & seconds

	ALCFGRPTbits.AMASK		= 2;		//alarm every 10 seconds
	ALCFGRPTbits.ARPT		= 0xff;		//alarm 255 times
	ALCFGRPTbits.CHIME		= 1;		//enable chime
    ALCFGRPTbits.ALRMEN		= 1;  	 	//enable the alarm

	RCFGCALbits.RTCWREN = 0;	// Lock the RTCC
}

/*********************************************************************
 * Function: RTCCUnlock
 *
 * Preconditions: None.
 *
 * Overview: The function allows a writing into the clock registers.
 *
 * Input: None.
 *
 * Output: None.
 *
 ********************************************************************/
void RTCCUnlock()
{
	asm volatile("disi	#5");
	asm volatile("mov #0x55, w7");		// write 0x55 and 0xAA to
	asm volatile("mov w7, _NVMKEY"); 	//  NVMKEY to disable
	asm volatile("mov #0xAA, w8");		// 	write protection
	asm volatile("mov w8, _NVMKEY");
    asm volatile("bset _RCFGCAL, #13");	// set the RTCWREN bit
	asm volatile("nop");
	asm volatile("nop");
}

/*********************************************************************
 * Function: RTCCSetBinSec
 *
 * Preconditions: None.
 *
 * Overview: The function verifies setting seconds range, translates
 * it into BCD format and writes into _time_chk structure. To write
 * the structure into clock RTCCSet must be called.
 *
 * Input: Seconds binary value.
 *
 * Output: Checked BCD value in _time_chk structure.
 *
 ********************************************************************/
void RTCCSetBinSec(unsigned char Sec)
{
    if(Sec >= 60)  Sec = 0;
    _time_chk.sec = mRTCCBin2Dec(Sec);
}

/*********************************************************************
 * Function: RTCCSetBinMin
 *
 * Preconditions: None.
 *
 * Overview: The function verifies a setting minutes range, translates
 * it into BCD format and writes into _time_chk structure. To write
 * the structure into clock RTCCSet must be called.
 *
 * Input: Minutes binary value.
 *
 * Output: Checked BCD value in _time_chk structure.
 *
 ********************************************************************/
void RTCCSetBinMin(unsigned char Min)
{
    if(Min >= 60)  Min = 0;
    _time_chk.min = mRTCCBin2Dec(Min);
}

/*********************************************************************
 * Function: RTCCSetBinHour
 *
 * Preconditions: None.
 *
 * Overview: The function verifies a setting hours range, translates
 * it into BCD format and writes into _time_chk structure. To write
 * the structure into clock RTCCSet must be called.
 *
 * Input: Hours binary value.
 *
 * Output: Checked BCD value in _time_chk structure.
 *
 ********************************************************************/
void RTCCSetBinHour(unsigned char Hour)
{
    if(Hour >= 24) Hour = 0;
    _time_chk.hr = mRTCCBin2Dec(Hour);
}

/*********************************************************************
 * Function: RTCCCalculateWeekDay
 *
 * Preconditions:
 * Valid values of day, month and year must be presented in
 * _time_chk structure.
 *
 * Overview: The function reads day, month and year from _time_chk and
 * calculates week day. Than It writes result into _time_chk. To write
 * the structure into clock RTCCSet must be called.
 *
 * Input: _time_chk with valid values of day, month and year.
 *
 * Output: Zero based week day in _time_chk structure.
 *
 ********************************************************************/
void RTCCCalculateWeekDay()
{
	const char MonthOffset[] =
	//jan feb mar apr may jun jul aug sep oct nov dec
	{   0,  3,  3,  6,  1,  4,  6,  2,  5,  0,  3,  5 };
	unsigned Year;
	unsigned Month;
	unsigned Day;
	unsigned Offset;
    // calculate week day
    Year  = mRTCCDec2Bin(_time_chk.yr);
    Month = mRTCCDec2Bin(_time_chk.mth);
    Day  = mRTCCDec2Bin(_time_chk.day);

    // 2000s century offset = 6 +
    // every year 365%7 = 1 day shift +
    // every leap year adds 1 day
    Offset = 6 + Year + Year/4;
    // Add month offset from table
    Offset += MonthOffset[Month-1];
    // Add day
    Offset += Day;

    // If it's a leap year and before March there's no additional day yet
    if((Year%4) == 0)
        if(Month < 3)
            Offset -= 1;

    // Week day is
    Offset %= 7;

    _time_chk.wkd = Offset;
}

/*********************************************************************
 * Function: RTCCSetBinDay
 *
 * Preconditions: None.
 *
 * Overview: The function verifies a setting day range, translates it
 * into BCD format and writes into _time_chk structure. To write the
 * structure into clock RTCCSet must be called.
 *
 * Input: Day binary value.
 *
 * Output: Checked BCD value in _time_chk structure.
 *
 ********************************************************************/
void RTCCSetBinDay(unsigned char Day)
{
	const char MonthDaymax[] =
	//jan feb mar apr may jun jul aug sep oct nov dec
	{  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	unsigned Daymax;
	unsigned Month;
	unsigned Year;

    Month = mRTCCDec2Bin(_time_chk.mth);
    Year = mRTCCDec2Bin(_time_chk.yr);

    Daymax = MonthDaymax[Month-1];

    // February has one day more for a leap year
    if(Month == 2)
    if( (Year%4) == 0)
        Daymax++;

    if(Day == 0) Day = Daymax;
    if(Day > Daymax) Day = 1;
    _time_chk.day = mRTCCBin2Dec(Day);
}

/*********************************************************************
 * Function: RTCCSetBinMonth
 *
 * Preconditions: None.
 *
 * Overview: The function verifies a setting month range, translates
 * it into BCD format and writes into _time_chk structure. To write
 * the structure into clock RTCCSet must be called.
 *
 * Input: Month binary value.
 *
 * Output: Checked BCD value in _time_chk structure.
 *
 ********************************************************************/
void RTCCSetBinMonth(unsigned char Month)
{
    if(Month < 1) Month = 12;
    if(Month > 12) Month = 1;
    _time_chk.mth = mRTCCBin2Dec(Month);
}

/*********************************************************************
 * Function: RTCCSetBinYear
 *
 * Preconditions: None.
 *
 * Overview: The function verifies a setting year range, translates it
 * into BCD format and writes into _time_chk structure. To write the
 * structure into clock RTCCSet must be called.
 *
 * Input: Year binary value.
 *
 * Output: Checked BCD value in _time_chk structure.
 *
 ********************************************************************/
void RTCCSetBinYear(unsigned char Year)
{
   if(Year >= 100)  Year = 0;
    _time_chk.yr = mRTCCBin2Dec(Year);
    // Recheck day. Leap year influences to Feb 28/29.
    RTCCSetBinDay(mRTCCDec2Bin(_time_chk.day));
}

/*********************************************************************
 * Interrupt for RTCC Alarm.
 *
 ********************************************************************/
void __attribute__((interrupt, no_auto_psv)) _RTCCInterrupt(void)
{
	IFS3bits.RTCIF = 0;

//	__builtin_btg((unsigned int *)&LATA, 6);	// Toggle RA6
//	__builtin_btg((unsigned int *)&LATC, 2);	// Toggle RC2

	// TO DO:
	// User Implementation.

	if (GreenLEDState == 0) GreenLEDState = 1;
	else GreenLEDState = 0;
	PORTAbits.RA6 = GreenLEDState;   // Was RC2

	TenSecondTick = 1;
//	if (TenSecondTick == 6)
//		{
//			__builtin_btg((unsigned int *)&LATC, 2);	// Toggle RC2  //60 seconds on, 60 off
//			TenSecondTick = 0;
//		}

}



