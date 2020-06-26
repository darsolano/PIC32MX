/* Created by plibgen $Revision: 1.31 $ */

#ifndef _INT_P32MK0512MCF100_H
#define _INT_P32MK0512MCF100_H

/* Section 1 - Enumerate instances, define constants, VREGs */

#include <xc.h>
#include <stdbool.h>

#include "peripheral/peripheral_common_32bit.h"

/* Default definition used for all API dispatch functions */
#ifndef PLIB_INLINE_API
    #define PLIB_INLINE_API extern inline
#endif

/* Default definition used for all other functions */
#ifndef PLIB_INLINE
    #define PLIB_INLINE extern inline
#endif

typedef enum {

    INT_ID_0 = 0,
    INT_NUMBER_OF_MODULES = 1

} INT_MODULE_ID;

typedef enum {

    INT_EXTERNAL_INT_SOURCE0 = 0x01,
    INT_EXTERNAL_INT_SOURCE1 = 0x02,
    INT_EXTERNAL_INT_SOURCE2 = 0x04,
    INT_EXTERNAL_INT_SOURCE3 = 0x08,
    INT_EXTERNAL_INT_SOURCE4 = 0x10

} INT_EXTERNAL_SOURCES;

typedef enum {

    INT_DISABLE_INTERRUPT = 0,
    INT_PRIORITY_LEVEL1 = 1,
    INT_PRIORITY_LEVEL2 = 2,
    INT_PRIORITY_LEVEL3 = 3,
    INT_PRIORITY_LEVEL4 = 4,
    INT_PRIORITY_LEVEL5 = 5,
    INT_PRIORITY_LEVEL6 = 6,
    INT_PRIORITY_LEVEL7 = 7

} INT_PRIORITY_LEVEL;

typedef enum {

    INT_SUBPRIORITY_LEVEL0 = 0x00,
    INT_SUBPRIORITY_LEVEL1 = 0x01,
    INT_SUBPRIORITY_LEVEL2 = 0x02,
    INT_SUBPRIORITY_LEVEL3 = 0x03

} INT_SUBPRIORITY_LEVEL;

