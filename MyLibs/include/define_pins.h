/*
 * define_pins.h
 *
 *  Created on: Apr 1, 2016
 *      Author: dsolano
 */

#ifndef INCLUDE_DEFINE_PINS_H_
#define INCLUDE_DEFINE_PINS_H_

/* define_pins.h
 *
 * Define a useful macro for giving GPIO pins nicer, semantic names.
 *
 * The only macro exported is DEFINE_PIN. Use as:
 *  DEFINE_PIN(PIN_NAME, port number, pin number)
 * this will define the following:
 *  PIN_NAME_DEASSERT(), _OFF(), _LOW() to bring the pin low
 *  PIN_NAME_ASSERT(), _ON(), _HIGH() to bring the pin high
 *  PIN_NAME_INPUT() to make the pin an input
 *  PIN_NAME_OUTPUT() to make the pin an output
 *  PIN_NAME_READ() to read the current state of the pin
 *                  (returns a 1 or 0, typed as a uint_fast8_t)
 */

#include <stdint.h>
#include <xc.h>
#include <GenericTypeDefs.h>


#define DIR_INPUT   1
#define DIR_OUTPUT  0

/* INPUT CHANGE NOTIFICATION */
// ICN for PIC32MX360F512
#define CN0_RC14    0
#define CN1_RC13    1
#define CN2_RB0    2
#define CN3_RB1    3
#define CN4_RB2    4
#define CN5_RB3    5
#define CN6_RB4    6
#define CN7_RB5     7
#define CN8_RG6     8
#define CN9_RG7     9
#define CN10_RG8     10
#define CN11_RG9     11
#define CN12_RB15    12
#define CN13_RD4     13
#define CN14_RD5     14
#define CN15_RD6     15
#define CN16_RD7     16
#define CN17_RF4     17
#define CN18_RF5     18
#define CN19_RD13    19
#define CN20_RD14    20
#define CN21_RD15    21

/* IN order to define Analog or Digital PINS on AD1PCG Register */
#define AN0_RB0     0
#define AN1_RB1     1
#define AN2_RB2     2
#define AN3_RB3     3
#define AN4_RB4     4
#define AN5_RB5     5
#define AN6_RB6     6
#define AN7_RB7     7
#define AN8_RB8     8
#define AN9_RB9     9
#define AN10_RB10   10
#define AN11_RB11   11
#define AN12_RB12   12
#define AN13_RB13   13
#define AN14_RB14   14
#define AN15_RB15   15

/*
 * Define globla interrupt for Input Change Notification
 */
#define mGLOBAL_CN_INT_ENABLE(name,priority,subpri)\
inline static void name##_cn_interrupt_enable()\
{\
    CNCONbits.ON = 1; \
    CNCONbits.SIDL = 0;\
    IEC1bits.CNIE = 1;\
    IPC6bits.CNIP = priority;\
    IPC6bits.CNIS = subpri;\
}\
inline static void name##_cn_clear_int_flag()\
{\
    IFS1bits.CNIF = 0;\
}

/*
 Define INPUT CHANGE NOTIFICATION interrupts
 * //                                         Vector
//                                     IRQ    Number           Interrupt Bit Location
//    Highest Natural OrderPriority                      Flag    Enable    Priority    Subpriority
 * CN ? Input Change Interrupt          32      26      IFS1<0> IEC1<0> IPC6<20:18>     IPC6<17:16>
 */
#define DEFINE_INPUT_CHANGE_NOTIFICATION_INT(name, cn_number,port,pin)\
inline static void _##name##_##cn_number##_ENABLE_INT(){\
    CNENSET = (1<<cn_number);\
}\
inline static void _##name##_##cn_number##_ENABLE_PULLUP(){\
    CNPUESET = (1<<cn_number);\
}\
inline static void name##_##cn_number##_INPUT() { \
    TRIS##port##SET = (1<<pin); \
} \
inline static uint_fast8_t _##name##_##cn_number##_READ() { \
    return (PORTB & (1<<pin)) >> pin;\
}


//Interrupt Source(1) 
//                                     Vector
//                               IRQ   Number           Interrupt Bit Location
//Highest Natural Order Priority                   Flag    Enable    Priority    Subpriority
//INT0 ? External Interrupt 0     3       3       IFS0<3>  IEC0<3>  IPC0<28:26> IPC0<25:24>
//INT1 ? External Interrupt 1     7       7       IFS0<7>  IEC0<7>  IPC1<28:26> IPC1<25:24>
//INT2 ? External Interrupt 2     11      11      IFS0<11> IEC0<11> IPC2<28:26> IPC2<25:24>
//INT3 ? External Interrupt 3     15      15      IFS0<15> IEC0<15> IPC3<28:26> IPC3<25:24>
//INT4 ? External Interrupt 4     19      19      IFS0<19> IEC0<19> IPC4<28:26> IPC4<25:24>

#define DEFINE_EXTERNAL_INT(int_name,int_number,priority,port,pin) \
inline static void int_name##_FALLING_EDGE() { \
    INTCONCLR = (1<<int_number);\
}\
inline static void int_name##_RISING_EDGE() { \
    INTCONSET = (1<<int_number); \
}\
inline static void int_name##_CLEAR_INT_FLAG() { \
    IFS0bits.INT##int_number##IF = 0; \
}\
inline static void int_name##_INT_##int_number##_ENABLE() { \
    IEC0bits.INT##int_number##IE = 1; \
}\
inline static void int_name##_INT_##int_number##_DISABLE() { \
    IEC0bits.INT##int_number##IE = 0; \
}\
inline static void int_name##_INT_##int_number##_PRIORITY##priority##_() { \
    IPC##int_number##bits.INT##int_number##IP = priority; \
}\
inline static void int_name##_INT_##int_number##_INPUT() { \
  TRIS##port##SET = (1<<pin); \
} \
inline static uint_fast8_t int_name##_##int_number##_READ() { \
  return (PORT##port & (1<<pin)) >> pin;\
}


#define DEFINE_PIN(name, port, pin) \
inline static void name##_DEASSERT() { \
  LAT##port##CLR = (1<<pin); \
} \
inline static void name##_OFF() { \
  LAT##port##CLR = (1<<pin); \
} \
inline static void name##_LOW() { \
  LAT##port##CLR = (1<<pin); \
} \
inline static void name##_ASSERT() { \
  LAT##port##SET = (1<<pin); \
} \
inline static void name##_ON() { \
  LAT##port##SET = (1<<pin); \
} \
inline static void name##_HIGH() { \
  LAT##port##SET = (1<<pin); \
} \
inline static void name##_TOGGLE() { \
  LAT##port##INV = (1<<pin); \
} \
inline static void name##_INPUT() { \
  TRIS##port##SET = (1<<pin); \
} \
inline static void name##_OUTPUT() { \
  TRIS##port##CLR = (1<<pin); \
} \
inline static uint_fast8_t name##_READ() { \
  return (PORT##port & (1<<pin)) >> pin;\
}\
inline static void name##_SELECTED() { \
  LAT##port##CLR = (1<<pin); \
} \
inline static void name##_DESELECTED() { \
  LAT##port##SET = (1<<pin); \
}


/* FOR definition of analog or digital use of the pins*/
#define DEFINE_ANALOG_DIGITAL_PIN(name,pin_number)\
inline static void name##_##pin_number##_DIGITAL_PIN(){\
    AD1PCFGSET = (1<<pin_number);\
}\
inline static void name##_##pin_number##_ANALOG_PIN(){\
    AD1PCFGCLR = (1<<pin_number);\
}
#endif /* INCLUDE_DEFINE_PINS_H_ */
