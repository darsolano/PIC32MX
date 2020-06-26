/* SD Library for SD Breakout Board from LCSOFT.net LC STUDIO
 * File:   SDCARD_SPI.C
 * Author: dsolano
 *
 * Created on June 27, 2014, 8:42 PM
 */
#include "sdcard_spi.h"
#include "wireless.h"
#include <plib.h>
#include <stdbool.h>


/* Global variables */
UINT8 CardType; /* card type */
CARDCONFIG_sType CardConfig; /* Card configuration */

/* Local variables */
static volatile UINT32 Timer1, Timer2; /* 100Hz decrement timer stopped at zero (disk_timerproc()) */

/** Select the card */
#define SD_Select()  do {LATDbits.LATD12 = 0;} while (0)
/** DeSelect the card */
#define SD_DeSelect() do { LATDbits.LATD12 = 1;SpiIO (SDCARD_CHN,0xff);} while (0)

/* Local functions */
void SpiInit ( SpiChannel channel );
SD_BOOL SD_Init ( void );
SD_BOOL SD_ReadSector ( UINT32 sect, UINT8 *buf, UINT32 cnt );
SD_BOOL SD_WriteSector ( UINT32 sect, const UINT8 *buf, UINT32 cnt );
SD_BOOL SD_ReadConfiguration ( void );
UINT8   SD_SendCommand ( UINT8 cmd, UINT32 arg, UINT8 *buf, UINT32 len );
UINT8   SD_SendACommand ( UINT8 cmd, UINT32 arg, UINT8 *buf, UINT32 len );
SD_BOOL SD_RecvDataBlock ( UINT8 *buf, UINT32 len );
SD_BOOL SD_SendDataBlock ( const UINT8 *buf, UINT8 tkn, UINT32 len );
SD_BOOL SD_WaitForReady ( void );
UCHAR8  SpiIO ( SpiChannel channel, UCHAR8 data );

/*-----------------------------------------------------------------------*/
/* Device timer function  (Platform dependent)                           */
/*-----------------------------------------------------------------------*/

/* This function must be called from timer interrupt routine in period
/  of 10 ms to generate card control timing.
 */
void disk_timerproc ( void )
{
    UINT32 n;

    n = Timer1; /* 100Hz decrement timer stopped at 0 */
    if (n) Timer1 = --n;
    n = Timer2;
    if (n) Timer2 = --n;
}

/**
 * @brief  Initializes the memory card.
 *
 * @param  None
 * @retval SD_TRUE: Init OK.
 *         SD_FALSE: Init failed.
 *
 * Note: Refer to the init flow at http://elm-chan.org/docs/mmc/sdinit.png
 */