typedef enum {

    INT_SOURCE_TIMER_CORE = 0,
    INT_SOURCE_SOFTWARE_0 = 1,
    INT_SOURCE_SOFTWARE_1 = 2,
    INT_SOURCE_EXTERNAL_0 = 3,
    INT_SOURCE_TIMER_1 = 4,
    INT_SOURCE_INPUT_CAPTURE_1_ERROR = 5,
    INT_SOURCE_INPUT_CAPTURE_1 = 6,
    INT_SOURCE_OUTPUT_COMPARE_1 = 7,
    INT_SOURCE_EXTERNAL_1 = 8,
    INT_SOURCE_TIMER_2 = 9,
    INT_SOURCE_INPUT_CAPTURE_2_ERROR = 10,
    INT_SOURCE_INPUT_CAPTURE_2 = 11,
    INT_SOURCE_OUTPUT_COMPARE_2 = 12,
    INT_SOURCE_EXTERNAL_2 = 13,
    INT_SOURCE_TIMER_3 = 14,
    INT_SOURCE_INPUT_CAPTURE_3_ERROR = 15,
    INT_SOURCE_INPUT_CAPTURE_3 = 16,
    INT_SOURCE_OUTPUT_COMPARE_3 = 17,
    INT_SOURCE_EXTERNAL_3 = 18,
    INT_SOURCE_TIMER_4 = 19,
    INT_SOURCE_INPUT_CAPTURE_4_ERROR = 20,
    INT_SOURCE_INPUT_CAPTURE_4 = 21,
    INT_SOURCE_OUTPUT_COMPARE_4 = 22,
    INT_SOURCE_EXTERNAL_4 = 23,
    INT_SOURCE_TIMER_5 = 24,
    INT_SOURCE_INPUT_CAPTURE_5_ERROR = 25,
    INT_SOURCE_INPUT_CAPTURE_5 = 26,
    INT_SOURCE_OUTPUT_COMPARE_5 = 27,
    INT_SOURCE_TIMER_6 = 76,
    INT_SOURCE_INPUT_CAPTURE_6_ERROR = 77,
    INT_SOURCE_INPUT_CAPTURE_6 = 78,
    INT_SOURCE_OUTPUT_COMPARE_6 = 79,
    INT_SOURCE_TIMER_7 = 80,
    INT_SOURCE_INPUT_CAPTURE_7_ERROR = 81,
    INT_SOURCE_INPUT_CAPTURE_7 = 82,
    INT_SOURCE_OUTPUT_COMPARE_7 = 83,
    INT_SOURCE_TIMER_8 = 84,
    INT_SOURCE_INPUT_CAPTURE_8_ERROR = 85,
    INT_SOURCE_INPUT_CAPTURE_8 = 86,
    INT_SOURCE_OUTPUT_COMPARE_8 = 87,
    INT_SOURCE_TIMER_9 = 88,
    INT_SOURCE_INPUT_CAPTURE_9_ERROR = 89,
    INT_SOURCE_INPUT_CAPTURE_9 = 90,
    INT_SOURCE_OUTPUT_COMPARE_9 = 91,
    INT_SOURCE_ADC_1 = 92,
    INT_SOURCE_ADC_1_DC1 = 94,
    INT_SOURCE_ADC_1_DC2 = 95,
    INT_SOURCE_ADC_1_DC3 = 245,
    INT_SOURCE_ADC_1_DC4 = 246,
    INT_SOURCE_ADC_1_DF1 = 96,
    INT_SOURCE_ADC_1_DF2 = 97,
    INT_SOURCE_ADC_1_DF3 = 98,
    INT_SOURCE_ADC_1_DF4 = 99,
    INT_SOURCE_ADC_FAULT = 100,
    INT_SOURCE_ADC_1_DATA0 = 106,
    INT_SOURCE_ADC_1_DATA1 = 107,
    INT_SOURCE_ADC_1_DATA2 = 108,
    INT_SOURCE_ADC_1_DATA3 = 109,
    INT_SOURCE_ADC_1_DATA4 = 110,
    INT_SOURCE_ADC_1_DATA5 = 111,
    INT_SOURCE_ADC_1_DATA6 = 112,
    INT_SOURCE_ADC_1_DATA7 = 113,
    INT_SOURCE_ADC_1_DATA8 = 114,
    INT_SOURCE_ADC_1_DATA9 = 115,
    INT_SOURCE_ADC_1_DATA10 = 116,
    INT_SOURCE_ADC_1_DATA11 = 117,
    INT_SOURCE_ADC_1_DATA12 = 118,
    INT_SOURCE_ADC_1_DATA13 = 119,
    INT_SOURCE_ADC_1_DATA14 = 120,
    INT_SOURCE_ADC_1_DATA15 = 121,
    INT_SOURCE_ADC_1_DATA16 = 122,
    INT_SOURCE_ADC_1_DATA17 = 123,
    INT_SOURCE_ADC_1_DATA18 = 124,
    INT_SOURCE_ADC_1_DATA19 = 125,
    INT_SOURCE_ADC_1_DATA20 = 126,
    INT_SOURCE_ADC_1_DATA21 = 127,
    INT_SOURCE_ADC_1_DATA22 = 128,
    INT_SOURCE_ADC_1_DATA23 = 129,
    INT_SOURCE_ADC_1_DATA24 = 130,
    INT_SOURCE_ADC_1_DATA25 = 131,
    INT_SOURCE_ADC_1_DATA26 = 132,
    INT_SOURCE_ADC_1_DATA27 = 133,
    INT_SOURCE_ADC_1_DATA33 = 139,
    INT_SOURCE_ADC_1_DATA34 = 140,
    INT_SOURCE_ADC_1_DATA35 = 141,
    INT_SOURCE_ADC_1_DATA36 = 142,
    INT_SOURCE_ADC_1_DATA37 = 143,
    INT_SOURCE_ADC_1_DATA38 = 144,
    INT_SOURCE_ADC_1_DATA39 = 145,
    INT_SOURCE_ADC_1_DATA40 = 146,
    INT_SOURCE_ADC_1_DATA41 = 147,
    INT_SOURCE_CORE_PERF_COUNT = 254,
    INT_SOURCE_FAST_DEBUG = 255,
    INT_SOURCE_SYSTEM_BUS_PROTECTION = 230,
    INT_SOURCE_SPI_1_ERROR = 35,
    INT_SOURCE_SPI_1_RECEIVE = 36,
    INT_SOURCE_SPI_1_TRANSMIT = 37,
    INT_SOURCE_USART_1_ERROR = 38,
    INT_SOURCE_USART_1_RECEIVE = 39,
    INT_SOURCE_USART_1_TRANSMIT = 40,
    INT_SOURCE_I2C_1_BUS = 41,
    INT_SOURCE_I2C_1_SLAVE = 42,
    INT_SOURCE_I2C_1_MASTER = 43,
    INT_SOURCE_CHANGE_NOTICE_A = 44,
    INT_SOURCE_CHANGE_NOTICE_B = 45,
    INT_SOURCE_CHANGE_NOTICE_C = 46,
    INT_SOURCE_CHANGE_NOTICE_D = 47,
    INT_SOURCE_CHANGE_NOTICE_E = 48,
    INT_SOURCE_CHANGE_NOTICE_F = 49,
    INT_SOURCE_CHANGE_NOTICE_G = 50,
    INT_SOURCE_PARALLEL_PORT = 51,
    INT_SOURCE_PARALLEL_PORT_ERROR = 52,
    INT_SOURCE_COMPARATOR_1 = 32,
    INT_SOURCE_COMPARATOR_2 = 33,
    INT_SOURCE_USB_1 = 34,
    INT_SOURCE_DMA_0 = 72,
    INT_SOURCE_DMA_1 = 73,
    INT_SOURCE_DMA_2 = 74,
    INT_SOURCE_DMA_3 = 75,
    INT_SOURCE_DMA_4 = 182,
    INT_SOURCE_DMA_5 = 183,
    INT_SOURCE_DMA_6 = 184,
    INT_SOURCE_DMA_7 = 185,
    INT_SOURCE_SPI_2_ERROR = 53,
    INT_SOURCE_SPI_2_RECEIVE = 54,
    INT_SOURCE_SPI_2_TRANSMIT = 55,
    INT_SOURCE_USART_2_ERROR = 56,
    INT_SOURCE_USART_2_RECEIVE = 57,
    INT_SOURCE_USART_2_TRANSMIT = 58,
    INT_SOURCE_I2C_2_BUS = 59,
    INT_SOURCE_I2C_2_SLAVE = 60,
    INT_SOURCE_I2C_2_MASTER = 61,
    INT_SOURCE_CAN_1 = 167,
    INT_SOURCE_CAN_2 = 168,
    INT_SOURCE_SPI_3_ERROR = 218,
    INT_SOURCE_SPI_3_RECEIVE = 219,
    INT_SOURCE_SPI_3_TRANSMIT = 220,
    INT_SOURCE_USART_3_ERROR = 62,
    INT_SOURCE_USART_3_RECEIVE = 63,
    INT_SOURCE_USART_3_TRANSMIT = 64,
    INT_SOURCE_I2C_3_BUS = 179,
    INT_SOURCE_I2C_3_SLAVE = 180,
    INT_SOURCE_I2C_3_MASTER = 181,
    INT_SOURCE_SPI_4_ERROR = 221,
    INT_SOURCE_SPI_4_RECEIVE = 222,
    INT_SOURCE_SPI_4_TRANSMIT = 223,
    INT_SOURCE_USART_4_ERROR = 65,
    INT_SOURCE_USART_4_RECEIVE = 66,
    INT_SOURCE_USART_4_TRANSMIT = 67,
    INT_SOURCE_I2C_4_BUS = 194,
    INT_SOURCE_I2C_4_SLAVE = 195,
    INT_SOURCE_SPI_5_ERROR = 224,
    INT_SOURCE_SPI_5_RECEIVE = 225,
    INT_SOURCE_SPI_5_TRANSMIT = 226,
    INT_SOURCE_USART_5_ERROR = 68,
    INT_SOURCE_USART_5_RECEIVE = 69,
    INT_SOURCE_USART_5_TRANSMIT = 70,
    INT_SOURCE_SPI_6_ERROR = 227,
    INT_SOURCE_SPI_6_RECEIVE = 228,
    INT_SOURCE_SPI_6_TRANSMIT = 229,
    INT_SOURCE_USART_6_ERROR = 164,
    INT_SOURCE_USART_6_RECEIVE = 165,
    INT_SOURCE_USART_6_TRANSMIT = 166,
    INT_SOURCE_RTCC = 30,
    INT_SOURCE_FLASH_CONTROL = 31,
    INT_SOURCE_COMPARATOR_3 = 160,
    INT_SOURCE_CTMU = 71,
    INT_SOURCE_I2C_4_MASTER = 196,
    INT_SOURCE_ADC_END_OF_SCAN = 101,
    INT_SOURCE_ADC_ANALOG_CIRCUIT_READY = 102,
    INT_SOURCE_ADC_UPDATE_READY = 103,
    INT_SOURCE_ADC_GROUP = 105,
    INT_SOURCE_DATA_EEPROM = 186,
    INT_SOURCE_ADC_DMA = 104,
    INT_SOURCE_ADC_1_DATA45 = 151,
    INT_SOURCE_ADC_1_DATA46 = 152,
    INT_SOURCE_ADC_1_DATA47 = 153,
    INT_SOURCE_ADC_1_DATA48 = 154,
    INT_SOURCE_ADC_1_DATA49 = 155,
    INT_SOURCE_ADC_1_DATA50 = 156,
    INT_SOURCE_ADC_1_DATA51 = 157,
    INT_SOURCE_ADC_1_DATA52 = 158,
    INT_SOURCE_ADC_1_DATA53 = 159,
    INT_SOURCE_COMPARATOR_4 = 161,
    INT_SOURCE_COMPARATOR_5 = 162,
    INT_SOURCE_CAN_3 = 187,
    INT_SOURCE_CAN_4 = 188,
    INT_SOURCE_USB_2 = 244,
    INT_SOURCE_INPUT_CAPTURE_10_ERROR = 197,
    INT_SOURCE_INPUT_CAPTURE_10 = 198,
    INT_SOURCE_OUTPUT_COMPARE_10 = 199,
    INT_SOURCE_INPUT_CAPTURE_11_ERROR = 200,
    INT_SOURCE_INPUT_CAPTURE_11 = 201,
    INT_SOURCE_OUTPUT_COMPARE_11 = 202,
    INT_SOURCE_INPUT_CAPTURE_12_ERROR = 203,
    INT_SOURCE_INPUT_CAPTURE_12 = 204,
    INT_SOURCE_OUTPUT_COMPARE_12 = 205,
    INT_SOURCE_INPUT_CAPTURE_13_ERROR = 206,
    INT_SOURCE_INPUT_CAPTURE_13 = 207,
    INT_SOURCE_OUTPUT_COMPARE_13 = 208,
    INT_SOURCE_INPUT_CAPTURE_14_ERROR = 209,
    INT_SOURCE_INPUT_CAPTURE_14 = 210,
    INT_SOURCE_OUTPUT_COMPARE_14 = 211,
    INT_SOURCE_INPUT_CAPTURE_15_ERROR = 212,
    INT_SOURCE_INPUT_CAPTURE_15 = 213,
    INT_SOURCE_OUTPUT_COMPARE_15 = 214,
    INT_SOURCE_INPUT_CAPTURE_16_ERROR = 215,
    INT_SOURCE_INPUT_CAPTURE_16 = 216,
    INT_SOURCE_OUTPUT_COMPARE_16 = 217,
    INT_SOURCE_QEI1 = 169,
    INT_SOURCE_QEI2 = 170,
    INT_SOURCE_QEI3 = 189,
    INT_SOURCE_QEI4 = 190,
    INT_SOURCE_QEI5 = 191,
    INT_SOURCE_QEI6 = 192,
    INT_SOURCE_PWM_PRIMARY = 171,
    INT_SOURCE_PWM_SECONDARY = 172,
    INT_SOURCE_PWM1 = 173,
    INT_SOURCE_PWM2 = 174,
    INT_SOURCE_PWM3 = 175,
    INT_SOURCE_PWM4 = 176,
    INT_SOURCE_PWM5 = 177,
    INT_SOURCE_PWM6 = 178,
	INT_SOURCE_PWM7 = 238,
    INT_SOURCE_PWM8 = 239,
    INT_SOURCE_PWM9 = 240,
    INT_SOURCE_PWM10 = 241,
    INT_SOURCE_PWM11 = 242,
    INT_SOURCE_PWM12 = 243

} INT_SOURCE;

