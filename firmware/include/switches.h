/*
 * switches.h
 * 
 * Date: 	9/21/2012
 * Revision:
 * Author: 	DC
 * Comments: 	Explorer 16 Input Switch Processing Functions
 * 
 * Dependencies:Delayms()
 *
 */

#ifndef SWITCHES_H
#define SWITCHES_H

// Typedefs

// Data Structure for storing all input switch state

typedef union
{
  unsigned int Sw_all;
  unsigned char Sw_bytes[2];
  struct
  {
    unsigned int S4: 1;
    unsigned int S5: 1;
    unsigned int S6: 1;
    unsigned int S3: 1;
    unsigned int NotUsed: 12;
  } Sw_bits;
} SWSTATE;

// I/O definitions

// Input Switches

// S4
#define SW_S4               PORTDbits.RD13
#define SW_S4_PORT          PORTD
#define SW_S4_PIN           (13u)
#define TRIS_SW_S4          TRISDbits.TRISD13    // tris bit for S4 input
//#define ADCFG_LEFT  AD1PCFGLbits.PCFG5  			// analog pin config bit
//#define CNPU_SW_S4          CNPU2bits.CN19PUE  	// pull up enable
//#define CNEN_SW_S4          CNEN2bits.CN19IE  	// pin change int enable

// S5
#define SW_S5               PORTAbits.RA7
#define SW_S5_PORT          PORTA
#define SW_S5_PIN           (7u)
#define TRIS_SW_S5          TRISAbits.TRISA7    // tris bit for S5 input
//#define ADCFG_LEFT  AD1PCFGLbits.PCFG5  			// analog pin config bit
//#define CNPU_SW_S5          CNPU2bits.CN19PUE  	// pull up enable
//#define CNEN_SW_S5          CNEN2bits.CN19IE  	// pin change int enable

// S6
#define SW_S6               PORTDbits.RD7
#define SW_S6_PORT          PORTD
#define SW_S6_PIN           (7u)
#define TRIS_SW_S6          TRISDbits.TRISD7    // tris bit for S6 input
//#define ADCFG_LEFT  AD1PCFGLbits.PCFG5  			// analog pin config bit
#define CNPU_SW_S6          CNPU2bits.CN16PUE  	// pull up enable
#define CNEN_SW_S6          CNEN2bits.CN16IE  	// pin change int enable

// S3
#define SW_S3               PORTDbits.RD6
#define SW_S3_PORT          PORTD
#define SW_S3_PIN           (6u)
#define TRIS_SW_S3          TRISDbits.TRISD6    // tris bit for S3 input
//#define ADCFG_LEFT  AD1PCFGLbits.PCFG5  			// analog pin config bit
#define CNPU_SW_S3          CNPU2bits.CN15PUE  	// pull up enable
#define CNEN_SW_S3          CNEN2bits.CN15IE  	// pin change int enable

// Switch initialization functions

// Initialize all Switches
void initAllSwitches(unsigned int max_debounce_time_ms); // max debounce in ms

// Switch initialization functions
void initSwitchS4(uint16_t max_debounce_time_ms);
void initSwitchS5(uint16_t max_debounce_time_ms);
void initSwitchS6(uint16_t max_debounce_time_ms);
void initSwitchS3(uint16_t max_debounce_time_ms);

// Switch level change detections (individual)
void SwitchS4IsPressed(uint16_t max_debounce_time_ms);
void SwitchS5IsPressed(uint16_t max_debounce_time_ms);
void SwitchS6IsPressed(uint16_t max_debounce_time_ms);
void SwitchS3IsPressed(uint16_t max_debounce_time_ms);

// Read/Save all Player switch inputs (complete)
void CheckSwitchIsPressed(SWSTATE *ptr, uint16_t max_debounce_time_ms);

// Player Switch Level Debounce Function Definitions
#define COUNT_BASE 32768

uint16_t SwitchLevel(volatile unsigned int *sw, unsigned int bit, uint16_t range);

#endif // SWITCHES_H

