/******************************************************************************
 * FileName:        incap_capture_event.c
 * Dependencies:
 * Processor:       PIC32
 *
 * Compiler:        MPLAB XC32
 *                  MPLAB IDE
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
 *********************************************************************
 * File Description: incap Peripheral Lib Demo
 *
 * Change History:
 * Name        Date            Changes
 * A Rafiq     3/27/07         Initial Version
 *
 * $Id: $
 *
 * Demo Code Description:
 * The purpose of this example code is to demonstrate the use of
 * peripheral library macros and functions supporting the PIC32MX
 * incap module (Input Capture).
 *
 * Features demonstrated:
 *      - OpenCapture1, IC1CaptureReady, mIC1ReadCapture, CloseCapture1
 *
 ********************************************************************/
#include <plib.h>


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 10 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8


#define FOSC           80E6
#define PB_DIV         8
#define PRESCALE       256
#define MSEC           10E-3
#define T1_TICK       (500 * MSEC * FOSC)/(PB_DIV * PRESCALE)

/////////////////////////////////////////////
/////////////////////////////////////////////
int main(void)
{
unsigned int CaptureTime;

	//Clear interrupt flag
	mIC1ClearIntFlag();

	// Setup Timer 3
    OpenTimer3(T3_ON | T1_PS_1_256, T1_TICK);

	// Enable Input Capture Module 1
	// - Capture Every edge
	// - Enable capture interrupts
	// - Use Timer 3 source
	// - Capture rising edge first
	OpenCapture1( IC_EVERY_EDGE | IC_INT_1CAPTURE | IC_TIMER3_SRC | IC_FEDGE_RISE | IC_ON );

	// Wait for Capture events
	while( !mIC1CaptureReady() ) ;

	//Now Read the captured timer value
	while( mIC1CaptureReady() )
	{
		CaptureTime = mIC1ReadCapture();
		//process data
		// ...
	}

	CloseCapture1();
	CloseTimer3();

	while(1)
	{}

}







