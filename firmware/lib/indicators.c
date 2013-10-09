/*
 * indicators.c
 *
 * Date: 9/21/2012
 * Revision:
 * Author: DC
 * Comments: Explorer 16 LED Indicator Routines
 *
 */

//#include <xc.h>  // JKW
#include "p24FJ128GA010.h"   //JKW from C:\Program Files (x86)\Microchip\mplabc30\v3.31\support\PIC24F\h
#include "HardwareProfile - PIC24FJ256GB110 PIM.h"   //JKW
#include <stdint.h>
#include <stdbool.h>
#include "indicators.h"

void initAllIndicators(void)
{
  // LED_D3
  LED_D3 = 0;
  TRIS_LED_D3 = 0;

  // LED_D4
  LED_D4 = 0;
  TRIS_LED_D4 = 0;

  // LED_D5
  LED_D5 = 0;
  TRIS_LED_D5 = 0;

  // LED_D6
  LED_D6 = 0;
  TRIS_LED_D6 = 0;

  // LED_D7
  LED_D7 = 0;
  TRIS_LED_D7 = 0;

  // LED_D8
  LED_D8 = 0;
  TRIS_LED_D8 = 0;

  // LED_D9
  LED_D9 = 0;
  TRIS_LED_D9 = 0;

  // LED_D10
  //LED_D10 = 0;
  //TRIS_LED_D10 = 0;

}


