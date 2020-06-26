/*********************************************************************
 *
 *                  DMA CRC Calculation example file
 *
 *********************************************************************
 * FileName:        crc_calculation.c
 * Dependencies:	plib.h
 *
 * Processor:       PIC32MX
 *
 * Compiler:        MPLAB XC32 v1 or higher
 *                  MPLAB IDE v8 or higher
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC Microcontroller products.
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * $Id: dma_api_example.c 4261 2007-08-22 16:32:28Z aura $
 *
 ********************************************************************/

#include <stdlib.h>
#include <time.h>

#include <plib.h>

#ifndef _DMAC
    #error "This example needs a PIC32MX processor with DMA controller present. Aborting build!"
#endif  // _DMAC

// prototypes
int		DmaDoCrcExample(void);


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

#define SYS_FREQ (80000000L)

/*********************************************************************
 * Function:        int main(void)
 *
 * PreCondition:    None
 *
 * Input:			None
 *
 * Output:          0 if DMA test failed,
 * 					1 if the DMA test suceeded
 *
 * Side Effects:    None
 *
 * Overview:		Examples for the usage of the DMA Peripheral Lib
 *
 * Note:            None.
 ********************************************************************/
int	main(void)
{

	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	// calculate CRC using the DMA controller
	if(!DmaDoCrcExample())
	{
		return 0;		// if our example failed
	}


	return 1;
}



/*********************************************************************
 * Function:        int DmaDoCrcExample(void)
 *
 * PreCondition:    None
 *
 * Input:			None
 *
 * Output:          0 if some DMA transfer failed,
 * 					1 if the DMA transfers suceeded
 *
 * Side Effects:    None
 *
 * Overview:		Examples for calculating CRC using the DMA Peripheral Lib
 *
 * Note:            None.
 ********************************************************************/
int DmaDoCrcExample(void)
{
	#define			CRC_BUFF_SIZE	200			// the size of the memory area for which to calculate the CRC
									// has to be less than DmaGetMaxTxferSize().
	unsigned char*	romBuff=(unsigned char*)0xbfc00000;		// use the BOOT Flash to calculate its CRC

	unsigned int	crc1, crc2;					// we're going to calculate the CRC with 2 different methods
	int		chn=2;						// DMA channel to use for our example
	DmaTxferRes	res;

/****************************************
 * 1. first we'll calculate the CRC of a flash area
 * ******************************************************/

// we'll use the standard CCITT CRC 16 polynomial: X^16+X^12+X^5+1, hex=0x00011021


	// 1. calculate the CRC of the FLASH area. No DMA transfer occurs.

	// open the channel
	DmaChnOpen(chn, DMA_CHN_PRI2, DMA_OPEN_DEFAULT);

	// set the transfer source and dest addresses, source, dest and cell sizes
	DmaChnSetTxfer(chn, romBuff, &crc1, CRC_BUFF_SIZE, CRC_BUFF_SIZE, CRC_BUFF_SIZE);

	// configure the CRC generator
	DmaCrcConfigure(0x11021, 16, 0xffff);		// initial seed set to 0xffff
	CrcAttachChannel(chn, 1);				// we use append mode, the result is written to the destination address

	res=DmaChnStartTxfer(chn, DMA_WAIT_BLOCK, 0);	// start the DMA calculation and wait it to complete
	if(res!=DMA_TXFER_OK)
	{
		return 0;
	}
	// we have now the CRC available in the crc1 variable and we can use it.

// 2. we now use an easier method, exposed by the DMA API, to obtain the same CRC


	DmaCrcSetSeed(0xffff);		// re-seed the CRC generator to start from exactly the same conditions

	res=DmaChnMemCrc(&crc2, romBuff, CRC_BUFF_SIZE, chn+1, DMA_CHN_PRI2);	// we could use for this example a different DMA channel
	// keep in mind that before using the DmaChnMemCrc() function, the CRC has to be initialized, which we did before
	if(res!=DMA_TXFER_OK)
	{
		return	0;		// failed
	}

	// we can check now that the 2 values obtained are identical
	if(crc1!=crc2)
	{
		return 0;
	}

	// CRC calculation done successfully

	return 1;
}