SD_BOOL SD_Init ( void )
{
    UINT32 i;
    UINT8 r1, buf[4];

    /* Set card type to unknown */
    CardType = CARDTYPE_UNKNOWN;

    /* Init SPI interface for SD Card*/
    SpiInit ( SDCARD_CHN );

    /* Before reset, Send at least 74 clocks at low frequency
    (between 100kHz and 400kHz) with CS high and DI (MISO) high. */
    SD_DeSelect ( );
    SpiChnSetBitRate(SDCARD_CHN,FREQ_PBCLK,FREQ_SPICHN_LOW);
    for (i = 0; i < 10; i++) SpiIO ( SDCARD_CHN, 0xFF );

    /* Send CMD0 with CS low to enter SPI mode and reset the card.
    The card will enter SPI mode if CS is low during the reception of CMD0.
    Since the CMD0 (and CMD8) must be sent as a native command, the CRC field
    must have a valid value. */
    if (SD_SendCommand ( GO_IDLE_STATE, 0, NULL, 0 ) != R1_IN_IDLE_STATE) // CMD0
    {
        goto init_end;
    }

    /* Now the card enters IDLE state. */

    /* Card type identification Start ... */

    /* Check the card type, needs around 1000ms */
    r1 = SD_SendCommand ( SEND_IF_COND, 0x1AA, buf, 4 ); // CMD8
    if (r1 & 0x80) goto init_end;

    ShortDelay ( delay_1sec ); // 1000ms
    if (r1 == R1_IN_IDLE_STATE) { /* It's V2.0 or later SD card */
        if (buf[2] != 0x01 || buf[3] != 0xAA) goto init_end;

        /* The card is SD V2 and can work at voltage range of 2.7 to 3.6V */

        do {
            r1 = SD_SendACommand ( SD_SEND_OP_COND, 0x40000000, NULL, 0 ); // ACMD41
            if (r1 == 0x00) break;
            else if (r1 > 0x01) goto init_end;
        } while (Timer1);

        if (Timer1 && SD_SendCommand ( READ_OCR, 0, buf, 4 ) == R1_NO_ERROR) // CMD58
            CardType = (buf[0] & 0x40) ? CARDTYPE_SDV2_HC : CARDTYPE_SDV2_SC;

    } else { /* It's Ver1.x SD card or MMC card */

        /* Check if it is SD card */
        if (SD_SendCommand ( APP_CMD, 0, NULL, 0 ) & R1_ILLEGAL_CMD) {
            CardType = CARDTYPE_MMC;
            while (Timer1 && SD_SendCommand ( SEND_OP_COND, 0, NULL, 0 ));
        } else {
            CardType = CARDTYPE_SDV1;
            while (Timer1 && SD_SendACommand ( SD_SEND_OP_COND, 0, NULL, 0 ));
        }

        if (Timer1 == 0) CardType = CARDTYPE_UNKNOWN;
    }

    /* For SDHC or SDXC, block length is fixed to 512 bytes, for others,
    the block length is set to 512 manually. */
    if (CardType == CARDTYPE_MMC ||
            CardType == CARDTYPE_SDV1 ||
            CardType == CARDTYPE_SDV2_SC) {
        if (SD_SendCommand ( SET_BLOCKLEN, SECTOR_SIZE, NULL, 0 ) != R1_NO_ERROR)
            CardType = CARDTYPE_UNKNOWN;
    }

init_end:
    SD_DeSelect ( );

    if (CardType == CARDTYPE_UNKNOWN) {
        return (SD_FALSE);
    } else /* Init OK. use high speed during data transaction stage. */ {
        SpiChnSetBitRate(SDCARD_CHN,FREQ_PBCLK,FREQ_SPICHN);
        return (SD_TRUE);
    }
}

/**
 * @brief  Wait for the card is ready.
 *
 * @param  None
 * @retval SD_TRUE: Card is ready for read commands.
 *         SD_FALSE: Card is not ready
 */
SD_BOOL SD_WaitForReady ( void )
{
    ShortDelay ( delay_half_sec ); // 500ms
    SpiIO ( SDCARD_CHN, 0xff ); /* Read a byte (Force enable DO output) */
    do {
        if (SpiIO ( SDCARD_CHN, 0xff ) == 0xFF) return SD_TRUE;
    } while (Timer2);

    return SD_FALSE;
}

/**
 * @brief  Send a command and receive a response with specified format.
 *
 * @param  cmd: Specifies the command index.
 * @param  arg: Specifies the argument.
 * @param  buf: Pointer to byte array to store the response content.
 * @param  len: Specifies the byte number to be received after R1 response.
 * @retval Value below 0x80 is the normal R1 response (0x0 means no error)
 *         Value above 0x80 is the additional returned status code.
 *             0x81: Card is not ready
 *             0x82: command response time out error
 */
