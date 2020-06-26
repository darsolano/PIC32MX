/* 
 * File:   LCDDRV.h
 * Author: dsolano
 *I2C LCD driver for LCD 2004 from Saintsmart. Has I2C module that interface
 * HD44780 LCD CHIP with MCU.
 * Must Send command thru I2C
 * Created on May 9, 2014, 6:33 PM
 */
#pragma once
#ifndef LCDDRV_H
#define	LCDDRV_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <GenericTypeDefs.h>
#include <stdint.h>


// Clock Constants
//#ifndef SYS_CLOCK
//	#error "Define SYS_CLOCK (ex. -DSYS_CLOCK=80000000) on compiler command line"
//#endif

#define LCD_I2C_BUS                 I2C2        // Active I2C

/* INSTRUCTION CODE FOR LCD HD 44780
Instruction Code
                                RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
                                == === === === === === === === === ===
Clear Display                    0  0   0   0   0   0   0   0   0   1
Return Home                      0  0   0   0   0   0   0   0   1   *
Entry Mode Set                   0  0   0   0   0   0   0   1   I/D S
Display ON/OFF                   0  0   0   0   0   0   1   D   C   B
Cursor and Display Shift         0  0   0   0   0   1  S/C R/L  *   *
Function Set                     0  0   0   0   1   DL  N   F   *   *
Set CG RAM address               0  0   0   1   A   A   A   A   A   A
Set DD RAM address               0  0   1   A   A   A   A   A   A   A
Read busy flag and address       0  1   BF  A   A   A   A   A   A   A
Write data to CG or DD RAM       1  0   D   D   D   D   D   D   D   D
Read data from CG or DD RAM      1  1   D   D   D   D   D   D   D   D
 */

#define LCD_I2C_ADDR                0x27            // try 0x3f or 0x27
#define SYS_FREQ                    (80000000uL)
#define SYS_CLOCK                   (80000000uL)
#define	GetSystemClock()            (SYS_CLOCK)
#define	GetPeripheralClock()        (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()       (GetSystemClock())
#define LCD_DELAY                   10


// BASIC INSTRUCTION SET MUST BE ORED WITH OTHER PARAMETERS TO CONFORM A BYTE 8 BITS
#define LCD_CLEARDISPLAY            0x01
#define LCD_RETURNHOME              0x02
#define LCD_ENTRYMODESET            0x04
#define LCD_DISPLAYCONTROL          0x08
#define LCD_CURSOR_DISPLAY_SHIFT    0x10
#define LCD_FUNCTIONSET             0x20
#define LCD_SETCGRAMADDR            0x40
#define LCD_SETDDRAMADDR            0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT              0x00
#define LCD_ENTRYLEFT               0x02
#define LCD_ENTRYSHIFTINCREMENT     0x01
#define LCD_ENTRYSHIFTDECREMENT     0x00

// flags for display on/off control
#define LCD_DISPLAYON               0x04
#define LCD_DISPLAYOFF              0x00
#define LCD_CURSORON                0x02
#define LCD_CURSOROFF               0x00
#define LCD_BLINKON                 0x01
#define LCD_BLINKOFF                0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE_LEFT        0x08
#define LCD_DISPLAYMOVE_RIGHT       0x0C
#define LCD_CURSOR_MOVERIGHT        0x04
#define LCD_CURSOR_MOVELEFT         0x00

// flags for function set
#define LCD_8BITMODE                0x10
#define LCD_4BITMODE                0x00
#define LCD_2LINE                   0x08
#define LCD_1LINE                   0x00
#define LCD_5x10DOTS                0x04
#define LCD_5x8DOTS                 0x00

// flags for backlight control
#define LCD_BACKLIGHT               0x08
#define LCD_NOBACKLIGHT             0x00


// Following line tested for CCS compiler and best goes into main program body
// #use i2c(MASTER, FAST, SCL=PIN_A3, SDA=PIN_A2, FORCE_SW, STREAM=LCDI2C)
// Note: Using PICs with HW I2C capability can increase speed up to 2x over CCS SW I2

// Line addresses for LCDs which use
// the Hitachi HD44780U controller chip
#define LCD2004_LINE_1_ADDRESS      0x00
#define LCD2004_LINE_2_ADDRESS      0x40
#define LCD2004_LINE_3_ADDRESS      0x14
#define LCD2004_LINE_4_ADDRESS      0x54

#define BL                          0b00001000  // LCD Backlight
#define En                          0b00000100  // Enable bit
#define Rw                          0b00000010  // Read/Write bit
#define Rs                          0b00000001  // Register select bit for Data
    
    // Character to display using putc
#define CUST_FONT_FARENHEIT			0x00
#define CUST_FONT_CELSIUS			0x01
#define CUST_FONT_AM    			0x02
#define CUST_FONT_PM    			0x03
#define CUST_FONT_DEGREES			0x04
#define CUST_FONT_BAR   			0x05
#define CUST_FONT_ANTENNA			0x06
#define CUST_FONT_BATTERY			0x07

typedef enum{
    command = 0,
    data = 1
}LCD_RegisterSelect;

typedef union LCDBYTE{
    struct{
        unsigned lcd_RS     :   1;
        unsigned lcd_RW     :   1;
        unsigned lcd_E      :   1;
        unsigned lcd_BL     :   1;
        unsigned lcd_DB4    :   1;
        unsigned lcd_DB5    :   1;
        unsigned lcd_DB6    :   1;
        unsigned lcd_DB7    :   1;
    }lcd_bits_t;
   struct{
        UCHAR8 lcd_byte;
    }lcd_byte_t;
}LCD_DATA_Type;



void StopTransfer( void );
BOOL TransmitOneByte( UINT8 data );
BOOL StartTransfer( BOOL restart, UCHAR8 address );
void LCDI2C_Init(void);
void LCDI2C_Write(UCHAR8 data , UCHAR8 RS);
void LCDI2C_SetCursorPos(UINT8 x, UINT8 y);
void LCDI2C_SetDisplayOnOff(BOOL);
void LCDI2C_SetCursorBlink(BOOL);
void LCDI2C_SetCursorOnOff(BOOL);
void LCDI2C_SetDisplayClear();
void LCDI2C_SetCursorHome();
void LCDI2C_SetChrDisplayBlink();
void LCDI2C_SetCursorShift();
void LCDI2C_SetDisplayShift();
void LCDI2C_Printrs(const char * buffer);
void LCDI2C_Prints(char * buffer);
void LCDI2C_Putc(UCHAR8 c);
void LCDI2C_ClearLine(int len, int line);

void LCDI2C_PutDec99(uint8_t decnum);
void LCDI2C_PutDec(uint8_t decnum);
void LCDI2C_PutDec16(uint16_t decnum);
void LCDI2C_PutDec32(uint32_t decnum);
void LCDI2C_PutHex_(uint8_t hexnum);
void LCDI2C_PutHex(uint8_t hexnum);
void LCDI2C_PutHex16(uint16_t hexnum);
void LCDI2C_PutHex32(uint32_t hexnum);

void LCDI2C_InitHD44780CustomChars(void);
void LCDI2C_DefineSpecialChars(const char* spclchar, uint8_t charpos);

#ifdef	__cplusplus
}
#endif

#endif	/* LCDDRV_H */

