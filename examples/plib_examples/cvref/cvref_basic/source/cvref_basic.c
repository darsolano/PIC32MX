/*********************************************************************
 *
 *                  CVREF API Example
 *
 *	This example generates a triangle wave using the CVref output
 *
 *
 *
 *********************************************************************
 * FileName:        cvref_api_example.c
 * Dependencies:	cvref.h
 *
 *
 * Processor:       PIC32
 *
 * Complier:        MPLAB Cxx
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
 * This program Demonstrates the operation of the Comparator Voltage Reference API lib
 *  *
 * The Hardware setup consist of a PIC32 pim on an Explorer16 board
 * This example will generate a trinagle wave on the CVref output pin.
 * This signal may be viewed with an oscilloscope attached to the CVREF output pin.
 *
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
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

#define SYS_FREQ (80000000L)



int main(void)
{
	unsigned char step; // used to hold the value of the desired CVREF step
	unsigned char loop;	// used to count the number of steps before changing direction
	unsigned char ramp; // used to track the direction of the waveform (rising or falling)

	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	while(1)
	{
		for ( loop =0; loop <= 15; loop ++)
		{
			for ( ramp = 0;  ramp <= 31; ramp ++)
			{
				if (  ramp <= 15 )
				{
					// ramp up
					step = 	ramp;
				}
				else
				{
					// ramp down
					step = 	31 - ramp;
				}
						// enable module| eanble output       | use high range output | use internal reference | desired step
				CVREFOpen( CVREF_ENABLE | CVREF_OUTPUT_ENABLE | CVREF_RANGE_HIGH | CVREF_SOURCE_AVDD | step );
 			}
		}
		// Disable CVREF (not executed)
		CVREFClose();
	}

	return 0;
}










