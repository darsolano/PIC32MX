/* Created by plibgen $Revision: 1.31 $ */

#ifndef _DMA_P32MX360F512L_H
#define _DMA_P32MX360F512L_H

/* Section 1 - Enumerate instances, define constants, VREGs */

#include <xc.h>
#include <stdbool.h>

#include "peripheral_common_32bit.h"

/* Default definition used for all API dispatch functions */
#ifndef PLIB_INLINE_API
    #define PLIB_INLINE_API extern inline
#endif

/* Default definition used for all other functions */
#ifndef PLIB_INLINE
    #define PLIB_INLINE extern inline
#endif

typedef enum {

    DMA_ID_0 = _DMAC_BASE_ADDRESS,
    DMA_NUMBER_OF_MODULES = 1

} DMA_MODULE_ID;

typedef enum {

    DMA_CHANNEL_0 = 0x0,
    DMA_CHANNEL_1 = 0x1,
    DMA_CHANNEL_2 = 0x2,
    DMA_CHANNEL_3 = 0x3,
    DMA_NUMBER_OF_CHANNELS = 0x4

} DMA_CHANNEL;

typedef enum {

    DMA_CHANNEL_0_INT_SOURCE = 48,
    DMA_CHANNEL_1_INT_SOURCE = 49,
    DMA_CHANNEL_2_INT_SOURCE = 50,
    DMA_CHANNEL_3_INT_SOURCE = 51

} DMA_CHANNEL_INT_SOURCE;

typedef enum {

    DMA_CHANNEL_PRIORITY_0 = 0x0,
    DMA_CHANNEL_PRIORITY_1 = 0x1,
    DMA_CHANNEL_PRIORITY_2 = 0x2,
    DMA_CHANNEL_PRIORITY_3 = 0x3

} DMA_CHANNEL_PRIORITY;

typedef enum {

    DMA_CHANNEL_TRIGGER_TRANSFER_START = 0x0,
    DMA_CHANNEL_TRIGGER_TRANSFER_ABORT = 0x1,
    DMA_CHANNEL_TRIGGER_PATTERN_MATCH_ABORT = 0x2

} DMA_CHANNEL_TRIGGER_TYPE;

typedef enum {

    DMA_TRIGGER_SOURCE_NONE = -1,
    DMA_TRIGGER_TIMER_CORE = 0,
    DMA_TRIGGER_SOFTWARE_0 = 1,
    DMA_TRIGGER_SOFTWARE_1 = 2,
    DMA_TRIGGER_EXTERNAL_0 = 3,
    DMA_TRIGGER_EXTERNAL_1 = 7,
    DMA_TRIGGER_EXTERNAL_2 = 11,
    DMA_TRIGGER_EXTERNAL_3 = 15,
    DMA_TRIGGER_EXTERNAL_4 = 19,
    DMA_TRIGGER_TIMER_1 = 4,
    DMA_TRIGGER_TIMER_2 = 8,
    DMA_TRIGGER_TIMER_3 = 12,
    DMA_TRIGGER_TIMER_4 = 16,
    DMA_TRIGGER_TIMER_5 = 20,
    DMA_TRIGGER_INPUT_CAPTURE_1 = 5,
    DMA_TRIGGER_INPUT_CAPTURE_2 = 9,
    DMA_TRIGGER_INPUT_CAPTURE_3 = 13,
    DMA_TRIGGER_INPUT_CAPTURE_4 = 17,
    DMA_TRIGGER_INPUT_CAPTURE_5 = 21,
    DMA_TRIGGER_OUTPUT_COMPARE_1 = 6,
    DMA_TRIGGER_OUTPUT_COMPARE_2 = 10,
    DMA_TRIGGER_OUTPUT_COMPARE_3 = 14,
    DMA_TRIGGER_OUTPUT_COMPARE_4 = 18,
    DMA_TRIGGER_OUTPUT_COMPARE_5 = 22,
    DMA_TRIGGER_SPI_1_ERROR = 23,
    DMA_TRIGGER_SPI_1_RECEIVE = 25,
    DMA_TRIGGER_SPI_1_TRANSMIT = 24,
    DMA_TRIGGER_SPI_2_ERROR = 37,
    DMA_TRIGGER_SPI_2_RECEIVE = 39,
    DMA_TRIGGER_SPI_2_TRANSMIT = 38,
    DMA_TRIGGER_I2C_1_ERROR = 29,
    DMA_TRIGGER_I2C_1_SLAVE = 30,
    DMA_TRIGGER_I2C_1_MASTER = 31,
    DMA_TRIGGER_I2C_2_ERROR = 43,
    DMA_TRIGGER_I2C_2_SLAVE = 44,
    DMA_TRIGGER_I2C_2_MASTER = 45,
    DMA_TRIGGER_USART_1_ERROR = 26,
    DMA_TRIGGER_USART_1_RECEIVE = 27,
    DMA_TRIGGER_USART_1_TRANSMIT = 28,
    DMA_TRIGGER_USART_2_ERROR = 40,
    DMA_TRIGGER_USART_2_RECEIVE = 41,
    DMA_TRIGGER_USART_2_TRANSMIT = 42,
    DMA_TRIGGER_CHANGE_NOTICE = 32,
    DMA_TRIGGER_DMA_0 = 48,
    DMA_TRIGGER_DMA_1 = 49,
    DMA_TRIGGER_DMA_2 = 50,
    DMA_TRIGGER_DMA_3 = 51,
    DMA_TRIGGER_COMPARATOR_1 = 35,
    DMA_TRIGGER_COMPARATOR_2 = 36,
    DMA_TRIGGER_ADC_1 = 33,
    DMA_TRIGGER_PARALLEL_PORT = 34,
    DMA_TRIGGER_CLOCK_MONITOR = 46,
    DMA_TRIGGER_RTCC = 47,
    DMA_TRIGGER_FLASH_CONTROL = 56

} DMA_TRIGGER_SOURCE;

