/*********************************************************************
 *
 *      Operate Timer in IDLE mode
 *
 *********************************************************************
 * FileName:        operate_in_idle.c
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
 * (the �Company�) for its PIC32MX Microcontroller is intended
 * and supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *********************************************************************
 * $Id: operate_in_idle_mode.c 9390 2008-06-16 23:43:04Z rajbhartin $
 *
 * $Name:  $
 *********************************************************************
 * The purpose of this example code is to demonstrate how one would
 * configure a peripheral (Timer in this example) to operate in IDLE
 * mode.
 *
 * Platform: 	Explorer-16 board with PIC32MX PIM.
 *
 * Features demonstrated:
 *      - Timer configuration
 *      - Interrupt configuration
 *      - ISR syntax
 *		- IDLE mode entry/exit
 *
 * Description:
 *		- This example configures Timer1 to continue to operate in IDLE
 *		  mode. CPU is placed in IDLE mode and Timer1 interrupt causes
 *		  CPU to exit IDLE mode.
 *		  RA0 is toggled to indicatat the the Timer1 interrupt has
 *		  awaken the CPU.
 *
 * 	Note: You must program the Explorer16 board in "Programmer" mode
 *		or change REAL ICE setting to not halt processor when entering IDLE mode.
 *
 *
 * Oscillator Configuration Bit Settings:
 * 		- Oscillator Selection Bits = 	Primary Osc w/PLL (XT+HS+EC+PLL)
 *		- Primary Oscillator Config = 	HS osc mode
 *		- PLL Input Divider			=	2x Divider
 *		- PLL Multiplier			=	18x Multiplier
 *
 * Notes:
 *		- Set PBCLK divisor = 1:8 to achieve slower timer clock
 *		- Timer1 clock 		= FOSC/PB_DIV/PRESCALE
 *							= 72E6/8/256 = 35.156KHz
 *		- To toggle led once every second, PR1 is loaded with 35156
 *
 ********************************************************************/
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


// Let compile time pre-processor calculate the PR1 (period)
#define SYS_FREQ 			(80000000L)
#define PB_DIV         		8
#define PRESCALE       		256
#define TOGGLES_PER_SEC		1
#define T1_TICK       		(SYS_FREQ/PB_DIV/PRESCALE/TOGGLES_PER_SEC)

int main(void)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //STEP 1. Configure cache, wait states and peripheral bus clock
	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // STEP 2. configure Timer 1 using internal clock, 1:256 prescale

    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256 | T1_IDLE_CON, T1_TICK );

    // set up the timer interrupt with a priority of 2
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

    // enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

	// Explorer16 LEDs are on lower 8-bits of PORTA and to use all LEDs, JTAG port must be disabled.
	mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

	// Make all lower 8-bits of PORTA as output. Turn them off before changing
	// direction so that we don't have unexpected flashes
	mPORTAClearBits(BIT_7 | BIT_6 | BIT_5 | BIT_5 | BIT_4 | \
							 BIT_3 | BIT_2 | BIT_1 | BIT_0 );

	mPORTASetPinsDigitalOut( BIT_7 | BIT_6 | BIT_5 | BIT_5 | BIT_4 | \
							 BIT_3 | BIT_2 | BIT_1 | BIT_0 );

    while(1)
    {
		PowerSaveIdle();
	}
}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // STEP 3. configure the Timer 1 interrupt handler

void __ISR(_TIMER_1_VECTOR, IPL2SOFT) Timer1Handler(void)
{
    // clear the interrupt flag
    mT1ClearIntFlag();

    // .. things to do
    // .. in this case, toggle the LED
    mPORTAToggleBits(BIT_0);
}
