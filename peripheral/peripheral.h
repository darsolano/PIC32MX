/*******************************************************************************
  Peripheral Library Interface Header

  Company:
    Microchip Technology Inc.

  File Name:
    peripheral.h

  Summary:
    This file aggregates all of the peripheral library interface headers.

  Description:
    This file aggregates all of the peripheral library interface headers so
    client code only needs to include this one single header to obtain
    prototypes and definitions for the interfaces to all peripheral libraries.

    Peripheral Libraries (PLIBs) provide low-level abstractions of peripherals
    found on Microchip microcontrollers in the form of a convenient C language
    interface.  PLIBs can be used to simplify low-level access to peripheral
    modules without the necessity of interacting directly with module
    registers, thus hiding differences from one microcontroller to another.

  Remarks:
    The directory in which this file resides should be added to the compiler's
    search path for header files.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef _PLIB_H_
#define _PLIB_H_


// *****************************************************************************
/* Refer to the "Prebuilt Libraries" help documentation on Peripheral 
   Libraries for additional information.
*/
#ifndef PLIB_DISABLE_OPTIMIZATION_WARNING
    #ifndef __OPTIMIZE__
        #warning "MPLAB Harmony peripheral libraries are intended to be built with optimizations (-O1 or higher) enabled"
    #endif
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* Individual Peripheral Library Interface Headers 
*/

#include "adc/plib_adc.h"        // Only on PIC32MX devices
#include "bmx/plib_bmx.h"        // Only on PIC32MX devices
#include "can/plib_can.h"
#include "cdac/plib_cdac.h"
#include "cmp/plib_cmp.h"
#include "ctr/plib_ctr.h"
#include "devcon/plib_devcon.h"
#include "ddr/plib_ddr.h"
#include "dma/plib_dma.h"
#include "dmt/plib_dmt.h"
//#include "eth/plib_eth.h"
#include "i2c/plib_i2c.h"
#include "ic/plib_ic.h"
#include "int/plib_int.h"
#include "mcpwm/plib_mcpwm.h"
#include "nvm/plib_nvm.h"
#include "oc/plib_oc.h"
#include "osc/plib_osc.h"
#include "mcpwm/plib_mcpwm.h"
#include "pcache/plib_pcache.h"
#include "pmp/plib_pmp.h"
#include "ports/plib_ports.h"
#include "power/plib_power.h"
#include "ptg/plib_ptg.h"
#include "reset/plib_reset.h"
#include "rtcc/plib_rtcc.h"
#include "spi/plib_spi.h"
#include "tmr/plib_tmr.h"
#include "usart/plib_usart.h"
//#include "usb/plib_usb.h"
//#include "usbhs/plib_usbhs.h"
#include "wdt/plib_wdt.h"
#include "ctmu/plib_ctmu.h"
#include "glcd/plib_glcd.h"

#endif // _PLIB_H
/*******************************************************************************
 End of File
*/