typedef enum {

    DMA_CRC_IP_HEADER = 0x1,
    DMA_CRC_LFSR = 0x0

} DMA_CRC_TYPE;

typedef enum {

    DMA_CRC_BYTEORDER_NO_SWAPPING = 0x0,
    DMA_CRC_SWAP_BYTE_ON_WORD_BOUNDARY = 0x1,
    DMA_CRC_SWAP_HALF_WORD_ON_WORD_BOUNDARY = 0x2,
    DMA_CRC_SWAP_BYTE_ON_HALF_WORD_BOUNDARY = 0x3

} DMA_CRC_BYTE_ORDER;

typedef enum {

    DMA_INT_ADDRESS_ERROR = 0x1,
    DMA_INT_TRANSFER_ABORT = 0x2,
    DMA_INT_CELL_TRANSFER_COMPLETE = 0x4,
    DMA_INT_BLOCK_TRANSFER_COMPLETE = 0x8,
    DMA_INT_DESTINATION_HALF_FULL = 0x10,
    DMA_INT_DESTINATION_DONE = 0x20,
    DMA_INT_SOURCE_HALF_EMPTY = 0x40,
    DMA_INT_SOURCE_DONE = 0x80

} DMA_INT_TYPE;

typedef enum {

    DMA_CRC_BIT_ORDER_LSB = 0x1,
    DMA_CRC_BIT_ORDER_MSB = 0x0

} DMA_CRC_BIT_ORDER;

typedef enum {

    DMA_PATTERN_LENGTH_NONE

} DMA_PATTERN_LENGTH;

typedef enum {

    DMA_CHANNEL_COLLISION_NOT_SUPPORTED = 0x0

} DMA_CHANNEL_COLLISION;

typedef enum {

    DMA_PING_PONG_MODE_NOT_SUPPORTED = 0x0

} DMA_PING_PONG_MODE;

typedef enum {

    DMA_CHANNEL_TRANSFER_DIRECTION_NOT_SUPPORTED = 0

} DMA_CHANNEL_TRANSFER_DIRECTION;

typedef enum {

    DMA_ADDRESS_OFFSET_TYPE_NOT_SUPPORTED = 0

} DMA_ADDRESS_OFFSET_TYPE;

typedef enum {

    DMA_SOURCE_ADDRESSING_MODE_NOT_SUPPORTED = 0

} DMA_SOURCE_ADDRESSING_MODE;

