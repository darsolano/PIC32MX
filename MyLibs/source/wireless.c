/*
 * nRF24L01.c
 *
 *  Created on: 25/12/2014
 *      Author: dsolano
 ** @mainpage
 @version 0.9
 @author
 Copyright (C) 2014 Darwin O. Solano

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 @section intro Introduction
 This library is intended to be universal and easy to use.

 Here's the functions we want to support:

 Set our local address. Think of this as our MAC or hardware address.
 Configure our RF24 channel or frequency.
 Configure the data rate mode of the RF24.
 Turn the RF24 receiver on/off.
 Power radio on/off.
 Configure dynamic or fixed length payloads.
 Configuring transparent operating mode of our modem.
 Configuring RF24 receiver pipeline address (0-5).
 Set Shockburst mode, timeouts, and retries.
 RF24 CRC, off, 1 or 2 bytes
 Store the modem configuration on EEPROM.
 Reset the uC + RF24 module; restarting with saved configuration.
 Scan a frequency

 */


#include <wireless.h>
#include <plib.h>
#include <utilities.h>
#include <string.h>
#include <UART_app.h>
#include <UDBconfig.h>

	 //* CE PIN RD8	-- RADIO
	 //* CSN PIN RD9

/**
 * Set chip enable
 *
 * @param level HIGH to actively begin transmission or LOW to put in standby.  Please see data sheet
 * for a much more detailed description of this pin.
 */
#define RADIO_ON	PORTDbits.RD8 = 1
#define RADIO_OFF	PORTDbits.RD8 = 0


/**
 * Set chip select pin
 *
 * @param mode HIGH to take this unit off the SPI bus, LOW to put it on
 */
#define CSN_ON		PORTDbits.RD9 = 0
#define CSN_OFF		PORTDbits.RD9 = 1


