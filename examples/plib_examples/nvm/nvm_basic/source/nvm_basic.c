/*********************************************************************
 *
 *                  NVM Simple Example
 *
 *********************************************************************
 * FileName:        nvm_basic.c
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
 *
 * $Id: nvm_basic.c 9390 2008-06-16 23:43:04Z rajbhartin $
 * $Name: x.x $
 *
 **********************************************************************/
#include <stdlib.h>
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



// PFM KERNEL AND USER MODE MAPPING
//  Virtual Address                     Physical Address
//  ---------------                     ----------------
//  0xBFC0_2FFF or  *****************   0x1FC0_2FFF
//  0X9FC0_2FFF     *               *
//                  *               *
//                  *  Boot Flash   *
//  0xBFC0_0000 or  *               *
//  0x9FC0_0000     *****************   0x1FC0_0000


//                  *****************
//                  *               *
//                  *   Optional    *
//                  * User Program  *
//                  *     Flash     *
//                  *               *
//                  *               *
//  0x7D00_0000 +   *               *   0xBD00_0000 +
//  BMXPUPBA        *****************   BMXPUPBA
//                  *               *
//                  *   Program     *
//                  *    Flash      *
//  0xBD00_0000 or  *               *
//  0x9D00_0000     *****************   0x1D00_0000

#define mInitAllLEDs()  {TRISA = 0; LATA = 0;}

#define mLED_1              LATAbits.LATA2
#define mLED_2              LATAbits.LATA3
#define mLED_3              LATAbits.LATA6
#define mLED_4              LATAbits.LATA7

#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_3_On()         mLED_3 = 1;
#define mLED_4_On()         mLED_4 = 1;

#define mLED_1_Off()        mLED_1 = 0;
#define mLED_2_Off()        mLED_2 = 0;
#define mLED_3_Off()        mLED_3 = 0;
#define mLED_4_Off()        mLED_4 = 0;

#define mLED_1_Toggle()     mLED_1 = !mLED_1;
#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#define mLED_3_Toggle()     mLED_3 = !mLED_3;

#define SYS_CLK             72000000L

#define NVM_PROGRAM_PAGE 0xbd008000
#define NVM_PAGE_SIZE	4096

unsigned int pagebuff[1024];
unsigned int databuff[128];

int main(void)
{
	unsigned int x;

	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	// Initialize the Led Port
	mInitAllLEDs();

	// Fill databuff with some data
	for(x =0; x < sizeof(databuff); x++)
		databuff[x] = x;

	// Erase second page of Program Flash
	NVMErasePage((void *)NVM_PROGRAM_PAGE);

	// Write 128 words starting at Row Address NVM_PROGRAM_PAGE
	NVMWriteRow((void *)NVM_PROGRAM_PAGE, (void*)databuff);

	// Verify data matches
	if(memcmp(databuff, (void *)NVM_PROGRAM_PAGE, sizeof(databuff)))
	{
		// If not turn led1 on to indicate an error
		mLED_1_On();
	}

	// Write 0x12345678 to Address NVM_PROGRAM_PAGE + 0x200
	NVMWriteWord((void*)(NVM_PROGRAM_PAGE + 0x200), 0x12345678);

	// Verify data matches
    if(*(int *)(NVM_PROGRAM_PAGE + 0x200) != 0x12345678)
	{
		// Turn led2 on to indicate an error
		mLED_2_On();
	}

	// Write the pagebuff data to NVM_PROGRAM_PAGE + NVM_PAGE_SIZE
	NVMProgram((void *)(NVM_PROGRAM_PAGE + NVM_PAGE_SIZE), (const void *)databuff, sizeof(databuff), (void*) pagebuff);

	// Verify data matches
	if(memcmp(databuff, (void *)(NVM_PROGRAM_PAGE + NVM_PAGE_SIZE), sizeof(databuff)))
	{
		// Turn led3 on to indicate an error
		mLED_3_On();
	}

	return 0;
}
