/*****************************************************************************
 *
 * Explorer 16 Development Board Demo Program.
 * Modified for PIC24FJ64GA004/GB004/GA104 family with PPS.
 *
 *****************************************************************************
 * FileName:        PIC24ExplDemo.c
 * Dependencies:    system.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30 v3.00 or later
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") is intended and supplied to you, the Company's
 * customer, for use solely and exclusively with products manufactured
 * by the Company.
 *
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Ross Fosler	        XXX         ...
 * Anton Alkhimenok     10/21/05    ...
 * Brant Ivey			3/14/06     Modified for PIC24FJ64GA004 family
 * C11842               5/27/2009   Added support for PIC24FJGB004, PIC24FJGA104
 * C11824               5/16/2010   Added support for PIC24FJ256GB210 PIM
 * Kevin Hengehold		4/27/2011	Added support for PIC24F32KA304 PIM
 * Naveen Raj           10/12/2011  Added support for PIC24FJ128GA310 PIM
 *****************************************************************************/

#include "system.h"

//----------- Setup configuration bits
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2 )
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_XT & FNOSC_PRI & DISUVREG_OFF & PLL_96MHZ_ON & IOL1WAY_ON)
//--------- End of configuation bits


#define     DISP_MESSAGE    0
#define     HYPERTERM       1      // either hyperterm == 1 or go out to our ftp site
unsigned char _display_state;

extern char UART_WriteMode;
unsigned char temp;
unsigned char temp1;
unsigned char temp2;
unsigned char temp3;
char WelcomeToSymp1e[32]        = "Welcome to Symp1e               ";
char ClearLCD[32]               = "                                ";
char ExclaimPt[32]              = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
char PoundSign[32]              = "################################";
char Waterman_network[32]       = "Waterman network?         Y=btn1";
char Steingart_network[32]      = "Steingart network?        Y=btn1";
char Horton_network[32]         = "Horton network?           Y=btn1";
char NextEnterYourPassword[32]  = "Network chosen, next do password";
char AcceptIgnoreGoBack[32]     = "Acpt(B1),Ignore(wait)or GoBk(B2)";
char AskIfDone[32]              = "If Password done(B1), else wait ";
char NetworkPassword[32]        = "                                ";
char Password[32]               = "                                ";

char SlowSpeed;

char NetworkFound;
char NetworkPasswordSetupDone;

int count;

unsigned char i;
unsigned char j;
unsigned char k;

