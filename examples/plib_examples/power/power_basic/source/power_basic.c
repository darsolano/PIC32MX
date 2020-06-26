/*********************************************************************
 *
 *
 * Simple Power Example
 *
 * This example places the device in Sleep and then wakes using
 * the WDT
 *
 *********************************************************************
 * FileName:        power_basic.c
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
 * This program Demonstrates the operation of the power API lib
 *
 * The Hardware setup consists of a PIC32 pim on an Explorer16 board
 * The Explorer16 board is assumed to have a 8MHz crystal for POSC
 *
 * The device is placed in Sleep using the power save libray macro PowerSaveSleep()
 * The device is them woken by the WDT.
 *
 ********************************************************************/

// Master header file for all peripheral library includes
#include <plib.h>

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 80 MHz (80MHz / 1)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT off (It will be enabled by firmware)
// WDTPS 1:8192 (the WDT when enabled, will expire in approximately 8.2 seconds)
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = HS, FNOSC = PRIPLL 
#pragma config FWDTEN = OFF
#pragma config WDTPS = PS8192

#define SYS_FREQ (80000000L)

int main(void)
{
	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	while (1)
	{
		EnableWDT();	//turn on the WDT
		// *** insert user code ***

		ClearWDT(); // service the WDT to ensure a full WDT period before wakeup
		PowerSaveSleep();	// Configure device power save mode for Sleep and enter Sleep
							// device will sleep for the time set by WDTPS in the device configuration
		// device continues operation here after it wakes from Sleep by the WDT timeout

		DisableWDT();	//turn off the WDT so it does not need to be serviced
						//If WDT was enable in the device configuration this
						// disable will have no effect

		// *** insert user code***
	}

	return 0;
}