typedef enum {

    DMA_DESTINATION_ADDRESSING_MODE_NOT_SUPPORTED = 0

} DMA_DESTINATION_ADDRESSING_MODE;

typedef enum {

    DMA_CHANNEL_ADDRESSING_MODE_NOT_SUPPORTED = 0

} DMA_CHANNEL_ADDRESSING_MODE;

typedef enum {

    DMA_CHANNEL_DATA_SIZE_NOT_SUPPORTED = 0

} DMA_CHANNEL_DATA_SIZE;

typedef enum {

    DMA_TRANSFER_MODE_NOT_SUPPORTED = 0

} DMA_TRANSFER_MODE;

/* Section 2 - Feature variant inclusion */

#define PLIB_TEMPLATE PLIB_INLINE
#include "../templates/dma_Busy_Unsupported.h"
#include "../templates/dma_Suspend_Default.h"
#include "../templates/dma_StopInIdle_Default.h"
#include "../templates/dma_EnableControl_Default.h"
#include "../templates/dma_ChannelBits_Default.h"
#include "../templates/dma_LastBusAccess_Default.h"
#include "../templates/dma_RecentAddress_Default.h"
#include "../templates/dma_CRCChannel_Default.h"
#include "../templates/dma_CRCType_Unsupported.h"
#include "../templates/dma_CRCAppendMode_Default.h"
#include "../templates/dma_Crc_Default.h"
#include "../templates/dma_CRCPolynomialLength_Default.h"
#include "../templates/dma_CRCBitOrder_Unsupported.h"
#include "../templates/dma_CRCWriteByteOrder_Unsupported.h"
#include "../templates/dma_CRCByteOrder_Unsupported.h"
#include "../templates/dma_CRCData_Default.h"
#include "../templates/dma_CRCXOREnable_Default.h"
#include "../templates/dma_ChannelXPriority_Default.h"
#include "../templates/dma_ChannelXEvent_Default.h"
#include "../templates/dma_ChannelXAuto_Default.h"
#include "../templates/dma_ChannelXChainEnbl_Default.h"
#include "../templates/dma_ChannelXDisabled_Default.h"
#include "../templates/dma_ChannelX_Default.h"
#include "../templates/dma_ChannelXChain_Default.h"
#include "../templates/dma_ChannelXPatternLength_Unsupported.h"
#include "../templates/dma_ChannelXPatternIgnoreByte_Unsupported.h"
#include "../templates/dma_ChannelXBusy_Unsupported.h"
#include "../templates/dma_ChannelXPatternIgnore_Unsupported.h"
#include "../templates/dma_ChannelXTrigger_Default.h"
#include "../templates/dma_AbortTransfer_Default.h"
#include "../templates/dma_StartTransfer_Default.h"
#include "../templates/dma_ChannelXStartIRQ_Default.h"
#include "../templates/dma_ChannelXAbortIRQ_Default.h"
#include "../templates/dma_ChannelXINTSourceFlag_Default.h"
#include "../templates/dma_ChannelXINTSource_Default.h"
#include "../templates/dma_ChannelXSourceStartAddress_Default.h"
#include "../templates/dma_ChannelXDestinationStartAddress_Default.h"
#include "../templates/dma_ChannelXSourceSize_Default.h"
#include "../templates/dma_ChannelXDestinationSize_Default.h"
#include "../templates/dma_ChannelXSourcePointer_Default.h"
#include "../templates/dma_ChannelXDestinationPointer_Default.h"
#include "../templates/dma_ChannelXCellSize_Default.h"
#include "../templates/dma_ChannelXCellProgressPointer_Default.h"
#include "../templates/dma_ChannelXPatternData_Default.h"

/* Section 3 - PLIB dispatch function definitions */

