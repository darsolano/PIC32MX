/*
 * pic32mx360f512l
  EEP24C32.h - library for 24C32 EEPROM from TinyRTC v1.1 by Weiman
 *Created on June 26, 2014, 12:36 AM
 * Author: Darwin O. Solano
 */

#include "EEP24C32.h"



I2C_RESULT status;
UCHAR8 txbuff[32];
UCHAR8 rxbuff[32];

/* I2C Write Data*/
PRIVATE I2C_RESULT I2CWriteData ( UCHAR8* buffer, UCHAR8 len, UCHAR8 addr )
{
    /* Sets data to be send to RTC to init*/
    I2C_MData_Cfg_sType i2ctx; //Data structure to be used to send byte thru I2C Master Data Transfer

    // Fill Data Structure with proper data
    i2ctx.prxBuff = 0;
    i2ctx.rxDataLen = 0;
    i2ctx.sl_addr_7bit = addr;
    i2ctx.ptxBuff = buffer;
    i2ctx.txDataLen = len;
    // Send data to I2C
    status = I2C_MasterDataTransfer ( I2C_24C32_BUS, &i2ctx );
    return status;
}

PRIVATE I2C_RESULT I2CReadData ( UCHAR8* buffer, UCHAR8 len, UCHAR8 addr )
{
    /* Sets data to be send to RTC to init*/
    I2C_MData_Cfg_sType i2crx; //Data structure to be used to send byte thru I2C Master Data Transfer

    // Fill Data Structure with proper data
    i2crx.prxBuff = buffer;
    i2crx.rxDataLen = len;
    i2crx.sl_addr_7bit = addr;
    i2crx.ptxBuff = 0;
    i2crx.txDataLen = 0;
    // Send data to I2C
    status = I2C_MasterDataTransfer ( I2C_24C32_BUS, &i2crx );
    return status;
}

/*
 * Read a single byte from pointed address of EEPROM
 */
UCHAR8 EEPReadByte ( UINT16 address )
{
    UCHAR8 addrl;
    UCHAR8 addrh;

    addrl = address & 0x00ff;
    addrh = ((address & 0xff00) >> 8);

    txbuff[0] = addrh;
    txbuff[1] = addrl;
    status = I2CWriteData ( txbuff, 2, I2C_24C32 );
    ShortDelay ( delay_5ms );
    status = I2CReadData ( rxbuff, 1, I2C_24C32 );
    return (rxbuff[0]);
}

/*
 * Write a single to EEPROM
 */
UCHAR8 EEPWriteByte ( UINT16 addr, UCHAR8 data )
{
    UCHAR8 addrl;
    UCHAR8 addrh;
    addrl = addr & 0x00ff;
    addrh = ((addr & 0xff00) >> 8);

    txbuff[0] = addrh;
    txbuff[1] = addrl;
    txbuff[2] = data;
    status = I2CWriteData ( txbuff, 3, I2C_24C32 );
    ShortDelay ( delay_10ms ); // 1ms
    return (data);
}

/*
 * Write Multiple bytes to EEPROM 1 page + 32 bytes
 */
void EEPWriteString ( UINT16 addr, UCHAR8* buff, UINT8 size )
{

}

/*
 * Read multiple bytes from EEPROM
 */
UCHAR8* EEPReadString ( UINT16 addr, UINT8 size, UCHAR8 *ptr )
{
    return (ptr);
}

void EEPReadPage ( UINT16 page, UCHAR8 *pbuff )
{
    UINT16 fulladdr;
    UINT8 addr;
    UCHAR8 *ptr;

    ptr = pbuff;
    printf ( "\nEEPROM 24C32 Full Page %02X read", page );
    printf ( "\n----------------------------------------------------");
    printf ( "\n     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" );
    printf ( "\n====================================================" );

    fulladdr = page << 5;
    for (addr = 0; addr < 32; addr++) {
        if (!(addr & 0x0F)) printf ( "\n%02X:  ", addr >> 4 );
        *ptr = EEPReadByte ( fulladdr | (addr & 0x1f) );
        printf ( "%02X ", *ptr );
        ptr++;
        ShortDelay ( delay_100us );
    }
    printf ( "\n" );
}

void EEPReadByteLen ( UINT16 address, UINT16 len )
{
    /* Sets data to be send to RTC to init*/
    I2C_MData_Cfg_sType i2ctx; //Data structure to be used to send byte thru I2C Master Data Transfer

    UCHAR8 addrl;
    UCHAR8 addrh;
    UINT16 ctr;
    UCHAR8 page;
    UCHAR8 *ptr;

    page  = (address  & 0x001f) >> 5;
    addrl = address   & 0x00ff;
    addrh = ((address & 0xff00) >> 8);


    printf ( "\nEEPROM 24C32 Full %d bytes on Page:%02X read", len, page );
    printf ( "\n----------------------------------------------------");
    printf ( "\n     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" );
    printf ( "\n====================================================" );

    txbuff[0] = addrh;
    txbuff[1] = addrl;
    rxbuff[0] = 0;

    // Fill Data Structure with proper data
    i2ctx.prxBuff = rxbuff;
    i2ctx.rxDataLen = len;
    i2ctx.sl_addr_7bit = I2C_24C32;
    i2ctx.ptxBuff = txbuff;
    i2ctx.txDataLen = 2;
    // Send data to I2C
    status = I2C_MasterDataTransfer ( I2C_24C32_BUS, &i2ctx );
    ptr = rxbuff;
    for (ctr = 0; ctr < len; ctr++) {
        if (!(ctr & 0x0F)) printf ( "\n%02X:  ", ctr >> 4 );
        printf ( "%02X ", *ptr );
        ptr++;
    }
    printf ( "\n" );
}