typedef enum {

    INT_VECTOR_CT = _CORE_TIMER_VECTOR,
    INT_VECTOR_CS0 = _CORE_SOFTWARE_0_VECTOR,
    INT_VECTOR_CS1 = _CORE_SOFTWARE_1_VECTOR,
    INT_VECTOR_INT0 = _EXTERNAL_0_VECTOR,
    INT_VECTOR_T1 = _TIMER_1_VECTOR,
    INT_VECTOR_IC1 = _INPUT_CAPTURE_1_VECTOR,
    INT_VECTOR_IC1_ERROR = _INPUT_CAPTURE_1_ERROR_VECTOR,
    INT_VECTOR_OC1 = _OUTPUT_COMPARE_1_VECTOR,
    INT_VECTOR_INT1 = _EXTERNAL_1_VECTOR,
    INT_VECTOR_T2 = _TIMER_2_VECTOR,
    INT_VECTOR_IC2 = _INPUT_CAPTURE_2_VECTOR,
    INT_VECTOR_IC2_ERROR = _INPUT_CAPTURE_2_ERROR_VECTOR,
    INT_VECTOR_OC2 = _OUTPUT_COMPARE_2_VECTOR,
    INT_VECTOR_INT2 = _EXTERNAL_2_VECTOR,
    INT_VECTOR_T3 = _TIMER_3_VECTOR,
    INT_VECTOR_IC3 = _INPUT_CAPTURE_3_VECTOR,
    INT_VECTOR_IC3_ERROR = _INPUT_CAPTURE_3_ERROR_VECTOR,
    INT_VECTOR_OC3 = _OUTPUT_COMPARE_3_VECTOR,
    INT_VECTOR_INT3 = _EXTERNAL_3_VECTOR,
    INT_VECTOR_T4 = _TIMER_4_VECTOR,
    INT_VECTOR_IC4 = _INPUT_CAPTURE_4_VECTOR,
    INT_VECTOR_IC4_ERROR = _INPUT_CAPTURE_4_ERROR_VECTOR,
    INT_VECTOR_OC4 = _OUTPUT_COMPARE_4_VECTOR,
    INT_VECTOR_INT4 = _EXTERNAL_4_VECTOR,
    INT_VECTOR_T5 = _TIMER_5_VECTOR,
    INT_VECTOR_IC5 = _INPUT_CAPTURE_5_VECTOR,
    INT_VECTOR_IC5_ERROR = _INPUT_CAPTURE_5_ERROR_VECTOR,
    INT_VECTOR_OC5 = _OUTPUT_COMPARE_5_VECTOR,
    INT_VECTOR_T6 = _TIMER_6_VECTOR,
    INT_VECTOR_IC6_ERROR = _INPUT_CAPTURE_6_ERROR_VECTOR,
    INT_VECTOR_IC6 = _INPUT_CAPTURE_6_VECTOR,
    INT_VECTOR_OC6 = _OUTPUT_COMPARE_6_VECTOR,
    INT_VECTOR_T7 = _TIMER_7_VECTOR,
    INT_VECTOR_IC7_ERROR = _INPUT_CAPTURE_7_ERROR_VECTOR,
    INT_VECTOR_IC7 = _INPUT_CAPTURE_7_VECTOR,
    INT_VECTOR_OC7 = _OUTPUT_COMPARE_7_VECTOR,
    INT_VECTOR_T8 = _TIMER_8_VECTOR,
    INT_VECTOR_IC8_ERROR = _INPUT_CAPTURE_8_ERROR_VECTOR,
    INT_VECTOR_IC8 = _INPUT_CAPTURE_8_VECTOR,
    INT_VECTOR_OC8 = _OUTPUT_COMPARE_8_VECTOR,
    INT_VECTOR_T9 = _TIMER_9_VECTOR,
    INT_VECTOR_IC9_ERROR = _INPUT_CAPTURE_9_ERROR_VECTOR,
    INT_VECTOR_IC9 = _INPUT_CAPTURE_9_VECTOR,
    INT_VECTOR_OC9 = _OUTPUT_COMPARE_9_VECTOR,
    INT_VECTOR_ADC1 = _ADC_VECTOR,
    INT_VECTOR_ADC1_DC1 = _ADC_DC1_VECTOR,
    INT_VECTOR_ADC1_DC2 = _ADC_DC2_VECTOR,
    INT_VECTOR_ADC1_DC3 = _ADC_DC3_VECTOR,
    INT_VECTOR_ADC1_DC4 = _ADC_DC4_VECTOR,
    INT_VECTOR_ADC1_DF1 = _ADC_DF1_VECTOR,
    INT_VECTOR_ADC1_DF2 = _ADC_DF2_VECTOR,
    INT_VECTOR_ADC1_DF3 = _ADC_DF3_VECTOR,
    INT_VECTOR_ADC1_DF4 = _ADC_DF4_VECTOR,
    INT_VECTOR_ADC_FAULT = _ADC_FAULT_VECTOR,
    INT_VECTOR_ADC1_DATA0 = _ADC_DATA0_VECTOR,
    INT_VECTOR_ADC1_DATA1 = _ADC_DATA1_VECTOR,
    INT_VECTOR_ADC1_DATA2 = _ADC_DATA2_VECTOR,
    INT_VECTOR_ADC1_DATA3 = _ADC_DATA3_VECTOR,
    INT_VECTOR_ADC1_DATA4 = _ADC_DATA4_VECTOR,
    INT_VECTOR_ADC1_DATA5 = _ADC_DATA5_VECTOR,
    INT_VECTOR_ADC1_DATA6 = _ADC_DATA6_VECTOR,
    INT_VECTOR_ADC1_DATA7 = _ADC_DATA7_VECTOR,
    INT_VECTOR_ADC1_DATA8 = _ADC_DATA8_VECTOR,
    INT_VECTOR_ADC1_DATA9 = _ADC_DATA9_VECTOR,
    INT_VECTOR_ADC1_DATA10 = _ADC_DATA10_VECTOR,
    INT_VECTOR_ADC1_DATA11 = _ADC_DATA11_VECTOR,
    INT_VECTOR_ADC1_DATA12 = _ADC_DATA12_VECTOR,
    INT_VECTOR_ADC1_DATA13 = _ADC_DATA13_VECTOR,
    INT_VECTOR_ADC1_DATA14 = _ADC_DATA14_VECTOR,
    INT_VECTOR_ADC1_DATA15 = _ADC_DATA15_VECTOR,
    INT_VECTOR_ADC1_DATA16 = _ADC_DATA16_VECTOR,
    INT_VECTOR_ADC1_DATA17 = _ADC_DATA17_VECTOR,
    INT_VECTOR_ADC1_DATA18 = _ADC_DATA18_VECTOR,
    INT_VECTOR_ADC1_DATA19 = _ADC_DATA19_VECTOR,
    INT_VECTOR_ADC1_DATA20 = _ADC_DATA20_VECTOR,
    INT_VECTOR_ADC1_DATA21 = _ADC_DATA21_VECTOR,
    INT_VECTOR_ADC1_DATA22 = _ADC_DATA22_VECTOR,
    INT_VECTOR_ADC1_DATA23 = _ADC_DATA23_VECTOR,
    INT_VECTOR_ADC1_DATA24 = _ADC_DATA24_VECTOR,
    INT_VECTOR_ADC1_DATA25 = _ADC_DATA25_VECTOR,
    INT_VECTOR_ADC1_DATA26 = _ADC_DATA26_VECTOR,
    INT_VECTOR_ADC1_DATA27 = _ADC_DATA27_VECTOR,
    INT_VECTOR_ADC1_DATA33 = _ADC_DATA33_VECTOR,
    INT_VECTOR_ADC1_DATA34 = _ADC_DATA34_VECTOR,
    INT_VECTOR_ADC1_DATA35 = _ADC_DATA35_VECTOR,
    INT_VECTOR_ADC1_DATA36 = _ADC_DATA36_VECTOR,
    INT_VECTOR_ADC1_DATA37 = _ADC_DATA37_VECTOR,
    INT_VECTOR_ADC1_DATA38 = _ADC_DATA38_VECTOR,
    INT_VECTOR_ADC1_DATA39 = _ADC_DATA39_VECTOR,
    INT_VECTOR_ADC1_DATA40 = _ADC_DATA40_VECTOR,
    INT_VECTOR_ADC1_DATA41 = _ADC_DATA41_VECTOR,
    INT_VECTOR_CORE_PERF_COUNT = _CORE_PERF_COUNT_VECTOR,
    INT_VECTOR_CORE_FAST_DEBUG_CHANNEL = _CORE_FAST_DEBUG_CHAN_VECTOR,
    INT_VECTOR_CORE_SYSTEM_BUS_PROTECTION = _SYSTEM_BUS_PROTECTION_VECTOR,
    INT_VECTOR_SPI1_FAULT = _SPI1_FAULT_VECTOR,
    INT_VECTOR_SPI1_RX = _SPI1_RX_VECTOR,
    INT_VECTOR_SPI1_TX = _SPI1_TX_VECTOR,
    INT_VECTOR_UART1_FAULT = _UART1_FAULT_VECTOR,
    INT_VECTOR_UART1_RX = _UART1_RX_VECTOR,
    INT_VECTOR_UART1_TX = _UART1_TX_VECTOR,
    INT_VECTOR_I2C1_BUS = _I2C1_BUS_VECTOR,
    INT_VECTOR_I2C1_SLAVE = _I2C1_SLAVE_VECTOR,
    INT_VECTOR_I2C1_MASTER = _I2C1_MASTER_VECTOR,
    INT_VECTOR_SPI2_FAULT = _SPI2_FAULT_VECTOR,
    INT_VECTOR_SPI2_RX = _SPI2_RX_VECTOR,
    INT_VECTOR_SPI2_TX = _SPI2_TX_VECTOR,
    INT_VECTOR_UART2_FAULT = _UART2_FAULT_VECTOR,
    INT_VECTOR_UART2_RX = _UART2_RX_VECTOR,
    INT_VECTOR_UART2_TX = _UART2_TX_VECTOR,
    INT_VECTOR_I2C2_BUS = _I2C2_BUS_VECTOR,
    INT_VECTOR_I2C2_SLAVE = _I2C2_SLAVE_VECTOR,
    INT_VECTOR_I2C2_MASTER = _I2C2_MASTER_VECTOR,
    INT_VECTOR_SPI3_FAULT = _SPI3_FAULT_VECTOR,
    INT_VECTOR_SPI3_RX = _SPI3_RX_VECTOR,
    INT_VECTOR_SPI3_TX = _SPI3_TX_VECTOR,
    INT_VECTOR_UART3_FAULT = _UART3_FAULT_VECTOR,
    INT_VECTOR_UART3_RX = _UART3_RX_VECTOR,
    INT_VECTOR_UART3_TX = _UART3_TX_VECTOR,
    INT_VECTOR_I2C3_BUS = _I2C3_BUS_VECTOR,
    INT_VECTOR_I2C3_SLAVE = _I2C3_SLAVE_VECTOR,
    INT_VECTOR_I2C3_MASTER = _I2C3_MASTER_VECTOR,
    INT_VECTOR_SPI4_FAULT = _SPI4_FAULT_VECTOR,
    INT_VECTOR_SPI4_RX = _SPI4_RX_VECTOR,
    INT_VECTOR_SPI4_TX = _SPI4_TX_VECTOR,
    INT_VECTOR_UART4_FAULT = _UART4_FAULT_VECTOR,
    INT_VECTOR_UART4_RX = _UART4_RX_VECTOR,
    INT_VECTOR_UART4_TX = _UART4_TX_VECTOR,
    INT_VECTOR_I2C4_BUS = _I2C4_BUS_VECTOR,
    INT_VECTOR_I2C4_SLAVE = _I2C4_SLAVE_VECTOR,
    INT_VECTOR_I2C4_MASTER = _I2C4_MASTER_VECTOR,
    INT_VECTOR_SPI5_FAULT = _SPI5_FAULT_VECTOR,
    INT_VECTOR_SPI5_RX = _SPI5_RX_VECTOR,
    INT_VECTOR_SPI5_TX = _SPI5_TX_VECTOR,
    INT_VECTOR_UART5_FAULT = _UART5_FAULT_VECTOR,
    INT_VECTOR_UART5_RX = _UART5_RX_VECTOR,
    INT_VECTOR_UART5_TX = _UART5_TX_VECTOR,
    INT_VECTOR_SPI6_FAULT = _SPI6_FAULT_VECTOR,
    INT_VECTOR_SPI6_RX = _SPI6_RX_VECTOR,
    INT_VECTOR_SPI6_TX = _SPI6_TX_VECTOR,
    INT_VECTOR_UART6_FAULT = _UART6_FAULT_VECTOR,
    INT_VECTOR_UART6_RX = _UART6_RX_VECTOR,
    INT_VECTOR_UART6_TX = _UART6_TX_VECTOR,
    INT_VECTOR_CHANGE_NOTICE_A = _CHANGE_NOTICE_A_VECTOR,
    INT_VECTOR_CHANGE_NOTICE_B = _CHANGE_NOTICE_B_VECTOR,
    INT_VECTOR_CHANGE_NOTICE_C = _CHANGE_NOTICE_C_VECTOR,
    INT_VECTOR_CHANGE_NOTICE_D = _CHANGE_NOTICE_D_VECTOR,
    INT_VECTOR_CHANGE_NOTICE_E = _CHANGE_NOTICE_E_VECTOR,
    INT_VECTOR_CHANGE_NOTICE_F = _CHANGE_NOTICE_F_VECTOR,
    INT_VECTOR_CHANGE_NOTICE_G = _CHANGE_NOTICE_G_VECTOR,
    INT_VECTOR_PMP = _PMP_VECTOR,
    INT_VECTOR_PMP_ERROR = _PMP_ERROR_VECTOR,
    INT_VECTOR_USB1 = _USB_1_VECTOR,
    INT_VECTOR_RTCC = _RTCC_VECTOR,
    INT_VECTOR_FLASH = _FLASH_CONTROL_VECTOR,
    INT_VECTOR_CMP1 = _COMPARATOR_1_VECTOR,
    INT_VECTOR_CMP2 = _COMPARATOR_2_VECTOR,
    INT_VECTOR_CMP3 = _COMPARATOR_3_VECTOR,
    INT_VECTOR_CTMU = _CTMU_VECTOR,
    INT_VECTOR_DMA0 = _DMA0_VECTOR,
    INT_VECTOR_DMA1 = _DMA1_VECTOR,
    INT_VECTOR_DMA2 = _DMA2_VECTOR,
    INT_VECTOR_DMA3 = _DMA3_VECTOR,
    INT_VECTOR_DMA4 = _DMA4_VECTOR,
    INT_VECTOR_DMA5 = _DMA5_VECTOR,
    INT_VECTOR_DMA6 = _DMA6_VECTOR,
    INT_VECTOR_DMA7 = _DMA7_VECTOR,
    INT_VECTOR_CAN1 = _CAN1_VECTOR,
    INT_VECTOR_CAN2 = _CAN2_VECTOR,
    INT_VECTOR_ADC_END_OF_SCAN = _ADC_EOS_VECTOR,
    INT_VECTOR_ADC_ANALOG_CIRCUIT_READY = _ADC_ARDY_VECTOR,
    INT_VECTOR_ADC_UPDATE_READY = _ADC_URDY_VECTOR,
    INT_VECTOR_ADC_GROUP = _ADC_EARLY_VECTOR,
    INT_VECTOR_ADC_DMA = _ADC_DMA_VECTOR,
    INT_VECTOR_DATA_EEPROM = _DATA_EE_VECTOR,
    INT_VECTOR_ADC1_DATA45 = _ADC_DATA45_VECTOR,
    INT_VECTOR_ADC1_DATA46 = _ADC_DATA46_VECTOR,
    INT_VECTOR_ADC1_DATA47 = _ADC_DATA47_VECTOR,
    INT_VECTOR_ADC1_DATA48 = _ADC_DATA48_VECTOR,
    INT_VECTOR_ADC1_DATA49 = _ADC_DATA49_VECTOR,
    INT_VECTOR_ADC1_DATA50 = _ADC_DATA50_VECTOR,
    INT_VECTOR_ADC1_DATA51 = _ADC_DATA51_VECTOR,
    INT_VECTOR_ADC1_DATA52 = _ADC_DATA52_VECTOR,
    INT_VECTOR_ADC1_DATA53 = _ADC_DATA53_VECTOR,
    INT_VECTOR_CMP4 = _COMPARATOR_4_VECTOR,
    INT_VECTOR_CMP5 = _COMPARATOR_5_VECTOR,
    INT_VECTOR_CAN3 = _CAN3_VECTOR,
    INT_VECTOR_CAN4 = _CAN4_VECTOR,
    INT_VECTOR_USB2 = _USB_2_VECTOR,
    INT_VECTOR_IC10_ERROR = _INPUT_CAPTURE_10_ERROR_VECTOR,
    INT_VECTOR_IC10 = _INPUT_CAPTURE_10_VECTOR,
    INT_VECTOR_OC10 = _OUTPUT_COMPARE_10_VECTOR,
    INT_VECTOR_IC11_ERROR = _INPUT_CAPTURE_11_ERROR_VECTOR,
    INT_VECTOR_IC11 = _INPUT_CAPTURE_11_VECTOR,
    INT_VECTOR_OC11 = _OUTPUT_COMPARE_11_VECTOR,
    INT_VECTOR_IC12_ERROR = _INPUT_CAPTURE_12_ERROR_VECTOR,
    INT_VECTOR_IC12 = _INPUT_CAPTURE_12_VECTOR,
    INT_VECTOR_OC12 = _OUTPUT_COMPARE_12_VECTOR,
    INT_VECTOR_IC13_ERROR = _INPUT_CAPTURE_13_ERROR_VECTOR,
    INT_VECTOR_IC13 = _INPUT_CAPTURE_13_VECTOR,
    INT_VECTOR_OC13 = _OUTPUT_COMPARE_13_VECTOR,
    INT_VECTOR_IC14_ERROR = _INPUT_CAPTURE_14_ERROR_VECTOR,
    INT_VECTOR_IC14 = _INPUT_CAPTURE_14_VECTOR,
    INT_VECTOR_OC14 = _OUTPUT_COMPARE_14_VECTOR,
    INT_VECTOR_IC15_ERROR = _INPUT_CAPTURE_15_ERROR_VECTOR,
    INT_VECTOR_IC15 = _INPUT_CAPTURE_15_VECTOR,
    INT_VECTOR_OC15 = _OUTPUT_COMPARE_15_VECTOR,
    INT_VECTOR_IC16_ERROR = _INPUT_CAPTURE_16_ERROR_VECTOR,
    INT_VECTOR_IC16 = _INPUT_CAPTURE_16_VECTOR,
    INT_VECTOR_OC16 = _OUTPUT_COMPARE_16_VECTOR,
    INT_VECTOR_QEI1 = _QEI1_VECTOR,
    INT_VECTOR_QEI2 = _QEI2_VECTOR,
    INT_VECTOR_QEI3 = _QEI3_VECTOR,
    INT_VECTOR_QEI4 = _QEI4_VECTOR,
    INT_VECTOR_QEI5 = _QEI5_VECTOR,
    INT_VECTOR_QEI6 = _QEI6_VECTOR,
    INT_VECTOR_PWM_PRIMARY = _PWM_PRI_VECTOR,
    INT_VECTOR_PWM_SECONDARY = _PWM_SEC_VECTOR,
    INT_VECTOR_PWM1 = _PWM1_VECTOR,
    INT_VECTOR_PWM2 = _PWM2_VECTOR,
    INT_VECTOR_PWM3 = _PWM3_VECTOR,
    INT_VECTOR_PWM4 = _PWM4_VECTOR,
    INT_VECTOR_PWM5 = _PWM5_VECTOR,
    INT_VECTOR_PWM6 = _PWM6_VECTOR,
	INT_VECTOR_PWM7 = _PWM7_VECTOR,
    INT_VECTOR_PWM8 = _PWM8_VECTOR,
    INT_VECTOR_PWM9 = _PWM9_VECTOR,
    INT_VECTOR_PWM10 = _PWM10_VECTOR,
    INT_VECTOR_PWM11 = _PWM11_VECTOR,
    INT_VECTOR_PWM12 = _PWM12_VECTOR

} INT_VECTOR;