PLIB_INLINE_API bool PLIB_DMA_ExistsBusy(DMA_MODULE_ID index)
{
     return DMA_ExistsBusy_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_BusyActiveSet(DMA_MODULE_ID index)
{
     DMA_BusyActiveSet_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_BusyActiveReset(DMA_MODULE_ID index)
{
     DMA_BusyActiveReset_Unsupported(index);
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_DMA_IsBusy(DMA_MODULE_ID index)
{
     return DMA_IsBusy_Unsupported(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsSuspend(DMA_MODULE_ID index)
{
     return DMA_ExistsSuspend_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_SuspendEnable(DMA_MODULE_ID index)
{
     DMA_SuspendEnable_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_SuspendDisable(DMA_MODULE_ID index)
{
     DMA_SuspendDisable_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_SuspendIsEnabled(DMA_MODULE_ID index)
{
     return DMA_SuspendIsEnabled_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsStopInIdle(DMA_MODULE_ID index)
{
     return DMA_ExistsStopInIdle_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_StopInIdleEnable(DMA_MODULE_ID index)
{
     DMA_StopInIdleEnable_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_StopInIdleDisable(DMA_MODULE_ID index)
{
     DMA_StopInIdleDisable_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsEnableControl(DMA_MODULE_ID index)
{
     return DMA_ExistsEnableControl_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_Enable(DMA_MODULE_ID index)
{
     DMA_Enable_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_Disable(DMA_MODULE_ID index)
{
     DMA_Disable_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_IsEnabled(DMA_MODULE_ID index)
{
     return DMA_IsEnabled_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelBits(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelBits_Default(index);
}

PLIB_INLINE_API uint8_t PLIB_DMA_ChannelBitsGet(DMA_MODULE_ID index)
{
     return DMA_ChannelBitsGet_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsLastBusAccess(DMA_MODULE_ID index)
{
     return DMA_ExistsLastBusAccess_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_LastBusAccessIsRead(DMA_MODULE_ID index)
{
     return DMA_LastBusAccessIsRead_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_LastBusAccessIsWrite(DMA_MODULE_ID index)
{
     return DMA_LastBusAccessIsWrite_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsRecentAddress(DMA_MODULE_ID index)
{
     return DMA_ExistsRecentAddress_Default(index);
}

PLIB_INLINE_API uint32_t PLIB_DMA_RecentAddressAccessed(DMA_MODULE_ID index)
{
     return DMA_RecentAddressAccessed_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRCChannel(DMA_MODULE_ID index)
{
     return DMA_ExistsCRCChannel_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_CRCChannelSelect(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_CRCChannelSelect_Default(index, channel);
}

PLIB_INLINE_API DMA_CHANNEL PLIB_DMA_CRCChannelGet(DMA_MODULE_ID index)
{
     return DMA_CRCChannelGet_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRCType(DMA_MODULE_ID index)
{
     return DMA_ExistsCRCType_Unsupported(index);
}

PLIB_INLINE_API DMA_CRC_TYPE _PLIB_UNSUPPORTED PLIB_DMA_CRCTypeGet(DMA_MODULE_ID index)
{
     return DMA_CRCTypeGet_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_CRCTypeSet(DMA_MODULE_ID index, DMA_CRC_TYPE CRCType)
{
     DMA_CRCTypeSet_Unsupported(index, CRCType);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRCAppendMode(DMA_MODULE_ID index)
{
     return DMA_ExistsCRCAppendMode_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_CRCAppendModeEnable(DMA_MODULE_ID index)
{
     DMA_CRCAppendModeEnable_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_CRCAppendModeDisable(DMA_MODULE_ID index)
{
     DMA_CRCAppendModeDisable_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_CRCAppendModeIsEnabled(DMA_MODULE_ID index)
{
     return DMA_CRCAppendModeIsEnabled_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRC(DMA_MODULE_ID index)
{
     return DMA_ExistsCRC_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_CRCEnable(DMA_MODULE_ID index)
{
     DMA_CRCEnable_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_CRCDisable(DMA_MODULE_ID index)
{
     DMA_CRCDisable_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_CRCIsEnabled(DMA_MODULE_ID index)
{
     return DMA_CRCIsEnabled_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRCPolynomialLength(DMA_MODULE_ID index)
{
     return DMA_ExistsCRCPolynomialLength_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_CRCPolynomialLengthSet(DMA_MODULE_ID index, uint8_t polyLength)
{
     DMA_CRCPolynomialLengthSet_Default(index, polyLength);
}

PLIB_INLINE_API uint8_t PLIB_DMA_CRCPolynomialLengthGet(DMA_MODULE_ID index)
{
     return DMA_CRCPolynomialLengthGet_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRCBitOrder(DMA_MODULE_ID index)
{
     return DMA_ExistsCRCBitOrder_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_CRCBitOrderSelect(DMA_MODULE_ID index, DMA_CRC_BIT_ORDER bitOrder)
{
     DMA_CRCBitOrderSelect_Unsupported(index, bitOrder);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRCWriteByteOrder(DMA_MODULE_ID index)
{
     return DMA_ExistsCRCWriteByteOrder_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_CRCWriteByteOrderAlter(DMA_MODULE_ID index)
{
     DMA_CRCWriteByteOrderAlter_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_CRCWriteByteOrderMaintain(DMA_MODULE_ID index)
{
     DMA_CRCWriteByteOrderMaintain_Unsupported(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRCByteOrder(DMA_MODULE_ID index)
{
     return DMA_ExistsCRCByteOrder_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_CRCByteOrderSelect(DMA_MODULE_ID index, DMA_CRC_BYTE_ORDER byteOrder)
{
     DMA_CRCByteOrderSelect_Unsupported(index, byteOrder);
}

PLIB_INLINE_API DMA_CRC_BYTE_ORDER _PLIB_UNSUPPORTED PLIB_DMA_CRCByteOrderGet(DMA_MODULE_ID index)
{
     return DMA_CRCByteOrderGet_Unsupported(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRCData(DMA_MODULE_ID index)
{
     return DMA_ExistsCRCData_Default(index);
}

PLIB_INLINE_API uint32_t PLIB_DMA_CRCDataRead(DMA_MODULE_ID index)
{
     return DMA_CRCDataRead_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_CRCDataWrite(DMA_MODULE_ID index, uint32_t DMACRCdata)
{
     DMA_CRCDataWrite_Default(index, DMACRCdata);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsCRCXOREnable(DMA_MODULE_ID index)
{
     return DMA_ExistsCRCXOREnable_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_CRCXOREnableSet(DMA_MODULE_ID index, uint32_t DMACRCXOREnableMask)
{
     DMA_CRCXOREnableSet_Default(index, DMACRCXOREnableMask);
}

PLIB_INLINE_API uint32_t PLIB_DMA_CRCXOREnableGet(DMA_MODULE_ID index)
{
     return DMA_CRCXOREnableGet_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXPriority(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXPriority_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXPrioritySelect(DMA_MODULE_ID index, DMA_CHANNEL channel, DMA_CHANNEL_PRIORITY channelPriority)
{
     DMA_ChannelXPrioritySelect_Default(index, channel, channelPriority);
}

PLIB_INLINE_API DMA_CHANNEL_PRIORITY PLIB_DMA_ChannelXPriorityGet(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     return DMA_ChannelXPriorityGet_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXEvent(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXEvent_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ChannelXEventIsDetected(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     return DMA_ChannelXEventIsDetected_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXAuto(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXAuto_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXAutoEnable(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXAutoEnable_Default(index, channel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXAutoDisable(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXAutoDisable_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ChannelXAutoIsEnabled(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     return DMA_ChannelXAutoIsEnabled_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXChainEnbl(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXChainEnbl_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXChainEnable(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXChainEnable_Default(index, channel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXChainDisable(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXChainDisable_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ChannelXChainIsEnabled(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     return DMA_ChannelXChainIsEnabled_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXDisabled(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXDisabled_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXDisabledEnablesEvents(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXDisabledEnablesEvents_Default(index, channel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXDisabledDisablesEvents(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXDisabledDisablesEvents_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelX(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelX_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXEnable(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXEnable_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ChannelXIsEnabled(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     return DMA_ChannelXIsEnabled_Default(index, channel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXDisable(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXDisable_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXChain(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXChain_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXChainToLower(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXChainToLower_Default(index, channel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXChainToHigher(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXChainToHigher_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXPatternLength(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXPatternLength_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_ChannelXPatternLengthSet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, DMA_PATTERN_LENGTH patternLen)
{
     DMA_ChannelXPatternLengthSet_Unsupported(index, dmaChannel, patternLen);
}

PLIB_INLINE_API DMA_PATTERN_LENGTH _PLIB_UNSUPPORTED PLIB_DMA_ChannelXPatternLengthGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXPatternLengthGet_Unsupported(index, dmaChannel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXPatternIgnoreByte(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXPatternIgnoreByte_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_ChannelXPatternIgnoreByteEnable(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXPatternIgnoreByteEnable_Unsupported(index, channel);
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_DMA_ChannelXPatternIgnoreByteIsEnabled(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     return DMA_ChannelXPatternIgnoreByteIsEnabled_Unsupported(index, channel);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_ChannelXPatternIgnoreByteDisable(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXPatternIgnoreByteDisable_Unsupported(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXBusy(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXBusy_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_ChannelXBusyActiveSet(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXBusyActiveSet_Unsupported(index, channel);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_ChannelXBusyInActiveSet(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_ChannelXBusyInActiveSet_Unsupported(index, channel);
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_DMA_ChannelXBusyIsBusy(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     return DMA_ChannelXBusyIsBusy_Unsupported(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXPatternIgnore(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXPatternIgnore_Unsupported(index);
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_DMA_ChannelXPatternIgnoreSet(DMA_MODULE_ID index, DMA_CHANNEL channel, uint8_t pattern)
{
     DMA_ChannelXPatternIgnoreSet_Unsupported(index, channel, pattern);
}

PLIB_INLINE_API uint8_t _PLIB_UNSUPPORTED PLIB_DMA_ChannelXPatternIgnoreGet(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     return DMA_ChannelXPatternIgnoreGet_Unsupported(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXTrigger(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXTrigger_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXTriggerEnable(DMA_MODULE_ID index, DMA_CHANNEL channel, DMA_CHANNEL_TRIGGER_TYPE trigger)
{
     DMA_ChannelXTriggerEnable_Default(index, channel, trigger);
}

PLIB_INLINE_API bool PLIB_DMA_ChannelXTriggerIsEnabled(DMA_MODULE_ID index, DMA_CHANNEL channel, DMA_CHANNEL_TRIGGER_TYPE trigger)
{
     return DMA_ChannelXTriggerIsEnabled_Default(index, channel, trigger);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXTriggerDisable(DMA_MODULE_ID index, DMA_CHANNEL channel, DMA_CHANNEL_TRIGGER_TYPE trigger)
{
     DMA_ChannelXTriggerDisable_Default(index, channel, trigger);
}

PLIB_INLINE_API DMA_CHANNEL_INT_SOURCE PLIB_DMA_ChannelXTriggerSourceNumberGet(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     return DMA_ChannelXTriggerSourceNumberGet_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsAbortTransfer(DMA_MODULE_ID index)
{
     return DMA_ExistsAbortTransfer_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_AbortTransferSet(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_AbortTransferSet_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsStartTransfer(DMA_MODULE_ID index)
{
     return DMA_ExistsStartTransfer_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_StartTransferSet(DMA_MODULE_ID index, DMA_CHANNEL channel)
{
     DMA_StartTransferSet_Default(index, channel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXStartIRQ(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXStartIRQ_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXStartIRQSet(DMA_MODULE_ID index, DMA_CHANNEL channel, DMA_TRIGGER_SOURCE IRQnum)
{
     DMA_ChannelXStartIRQSet_Default(index, channel, IRQnum);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXAbortIRQ(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXAbortIRQ_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXAbortIRQSet(DMA_MODULE_ID index, DMA_CHANNEL channel, DMA_TRIGGER_SOURCE IRQ)
{
     DMA_ChannelXAbortIRQSet_Default(index, channel, IRQ);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXINTSourceFlag(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXINTSourceFlag_Default(index);
}

PLIB_INLINE_API bool PLIB_DMA_ChannelXINTSourceFlagGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, DMA_INT_TYPE dmaINTSource)
{
     return DMA_ChannelXINTSourceFlagGet_Default(index, dmaChannel, dmaINTSource);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXINTSourceFlagSet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, DMA_INT_TYPE dmaINTSource)
{
     DMA_ChannelXINTSourceFlagSet_Default(index, dmaChannel, dmaINTSource);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXINTSourceFlagClear(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, DMA_INT_TYPE dmaINTSource)
{
     DMA_ChannelXINTSourceFlagClear_Default(index, dmaChannel, dmaINTSource);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXINTSource(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXINTSource_Default(index);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXINTSourceEnable(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, DMA_INT_TYPE dmaINTSource)
{
     DMA_ChannelXINTSourceEnable_Default(index, dmaChannel, dmaINTSource);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXINTSourceDisable(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, DMA_INT_TYPE dmaINTSource)
{
     DMA_ChannelXINTSourceDisable_Default(index, dmaChannel, dmaINTSource);
}

PLIB_INLINE_API bool PLIB_DMA_ChannelXINTSourceIsEnabled(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, DMA_INT_TYPE dmaINTSource)
{
     return DMA_ChannelXINTSourceIsEnabled_Default(index, dmaChannel, dmaINTSource);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXSourceStartAddress(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXSourceStartAddress_Default(index);
}

PLIB_INLINE_API uint32_t PLIB_DMA_ChannelXSourceStartAddressGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXSourceStartAddressGet_Default(index, dmaChannel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXSourceStartAddressSet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, uint32_t sourceStartAddress)
{
     DMA_ChannelXSourceStartAddressSet_Default(index, dmaChannel, sourceStartAddress);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXDestinationStartAddress(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXDestinationStartAddress_Default(index);
}

PLIB_INLINE_API uint32_t PLIB_DMA_ChannelXDestinationStartAddressGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXDestinationStartAddressGet_Default(index, dmaChannel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXDestinationStartAddressSet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, uint32_t destinationStartAddress)
{
     DMA_ChannelXDestinationStartAddressSet_Default(index, dmaChannel, destinationStartAddress);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXSourceSize(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXSourceSize_Default(index);
}

PLIB_INLINE_API uint16_t PLIB_DMA_ChannelXSourceSizeGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXSourceSizeGet_Default(index, dmaChannel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXSourceSizeSet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, uint16_t sourceSize)
{
     DMA_ChannelXSourceSizeSet_Default(index, dmaChannel, sourceSize);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXDestinationSize(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXDestinationSize_Default(index);
}

PLIB_INLINE_API uint16_t PLIB_DMA_ChannelXDestinationSizeGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXDestinationSizeGet_Default(index, dmaChannel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXDestinationSizeSet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, uint16_t destinationSize)
{
     DMA_ChannelXDestinationSizeSet_Default(index, dmaChannel, destinationSize);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXSourcePointer(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXSourcePointer_Default(index);
}

PLIB_INLINE_API uint16_t PLIB_DMA_ChannelXSourcePointerGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXSourcePointerGet_Default(index, dmaChannel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXDestinationPointer(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXDestinationPointer_Default(index);
}

PLIB_INLINE_API uint16_t PLIB_DMA_ChannelXDestinationPointerGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXDestinationPointerGet_Default(index, dmaChannel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXCellSize(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXCellSize_Default(index);
}

PLIB_INLINE_API uint16_t PLIB_DMA_ChannelXCellSizeGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXCellSizeGet_Default(index, dmaChannel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXCellSizeSet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, uint16_t CellSize)
{
     DMA_ChannelXCellSizeSet_Default(index, dmaChannel, CellSize);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXCellProgressPointer(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXCellProgressPointer_Default(index);
}

PLIB_INLINE_API uint16_t PLIB_DMA_ChannelXCellProgressPointerGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXCellProgressPointerGet_Default(index, dmaChannel);
}

PLIB_INLINE_API bool PLIB_DMA_ExistsChannelXPatternData(DMA_MODULE_ID index)
{
     return DMA_ExistsChannelXPatternData_Default(index);
}

PLIB_INLINE_API uint16_t PLIB_DMA_ChannelXPatternDataGet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel)
{
     return DMA_ChannelXPatternDataGet_Default(index, dmaChannel);
}

PLIB_INLINE_API void PLIB_DMA_ChannelXPatternDataSet(DMA_MODULE_ID index, DMA_CHANNEL dmaChannel, uint16_t patternData)
{
     DMA_ChannelXPatternDataSet_Default(index, dmaChannel, patternData);
}

#endif