UINT8 SD_SendCommand ( UINT8 cmd, UINT32 arg, UINT8 *buf, UINT32 len )
{
    UINT32 r1, i;
    UINT8 crc_stop;

    /* The CS signal must be kept low during a transaction */
    SD_Select ( );

    /* Wait until the card is ready to read (DI signal is High) */
    if (SD_WaitForReady ( ) == SD_FALSE) return 0x81;

    /* Prepare CRC7 + stop bit. For cmd GO_IDLE_STATE and SEND_IF_COND,
    the CRC7 should be valid, otherwise, the CRC7 will be ignored. */
    if (cmd == GO_IDLE_STATE) crc_stop = 0x95; /* valid CRC7 + stop bit */
    else if (cmd == SEND_IF_COND) crc_stop = 0x87; /* valid CRC7 + stop bit */
    else crc_stop = 0x01; /* dummy CRC7 + Stop bit */

    /* Send 6-byte command with CRC. */
    SpiIO ( SDCARD_CHN, cmd | 0x40 );
    SpiIO ( SDCARD_CHN, arg >> 24 );
    SpiIO ( SDCARD_CHN, arg >> 16 );
    SpiIO ( SDCARD_CHN, arg >> 8 );
    SpiIO ( SDCARD_CHN, arg );
    SpiIO ( SDCARD_CHN, crc_stop ); /* Valid or dummy CRC plus stop bit */


    /* The command response time (Ncr) is 0 to 8 bytes for SDC,
    1 to 8 bytes for MMC. */
    for (i = 8; i; i--) {
        r1 = SpiIO ( SDCARD_CHN, 0xff );
        if (r1 != 0xFF) break; /* received valid response */
    }
    if (i == 0) return (0x82); /* command response time out error */

    /* Read remaining bytes after R1 response */
    if (buf && len) {
        do {
            *buf++ = SpiIO ( SDCARD_CHN, 0xff );
        } while (--len);
    }

    return (r1);
}

/**
 * @brief  Send an application specific command for SD card
 *         and receive a response with specified format.
 *
 * @param  cmd: Specifies the command index.
 * @param  arg: Specifies the argument.
 * @param  buf: Pointer to byte array to store the response content.
 * @param  len: Specifies the byte number to be received after R1 response.
 * @retval Value below 0x80 is the normal R1 response(0x0 means no error)
 *         Value above 0x80 is the additional returned status code.
 *             0x81: Card is not ready
 *             0x82: command response time out error
 *
 * Note: All the application specific commands should be precdeded with APP_CMD
 */
UINT8 SD_SendACommand ( UINT8 cmd, UINT32 arg, UINT8 *buf, UINT32 len )
{
    UINT8 r1;

    /* Send APP_CMD (CMD55) first */
    r1 = SD_SendCommand ( APP_CMD, 0, NULL, 0 );
    if (r1 > 1) return r1;

    return (SD_SendCommand ( cmd, arg, buf, len ));
}

/**
 * @brief  Read single or multiple sector(s) from memory card.
 *
 * @param  sect: Specifies the starting sector index to read
 * @param  buf:  Pointer to byte array to store the data
 * @param  cnt:  Specifies the count of sectors to read
 * @retval SD_TRUE or SD_FALSE.
 */
SD_BOOL SD_ReadSector ( UINT32 sect, UINT8 *buf, UINT32 cnt )
{
    SD_BOOL flag;

    /* Convert sector-based address to byte-based address for non SDHC */
    if (CardType != CARDTYPE_SDV2_HC) sect <<= 9;

    flag = SD_FALSE;

    if (cnt > 1) /* Read multiple block */ {
        if (SD_SendCommand ( READ_MULTIPLE_BLOCK, sect, NULL, 0 ) == R1_NO_ERROR) {
            do {
                if (SD_RecvDataBlock ( buf, SECTOR_SIZE ) == SD_FALSE) break;
                buf += SECTOR_SIZE;
            } while (--cnt);

            /* Stop transmission */
            SD_SendCommand ( STOP_TRANSMISSION, 0, NULL, 0 );

            /* Wait for the card is ready */
            if (SD_WaitForReady ( ) && cnt == 0) flag = SD_TRUE;
        }
    } else /* Read single block */ {
        if ((SD_SendCommand ( READ_SINGLE_BLOCK, sect, NULL, 0 ) == R1_NO_ERROR) &&
                SD_RecvDataBlock ( buf, SECTOR_SIZE ) == SD_TRUE)
            flag = SD_TRUE;
    }

    /* De-select the card */
    SD_DeSelect ( );

    return (flag);
}

/**
 * @brief  Write single or multiple sectors to SD/MMC.
 *
 * @param  sect: Specifies the starting sector index to write
 * @param  buf: Pointer to the data array to be written
 * @param  cnt: Specifies the number sectors to be written
 * @retval SD_TRUE or SD_FALSE
 */
