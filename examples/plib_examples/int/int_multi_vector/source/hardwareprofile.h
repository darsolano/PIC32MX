/********************************************************************
 * FileName:		hardwareprofile.h
 * Dependencies:
 * Processor:
 * Hardware:
 * Assembler:
 * Linker:
 * Company:		Microchip Technology Inc.
 *
 * Software License Agreement:
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *********************************************************************
 * File Description:
 *
 * Mulit-vectored interrupt
 *
 * Change History:
 * Name				Date            Changes
 * Sean Justice		01/02/07		Initial Version
 *
 * $Id: int_multi_vector.c 9390 2008-06-16 23:43:04Z rajbhartin $
 ********************************************************************/

#ifndef _HARDWARE_PROFILE_H
#define _HARDWARE_PROFILE_H

#include <plib.h>

#ifdef _USE_STARTER_KIT

#define LED_PORT        IOPORT_D

#define CORE_TIMER_LED  BIT_0
#define CORE_SW_0_LED   BIT_1
#define CORE_SW_1_LED   BIT_2

#else

#define LED_PORT        IOPORT_A

#define CORE_TIMER_LED  BIT_7
#define CORE_SW_0_LED   BIT_6
#define CORE_SW_1_LED   BIT_2

#endif     

#endif
