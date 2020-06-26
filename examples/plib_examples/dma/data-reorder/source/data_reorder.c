/*********************************************************************
 *
 *                  DMA data-reordering and checksum calculation example file
 *
 *********************************************************************
 * FileName:        data_reorder.c
 * Dependencies:    plib.h
 *                  GenericTypeDefs.h
 *                  crc.h
 *
 * Processor:       PIC32 5xx/6xx/7xx
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
 * This example transfers data and calculates various checksums using the DMA Special Function Module (SFM)
 * re-ordering capabilities.
 * It is able to transfer buffers no larger than DmaGetMaxTxferSize() bytes.
 * The example is meant to run on PIC32 5xx, 6xx and 7xx family of processors.
 *
 ********************************************************************/

#include <stdlib.h>

#include <GenericTypeDefs.h>
#include <plib.h>


#ifndef _DMAC
    #error "This example needs a PIC32MX processor with DMA controller present. Aborting build!"
#endif  // _DMAC


#include "crc.h"

#if (((__PIC32_FEATURE_SET__ < 500) || (__PIC32_FEATURE_SET__ > 799)))
	#error "This example has to run on PIC32 5xx/6xx/7xx processors. Aborting build!"
#endif


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1


#define	SYS_CLOCK	80000000L	// system clock, Hz

// examples
BOOL		DmaM2MReorderEx(void);
BOOL		DmaCrcAppendEx(void);
BOOL		DmaChksumAppendEx(void);


// helpers
void		DmaSwapBuffers(unsigned char* dstBuff, unsigned char* srcBuff, int buffSize, DmaReorderMode rMode);
int		DmaCompareBuffers(unsigned char* srcBuff, unsigned char* dstBuff, int buffSize);
unsigned short	ChkSumBuffer(unsigned char* srcBuff, int buffSize);
void		MirrorBufferBytes(unsigned char* dstBuff, unsigned char* srcBuff, int buffSize);


// externs

void		Le2BeWords(unsigned char* dst, unsigned char* src, int size);
void		SwapHalves(unsigned char* dst, unsigned char* src, int size);
void		Le2BeHalves(unsigned char* dst, unsigned char* src, int size);



// test data/selection
// modify these selections at run time to obtain different example processing

DmaChannel	exChn=DMA_CHANNEL0;			// the DMA channel we use in our example. 
DmaChannelPri	exChnPri=DMA_CHN_PRI3;			// the relative priority to use (against other DMA channels)


BOOL		exDataReorderEn=TRUE;			// if data re-ordering to occur


DmaReorderMode	exReorderMode=DMA_REORDER_ENDIAN;	// re-ordering type:
							// 0(DMA_REORDER_NOT) - no endianes change
							// 1(DMA_REORDER_ENDIAN) - LE<->BE
							// 2(DMA_REORDER_SWAP_HALF) - swap halves within the word
							// 3(DMA_REORDER_SWAP_BYTE) - swap bytes within the halves

DmaBitOrder	exBitOrder=DMA_BITO_MSb;		// the bit order to be fed into the checksum block:
							// 0(DMA_BITO_MSb) - MSb first (not reflected)
							// 1(DMA_BITO_LSb) - LSb first (reflected)

DmaChksumType	exChkType=DMA_CHKSUM_CRC;		// 0(DMA_CHKSUM_CRC) - CRC mode
							// 1(DMA_CHKSUM_IP) - IP Checksum mode


int		exTxferSize=1024;			// bytes to transfer/calculate: 4 multiple
int		exSrcOffs=0;				// offset into source buffer: 0-3
int		exDstOffs=0;				// offset into destination buffer: 0-3



unsigned int	exPolynomial=0x04c11db7;		// the CRC polynomial to use
int		exPLen=32;				// polynomial length
int		exSeed=0xffffffff;			// initial seed



int main(void)
{
	BOOL	res;

	
	SYSTEMConfigPerformance(SYS_CLOCK);

	srand(ReadCoreTimer());	

	exTxferSize&=0xfffffffc;	// always word multiple
	exSrcOffs%=4;
	exDstOffs%=4;


	res=DmaM2MReorderEx();

	res&=DmaCrcAppendEx();

	res&=DmaChksumAppendEx();
	
	
	return res;


}