SD_BOOL SD_WriteSector ( UINT32 sect, const UINT8 *buf, UINT32 cnt )
{
    SD_BOOL flag;

    /* Convert sector-based address to byte-based address for non SDHC */
    if (CardType != CARDTYPE_SDV2_HC) sect <<= 9;

    flag = SD_FALSE;
    if (cnt > 1) /* write multiple block */ {
        if (SD_SendCommand ( WRITE_MULTIPLE_BLOCK, sect, NULL, 0 ) == R1_NO_ERROR) {
            do {
                if (SD_SendDataBlock ( buf, 0xFC, SECTOR_SIZE ) == SD_FALSE) break;
                buf += SECTOR_SIZE;
            } while (--cnt);

            /* Send Stop Transmission Token. */
            SpiIO ( SDCARD_CHN, 0xFD );

            /* Wait for complete */
            if (SD_WaitForReady ( ) && cnt == 0) flag = SD_TRUE;
        }
    } else /* write single block */ {

        if ((SD_SendCommand ( WRITE_SINGLE_BLOCK, sect, NULL, 0 ) == R1_NO_ERROR) &&
                (SD_SendDataBlock ( buf, 0xFE, SECTOR_SIZE ) == SD_TRUE))
            flag = SD_TRUE;
    }

    /* De-select the card */
    SD_DeSelect ( );

    return (flag);
}

/**
 * @brief  Read card configuration and fill structure CardConfig.
 *
 * @param  None
 * @retval SD_TRUE or SD_FALSE.
 */
SD_BOOL SD_ReadConfiguration ( )
{
    UINT8 buf[16];
    UINT32 i, c_size, c_size_mult, read_bl_len;
    SD_BOOL retv;

    retv = SD_FALSE;

    /* Read OCR */
    if (SD_SendCommand ( READ_OCR, 0, CardConfig.ocr, 4 ) != R1_NO_ERROR) goto end;

    /* Read CID */
    if ((SD_SendCommand ( SEND_CID, 0, NULL, 0 ) != R1_NO_ERROR) ||
            SD_RecvDataBlock ( CardConfig.cid, 16 ) == SD_FALSE) goto end;

    /* Read CSD */
    if ((SD_SendCommand ( SEND_CSD, 0, NULL, 0 ) != R1_NO_ERROR) ||
            SD_RecvDataBlock ( CardConfig.csd, 16 ) == SD_FALSE) goto end;

    /* sector size */
    CardConfig.sectorsize = 512;

    /* sector count */
    if (((CardConfig.csd[0] >> 6) & 0x3) == 0x1) /* CSD V2.0 (for High/eXtended Capacity) */ {
        /* Read C_SIZE */
        c_size = (((UINT32) CardConfig.csd[7] << 16) + ((UINT32) CardConfig.csd[8] << 8) + CardConfig.csd[9]) & 0x3FFFFF;
        /* Calculate sector count */
        CardConfig.sectorcnt = (c_size + 1) * 1024;

    } else /* CSD V1.0 (for Standard Capacity) */ {
        /* C_SIZE */
        c_size = (((UINT32) (CardConfig.csd[6]&0x3) << 10) + ((UINT32) CardConfig.csd[7] << 2) + (CardConfig.csd[8] >> 6)) & 0xFFF;
        /* C_SIZE_MUTE */
        c_size_mult = ((CardConfig.csd[9]&0x3) << 1) + ((CardConfig.csd[10]&0x80) >> 7);
        /* READ_BL_LEN */
        read_bl_len = CardConfig.csd[5] & 0xF;
        /* sector count = BLOCKNR*BLOCK_LEN/512, we manually set SECTOR_SIZE to 512*/
        //CardConfig.sectorcnt = (c_size+1)*(1<<read_bl_len) * (1<<(c_size_mult+2)) / 512;
        CardConfig.sectorcnt = (c_size + 1) << (read_bl_len + c_size_mult - 7);
    }

    /* Get erase block size in unit of sector */
    switch (CardType) {
        case CARDTYPE_SDV2_SC:
        case CARDTYPE_SDV2_HC:
            if ((SD_SendACommand ( SD_STATUS, 0, buf, 1 ) != R1_NO_ERROR) ||
                    SD_RecvDataBlock ( buf, 16 ) == SD_FALSE) goto end; /* Read partial block */
            for (i = 64 - 16; i; i--) SpiIO ( SDCARD_CHN, 0 ); /* Purge trailing data */
            CardConfig.blocksize = 16UL << (buf[10] >> 4); /* Calculate block size based on AU size */
            break;
        case CARDTYPE_MMC:
            CardConfig.blocksize = ((UINT16) ((CardConfig.csd[10] & 124) >> 2) + 1) * (((CardConfig.csd[10] & 3) << 3) + ((CardConfig.csd[11] & 224) >> 5) + 1);
            break;
        case CARDTYPE_SDV1:
            CardConfig.blocksize = (((CardConfig.csd[10] & 63) << 1) + ((UINT16) (CardConfig.csd[11] & 128) >> 7) + 1) << ((CardConfig.csd[13] >> 6) - 1);
            break;
        default:
            goto end;
    }

    retv = SD_TRUE;
end:
    SD_DeSelect ( );

    return retv;
}

