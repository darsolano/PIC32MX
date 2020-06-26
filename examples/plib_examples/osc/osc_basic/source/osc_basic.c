
/*********************************************************************
 *
 *                  Oscillator API Example
 *
 *	This example changes the clock source and Peripheral bus divisor
 *
 *
 *
 *********************************************************************
 * FileName:        osc_basic.c
 * Dependencies:	plib.h
 *
 *
 * Processor:       PIC32
 *
 * Compiler:        MPLAB XC32
 *                  MPLAB IDE v8+
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC32 Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
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
 ******************************************************************
 * This program Demonstrates the operation of the Oscillator API lib
 *  *
 * The Hardware setup consist of a PIC32 pim on an Explorer16 board
 *
 * The CPU clock source and the Peripheral Bus dividers are
 * changed to reduce power consumption
 *
 * $Id: osc_basic.c 9390 2008-06-16 23:43:04Z rajbhartin $
 ********************************************************************/

// Master header file for all peripheral library includes
#include <plib.h>


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define SYS_FREQ 			(80000000L)


int main(void)
{
	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	// 			 // osc source, PLL multipler value, PLL postscaler , RC divisor
	OSCConfig( OSC_FRC_DIV, 0, 0, OSC_FRC_POST_8 ); // configure the FRC for divide-by-8(1 MHz)

	mOSCSetPBDIV( OSC_PB_DIV_4 );	// Configure the PB bus to run at 1/4 the CPU frequency

	while (1)
	{
		// *** insert user code ***

	}
}













