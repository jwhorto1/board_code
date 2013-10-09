/*
 * wifly_module.h
 *
 * Date: 9/23/2012
 * Revision:
 * Author: DC
 * Comments: wifly module-level command/response apis
 *
 * Schematic Ref: "rn-171-pictail_rev2_sch.pdf" 09/18/201
 * WiFly UM Reference: "rn-wiflycr-um-1.0r.pdf" (covers v2.36-v2.45)
 *
 */

#ifndef WIFLY_MODULE_H
#define	WIFLY_MODULE_H

// WLAN Parameter Values
// "set wlan <parameter> <value>"
#define WF_SSID             "Cisco40671"
#define WF_PASSPHRASE       "Microchip 802.11 Secret PSK Password"
#define WF_CHANNEL          "0"
#define WF_CHANNEL_MASK     "0x0421" // scan for channels 1, 6, 11
#define WF_AUTH_OPEN        "0"
#define WF_AUTH_WPA2_PSK    "4"




#endif	/* WIFLY_MODULE_H */

