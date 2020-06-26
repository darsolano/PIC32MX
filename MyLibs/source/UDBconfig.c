
#include <UDBconfig.h>
#include <proc/p32mx360f512l.h>

#define LED_ON  1
#define LED_OFF 0

#define PIN_INPUT   1
#define PIN_OUTPUT  0

/*** Button Definitions *********************************************/
#define SW0_PORT  PORTEbits.RE8
#define SW1_PORT  PORTEbits.RE9
#define SW2_PORT  PORTDbits.RD13

#define SW0_TRIS  TRISEbits.TRISE8
#define SW1_TRIS  TRISEbits.TRISE9
#define SW2_TRIS  TRISDbits.TRISD13

#define BUTTON_PRESSED      1
#define BUTTON_NOT_PRESSED  0

#define PIN_INPUT           1
#define PIN_OUTPUT          0

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
void UDB_LED_On(LED led)
{
    LATACLR = 1<<led;
}

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
void UDB_LED_Off(LED led)
{
    LATASET = 1<<led;
}

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
void UDB_LED_Toggle(LED led)
{
    LATAINV = 1<<led;
}

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
bool UDB_LED_Get(LED led)
{
    return ( (LATA && LED_ON<<led) ? true : false );

}

/*********************************************************************
* Function: void LED_Enable(LED led);
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
void UDB_LED_Enable(void)
{
    TRISA = 0xff00;
    LATA = 0xffff;
}

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
void UDB_LED_All(uint8_t value)
{
    LATA = ~(value<<0);
}


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
bool UDB_BUTTON_IsPressed(BUTTON button)
{
    switch(button)
    {
        case BUTTON_SW0:
            return ( (SW0_PORT == BUTTON_PRESSED) ? true : false);

        case BUTTON_SW1:
            return ( (SW1_PORT == BUTTON_PRESSED) ? true : false);

        case BUTTON_SW2:
            return ( (SW2_PORT == BUTTON_PRESSED) ? true : false);
    }
    
    return false;
}

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
void UDB_BUTTON_Enable(BUTTON button)
{
    switch(button)
    {
        case BUTTON_SW0:
            SW0_TRIS = PIN_INPUT;
            break;

        case BUTTON_SW1:
            SW1_TRIS = PIN_INPUT;
            break;

        case BUTTON_SW2:
            SW2_TRIS = PIN_INPUT;
            break;
    }
}