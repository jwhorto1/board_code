/*
 * delayms.h
 *
 * Date: 9/21/2012
 * Revision:
 * Author: DC
 * Comments: Implement 1-65536 mS blocking delay using Timer 1 & 4MHz Fcyc
 *
 */

#ifndef DELAY_MS_H
#define DELAY_MS_H

// enable and initialize Timer 1
void initDelayms(void);
// initiate xxx mS delay
void Delayms(uint16_t delay);

#endif // DELAY_MS_H
