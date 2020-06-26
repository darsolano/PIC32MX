/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _PUSHBUTTONS_H    /* Guard against multiple inclusion */
#define _PUSHBUTTONS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <xc.h>
#include <GenericTypeDefs.h>
#include <stdint.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        PSHBTN_WAIT  = 0x00,
        PSHBTN_MENU  = 0x01,
        PSHBTN_LEFT  = 0x02,
        PSHBTN_UP    = 0x04,
        PSHBTN_DOWN  = 0x08,
        PSHBTN_RIGHT = 0x10,
        PSHBTN_SELECT= 0x20                
    }PUSH_BUTTON_t;
    
    
    typedef struct PB_STATE_s
    {
        uint8_t CURR_STATE;      // Indicate which PB was pressed
        uint8_t PREV_STATE;      // It stores the previous state of the buttons
    }PB_STATE_t;
    PB_STATE_t pushB;

    void            pb_Init(void);
    PUSH_BUTTON_t   pb_Get_PshBtn_State(void);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
