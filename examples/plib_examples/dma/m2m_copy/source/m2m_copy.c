/*********************************************************************
 *
 *                  DMA M2M copy example file
 *
 *********************************************************************
 * FileName:        m2m_copy.c
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
int		DmaDoM2MExample(void);


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
 * Output:          0 if the DMA transfer failed,
 * 					1 if the DMA transfers suceeded
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


	// a memory to memory copy
	if(!DmaDoM2MExample())
	{
		return 0;		// if our example failed
	}

	return 1;
}


/*********************************************************************
 * Function:        void DmaDoM2MExample(void)
 *
 * PreCondition:    None
 *
 * Input:			None
 *
 * Output:          1 (true) if the transfer succeeded,
 * 					0 (false) if the memory copy failed
 *
 * Side Effects:    None
 *
 * Overview:		Example of the DMA controller functionality for memory to memory transfers.
 * 					Fill one arbitrary buffer with a pseudo-random data pattern,
 * 					transfer the buffer to another memory area and compare the two buffers.
 *
 * Note:            None.
 ********************************************************************/
int DmaDoM2MExample(void)
{
#define	MIN_RAM_TXFER_SIZE		8		// min size per transfer
#define	MAX_RAM_TXFER_SIZE		250		// less than DmaGetMaxTxferSize() bytes

	unsigned char*	pDmaSrc;
	unsigned char*	pDmaDst;
	unsigned int	txferSize;
	DmaTxferRes		txferRes;
	int				dmaOk=0;
	int				matchOk=0;
	int				allocOk=0;			// operations ok flags
	int				chn=3;				// DMA channel to use for our example

	srand(ReadCoreTimer());		// seed the pseudo random generator

	txferSize=MIN_RAM_TXFER_SIZE+rand()%(MAX_RAM_TXFER_SIZE-MIN_RAM_TXFER_SIZE+1);	// get a random transfer size


	DmaChnOpen(chn, DMA_CHN_PRI2, DMA_OPEN_DEFAULT);		// configure the DMA controller appropriately


	pDmaSrc=(unsigned char*)malloc(txferSize);
	pDmaDst=(unsigned char*)malloc(txferSize);

	if(pDmaSrc && pDmaDst)
	{
		unsigned char*	pS;
		unsigned char*	pD;
		int		ix;

		allocOk=1;

		for(ix=0, pS=pDmaSrc; ix<txferSize; ix++)
		{
			*pS++=rand();		// fill the source buffer
		}


		// program the DMA channel source add, dest add, source and dest size, cell size adjusted
		DmaChnSetTxfer(chn, pDmaSrc, pDmaDst, txferSize, txferSize, txferSize);

		// start the DMA transfer and wait for it to finish
		txferRes=DmaChnStartTxfer(chn, DMA_WAIT_BLOCK, 0);
		if(txferRes==DMA_TXFER_OK)
		{
			dmaOk=1;
			matchOk=1;
			for(ix=0, pS=pDmaSrc, pD=pDmaDst; ix<txferSize; ix++)
			{
				if(*pS++!=*pD++)
				{
					matchOk=0;
					break;
				}
			}
		}
	}

	free(pDmaDst);
	free(pDmaSrc);

	return dmaOk && matchOk && allocOk;

}



