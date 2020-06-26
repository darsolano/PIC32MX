/*
 * File:   mypic32_i2c.c
 * Author: dsolano
 * For PIC32MX360F512L
 * Created on August 17, 2014, 6:25 PM
 */

#include "mypic32_i2c.h"
#include "utilities.h"
#include <stdio.h>

//-----------------------------------------------------------------------------------------
//					Local Functions and Variables
//-----------------------------------------------------------------------------------------

PRIVATE I2C_RESULT i2cStart     ( I2C_MODULE module );
PRIVATE I2C_RESULT i2cStop      ( I2C_MODULE module );
PRIVATE BOOL       i2cByteWasAck ( I2C_MODULE module );
PRIVATE I2C_RESULT i2cSendAck   ( I2C_MODULE module, ACK_Type_eType acktype );
PRIVATE I2C_RESULT i2cReStart   ( I2C_MODULE module );
PRIVATE BOOL       i2cCheckIfIdle ( I2C_MODULE module );
PRIVATE I2C_RESULT i2cSendByte  ( I2C_MODULE module, UCHAR8 byte );
PRIVATE I2C_RESULT i2cGetByte   ( I2C_MODULE module, UCHAR8* byte, ACK_Type_eType acktype );
PRIVATE UINT32     i2cSetBaudRate ( I2C_MODULE module, UINT32 pClock, UINT32 i2cClock );
PRIVATE I2C_RESULT i2cEnable    ( I2C_MODULE module, BOOL stat );
PRIVATE I2C_RESULT i2cConfig    ( I2C_MODULE module, I2C_CONFIGURATION config );
//-----------------------------------------------------------------------------------------
//					Local Variables
//-----------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------
//					End of Local Variables
//-----------------------------------------------------------------------------------------

/*
 *
 *
 */
PRIVATE I2C_RESULT i2cStart ( I2C_MODULE module )
{
    while (i2cCheckIfIdle ( module ) == FALSE);
    if (module == I2C1) {
        I2C1CONbits.SEN = 1;
        while (I2C1CONbits.SEN);
    } else if (module == I2C2) {
        I2C2CONbits.SEN = 1;
        while (I2C2CONbits.SEN);
    }
    return I2C_SUCCESS;
}

/*
 *
 *
 */
PRIVATE I2C_RESULT i2cStop ( I2C_MODULE module )
{
    while (i2cCheckIfIdle ( module ) == FALSE);
    if (module == I2C1) {
        I2C1CONbits.PEN = 1;
        while (I2C1CONbits.PEN);
    } else if (module == I2C2) {
        I2C2CONbits.PEN = 1;
        while (I2C2CONbits.PEN);
    }
}

/*
 *
 *
 */
PRIVATE BOOL i2cByteWasAck ( I2C_MODULE module )
{
    if (module == I2C1) {
        if (I2C1STATbits.ACKSTAT == NACK) return FALSE;
        else return TRUE;
    } else if (module == I2C2) {
        if (I2C2STATbits.ACKSTAT == NACK) return FALSE;
        else return TRUE;
    }
}

/*
 *
 *
 */
PRIVATE I2C_RESULT i2cSendAck ( I2C_MODULE module, ACK_Type_eType ack )
{
    if (module == I2C1) {
        if (ack == ACK) I2C1CONbits.ACKDT = ACK;
        else I2C1CONbits.ACKDT = NACK;
        I2C1CONbits.ACKEN = 1;
        while (I2C1CONbits.ACKEN);
    } else if (module == I2C2) {
        if (ack == ACK) I2C2CONbits.ACKDT = ACK;
        else I2C2CONbits.ACKDT = NACK;
        I2C2CONbits.ACKEN = 1;
        while (I2C2CONbits.ACKEN);
    }
    return I2C_SUCCESS;
}

/*
 *
 *
 */
PRIVATE I2C_RESULT i2cReStart ( I2C_MODULE module )
{
    while (!i2cCheckIfIdle ( module ));
    if (module == I2C1) {
        I2C1CONbits.RSEN = 1;
        while (I2C1CONbits.RSEN);
    } else if (module == I2C2) {
        I2C2CONbits.RSEN = 1;
        while (I2C2CONbits.RSEN);
    }
    return I2C_SUCCESS;
}

/*
 *
 *
 */
PRIVATE BOOL i2cCheckIfIdle ( I2C_MODULE module )
{
    if (module == I2C1) {
        if ((I2C1STAT & (STATUS_MASK)) & (I2C1CON & (CONTROL_MASK))) return FALSE;
        else return TRUE;
    } else if (module == I2C2){
        if ((I2C2STAT & (STATUS_MASK)) & (I2C2CON & (CONTROL_MASK))) return FALSE;
        else return TRUE;
    }
    return FALSE;
}

