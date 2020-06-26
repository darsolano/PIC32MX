///*
// * tftTouch.c// *
// *  Created on: Oct 13, 2015
// *      Author: dsolano
// */
//
#include <grfx/tftTouch.h>
#include <define_pins.h>
#include <utilities.h>
#include <plib.h>
#include <sys/attribs.h>




#define TOUCHSCREEN			SPI_CHANNEL1

DEFINE_EXTERNAL_INT(EXT_INT_1,1,7,B,1       )
// Define pins correctly before use them
DEFINE_PIN(T_CS, B, 1)
DEFINE_PIN(T_PENIRQ, E, 8)
DEFINE_PIN(LED1,B,4)

void __ISR(_EXTERNAL_1_VECTOR,IPL7SOFT) touch_PENIRQ_ISRhandler(void);

/* Interrupt setup for PENIRQ EXT INT 0*/
void touch_setup_interrupt(void)
{
    EXT_INT_1_INT_1_ENABLE();       // Enable INT1
    EXT_INT_1_INT_1_PRIORITY7_();   // Highest Interrupt available
    EXT_INT_1_RISING_EDGE();       // Going from High to Low as PENIRQ behaviour
}
void __ISR(_EXTERNAL_1_VECTOR,IPL7SOFT) touch_PENIRQ_ISRhandler(void)
{
    int status;
    EXT_INT_1_CLEAR_INT_FLAG();
    status = INTDisableInterrupts();
    
    touch.PENIRQ_Pressed = TRUE;

    LED1_TOGGLE();
    
    INTEnable(INT_INT0,INT_ENABLED);
}

/* END EXT INT o Setup*/



struct touch_panel_s
{
    long _default_orientation;
    byte orient;
    byte prec;
    byte display_model;
    long disp_x_size, disp_y_size, default_orientation;
    long touch_x_left, touch_x_right, touch_y_top, touch_y_bottom;
};
struct touch_panel_s tp;

void touch_InitTouch(uint8_t orient)
{
    AD1PCFG = 0xffff;
    pic32_Spi_Init(TOUCHSCREEN,  500000);
    LED1_OUTPUT();
    T_CS_OUTPUT();      // OUT		Chip Select Out
    T_PENIRQ_INPUT();   // IN		Touch IRQ
    T_CS_HIGH();
    touch.PENIRQ_Pressed = FALSE;
    
    tp.orient                   = orient;
    tp._default_orientation     = CAL_S>>31;
    tp.touch_x_left             = (CAL_X>>14) & 0x3FFF;
    tp.touch_x_right            = CAL_X & 0x3FFF;
    tp.touch_y_top              = (CAL_Y>>14) & 0x3FFF;
    tp.touch_y_bottom           = CAL_Y & 0x3FFF;
    tp.disp_x_size              = (CAL_S>>12) & 0x0FFF;
    tp.disp_y_size              = CAL_S & 0x0FFF;
    tp.prec = 10;
    touch_setup_interrupt();
}

// SPI to send data
static void touch_WriteData(uint8_t data)
{
    pic32_SpiXfer(TOUCHSCREEN, data);
}

// SPI to get data
static uint16_t touch_ReadData(void)
{
    uint8_t temp = 0;
    uint16_t data = 0;

    data = pic32_SpiXfer(TOUCHSCREEN, 0xff); // HIGH nibble - MSB first
    data <<= 8;
    temp = pic32_SpiXfer(TOUCHSCREEN, 0xff); // LOW  nibble - LSB last
    data |= temp;
    return (data>>3);
}

void touch_read(void)
{
    unsigned long tx    = 0, temp_x     = 0;
    unsigned long ty    = 0, temp_y     = 0;
    unsigned long minx  = 99999, maxx   = 0;
    unsigned long miny  = 99999, maxy   = 0;
    int datacount = 0;
    int i;

    T_CS_DEASSERT();
    for (i = 0; i < tp.prec; i++)
    {
        if (touch.PENIRQ_Pressed)
        {
            touch_WriteData(0x90);  // X Axis read switch, 12 bits, differential read command
            temp_x = touch_ReadData();

            ShortDelay(delay_100us);

            touch_WriteData(0xD0);
            temp_y = touch_ReadData();

            if ((temp_x > 0) && (temp_x < 4096) && (temp_y > 0) && (temp_y < 4096))
            {
                tx += temp_x;
                ty += temp_y;
                datacount++;
            }
        }
    }
    T_CS_ASSERT();
    
    if (datacount == tp.prec)
    {
        if (tp.orient == tp._default_orientation)
        {
            touch.TP_X = ty / datacount;
            touch.TP_Y = tx / datacount;
        }
        else
        {
            touch.TP_X = tx / datacount;
            touch.TP_Y = ty / datacount;
        }
    }
    else
    {
        touch.TP_X = -1;
        touch.TP_Y = -1;
    }
    touch.PENIRQ_Pressed = FALSE;
}

