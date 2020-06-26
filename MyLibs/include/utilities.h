/* 
 * File:   utilities.h
 * Author: dsolano
 *
 * Created on June 29, 2014, 11:46 AM
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <GenericTypeDefs.h>
#include <plib.h>


// Delay routine
#define CPU_CLOCK_HZ                (80000000UL)                // CPU Clock Speed in Hz
#define CPU_CT_HZ                   (CPU_CLOCK_HZ/2)            // CPU CoreTimer   in Hz
#define PERIPHERAL_CLOCK_HZ         (80000000UL)                // Peripheral Bus  in Hz
#define US_TO_CT_TICKS              (CPU_CT_HZ/1000000UL)       // uS to CoreTimer Ticks
#define DELAY_TIME                  ( US_TO_CT_TICKS )          // 1us delay,  1000000 ticks = 1 second

//==============================================================================
// Constants for Short Delay
#define delay_1us                   1
#define delay_10us                  delay_1us*10
#define delay_50us                  50
#define delay_100us                 delay_10us*10
#define delay_1ms                   (delay_1us*1000)
#define delay_5ms                   delay_1ms*5
#define delay_10ms                  delay_1ms*10
#define delay_100ms                 delay_1ms*100
#define delay_1sec                  delay_1ms*1000
#define delay_half_sec              (delay_1sec/2)
#define delay_qtr_sec               (delay_half_sec/2)
    
#define FREQ_IN_MILLIS              1000
#define FREQ_IN_MICROS              1000000
//==============================================================================

void    ShortDelay      ( UINT32        DelayCount);         // Short Delay
void    start_timer     ( void);
double  read_timer      ( void);
void    SystemClockInitialization(void);
UCHAR8  pic32_SpiXfer   ( SpiChannel channelid, UCHAR8 data ); // Only for SPI Channel 1 & 2
void    pic32_Spi_Init  ( SpiChannel channelid, int rate );   //Init system SPI Channel
uint8_t bcd2bin         (uint8_t bcd_value);
UCHAR8  bin2bcd         (UCHAR8 binary_value);

// Use of Timer2 for timeout clock follow up
void              timer_init(uint32_t rate);
void              timer_start(void);
volatile int      timer_get_counter(void);
volatile int      timer_stop(void);



#ifdef DBGPRINT
    void    DBUGPRINT       (const char *buffstring,    UCHAR8 data);
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* UTILITIES_H */

