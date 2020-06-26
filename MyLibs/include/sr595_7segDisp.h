/* 
 * File:   sr595_7segDisp.h
 * Author: dsolano
 *
 * Created on July 20, 2014, 2:17 PM
 */
#pragma once
#ifndef SR595_7SEGDISP_H
#define	SR595_7SEGDISP_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "utilities.h"
#include <plib.h>


#define SR_LATCH_OFF()          LATDbits.LATD8 = 0
#define SR_LATCH_ON()           LATDbits.LATD8 = 1
#define SR_CHANNEL              SPI_CHANNEL1
#define SET_DOT                 0b10000000
#define CLEAR_DOT               0b01111111

    typedef enum{
        Letter,
        Number
    }TYPEOF_t;

void InitShiftRegister(void);
void DisplayNumber(UINT8 number, TYPEOF_t type);
void ClearDot(void);
void SetDot(void);
void WriteShiftRegister(UCHAR8 data);
void Count2Display(void);
UCHAR8 GetLastDisplayChar();
void ClearDisplays(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SR595_7SEGDISP_H */

