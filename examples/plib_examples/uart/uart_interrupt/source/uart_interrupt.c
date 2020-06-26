/*********************************************************************
 *
 *                  UART Interrupt Example
 *
 *********************************************************************
 * FileName:        uart_interrupt.c
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
 * $Id: uart_interrupt.c 9390 2008-06-16 23:43:04Z rajbhartin $
 * $Name: x.x $
 *
 **********************************************************************/

/***********************************************************************************
		UART Interrupt Example README
 ***********************************************************************************
 * Objective: Become familiar with PIC32MX tool suite and understand
 *		 	  basic UART Interrupt operations.
 *
 * Tools:
 *			1. MPLAB IDE with PIC32MX support
 *			2. C32 Compiler
 *			3. Explorer 16 Rev 4 or 5 board.
 *			4. RS-232 Cable
 *			5. A Terminal program for Windows - HyperTerminal
 *
 *
 ***********************************************************************************
 ***********************************************************************************/

#include <plib.h>			// Peripheral Library


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
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

// *****************************************************************************
// *****************************************************************************
// Section: System Macros
// *****************************************************************************
// *****************************************************************************
#define	GetSystemClock()              (80000000ul)
#define	GetPeripheralClock()          (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()         (GetSystemClock())

#define DESIRED_BAUDRATE              (9600)      //The desired BaudRate


void WriteString(const char *string);

int main(void)
{

  // Configure the device for maximum performance but do not change the PBDIV
  // Given the options, this function will change the flash wait states, RAM
  // wait state and enable prefetch cache but will not change the PBDIV.
  // The PBDIV value is already set via the pragma FPBDIV option above..
  SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

  mPORTAClearBits(BIT_7); 		// Turn off RA7 on startup.
  mPORTASetPinsDigitalOut(BIT_7);	// Make RA7 as output.


  // Explorer-16 uses UART2 to connect to the PC.
  // This initialization assumes 36MHz Fpb clock. If it changes,
  // you will have to modify baud rate initializer.
  UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
  UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
  UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
  UARTSetDataRate(UART2, GetPeripheralClock(), DESIRED_BAUDRATE);
  UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

  // Configure UART2 RX Interrupt
  INTEnable(INT_SOURCE_UART_RX(UART2), INT_ENABLED);
  INTSetVectorPriority(INT_VECTOR_UART(UART2), INT_PRIORITY_LEVEL_2);
  INTSetVectorSubPriority(INT_VECTOR_UART(UART2), INT_SUB_PRIORITY_LEVEL_0);

  // configure for multi-vectored mode
  INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

  // enable interrupts
  INTEnableInterrupts();


  WriteString("*** UART Interrupt-driven Application Example ***\r\n");
  WriteString("*** Type some characters and observe echo and RA7 LED toggle ***\r\n");


  // Let interrupt handler do the work
  while (1);

  return 0;
}
// helper functions
void WriteString(const char *string)
{
  while (*string != '\0')
    {
      while (!UARTTransmitterIsReady(UART2))
        ;

      UARTSendDataByte(UART2, *string);

      string++;

      while (!UARTTransmissionHasCompleted(UART2))
        ;
    }
}
void PutCharacter(const char character)
{
  while (!UARTTransmitterIsReady(UART2))
    ;

  UARTSendDataByte(UART2, character);

  while (!UARTTransmissionHasCompleted(UART2))
    ;
}
// UART 2 interrupt handler
// it is set at priority level 2 with software context saving
void __ISR(_UART2_VECTOR, IPL2SOFT) IntUart2Handler(void)
{
  // Is this an RX interrupt?
  if (INTGetFlag(INT_SOURCE_UART_RX(UART2)))
    {
      // Echo what we just received.
      PutCharacter(UARTGetDataByte(UART2));

      // Clear the RX interrupt Flag
      INTClearFlag(INT_SOURCE_UART_RX(UART2));

      // Toggle LED to indicate UART activity
      mPORTAToggleBits(BIT_7);

    }

  // We don't care about TX interrupt
  if ( INTGetFlag(INT_SOURCE_UART_TX(UART2)) )
    {
      INTClearFlag(INT_SOURCE_UART_TX(UART2));
    }
}

