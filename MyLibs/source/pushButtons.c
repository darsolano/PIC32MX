
#include <define_pins.h>
#include <pushButtons.h>
#include <stdint.h>
#include <utilities.h>

// Data Structure pointer
PB_STATE_t *pb = &pushB;

void (*btnReadFunc)(void);
// Five Buttons for input
DEFINE_PIN (PB_MENU,D,0)
DEFINE_PIN (PB_LEFT,D,1)
DEFINE_PIN (PB_UP,D,2)
DEFINE_PIN (PB_DOWN,D,3)
DEFINE_PIN (PB_RIGHT,D,4)
DEFINE_PIN (PB_SELECT,D,5)


void pb_Init(void)
{
    PB_MENU_INPUT();
    PB_RIGHT_INPUT();
    PB_LEFT_INPUT();
    PB_UP_INPUT();
    PB_DOWN_INPUT();
    PB_SELECT_INPUT();
}

void pb_Debounce(btnReadFunc)
{
    
}

/*
 * State must be ored in the current state variable
 * and previous state variable.
 */
PUSH_BUTTON_t pb_Get_PshBtn_State(void)
{
    pb->CURR_STATE = 0;
    if (!PB_MENU_READ()) pb->CURR_STATE = PSHBTN_MENU;
    if (!PB_RIGHT_READ()) pb->CURR_STATE = PSHBTN_RIGHT;
    if (!PB_LEFT_READ()) pb->CURR_STATE = PSHBTN_LEFT;
    if (!PB_UP_READ()) pb->CURR_STATE = PSHBTN_UP;
    if (!PB_DOWN_READ()) pb->CURR_STATE = PSHBTN_DOWN;
    if (!PB_SELECT_READ()) pb->CURR_STATE = PSHBTN_SELECT;
    
    ShortDelay(delay_10ms);
    
    // Verify if there has being a change in the button state
    if (pb->CURR_STATE != pb->PREV_STATE)
        pb->PREV_STATE = pb->CURR_STATE;
    else pb->CURR_STATE = PSHBTN_WAIT;
    return pb->CURR_STATE;
}