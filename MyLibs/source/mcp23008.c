/*
 * File:   mcp23008.h
 * Author: dsolano
 * For PIC32MX360F512L
 * Created on August 17, 2014, 6:07 PM
 */

#include "mcp23008.h"
#include "mypic32_i2c.h"

#define MCP23008_I2C_BUS            I2C1

//*******************************************************************************
//				START OF LOCAL FUNCTIONS AND VARIABLES
//*******************************************************************************

PRIVATE UCHAR8 txbuff[4];
PRIVATE UCHAR8 rxbuff[4];
PRIVATE I2C_RESULT status;

//*******************************************************************************
//				END OF LOCAL FUNCTIONS AND VARIABLES
//*******************************************************************************

//*******************************************************************************
//				START OF PUBLIC FUNCTIONS AND VARIABLES
//*******************************************************************************

void mcp23008_init(void)
{
    I2CInitDevice(MCP23008_I2C_BUS, B1MHZ);
}

uint8_t mcp23008_WriteGPIO(UCHAR8 data, UCHAR8 addr)
{
    txbuff[0] = GPIO;
    txbuff[1] = data;
    status = i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);
    return status;
}

UCHAR8 mcp23008_ReadGPIO(UCHAR8 addr)
{
    i2c_master_cmd_read(MCP23008_I2C_BUS, addr, GPIO, rxbuff, 1);
    return rxbuff[0];
}

uint8_t mcp23008_SetGPIODir(uint8_t iodircfg, UCHAR8 addr)
{
    txbuff[0] = IODIR;
    txbuff[1] = iodircfg;
    i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);
    return status;
}

uint8_t mcp23008_SetGPIOPullUps(UCHAR8 pullup, UCHAR8 addr)
{
    txbuff[0] = GPPU;
    txbuff[1] = pullup;

    i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);
    return status;

}

void mcp23008_SetDEFVAL(UCHAR8 defval, UCHAR8 addr)
{
    txbuff[0] = DEFVAL;
    txbuff[1] = defval;
    i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);

}
uint8_t mcp23008_SetGPIOInt(UCHAR8 config, UCHAR8 defval, UCHAR8 addr)
{
    txbuff[0] = _INTCON;
    txbuff[1] = config;
    i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);

    txbuff[0] = DEFVAL;
    txbuff[1] = defval;
    i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);

    txbuff[0] = GPINTEN;
    txbuff[1] = config;
    i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);

    return status;
}

UCHAR8 mcp23008_ReadINTCAP(UCHAR8 addr)
{
    i2c_master_cmd_read(MCP23008_I2C_BUS, addr, INTCAP, rxbuff, 1);
    return rxbuff[0];
}

UCHAR8 mcp23008_ReadINTF(UCHAR8 addr)
{
    i2c_master_cmd_read(MCP23008_I2C_BUS, addr, INTF, rxbuff, 1);
    return rxbuff[0];
}

uint8_t mcp23008_SetIPOL(UCHAR8 config, UCHAR8 addr)
{ // Polarity = 1 is inverted if 0 output = 1 and viceversa
    txbuff[0] = IPOL;
    txbuff[1] = config; // bitwise for set up all bits from 0 to 7

    status = i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);
    return status;

}

uint8_t mcp23008_ConfigIOCON(MCP23008_IOCON_REG_sType *iocon, uint8_t addr)
{
    txbuff[0] = IOCON;
    txbuff[1] = iocon->IOCON_Reg;
    status = i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);
    return status;
}

uint8_t mcp23008_ReadOLAT(uint8_t addr)
{
    i2c_master_cmd_read(MCP23008_I2C_BUS, addr, OLAT, rxbuff, 1);
    return rxbuff[0];
}

// Modify the output pins directly
uint8_t mcp23008_WriteOLAT(uint8_t data, uint8_t addr)
{
    txbuff[0] = OLAT;
    txbuff[1] = data;

    return i2c_master_send(MCP23008_I2C_BUS,addr,txbuff,2);
}
//*******************************************************************************
//				END OF PUBLIC FUNCTIONS AND VARIABLES
//*******************************************************************************
