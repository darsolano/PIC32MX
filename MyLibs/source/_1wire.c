/*********************************************************************
 *
 *                 1-Wire Communication Protocol
 *
 *********************************************************************
 * FileName:        1wire.c
 * Dependencies:
 * Processor:       PIC32MX360F512L
 * Complier:        MPLAB X IDE v2.05 XC32 1.31
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright � 2004-2007 Microchip Technology Inc.  All rights reserved.
 *
 * Microchip licenses to you the right to use, copy and distribute Software 
 * only when embedded on a Microchip microcontroller or digital signal 
 * controller and used with a Microchip radio frequency transceiver, which 
 * are integrated into your product or third party product (pursuant to the 
 * sublicense terms in the accompanying license agreement).  You may NOT 
 * modify or create derivative works of the Software.  
 *
 *
 * You should refer to the license agreement accompanying this Software for 
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY 
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR 
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED 
 * UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF 
 * WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR 
 * EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, 
 * PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY 
 * THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER 
 * SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Sasha     			12/20/07    Original
 ********************************************************************/

/****** I N C L U D E S **********************************************************/
#include "_1wire.h"
#include "utilities.h"
#include <define_pins.h>


//ONE WIRE PORT PIN DEFINITION
///****************************************************
// This Configuration is required to make any PIC MicroController
// I/O pin as Open drain to drive 1-wire.
// Verify carefully to choose the free pins on board, Avoid Mistakes and errors
///****************************************************

//#define OW_PIN_DIRECTION 	TRISDbits.TRISD9 
//#define OW_PIN  		LATDbits.LATD9

DEFINE_PIN(OneWire, G, 0)

//****** V A R I A B L E S ********************************************************/
unsigned char macro_delay;

/**********************************************************************
 * Function:        void drive_OW_low (void)
 * PreCondition:    None
 * Input:		   None	
 * Output:		   None	
 * Overview:		   Configure the OW_PIN as Output and drive the OW_PIN LOW.	
 ***********************************************************************/
PRIVATE void drive_OW_low(void)
{
    OneWire_OUTPUT();
    OneWire_LOW();
}

/**********************************************************************
 * Function:        void drive_OW_high (void)
 * PreCondition:    None
 * Input:		   None	
 * Output:		   None	
 * Overview:		   Configure the OW_PIN as Output and drive the OW_PIN HIGH.	
 ***********************************************************************/
PRIVATE void drive_OW_high(void)
{
    OneWire_OUTPUT();
    OneWire_HIGH();
}

/**********************************************************************
 * Function:        unsigned char read_OW (void)
 * PreCondition:    None
 * Input:		   None	
 * Output:		   Return the status of OW pin.	
 * Overview:		   Configure as Input pin and Read the status of OW_PIN 	
 ***********************************************************************/
PRIVATE UCHAR8 read_OW(void)
{

    UCHAR8 read_data = 0;
    OneWire_INPUT();
    if (OneWire_READ() == HIGH)
        read_data = SET;
    else
        read_data = CLEAR;
    return read_data;
}

/**********************************************************************
 * Function:        unsigned char OW_reset_pulse(void)
 * PreCondition:    None
 * Input:		   None	
 * Output:		   Return the Presense Pulse from the slave.	
 * Overview:		   Initialization sequence start with reset pulse.
 *				   This code generates reset sequence as per the protocol
 ***********************************************************************/
PRESENCE_t OW_reset_pulse(void)
{
    PRESENCE_t presence;

    drive_OW_low();             // Drive the bus low
    ShortDelay(DELAY_480Us);    // delay 480 microsecond + 60 (us) Master reset
    OneWire_INPUT();
    //drive_OW_high();          // Release the bus
    ShortDelay(DELAY_70Us);     // delay 64 microsecond (us)
    presence = read_OW();       // Sample for presence pulse from slave
    ShortDelay(DELAY_410Us);    // delay 410 microsecond (us)
    drive_OW_high();            // Release the bus
    if (presence) return (OW_DEV_NOT_PRESENT);
    else return (OW_DEV_PRESENT);
}

/**********************************************************************
 * Function:        void OW_write_bit (unsigned char write_data)
 * PreCondition:    None
 * Input:		   Write a bit to 1-wire slave device.
 * Output:		   None
 * Overview:		   This function used to transmit a single bit to slave device.
 *				   
 ***********************************************************************/

PRIVATE void OW_write_bit(UCHAR8 write_bit)
{
    if (write_bit)
    {
        //writing a bit '1'
        drive_OW_low(); // Drive the bus low
        ShortDelay(DELAY_6Us); // delay 6 microsecond (us)
        drive_OW_high(); // Pull High
        ShortDelay(DELAY_64Us); // delay 64 microsecond (us)
    }
    else
    {
        //writing a bit '0'
        drive_OW_low(); // Drive the bus low
        ShortDelay(DELAY_60Us); // delay 60 microsecond (us)
        drive_OW_high();      // Release the bus
        ShortDelay(delay_10us); // delay 10 microsecond for recovery (us)
    }
}

/**********************************************************************
 * Function:        unsigned char OW_read_bit (void)
 * PreCondition:    None
 * Input:           None
 * Output:          Return the status of the OW PIN
 * Overview:        This function used to read a single bit from the slave device.
 *				   
 ***********************************************************************/

PRIVATE UCHAR8 OW_read_bit(void)
{
    UCHAR8 read_data;
    //reading a bit 
    drive_OW_low();         // Drive the bus low
    ShortDelay(DELAY_6Us);  // delay 6 microsecond (us)
    OneWire_INPUT();        // Release the bus
    ShortDelay(DELAY_9Us);
    read_data = read_OW();  //Read the status of OW_PIN
    ShortDelay(DELAY_55Us);
    return read_data;
}

/**********************************************************************
 * Function:        void OW_write_byte (unsigned char write_data)
 * PreCondition:    None
 * Input:		   Send byte to 1-wire slave device
 * Output:		   None
 * Overview:		   This function used to transmit a complete byte to slave device.
 *				   
 ***********************************************************************/
void OW_write_byte(UCHAR8 write_data)
{
    UCHAR8 loop;
    for (loop = 0; loop < 8; loop++)
    {
        OW_write_bit(write_data & 0x01); //Sending LS-bit first
        write_data >>= 1; // shift the data byte for the next bit to send
    }
}

/**********************************************************************
 * Function:        unsigned char OW_read_byte (void)
 * PreCondition:    None
 * Input:		   None
 * Output:		   Return the read byte from slave device
 * Overview:		   This function used to read a complete byte from the slave device.
 *				   
 ***********************************************************************/

UCHAR8 OW_read_byte(void)
{
    UCHAR8 loop, result = 0;
    for (loop = 0; loop < 8; loop++)
    {
        result >>= 1; // shift the result to get it ready for the next bit to receive
        if (OW_read_bit())
            result |= 0x80; // if result is one, then set MS-bit
    }
    return result;
}

/********************************************************************************************
                  E N D     O F     1 W I R E . C  
 *********************************************************************************************/