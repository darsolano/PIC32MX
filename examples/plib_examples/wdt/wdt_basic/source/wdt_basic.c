/*********************************************************************
 *
 *                  WDT Example
 *
 *********************************************************************
 * FileName:        wdt_basic.c
 * Dependencies:	None
 *
 *
 * Processor:       PIC32
 *
 * Compiler:        MPLAB XC32
 *                  MPLAB IDE vxx
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
 * This program Demonstrates the operation of the WDT peripheral lib
 *
 * The Hardware setup consist of a PIC32 pim on an Explorer16 board
 * In the main loop the WDT event bit is tested to see if an event did occur.
 * ( The WDT event testing would normally be part of the startup code.
 * If an WDT event did not occur the WDT is enabled and serviced.
 *
 * NOTE: debugger operation may interfere will WDT servicing.  This example
 * may not function as expected while a debugger is in operation.
 *
 * $Id: wdt_basic.c 9390 2008-06-16 23:43:04Z rajbhartin $
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
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

#define SYS_FREQ 				(80000000L)

int main (void)
{
	unsigned int val;

	val = (unsigned int)(*((unsigned int*)0xBFC02FF8));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

	// check for a previous WDT Event
	if ( ReadEventWDT() )
	{
		// A WDT event did occur
		 DisableWDT();
		 ClearEventWDT();	// clear the WDT event flag so a subsequent event can set the event bit


		// Blink all LEDs ON/OFF forever to indicate that we have seen WDT timeout reset.
		while(1)
		{
			int i;

			mPORTAToggleBits(BIT_7 | BIT_6 | BIT_5 | BIT_5 | BIT_4 | \
								 BIT_3 | BIT_2 | BIT_1 | BIT_0 );

			// Insert some delay
			i = 1024*1024;
			while(i--);
		}
	}

	// WDT timeout period is set in the Device Configuration
	EnableWDT(); // enable the WDT

	while(1)
	{
		int i;

		// Toggle LED0 to indicate that we are running normally
		// clearing WDT on time.
		mPORTAToggleBits(BIT_0 );

		// Insert some delay
		i = 1024*1024;
		while(i--)
		{
			// user code
			ClearWDT(); // service the WDT
		}
	}
}