typedef enum {

    INT_VECTOR_SPACING_0 = 0x00,
    INT_VECTOR_SPACING_8 = 0x01,
    INT_VECTOR_SPACING_16 = 0x02,
    INT_VECTOR_SPACING_32 = 0x04,
    INT_VECTOR_SPACING_64 = 0x08,
    INT_VECTOR_SPACING_128 = 0x10,
    INT_VECTOR_SPACING_256 = 0x20,
    INT_VECTOR_SPACING_512 = 0x40

} INT_VECTOR_SPACING;

typedef enum {

    INT_SHADOW_REGISTER_0 = 0x00,
    INT_SHADOW_REGISTER_1 = 0x01

} INT_SHADOW_REGISTER;

/* Section 2 - Feature variant inclusion */

#define PLIB_TEMPLATE PLIB_INLINE
#include "../templates/int_SingleVectorShadowSet_Default.h"
#include "../templates/int_VectorSelect_Default.h"
#include "../templates/int_ProximityTimerEnable_Default.h"
#include "../templates/int_ProximityTimerControl_Default.h"
#include "../templates/int_ExternalINTEdgeSelect_Default.h"
#include "../templates/int_INTCPUPriority_Default.h"
#include "../templates/int_INTCPUVector_Default.h"
#include "../templates/int_SourceFlag_Default.h"
#include "../templates/int_SourceControl_Default.h"
#include "../templates/int_VectorPriority_Default.h"
#include "../templates/int_CPUCurrentPriorityLevel_Default.h"
#include "../templates/int_EnableControl_PIC32.h"
#include "../templates/int_ShadowRegisterAssign_Default.h"
#include "../templates/int_VariableOffset_Default.h"
#include "../templates/int_SoftwareNMI_Default.h"

