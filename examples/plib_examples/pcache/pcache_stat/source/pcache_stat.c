/*********************************************************************
 *
 *      PIC32MX Prefetch Cache Stat Example
 *
 *********************************************************************
 * FileName:        pcache_stat.c
 * Dependencies:	plib.h
 *
 * Processor:       PIC32MX
 *
 * Compiler:        MPLAB XC32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Hardware:		Explorer 6 with PIC32 PIM.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC32MX Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip Microcontroller products.
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
 *********************************************************************
 * $Id: pcache_stat.c 9390 2008-06-16 23:43:04Z rajbhartin $
 *
 * $Name:  $
 *
 * The purpose of this example code is to demonstrate the statistic
 * gathering capability of Prefech Cache and show the effect of
 * prefetch cache.
 *
 ********************************************************************/
#include <plib.h>

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

#define SYS_FREQ 			(80000000L)

// Private helper functions
static char *CvtIntToStr(char *str, int val);
static void WasteTime();
static unsigned long GetClocks();
static unsigned long RunCacheTest();

#define	BAUDRATE	57600	// serial baudrate

int main(void)
{
	unsigned long loopTimeWithoutPrefetch;
	unsigned long loopTimeWithPrefetch;
	unsigned long percentSaving;
	char buf[50];

	char 	choice;			// Stores the menu choice.

	int	pbClk;		// the PB frequency

	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	pbClk = SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	OpenUART2(UART_EN, UART_RX_ENABLE|UART_TX_ENABLE, pbClk/16/BAUDRATE-1); 		// selected baud rate, 8-N-1

	putsUART2("Prefetch Cache Effect Demo.\r\n");
	putsUART2("Frequency: 80MHz\r\n");
	putsUART2("Flash Wait States: 2\r\n");

	putsUART2("\r\n*****************************************************\r\n");
	putsUART2("Running a small loop with Prefetch Cache disabled...\n");

	    // Use the macro provided by the peripheral library
		CheKseg0CacheOff();

		loopTimeWithoutPrefetch = RunCacheTest();

	putsUART2("\r\n*****************************************************\r\n");

	putsUART2("\r\n*****************************************************\r\n");
	putsUART2("Now running the same small loop with Prefetch Cache enabled...\n");

	    // Use the macro provided by the peripheral library
		CheKseg0CacheOn();

		loopTimeWithPrefetch = RunCacheTest();

	putsUART2("\r\n*****************************************************\r\n");

	percentSaving = loopTimeWithoutPrefetch/loopTimeWithPrefetch;

	putsUART2("\r\nConclusion: The small loop executed ");
	putsUART2(CvtIntToStr(buf, percentSaving));
	putsUART2("x faster with Prefetch Cache enabled.\r\n");


	while(1);

	return 0;
}


static unsigned long RunCacheTest()
{

	char buf[20];
	unsigned long start_count;
	unsigned long end_count;
	unsigned long result;
	unsigned long initialMiss;
	unsigned long initialHit;
	unsigned long Misses;
	unsigned long Hits;
	unsigned long hitPercent;

	cheConfigure(0, 0, 1, 6);


// SOLUTION: read initial values of cache hit and miss counters here
	initialHit  = mCheGetHit();
	initialMiss = mCheGetMis();

	start_count = GetClocks();

	WasteTime();

	end_count = GetClocks();
	result = 2*(end_count - start_count);

// SOLUTION: read final values of cache hit and miss counters here
//       subtract differences, calculate hit percentage
	Hits   = mCheGetHit() - initialHit;
	Misses = mCheGetMis() - initialMiss;
	if (Hits + Misses == 0)
		hitPercent = 0;
	else
		hitPercent = (Hits * 100) / (Hits + Misses);

	putsUART2("\r\nTotal CPU cycles to Execute = ");
	putsUART2(CvtIntToStr(buf, result));
	putsUART2(" clocks\r\n");
	putsUART2("Cache Hits = ");
	putsUART2(CvtIntToStr(buf, Hits));
	putsUART2(" Cache Misses = ");
	putsUART2(CvtIntToStr(buf, Misses));
	putsUART2(" Percentage = ");
	putsUART2(CvtIntToStr(buf, hitPercent));
	putsUART2("%\r\n");

	return result;

}

static unsigned long GetClocks()
{
	asm("mfc0 $v0,$9,0");
}


static void WasteTime()
{
	register int i;
	register int j;

	for(i=0; i<100; i++)
	{
		for(j=0; j<100; j++)
		{
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
		}
	}

	for(i=0; i<100; i++)
	{
		for(j=0; j<100; j++)
		{
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
			asm("nop");	asm("nop");	asm("nop");	asm("nop");
		}
	}
}

static char *CvtIntToStr(char *str, int val)
{
	char *s = str;
	char buf[11];
	int n=0;			// number of saved chars

	if (val == 0)
	{
		*s++ = '0';
	}
	else
	{
		if (val < 0)
		{
			*s++ = '-';
			val *= -1;
		}
		while (val)
		{
			buf[n++] = val % 10;
			val = val / 10;
		}
		while (n)
		{
			*s++ = buf[--n] + 0x30;
		}
	}
	*s = 0;
	return str;
}


