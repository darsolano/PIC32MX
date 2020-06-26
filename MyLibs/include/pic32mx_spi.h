/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <plib.h>
/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

// SPIxCON: SPI Control Register
#define     SPIxCON_FRMENABLE   (1<<31)     //Framed SPI support is enabled (SSx pin used as FSYNC input/output)
#define     SPIxCON_FRMSYNC     (1<<30)     //Frame Sync Pulse Direction Control on SSx pin bit (Framed SPI mode only)
#define     SPIxCON_FRMPOL      (1<<29)     //Frame Sync Polarity bit (Framed SPI mode only)
#define     SPIxCON_MSSEN       (1<<28)     //Frame Sync Polarity bit (Framed SPI mode only)
#define     SPIxCON_FRMSYPW     (1<<27)     //Frame Sync Polarity bit (Framed SPI mode only)
/**/
#define     SPIxCON_FRMCNT1      (0<<24)     //Generate a frame sync pulse on every data character
#define     SPIxCON_FRMCNT2      (1<<24)     //Generate a frame sync pulse on every 2 data characters
#define     SPIxCON_FRMCNT4      (2<<24)     //Generate a frame sync pulse on every 4 data characters
#define     SPIxCON_FRMCNT8      (3<<24)     //Generate a frame sync pulse on every 8 data characters
#define     SPIxCON_FRMCNT16     (4<<24)     //Generate a frame sync pulse on every 16 data characters
#define     SPIxCON_FRMCNT32     (5<<24)     //Generate a frame sync pulse on every 32 data characters
/**/
#define     SPIxCON_MCLKSEL_MCLK (1<<23)     //Master Clock Select bit(2) ,MCLK is used by the Baud Rate Generator
#define     SPIxCON_MCLKSEL_PBCLK (1<<23)    //Frame Sync Polarity bit (Framed SPI mode only)
#define     SPIxCON_MASTER_MODE_ENABLE  (1<<5)     //Master Mode Enable bit
#define     SPIxCON_FRMPOL      (1<<29)     //Frame Sync Polarity bit (Framed SPI mode only)
#define     SPIxCON_FRMPOL      (1<<29)     //Frame Sync Polarity bit (Framed SPI mode only)
#define     SPIxCON_FRMPOL      (1<<29)     //Frame Sync Polarity bit (Framed SPI mode only)
#define     SPIxCON_FRMPOL      (1<<29)     //Frame Sync Polarity bit (Framed SPI mode only)
#define     SPIxCON_FRMPOL      (1<<29)     //Frame Sync Polarity bit (Framed SPI mode only)
#define     SPIxCON_FRMPOL      (1<<29)     //Frame Sync Polarity bit (Framed SPI mode only)



#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
