
#include <max7219.h>
#include <serial_3w.h>
#include <define_pins.h>
#include <stdint.h>
#include <utilities.h>

//void dm8x8_Setup(void)
//{
//    max7219_init(DISP_ALL_DIGITS, INT_29,NO_DECODE_DIG_0_7);
//    dm8x8_TestLeds1x1();
//}
//
//void dm8x8_TestLeds1x1(void)
//{
//    uint8_t reg,data;
//    for (reg =1;reg<9;reg++)
//    {
//        for (data=0;data<8;data++)
//        {
//            MAX7219(reg,(1<<data));
//            ShortDelay(delay_100ms);
//            MAX7219(reg,0);
//        }
//    }
//}