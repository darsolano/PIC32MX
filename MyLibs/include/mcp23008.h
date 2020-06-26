/* 
 * File:   mcp23008.h
 * Author: dsolano
 * For PIC32MX360F512L
 * Created on August 17, 2014, 6:07 PM
 */

#ifndef MCP23008_H
#define	MCP23008_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <GenericTypeDefs.h>
#include <stdint.h>

#define MCP23008_LCD_ADDR           0x20    // For LCD1602
#define MCP23008_KP_ADDR            0x21    // For Keypad 4x4


    //Address Access to MCP23008 Internal registers
#define IODIR                      0x00 // Used
#define IPOL                       0x01 // Used
#define GPINTEN                    0x02 // Used
#define DEFVAL                     0x03 // Used
#define _INTCON                     0x04 // Used
#define IOCON                      0x05 // Used
#define GPPU                       0x06 // Used
#define INTF                       0x07 // Used
#define INTCAP                     0x08  //(Read-only)
#define GPIO                       0x09 // Used
#define OLAT                       0x0A // Used


    // IOCON – I/O EXPANDER CONFIGURATION REGISTER (ADDR 0x05)

    typedef union {

        struct {
            unsigned : 1;
            unsigned INTPOL : 1;
            unsigned ODR : 1;
            unsigned HAEN : 1;
            unsigned DISSLW : 1;
            unsigned SEQOP : 1;
            unsigned : 2;
        };
        UCHAR8 IOCON_Reg;
    } MCP23008_IOCON_REG_sType;



    uint8_t mcp23008_WriteGPIO(UCHAR8 data, UCHAR8 addr);
    UCHAR8  mcp23008_ReadGPIO(UCHAR8 addr);
    uint8_t mcp23008_SetGPIODir(uint8_t iodircfg, UCHAR8 addr);
    uint8_t mcp23008_SetGPIOPullUps(UCHAR8 pullup, UCHAR8 addr);
    uint8_t mcp23008_SetGPIOInt ( UCHAR8 config, UCHAR8 defval, UCHAR8 addr );
    UCHAR8  mcp23008_ReadINTCAP(UCHAR8 addr);
    UCHAR8  mcp23008_ReadINTF(UCHAR8 addr);
    uint8_t mcp23008_SetIPOL(UCHAR8 config, UCHAR8 addr);
    uint8_t mcp23008_ConfigIOCON(MCP23008_IOCON_REG_sType *iocon, uint8_t addr);
    uint8_t mcp23008_ReadOLAT ( uint8_t addr );
    uint8_t mcp23008_WriteOLAT ( uint8_t data, uint8_t addr );
    void    mcp23008_init(void);
    void    mcp23008_SetDEFVAL(UCHAR8 defval, UCHAR8 addr);

#ifdef	__cplusplus
}
#endif

#endif	/* MCP23008_H */

