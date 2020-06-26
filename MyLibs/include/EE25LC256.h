/* 
 * File:   EE25LC256.h
 * Author: dsolano
 *
 * Created on June 28, 2014, 5:02 AM
 */

#ifndef EE25LC256_H
#define	EE25LC256_H

#ifdef	__cplusplus
extern "C" {
#endif


/**********************************************************************
* ?2007 Microchip Technology Inc.
*
* FileName:        EE25LC256.h
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

#include <xc.h>
#include <plib.h>
#include <GenericTypeDefs.h>

// peripheral configurations
#define EEPROM_CHANNEL  SPI_CHANNEL1
#define FREQ_PBCLK      80000000
#define FREQ_SPICHN     5000000
#define SPI_BRG         (FREQ_PBCLK/(2 *FREQ_SPICHN)) - 1


/************************************************************************
* EEPROM Commands                                                       *
*                                                                       *
************************************************************************/
#define EEPROM_PAGE_SIZE    (unsigned)64
#define EEPROM_PAGE_MASK    (unsigned)0x003f
#define EEPROM_CMD_READ     (unsigned)0b00000011
#define EEPROM_CMD_WRITE    (unsigned)0b00000010
#define EEPROM_CMD_WRDI     (unsigned)0b00000100
#define EEPROM_CMD_WREN     (unsigned)0b00000110
#define EEPROM_CMD_RDSR     (unsigned)0b00000101
#define EEPROM_CMD_WRSR     (unsigned)0b00000001

/************************************************************************
* EEPROM Status Register Bits                                                       *
*                                                                       *
************************************************************************/
#define WEL_IS_SET          0x02
#define BUSY_WRITING        0x01

/************************************************************************
* Aliases for IOs registers related to SPI connected to EEPROM          *
*                                                                       *
************************************************************************/
#define EEPROM_SS_ACTIVE()    TRISDbits.TRISD12 = 0
#define EEPROM_SS_PORT_RDY()  LATDbits.LATD12   = 1
#define EEPROM_SS_ENABLE()    LATDbits.LATD12   = 0
#define EEPROM_SS_DISABLE()   LATDbits.LATD12   = 1

/************************************************************************
* Bounderies of EEPROM 24LC256                                          *
*                                                                       *
************************************************************************/
#define MAX_ADDRESS             0x7fff  //32768 bytes
#define EEP_PAGE_SIZE           0x3f    //64 byte per page
#define MAX_AMOUNT_PAGE         0x1ff   //512 paginas
#define PAGE_INCREMENT          0x40    //64 bytes increment + 1
#define START_ADDRESS           0x0000  // 0x0000 is the start ADDRESS

/************************************************************************
* Structure STATREG and union _EEPROMStatus_                            *
*                                                                       *
* Overview: Provide a bits and byte access to EEPROM status value.      *
*                                                                       *
************************************************************************/
struct  STATREG{
	unsigned    WIP:1;
	unsigned    WEL:1;
	unsigned    BP0:1;
	unsigned    BP1:1;
	unsigned    RESERVED:3;
	unsigned    WPEN:1;
};

union _EEPROMStatus_{
	struct  STATREG Bits;
	unsigned char	Char;
};

/************************************************************************
* Macro: Lo                                                             *
*                                                                       *
* Preconditions: None                                                   *
*                                                                       *
* Overview: This macro extracts a low byte from a 2 byte word.          *
*                                                                       *
* Input: None.                                                          *
*                                                                       *
* Output: None.                                                         *
*                                                                       *
************************************************************************/
#define LoByte(X)   (unsigned char)(X&0x00ff)

/************************************************************************
* Macro: Hi                                                             *
*                                                                       *
* Preconditions: None                                                   *
*                                                                       *
* Overview: This macro extracts a high byte from a 2 byte word.         *
*                                                                       *
* Input: None.                                                          *
*                                                                       *
* Output: None.                                                         *
*                                                                       *
************************************************************************/
#define HiByte(X)   (unsigned char)((X>>8)&0x00ff)

/************************************************************************
* Function: EEPROMInit                                                  *
*                                                                       *
* Preconditions: SPI module must be configured to operate with          *
*                 parameters: Master, MODE16 = 0, CKP = 1, SMP = 1.     *
*                                                                       *
* Overview: This function setup SPI IOs connected to EEPROM.            *
*                                                                       *
* Input: None.                                                          *
*                                                                       *
* Output: None.                                                         *
*                                                                       *
************************************************************************/
extern void SpiEEPROMInit(void);

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
extern void EEPROMWriteByte(unsigned char, unsigned int);

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
extern unsigned char EEPROMReadByte(unsigned int);

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
extern BOOL EEPROMWriteEnable(void);
extern BOOL EEPROMWriteDisable(void);
UCHAR8 EEPROMReadStatusRegister();

/************************************************************************
* Function: EEPROMWriteStringROM(UCHAR8 * buffer, int size, UINT16 address)*
*                                                                       *
* Preconditions: SPI module must be configured to operate with EEPROM.  *
*                                                                       *
* Overview: This function writes a string from ROM based string defined *
* by string in quotes outside the program memory                        *
* Input: *buffer[] - pointer to the string                              *
*        size - lenght of the string                                    *
*        address - location to start writing string. Becareful not to   *
*        cross page boundaries. PAGE size is 64 bytes                   *
* Output: None.                                                         *
*                                                                       *
************************************************************************/
UINT16 EEPROMWriteString(UCHAR8 * buffer, int size, UINT16 address );

/************************************************************************
* Function: EEPROMWriteStringROM(UCHAR8 * buffer, int size, UINT16 address)*
*                                                                       *
* Preconditions: SPI module must be configured to operate with EEPROM.  *
*                                                                       *
* Overview: This function writes a string from ROM based string defined *
* by string in quotes outside the program memory                        *
* Input: *buffer[] - pointer to the string to read                      *
*        size - lenght of the string to read                            *
*        address - location to start writing string. Becareful not to   *
*        cross page boundaries. PAGE size is 64 bytes                   *
* Output: None.                                                         *
*                                                                       *
************************************************************************/
void EEPROMReadString(UCHAR8 * buffer, int size, UINT16 address );




#ifdef	__cplusplus
}
#endif

#endif	/* EE25LC256_H */

