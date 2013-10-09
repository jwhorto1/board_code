/*
 * lcd.h
 * 
 * Date: 9/21/2012
 * Revision:
 * Author: DC
 * Comments: Explorer 16 LCD Routines
 * 
 * (Based upon LDJ "Flying the PIC24 SE" (LCD.c) with updates)
 *
 */

#ifndef LCD_H
#define LCD_H

#define LCDDATA 1
#define LCDCMD  0
#define PMDATA  PMDIN1

void initLCD( void);
char readLCD( uint16_t addr);

#define busyLCD() readLCD( LCDCMD) & 0x80
#define addrLCD() readLCD( LCDCMD) & 0x7F
#define getLCD()  readLCD( LCDDATA)


void writeLCD( uint16_t addr, char c);

#define putLCD( d)  writeLCD( LCDDATA, (d))
#define cmdLCD( c)  writeLCD( LCDCMD, (c))

#define homeLCD()   writeLCD( LCDCMD, 2)
#define clrLCD()    writeLCD( LCDCMD, 1)
#define setLCDG( a) writeLCD( LCDCMD, (a & 0x3F) | 0x40)
#define setLCDC( a) writeLCD( LCDCMD, (a & 0x7F) | 0x80)

void putsLCD( char *s);

#endif // LCD_H



