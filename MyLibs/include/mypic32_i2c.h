/* 
 * File:   mypic32_i2c.h
 * Author: dsolano
 * For PIC32MX360F512L
 * Created on August 17, 2014, 6:25 PM
 */

#ifndef MYPIC32_I2C_H
#define	MYPIC32_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <GenericTypeDefs.h>
#include <stdint.h>
#include <peripheral/i2c.h>

//==============================================================================
// I2C
#define SYS_FREQ                    (80000000uL)
#define SYS_CLOCK                   (80000000uL)
#define	GetSystemClock()            (SYS_CLOCK)
#define	GetPeripheralClock()        (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()       (GetSystemClock())
#define PRG_FACTOR                  .000000104
#define I2C_CLOCK_FREQ              400000      // 100KHz, 400KHz or 1MHz
#define I2C_BRG_VALUE               abs(((1/(2*I2C_CLOCK_FREQ))- PRG_FACTOR)*GetPeripheralClock()) - 2

//==============================================================================

    
// *****************************************************************************
// *****************************************************************************
// Section: Constants & Data Types
// *****************************************************************************
// *****************************************************************************





// *****************************************************************************
// I2C Read/Write Bits

/* I2C Read Bit

  Summary:
    Used to indicate that the following data transfer is from slave to master.

  Description:
    This definition can be used to set the read/write direction bit
    (indicating a read) when addressing I2C slave devices.
*/

#define I2C_READ    1


/* I2C Read Bit

  Summary:
    Used to indicate that the following data transfer is from master to slave.

  Description:
    This definition can be used to clear the read/write direction bit
    (indicating a write) when addressing I2C slave devices.
*/

#define I2C_WRITE   0


// *****************************************************************************
// I2C Reserved Slave Addresses

/* I2C General Call Reserved Slave Addresses

  Summary:
    Used to send a long pulse to get the attention of a slow slave device.

  Description:
    The General Call address can be used to broadcast a message to every slave
    device at the same time.
*/

#define I2C_GENERAL_CALL_ADDRESS        0x00


/* I2C "Start Byte" Reserved Slave Addresses

  Summary:
    Used to send a long pulse.

  Description:
    The start byte can be used to send a long pulse to get the attention of a
    slow slave device.
*/

#define I2C_START_BYTE                  0x01


/* I2C 10-Bit Address Reserved Slave Addresses

  Summary:
    Used to form a 10-bit address.

  Description:
    This constant is used by the I2C_INITIALIZE_10_BIT_ADDRESS macro to form
    a 10-bit address so that it can be correctly transmitted and recognized
    as a 2-byte sequence.
*/

#define I2C_10_BIT_ADDRESS_CONST_BITS   0x1E






typedef enum{
    B400KHz = 400000,
    B100KHz = 100000,
    B1MHZ   = 1000000
}I2CSpeed_eType;

typedef enum{
    ACK,
    NACK
}ACK_Type_eType;

// I2CCONRegister must be off bits for idle state
typedef enum{
    START_CONDITION_ENABLE      = 0x0001,
    RESTART_CONDITION_ENABLE    = 0x0002,
    STOP_CONDITION_ENABLE       = 0x0004,
    RECEIVE_CONDITION_ENABLE    = 0x0008,
    ACK_SEQUENCE_IN_PROGRESS    = 0x0010
}BUSY_FLAGS_eType;

typedef struct{
    I2C_MODULE module;
    UCHAR8 sl_addr_7bit;
    UINT8 txCount;
    UCHAR8 *ptxBuff;
    UINT8 txDataLen;
    UINT8 rxCount;
    UINT8 rxDataLen;
    UCHAR8 *prxBuff;
    I2C_RESULT status;
}I2C_MData_Cfg_sType;

#define STATUS_MASK                 I2C_TRANSMITTER_FULL | I2C_DATA_AVAILABLE | I2C_START | I2C_START | I2C_TRANSMITTER_BUSY
#define CONTROL_MASK                START_CONDITION_ENABLE | STOP_CONDITION_ENABLE | RESTART_CONDITION_ENABLE \
                                    | RECEIVE_CONDITION_ENABLE | ACK_SEQUENCE_IN_PROGRESS


I2C_RESULT    I2C_MasterDataTransfer   (I2C_MODULE module, I2C_MData_Cfg_sType* i2cfg);
int           i2c_master_send         (I2C_MODULE module, uint8_t slaveAddr, uint8_t *buff, uint8_t len);
int           i2c_master_cmd_read     (I2C_MODULE module, uint8_t slaveAddr, uint8_t cmd, uint8_t *buff, int len);
int           i2c_master_read         (I2C_MODULE module, uint8_t slaveAddr, uint8_t *buff, int len);
void          I2CInitDevice           (I2C_MODULE module,I2CSpeed_eType rate );
void          i2c_probe_slaves        (I2C_MODULE i2c);
void          i2c_dtect               (I2C_MODULE module);

#ifdef	__cplusplus
}
#endif

#endif	/* MYPIC32_I2C_H */