/*Function to check if I2C bus is idle.
 * in	: Module number (I2C1 or I2C2)
 *		: Byte to be sent
 * out	: operational messages, if byte was sent I2C_SUCCESS
 *		  id collision or not acknowledge, a I2C_ERROR return
 */
PRIVATE I2C_RESULT i2cSendByte ( I2C_MODULE module, UCHAR8 byte )
{
    while (!i2cCheckIfIdle ( module ));
    switch (module) {
        case(I2C1):
            I2C1TRN = byte;
            ShortDelay(100);
            while(I2C1STATbits.TBF);
            if (I2C1STATbits.IWCOL) {
                I2C1STATbits.IWCOL = 0;
                return I2C_MASTER_BUS_COLLISION;
            }
            if (i2cByteWasAck ( module )) return I2C_SUCCESS;
            else return I2C_ERROR;
            break;
        case(I2C2):
            I2C2TRN = byte;
            ShortDelay(100);
            while (I2C2STATbits.TBF);
            if (I2C2STATbits.IWCOL) {
                I2C2STATbits.IWCOL = 0;
                return I2C_MASTER_BUS_COLLISION;
            }
            if (i2cByteWasAck ( module )) return I2C_SUCCESS;
            else return I2C_ERROR;
            break;
        default:
            return I2C_ERROR;
            break;
    }
}

PRIVATE I2C_RESULT i2cGetByte ( I2C_MODULE module, UCHAR8* byte, ACK_Type_eType acktype )
{
    I2C_RESULT status;
    while (!i2cCheckIfIdle ( module ));
    switch (module) {
        case (I2C1):
            I2C1CONbits.RCEN = 1;
            while (I2C1CONbits.RCEN);
            if (I2C1STATbits.RBF) {
                ShortDelay(100);
                *byte = I2C1RCV;
                if (I2C1STATbits.I2COV) {
                    I2C1STATbits.I2COV = 0;
                    return I2C_RECEIVE_OVERFLOW;
                }
                i2cSendAck ( module, acktype );
            } else return I2C_ERROR;
            break;
        case (I2C2):
            I2C2CONbits.RCEN = 1;
            while (I2C2CONbits.RCEN);
            if (I2C2STATbits.RBF) {
                ShortDelay(100);
                *byte = I2C2RCV;
                if (I2C1STATbits.I2COV) {
                    I2C1STATbits.I2COV = 0;
                    return I2C_RECEIVE_OVERFLOW;
                }
                i2cSendAck ( module, acktype );
            } else return I2C_ERROR;
            break;
        default:
            break;
    }
}

/*
 *
 * 
 */
PRIVATE UINT32 i2cSetBaudRate ( I2C_MODULE module, UINT32 pClock, UINT32 i2cClock )
{
    if (module == I2C1) {
        I2C1BRG = ((pClock / i2cClock) / 2) - 2;
        return ((pClock / (I2C1BRG + 2)) / 2);
    } else if (module == I2C2) {
        I2C2BRG = ((pClock / i2cClock) / 2) - 2;
        return ((pClock / (I2C2BRG + 2)) / 2);
    }
}


/*********************************************************************//**
 * @brief 		Enable i2c operation
 * @param[in]	I2Cx			I2C peripheral selected, should be:
 * @return 		SUCCESS or ERROR
 *
 * Note:
 * Enable Bit ON in Configuration register
 **********************************************************************/

PRIVATE I2C_RESULT i2cEnable ( I2C_MODULE module, BOOL stat )
{
    if (module == I2C1) {
        if (stat) I2C1CONbits.ON = 1;
        else I2C1CONbits.ON = 0;
    } else if (module == I2C2) {
        if (stat) I2C2CONbits.ON = 1;
        else I2C2CONbits.ON = 0;
    }
    return I2C_SUCCESS;
}

/*
 *
 *
 */
