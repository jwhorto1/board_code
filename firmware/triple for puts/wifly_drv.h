/*
 * wifly_drv.h
 *
 * Date: 9/22/2012
 * Revision:
 * Author: DC
 * Comments: Basic UART1 and GPIO driver routines for RN1xx PICTail on Explorer16
 *
 * Schematic Ref: "rn-171-pictail_rev2_sch.pdf" 09/18/201
 * WiFly UM Reference: "rn-wiflycr-um-1.0r.pdf" (covers v2.36-v2.45)
 *
 */

#ifndef WIFLY_DRV_H
#define WIFLY_DRV_H

// WiFly PicTail I/O Definitions (Refer to RN-171/131 PicTail Schematic)

// RN RESET input (PIC output)
// module has internal 100kOhm pull-up
// configure PIC pin as OD output
#define RN_RESET                PORTFbits.RF0
#define TRIS_RN_RESET           TRISFbits.TRISF0
#define OD_RN_RESET             ODCFbits.ODF0

// RN GPIO9 Line - Used for restoral of factory defaults
#define RN_GPIO9                PORTFbits.RF6
#define TRIS_RN_GPIO9           TRISFbits.TRISF6

// RN GPIO5 Line - Used to trigger remote IP host connection
// Configured as open drain O/P
#define RN_GPIO5                PORTFbits.RF7
#define TRIS_RN_GPIO5           TRISFbits.TRISF7
#define OD_RN_GPIO5             ODCFbits.ODF7

extern int16_t  WiFlyMsgLen;  //JKW
extern char  WiFlyMsg[128];  //JKW
extern char *WiFlyMsg_ptr;  //JKW
extern char ftpDataIncoming; //JKW

// init the PIC I/O and UART settings
void initWiFly(uint32_t Fcyc);

// send a character to the serial port
void putWiFly(int16_t c);

// wait for a new character to arrive to the serial port
char getWiFly(void);

// send a null terminated string to the serial port
void putsWiFly(char *s);

// receive a null terminated string in a buffer of len char
char *getsnWiFly(char *s, uint16_t n);

#endif // WIFLY_DRV_H
