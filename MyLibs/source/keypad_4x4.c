/*
 * File:   keypad_4x4.h
 * Author: Darwin O. Solano
 * PIC32MC360F512L
 * Created on June 28, 2014, 11:43 AM
 */
#include <utilities.h>
#include <keypad_4x4.h>
#include <mypic32_i2c.h>
#include <define_pins.h>
#include <stdint.h>


#define NO_KEY  0x20
//-----------------------------------------------------------------------------------------
//					Local Functions and Variables
//-----------------------------------------------------------------------------------------
PRIVATE uint8_t kp_KeyScan(void);
uint8_t status;


//-----------------------------------------------------------------------------------------
//					Local Variables
//-----------------------------------------------------------------------------------------
uint8_t kp_LastPressed;
uint8_t KeyCode;;
PRIVATE UCHAR8 txbuff[4];
PRIVATE UCHAR8 rxbuff[4];
uint8_t buf[11];

//-----------------------------------------------------------------------------------------
//					END of Local Variables
//-----------------------------------------------------------------------------------------

PRIVATE uint8_t kp_KeyScan(void)
{
    uint8_t Col = 0; // Lower 4 Bits
    uint8_t Row = 0; // Upper 4 Bits
    uint8_t debounce = 0;
    
    mcp23008_WriteGPIO(0xf0,MCP23008_KP_ADDR);
    
    for(debounce=0;debounce<5;debounce++)
    {
        Row = mcp23008_ReadINTCAP(MCP23008_KP_ADDR);
    }
    
    // Invert functions pins and interrupts
    // SET GPIO DIRECTION
    mcp23008_SetGPIODir(0x0f,MCP23008_KP_ADDR);
    
    mcp23008_SetGPIOInt(0x0f,0x0f,MCP23008_KP_ADDR);

    mcp23008_WriteGPIO(0x0f,MCP23008_KP_ADDR);
     
    for(debounce=0;debounce<5;debounce++)				// Reads cols status
    {
        Col = mcp23008_ReadGPIO(MCP23008_KP_ADDR);
    }
    
    kp_Init();
    ShortDelay(delay_5ms);
    
    mcp23008_ReadINTCAP(MCP23008_KP_ADDR);        
    return ((Row)|(Col));
}



//-----------------------------------------------------------------------------------------
//					End of Local Functions
//-----------------------------------------------------------------------------------------

void kp_Init(void)
{
    mcp23008_init();
    MCP23008_IOCON_REG_sType iocon;
    iocon.IOCON_Reg = 0x30;

    // Set GPIO Pullup resistors on GPIO0-3
    mcp23008_SetGPIOPullUps(0x0f,MCP23008_KP_ADDR);
    
    // SET GPIO DIRECTION
    mcp23008_SetGPIODir(0xf0,MCP23008_KP_ADDR);
    
    // WRITE GPIO INITIAL VALUES
    mcp23008_WriteGPIO(0xf0,MCP23008_KP_ADDR);
    
    // SET INPUT POLARITY AS NORMAL INPUTS
    mcp23008_SetIPOL(0,MCP23008_KP_ADDR);

    // Setup Interrupt, DefVal anf GPIO Interrupt Enable register
    mcp23008_SetGPIOInt(0xf0,0xf0,MCP23008_KP_ADDR);
}


uint8_t kp_GetKey(void)
{
    uint8_t keypressed = 0;
    if (mcp23008_ReadINTF(MCP23008_KP_ADDR))
    {
        keypressed = kp_KeyScan(); // Performs the Keyscan for Row and Columns
    }
    
    switch (keypressed)
    {
        case 0x77:
            KeyCode =  '1';
            break;
        case 0x7B:
            KeyCode =  '2';
            break;
        case 0x7D:
            KeyCode =  '3';
            break;
        case 0x7E:
            KeyCode =  'A';
            break;
        case 0xB7:
            KeyCode =  '4';
            break;
        case 0xBB:
            KeyCode =  '5';
            break;
        case 0xBD:
            KeyCode =  '6';
            break;
        case 0xBE:
            KeyCode =  'B';
            break;
        case 0xD7:
            KeyCode =  '7';
            break;
        case 0xDB:
            KeyCode =  '8';
            break;
        case 0xDD:
            KeyCode =  '9';
            break;
        case 0xDE:
            KeyCode =  'C';
            break;
        case 0xE7:
            KeyCode =  '*';
            break;
        case 0xEB:
            KeyCode =  '0';
            break;
        case 0xED:
            KeyCode =  '#';
            break;
        case 0xEE:
            KeyCode =  'D';
            break;
    }
    
    if (kp_LastPressed != KeyCode)
        kp_LastPressed = KeyCode;
    else if (kp_LastPressed == KeyCode)
        KeyCode = NO_KEY;
    
    return KeyCode;
}


//uint8_t* kp_read_mcp23008_regs(void)
//{
//    int i=0;
//    for (i=0;i<11;i++)
//    {
//        i2c_master_cmd_read(KP_I2C_BUS,KP_I2C_ADDR,i,&buf[i],1);
//    }
//    return buf;
//}