/* Section 3 - PLIB dispatch function definitions */

PLIB_INLINE_API bool PLIB_INT_ExistsSingleVectorShadowSet(INT_MODULE_ID index)
{
     return INT_ExistsSingleVectorShadowSet_Default(index);
}

PLIB_INLINE_API void PLIB_INT_SingleVectorShadowSetDisable(INT_MODULE_ID index)
{
     INT_SingleVectorShadowSetDisable_Default(index);
}

PLIB_INLINE_API void PLIB_INT_SingleVectorShadowSetEnable(INT_MODULE_ID index)
{
     INT_SingleVectorShadowSetEnable_Default(index);
}

PLIB_INLINE_API bool PLIB_INT_ExistsVectorSelect(INT_MODULE_ID index)
{
     return INT_ExistsVectorSelect_Default(index);
}

PLIB_INLINE_API void PLIB_INT_MultiVectorSelect(INT_MODULE_ID index)
{
     INT_MultiVectorSelect_Default(index);
}

PLIB_INLINE_API void PLIB_INT_SingleVectorSelect(INT_MODULE_ID index)
{
     INT_SingleVectorSelect_Default(index);
}

PLIB_INLINE_API bool PLIB_INT_ExistsProximityTimerEnable(INT_MODULE_ID index)
{
     return INT_ExistsProximityTimerEnable_Default(index);
}

