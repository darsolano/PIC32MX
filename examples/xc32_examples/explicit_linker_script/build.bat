@echo off

REM  *********************************************************************
REM  *
REM  *            Windows batch file to build this module
REM  *
REM  *********************************************************************
REM  * Company:         Microchip Technology, Inc.
REM  *
REM  * Software License Agreement
REM  *
REM  * The software supplied herewith by Microchip Technology Incorporated
REM  * (the “Company”) for its PICmicro® Microcontroller is intended and
REM  * supplied to you, the Company’s customer, for use solely and
REM  * exclusively on Microchip PICmicro Microcontroller products. The
REM  * software is owned by the Company and/or its supplier, and is
REM  * protected under applicable copyright laws. All rights are reserved.
REM  * Any use in violation of the foregoing restrictions may subject the
REM  * user to criminal sanctions under applicable laws, as well as to
REM  * civil liability for the breach of the terms and conditions of this
REM  * license.
REM  *
REM  * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
REM  * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
REM  * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
REM  * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
REM  * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
REM  * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
REM  *
REM  * $Id: $
REM  * 
REM  * This batch file builds the current module.
REM  * 
REM  * Usage:
REM  *	1.	Execute "build.bat" to build using default processor 
REM  *		(32MX360F512L) and default compiler and linker options
REM  *
REM  *	2.	Execute "build.bat 32MX360F512L" to build for a specific
REM  *		processor.
REM  *
REM  *	3.	To define desired xc32-gcc.exe build options: 
REM  *		Define and set the GCC_OPTIONS environment variable with
REM  *		with desired xc32-gcc command-line options.
REM  *			
REM  *		For example, following example will build for mips16,
REM  *			set GCC_OPTIONS = -mips16
REM  *			build.bat
REM  ********************************************************************

REM Module specific steps

REM Set the tools installation directory
set TOOLS_DIR="C:\Program Files\Microchip\MPLAB XC32\bin\"

REM Set the relative name of the output elf file name
set ELF_FILE_NAME=explicit_linker_script.hex

REM List of source files that make up this project
REM note the use of batch file line-continuation character
set SOURCE=src\main.c

set OBJECTFILES=main.o


REM Define include search paths specific to your project.
REM If no special paths are required, keep this variable blank
set INCLUDE_PATHS=

set LINKER_PATHS=
set LD_OPTIONS=-Wl,-L"ldscript",--script="ldscript\elf32pic32mx.ld",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,-Map="explicit_linker_script.map"



REM Generic steps

REM Define the default processor to build for.
REM Caller may change the processor by specifying it from command-line
set DEFAULT_PROCESSOR=32MX360F512L


REM Type of processor we will be building for.
REM Use the value from command-line if there is any.
set PROCESSOR=%1


REM This build file uses following environment variables to
REM to allow caller to define its own settings
REM GCC_OPTIONS is a list of C and linker options to be passed via xc32-gcc
REM By default, this batch file does not define any GCC_OPTIONS


REM If there is a processor given via command-line, use it or else use the default one of 32MX360F512L
if NOT "%PROCESSOR%" == "" goto :Continue

REM Processor is not provided, use the default one
set PROCESSOR=%DEFAULT_PROCESSOR%

:Continue
REM Now build it
echo Building for %PROCESSOR% using "%GCC_OPTIONS%" options... >&2
echo Use BUILD_PROCESSOR and GCC_OPTIONS env variables to specify your choice. >&2

REM "C:\Program Files\Microchip\MPLAB XC32\bin\xc32-gcc.exe" -mprocessor=32MX360F512L -x c -c "src\main.c" -o"main.o" -MMD -MF"main.d" -D__DEBUG -g
REM "C:\Program Files\Microchip\MPLAB XC32\bin\xc32-gcc.exe" -mprocessor=32MX360F512L "main.o" -o"explicit_linker_script.elf" -Wl,-L"ldscript",--script="ldscript\elf32pic32mx.ld",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,-Map="explicit_linker_script.map"

REM Turn echo on so that user can see actual buld command we are executing
@echo on
%TOOLS_DIR%xc32-gcc %SOURCE% -mprocessor=%PROCESSOR% -c %INCLUDE_PATHS% %GCC_OPTIONS%
%TOOLS_DIR%xc32-gcc %OBJECTFILES%  -mprocessor=%PROCESSOR% -o %ELF_FILE_NAME% %LINKER_PATHS% %LD_OPTIONS%







