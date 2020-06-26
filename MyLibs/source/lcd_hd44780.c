/*
 * lcd_hd44780.c
 *
 *  Created on: Dec 12, 2018
 *      Author: dsolano
 *      Taken from Tim Mayerlee libraries
 */

#include "lcd_hd44780.h"
#include "utilities.h"
#include "define_pins.h"

#if LCD_HD44780_PMP==1
#include <plib.h>
#include <define_pins.h>

// Pre-defined bit masks are located in the PIC32MX PMP peripheral library header file
// pmp.h
#define CONTROL     ( 1<<_PMCON_ON_POSITION | 0<<_PMCON_SIDL_POSITION | 0<<_PMCON_ADRMUX_POSITION  | 1<<_PMCON_PTWREN_POSITION|\
                     1<<_PMCON_CSF_POSITION | 0<<_PMCON_ALP_POSITION  | 0<<_PMCON_CS2P_POSITION    |\
                     0<<_PMCON_WRSP_POSITION  )

#define MODE        ( 0<<_PMMODE_MODE16_POSITION | 3<<_PMMODE_MODE_POSITION |\
                     2<<_PMMODE_WAITB0_POSITION  | 7<<_PMMODE_WAITM_POSITION| 3<<_PMMODE_WAITE_POSITION  )



DEFINE_PIN(LCD_DATA_CMD, G, 14)      // From PAD00
DEFINE_PIN(LCD_ENABLE, D, 12)        // From PAD15 or CS2
DEFINE_PIN(LCD_RW, D, 10)            // From 
DEFINE_PIN(LCD_LED,G,13)             // 

#endif

/* Private HD44780 structure */
typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
#if LCD_HD44780_I2C || LCD_HD44780_I2C_MCP23008
	I2C_MODULE I2Cx;
	uint8_t i2c_addr;
#endif

} HD44780_Options_t;

/* Private functions */
static void lcd_HD44780_InitPins(void);
static void lcd_HD44780_Cmd(uint8_t cmd);
static void lcd_HD44780_Cmd4bit(uint8_t cmd);
static void lcd_HD44780_Data(uint8_t data);
static void lcd_HD44780_CursorSet(uint8_t col, uint8_t row);

#if LCD_HD44780_I2C
static I2C_RESULT lcd_HD44780_i2c_WriteData(uint8_t data, uint8_t len);
#endif

#if LCD_HD44780_I2C_MCP23008
static Status lcd_HD44780_i2c_WriteData(uint8_t mcpreg, uint8_t data, uint8_t len);
static void lcd_HD44780_init_mcp23008(void);
#endif

/* Private variable */
static HD44780_Options_t HD44780_Opts;


/* Commands*/
#define HD44780_CLEARDISPLAY        0x01
#define HD44780_RETURNHOME          0x02
#define HD44780_ENTRYMODESET        0x04
#define HD44780_DISPLAYCONTROL      0x08
#define HD44780_CURSORSHIFT         0x10
#define HD44780_FUNCTIONSET         0x20
#define HD44780_SETCGRAMADDR        0x40
#define HD44780_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define HD44780_ENTRYRIGHT          0x00
#define HD44780_ENTRYLEFT           0x02
#define HD44780_ENTRYSHIFTINCREMENT 0x01
#define HD44780_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define HD44780_DISPLAYON           0x04
#define HD44780_CURSORON            0x02
#define HD44780_BLINKON             0x01

/* Flags for display/cursor shift */
#define HD44780_DISPLAYMOVE         0x08
#define HD44780_CURSORMOVE          0x00
#define HD44780_MOVERIGHT           0x04
#define HD44780_MOVELEFT            0x00

/* Flags for function set */
#define HD44780_8BITMODE            0x10
#define HD44780_4BITMODE            0x00
#define HD44780_2LINE               0x08
#define HD44780_1LINE               0x00
#define HD44780_5x10DOTS            0x04
#define HD44780_5x8DOTS             0x00

/*Print direction of characters - Left to right or right to left*/

