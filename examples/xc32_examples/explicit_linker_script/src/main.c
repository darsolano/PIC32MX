/*********************************************************************
 *
 *            Simple Explicit Custom Linker Script
 *
 *********************************************************************
 * FileName:        main.c
 * Dependencies:    plib.h
 *
 *
 * Processor:       PIC32MX360512L
 *
 * Compiler:        MPLAB XC32
 *                  MPLAB IDE v8.73
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the Company) for its PIC32 Microcontroller is intended
 * and supplied to you, the Companys customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN AS IS CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ********************************************************************/

#include <xc.h>
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

#ifndef SYS_FREQ
#define SYS_FREQ 80000000
#endif

// .bss
// 'mybss' is stored in BSS.
int mybss[256];

// .data
// 'mystring' is stored in .data section
char mystring[] = "Microchip Technology Inc.";

int
main()
{
   // first item to be stored on stack
   int stack1;
   
   // Configure the device for maximum performance but do not change the PBDIV
   // Given the options, this function will change the flash wait states and
   // enable prefetch cache but will not change the PBDIV. The PBDIV value
   // is already set via the pragma FPBDIV option above..
   SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
   
   mybss[0] = 'M'-1;
   mybss[1] = 'C'+1;
   mybss[2] = 'H'-1;
   mybss[3] = 'P'+1;
   
   stack1 = 1;
   
   mybss[0] += stack1;
   mybss[1] -= stack1;
   mybss[2] += stack1;
   mybss[3] -= stack1;
   
   while(1)
   {
      stack1++;
      stack1--;
   }
   
   return 0;
}