/*********************************************************************
 * Function:        BOOL DmaM2MReorderEx(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          TRUE if success,
 *                  FALSE otherwise
 *
 * Side Effects:    None
 *
 * Overview:        Example for re-ordered data transfers using the DMA SFM capabilities.
 *
 * Note:            None.
 ********************************************************************/
BOOL DmaM2MReorderEx(void)
{
	unsigned char	*pSrcBuff, *pDstBuff, *pRDstBuff;
	DmaTxferRes	txRes;
	int		srcOffs, dstOffs;
	BOOL		success=FALSE;


	pSrcBuff=(unsigned char*)malloc(exTxferSize+4);
	pDstBuff=(unsigned char*)malloc(exTxferSize+4);
	pRDstBuff=(unsigned char*)malloc(exTxferSize+4);		// accomodate offsets
	
	if(pSrcBuff && pDstBuff && pRDstBuff)
	{
		int		ix;
		unsigned char	*pS, *pD;
		
		// fill the buffers
		pS=pSrcBuff;
		pD=pDstBuff;
		for(ix=0; ix<exTxferSize+4; ix++)
		{
			*pS++=rand();
			*pD++=rand();
		}
		
		DmaChnOpen(exChn, exChnPri, DMA_OPEN_DEFAULT);


		// configure the CRC generator
		DmaSfmCrcConfigure(exPolynomial, exPLen, exSeed);
		// configure the SFM block		
		DmaSfmConfigure(exChkType, exBitOrder, exReorderMode);
		DmaSfmTxferReorder(exDataReorderEn);
		
		DmaSfmAttachChannel(exChn, 0);	// we want background mode, to actually transfer data
		
		// Note that un-aligned re-ordered transfers are not supported!
		if(exDataReorderEn || exReorderMode!=DMA_REORDER_NOT)
		{
			srcOffs=dstOffs=0;
		}
		else
		{
			srcOffs=exSrcOffs;
			dstOffs=exDstOffs;
		}

		DmaChnSetTxfer(exChn, pSrcBuff+srcOffs, pDstBuff+dstOffs, exTxferSize, exTxferSize, exTxferSize);

		txRes=DmaChnStartTxfer(exChn, DMA_WAIT_BLOCK, 0);

		if(txRes==DMA_TXFER_OK)
		{
			BOOL	cmpRes;
			
			DmaSwapBuffers(pRDstBuff+dstOffs, pDstBuff+dstOffs, exTxferSize, exReorderMode);	// swap in place
			// compare the destination to see that the data was transferred correctly
			if(exDataReorderEn)
			{
				cmpRes=DmaCompareBuffers(pSrcBuff+srcOffs, pRDstBuff+dstOffs, exTxferSize);	// destination buffer is re-ordered
			}
			else
			{
				cmpRes=DmaCompareBuffers(pSrcBuff+srcOffs, pDstBuff+dstOffs, exTxferSize);	// destination buffer is not re-ordered
			}
			
			if(cmpRes)
			{	// ok, transfer went on ok
				// now verifying that the checksum was calculated corectly
				if(exBitOrder!=DMA_BITO_MSb)
				{
					MirrorBufferBytes(pDstBuff+dstOffs, pDstBuff+dstOffs, exTxferSize);
					MirrorBufferBytes(pRDstBuff+dstOffs, pRDstBuff+dstOffs, exTxferSize);						
				}

				if(exChkType)
				{	// calculating checksums
					unsigned int chkHw, chkSwDst, chkSwRDst;
					chkSwRDst=ChkSumBuffer(pRDstBuff+dstOffs, exTxferSize);
					chkSwDst=ChkSumBuffer(pDstBuff+dstOffs, exTxferSize);
					chkHw=DmaSfmChecksum();
					success=exDataReorderEn?(chkHw==chkSwDst):(chkHw==chkSwRDst);
				}
				else
				{	// CRC's
					unsigned int crcHw, crcSwDst, crcSwRDst;

					Crc32Init(exPolynomial, exPLen, exSeed);
					Crc32Add(pDstBuff+dstOffs, exTxferSize);
					crcSwDst=Crc32Result();

					Crc32Init(exPolynomial, exPLen, exSeed);
					Crc32Add(pRDstBuff+dstOffs, exTxferSize);
					crcSwRDst=Crc32Result(); 
					crcHw=DmaSfmChecksum();
					success=exDataReorderEn?(crcHw==crcSwDst):(crcHw==crcSwRDst);
				}
			}
		}

	}

	free(pRDstBuff);
	free(pDstBuff);
	free(pSrcBuff);

	return success;
	
}