const uint8_t TX_PAYLOAD[16] = 		{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                                 	 0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
const uint8_t _TX_ADDR[] = {0x0a,0x0a,0x0a,0x0a,0x10};
uint8_t databuff[32];
uint8_t rxbuff[5];

// ---------------------------------------------------------------------------
// This one send and receive data from SPI on the selected channel
// ---------------------------------------------------------------------------

UCHAR8 SPI_IO ( SpiChannel channel, UCHAR8 data ) // Only for SPI Channel 1 & 2
{
    if (channel == SPI_CHANNEL2) {
        SPI2BUF = data; // write to buffer for TX
        while (!SPI2STATbits.SPIRBF); // wait transfer complete
        return SPI2BUF; // read the received value
    } else if (channel == SPI_CHANNEL1) {
        SPI1BUF = data; // write to buffer for TX
        while (!SPI1STATbits.SPIRBF); // wait transfer complete
        return SPI1BUF; // read the received value
    }
} // END SPI_IO()


/************************************************************************
 * Function: SpiInit                                                 *
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
static void SpiInit ( SpiChannel channel )
{
    //* CE PIN RD8
    //* CSN PIN RD9
    TRISDbits.TRISD8 = OUT;	// CE Radio Pin
    TRISDbits.TRISD9 = OUT;	// CSN Chip Select Pin from SPI
    SpiChnEnable(channel,false);
    SpiChnOpen(channel,SPI_OPEN_MSTEN | SPI_OPEN_MODE8 , FREQ_PBCLK/WFREQ_SPICHN);
    SpiChnEnable(channel,true);
}


/****************************************************************
 * RADIO BASIC LOW LEVEL FUNCTIONS - ALL PRIVATE
 ****************************************************************/

/**
 Read a register

 @param Reg Register to read

 @return Registry Value
*/
char NRF24L01_ReadReg(char Reg) {
	char Result;

	CSN_ON;
	SPI_IO(NRF24_SPICHN , Reg);
	Result = SPI_IO(NRF24_SPICHN,NOP);
	CSN_OFF;

	return Result;
}

/**
 Returns the STATUS register and then modify a register

 @param Reg Register to change
 @param Value New value

 @return STATUS Register
*/
char NRF24L01_WriteReg(char Reg, char Value) {
	char Result;

	CSN_ON;
	Result = SPI_IO(NRF24_SPICHN , Reg);
	SPI_IO(NRF24_SPICHN , Value);
	CSN_OFF;

	return Result;
}

/**
 Returns the STATUS register and then read "n" registers

 @param Reg Register to read
 @param Buf Pointer to a buffer
 @param Size Buffer Size

 @return STATUS Register
*/
char NRF24L01_ReadRegBuf(char Reg, char *Buf, int Size) {
	int i;
	char Result;

	CSN_ON;
	Result = SPI_IO(NRF24_SPICHN , Reg);

	for (i = 0; i < Size; i++) {
		Buf[i] = SPI_IO(NRF24_SPICHN , NOP);
	}

	CSN_OFF;

	return Result;
}

/**
 Returns the STATUS register and then write "n" registers

 @param Reg Registers to change
 @param Buf Pointer to a buffer
 @param Size Buffer Size

 @return STATUS Register
*/
char NRF24L01_WriteRegBuf(char Reg, char *Buf, int Size) {
	int i;
	char Result;

	CSN_ON;
	Result = SPI_IO(NRF24_SPICHN , Reg);

	for (i = 0; i < Size; i++) {
		SPI_IO(NRF24_SPICHN , Buf[i]);
	}
	CSN_OFF;

	return Result;
}

/**
 Returns the STATUS register

 @return STATUS Register
*/
char NRF24L01_Get_Status(void) {
	char Result;

	CSN_ON;
	Result = SPI_IO(NRF24_SPICHN , NOP);
	CSN_OFF;

	return Result;
}

/**
 Returns the carrier signal in RX mode (high when detected)

 @return CD
*/
char NRF24L01_Get_CD(void) {
	return (NRF24L01_ReadReg(CD) & 1);
}

/**
 Select power mode

 @param Mode = _POWER_DOWN, _POWER_UP

 @see _POWER_DOWN
 @see _POWER_UP

*/
void NRF24L01_Set_Power(char Mode) {
	char Result;

	Result = NRF24L01_ReadReg(CONFIG) & 0b01111101; // Read Conf. Reg. AND Clear bit 1 (PWR_UP) and 7 (Reserved)
	NRF24L01_WriteReg(W_REGISTER | CONFIG, Result | Mode);
}

/**
 Select the radio channel

 @param CH = 0..125

*/
void NRF24L01_Set_CH(char CH) {
	NRF24L01_WriteReg(W_REGISTER | RF_CH, (CH & 0b01111111)); // Clear bit 8
}

/**
 Select Enhanced ShockBurst ON/OFF

 Disable this functionality to be compatible with nRF2401

 @param Mode = _ShockBurst_ON, _ShockBurst_OFF

 @see _ShockBurst_ON
 @see _ShockBurst_OFF

*/
void NRF24L01_Set_ShockBurst(char Mode) {
	NRF24L01_WriteReg(W_REGISTER | SETUP_RETR, Mode);
	NRF24L01_WriteReg(W_REGISTER | EN_AA, Mode);
}

/**
 Select the address width

 @param Width = 3..5
*/
void NRF24L01_Set_Address_Width(char Width) {
	NRF24L01_WriteReg(W_REGISTER | SETUP_AW, (Width & 3) -2);
}

/**
 Select mode receiver or transmitter

 @param Device_Mode = _TX_MODE, _RX_MODE

 @see _TX_MODE
 @see _RX_MODE
*/
void NRF24L01_Set_Device_Mode(char Device_Mode) {
	char Result;

	Result = NRF24L01_ReadReg(CONFIG) & 0b01111110; // Read Conf. Reg. AND Clear bit 0 (PRIM_RX) and 7 (Reserved)
	NRF24L01_WriteReg(W_REGISTER | CONFIG, Result | Device_Mode);
}

/**
 Enables and configures the pipe receiving the data

 @param PipeNum Number of pipe
 @param Address Address
 @param AddressSize Address size
 @param PayloadSize Buffer size, data receiver

*/
void NRF24L01_Set_RX_Pipe(char PipeNum, char *Address, int AddressSize, char PayloadSize) {
	char Result;

	Result = NRF24L01_ReadReg(EN_RXADDR);
	NRF24L01_WriteReg(W_REGISTER | EN_RXADDR, Result | (1 << PipeNum));

	NRF24L01_WriteReg(W_REGISTER | (RX_PW_P0 + PipeNum), PayloadSize);
	NRF24L01_WriteRegBuf(W_REGISTER | (RX_ADDR_P0 + PipeNum), Address, AddressSize);
}

/**
 Disable all pipes
*/
void NRF24L01_Disable_All_Pipes(void) {
	NRF24L01_WriteReg(W_REGISTER | EN_RXADDR, 0);
}


/** Returns the STATUS register and then clear all interrupts
 *
 * @return STATUS Register
 */
char NRF24L01_Clear_Interrupts(void) {
	return NRF24L01_WriteReg(W_REGISTER | STATUS, _RX_DR | _TX_DS | _MAX_RT);
}

/**
 Sets the direction of transmission

 @param Address Address
 @param Size Address size 3..5

*/
void NRF24L01_Set_TX_Address(char *Address, int Size) {
	NRF24L01_WriteRegBuf(W_REGISTER | TX_ADDR, Address, Size);
}

/**
 Empty the transmit buffer

*/
void NRF24L01_Flush_TX(void) {
	CSN_ON;
	SPI_IO(NRF24_SPICHN , FLUSH_TX);
	CSN_OFF;
}

/**
 Empty the receive buffer
*/
void NRF24L01_Flush_RX(void) {
	CSN_ON;
	SPI_IO(NRF24_SPICHN , FLUSH_RX);
	CSN_OFF;
}

/**
 Initializes the device
 @param Device_Mode = _TX_MODE, _RX_MODE
 @param CH = 0..125
 @param DataRate = _1Mbps, _2Mbps
 @param Address Address
 @param Address_Width Width direction: 3..5
 @param Size_Payload Data buffer size

 @see _TX_MODE
 @see _RX_MODE
 @see _1Mbps
 @see _2Mbps
*/
void NRF24L01_Init(char Device_Mode, char CH, char DataRate,
		char *Address, char Address_Width, char Size_Payload) {

	SpiInit (NRF24_SPICHN);
	// Disable Enhanced ShockBurst
	NRF24L01_Set_ShockBurst(_ShockBurst_OFF);

	// RF output power in TX mode = 0dBm (Max.)
	// Set LNA gain
	NRF24L01_WriteReg(W_REGISTER | RF_SETUP, 0b00000111 | DataRate);

	NRF24L01_Set_Address_Width(Address_Width);

	NRF24L01_Set_RX_Pipe(0, Address, Address_Width, Size_Payload);

	NRF24L01_Set_CH(CH);

	NRF24L01_Set_TX_Address(Address, Address_Width); // Set Transmit address

	// Bits 4..6: Reflect interrupts as active low on the IRQ pin
	// Bit 3: Enable CRC
	// Bit 2: CRC 1 Byte
	// Bit 1: Power Up
	NRF24L01_WriteReg(W_REGISTER | CONFIG, 0b00001010 | Device_Mode);

	ShortDelay (1500);
}

/**
 Turn on transmitter, and transmits the data loaded into the buffer
*/
void NRF24L01_RF_TX(void) {
	RADIO_OFF;
	RADIO_ON;
	ShortDelay (10);
	RADIO_OFF;
}

/**
 Writes the buffer of data transmission

 @param Buf Buffer with data to send
 @param Size Buffer size

*/
void NRF24L01_Write_TX_Buf(char *Buf, int Size) {
	NRF24L01_WriteRegBuf( W_TX_PAYLOAD, Buf, Size);
}

/**
 Read the data reception buffer

 @param Buf Buffer with data received
 @param Size Buffer size

*/
void NRF24L01_Read_RX_Buf(char *Buf, int Size) {
	NRF24L01_ReadRegBuf(R_RX_PAYLOAD, Buf, Size);
}


void NRF24L01_Receive(char *Buf, int len) {
	RADIO_ON;
	ShortDelay (130);

	while ((NRF24L01_Get_Status() & _RX_DR) != _RX_DR);

	RADIO_OFF;

	NRF24L01_Read_RX_Buf(Buf, len);
	NRF24L01_Clear_Interrupts();
}


void NRF24L01_Send(char* Buf, int len) {
	NRF24L01_Write_TX_Buf(Buf, len);

	NRF24L01_RF_TX();

	while ((NRF24L01_Get_Status() & _TX_DS) != _TX_DS);

	NRF24L01_Clear_Interrupts();

}
