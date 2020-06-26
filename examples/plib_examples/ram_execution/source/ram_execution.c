/*********************************************************************
 *
 *                  Executing from RAM
 *
 *	This example demonstrateS how PIC32 can be programmed to execute from RAM
 *
 *
 *
 *********************************************************************
 * FileName:        ram_execution.c
 * Dependencies:	xc.h
 *					sys/attribs.h
 *
 *
 * Processor:       PIC32
 *
 * Compiler:        MPLAB XC32
 *                  MPLAB IDE v8.0+
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
 * $Id: ram_execution.c 9555 2008-07-04 10:06:37Z deshmukha $
 ********************************************************************/
// Master header file for all peripheral library includes
#include <plib.h>				// Includes processor header file
								// SFR defs and
								// sys/attribs.h for __longramfunc__
								// and __ramfunc__ macros.


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2


int __longramfunc__ FunctionInRAMCallableFromAnywhere(int a, int b);
int __ramfunc__ FunctionInRAMCallableFromRAMOnly(int a);

int main(void)
{
	int i, j;

	// Prepare some dummy parameters - don't care what they are.
	i = 123;
	j = 456;

	// Call a function RAM. When a code in Flash memory makes a call to RAM,
	// it is considered a "long call", meaning that the call jumps over 256MB
	// boundary. To make such a call, the target function must be defined using
	// __longramfunc__ macro.
	// Long calls incur extra cycles. As a result, if timing is a prime concern,
	// long calls should be strategically placed.
	FunctionInRAMCallableFromAnywhere(i, j);

	while(1);
}

// A function in RAM - callable from anywhere.
int FunctionInRAMCallableFromAnywhere(int a, int b)
{
	int t;


	t = a*b + a + b;

	// Call to another RAM function that must called from same segment RAM only.
	return FunctionInRAMCallableFromRAMOnly(t);

}


// Another RAM function. This function can only be called from another RAM function in the
// same segment.
int FunctionInRAMCallableFromRAMOnly(int a)
{
	return a/2;
}