/*********************************************************************
 * Function:        BOOL DmaCrcAppendEx(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          TRUE if success,
 *                  FALSE otherwise
 *
 * Side Effects:    None
 *
 * Overview:        Example for re-ordered CRC calculation in append mode using the DMA SFM capabilities.
 *
 * Note:            None.
 ********************************************************************/
BOOL DmaCrcAppendEx(void)
{
	unsigned char*	romBuff=(unsigned char*)0xbfc00000;		// use the BOOT Flash to calculate its CRC
	unsigned char	*pDstBuff;
	unsigned int	crc1, crc2;					// we're going to calculate the CRC with 2 different methods
	DmaTxferRes	txRes;
	BOOL		success=FALSE;


	pDstBuff=(unsigned char*)malloc(exTxferSize);
	
	if(pDstBuff)
	{
		// open the channel
		DmaChnOpen(exChn, exChnPri, DMA_OPEN_DEFAULT);

		// set the transfer source and dest addresses, source, dest and cell sizes
		// Note that we'll be using the append mode so un-aligned transfers are not supported!
		DmaChnSetTxfer(exChn, romBuff, &crc1, exTxferSize, exTxferSize, exTxferSize);

		// configure the SFM-CRC generator
		DmaSfmConfigure(DMA_CHKSUM_CRC, exBitOrder, exReorderMode);
		DmaSfmCrcConfigure(exPolynomial, exPLen, exSeed);
		// DmaSfmTxferReorder(exDataReorderEn);		// we don't configure the re-ordering;
								// it's irrelevant since the append mode will disable it (re-ordering in append mode is illegal)!
		DmaSfmAttachChannel(exChn, 1);			// we use append mode, the result is written to the destination address
		
		txRes=DmaChnStartTxfer(exChn, DMA_WAIT_BLOCK, 0);	// start the DMA calculation and wait it to complete
		if(txRes==DMA_TXFER_OK)
		{
			// check the CRC in the hardware register
			crc2=DmaSfmChecksum();
			if(crc1==crc2)
			{
				// we have now the CRC available in the crc1 variable and we can use it.
				// check it out using the software
				//
				unsigned int crcSwDst;

				DmaSwapBuffers(pDstBuff, romBuff, exTxferSize, exReorderMode);	// swap the source to pDst so that we can calculate the buffer's CRC
				if(exBitOrder!=DMA_BITO_MSb)
				{
					MirrorBufferBytes(pDstBuff, pDstBuff, exTxferSize);
				}

				Crc32Init(exPolynomial, exPLen, exSeed);
				Crc32Add(pDstBuff, exTxferSize);
				crcSwDst=Crc32Result();
				success=(crc1==crcSwDst);
			}
		}
	}
		
	free(pDstBuff);
	
	
	return success;
}

/*********************************************************************
 * Function:        BOOL DmaChksumAppendEx(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          TRUE if success,
 *                  FALSE otherwise
 *
 * Side Effects:    None
 *
 * Overview:        Example for re-ordered IP checksum calculation in append mode using the DMA SFM capabilities.
 *
 * Note:            None.
 ********************************************************************/
