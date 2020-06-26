/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include <rotary_encoder.h>
#include <plib.h>
#include <stdint.h>
#include <define_pins.h>

#include "serial_3w.h"


/* TODO:  Include other files here if needed. */

mGLOBAL_CN_INT_ENABLE(Rotary_Encoder,7,0)
DEFINE_PIN(CENTER,B,0)
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
typedef struct _qei_t {
    /* Data Structure for QEI. */
    QEI_STATE   State;
    uint8_t     Curr_Read;
    uint8_t     Last_Read;
    uint8_t     Encoded_Value;
    BOOL        PB_State;
} QEI_t;
QEI_t qei;
QEI_t* pQei = &qei;

/*
 *  State Machine Match Table array
 */
//QEI_STATE QEI_CURR_STATE[] = {  QEI_STATE_WAIT, // 0b 00    00
//                                QEI_STATE_LEFT, // 0b 00    01
//                                QEI_STATE_RIGHT,// 0b 00    10
//                                QEI_STATE_WAIT, // 0b 00    11
//                                QEI_STATE_RIGHT,// 0b 01    00
//                                QEI_STATE_WAIT, // 0b 01    01
//                                QEI_STATE_WAIT, // 0b 01    10
//                                QEI_STATE_LEFT, // 0b 01    11
//                                QEI_STATE_LEFT, // 0b 10    00
//                                QEI_STATE_WAIT, // 0b 10    01
//                                QEI_STATE_WAIT, // 0b 10    10
//                                QEI_STATE_RIGHT,// 0b 10    11
//                                QEI_STATE_WAIT, // 0b 11    00
//                                QEI_STATE_RIGHT,// 0b 11    01
//                                QEI_STATE_LEFT, // 0b 11    10
//                                QEI_STATE_WAIT  // 0b 11    11
//};
/* This will only takes the transition states for a full rotation */
QEI_STATE QEI_CURR_STATE[] = {  QEI_STATE_WAIT, // 0b 00    00
                                QEI_STATE_WAIT, // 0b 00    01
                                QEI_STATE_WAIT, // 0b 00    10
                                QEI_STATE_WAIT, // 0b 00    11
                                QEI_STATE_RIGHT,// 0b 01    00
                                QEI_STATE_WAIT, // 0b 01    01
                                QEI_STATE_WAIT, // 0b 01    10
                                QEI_STATE_WAIT, // 0b 01    11
                                QEI_STATE_LEFT, // 0b 10    00
                                QEI_STATE_WAIT, // 0b 10    01
                                QEI_STATE_WAIT, // 0b 10    11
                                QEI_STATE_WAIT, // 0b 11    00
                                QEI_STATE_WAIT, // 0b 11    01
                                QEI_STATE_WAIT, // 0b 11    10
                                QEI_STATE_WAIT  // 0b 11    11
};


// *****************************************************************************
//  ISR to capture Input Changes
// *****************************************************************************
//void __ISR(_CHANGE_NOTICE_VECTOR,IPL7SRS) Rotary_Encoder_CN_ISR_Handler(void)
//{
//    int status;
//    int port_read;
//    
//    /* Read first and put it into Current read*/
//    Rotary_Encoder_cn_clear_int_flag();    
//    //status = INTDisableInterrupts();
//    
//     
//    PLIB_PORTS_PinToggle(PORTS_ID_0 , PORT_CHANNEL_A, PORTS_BIT_POS_10);
//    //INTClearFlag(INT_CN);
//    _
//          
//    INTRestoreInterrupts(status);
//    SendString("Access CN interrupt\n");
//}
        
void qei_setup(void)
{
    PORTSetPinsDigitalIn(IOPORT_B,BIT_0|BIT_3|BIT_4);

}


// *****************************************************************************
// This implements a Rotary Encoder State Machine - Quadrature Encoder Interface
// RB3 = A ,  RB4 = B
// *****************************************************************************
QEI_STATE qei_state(void)
{
    uint16_t port_read = PORTB;
    /* DO the data manipulation to determine the direction
    of the rotary encoder rotation */
    pQei->Curr_Read = (port_read & Bit(3)) == 0 ? 1:0; // A pin
    pQei->Curr_Read <<= 1;
    pQei->Curr_Read |= (port_read & Bit(4)) == 0 ? 1:0; // B pin
        
    pQei->Encoded_Value = (pQei->Last_Read << 2) | pQei->Curr_Read;
    pQei->State = QEI_CURR_STATE[pQei->Encoded_Value];
    pQei->Last_Read = pQei->Curr_Read;
    return pQei->State;
}


// *****************************************************************************
// This implements a Rotary Encoder State Machine - Reads central PB Push true
// Not pushed false
// *****************************************************************************
BOOL qei_check_PB(void)
{
    /* Read Select Button pushing the Center */
    pQei->PB_State = !CENTER_READ();
    return pQei->PB_State;
}

/* *****************************************************************************
 End of File
 */
