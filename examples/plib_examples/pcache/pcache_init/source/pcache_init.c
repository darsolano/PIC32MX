/*********************************************************************
 *
 *      PIC32MX Picocache Library Example Code
 *
 *********************************************************************
 * FileName:        pcache_example.c
 * Dependencies:	plib.h pcache.h
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
 * $Id: pcache_init.c 9556 2008-07-04 10:07:26Z deshmukha $
 *
 * $Name:  $
 *********************************************************************
 * Author                   Date        Comment
 * Larry Gass             3/15/2007   Inital version of example code
 *
 *
 * The purpose of this example code is to demonstrate the use of
 * peripheral library macros and functions supporting the PIC32MX
 * prefetch cache (picocache).
 *
 * Features demonstrated:
 *      - Configure picocache
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
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

#define SYSCLK			(80000000)
#define PBCLK       	(SYSCLK/2)

#define DESIRED_BAUDRATE    	(9600)      //The desired BaudRate

// calculate actual BAUD generate value.
#define BAUD_VALUE 	((PBCLK/16/DESIRED_BAUDRATE)-1)

int main(void)
{
	// Explorer-16 uses UART2 to connect to the PC.
	// This initialization assumes 40MHz Fpb clock. If it changes,
	// you will have to modify baud rate initializer.
	// Open UART2 with Receive and Transmitter enable.
   	OpenUART2(UART_EN, UART_RX_ENABLE | UART_TX_ENABLE, BAUD_VALUE);

	/* enable cacheability for KSEG0 */

	CheKseg0CacheOn();

	/* configure the cache for prefetch and 2 wait-state operation */

	mCheConfigure(CHE_CONF_WS2 | CHE_CONF_PF_C);

	return 0;
}

