/* 
 * File:   SDCARD_SPI_H
 * Author: dsolano
 *
 * Created on June 27, 2014, 8:42 PM
 */
/**************************************************************************//**
 * @file     sd.h
 * @brief    Header file for sd.c
 * @version  1.0
 * @date     18. Nov. 2010
 *
 * @note
 * Copyright (C) 2010 NXP Semiconductors(NXP), ChaN. All rights reserved.
 *
 * General SD driver (SD_xxxx()): NXP
 * SD card initilization flow and some code snippets are inspired from ChaN.
 *
 ******************************************************************************/

#ifndef SDCARD_SPI_H
#define	SDCARD_SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <GenericTypeDefs.h>
#include "utilities.h"

    //==============================================================================
    // SPI
#define FREQ_PBCLK                  80000000
#define FREQ_SPICHN                 10000000 // 10MHz
#define FREQ_SPICHN_LOW             400000  //  400KHz
#define SPI_BRG                     (FREQ_PBCLK/(2 *FREQ_SPICHN)) - 1
#define SDCARD_CHN                  SPI_CHANNEL1


    /* type definition */
    typedef unsigned char SD_BOOL;
#define SD_TRUE     1
#define SD_FALSE    0

    /* Command definitions in SPI bus mode */
#define GO_IDLE_STATE           0
#define SEND_OP_COND            1
#define SWITCH_FUNC             6
#define SEND_IF_COND            8
#define SEND_CSD                9
#define SEND_CID                10
#define STOP_TRANSMISSION       12
#define SEND_STATUS             13
#define SET_BLOCKLEN            16
#define READ_SINGLE_BLOCK       17
#define READ_MULTIPLE_BLOCK     18
#define WRITE_SINGLE_BLOCK      24
#define WRITE_MULTIPLE_BLOCK    25
#define APP_CMD                 55
#define READ_OCR                58
#define CRC_ON_OFF              59

    /* Application specific commands supported by SD.
    All these commands shall be preceded with APP_CMD (CMD55). */
#define SD_STATUS               13
#define SD_SEND_OP_COND         41

    /* R1 response bit flag definition */
#define R1_NO_ERROR         0x00
#define R1_IN_IDLE_STATE    0x01
#define R1_ERASE_RESET      0x02
#define R1_ILLEGAL_CMD      0x04
#define R1_COM_CRC_ERROR    0x08
#define R1_ERASE_SEQ_ERROR  0x10
#define R1_ADDRESS_ERROR    0x20
#define R1_PARA_ERROR       0x40
#define R1_MASK             0x7F

    /* The sector size is fixed to 512bytes in most applications. */
#define SECTOR_SIZE 512

    /* Memory card type definitions */
#define CARDTYPE_UNKNOWN        0
#define CARDTYPE_MMC            1   /* MMC */
#define CARDTYPE_SDV1           2   /* V1.x Standard Capacity SD card */
#define CARDTYPE_SDV2_SC        3   /* V2.0 or later Standard Capacity SD card */
#define CARDTYPE_SDV2_HC        4   /* V2.0 or later High/eXtended Capacity SD card */

    /* SD/MMC card configuration */
    typedef struct tagCARDCONFIG {
        UINT32 sectorsize; /* size (in byte) of each sector, fixed to 512bytes */
        UINT32 sectorcnt; /* total sector number */
        UINT32 blocksize; /* erase block size in unit of sector */
        UINT8 ocr[4]; /* OCR */
        UINT8 cid[16]; /* CID */
        UINT8 csd[16]; /* CSD */
    } CARDCONFIG_sType;

    /* Public variables */
    extern UINT8 CardType;
    extern CARDCONFIG_sType CardConfig;


    /* Public functions */
    SD_BOOL SD_Init(void);
    SD_BOOL SD_ReadSector(UINT32 sect, UINT8 *buf, UINT32 cnt);
    SD_BOOL SD_WriteSector(UINT32 sect, const UINT8 *buf, UINT32 cnt);
    SD_BOOL SD_ReadConfiguration(void);
    void    disk_timerproc(void);

    /* --------------------------------- End Of File ------------------------------ */


#ifdef	__cplusplus
}
#endif

#endif	/* SDCARD_SPI_H */

