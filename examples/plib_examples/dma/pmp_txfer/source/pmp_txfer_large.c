/*********************************************************************
 *
 *                  DMA PMP transfer example file
 *
 *********************************************************************
 * FileName:        pmp_txfer_large.c
 * Dependencies:	plib.h
 *
 * Processor:       PIC32
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
 * $Id$
 *
 ********************************************************************/


/*********************************************************************
 *
 * This example transfers data to an external device connected to the Explorer16 board using the PIC32MX
 * Enhanced Parallel Master Port.
 * It uses the DMA controller to transfer data from RAM to the PMP port.
 * It is able to transfer buffers larger than DmaGetMaxTxferSize() bytes.
 *
 ********************************************************************/
#include <stdlib.h>

#include <plib.h>        /* PIC32 peripheral library */

#if !defined(_DMAC) || !defined(_PMP)
    #error "This example needs a PIC32MX processor with DMA controller and PMP present. Aborting build!"
#endif  // _DMAC

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


// PMP defines
#define PMP_EXT_ADDR		0x8000		// the external memory device address


#define PMP_CONTROL	(PMP_ON | PMP_MUX_OFF | PMP_READ_WRITE_EN |\
             		PMP_CS2_EN | PMP_CS2_POL_LO | PMP_WRITE_POL_LO | PMP_READ_POL_LO)

#define PMP_MODE        (PMP_IRQ_READ_WRITE | PMP_AUTO_ADDR_INC | PMP_DATA_BUS_16 | PMP_MODE_MASTER2 |\
                     PMP_WAIT_BEG_3 | PMP_WAIT_MID_7 | PMP_WAIT_END_3 )


static unsigned char srcBuff[5000];		// the data buffer to write to the PMP


static unsigned char dstBuff[sizeof(srcBuff)];		// the data buffer to read from the PMP

static unsigned char* volatile	pCurrDmaSrc;		// current DMA source buffer pointer
static unsigned char* volatile	pCurrDmaDst;		// current DMA destination buffer pointer
static volatile int		dmaTxferSz;		// total DMA transfer size
static volatile int		dmaPmpWrite;		// telling the ISR the transfer direction
static volatile int		dmaTxferDone;		// transfer done flag

static int			dmaChn;			// the DMA channel to use



int main(void)
{
	int	ix;
	int	errCnt=0;



	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	// init the pseudo-random generator
	srand(ReadCoreTimer());

	// fill the bufffers with some random data
	for(ix=0; ix<sizeof(srcBuff); ix++)
	{
		srcBuff[ix]=rand();
		dstBuff[ix]=rand();
	}


	// setup the PMP
    	mPMPOpen(PMP_CONTROL, PMP_MODE, PMP_PEN_ALL, PMP_INT_OFF); 

	
    	// setup the external memory device address
    	PMPSetAddress(PMP_EXT_ADDR);

	dmaChn=0;		// set the DMA channel to use

	// Open the desired DMA channel. We use priority 0.
	DmaChnOpen(dmaChn, 0, DMA_OPEN_DEFAULT);

	// set the transfer event control: what event is to start the DMA transfer
	DmaChnSetEventControl(dmaChn, DMA_EV_START_IRQ(_PMP_IRQ));

	// init the variables that the ISR needs
	pCurrDmaSrc=srcBuff;
	pCurrDmaDst=(void*)&PMDIN;
	dmaPmpWrite=1;	// transfer direction

	dmaTxferSz=sizeof(srcBuff);
	dmaTxferDone=0;		// the ISR uses this flag to tell the main application that the job is done


	// set the transfer parameters: source & destination address, source & destination size, number of bytes per event
	DmaChnSetTxfer(dmaChn, pCurrDmaSrc, pCurrDmaDst, dmaTxferSz>DmaGetMaxTxferSize()?DmaGetMaxTxferSize():dmaTxferSz, 2, 2);

	// set the DMA events to generate an interrupt, the interrupt priority and sub-priority. Enable the DMA interrupt.
	DmaChnSetEvEnableFlags(dmaChn, DMA_EV_BLOCK_DONE);
	INTSetVectorPriority(INT_VECTOR_DMA(dmaChn), INT_PRIORITY_LEVEL_5);
	INTSetVectorSubPriority(INT_VECTOR_DMA(dmaChn), INT_SUB_PRIORITY_LEVEL_3);
	INTEnable(INT_SOURCE_DMA(dmaChn), INT_ENABLED);
	
	// globally enable the interrupts
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
	INTEnableInterrupts();

	// once we configured the DMA channel we can enable it
	// now it's ready and waiting for an event to occur...
	DmaChnEnable(dmaChn);
	// force the first transfer, the PMP is quiet
	DmaChnForceTxfer(dmaChn);

	while(!dmaTxferDone)
	{
		// do some other useful work
	}

    	// setup the external memory device address
    	PMPSetAddress(PMP_EXT_ADDR);
    	PMPMasterRead();	// flush the data latches


	pCurrDmaSrc=(void*)&PMDIN;
	pCurrDmaDst=dstBuff;
	dmaPmpWrite=0;
	dmaTxferSz=sizeof(dstBuff);
	dmaTxferDone=0;

	// set the transfer parameters: source & destination address, source & destination size, number of bytes per event
	DmaChnSetTxfer(dmaChn, pCurrDmaSrc, pCurrDmaDst, 2,dmaTxferSz>DmaGetMaxTxferSize()?DmaGetMaxTxferSize():dmaTxferSz, 2);

	DmaChnEnable(dmaChn);
	// force the first transfer, the PMP is quiet
	DmaChnForceTxfer(dmaChn);

	while(!dmaTxferDone)
	{
		// do some other useful work
	}

	for(ix=0; ix<sizeof(srcBuff); ix++)
	{
		if(srcBuff[ix]!=dstBuff[ix])
		{
			errCnt++;
		}
	}


	mPMPClose();	// we're done

	return errCnt;

}


void __ISR(_DMA0_VECTOR, IPL5SOFT) DmaIntHandler(void)
{
	// another block is done
	if(dmaTxferSz<=DmaGetMaxTxferSize())
	{	// we're done, last block was transferred
		dmaTxferDone=1;
	}
	else
	{	// we'll have to start a new transfer
		dmaTxferSz-=DmaGetMaxTxferSize();
		if(dmaPmpWrite)
		{
			pCurrDmaSrc+=DmaGetMaxTxferSize();
			DmaChnSetTxfer(dmaChn, pCurrDmaSrc, pCurrDmaDst, dmaTxferSz>DmaGetMaxTxferSize()?DmaGetMaxTxferSize():dmaTxferSz, 2, 2);
		}
		else
		{
			pCurrDmaDst+=DmaGetMaxTxferSize();
			DmaChnSetTxfer(dmaChn, pCurrDmaSrc, pCurrDmaDst, 2, dmaTxferSz>DmaGetMaxTxferSize()?DmaGetMaxTxferSize():dmaTxferSz, 2);
		}


		DmaChnEnable(dmaChn);
		DmaChnForceTxfer(dmaChn);
	}

	DmaChnClrEvFlags(dmaChn, DMA_EV_BLOCK_DONE);
	INTClearFlag(INT_SOURCE_DMA(dmaChn));
}