PRIVATE I2C_RESULT i2cConfig ( I2C_MODULE module, I2C_CONFIGURATION config )
{
    if (module == I2C1) {
        if (config && I2C_ENABLE_HIGH_SPEED)
            I2C1CONbits.DISSLW = 0;
        if (config && I2C_ENABLE_SLAVE_CLOCK_STRETCHING)
            I2C1CONbits.STREN = 1;
        if (config && I2C_STOP_IN_IDLE)
            I2C1CONbits.SIDL = 1;
        if (config && I2C_ENABLE_SMB_SUPPORT)
            I2C1CONbits.SMEN = 1;
    } else if (module == I2C2) {
        if (config && I2C_ENABLE_HIGH_SPEED)
            I2C2CONbits.DISSLW = 0;
        if (config && I2C_ENABLE_SLAVE_CLOCK_STRETCHING)
            I2C2CONbits.STREN = 1;
        if (config && I2C_STOP_IN_IDLE)
            I2C2CONbits.SIDL = 1;
        if (config && I2C_ENABLE_SMB_SUPPORT)
            I2C2CONbits.SMEN = 1;
    }
    return I2C_SUCCESS;
}


//-----------------------------------------------------------------------------------------
//					End Of Local Functions and Variables
//-----------------------------------------------------------------------------------------

/*********************************************************************//**
 * @brief 		Transmit and Receive data in master mode
 * @param[in]	I2Cx			I2C peripheral selected, should be:
 * @param[in]	txcfg   		Pointer to a I2C_Tranfer_Cfg_Type structure that
 * 								contains specified information about the
 * 								configuration for master transfer.
 * @return 		SUCCESS or ERROR
 *
 * Note:
 * - In case of using I2C to transmit data only, either transmit length set to 0
 * or transmit data pointer set to NULL.
 * - In case of using I2C to receive data only, either receive length set to 0
 * or receive data pointer set to NULL.
 * - In case of using I2C to transmit followed by receive data, transmit length,
 * transmit data pointer, receive length and receive data pointer should be set
 * corresponding.
 **********************************************************************/

I2C_RESULT I2C_MasterDataTransfer ( I2C_MODULE module, I2C_MData_Cfg_sType *i2ccfg )
{
    UCHAR8 *ptxdat;
    UCHAR8 *prxdat;
    UCHAR8 rxtmp;
    I2C_RESULT status;

    // reset all variables to initial must be states
    i2ccfg->status = I2C_SUCCESS;
    i2ccfg->txCount = 0;
    i2ccfg->rxCount = 0;
    ptxdat = i2ccfg->ptxBuff;
    prxdat = i2ccfg->prxBuff;

    // First pay attention to send Start Command
    // Send Start Command
    status = i2cStart ( module );
    // review start error
    if (status) {
        i2ccfg->status = status;
        goto error;
    }
    // + Slave Address + Bunch of data bytes
    if ((i2ccfg->txDataLen != 0) && (i2ccfg->ptxBuff != NULL)) {
        status = i2cSendByte ( module, i2ccfg->sl_addr_7bit << 1 );
        if (status) {
            i2ccfg->status = status;
            goto error;
        }
        // Send pack of bytes
        while (i2ccfg->txCount < i2ccfg->txDataLen) {
            status = i2cSendByte ( module, *ptxdat );
            if (status) {
                i2ccfg->status = status;
                goto error;
            }
            i2ccfg->txCount++;
            ptxdat++;
        }
    }// End of sending data with Write Enable as a LSB os Slv_Addr

    // After Sending Data we need to check if it is neccesary to get some data from I2C device
    // or no data need to be send just get some from the device
    if ((i2ccfg->rxDataLen != 0) && (i2ccfg->prxBuff != NULL)
            && (i2ccfg->txDataLen != 0) && (i2ccfg->ptxBuff != NULL)) {
        status = i2cReStart ( module );
        if (status) {
            i2ccfg->status = status;
            goto error;
        }
    }

    // If we only need to recieve some data from I2C device, this is the function
    if ((i2ccfg->rxDataLen != 0) && (i2ccfg->prxBuff != NULL)) {
        // Send Slave Address again at restart
        status = i2cSendByte ( module, (i2ccfg->sl_addr_7bit << 1) | I2C_READ );
        if (status) {
            i2ccfg->status = status;
            goto error;
        }
        while (i2ccfg->rxCount < i2ccfg->rxDataLen) {
            if (i2ccfg->rxCount < (i2ccfg->rxDataLen - 1)) { // if data lenth is greater than 1 getbyte and ACK
                status = i2cGetByte ( module, &rxtmp, ACK );
                if (status) {
                    i2ccfg->status = status;
                    goto error;
                }
            } else { // if data is only 1 lenght or the last one to get send a NACK
                status = i2cGetByte ( module, &rxtmp, NACK );
                if (status) {
                    i2ccfg->status = status;
                    goto error;
                }
            }
            *prxdat++ = rxtmp;
            //printf("status: %u data read: %02X ",status, *prxdat);
            i2ccfg->rxCount++;
        }
    }
    i2cStop ( module );
    return (I2C_SUCCESS);

error:
    i2cStop ( module );
    return (I2C_ERROR);
}//End I2C_MasterDataTransfer**********************************************