int main(void)
{

//    TRISA = 0xFFFB; // LED2
//    ODCA = 0x0004;  // no open drains on PORTA
//    TRISB = 0xFFFF; // No outputs on RB.... for now
//    ODCB = 0x0000;  // no open drains on PORTB
//    TRISC = 0xFFFF; // No outputs on RC.... for now
//    ODCC = 0x0000;  // no open drains on PORTC
//    TRISD = 0xFFFF; // No outputs on RD.... for now
//    ODCD = 0x0000;  // no open drains on PORTD

//    PORTAbits.RA2 = 1;  // Red LED



// Initialize LongMessage

UART_WriteMode = 0;


    // Start from displaying of PIC24 banners
	_display_state = DISP_MESSAGE;


    AD1PCFG = 0xffff;


    //LED startup display - currently only implemented on PI24F32KA304
	//IO Mapping
	ioMap();
	lockIO();

    // Setup SPI to communicate to EEPROM
    SPIMPolInit();

    // Setup EEPROM IOs
    EEPROMInit();

    // Setup the UART
    UART2Init();

    // Setup the timer
    TimerInit();

    // Setup the LCD
    mLCDInit();

    // Setup debounce processing
    BtnInit();


	// Setup the RTCC
    RTCCInit();
    RTCCProcessEvents();

    UART_WriteMode = 0;
    LCDProcessEvents();

    // Clear the LCD Screen
    temp = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
    temp1 = temp;
    while ((temp1 - temp) <2)
    {
       RTCCProcessEvents();
       temp1 = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
       if (temp1 <=1) {temp1 = temp1 + 60;}
       UBannerProcessEvents(&ClearLCD[0]);
       LCDProcessEvents();
       DelayMs(1);  // Don't really need this, but I thought it was nice               }
    }


	  // Button debounce processing
	  BtnProcessEvents();
    while (!BtnIsPressed(4)) {BtnProcessEvents();}
    while (BtnIsPressed(4)) {BtnProcessEvents();}

    k = 0;
    NetworkFound = 0;
    UBannerInit();   // Only need to do once, more doesn't hurt
    mLCDClear();

    while (NetworkFound == 0)
	{
        temp = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
        temp1 = temp;
        while ( (temp1 - temp) <3)
        {
             RTCCProcessEvents();
             temp1 = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
             if (temp1 <=2) {temp1 = temp1 + 60;}
             if (k==0) {UBannerProcessEvents(&Waterman_network[0]);}
             if (k==1) {UBannerProcessEvents(&Steingart_network[0]);}
             if (k==2) {UBannerProcessEvents(&Horton_network[0]);}
             LCDProcessEvents();
             BtnProcessEvents();
             if ( (BtnIsPressed(1)))
             {
                while (BtnIsPressed(1)) {BtnProcessEvents();} // wait until Btn1 is released
                temp1 = temp+6;
                NetworkFound = 1;
                for (j=0; j<=31; j++)
                {
                    if (k==0) {NetworkPassword[j] = Waterman_network[j];}
                    if (k==1) {NetworkPassword[j] = Steingart_network[j];}
                    if (k==2) {NetworkPassword[j] = Horton_network[j];}
                }
             }
             DelayMs(1);  // Don't really need this, but I thought it was nice               }
	    }
        k++;
        if (k>2) {k=0;}
    } // end of NetworkFound while loop


    // Prompt for password
    temp = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
    temp1 = temp;
    while ((temp1 - temp) <3)
    {
        RTCCProcessEvents();
        temp1 = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
        if (temp1 <=2) {temp1 = temp1 + 60;}
        UBannerProcessEvents(&NextEnterYourPassword[0]);
        LCDProcessEvents();
        DelayMs(1);  // Don't really need this, but I thought it was nice               }
    }

    k = 0;
    i = 32;
    count = 0;
    SlowSpeed = 0;
    NetworkPasswordSetupDone = 0;
    
    while (!NetworkPasswordSetupDone)
    {
       Password[k] = i;
       UBannerProcessEvents(&Password[0]);
       LCDProcessEvents();
       BtnProcessEvents();
       if (BtnIsPressed(1))
       {
           while (BtnIsPressed(1)) {BtnProcessEvents();} // wait until Btn1 is released
           // Pause to display
           RTCCProcessEvents();
           temp = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
           temp1 = temp;
           while ((temp1 - temp) <5)
           {
               RTCCProcessEvents();
               temp1 = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
               if (temp1 <=4) {temp1 = temp1 + 60;}
               UBannerProcessEvents(&Password[0]);
               LCDProcessEvents();
           }

           // Accept (Btn1), Ignore or Go Back (Btn2)
           RTCCProcessEvents();
           temp = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
           temp1 = temp;
           while ((temp1 - temp) <5)
           {
               RTCCProcessEvents();
               temp1 = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
               if (temp1 <=4) {temp1 = temp1 + 60;}
               UBannerProcessEvents(&AcceptIgnoreGoBack[0]);
               LCDProcessEvents();
               BtnProcessEvents();
               // Go Back chosen...............
               if (BtnIsPressed(2))
               {
                   while (BtnIsPressed(2)) {BtnProcessEvents();}  // wait until Btn2 is released
                   Password[k] = ' ';
                   k--;
                   if (k > 31) {k=0;}
                   temp1 = 100; //force exit
               }
               // Accept chosen, but after that, we must ask if they are done....
               else if (BtnIsPressed(1))
               {
                   while (BtnIsPressed(1)) {BtnProcessEvents();}  // wait until Btn1 is released
                   Password[k] = i;
                   RTCCProcessEvents();
                   temp2 = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
                   temp3 = temp2;
                   while ((temp3 - temp2) <5)
                   {
                        RTCCProcessEvents();
                        temp3 = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
                        if (temp3 <=4) {temp3 = temp3 + 60;}
                        UBannerProcessEvents(&AskIfDone[0]);
                        LCDProcessEvents();
                        BtnProcessEvents();
                        // All done is chosen
                        if (BtnIsPressed(1))
                        {
                            while (BtnIsPressed(1)) {BtnProcessEvents();}  // wait until Btn1 is released
                            NetworkPasswordSetupDone = 1;
                            temp3 = 100; //force exit
                        }
                   }
                   if (!NetworkPasswordSetupDone) k++;
                   if (k > 31) {k=0;}
                   temp1 = 100; //force exit
               }
               else {}
           } // end of while or Accept/Ignore/GoBack message
       } // end of Btn1 pressed for prompting Accept/Ignore/GoBack
       // Speed change is chosen....
       if ( (BtnIsPressed(2)))
       {
          while (BtnIsPressed(2)) {BtnProcessEvents();}  // wait until Btn2 is released
          if (SlowSpeed == 0)
             {
                SlowSpeed = 1;
                i = i -5;
                if (i<32) {i = 32;}
             }
          else {SlowSpeed = 0;}
       }  // end of Btn2 speed change
       DelayMs(1);
       count++;
       if (count > 32000) count = 0;
       if ((SlowSpeed == 0) && (count%1300 == 0)) i++;
       if ((SlowSpeed == 1) && (count%5000 == 0)) i++;
       if ((i>126) || (i<32)) i=32;
  } // end of password while

    for (i=16; i<=31; i++)
    {
        NetworkPassword[i] = Password[i-16];
    }

    RTCCProcessEvents();
    temp = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
    temp1 = temp;
    while ((temp1 - temp) <5)
    {
        RTCCProcessEvents();
        temp1 = (_time_str[10]-0x30) *10 + (_time_str[11] - 0x30); // seconds
        if (temp1 <=4) {temp1 = temp1 + 60;}
        UBannerProcessEvents(&NetworkPassword[0]);
        LCDProcessEvents();
    }




     while (1) {}



}// End of main()...
