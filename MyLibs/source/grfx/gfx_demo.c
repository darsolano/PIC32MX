// UTFT_Demo_320x240 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//

#include <grfx/ssd1289.h>
#include <utilities.h>
#include <stdlib.h>
#include <math.h>
#include <grfx/font_SmallFont.h>

// Declare which fonts we will be using

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// My chipKit Uno32/uC32 shield                : <display model>,38,39,40,41
// My chipKit Max32 shield                     : <display model>,82,83,84,85
// AquaLEDSource All in One Super Screw Shield : <display model>,82,83,84,85
//
// Remember to change the model parameter to suit your display module!

void demo_setup(void)
{
    //randomSeed(analogRead(0));

    // Setup the LCD
    tft32_PINS();
    tft32_hdw_reset();
    tft32_InitLCD();
    tft32_backlight_on();
    tft32_setFont(SmallFont, SmallFont_Param);
}

void gfx_demo(void)
{
    demo_setup();   // Setup peripherals and apps
    
    int buf[318];
    int x, x2;
    int y, y2;
    int r, i;

    // Clear the screen and draw the frame
    tft32_clrScr(TFT_BLACK);

    tft32_setColor(TFT_RED);
    tft32_fillRect(0, 0, 319, 13);
    tft32_setColor(TFT_CYAN);
    tft32_fillRect(0, 226, 319, 239);
    tft32_setColor(TFT_WHITE);
    tft32_setBackColor(TFT_RED);
    tft32_print("* Universal Color TFT Display Library *", CENTER, 1, 0);
    tft32_setBackColor(TFT_CYAN);
    tft32_setColor(TFT_YELLOW);
    tft32_print("<http://www.RinkyDinkElectronics.com/>", CENTER, 227, 0);

    tft32_setColor(TFT_BLUE);
    tft32_drawRect(0, 14, 319, 225);

    // Draw crosshairs
    tft32_setColor(TFT_BLUE);
    tft32_setBackColor(TFT_BLACK);
    tft32_drawLine(159, 15, 159, 224);
    tft32_drawLine(1, 119, 318, 119);
    for (i = 9; i < 310; i += 10)
        tft32_drawLine(i, 117, i, 121);
    for (i = 19; i < 220; i += 10)
        tft32_drawLine(157, i, 161, i);

    // Draw sin-, cos- and tan-lines  
    tft32_setColor(TFT_CYAN);
    tft32_print("Sin", 5, 15, 0);
    for (i = 1; i < 318; i++)
    {
        tft32_drawPixel(i, 119 + (sin(((i * 1.13)*3.14) / 180)*95));
    }

    tft32_setColor(TFT_RED);
    tft32_print("Cos", 5, 27, 0);
    for (i = 1; i < 318; i++)
    {
        tft32_drawPixel(i, 119 + (cos(((i * 1.13)*3.14) / 180)*95));
    }

    tft32_setColor(TFT_YELLOW);
    tft32_print("Tan", 5, 39, 0);
    for (i = 1; i < 318; i++)
    {
        tft32_drawPixel(i, 119 + (tan(((i * 1.13)*3.14) / 180)));
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);
    tft32_setColor(TFT_BLUE);
    tft32_setBackColor(TFT_BLACK);
    tft32_drawLine(159, 15, 159, 224);
    tft32_drawLine(1, 119, 318, 119);

    // Draw a moving sinewave
    x = 1;
    for (i = 1; i < (318 * 20); i++)
    {
        x++;
        if (x == 319)
            x = 1;
        if (i > 319)
        {
            if ((x == 159) || (buf[x - 1] == 119))
                tft32_setColor(TFT_BLUE);
            else
                tft32_setColor(TFT_BLACK);
            tft32_drawPixel(x, buf[x - 1]);
        }
        tft32_setColor(TFT_CYAN);
        y = 119 + (sin(((i * 1.1)*3.14) / 180)*(90 - (i / 100)));
        tft32_drawPixel(x, y);
        buf[x - 1] = y;
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);

    // Draw some filled rectangles
    for (i = 1; i < 6; i++)
    {
        switch (i)
        {
        case 1:
            tft32_setColor(TFT_FIREBRICK);
            break;
        case 2:
            tft32_setColor(TFT_RED);
            break;
        case 3:
            tft32_setColor(TFT_GREEN);
            break;
        case 4:
            tft32_setColor(TFT_BLUE);
            break;
        case 5:
            tft32_setColor(TFT_YELLOW);
            break;
        }
        tft32_fillRect(70 + (i * 20), 30 + (i * 20), 130 + (i * 20), 90 + (i * 20));
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);

    // Draw some filled, rounded rectangles
    for (i = 1; i < 6; i++)
    {
        switch (i)
        {
        case 1:
            tft32_setColor(TFT_FIREBRICK);
            break;
        case 2:
            tft32_setColor(TFT_RED);
            break;
        case 3:
            tft32_setColor(TFT_GREEN);
            break;
        case 4:
            tft32_setColor(TFT_BLUE);
            break;
        case 5:
            tft32_setColor(TFT_YELLOW);
            break;
        }
        tft32_fillRoundRect(190 - (i * 20), 30 + (i * 20), 250 - (i * 20), 90 + (i * 20));
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);

    // Draw some filled circles
    for (i = 1; i < 6; i++)
    {
        switch (i)
        {
        case 1:
            tft32_setColor(TFT_FIREBRICK);
            break;
        case 2:
            tft32_setColor(TFT_RED);
            break;
        case 3:
            tft32_setColor(TFT_GREEN);
            break;
        case 4:
            tft32_setColor(TFT_BLUE);
            break;
        case 5:
            tft32_setColor(TFT_YELLOW);
            break;
        }
        tft32_fillCircle(100 + (i * 20), 60 + (i * 20), 30);
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);

    // Draw some lines in a pattern
    tft32_setColor(TFT_RED);
    for (i = 15; i < 224; i += 5)
    {
        tft32_drawLine(1, i, (i * 1.44) - 10, 224);
    }
    tft32_setColor(TFT_RED);
    for (i = 224; i > 15; i -= 5)
    {
        tft32_drawLine(318, i, (i * 1.44) - 11, 15);
    }
    tft32_setColor(TFT_CYAN);
    for (i = 224; i > 15; i -= 5)
    {
        tft32_drawLine(1, i, 331 - (i * 1.44), 15);
    }
    tft32_setColor(TFT_CYAN);
    for (i = 15; i < 224; i += 5)
    {
        tft32_drawLine(318, i, 330 - (i * 1.44), 224);
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);

    // Draw some random circles
    for (i = 0; i < 100; i++)
    {
        tft32_setColor(rand() % 0xffff);
        x = 32 + rand() % 256;
        y = 45 + rand() % 146;
        r = rand() % 30;
        tft32_drawCircle(x, y, r);
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);

    // Draw some random rectangles
    for (i = 0; i < 100; i++)
    {
        tft32_setColor(rand() % 0xffff);
        x = 2 + rand() % 316;
        y = 16 + rand() % 207;
        x2 = 2 + rand() % 316;
        y2 = 16 + rand() % 207;
        tft32_drawRect(x, y, x2, y2);
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);

    // Draw some random rounded rectangles
    for (i = 0; i < 100; i++)
    {
        tft32_setColor(rand() % 0xffff);
        x = 2 + rand() % 316;
        y = 16 + rand() % 207;
        x2 = 2 + rand() % 316;
        y2 = 16 + rand() % 207;
        tft32_drawRoundRect(x, y, x2, y2);
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);

    for (i = 0; i < 100; i++)
    {
        tft32_setColor(rand() % 0xffff);
        x = 2 + rand() % 316;
        y = 16 + rand() % 209;
        x2 = 2 + rand() % 316;
        y2 = 16 + rand() % 209;
        tft32_drawLine(x, y, x2, y2);
    }

    ShortDelay(delay_1sec);

    tft32_setColor(TFT_BLACK);
    tft32_fillRect(1, 15, 318, 224);

    for (i = 0; i < 10000; i++)
    {
        tft32_setColor(rand()&0xffff);
        tft32_drawPixel(2 + rand() % 316, 16 + rand() % 209);
    }

    ShortDelay(delay_1sec);

    tft32_fillScr(TFT_BLUE);
    tft32_setColor(TFT_RED);
    tft32_fillRoundRect(80, 70, 239, 169);

    tft32_setColor(TFT_WHITE);
    tft32_setBackColor(TFT_RED);
    tft32_print("That's it!", CENTER, 93, 0);
    tft32_print("Restarting in a", CENTER, 119, 0);
    tft32_print("few seconds...", CENTER, 132, 0);

    tft32_setColor(TFT_GREEN);
    tft32_setBackColor(TFT_BLUE);
    tft32_print("Runtime: (msecs)", CENTER, 210, 0);
    tft32_printNumI(read_timer(), CENTER, 225);

    ShortDelay(delay_1sec * 10);
}

