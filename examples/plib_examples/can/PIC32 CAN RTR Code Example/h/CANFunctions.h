/**********************************************************************
* © 2007 Microchip Technology Inc.
*
* FileName:        CANFunctions.h	 
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

#ifndef _CAN_FUNCTIONS_H_
#define _CAN_FUNCTIONS_H_

#include "GenericTypeDefs.h"
#include "h\Explorer16.h"
#include <xc.h>
#include <sys/kmem.h>
#include "plib.h"

#define SYSTEM_FREQ 80000000
#define CAN_BUS_SPEED 250000

BYTE CAN1MessageFifoArea[2 * 8 * 16];
BYTE CAN2MessageFifoArea[1 * 8 * 16];

/****************************************************************************
  Function:
    void CAN1Init(void);

  Description:
	This function initializes CAN1 module. It sets up speed, FIFOs,
	filters and interrupts. FIFO0 is set up for TX with 8 message 
	buffers. FIFO1 is set up for RX with 8 message buffers. Filter 0
	is set with Mask 0 for SID 0x201. Only RXNEMPTY interrupt and
	RBIF interrupt is enabled.	

  Precondition:
	None.

  Parameters:
	None.
	
  Return Values:
	None.
 
  Remarks:
 	None. 
	
  Example:
  	CAN1Init();
  ***************************************************************************/
void CAN1Init(void);

/****************************************************************************
  Function:
    void CAN2Init(void);

  Description:
	This function initializes CAN2 module. It sets up speed, FIFOs,
	filters and interrupts. FIFO0 is set up for TX with 8 message 
	buffers with RTR enabled. Filter 0 is set with Mask 0 for SID
	0x202 and points to FIFO0. Interrupts are not enabled.	

  Precondition:
	None.

  Parameters:
	None.
	
  Return Values:
	None.
 
  Remarks:
 	None. 
	
  Example:
  	CAN2Init();
  ***************************************************************************/
void CAN2Init(void);

/****************************************************************************
  Function:
    void CAN1RxMsgProcess(void);

  Description:
	This function checks if a message is available to be read in
	CAN1 FIFO1. If a message is available, the function will check
	byte 0 of the CAN message payload and will switch LED5 accordingly.

  Precondition:
	None.

  Parameters:
	None.
	
  Return Values:
	None.
 
  Remarks:
 	None. 
	
  Example:
  	CAN1RxMsgProcess();
  ***************************************************************************/
void CAN1RxMsgProcess(void);

/****************************************************************************
  Function:
    void CAN2UpdateLEDMessage(BYTE led5Command);

  Description:
	This function checks if a CAN2 FIFO0 is empty. If empty it creates a
	message with SID 0x201 and single byte data payload consisting of
	led5Command value. This message is will be the response to RTR message
	from CAN1.

  Precondition:
	None.

  Parameters:
	led5Command - 1 to switch on LED5, 0 to switch it off.
	
  Return Values:
	None.
 
  Remarks:
 	None. 
	
  Example:
  BYTE led5Command = 1;
  CAN2UpdateLEDMessage(led5Command);
  ***************************************************************************/
 void CAN2UpdateLEDMessage(BYTE led5Command);

/****************************************************************************
  Function:
   void CAN1TxSendRTRMsg(void);

  Description:
	This function will send a RTR CAN message with SID 0x202. Payload size of 
	this message is zero. CAN2 will respond to this message with the status 
	of LED5.

  Precondition:
	None.

  Parameters:
	None.
	
  Return Values:
	None.
 
  Remarks:
 	None. 
	
  Example:
  	CAN1TxSendRTRMsg();
  ***************************************************************************/
void CAN1TxSendRTRMsg(void);




#endif
