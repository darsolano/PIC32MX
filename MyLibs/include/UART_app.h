/* 
 * File:   UART_app.h
 * Author: dsolano
 *
 * Created on May 14, 2014, 12:47 AM
 */


#ifndef UART_APP_H
#define	UART_APP_H

#ifdef	__cplusplus
extern "C" {
#endif
#pragma once

#include <xc.h> // PIC 32 COMPILER INCLUDE THAT INTERFACE ALL PIC 32 FUNCTIONS
#include <plib.h>
#include <stdint.h>


#define UDB_UART    UART1
#define BUFFER_SIZE 96
#define UART_STDOUT

// *****************************************************************************
// *****************************************************************************
// Section: System Macros
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Buffer definition for UART
// *****************************************************************************
// *****************************************************************************





// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes for UART
// *****************************************************************************
// *****************************************************************************
void                SendDataBuffer(const char *buffer, UINT32 size);
void                SendString(const char *buffer);
void                Sendchr(UCHAR8 character );
UCHAR8              Getchr(void);
void                UART_Init(void);
void                InitUARTInterrupt(void);
void                prompt(void);
uint8_t*            GetUARTData(void);
BOOL                IsUARTDataReady(void);

void                _mon_putc(char c);

void                prnPutDec99(uint8_t decnum);
void                prnPutDec( uint8_t decnum);
void                prnPutDec16( uint16_t decnum);
void                prnPutDec32( uint32_t decnum);
void                prnPutHex_( uint8_t hexnum);
void                prnPutHex ( uint8_t hexnum);
void                prnPutHex16 ( uint16_t hexnum);
void                prnPutHex32 ( uint32_t hexnum);




#ifdef	__cplusplus
}
#endif

#endif	/* UART_APP_H */