#if  LCD_HD44780_I2C_MCP23008
/*
 * Control Bit position
 * MY interface I2C MCP23008
 */
#define BL_BIT_POS		0
#define RS_BIT_POS		1
#define RW_BIT_POS		2
#define E_BIT_POS		3

#endif

#if LCD_HD44780_I2C
/*
 * Control Bit position
 * I2C Interface
 */
#define RS_BIT_POS		0
#define RW_BIT_POS		1
#define E_BIT_POS		2
#define BL_BIT_POS		3

#endif

#if LCD_HD44780_I2C || LCD_HD44780_I2C_MCP23008
/*
 * Control bit definition
 */
#define RS_DATA_BIT		(1<<RS_BIT_POS)
#define RS_CMD_BIT		(0<<RS_BIT_POS)
#define RW_READ_BIT		(1<<RW_BIT_POS)
#define RW_WRITE_BIT	(0<<RW_BIT_POS)
#define E_ENABLE_BIT	(1<<E_BIT_POS)
#define E_DISABLE_BIT	(0<<E_BIT_POS)
#define BL_ON_BIT		(1<<BL_BIT_POS)
#define BL_OFF_BIT		(0<<BL_BIT_POS)

void lcd_HD44780_Init(uint8_t cols, uint8_t rows, I2C_MODULE i2c, uint8_t i2caddr) {

#endif

#if LCD_HD44780_PMP
void lcd_HD44780_Init(uint8_t cols, uint8_t rows) {

#endif

	/* Set LCD width and height */
	HD44780_Opts.Rows = rows;
	HD44780_Opts.Cols = cols;

	/* Set cursor pointer to beginning for LCD */
	HD44780_Opts.currentX = 0;
	HD44780_Opts.currentY = 0;

	/* Init pinout */
	lcd_HD44780_InitPins();

	/* At least 40ms */
	ShortDelay(delay_5ms);


#if LCD_HD44780_I2C || LCD_HD44780_I2C_MCP23008
   	HD44780_Opts.DisplayFunction = HD44780_4BITMODE | HD44780_5x8DOTS | HD44780_1LINE;
	if (rows > 1) {
		HD44780_Opts.DisplayFunction |= HD44780_2LINE;
	}

	HD44780_Opts.I2Cx = i2c;
	HD44780_Opts.i2c_addr = i2caddr;


#if LCD_HD44780_I2C_MCP23008
	lcd_HD44780_init_mcp23008();
#endif


	/* Try to set 4bit mode */
	lcd_HD44780_Cmd4bit(0x30);
	ShortDelay(delay_10ms*4);
	/* Second try */
	lcd_HD44780_Cmd4bit(0x30);
	ShortDelay(delay_10ms*4);
	/* Third goo! */
	lcd_HD44780_Cmd4bit(0x30);
	ShortDelay(delay_10ms*4);
	/* Set 4-bit interface */
	lcd_HD44780_Cmd4bit(0x20);
	ShortDelay(delay_5ms);
#endif

#if LCD_HD44780_PMP
   	HD44780_Opts.DisplayFunction = HD44780_8BITMODE | HD44780_5x8DOTS | HD44780_1LINE;
	if (rows > 1) {
		HD44780_Opts.DisplayFunction |= HD44780_2LINE;
	}

	/* Try to set 8bit mode */
	lcd_HD44780_Cmd(0x30);
	ShortDelay(delay_10ms);
	/* Second try */
	lcd_HD44780_Cmd(0x30);
	ShortDelay(delay_10ms);
	/* Third goo! */
	lcd_HD44780_Cmd(0x30);
	ShortDelay(delay_10ms);
#endif
	/* Set # lines, font size, etc. */
	lcd_HD44780_Cmd(HD44780_FUNCTIONSET | HD44780_Opts.DisplayFunction);

	/* Turn the display on with no cursor or blinking default */
	HD44780_Opts.DisplayControl = HD44780_DISPLAYON;
	lcd_HD44780_DisplayOn();

	/* Clear lcd */
	lcd_HD44780_Clear();

	/* Default font directions */
	HD44780_Opts.DisplayMode = HD44780_ENTRYLEFT | HD44780_ENTRYSHIFTDECREMENT;
	lcd_HD44780_Cmd(HD44780_ENTRYMODESET | HD44780_Opts.DisplayMode);

	/* Delay */
	ShortDelay(delay_5ms);
}

void __dly(int delay){
	int i;
	while (delay--){
		for (i=0;i<1000000;i++);
	}
}

void lcd_HD44780_Clear(void) {
	lcd_HD44780_Cmd(HD44780_CLEARDISPLAY);
	ShortDelay(delay_5ms);
}

void lcd_HD44780_ClearLine(int len, int line)
{
    int i=0;
    for (i=0;i<len;i++)
    {
        lcd_HD44780_putChar(1+i, line, ' ');
    }
}


void lcd_HD44780_Puts(uint8_t x, uint8_t y, char* str) {
	lcd_HD44780_CursorSet(x, y);
	while (*str) {
		if (HD44780_Opts.currentX >= HD44780_Opts.Cols) {
			HD44780_Opts.currentX = 0;
			HD44780_Opts.currentY++;
			lcd_HD44780_CursorSet(HD44780_Opts.currentX, HD44780_Opts.currentY);
		}
		if (*str == '\n') {
			HD44780_Opts.currentY++;
			lcd_HD44780_CursorSet(HD44780_Opts.currentX, HD44780_Opts.currentY);
		} else if (*str == '\r') {
			lcd_HD44780_CursorSet(0, HD44780_Opts.currentY);
		} else {
			lcd_HD44780_Data(*str);
			HD44780_Opts.currentX++;
		}
		str++;
	}
}

void lcd_HD44780_putChar(uint8_t x, uint8_t y, char c){
	lcd_HD44780_CursorSet(x, y);
	if (c == '\n') {
		HD44780_Opts.currentY++;
		lcd_HD44780_CursorSet(HD44780_Opts.currentX, HD44780_Opts.currentY);
	} else if (c == '\r') {
		lcd_HD44780_CursorSet(0, HD44780_Opts.currentY);
	} else {
		lcd_HD44780_Data(c);
		HD44780_Opts.currentX++;
	}
}


void lcd_HD44780_DisplayOn(void) {
	HD44780_Opts.DisplayControl |= HD44780_DISPLAYON;
	lcd_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void lcd_HD44780_DisplayOff(void) {
	HD44780_Opts.DisplayControl &= ~HD44780_DISPLAYON;
	lcd_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void lcd_HD44780_BlinkOn(void) {
	HD44780_Opts.DisplayControl |= HD44780_BLINKON;
	lcd_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void lcd_HD44780_BlinkOff(void) {
	HD44780_Opts.DisplayControl &= ~HD44780_BLINKON;
	lcd_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void lcd_HD44780_CursorOn(void) {
	HD44780_Opts.DisplayControl |= HD44780_CURSORON;
	lcd_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void lcd_HD44780_CursorOff(void) {
	HD44780_Opts.DisplayControl &= ~HD44780_CURSORON;
	lcd_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void lcd_HD44780_ScrollLeft(uint8_t count) {
	uint8_t c = 0;
	for (c=0;c<count;c++){
		lcd_HD44780_Cmd(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVELEFT);
		__dly(5);
	}
}

void lcd_HD44780_ScrollRight(uint8_t count) {
	uint8_t c = 0;
	for (c=0;c<count;c++){
		lcd_HD44780_Cmd(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVERIGHT);
		__dly(5);
	}
}

void lcd_HD44780_CreateChar(uint8_t location, uint8_t *data) {
	uint8_t i;

	/* We have 8 locations available for custom characters */
	location &= 0x07;
	lcd_HD44780_Cmd(HD44780_SETCGRAMADDR | (location << 3));

	for (i = 0; i < 8; i++) {
		lcd_HD44780_Data(data[i]);
	}
}

void lcd_HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location) {
	lcd_HD44780_CursorSet(x, y);
	lcd_HD44780_Data(location);
}

/* Private functions */
static void lcd_HD44780_Cmd(uint8_t cmd) {
#if LCD_HD44780_PMP
	/* Command mode */
	LCD_DATA_CMD_LOW();
    LCD_RW_LOW();   // Write = low
    while (mIsPMPBusy()); // Wait for PMP device come out of busy state
    PMDIN = cmd;
    LCD_ENABLE_LOW();
    ShortDelay(delay_1ms);
    LCD_ENABLE_HIGH();
#endif

#if LCD_HD44780_I2C || LCD_HD44780_I2C_MCP23008
	uint8_t hi_nib, lo_nib;

	hi_nib = (cmd & 0xf0) | BL_ON_BIT | RW_WRITE_BIT | RS_CMD_BIT;
	lcd_HD44780_Cmd4bit(hi_nib);

	lo_nib = ((cmd & 0x0f) << 4) | BL_ON_BIT | RW_WRITE_BIT | RS_CMD_BIT;
	lcd_HD44780_Cmd4bit(lo_nib);

#endif
}

static void lcd_HD44780_Data(uint8_t data) {
#if LCD_HD44780_PMP
	/* Data mode */
	LCD_DATA_CMD_HIGH();    // Data = high
    LCD_RW_LOW();   // Write = low
    while (mIsPMPBusy()); // Wait for PMP device come out of busy state
    PMDIN = data;
    LCD_ENABLE_LOW();
    ShortDelay(delay_1ms);
    LCD_ENABLE_HIGH();
#endif

#if LCD_HD44780_I2C || LCD_HD44780_I2C_MCP23008
	uint8_t hi_nib, lo_nib;

	hi_nib = (data & 0xf0) | BL_ON_BIT | RW_WRITE_BIT | RS_DATA_BIT;
	lcd_HD44780_Cmd4bit(hi_nib);

	lo_nib = ((data & 0x0f) << 4) | BL_ON_BIT | RW_WRITE_BIT | RS_DATA_BIT;
	lcd_HD44780_Cmd4bit(lo_nib);

#endif
}

static void lcd_HD44780_Cmd4bit(uint8_t cmd) {
#if LCD_HD44780_I2C
	ShortDelay(delay_5ms);
	cmd &= ~E_ENABLE_BIT;
	lcd_HD44780_i2c_WriteData(cmd, 1);
	ShortDelay(delay_5ms);

	cmd |= E_ENABLE_BIT;
	lcd_HD44780_i2c_WriteData(cmd, 1);
	ShortDelay(delay_5ms);

	cmd &= ~E_ENABLE_BIT;
	lcd_HD44780_i2c_WriteData(cmd, 1);
	ShortDelay(delay_5ms);
#endif

#if LCD_HD44780_I2C_MCP23008
	_delay_ms(5);
	cmd &= ~E_ENABLE_BIT;
	lcd_HD44780_i2c_WriteData(0x09, cmd, 2);	//GPIO REG
	_delay_ms(5);

	cmd |= E_ENABLE_BIT;
	lcd_HD44780_i2c_WriteData(0X09, cmd, 2);
	_delay_ms(5);

	cmd &= ~E_ENABLE_BIT;
	lcd_HD44780_i2c_WriteData(0X09, cmd, 2);
	_delay_ms(5);
#endif

}

static void lcd_HD44780_CursorSet(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	/* Set col from 1 to ...
	 * Set row from 1 to...
	 * we substract 1 to be compliant with code
	 */
	if (row) {
		row -= 1;
		col -= 1;
	}
	/* Go to beginning */
	if (row >= HD44780_Opts.Rows) {
		row = 0;
	}

	/* Set current column and row */
	HD44780_Opts.currentX = col;
	HD44780_Opts.currentY = row;

	/* Set location address */
	lcd_HD44780_Cmd(HD44780_SETDDRAMADDR | (col + row_offsets[row]));
}

static void lcd_HD44780_InitPins(void) {
#if LCD_HD44780_I2C
	I2CInitDevice(HD44780_Opts.I2Cx, 100000);
#endif
#if LCD_HD44780_I2C_MCP23008
    I2CInitDevice(HD44780_Opts.I2Cx, 400000);
#endif

#if LCD_HD44780_PMP
	/* Init all pins */
	//LCD_BL_OUTPUT();
    PMCON = CONTROL;
    PMMODE = MODE;
    mPMPEnable();
    LCD_RW_OUTPUT();
    LCD_DATA_CMD_OUTPUT();
    LCD_LED_OUTPUT();
    LCD_ENABLE_OUTPUT();
    LCD_LED_HIGH();
#endif
}

#if LCD_HD44780_I2C_MCP23008
static void lcd_HD44780_init_mcp23008(void)
{
	lcd_HD44780_i2c_WriteData(0x00, 0, 2);	//IODIR

	lcd_HD44780_i2c_WriteData(0x01, 0, 2);	//IPOL

	//IOCON 0b00111100
	lcd_HD44780_i2c_WriteData(0x05, 0x3c, 2);
}
#endif


#if LCD_HD44780_I2C || LCD_HD44780_I2C_MCP23008
#if LCD_HD44780_I2C_MCP23008
Status status;
static Status lcd_HD44780_i2c_WriteData(uint8_t mcpreg, uint8_t data, uint8_t len){
#endif
#if LCD_HD44780_I2C
I2C_RESULT status;
static I2C_RESULT lcd_HD44780_i2c_WriteData(uint8_t data, uint8_t len){
#endif

	/* Sets data to be send to MCP23008 to init*/
	I2C_MData_Cfg_sType i2ctx; //Data structure to be used to send byte thru I2C Master Data Transfer
	// Fill Data Structure with proper data
#if LCD_HD44780_I2C_MCP23008
	uint8_t txbuff[4] = {0};
    txbuff[0] = mcpreg;
    txbuff[1] = data;
	i2ctx.ptxBuff = txbuff;
	i2ctx.txDataLen = len;
#endif
	i2ctx.prxBuff = 0;
	i2ctx.rxDataLen = 0;
	i2ctx.sl_addr_7bit = HD44780_Opts.i2c_addr;
#if LCD_HD44780_I2C
	i2ctx.ptxBuff = &data;
	i2ctx.txDataLen = len;
#endif

	// Send data to I2C
	status = I2C_MasterDataTransfer( HD44780_Opts.I2Cx, &i2ctx);

	return status;
}
#endif

/*********************************************************************//**
 * @brief		Puts a decimal number to LCD-I2C
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void lcd_HD44780_PutDec99(uint8_t x, uint8_t y, uint8_t decnum)
{
	char buff[3] = {0};

	uint8_t c1 = decnum % 10;
	uint8_t c2 = (decnum / 10) % 10;
	buff[0] = '0' + c2;
	buff[1] = '0' + c1;
	lcd_HD44780_Puts(x,y, buff);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to LCD-I2C
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void lcd_HD44780_PutDec(uint8_t x, uint8_t y, uint8_t decnum)
{
	char buff[4] = {0};

	uint8_t c1 = decnum % 10;
	uint8_t c2 = (decnum / 10) % 10;
	uint8_t c3 = (decnum / 100) % 10;
	buff[0] = ('0' + c3);
	buff[1] = ('0' + c2);
	buff[2] = ('0' + c1);
	lcd_HD44780_Puts(x,y, buff);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to LCD-I2C
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void lcd_HD44780_PutDec16(uint8_t x, uint8_t y, uint16_t decnum)
{
	char buff[6] = {0};

	uint8_t c1 = decnum % 10;
	uint8_t c2 = (decnum / 10) % 10;
	uint8_t c3 = (decnum / 100) % 10;
	uint8_t c4 = (decnum / 1000) % 10;
	uint8_t c5 = (decnum / 10000) % 10;
	buff[0] = ('0' + c5);
	buff[1] = ('0' + c4);
	buff[2] = ('0' + c3);
	buff[3] = ('0' + c2);
	buff[4] = ('0' + c1);
	lcd_HD44780_Puts(x,y, buff);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to LCD-I2C
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void lcd_HD44780_PutDec32(uint8_t x, uint8_t y, uint32_t decnum)
{
	char buff[11] = {0};

	uint8_t c1 = (decnum % 10);
	uint8_t c2 = (decnum / 10) % 10;
	uint8_t c3 = (decnum / 100) % 10;
	uint8_t c4 = (decnum / 1000) % 10;
	uint8_t c5 = (decnum / 10000) % 10;
	uint8_t c6 = (decnum / 100000) % 10;
	uint8_t c7 = (decnum / 1000000) % 10;
	uint8_t c8 = (decnum / 10000000) % 10;
	uint8_t c9 = (decnum / 100000000) % 10;
	uint8_t c10 = (decnum / 1000000000) % 10;
	buff[0] = ('0' + c10);
	buff[1] = ('0' + c9);
	buff[2] = ('0' + c8);
	buff[3] = ('0' + c7);
	buff[4] = ('0' + c6);
	buff[5] = ('0' + c5);
	buff[6] = ('0' + c4);
	buff[7] = ('0' + c3);
	buff[8] = ('0' + c2);
	buff[9] = ('0' + c1);
	lcd_HD44780_Puts(x,y, buff);
}

/*********************************************************************//**
 * @brief		Puts a hex number to LCD-I2C port without the 0x chars
 * @param[in]	hexnum	Hex number (8-bit long)
 * @return		None
 **********************************************************************/
void lcd_HD44780_PutHex_(uint8_t x, uint8_t y, uint8_t hexnum)
{
	uint8_t nibble, i, idx;
	char buff[3] = {0};
	i = 1;
	idx = 0;
	do
	{
		nibble = (hexnum >> (4 * i)) & 0x0F;
		buff[idx++] = ((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
	lcd_HD44780_Puts(x,y, buff);
}

/*********************************************************************//**
 * @brief		Puts a hex number to LCD-I2C
 * @param[in]	hexnum	Hex number (8-bit long)
 * @return		None
 **********************************************************************/
void lcd_HD44780_PutHex(uint8_t x, uint8_t y, uint8_t hexnum)
{
	uint8_t nibble, i, idx;
	char buff[3] = {0};

	lcd_HD44780_Puts(x, y, "0x");
	x+=2;
	i = 1;
	idx = 0;
	do
	{
		nibble = (hexnum >> (4 * i)) & 0x0F;
		buff[idx++] = ((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
	lcd_HD44780_Puts(x,y, buff);
}

/*********************************************************************//**
 * @brief		Puts a hex number to LCD-I2C
 * @param[in]	hexnum	Hex number (16-bit long)
 * @return		None
 **********************************************************************/
void lcd_HD44780_PutHex16(uint8_t x, uint8_t y, uint16_t hexnum)
{
	uint8_t nibble, i, idx;
	char buff[4] = {0};

	lcd_HD44780_Puts(x,y,"0x");
	x+=2;
	i = 3;
	idx = 0;
	do
	{
		nibble = (hexnum >> (4 * i)) & 0x0F;
		buff[idx++] = ((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
	lcd_HD44780_Puts(x,y, buff);
}

/*********************************************************************//**
 * @brief		Puts a hex number to LCD-I2C
 * @param[in]	hexnum	Hex number (32-bit long)
 * @return		None
 **********************************************************************/
void lcd_HD44780_PutHex32(uint8_t x, uint8_t y, uint32_t hexnum)
{
	uint8_t nibble, i, idx;
	char buff[8] = {0};

	lcd_HD44780_Puts(x,y,"0x");
	x+=2;
	i = 7;
	idx = 0;
	do
	{
		nibble = (hexnum >> (4 * i)) & 0x0F;
		buff[idx++] = ((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
	lcd_HD44780_Puts(x,y, buff);
}
