/*
 * console.h
 *
 * Date: 9/21/2012
 * Revision:
 * Author: DC
 * Comments: Basic UART CONSOLE I/O for Explorer16
 *
 */

#ifndef CONSOLE_U2_H
#define CONSOLE_U2_H

// init the console serial port (UART2, 9600, 8, N, 1, None )
void initConsole(uint32_t Fcyc);

// send a character to the serial port
void putConsole(int16_t c);

// wait for a new character to arrive to the serial port
char getConsole(void);

// send a null terminated string to the serial port
void putsConsole(char *s);

// receive a null terminated string in a buffer of len char
char *getsnConsole(char *s, uint16_t n);

// useful macros
#define clrscrConsole() putsConsole("\x1b[2J")
#define homeConsole()   putsConsole("\x1b[H")
#define pcrConsole()    putConsole('\r')

#endif // CONSOLE_U2_H