BOOL DmaChksumAppendEx(void)
{
	unsigned char*	romBuff=(unsigned char*)0xbfc00000;		// use the BOOT Flash to calculate its CRC
	unsigned char	*pDstBuff;
	unsigned int	chk1, chk2;					// we're going to calculate the CRC with 2 different methods
	DmaTxferRes	txRes;
	BOOL		success=FALSE;


	pDstBuff=(unsigned char*)malloc(exTxferSize);
	
	if(pDstBuff)
	{
		// open the channel
		DmaChnOpen(exChn, exChnPri, DMA_OPEN_DEFAULT);

		// set the transfer source and dest addresses, source, dest and cell sizes
		// Note that we'll be using the append mode so un-aligned transfers are not supported!
		DmaChnSetTxfer(exChn, romBuff, &chk1, exTxferSize, exTxferSize, exTxferSize);

		// configure the SFM-checksum generator
		DmaSfmConfigure(DMA_CHKSUM_IP, exBitOrder, exReorderMode);
		DmaSfmSetSeed(exSeed);
		// DmaSfmTxferReorder(exDataReorderEn);		// we don't configure the re-ordering;
								// it's irrelevant since the append mode will disable it (re-ordering in append mode is illegal)!
		DmaSfmAttachChannel(exChn, 1);			// we use append mode, the result is written to the destination address

		txRes=DmaChnStartTxfer(exChn, DMA_WAIT_BLOCK, 0);	// start the DMA calculation and wait it to complete
		if(txRes==DMA_TXFER_OK)
		{
			chk2=DmaSfmChecksum();
			if(chk1==chk2)
			{
				// we have now the IP checksum available in the chk1 variable and we can use it.
				// check it out using the software
				//
				unsigned int chkSwDst;

				DmaSwapBuffers(pDstBuff, romBuff, exTxferSize, exReorderMode);	// swap the source to pDst so that we can calculate the buffer's checksum
				if(exBitOrder!=DMA_BITO_MSb)
				{
					MirrorBufferBytes(pDstBuff, pDstBuff, exTxferSize);
				}

				chkSwDst=ChkSumBuffer(pDstBuff, exTxferSize);
				success=(chk1==chkSwDst);
			}
		}
	}

	free(pDstBuff);
	
	return success;
}


/***********************************
 * 		helper routines
 *************************************************/

// swap endianity of a buffer
// can be in place, dst==src
void DmaSwapBuffers(unsigned char* dstBuff, unsigned char* srcBuff, int buffSize, DmaReorderMode rMode)
{
	int	notProcessed=0;
	
	switch(rMode)
	{
		case DMA_REORDER_ENDIAN:
		
			Le2BeWords(dstBuff, srcBuff, buffSize);
			break;
			
		case DMA_REORDER_SWAP_HALF:
			SwapHalves(dstBuff, srcBuff, buffSize);
			break;
		
		case DMA_REORDER_SWAP_BYTE:
			Le2BeHalves(dstBuff, srcBuff, buffSize);
			break;

		default:
			notProcessed=1;
			break;
	}

	if(notProcessed && dstBuff!=srcBuff)
	{
		memcpy(dstBuff, srcBuff, buffSize);	// copy src to dest
	}
	
}





int DmaCompareBuffers(unsigned char* srcBuff, unsigned char* dstBuff, int buffSize)
{
	int	ix;
	
	for(ix=0; ix<buffSize; ix++)
	{
		if(*dstBuff++!=*srcBuff++)
		{
			return 0;
		}
	}

	return 1;
}


unsigned short ChkSumBuffer(unsigned char* srcBuff, int buffSize)
{
	unsigned short	s;
	unsigned long	chkSum=0;
	unsigned long	hiSum;
	int		ix;


	for(ix=0; ix<buffSize/2; ix++)
	{
		s=*srcBuff++;
		s|=(*srcBuff++)<<8;
		chkSum+=s;
	}	
	
	hiSum=chkSum>>16;
	chkSum&=0xffff;
	chkSum+=hiSum;

	return ~chkSum;
	
}


// mirrors each byte in the src buffer and stores it to the dest buffer
// dst can be = to the src
void MirrorBufferBytes(unsigned char* dstBuff, unsigned char* srcBuff, int buffSize)
{
	int		ix, jx;
	unsigned char	src, dst, lsb;
	
	for(ix=0; ix<buffSize; ix++)
	{
		src=*srcBuff++;
		dst=0;
		for(jx=0; jx<8; jx++)
		{
			lsb=src&1;
			src>>=1;
			dst<<=1;
			dst|=lsb;
		}
		*dstBuff++=dst;
	}	
}