/**
 * @brief  Receive a data block with specified length from SD/MMC.
 *
 * @param  buf: Pointer to the data array to store the received data
 * @param  len: Specifies the length (in byte) to be received.
 *              The value should be a multiple of 4.
 * @retval SD_TRUE or SD_FALSE
 */
SD_BOOL SD_RecvDataBlock ( UINT8 *buf, UINT32 len )
{
    UINT8 datatoken;
    UINT32 i;

    /* Read data token (0xFE) */
    Timer1 = 10; /* Data Read Timerout: 100ms */
    do {
        datatoken = SpiIO ( SDCARD_CHN, 0xff );
        if (datatoken == 0xFE) break;
    } while (Timer1);
    if (datatoken != 0xFE) return (SD_FALSE); /* data read timeout */

    /* Read data block */
    for (i = 0; i < len; i++) {
        buf[i] = SpiIO ( SDCARD_CHN, 0xff );
    }

    /* 2 bytes CRC will be discarded. */
    SpiIO ( SDCARD_CHN, 0xff );
    SpiIO ( SDCARD_CHN, 0xff );

    return (SD_TRUE);
}

/**
 * @brief  Send a data block with specified length to SD/MMC.
 *
 * @param  buf: Pointer to the data array to store the received data
 * @param  tkn: Specifies the token to send before the data block
 * @param  len: Specifies the length (in byte) to send.
 *              The value should be 512 for memory card.
 * @retval SD_TRUE or SD_FALSE
 */
SD_BOOL SD_SendDataBlock ( const UINT8 *buf, UINT8 tkn, UINT32 len )
{
    UINT8 recv;
    UINT32 i;

    /* Send Start Block Token */
    SpiIO ( SDCARD_CHN, tkn );

    /* Send data block */
    for (i = 0; i < len; i++) {
        SpiIO ( SDCARD_CHN, buf[i] );
    }

    /* Send 2 bytes dummy CRC */
    SpiIO ( SDCARD_CHN, 0xFF );
    SpiIO ( SDCARD_CHN, 0xFF );

    /* Read data response to check if the data block has been accepted. */
    if (((SpiIO ( SDCARD_CHN, 0 )) & 0x0F) != 0x05)
        return (SD_FALSE); /* write error */

    /* Wait for wirte complete. */
    Timer1 = 20; // 200ms
    do {
        recv = SpiIO ( SDCARD_CHN, 0xff );
        if (recv == 0xFF) break;
    } while (Timer1);

    if (recv == 0xFF) return SD_TRUE; /* write complete */
    else return (SD_FALSE); /* write time out */

}

/* --------------------------------- End Of File ------------------------------ */


// ---------------------------------------------------------------------------
// This one send and receive data from SPI on the selected channel
// ---------------------------------------------------------------------------

UCHAR8 SpiIO ( SpiChannel channel, UCHAR8 data ) // Only for SPI Channel 1 & 2
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
} // END SpiIO()

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
void SpiInit ( SpiChannel channel )
{
    TRISDbits.TRISD12 = 0; // CS to SD Card
    // Set IOs directions for EEPROM SPI
    SpiChnEnable(channel,false);
    SpiChnOpen(channel,SPI_OPEN_MSTEN | SPI_OPEN_MODE8 , FREQ_PBCLK/FREQ_SPICHN);
    SpiChnEnable(channel,true);
}
