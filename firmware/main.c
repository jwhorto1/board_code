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

// ***************************************************************************
// TO CHANGE THE MESSAGE, put 74.208.7.3/boardshortmessages into Browser
//
// ***************************************************************************


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

#include "adc.h"
#include "buttons.h"


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

/** VARIABLES *****************************************************************/

char  OutString[128];               //console output buffer used by sprintf()
char  SSID_text[32];
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
char ReadADC_char(void);
char ButtonState(void);
void LCD_and_Console_out(char *);
void LCD_only_out(char *);
void WiFly_and_Console_out(char *);


/** main() ********************************************************************/

int main(void)
{
unsigned char   temp1, temp2, temp3;
unsigned char   iloop, jloop;
char            CharChoice;
char            ADC_read;
char            Buttons;
char            ssid_size;
bool            flash, finish;

unsigned char   ChanSel, ChanTime;
unsigned char   Channel[7];
unsigned char   NewChannel[7];
unsigned char   charWiFlyMsgLen;
bool            MsgGood, MsgSame;


    InitializeSystem();
    AssociatedCheck = 0;
    initAllIndicators();

    for (jloop = 0; jloop <=6; jloop++)
      {
        Channel[jloop] = 0;
        NewChannel[jloop] = 1;
      }



    LED_D3 = 1;
  Delayms(1000);
    LED_D3 = 0;
    LED_D4 = 1;
  Delayms(1000);
    LED_D4 = 0;
    LED_D5 = 1;
  Delayms(1000);
    LED_D5 = 0;
    LED_D6 = 1;
  Delayms(1000);
    LED_D6 = 0;
    LED_D7 = 1;
  Delayms(1000);
    LED_D7 = 0;
    LED_D8 = 1;
  Delayms(1000);
    LED_D8 = 0;
    LED_D9 = 1;
  Delayms(1000);
    LED_D9 = 0;

    sprintf(OutString, "WiFly Pass_thru\nDemo");
    putsLCD(OutString);
    // JKW starts here
  Delayms(5000);
    sprintf(OutString,"$$$");
    putsWiFly(OutString);
    putsConsole(OutString);

    sprintf(OutString, "                \n                ");  // columns 41, 59
    setLCDC(0x00); // point to 1st line
  Delayms(200);
    putsLCD(&OutString[0]);
    jloop = 0;
    temp1 = 0;
    ssid_size = 0;

    Buttons = ButtonState();

/* ****************************
    sprintf(OutString, "Select SSID from\nmenu:           ");  // columns 41, 59
    LCD_and_Console_out(OutString);
    sprintf(OutString, "Hogwarts        \n                ");  // columns 41, 59
    LCD_and_Console_out(OutString);
    sprintf(OutString, "Press S3 for YES\nor wait for NO  ");  // columns 41, 59
    LCD_and_Console_out(OutString);
    sprintf(OutString, "Zoltan          \n                ");  // columns 41, 59
    LCD_and_Console_out(OutString);
    sprintf(OutString, "Press S3 for YES\nor wait for NO  ");  // columns 41, 59
    LCD_and_Console_out(OutString);
    sprintf(OutString, "These are all I \nsee, to manually");  // columns 41, 59
    LCD_and_Console_out(OutString);
    sprintf(OutString, "enter your own  \ntext, press S3  ");  // columns 41, 59
    LCD_and_Console_out(OutString);
    sprintf(OutString, "You have chosen \nto enter SSID by");  // columns 41, 59
    LCD_and_Console_out(OutString);
    sprintf(OutString, "text wheel,press\nS3 for next char");  // columns 41, 59
    LCD_and_Console_out(OutString);
  **************************** */

    sprintf(OutString, "S6 to go back   \nS5 when done    ");  // columns 41, 59
    LCD_and_Console_out(OutString);

    temp2 = 0;
    CharChoice = 0xFF; // impossible value
    finish = 0;
    ssid_size = 8;
    SSID_text[0] = 'H';
    SSID_text[1] = 'o';
    SSID_text[2] = 'g';
    SSID_text[3] = 'w';
    SSID_text[4] = 'a';
    SSID_text[5] = 'r';
    SSID_text[6] = 't';
    SSID_text[7] = 's';


while(!finish)
{
    sprintf(OutString, "                \n                ");  // columns 41, 59
    ADC_read = ReadADC_char();
    if ( (ADC_read > CharChoice + 0x05) || (ADC_read < CharChoice - 0x05) )
    {   CharChoice = ADC_read - 0x05;
        temp2 = 0;    }
    else
    {   if (temp2>0)
        {   CharChoice++;
            temp2 = 0;   }
        temp2++;    }

    if (CharChoice <= 0x20) CharChoice = 0x20;
    if (CharChoice >= 0x7F) CharChoice = 0x7F;

    if (ssid_size > 0)  // characters previously entered
    {  for (iloop = 0; iloop < ssid_size; iloop++)
       { OutString[iloop] = SSID_text[iloop];   }  }

    OutString[ssid_size] = CharChoice;
    OutString[17] = '_';
    OutString[18] = '_';
    OutString[19] = '_';
    OutString[20] = '_';
    OutString[21] = 0x30;  //(ssid_size - ssid_size %10) + 0x30;
    OutString[22] = (ssid_size %10) + 0x30;

   // putsConsole(OutString);
   LCD_only_out(OutString);

flash = 0;
finish = 0;

for (iloop = 0; iloop <6; iloop++)
{
    if (!( flash || finish) )
       {Buttons = ButtonState(); }  // only allow one button push per
    else Buttons = 0x00;
}

    if (Buttons == 0x01)
    {   SSID_text[ssid_size] = CharChoice;
        ssid_size++;
        Buttons = 0x00;
        OutString[17] = '*';
        flash = 1;  //button action seen, flash the underscore;
    }

    if (Buttons == 0x02)
    {   if (ssid_size != 0) ssid_size = ssid_size - 0x01;
        Buttons = 0x00;
        OutString[18] = '*';
        flash = 1;
    }

if (Buttons == 0x04)
    {   finish = 1;
        if (ssid_size != 0) ssid_size = ssid_size - 0x01;
        Buttons = 0x00;
        OutString[19] = '*';
        iloop = 6;
    }

    if (flash == 1)
    {   if (iloop %2) OutString[ssid_size] = ' ';
        else OutString[ssid_size] = '_';   }

   LCD_only_out(OutString);

  // Wait for button release
  while (Buttons != 0x00)
  {  Buttons = ButtonState();
     Delayms(100);  }

  OutString[17] = '_';
  OutString[18] = '_';
  OutString[19] = '_';
  OutString[20] = '_';
  OutString[21] = 0x30;  //(ssid_size - ssid_size %10) + 0x30;
  OutString[22] = (ssid_size %10) + 0x30;

}  //end of my little text wheel exercise

    // Output Final Result
    sprintf(OutString, "                \n                ");  // columns 41, 59
    if (ssid_size > 0)  // characters previously entered
    {  for (iloop = 0; iloop <= ssid_size; iloop++)
       { OutString[iloop] = SSID_text[iloop];   }
    OutString[17] = '_';
    OutString[18] = '_';
    OutString[19] = '_';
    OutString[20] = '_';
    OutString[21] = (ssid_size - ssid_size %10) + 0x30;
    OutString[22] = (ssid_size %10) + 0x30;
    LCD_only_out(OutString);

 //   setLCDC(0x00); // point to 1st line
 // Delayms(200);
 //   putsLCD(OutString);    }
    }




  Delayms(5000);
    sprintf(OutString,"set ip proto 18\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"set dns name 74.208.7.3\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"set ip remote 80\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"set comm remote 0\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"set ip timer 1\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"set ip a 192.168.1.115\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"set ip host 74.208.7.3\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"set ip gateway 192.168.1.1\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"set dns a 68.105.28.12\r\n");
    WiFly_and_Console_out(OutString);
//    sprintf(OutString,"set comm remote GET$/users/102\r\n");
    sprintf(OutString,"set comm remote GET$/boardshortmessages/1");
//    sprintf(OutString,"set comm remote GET$/users/105\r\n");
//    sprintf(OutString,"set comm remote GET$/users/112\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"set wlan ssid                     \r\n"); //set wlan ssid Hogwarts\r\n");
    for (iloop = 0; iloop <= ssid_size; iloop++)
    {
        if (iloop <= ssid_size)  { OutString[iloop+14] = SSID_text[iloop];}
//        if (iloop == (ssid_size+1)) { OutString[iloop+14] = 0x5C; } // use 0x5V for '\'
//        if (iloop == (ssid_size+2)) { OutString[iloop+14] = 'r'; }
//        if (iloop == (ssid_size+3)) { OutString[iloop+14] = 0x5C; } // use 0x5V for '\'
//        if (iloop == (ssid_size+4)) { OutString[iloop+14] = 'n'; }
    }

    WiFly_and_Console_out(OutString);
    LCD_only_out(OutString);

  Delayms(2000);

    sprintf(OutString,"set wlan phrase pooKums496\r\n");
    WiFly_and_Console_out(OutString);
    sprintf(OutString,"save\r\n");
    WiFly_and_Console_out(OutString);
//    sprintf(OutString,"get everything\r\n");
//    WiFly_and_Console_out(OutString);
  Delayms(200);
    setLCDC(0x00); // point to 1st line
  Delayms(200);
    sprintf(OutString, "Sept22 5:48PM   \nsocket gets only");  // columns 41, 59
    putsConsole(OutString);
    putsLCD(OutString);
  Delayms(2000);

//    sprintf(OutString,"scan\r\n");
//    WiFly_and_Console_out(OutString);
//    sprintf(OutString,"join # 1\r\n");  // NOT GOOD SINCE THIS CAN CHANGE EVERY MINUTE!!!
    sprintf(OutString,"join                  \r\n"); //set wlan ssid Hogwarts\r\n");
    for (iloop = 0; iloop <= ssid_size; iloop++)
    {
        if (iloop <= ssid_size)  { OutString[iloop+5] = SSID_text[iloop];}
    }
    WiFly_and_Console_out(OutString);
    LCD_only_out(OutString);

  Delayms(5000);

  if (AssociatedCheck)
  {
    putsConsole(WiFlyMsg);
  Delayms(200);
    setLCDC(0x00); // point to 1st line
    sprintf(OutString, "                \n                ");  // columns 41, 59
    putsLCD(OutString);
  Delayms(200);
    setLCDC(0x00); // point to 1st line
    putsLCD(WiFlyMsg);
  }
  else
  {
    sprintf(OutString, "nope\r\n");
    putsConsole(OutString);
  Delayms(200);
    setLCDC(0x00); // point to 1st line
    sprintf(OutString, "nope            \n                ");  // columns 41, 59
  Delayms(200);
    putsLCD(OutString);
  }

  Delayms(2000);

    for (iloop = 0; iloop <=25; iloop++)
    {
    AssociatedCheck = 1;
    MessageCheck = 0;
    MessageCnt = 0;

    sprintf(OutString,"open\r\n");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(1000);
    sprintf(OutString,"!");
    putsConsole(OutString);
    putsWiFly(OutString);
  Delayms(10000);
    sprintf(OutString,"$$$");
    putsConsole(OutString);
    putsWiFly(OutString);

  Delayms(200);
    setLCDC(0x00); // point to 1st line
  Delayms(200);
    sprintf(OutString, "iloop =         \nLoop to 25      ");
    OutString[9] = (char) (0x30 + iloop %10);
    OutString[8] = '0';
    if ((iloop >= 10) && (iloop <= 19)) OutString[8] = '1';
    if ((iloop >= 20) && (iloop <= 29)) OutString[8] = '2';
    putsLCD(OutString);
  Delayms(200);

  if (MessageCheck)
  {
    putsConsole(WiFlyMsg);
    
    sprintf(OutString, "                \n                ");  // 41, 59
//    if (charWiFlyMsgLen > 0)  LED_D5 = 1;
//    if (charWiFlyMsgLen > 5)  LED_D6 = 1;
//    if (charWiFlyMsgLen > 10) LED_D7 = 1;
//    if (charWiFlyMsgLen > 20) LED_D8 = 1;
//    if (charWiFlyMsgLen > 30) LED_D9 = 1;
    temp2 = ( (char) WiFlyMsgLen) %10;
    temp3 = (char) ((char)WiFlyMsgLen - temp2) /10;
    charWiFlyMsgLen = temp3*10+temp2;
    OutString[1] = temp3 + 0x30;
    OutString[2] = temp2 + 0x30;
    LCD_only_out(OutString);
  Delayms(2000);

    if (charWiFlyMsgLen <= 15)
    {
      for (jloop = 0; jloop<=charWiFlyMsgLen; jloop++)
      {  OutString[jloop] = WiFlyMsg[jloop]; }

    }
    else if (charWiFlyMsgLen >= 31 )
    {
      for (jloop = 0; jloop<=15; jloop++)
      {  OutString[jloop] = WiFlyMsg[jloop]; }

      for (jloop = 16; jloop<=31; jloop++)
      {  OutString[jloop+1] = WiFlyMsg[jloop]; }

    }
    else
    {
//      OutString[16] = 0x5C; // '\'
//      OutString[17] = 'n';
      for (jloop = 0; jloop<=15; jloop++)
      {  OutString[jloop] = WiFlyMsg[jloop]; }

      for (jloop = 16; jloop<=charWiFlyMsgLen; jloop++)
      {  OutString[jloop+1] = WiFlyMsg[jloop]; }
    }
    LCD_only_out(OutString);

  }
  else
  {
    sprintf(OutString, "nope\r\n");
    putsConsole(OutString);
  Delayms(200);
    setLCDC(0x00); // point to 1st line
    sprintf(OutString, "nope            \n                ");  // columns 41, 59
  Delayms(200);
    putsLCD(OutString);
  }



  // ***************************************************
  // processing WiFlyMsg quality, and channels and times
  // ***************************************************
  MsgGood = 1;
  MsgSame = 1;
  for (jloop = 12; jloop <=25; jloop++)
  {
      if ( (WiFlyMsg[jloop] >=0x30) && (WiFlyMsg[jloop] <=0x39) )
        {
          if (jloop == 8) LED_D3 = 1;
          if (jloop == 9) LED_D4 = 1;
          if (jloop == 10) LED_D5 = 1;
          if (jloop == 11) LED_D6 = 1;
          if (jloop == 12) LED_D7 = 1;
          if (jloop == 13) LED_D8 = 1;
          if (jloop == 14) LED_D9 = 1;
          if (jloop %2 == 0)
            { temp1 = WiFlyMsg[jloop] - 0x30; }
          else
            { NewChannel[(jloop-1)/2 - 6] = 10*temp1 + WiFlyMsg[jloop] - 0x30; }
        }
      else
        {
          MsgGood = 0;
        }
  }

  if (MsgGood == 1)
  {
    for (jloop = 0; jloop <=6; jloop++)
      {
      if ( NewChannel[jloop] != Channel[jloop] )
        { MsgSame = 0; }
      }
   }

  sprintf(OutString, "\r\n");
  putsConsole(OutString);

  sprintf(OutString, "                        \r\n");
  for (jloop = 0; jloop <=6; jloop++)
  {
  temp2 = NewChannel[jloop] %10;
  temp3 = (char) ((NewChannel[jloop] - temp2) /10);
  OutString[jloop*2] = temp3 + 0x30;
  OutString[jloop*2+1] = temp2 + 0x30;
  }
  putsConsole(OutString);


  if (1)   //MsgSame == 0)
  {
    for (jloop = 0; jloop <=6; jloop++)
      { Channel[jloop] = NewChannel[jloop]; }

    sprintf(OutString, "\r\n");
    putsConsole(OutString);
    sprintf(OutString, "                        \r\n");
    for (jloop = 0; jloop <=6; jloop++)
      {
        temp2 = Channel[jloop] %10;
        temp3 = (char) ((Channel[jloop] - temp2) /10);
        OutString[jloop*2] = temp3 + 0x30;
        OutString[jloop*2+1] = temp2 + 0x30;
      }
    putsConsole(OutString);
    
    ChanSel = 0;
    ChanTime = 0;
    for (jloop = 0; jloop <=6; jloop++)
      {
        sprintf(OutString, "\r\njloop=           ");
        OutString[8] = jloop + 0x30;
        putsConsole(OutString);
        ChanSel = jloop;
        ChanTime = 0;

        if (Channel[jloop] == 0)  // ignore this channel
        {
            sprintf(OutString, "ChanSel=      skip \r\n");
            OutString[12] = ChanSel + 0x31;  //start at 1 not 0
            putsConsole(OutString);
            ChanTime = 0;
        }
        else
        {
            sprintf(OutString, "ChanSel=     ");
            OutString[10] = ChanSel + 0x31;  //start at 1 not 0
            putsConsole(OutString);
            temp2 = ChanTime %10;
            temp3 = (ChanTime - temp2)/10;
            sprintf(OutString, "ChanTime=     ");
            OutString[10] = temp3 + 0x30;
            OutString[11] = temp2 + 0x30;
            putsConsole(OutString);
           while (ChanTime < Channel[jloop])
             {
               LED_D3 = 0;
               LED_D4 = 0;
               LED_D5 = 0;
               LED_D6 = 0;
               LED_D7 = 0;
               LED_D8 = 0;
               LED_D9 = 0;
               if (ChanSel == 0) LED_D3 = 1;
               if (ChanSel == 1) LED_D4 = 1;
               if (ChanSel == 2) LED_D5 = 1;
               if (ChanSel == 3) LED_D6 = 1;
               if (ChanSel == 4) LED_D7 = 1;
               if (ChanSel == 5) LED_D8 = 1;
               if (ChanSel == 6) LED_D9 = 1;
               Delayms(1000);  // the 1 second count
               ChanTime++;
               sprintf(OutString, "   ");
               temp2 = ChanTime %10;
               temp3 = (char) ((ChanTime - temp2) /10);
               OutString[0] = temp3 + 0x30;
               OutString[1] = temp2 + 0x30;
               putsConsole(OutString);
             }
               LED_D3 = 0;
               LED_D4 = 0;
               LED_D5 = 0;
               LED_D6 = 0;
               LED_D7 = 0;
               LED_D8 = 0;
               LED_D9 = 0;
           }  //  big else
        }  // main jloop
    }  // MsgSame if statement

  // ***************************************************






    } // big iloop of 25 passes

  return 0;
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

    /* Enable ADC to the Potentiometer channel and configure it as 10 bit
     interrupt driven mode */
      ADC_ChannelEnable ( ADC_CHANNEL_POTENTIOMETER ) ;
      ADC_SetConfiguration ( ADC_CONFIGURATION_EXP16_DEMO ) ;


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


char ReadADC_char(void)
{
  char          ADC_read;
  uint16_t      ADC_read_int;

    ADC_read_int = ADC_Read10bit( (char) 5);
    ADC_read_int = (unsigned int) ((float) ADC_read_int * 0.093) + (int)32;
    // maps 00,0000,0000 to 010,0000 and 11,1111,1111 to 111,1111
    ADC_read = (unsigned char) ADC_read_int;
    if (ADC_read <0x20) ADC_read = 0x20;
    if (ADC_read >0x7F) ADC_read = 0x7F;
    return ADC_read;
}

char ButtonState(void)
{
  char          ButtonFace;
  ButtonFace = 0x00;
  if (BUTTON_IsPressed(BUTTON_S3)) ButtonFace = 0x01;
  if (BUTTON_IsPressed(BUTTON_S6)) ButtonFace = 0x02;
  if (BUTTON_IsPressed(BUTTON_S5)) ButtonFace = 0x04;
  if (BUTTON_IsPressed(BUTTON_S4)) ButtonFace = 0x08;
  return ButtonFace;
}

void LCD_and_Console_out(char String[128])
{
    putsConsole(String);
    setLCDC(0x00); // point to 1st line
  Delayms(200);
    putsLCD(String);
  Delayms(2000);
}

void LCD_only_out(char String[128])
{
    setLCDC(0x00); // point to 1st line
  Delayms(200);
    putsLCD(String);
  Delayms(2000);
}


void WiFly_and_Console_out(char String[128])
{
    putsConsole(String);
    putsWiFly(String);
  Delayms(1000);
}


