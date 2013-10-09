/*
 * indicators.h
 *
 * Date: 9/21/2012
 * Revision:
 * Author: DC
 * Comments: Explorer 16 LED Indicator Routines
 *
 */

#ifndef INDICATORS_H
#define INDICATORS_H

// I/O Definitions

// LED_D3 (RA0)
#define LED_D3       	LATAbits.LATA0
#define TRIS_LED_D3  	TRISAbits.TRISA0

// LED_D4 (RA1)
#define LED_D4          LATAbits.LATA1
#define TRIS_LED_D4    	TRISAbits.TRISA1

// LED_D5 (RA2)
#define LED_D5          LATAbits.LATA2
#define TRIS_LED_D5    	TRISAbits.TRISA2

// LED_D6 (RA3)
#define LED_D6          LATAbits.LATA3
#define TRIS_LED_D6    	TRISAbits.TRISA3

// LED_D7 (RA4)
#define LED_D7          LATAbits.LATA4
#define TRIS_LED_D7    	TRISAbits.TRISA4

// LED_D8 (RA5)
#define LED_D8          LATAbits.LATA5
#define TRIS_LED_D8    	TRISAbits.TRISA5

// LED_D9 (RA6)
#define LED_D9          LATAbits.LATA6
#define TRIS_LED_D9    	TRISAbits.TRISA6

// RA5 reserved for switch SW_S5 use
// LED_D10 (RA7)
//#define LED_D10          LATAbits.LATA7
//#define TRIS_LED_D10    	TRISAbits.TRISA7


// Initialize all indicators

void initAllIndicators(void);

#endif // INDICATORS_H
