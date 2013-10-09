/**********************************************************************
* ?2007 Microchip Technology Inc.
*
* FileName:        RTCC.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC24Fxxxx
* Compiler:        MPLAB?C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your
* behalf, exclusively with Microchip's proprietary products.  Your
* acceptance and/or use of this code constitutes agreement to the
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT,
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY,
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author       	Date      	Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Albert Z.		03/24/08	First release of source file
* Albert Z.		07/10/08	Add RTCC alarm
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Copy the RTCC functions to your application.
*
* Code Tested on:
* Explorer 16 Development Board with  PIC24FJ128GA010 controller
*
* DESCRIPTION:
* Microchip's PIC24F microcontrollers feature a useful Real Time Clock
* for users to get an on chip clock in their applications.
* This code example demonstrates how to grab and set the RTC, set alarm
* and RTCC clock output / alarm output.
**********************************************************************/
#include <p24fxxxx.h>

#include <p24FJ192GA110.h>


// Setup configuration bits
// JTAG/Code Protect/Write Protect/Clip-on Emulation mode
// Watchdog Timer/ICD pins select
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2)

// Disable CLK switch and CLK monitor, OSCO or Fosc/2, HS oscillator,
// Primary oscillator
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)

// GLOBALS
unsigned char   TenSecondTick;
unsigned char	TimeSecBCD;
unsigned char	TimeMinBCD;
unsigned char	TimeHourBCD;
unsigned char	TimeDayOfWeekBCD;
unsigned char	TimeDayBCD;
unsigned char	TimeMonthBCD;
unsigned char	TimeYearBCD;

unsigned char	TimeMinBinary;
unsigned char	TimeHourBinary;


unsigned char GreenLEDState;
unsigned char RedLEDState;

unsigned char TimeMinLeftOver;

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



