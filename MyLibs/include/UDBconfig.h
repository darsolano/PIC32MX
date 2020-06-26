/* 
 * File:   system_config.h
 * Author: dsolano
 *
 * Created on May 9, 2014, 11:22 AM
 */

#pragma once


#ifndef SYSTEM_CONFIG_H
#define	SYSTEM_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
// Include Files
#include <xc.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <GenericTypeDefs.h>

#include <stdbool.h>

//    pot1 =  rb3
//    pot2 = rb4        
/** Type defintions *********************************/
typedef enum
{
    LED_D0,
    LED_D1,
    LED_D2,
    LED_D3,
    LED_D4,
    LED_D5,
    LED_D6,
    LED_D7,
    LED_COUNT
} LED;


/*********************************************************************
* Function: void LED_On(LED led);
*
* Overview: Turns requested LED on
*
* PreCondition: LED configured via LED_Configure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of 
*        the LEDs on the silkscreen on the board (as the demo code may 
*        be ported to other boards).
*         i.e. - LED_On(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void UDB_LED_On(LED led);

/*********************************************************************
* Function: void LED_Off(LED led);
*
* Overview: Turns requested LED off
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of 
*        the LEDs on the silkscreen on the board (as the demo code may 
*        be ported to other boards).
*         i.e. - LED_Off(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void UDB_LED_Off(LED led);

/*********************************************************************
* Function: void LED_Toggle(LED led);
*
* Overview: Toggles the state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of 
*        the LEDs on the silkscreen on the board (as the demo code may 
*        be ported to other boards).
*         i.e. - LED_Toggle(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void UDB_LED_Toggle(LED led);

/*********************************************************************
* Function: bool LED_Get(LED led);
*
* Overview: Returns the current state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of 
*        the LEDs on the silkscreen on the board (as the demo code may 
*        be ported to other boards).
*         i.e. - LED_Get(LED_CONNECTION_DETECTED);
*
* Output: true if on, false if off
*
********************************************************************/
bool UDB_LED_Get(LED led);

/*********************************************************************
* Function: bool LED_Enable(LED led);
*
* Overview: Configures the LED for use by the other LED API
*
* PreCondition: none
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*
* Output: none
*
********************************************************************/
void UDB_LED_Enable(void);

/*********************************************************************
* Function: bool LED_All(uint8_t);
*
* Overview: Configures the LED for use by the other LED API
*
* PreCondition: none
*
* Input: values - 8 bit value that will be presented to leds
*        demo.  each bit of the word represent a led in the array
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards). Remember that led will be on when
*        when a zero is applied to them. 
*
* Output: none
*
********************************************************************/
void UDB_LED_All(uint8_t value);

/*** Button Definitions *********************************************/
typedef enum
{
    BUTTON_SW0 = 0x01,
    BUTTON_SW1 = 0x02,
    BUTTON_SW2 = 0x04,
    BUTTON_COUNT = 3
            
} BUTTON;

/*********************************************************************
* Function: bool BUTTON_IsPressed(BUTTON button);
*
* Overview: Returns the current state of the requested button
*
* PreCondition: button configured via BUTTON_SetConfiguration()
*
* Input: BUTTON button - enumeration of the buttons available in
*        this demo.  They should be meaningful names and not the names 
*        of the buttons on the silkscreen on the board (as the demo 
*        code may be ported to other boards).
*         i.e. - ButtonIsPressed(BUTTON_SEND_MESSAGE);
*
* Output: TRUE if pressed; FALSE if not pressed.
*
********************************************************************/
bool UDB_BUTTON_IsPressed(BUTTON button);

/*********************************************************************
* Function: void BUTTON_Enable(BUTTON button);
*
* Overview: Returns the current state of the requested button
*
* PreCondition: button configured via BUTTON_SetConfiguration()
*
* Input: BUTTON button - enumeration of the buttons available in
*        this demo.  They should be meaningful names and not the names
*        of the buttons on the silkscreen on the board (as the demo
*        code may be ported to other boards).
*         i.e. - ButtonIsPressed(BUTTON_SEND_MESSAGE);
*
* Output: None
*
********************************************************************/
void UDB_BUTTON_Enable(BUTTON button);



#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_CONFIG_H */