PLIB_INLINE_API void PLIB_INT_ProximityTimerEnable(INT_MODULE_ID index, INT_PRIORITY_LEVEL priority)
{
     INT_ProximityTimerEnable_Default(index, priority);
}

PLIB_INLINE_API void PLIB_INT_ProximityTimerDisable(INT_MODULE_ID index)
{
     INT_ProximityTimerDisable_Default(index);
}

PLIB_INLINE_API bool PLIB_INT_ExistsProximityTimerControl(INT_MODULE_ID index)
{
     return INT_ExistsProximityTimerControl_Default(index);
}

PLIB_INLINE_API void PLIB_INT_ProximityTimerSet(INT_MODULE_ID index, uint32_t timerreloadvalue)
{
     INT_ProximityTimerSet_Default(index, timerreloadvalue);
}

PLIB_INLINE_API uint32_t PLIB_INT_ProximityTimerGet(INT_MODULE_ID index)
{
     return INT_ProximityTimerGet_Default(index);
}

PLIB_INLINE_API bool PLIB_INT_ExistsExternalINTEdgeSelect(INT_MODULE_ID index)
{
     return INT_ExistsExternalINTEdgeSelect_Default(index);
}

PLIB_INLINE_API void PLIB_INT_ExternalRisingEdgeSelect(INT_MODULE_ID index, INT_EXTERNAL_SOURCES source)
{
     INT_ExternalRisingEdgeSelect_Default(index, source);
}

