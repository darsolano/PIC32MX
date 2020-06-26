/*********************************************************************
 *
 *            CRC calculation header file
 *
 *********************************************************************
 * FileName:        crc.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC32MX
 * Compiler:		MPLAB C30 v1.31.00 or higher
 *                  MPLAB IDE v7.10.00 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
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
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * $Id: Crc.h,v 1.1 2006/07/20 15:52:08 C12878 Exp $
 *
 ********************************************************************/

#ifndef CRC_H_
#define CRC_H_

#include "GenericTypeDefs.h"

// interface functions

PUBLIC void		Crc32Init(UINT32 polynomial, UINT32 polOrder, UINT32 seed);
PUBLIC void		Crc32Add(BYTE* pBuff, unsigned int bSize);
PUBLIC UINT32	Crc32Result(void);


#endif //CRC_H_
