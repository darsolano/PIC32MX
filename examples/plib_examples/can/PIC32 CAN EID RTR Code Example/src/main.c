/**********************************************************************
* © 2009 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32
* Compiler:        MPLAB® C32 
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

#include <xc.h>
#include <peripheral\system.h>

#include "..\h\GenericTypeDefs.h"
#include "..\h\CANFunctions.h"
#include "..\h\Explorer16.h"

/* This code example runs on a Explorer 16 board
 * with a 8Mhz crystal and the ECAN LIN Pictail
 * plus board. The CAN1 and CAN2 connecters on the
 * ECAN LIN Pictail plus board are connected to 
 * each other to form a 2 node CAN bus.*/

/* The code example shows how to use the  PIC32 CAN module 
 * to send and receive Extended ID Remote Transmit Request (RTR)
 * messages. In this example. CAN1 sends an RTR request to
 * CAN2 every 10msec for the next status of LED5. CAN2 
 * replies with a  message containing LED5 command (ON or OFF).
 * The code toggles the status of LED5 command every second.
 *
 * A timer is setup for 0.1 millisecond. This is used to obtain a 1 second 
 * timer. The LED5 command  is toggled every second. The 0.1 millisecond timer
 * is also used to obtain the 10 msec timer for CAN1 RTR message send event.*/

/* Configuration Bit settings
 * SYSCLK = 80 MHz (((8MHz Crystal/ FPLLIDIV) * FPLLMUL) / FPLLODIV)
 * PBCLK = 80 MHz
 * Primary Osc w/PLL (XT+,HS+,EC+PLL)
 * WDT OFF
 * Other options are don't care */

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

#define SYS_FREQ 				(80000000L)

/* CAN1 module  uses FIFO0 for transmit and FIFO1 
 * for receive. Each FIFO will  be configured to be
 * 8 messages deep. CAN2 uses FIFO0 as transmit FIFO
 * with RTR enabled. FIFO size is set to 8. CAN module
 * configuration code is in CANFunctions.c */ 

/* CAN1 module uses 2 Channels (Channel 0
 * and Channel 1). Each channel is configured
 * to be 8 messages deep. Channel 0 is configured
 * for Transmit and Channel 1 is configured for
 * receive. CAN module configuration code
 * is in CANFunctions.c */ 
 
/* CAN2 module uses 1 channel (Channel 0).
 * This channel is configured to be 8 
 * messages deep. The channel is configured
 * as a transmit channel and is RTR enabled.
 * When filter 0 receives RTR message, then
 * Channel 0 will transmit all it contents.*/
 
/* Note the size of CAN1 message area.
 * It is 2 (Channels) * 8 (Messages Buffers) 
 * 16 (bytes/per message buffer) bytes. 
 * 
 * Note the size of CAN2 message area.
 * It is 1 (Channel) * 8 (Messages Buffers 
 * 16 (bytes/per message buffer) bytes. */

BYTE CAN1MessageFifoArea[2 * 8 * 16];
BYTE CAN2MessageFifoArea[1 * 8 * 16];

int main(void)
{
	BYTE led5Command;	/* 0 for LED5 OFF, 1 for LED5 ON. */

    /* Configure system for maximum performance
     * and enable multi vector interrupts. */
     
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
    
	/* Intialize the Explorer16 LED ports
	 * CAN module and start the timer. Make
	 * all analog ports digital. */
	 
	AD1PCFGSET = 0xFFFFFFFF;

	Explorer16Init();   /* Function is defined in Explorer16.c      */
 	CAN1Init();		    /* Function is defined in CANFunctions.c 	*/
	CAN2Init();		    /* Function is defined in CANFunctions.c 	*/
	Timer1Init();	    /* Function is defined in TimerFunctions.c	*/
	led5Command = 0;	/* Start with LED5 off. */

	while(1)
	{
		/* In this loop, we check if a 10 msec tick is available. If so then
		 * CAN1 will send a RTR message to CAN2. Next we check if a 1 second
		 * tick is available. If so then the state  of LED5 command is toggled.
		 * CAN1 will  process received messages every time in the loop. 
		 * */

		if(IsTenMsecUp() == TRUE)
		{
			/* If 10 milliseconds is up then CAN1 sends
			 * a RTR message to CAN2 to toggle LED5.
			 * */

		   CAN1TxSendRTRMsg();	/* Function is defined in CANFunctions.c	*/	
		}    

		if(IsOneSecondUp() == TRUE)
		{
			/* If 1 second is up then toggle the led
			 * LED5 command.
			 */

			led5Command ^= 1;
		}	
		
		/* CAN2UpdateLEDMessage will check if CAN2 is FIFO empty and populate
		 * it with a CAN message containing the latest state of LED5.*/

		CAN2UpdateLEDMessage(led5Command);

		/* CAN1RxMsgProcess() will check if CAN1 has received a message from 
		 * CAN2 and will switch LED5 on or off based on the LED command 
		 * received from CAN2. */

		CAN1RxMsgProcess();
	}

    
}