PLIB_INLINE_API void PLIB_INT_ExternalFallingEdgeSelect(INT_MODULE_ID index, INT_EXTERNAL_SOURCES source)
{
     INT_ExternalFallingEdgeSelect_Default(index, source);
}

PLIB_INLINE_API bool PLIB_INT_ExistsINTCPUPriority(INT_MODULE_ID index)
{
     return INT_ExistsINTCPUPriority_Default(index);
}

PLIB_INLINE_API INT_PRIORITY_LEVEL PLIB_INT_PriorityGet(INT_MODULE_ID index)
{
     return INT_PriorityGet_Default(index);
}

PLIB_INLINE_API bool PLIB_INT_ExistsINTCPUVector(INT_MODULE_ID index)
{
     return INT_ExistsINTCPUVector_Default(index);
}

PLIB_INLINE_API INT_VECTOR PLIB_INT_VectorGet(INT_MODULE_ID index)
{
     return INT_VectorGet_Default(index);
}

PLIB_INLINE_API bool PLIB_INT_ExistsSourceFlag(INT_MODULE_ID index)
{
     return INT_ExistsSourceFlag_Default(index);
}

PLIB_INLINE_API bool PLIB_INT_SourceFlagGet(INT_MODULE_ID index, INT_SOURCE source)
{
     return INT_SourceFlagGet_Default(index, source);
}

