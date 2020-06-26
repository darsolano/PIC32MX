/*********************************************************************
 *
 *      PIC32MX Reset API Example Code
 *
 *********************************************************************
 * FileName:        reset_basic.c
 * Dependencies:	plib.h
 *
 * Processor:       PIC32MX
 *
 * Compiler:        MPLAB XC32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC32MX Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *********************************************************************
 * $Id: reset_basic.c 9390 2008-06-16 23:43:04Z rajbhartin $
 *
 * $Name:  $
 *********************************************************************
 * The purpose of this example code is to demonstrate the use of
 * peripheral library macros and functions supporting the PIC32MX
 * Reset status and control.
 *
 * Features demonstrated:
 *      - Reset source monitoring (EXTR, SWR, CM, POR, BOR, WDTO)
 *
 * Oscillator Configuration Bit Settings:
 * 		- Oscillator Selection Bits = 	Primary Osc w/PLL (XT+HS+EC+PLL)
 *		- Primary Oscillator Config = 	HS osc mode
 *		- PLL Input Divider			=	2x Divider
 *		- PLL Multiplier			=	18x Multiplier
 *
 * Notes: User must clear these bits in software
 *
 ********************************************************************/
#include <xc.h>
#include <plib.h>

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

#define SYS_FREQ 		(80000000L)

int main(void)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //STEP 1. Configure cache, dma, wait states
	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	// Explorer16 LEDs are on lower 8-bits of PORTA and to use all LEDs, JTAG port must be disabled.
	mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    // Make all lower 8-bits of PORTA as output. Turn them off before changing
	// direction so that we don't have unexpected flashes
	mPORTAClearBits(BIT_7 | BIT_6 | BIT_5 | BIT_5 | BIT_4 | \
							 BIT_3 | BIT_2 | BIT_1 | BIT_0 );

	mPORTASetPinsDigitalOut( BIT_7 | BIT_6 | BIT_5 | BIT_5 | BIT_4 | \
							 BIT_3 | BIT_2 | BIT_1 | BIT_0 );

    if(mGetPORFlag())
    {
        mClearPORFlag();
        mPORTASetBits(BIT_0);
        // execute power on reset handler
        // ...

    }
    
    if(mGetBORFlag())
    {
        mClearBORFlag();
        mPORTASetBits(BIT_1);
        // execute brown out on reset handler
        // ...
    }

    if(mGetMCLRFlag())
    {
        mClearMCLRFlag();
        mPORTASetBits(BIT_2);
        // execute master clear reset handler
        // ...
    }
    
    if(mGetSWRFlag())
    {
        mClearSWRFlag();
        mPORTASetBits(BIT_3);
        // execute software reset handler
        // ...
    }
    
    if (mGetCMRFlag())
    {
        mClearCMRFlag();
        mPORTASetBits(BIT_4);
        // execute configuration mismatch reset handler
        // ...
    }
    
    if (mGetWDTOFlag())
    {
        mClearWDTOFlag();
        mPORTASetBits(BIT_5);
        // execute watchdog timeout reset handler
        // ...
    }

	//... perform other application specific tasks

    while(1);
}


