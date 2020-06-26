/* ************************************************************************** */
/** Rotary encoder library

  @Company
 Daruin O. Solano

  @File Name
    rotary_encoder.h

  @Summary
    takes the inputs from the switch and encode it to sense weather is Counter Clock Wise CCW 
 * or Clock Wise CW.

  @Description
    Maybe used as an input device to change time, hour, select from menus, etc..
 */
/* ************************************************************************** */

#ifndef _ROTARY_ENCODER_H    /* Guard against multiple inclusion */
#define _ROTARY_ENCODER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <xc.h>
#include <GenericTypeDefs.h>
#include <stdint.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    QEI_STATE_WAIT         =  0,
    QEI_STATE_RIGHT        =  1,
    QEI_STATE_LEFT         = -1,
}QEI_STATE;


/*Setup PINS for input output QEI*/
void qei_setup(void);
/*Read the state for QEI*/
QEI_STATE qei_state(void);
/* Chceck the Puch Button Status from Encoder*/
BOOL qei_check_PB(void);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _ROTARY_ENCODER_H */

/* *****************************************************************************
 End of File
 */