PLIB_INLINE_API void PLIB_INT_SourceFlagSet(INT_MODULE_ID index, INT_SOURCE source)
{
     INT_SourceFlagSet_Default(index, source);
}

PLIB_INLINE_API void PLIB_INT_SourceFlagClear(INT_MODULE_ID index, INT_SOURCE source)
{
     INT_SourceFlagClear_Default(index, source);
}

PLIB_INLINE_API bool PLIB_INT_ExistsSourceControl(INT_MODULE_ID index)
{
     return INT_ExistsSourceControl_Default(index);
}

PLIB_INLINE_API void PLIB_INT_SourceEnable(INT_MODULE_ID index, INT_SOURCE source)
{
     INT_SourceEnable_Default(index, source);
}

PLIB_INLINE_API void PLIB_INT_SourceDisable(INT_MODULE_ID index, INT_SOURCE source)
{
     INT_SourceDisable_Default(index, source);
}

PLIB_INLINE_API bool PLIB_INT_SourceIsEnabled(INT_MODULE_ID index, INT_SOURCE source)
{
     return INT_SourceIsEnabled_Default(index, source);
}

PLIB_INLINE_API bool PLIB_INT_ExistsVectorPriority(INT_MODULE_ID index)
{
     return INT_ExistsVectorPriority_Default(index);
}

PLIB_INLINE_API void PLIB_INT_VectorPrioritySet(INT_MODULE_ID index, INT_VECTOR vector, INT_PRIORITY_LEVEL priority)
{
     INT_VectorPrioritySet_Default(index, vector, priority);
}

PLIB_INLINE_API INT_PRIORITY_LEVEL PLIB_INT_VectorPriorityGet(INT_MODULE_ID index, INT_VECTOR vector)
{
     return INT_VectorPriorityGet_Default(index, vector);
}

PLIB_INLINE_API void PLIB_INT_VectorSubPrioritySet(INT_MODULE_ID index, INT_VECTOR vector, INT_SUBPRIORITY_LEVEL subPriority)
{
     INT_VectorSubPrioritySet_Default(index, vector, subPriority);
}

PLIB_INLINE_API INT_SUBPRIORITY_LEVEL PLIB_INT_VectorSubPriorityGet(INT_MODULE_ID index, INT_VECTOR vector)
{
     return INT_VectorSubPriorityGet_Default(index, vector);
}

PLIB_INLINE_API bool PLIB_INT_ExistsCPUCurrentPriorityLevel(INT_MODULE_ID index)
{
     return INT_ExistsCPUCurrentPriorityLevel_Default(index);
}

PLIB_INLINE_API INT_PRIORITY_LEVEL PLIB_INT_CPUCurrentPriorityLevelGet(INT_MODULE_ID index)
{
     return INT_CPUCurrentPriorityLevelGet_Default(index);
}

PLIB_INLINE_API bool PLIB_INT_ExistsEnableControl(INT_MODULE_ID index)
{
     return INT_ExistsEnableControl_PIC32(index);
}

PLIB_INLINE_API void PLIB_INT_Enable(INT_MODULE_ID index)
{
     INT_Enable_PIC32(index);
}

PLIB_INLINE_API void PLIB_INT_Disable(INT_MODULE_ID index)
{
     INT_Disable_PIC32(index);
}

PLIB_INLINE_API bool PLIB_INT_IsEnabled(INT_MODULE_ID index)
{
     return INT_IsEnabled_PIC32(index);
}

PLIB_INLINE_API void PLIB_INT_SetState(INT_MODULE_ID index, INT_STATE_GLOBAL interrupt_state)
{
     INT_SetState_PIC32(index, interrupt_state);
}

PLIB_INLINE_API INT_STATE_GLOBAL PLIB_INT_GetStateAndDisable(INT_MODULE_ID index)
{
     return INT_GetStateAndDisable_PIC32(index);
}

PLIB_INLINE_API bool PLIB_INT_ExistsShadowRegisterAssign(INT_MODULE_ID index)
{
     return INT_ExistsShadowRegisterAssign_Default(index);
}

PLIB_INLINE_API void PLIB_INT_ShadowRegisterAssign(INT_MODULE_ID index, INT_PRIORITY_LEVEL priority, INT_SHADOW_REGISTER shadowRegister)
{
     INT_ShadowRegisterAssign_Default(index, priority, shadowRegister);
}

PLIB_INLINE_API INT_SHADOW_REGISTER PLIB_INT_ShadowRegisterGet(INT_MODULE_ID index, INT_PRIORITY_LEVEL priority)
{
     return INT_ShadowRegisterGet_Default(index, priority);
}

PLIB_INLINE_API bool PLIB_INT_ExistsVariableOffset(INT_MODULE_ID index)
{
     return INT_ExistsVariableOffset_Default(index);
}

PLIB_INLINE_API void PLIB_INT_VariableVectorOffsetSet(INT_MODULE_ID index, INT_VECTOR vector, uint32_t offset)
{
     INT_VariableVectorOffsetSet_Default(index, vector, offset);
}

PLIB_INLINE_API uint32_t PLIB_INT_VariableVectorOffsetGet(INT_MODULE_ID index, INT_VECTOR vector)
{
     return INT_VariableVectorOffsetGet_Default(index, vector);
}

PLIB_INLINE_API bool PLIB_INT_ExistsSoftwareNMI(INT_MODULE_ID index)
{
     return INT_ExistsSoftwareNMI_Default(index);
}

PLIB_INLINE_API void PLIB_INT_SoftwareNMITrigger(INT_MODULE_ID index)
{
     INT_SoftwareNMITrigger_Default(index);
}

#endif