// main
int main(void)
{

//unsigned char Temp1, Temp2;

//Temp1 = 0b00010011;  //BCD 13
//Temp2 = Temp1&0xF0;
//Temp2 = (Temp1&0xF0) >> 4;
//Temp2 = (Temp1&0x0F);
//Temp2 = 10* ( (Temp1&0xF0) >> 4)  + (Temp1&0x0F);

//Temp1 = mRTCCBin2Dec(13);
//Temp2 = 10* ( (Temp1&0xF0) >> 4)  + (Temp1&0x0F);

//Temp1 = mRTCCBin2Dec(97);
//Temp2 = 10* ( (Temp1&0xF0) >> 4)  + (Temp1&0x0F);

//Temp1 = mRTCCBin2Dec(49);
//Temp2 = 10* ( (Temp1&0xF0) >> 4)  + (Temp1&0x0F);

//Temp1 = mRTCCBin2Dec(20);
//Temp2 = 10* ( (Temp1&0xF0) >> 4)  + (Temp1&0x0F);



	// I/O initialization
//    ODCAbits.ODA6 		= 0;  	// This is for the Explorer-16 Demo Board
//	TRISAbits.TRISA6 	= 0;		// This is for the Explorer-16 Demo Board
    ODCCbits.ODC2 		= 0;
	TRISCbits.TRISC2 	= 0;

unsigned long i;

	GreenLEDState = 0;
	RedLEDState = 0;

    RTCCInit();		// initialization includes set the time and date

	TimeMinBinary = 0;

	//while (1);
	// TO DO:
	// observe D9 on Explorer 16, it will toggle every 10 seconds as set.
	// probe RTCC pin on #68 of PIC24FJ128GA010, it is a second clock of RTCC.
	// Changed to RC2 on my board from RA6 on demo board

	TRISA = 0x3913; // Setup RA2, RA3, RA6, RA7, RA9, RA10, RA14 and RA15 as outputs
					// if the value is 0 it is an output, 1 is input
					// RA0, U1_U2_JTAGTMS is input....1
					// RA1, U1_U2_JTAGTCK is input....1
					// RA2, CH5_CTRL is output........0
					// RA3, CH6_CTRL is output........0

					// RA4, U1_U2_JTAGTDI is input....1
					// RA5, U1_U2_JTAGTDO is output...0
					// RA6, CH15_CTRL is output.......0
					// RA7, CH16_CTRL is output.......0

					// RA8, doesn't exist.............1
					// RA9, CH2_CTRL..................0
					// RA10, CH1_CTRL is output.......0
					// RA11, doesn't exist............1

					// RA12, doesn't exist............1
					// RA13, doesn't exist............1
					// RA14, CH7_CTRL is output.......0
					// RA15, CH8_CTRL is output.......0

	ODCA = 0x0000;  // no open drains on PORTA

	TRISB = 0xFFFF; // No outputs on RB.... for now
					// if the value is 0 it is an output, 1 is input

					// RB0, CURR_SENSES is ANALOG input..1 (for now)
					// RB1, spare.....................1
					// RB2, U2_CSb....................1 (later change to output)
					// RB3, pre_U3_CSb................1 (later change to output)

					// RB4, spare.....................1
					// RB5, spare.....................1
					// RB6, ICD_RB6...................1  (???)
					// RB7, ICD_RB7...................1  (???)

					// RB8, REV_CENTER is ANALOG input..1 (for now)
					// RB9, REV_MAJOR is ANALOG input...1 (for now)
					// RB10, REV_MINOR is ANALOG input..1 (for now)
					// RB11, spare....................1

					// RB12, RM12/PMPA11..............1 (???)
					// RB13, spare....................1
					// RB14, spare....................1
					// RB15, spare....................1

	ODCB = 0x0000;  // no open drains on PORTB

	TRISC = 0xFFF9; // Setup RC1, RC2 as outputs
					// if the value is 0 it is an output, 1 is input
					// RA0, doesn't exist.............1
					// RA1, RED_BOARD_LED is output...0
					// RA2, GRN_BOARD_LED is output...0
					// RA3, EXPAND_CHANS is input.....1

					// RC4, spare.....................1
					// RC5, doesn't exist.............1
					// RC6, doesn't exist.............1
					// RC7, doesn't exist.............1

					// RC8, doesn't exist.............1
					// RC9, doesn't exist.............1
					// RC10, doesn't exist............1
					// RC11, doesn't exist............1

					// RA12, 8MHZ_OSC1, consider input..1
					// RA13, 32KHZ_OSC1, consider input.1
					// RA14, 32KHZ_OSC2, consider input.1
					// RA15, 8MHZ_OSC2, consider input..1

	ODCC = 0x0000;  // no open drains on PORTC

	TRISD = 0x00DF; // Setup RD8 thru RD15 as outputs
					// if the value is 0 it is an output, 1 is input
					// RD0, spare.....................1
					// RD1, spare.....................1
					// RD2, spare.....................1
					// RD3, spare.....................1

					// RD4, spare.....................1
					// RD5, ACTIVATE_24V..............0
					// RD6, SW2, is input.............1
					// RD7, SW3, is input.............1

					// RD8, CH10_CTRL is output.......0
					// RD9, CH9_CTRL is output.......0
					// RD10, CH11_CTRL is output......0
					// RD11, CH12_CTRL is output......0

					// RD12, CH13_CTRL is output......0
					// RD13, CH14_CTRL is output.......0
					// RD14, CH3_CTRL is output.......0
					// RD15, CH4_CTRL is output.......0

	ODCD = 0x0000;  // no open drains on PORTD

					PORTDbits.RD5 = 0;  // ACTIVATE_24V
					PORTAbits.RA10 =0;  // CHAN 1
					PORTAbits.RA9 = 0;  // CHAN 2
					PORTDbits.RD14 = 0; // CHAN 3
					PORTDbits.RD15 = 0; // CHAN 4
					PORTAbits.RA2 = 0;  // CHAN 5
					PORTAbits.RA3 = 0;  // CHAN 6



// ############## short message processing  #################################

unsigned char ActiveFlag, ActiveChannel;

TenSecondTick = 0;


while (1)  // PERPETUAL
	{



if (TenSecondTick == 1)
	{
	
	RTCCgrab();		// get latest time/day/date  REAL RTCC

	TimeHourBinary = 10* ( (TimeHourBCD & 0xF0) >> 4) + (TimeHourBCD & 0x0F);
	TimeMinBinary  = 10* ( (TimeMinBCD & 0xF0) >> 4)  + (TimeMinBCD & 0x0F);

	while (TimeHourBinary > 24)
		{RTCCgrab();}


	if (GreenLEDState == 1) // Flash out hours
	{
	// long hour pulses
		if (TimeHourBinary >=15)
			{
			PORTCbits.RC1 = 1;  // Red LED
			for (i = 0; i<250000; i++) {asm volatile("nop");}
			PORTCbits.RC1 = 0;
			for (i = 0; i<100000; i++) {asm volatile("nop");}
			// medium hour pulses
			if ( (TimeHourBinary - 15) >= 5 )
				{
				PORTCbits.RC1 = 1;  // Red LED
				for (i = 0; i<75000; i++) {asm volatile("nop");;}
				PORTCbits.RC1 = 0;
				for (i = 0; i<100000; i++) {asm volatile("nop");}
				}
			}
		else
			{
			// medium hour pulses
			if (TimeHourBinary >=10)
				{
				PORTCbits.RC1 = 1;  // Red LED
				for (i = 0; i<75000; i++) {asm volatile("nop");}
				PORTCbits.RC1 = 0;
				for (i = 0; i<100000; i++) {asm volatile("nop");}
				}
			if (TimeHourBinary >=5)
				{
				PORTCbits.RC1 = 1;  // Red LED
				for (i = 0; i<75000; i++) {asm volatile("nop");}
				PORTCbits.RC1 = 0;
				for (i = 0; i<100000; i++) {asm volatile("nop");}
				}
			}

	// tiny hour pulses
		if (TimeHourBinary%5 >=4)
		{
		PORTCbits.RC1 = 1;  // Red LED
		for (i = 0; i<15000; i++) {asm volatile("nop");;}
		PORTCbits.RC1 = 0;
		for (i = 0; i<75000; i++) {asm volatile("nop");}
		}
		if (TimeHourBinary%5 >=3)
		{
		PORTCbits.RC1 = 1;  // Red LED
		for (i = 0; i<15000; i++) {asm volatile("nop");;}
		PORTCbits.RC1 = 0;
		for (i = 0; i<75000; i++) {asm volatile("nop");}
		}
		if (TimeHourBinary%5 >=2)
		{
		PORTCbits.RC1 = 1;  // Red LED
		for (i = 0; i<15000; i++) {asm volatile("nop");;}
		PORTCbits.RC1 = 0;
		for (i = 0; i<75000; i++) {asm volatile("nop");}
		}
		if (TimeHourBinary%5 >=1)
		{
		PORTCbits.RC1 = 1;  // Red LED
		for (i = 0; i<15000; i++) {asm volatile("nop");;}
		PORTCbits.RC1 = 0;
		for (i = 0; i<75000; i++) {asm volatile("nop");}
		}

	}

	if (GreenLEDState == 0) // Flash out minutes
  	{

	// long minute pulses
		TimeMinLeftOver = TimeMinBinary;
		if (TimeMinBinary >=45)
			{
			PORTCbits.RC1 = 1;  // Red LED
			for (i = 0; i<250000; i++) {asm volatile("nop");}
			PORTCbits.RC1 = 0;
			for (i = 0; i<100000; i++) {asm volatile("nop");}
			TimeMinLeftOver = TimeMinLeftOver - 15;
			}
		if (TimeMinBinary >=30)
			{
			PORTCbits.RC1 = 1;  // Red LED
			for (i = 0; i<250000; i++) {asm volatile("nop");}
			PORTCbits.RC1 = 0;
			for (i = 0; i<100000; i++) {asm volatile("nop");}
			TimeMinLeftOver = TimeMinLeftOver - 15;
			}
		if (TimeMinBinary >=15)
			{
			PORTCbits.RC1 = 1;  // Red LED
			for (i = 0; i<250000; i++) {asm volatile("nop");}
			PORTCbits.RC1 = 0;
			for (i = 0; i<100000; i++) {asm volatile("nop");}
			TimeMinLeftOver = TimeMinLeftOver - 15;
			}

	// wait before next set of medium pulses
	for (i = 0; i<150000; i++) {asm volatile("nop");;}


	// medium min pulses
			if ( TimeMinLeftOver >= 10 )
				{
				PORTCbits.RC1 = 1;  // Red LED
				for (i = 0; i<75000; i++) {asm volatile("nop");;}
				PORTCbits.RC1 = 0;
				for (i = 0; i<100000; i++) {asm volatile("nop");}
				}
			if ( TimeMinLeftOver >= 5 )
				{
				PORTCbits.RC1 = 1;  // Red LED
				for (i = 0; i<75000; i++) {asm volatile("nop");;}
				PORTCbits.RC1 = 0;
				for (i = 0; i<100000; i++) {asm volatile("nop");}
				}
			

	// wait before next set of tiny pulses
	for (i = 0; i<150000; i++) {asm volatile("nop");;}

	// tiny minute pulses
		if (TimeMinBinary%5 >=4)
		{
		PORTCbits.RC1 = 1;  // Red LED
		for (i = 0; i<15000; i++) {asm volatile("nop");;}
		PORTCbits.RC1 = 0;
		for (i = 0; i<75000; i++) {asm volatile("nop");}
		}
		if (TimeMinBinary%5 >=3)
		{
		PORTCbits.RC1 = 1;  // Red LED
		for (i = 0; i<15000; i++) {asm volatile("nop");;}
		PORTCbits.RC1 = 0;
		for (i = 0; i<75000; i++) {asm volatile("nop");}
		}
		if (TimeMinBinary%5 >=2)
		{
		PORTCbits.RC1 = 1;  // Red LED
		for (i = 0; i<15000; i++) {asm volatile("nop");;}
		PORTCbits.RC1 = 0;
		for (i = 0; i<75000; i++) {asm volatile("nop");}
		}
		if (TimeMinBinary%5 >=1)
		{
		PORTCbits.RC1 = 1;  // Red LED
		for (i = 0; i<15000; i++) {asm volatile("nop");;}
		PORTCbits.RC1 = 0;
		for (i = 0; i<75000; i++) {asm volatile("nop");}
		}

	}


	ActiveFlag = 0;
	ActiveChannel = 16;
// /* ********************FRONT YARD ***************
	//if ((TimeHourBinary == 14) && (TimeMinBinary >= 10) && (TimeMinBinary <= 11) )
 	//		{ActiveFlag = 1; ActiveChannel = 0;}
	if ((TimeHourBinary == 07) && (TimeMinBinary >= 15) && (TimeMinBinary <= 20) )
			{ActiveFlag = 1; ActiveChannel = 1;}
	if ((TimeHourBinary == 07) && (TimeMinBinary >= 22) && (TimeMinBinary <= 27) )
			{ActiveFlag = 1; ActiveChannel = 2;}
	if ((TimeHourBinary == 07) && (TimeMinBinary >= 29) && (TimeMinBinary <= 34) )
 			{ActiveFlag = 1; ActiveChannel = 3;}
	if ((TimeHourBinary == 07) && (TimeMinBinary >= 36) && (TimeMinBinary <= 41) )
			{ActiveFlag = 1; ActiveChannel = 4;}
	//if ((TimeHourBinary == 14) && (TimeMinBinary >= 39) && (TimeMinBinary <= 48) )
	//		{ActiveFlag = 1; ActiveChannel = 5;}

// Morning test
/*
 	if ((TimeHourBinary == 5) && (TimeMinBinary == 40))
 			{ActiveFlag = 1; ActiveChannel = 0;}
	if ((TimeHourBinary == 5) && (TimeMinBinary == 42))
			{ActiveFlag = 1; ActiveChannel = 1;}
	if ((TimeHourBinary == 5) && (TimeMinBinary == 44))
			{ActiveFlag = 1; ActiveChannel = 2;}
	if ((TimeHourBinary == 5) && (TimeMinBinary == 46))
 			{ActiveFlag = 1; ActiveChannel = 3;}
	if ((TimeHourBinary == 5) && (TimeMinBinary == 48))
			{ActiveFlag = 1; ActiveChannel = 4;}
	if ((TimeHourBinary == 5) && (TimeMinBinary == 50))
			{ActiveFlag = 1; ActiveChannel = 5;}
/*

// Evening test

/*
	if ((TimeHourBinary == 20) && (TimeMinBinary == 0))
 			{ActiveFlag = 1; ActiveChannel = 0;}
	if ((TimeHourBinary == 20) && (TimeMinBinary == 2))
			{ActiveFlag = 1; ActiveChannel = 1;}
	if ((TimeHourBinary == 20) && (TimeMinBinary == 4))
			{ActiveFlag = 1; ActiveChannel = 2;}
	if ((TimeHourBinary == 20) && (TimeMinBinary == 6))
 			{ActiveFlag = 1; ActiveChannel = 3;}
	if ((TimeHourBinary == 20) && (TimeMinBinary == 8))
			{ActiveFlag = 1; ActiveChannel = 4;}
	if ((TimeHourBinary == 20) && (TimeMinBinary == 10))
			{ActiveFlag = 1; ActiveChannel = 5;}
*/

        // ********************************** */



/* ********************BACK YARD ***************
	if ((TimeHourBinary == 11) && (TimeMinBinary >= 0) && (TimeMinBinary <= 25) )
 			{ActiveFlag = 1; ActiveChannel = 0;}
	if ((TimeHourBinary == 11) && (TimeMinBinary >= 27) && (TimeMinBinary <= 32) )
			{ActiveFlag = 1; ActiveChannel = 1;}
	if ((TimeHourBinary == 11) && (TimeMinBinary >= 35) && (TimeMinBinary <= 49) )
			{ActiveFlag = 1; ActiveChannel = 2;}

	if ((TimeHourBinary == 15) && (TimeMinBinary >= 0) && (TimeMinBinary <= 25) )
 			{ActiveFlag = 1; ActiveChannel = 0;}
	if ((TimeHourBinary == 15) && (TimeMinBinary >= 27) && (TimeMinBinary <= 32) )
			{ActiveFlag = 1; ActiveChannel = 1;}
	if ((TimeHourBinary == 15) && (TimeMinBinary >= 35) && (TimeMinBinary <= 49) )
			{ActiveFlag = 1; ActiveChannel = 2;}

// Morning test
	if ((TimeHourBinary == 5) && (TimeMinBinary == 30))
 			{ActiveFlag = 1; ActiveChannel = 0;}
	if ((TimeHourBinary == 5) && (TimeMinBinary == 32))
			{ActiveFlag = 1; ActiveChannel = 1;}
	if ((TimeHourBinary == 5) && (TimeMinBinary == 34))
			{ActiveFlag = 1; ActiveChannel = 2;}

// Evening test
	if ((TimeHourBinary == 19) && (TimeMinBinary == 4))
 			{ActiveFlag = 1; ActiveChannel = 0;}
	if ((TimeHourBinary == 19) && (TimeMinBinary == 6))
			{ActiveFlag = 1; ActiveChannel = 1;}
	if ((TimeHourBinary == 19) && (TimeMinBinary == 8))
			{ActiveFlag = 1; ActiveChannel = 2;}

********************************** */






			if ( (ActiveFlag == 1) && (ActiveChannel <=15) )
				{
				PORTDbits.RD5 = 1;  // ACTIVATE_24V
				PORTDbits.RD5 = 1;  // ACTIVATE_24V

				switch (ActiveChannel)
				{
				case 0: {PORTAbits.RA10 = 1; PORTAbits.RA10 = 1; break;}
				case 1: {PORTAbits.RA9 = 1; PORTAbits.RA9 = 1; break;}
				case 2: {PORTDbits.RD14 = 1; PORTDbits.RD14 = 1; break;}
				case 3: {PORTDbits.RD15 = 1; PORTDbits.RD15 = 1; break;}
				case 4: {PORTAbits.RA2 = 1; PORTAbits.RA2 = 1; break;}
				case 5: {PORTAbits.RA3 = 1; PORTAbits.RA3 = 1; break;}
				case 6: {PORTAbits.RA14 = 1; PORTAbits.RA14 = 1; break;}
				case 7: {PORTAbits.RA15 = 1; PORTAbits.RA15 = 1; break;}
				case 8: {PORTDbits.RD9 = 1; PORTDbits.RD9 = 1; break;}
				case 9: {PORTDbits.RD8 = 1; PORTDbits.RD8 = 1; break;}
				case 10: {PORTDbits.RD10 = 1; PORTDbits.RD10 = 1; break;}
				case 11: {PORTDbits.RD11 = 1; PORTDbits.RD11 = 1; break;}
				case 12: {PORTDbits.RD12 = 1; PORTDbits.RD12 = 1; break;}
				case 13: {PORTDbits.RD13 = 1; PORTDbits.RD13 = 1; break;}
				case 14: {PORTAbits.RA6 = 1; PORTAbits.RA6 = 1; break;}
				case 15: {PORTAbits.RA7 = 1; PORTAbits.RA7 = 1; break;}
				default:
  					{
					PORTDbits.RD5 = 0;
					PORTDbits.RD5 = 0;

					PORTAbits.RA10 = 0; PORTAbits.RA10 = 0;
					PORTAbits.RA9 = 0;  PORTAbits.RA9 = 0;
					PORTDbits.RD14 = 0; PORTDbits.RD14 = 0;
					PORTDbits.RD15 = 0; PORTDbits.RD15 = 0;
					PORTAbits.RA2 = 0;  PORTAbits.RA2 = 0;
					PORTAbits.RA3 = 0;  PORTAbits.RA3 = 0;
					PORTAbits.RA14 = 0; PORTAbits.RA14 = 0;
					PORTAbits.RA15 = 0; PORTAbits.RA15 = 0;
					PORTDbits.RD9 = 0;  PORTDbits.RD9 = 0;
					PORTDbits.RD8 = 0;  PORTDbits.RD8 = 0;
					PORTDbits.RD10 = 0; PORTDbits.RD10 = 0;
					PORTDbits.RD11 = 0; PORTDbits.RD11 = 0;
					PORTDbits.RD12 = 0; PORTDbits.RD12 = 0;
					PORTDbits.RD13 = 0; PORTDbits.RD13 = 0;
					PORTAbits.RA6 = 0;  PORTAbits.RA6 = 0;
					PORTAbits.RA7 = 0;  PORTAbits.RA7 = 0;
						}
					}
				}
			else
 					{
					PORTDbits.RD5 = 0;
					PORTDbits.RD5 = 0;

					PORTAbits.RA10 = 0; PORTAbits.RA10 = 0;
					PORTAbits.RA9 = 0;  PORTAbits.RA9 = 0;
					PORTDbits.RD14 = 0; PORTDbits.RD14 = 0;
					PORTDbits.RD15 = 0; PORTDbits.RD15 = 0;
					PORTAbits.RA2 = 0;  PORTAbits.RA2 = 0;
					PORTAbits.RA3 = 0;  PORTAbits.RA3 = 0;
					PORTAbits.RA14 = 0; PORTAbits.RA14 = 0;
					PORTAbits.RA15 = 0; PORTAbits.RA15 = 0;
					PORTDbits.RD9 = 0;  PORTDbits.RD9 = 0;
					PORTDbits.RD8 = 0;  PORTDbits.RD8 = 0;
					PORTDbits.RD10 = 0; PORTDbits.RD10 = 0;
					PORTDbits.RD11 = 0; PORTDbits.RD11 = 0;
					PORTDbits.RD12 = 0; PORTDbits.RD12 = 0;
					PORTDbits.RD13 = 0; PORTDbits.RD13 = 0;
					PORTAbits.RA6 = 0;  PORTAbits.RA6 = 0;
					PORTAbits.RA7 = 0;  PORTAbits.RA7 = 0;
					}

	TenSecondTick = 0;  // Must be at the bottom of the TenSecondTick while loop

	} // TenSecondTick if statement


  } // end of PERPETUAL while(1) loop


	return (0);
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
	_time_chk.min = mRTCCBin2Dec(50);  // 40 minutes
	_time_chk.hr =  mRTCCBin2Dec(21);   //7PM
	_time_chk.wkd = 0;
	_time_chk.day = mRTCCBin2Dec(2);
	_time_chk.mth = mRTCCBin2Dec(6);
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
	PORTCbits.RC2 = GreenLEDState;

	TenSecondTick = 1;
//	if (TenSecondTick == 6)
//		{
//			__builtin_btg((unsigned int *)&LATC, 2);	// Toggle RC2  //60 seconds on, 60 off
//			TenSecondTick = 0;
//		}

}

