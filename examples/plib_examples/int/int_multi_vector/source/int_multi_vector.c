/********************************************************************
 * FileName:		int_multi_vector.c
 * Dependencies:
 * Processor:
 * Hardware:
 * Assembler:
 * Linker:
 * Company:		Microchip Technology Inc.
 *
 * Software License Agreement:
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
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
 * File Description:
 *
 * Mulit-vectored interrupt
 *
 * Change History:
 * Name				Date            Changes
 * Sean Justice		01/02/07		Initial Version
 *
 * $Id: int_multi_vector.c 9390 2008-06-16 23:43:04Z rajbhartin $
 ********************************************************************/

#include <peripheral/int.h>
#include <peripheral/ports.h>
#include "hardwareprofile.h"

// *****************************************************************************
// *****************************************************************************
// Section: Configuration bits
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
// *****************************************************************************
// *****************************************************************************
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

// *****************************************************************************
// *****************************************************************************
// Section: System Macros
// *****************************************************************************
// *****************************************************************************
#define	GetSystemClock() 			(80000000ul)
#define	GetPeripheralClock()		(GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()		(GetSystemClock())


#define CORE_TIMER_PERIOD       GetSystemClock()


/*********************************************************************
 * Function:    int main(void)
 *
 * Overview:
 *  This application demostrates the multi-vectored interrupts using
 *  the core timer and care software interrupts.
 ********************************************************************/
int main(void)
{
	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	OpenCoreTimer(CORE_TIMER_PERIOD);              // load with the period
    
    // set up the core timer interrupt with a prioirty of 2 and zero sub-priority
    INTSetVectorPriority(INT_CORE_TIMER_VECTOR, INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_CORE_TIMER_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_CT);
    INTEnable(INT_CT, INT_ENABLED);

    // set up the core software interrupt with a prioirty of 3 and zero sub-priority
    INTSetVectorPriority(INT_CORE_SOFTWARE_0_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_CORE_SOFTWARE_0_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_CS0);
    INTEnable(INT_CS0, INT_ENABLED);

    // set up the core software interrupt with a prioirty of 3 and zero sub-priority
    INTSetVectorPriority(INT_CORE_SOFTWARE_1_VECTOR, INT_PRIORITY_LEVEL_7);
    INTSetVectorSubPriority(INT_CORE_SOFTWARE_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_CS1);
    INTEnable(INT_CS1, INT_ENABLED);

	// configure outputs
	PORTClearBits(LED_PORT, (CORE_TIMER_LED | CORE_SW_0_LED | CORE_SW_1_LED));
	PORTSetPinsDigitalOut(LED_PORT, (CORE_TIMER_LED | CORE_SW_0_LED | CORE_SW_1_LED));

    // configure for multi-vectored mode
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

    // enable interrupts
    INTEnableInterrupts();


    while(1)
    {
        unsigned int i;

        for(i = 0; i < 0xfffff; i++)
            ;

        // create a core software 0 interrupt
        CoreSetSoftwareInterrupt0();

        for(i = 0; i < 0xfffff; i++)
            ;

        // create a core software 1 interrupt
        CoreSetSoftwareInterrupt1();

    }
}

/*********************************************************************
 * Function:        void _CoreTimerHandler(void)
 *
 * PreCondition:    none
 *
 * Input:           none
 *
 * Output:          none
 *
 * Side Effects:    none
 *
 * Overview:        The interrupt handler function for the core timer
 *
 * Note:            The jump to this handler will be placed in the vactor
 *                  location
 ********************************************************************/
void __ISR(_CORE_TIMER_VECTOR, IPL2SOFT) _CoreTimerHandler(void)
{
    INTClearFlag(INT_CT);                      // clear the interrupt flag
    UpdateCoreTimer(CORE_TIMER_PERIOD);     // update the period
    PORTToggleBits(LED_PORT, CORE_TIMER_LED);
}
/*********************************************************************
 * Function:       void _CoreSoftwareInt0Handler(void)
 *
 * PreCondition:    none
 *
 * Input:           none
 *
 * Output:          none
 *
 * Side Effects:    none
 *
 * Overview:        The interrupt handler function for the core software
 *                  interrupt
 *
 * Note:           The handler is placed directly into the vector location
 ********************************************************************/
void __ISR(_CORE_SOFTWARE_0_VECTOR, IPL3SOFT) _CoreSoftwareInt0Handler(void)
{
    INTClearFlag(INT_CS0);                      // clear the interrupt flag
    CoreClearSoftwareInterrupt0();              // clear the core int flag
    PORTToggleBits(LED_PORT, CORE_SW_0_LED);

}
/*********************************************************************
 * Function:       void _CoreSoftwareInt1Handler(void)
 *
 * PreCondition:    none
 *
 * Input:           none
 *
 * Output:          none
 *
 * Side Effects:    none
 *
 * Overview:        The interrupt handler fucntion for the core software
 *                  interrupt
 *
 * Note:            The jump to this handler will be placed in the vactor
 *                  location
 ********************************************************************/
void __ISR(_CORE_SOFTWARE_1_VECTOR, IPL7AUTO) _CoreSoftwareInt1Handler(void)
{
    INTClearFlag(INT_CS1);                      // clear the interrupt flag
    CoreClearSoftwareInterrupt1();                // clear the core int flag
    PORTToggleBits(LED_PORT, CORE_SW_1_LED);
}

