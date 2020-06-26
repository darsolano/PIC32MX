/*********************************************************************
 *
 *      PIC32MX BMX Library Example Code
 *
 *********************************************************************
 * FileName:        bmx_basic.c
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
 * $Id: bmx_basic.c 7587 2008-01-10 18:16:01Z rajbhartin $
 *
 * $Name:  $
 *
 * The purpose of this example code is to demonstrate the simple use of
 * peripheral library macros and functions supporting PIC32MX BMX (Memory)
 * module and its various features.
 *
 ********************************************************************/
#include <plib.h>


// Will use default osc mode with WDT off
// Other options are don't care for this example
//
#pragma config FWDTEN = OFF

int main(void)
{

	mBMXSetArbMode(0);		// Init arbitration mode to 0
	mBMXEnableCpuDExpt();	// Enable Bus Error Excpt on invalid Data address
	mBMXEnableCpuIExpt();	// Enable Bus Error Excpt on invalid Instruction address

	mBMXDisableDRMWaitState();  //Disable DRM waitstate

	mBMXSetFlashUserPartition( 20*1024 );	//Set Flash partition. 20K for user code

	//Create a Data Ram User-Data partition of 0x2000 bytes, in addition to default kernel data partition
	mBMXSetRAMKernProgOffset( BMXDRMSZ-0x2000 );
	mBMXSetRAMUserDataOffset( BMXDRMSZ-0x2000 );	//Set kernel prog start same as user data start
													//this will remove kernel prog parition.
													//Only user data partition of 0x2000 bytes (MAX_RAM - 0x6000)
	mBMXSetRAMUserProgOffset( BMXDRMSZ );			//Set RAM User Prog offset to max RAM to remove this parition

	while(1)
	{
		// Run user code
		// ...
	}

	return 0;
}

