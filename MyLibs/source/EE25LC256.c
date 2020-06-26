
/**********************************************************************
* ?2007 Microchip Technology Inc.
*
* FileName:        EE25LC256.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32MX360F512L
* Compiler:        MPLAB?C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your
* behalf, exclusively with Microchip's proprietary products.  Your
* acceptance and/or use of this code constitutes agreement to the
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT,
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY,
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author        Date      	Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Albert Z.		05/16/08	First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* 25LC256 is connected to SPI2
************************************************************************/
#include "EE25LC256.h"
#include "utilities.h"

/************************************************************************
* Function: EEPROMInit                                                  *
*                                                                       *
* Preconditions: SPI module must be configured to operate with          *
*                 parameters: Master, MODE8 = 0, CKP = 1, SMP = 1.     *
*                                                                       *
* Overview: This function setup SPI IOs connected to EEPROM.            *
*                                                                       *
* Input: None.                                                          *
*                                                                       *
* Output: None.                                                         *
*                                                                       *
************************************************************************/
void SpiEEPROMInit(void)
{
    //unsigned int ClockHz;
    // Set IOs directions for EEPROM SPI
    
    pic32_Spi_Init(EEPROM_CHANNEL,FREQ_SPICHN);
    EEPROM_SS_ACTIVE();
    EEPROM_SS_PORT_RDY();
    EEPROM_SS_DISABLE();
}

/************************************************************************
* Function: EEPROMWriteByte()                                           *
*                                                                       *
* Preconditions: SPI module must be configured to operate with  EEPROM. *
*                                                                       *
* Overview: This function writes a new value to address specified.      *
*                                                                       *
* Input: Data to be written and address.                                *
*                                                                       *
* Output: None.                                                         *
*                                                                       *
************************************************************************/
void EEPROMWriteByte(unsigned char Data, unsigned int Address)
{
    while(!EEPROMWriteEnable());
	EEPROM_SS_ENABLE();
    SpiIO(EEPROM_CHANNEL , EEPROM_CMD_WRITE);
    SpiIO(EEPROM_CHANNEL , HiByte(Address));
    SpiIO(EEPROM_CHANNEL , LoByte(Address));
    SpiIO(EEPROM_CHANNEL , Data);
	EEPROM_SS_DISABLE();
	while(EEPROMReadStatusRegister());
}

/************************************************************************
* Function: EEPROMReadByte()                                            *
*                                                                       *
* Preconditions: SPI module must be configured to operate with  EEPROM. *
*                                                                       *
* Overview: This function reads a value from address specified.         *
*                                                                       *
* Input: Address.                                                       *
*                                                                       *
* Output: Data read.                                                    *
*                                                                       *
************************************************************************/
unsigned char EEPROMReadByte(unsigned int Address)
{
	unsigned char data;
    EEPROM_SS_ENABLE();
    SpiIO(EEPROM_CHANNEL , EEPROM_CMD_READ);
    SpiIO(EEPROM_CHANNEL , HiByte(Address));
    SpiIO(EEPROM_CHANNEL , LoByte(Address));
    data = SpiIO(EEPROM_CHANNEL,0);
    EEPROM_SS_DISABLE();
    return data;
}

/************************************************************************
* Function: EEPROMWriteEnable()                                         *
*                                                                       *
* Preconditions: SPI module must be configured to operate with EEPROM.  *
*                                                                       *
* Overview: This function allows a writing into EEPROM. Must be called  *
* before every writing command.                                         *
*                                                                       *
* Input: None.                                                          *
*                                                                       *
* Output: None.                                                         *
*                                                                       *
************************************************************************/
BOOL EEPROMWriteEnable()
{
    EEPROM_SS_ENABLE();
    SpiIO(EEPROM_CHANNEL , EEPROM_CMD_WREN);
	EEPROM_SS_DISABLE();
	if (EEPROMReadStatusRegister()) return TRUE;
		else return FALSE;
}

BOOL EEPROMWriteDisable()
{
	if (EEPROMReadStatusRegister()) return FALSE;
	else{
	    EEPROM_SS_ENABLE();
	    SpiIO(EEPROM_CHANNEL , EEPROM_CMD_WRDI);
		EEPROM_SS_DISABLE();
		return TRUE;
	}
}

// waitBusy() - checks if EEPROM is ready to be modified and waits if not ready
UCHAR8 EEPROMReadStatusRegister()
{
    UCHAR8 status = 0;
    EEPROM_SS_ENABLE();                         // Select EEPROM
    SpiIO(EEPROM_CHANNEL, EEPROM_CMD_RDSR);           // Read EEPROM status register
    status = SpiIO(EEPROM_CHANNEL, 0);     // send dummy byte to receive incoming data
    EEPROM_SS_DISABLE();                         // Release EEPROM
    return status;
} // END waitBusy()

UINT16  EEPROMWriteString(UCHAR8 * buffer, int size, UINT16 address ){

	return address;
}