/* Master tx only */
int i2c_master_send(I2C_MODULE module,  uint8_t slaveAddr,  uint8_t *buff, uint8_t len)
{
	I2C_MData_Cfg_sType xfer = {0};
	xfer.sl_addr_7bit = slaveAddr;
	xfer.ptxBuff = buff;
	xfer.txDataLen = len;
    xfer.status = I2C_MasterDataTransfer(module, &xfer);
	if (xfer.status == I2C_ERROR ) return 1;
	return len - (xfer.txDataLen - xfer.txCount);
}

/* Transmit one byte and receive an array of bytes after a repeated start condition is generated in Master mode.
 * This function is useful for communicating with the I2C slave registers
 */
int i2c_master_cmd_read(I2C_MODULE module, uint8_t slaveAddr, uint8_t cmd, uint8_t *buff, int len)
{
	I2C_MData_Cfg_sType xfer = {0};
	xfer.sl_addr_7bit = slaveAddr;
	xfer.ptxBuff = &cmd;
	xfer.txDataLen = 1;
	xfer.prxBuff = buff;
	xfer.rxDataLen = len;
    xfer.status = I2C_MasterDataTransfer(module, &xfer);
	if (xfer.status == I2C_ERROR) return 1;
	return len - (xfer.rxDataLen - xfer.txCount);
}

/* Sequential master read */
int i2c_master_read(I2C_MODULE module, uint8_t slaveAddr, uint8_t *buff, int len)
{
	I2C_MData_Cfg_sType xfer = {0};
	xfer.sl_addr_7bit = slaveAddr;
	xfer.prxBuff = buff;
	xfer.rxDataLen = len;
    xfer.status = I2C_MasterDataTransfer(module, &xfer);
	if (xfer.status == I2C_ERROR) return 1;
	return len - (xfer.rxDataLen - xfer.txCount);
}

/*
 * Initialize I2C device
 */

void I2CInitDevice ( I2C_MODULE module, I2CSpeed_eType rate )
{
    UINT32 actualClock;
    i2cConfig ( module, I2C_ENABLE_SLAVE_CLOCK_STRETCHING | I2C_ENABLE_HIGH_SPEED );
    // Set the I2C baudrate
    actualClock = i2cSetBaudRate ( module, GetPeripheralClock ( ), rate );
    if (abs ( actualClock - rate ) > rate / 10) {
        printf ( "Error: I2C1 clock frequency (%u) error exceeds 10%%.\n", (unsigned) actualClock );
    }
    i2cEnable ( module, TRUE );
    ShortDelay ( delay_1ms );
}

///* Function that probes all available slaves connected to an I2C bus */
void i2c_probe_slaves ( I2C_MODULE module )
{
    int i;
    printf ( "Probing available I2C devices...\n" );
    printf ( "\n     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" );
    printf ( "\n====================================================" );
    for (i = 0; i <= 0x7F; i++) {
        i2cStart ( module );
        if (!(i & 0x0F)) printf ( "\n%02X  ", i >> 4 );
        if (i <= 7 || i > 0x78) {
            printf ( "   " );
            continue;
        }
        if (i2cSendByte ( module, i << 1 ) == I2C_ERROR) printf ( " --" );
        else printf ( " %02X", i );
        ShortDelay ( delay_1ms );
        i2cStop ( module );
    }
    printf ( "\r\n" );
}
//
//
//
/////=================================
//
void i2c_dtect ( I2C_MODULE module )
{
    UINT8 i;
    I2C_RESULT status;
    UINT8 count = 0;

    printf ( "\nStart:\n" );

    ShortDelay ( 1000 );
//
//    // Try all slave addresses from 0x10 to 0xEF.
//    // See if we get a response from any slaves
//    // that may be on the i2c bus.
    for (i = 0x10; i < 0xF0; i += 2) {
        i2cStart ( module );
        status = i2cSendByte ( module, i );
        if (status == I2C_SUCCESS) {
            printf ( "ACK addr: 0x%02X\n", (i >> 1) );
            count++;
            ShortDelay ( 1000 ); //uSec
        }
        i2cStop ( module );
    }
    if (count == 0)
        printf ( "Nothing Found \n" );
    else
        printf ( "Number of i2c chips found: %u\n", count );
}