BOOL touch_dataAvailable(void)
{
    return touch.PENIRQ_Pressed;
}

int16_t touch_getTouchX(void)   // Raw X data
{
    long c;

    if ((touch.TP_X == -1) || (touch.TP_Y == -1))
        return -1;

    c = touch.TP_X;

    return c;
}

int16_t touch_getTouchY(void)   // Raw Y Data
{
    long c;

    if ((touch.TP_X == -1) || (touch.TP_Y == -1))
        return -1;

    c = touch.TP_Y;

    return c;
}

int touch_getX(void)
{
    long c;

    if ((touch.TP_X == -1) || (touch.TP_Y == -1))
        return -1;
    if (tp.orient == tp._default_orientation)
    {
            c = (long)((long)(touch.TP_X - tp.touch_x_left) * (tp.disp_x_size)) / (long)(tp.touch_x_right - tp.touch_x_left);
            if (c<0) c = 0;
            if (c>tp.disp_x_size) c = tp.disp_x_size;
    }
    else
    {
            if (tp._default_orientation == PORTRAIT)
                    c = (long)((long)(touch.TP_X - tp.touch_y_top) * (-tp.disp_y_size)) / (long)(tp.touch_y_bottom - tp.touch_y_top) + (long)(tp.disp_y_size);
            else
                    c = (long)((long)(touch.TP_X - tp.touch_y_top) * (tp.disp_y_size)) / (long)(tp.touch_y_bottom - tp.touch_y_top);
            if (c<0)
                    c = 0;
            if (c>tp.disp_y_size)
                    c = tp.disp_y_size;
    }
    return c;
}

int touch_getY(void)
{
    int c;

    if ((touch.TP_X == -1) || (touch.TP_Y == -1))
        return -1;

    if (tp.orient == tp._default_orientation)
    {
            c = (long)((long)(touch.TP_Y - tp.touch_y_top) * (tp.disp_y_size)) / (long)(tp.touch_y_bottom - tp.touch_y_top);
            if (c<0)
                    c = 0;
            if (c>tp.disp_y_size)
                    c = tp.disp_y_size;
    }
    else
    {
            if (tp._default_orientation == PORTRAIT)
                    c = (long)((long)(touch.TP_Y - tp.touch_x_left) * (tp.disp_x_size)) / (long)(tp.touch_x_right - tp.touch_x_left);
            else
                    c = (long)((long)(touch.TP_Y - tp.touch_x_left) * (-tp.disp_x_size)) / (long)(tp.touch_x_right - tp.touch_x_left) + (long)(tp.disp_x_size);
            if (c<0)
                    c = 0;
            if (c>tp.disp_x_size)
                    c = tp.disp_x_size;
    }
    return c;
}

void touch_setPrecision(uint8_t prec)
{
    switch (prec)
    {
    case PREC_LOW:
        tp.prec = 1;
        break;
    case PREC_MEDIUM:
        tp.prec = 12;
        break;
    case PREC_HI:
        tp.prec = 27;
        break;
    case PREC_EXTREME:
        tp.prec = 102;
        break;
    default:
        tp.prec = 12;
        break;
    }
}

void touch_calibrateRead()
{
    unsigned long tx = 0;
    unsigned long ty = 0;
    if (touch.PENIRQ_Pressed)
    {
        T_CS_DEASSERT();
        touch_WriteData(0x90);
        tx = touch_ReadData();

        touch_WriteData(0xD0);
        ty = touch_ReadData();

        T_CS_ASSERT();
        touch.TP_X = ty;
        touch.TP_Y = tx;
    }
    touch.PENIRQ_Pressed = FALSE;
}

