
#include <string.h>
#include <ring_buffer.h>
#include "UART_app.h"
#include <stdarg.h>
#include <plib.h>

#ifndef SYS_CLOCK
// Definition, Constants, Enum, Structs for the application board - UNIVERSAL DEVELOPMENT BOARD
#define SYS_FREQ                    (80000000UL)
#define SYS_CLOCK                   (80000000UL)
#define	GetSystemClock()            (80000000ul)
#define	GetPeripheralClock()        (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()       (GetSystemClock())
#define I2C_CLOCK_FREQ              400000
#endif
const char	    sprompt[] = "\nctrl :> ";
const char init_string[] = "UART Initialize for PIC32MX360L512\n";
RINGBUFF_T	    uartrb;	// Ring Buffer definition for UART1
uint8_t		    Command2Process[96];	// Ring buffer should be copied to this buffer in order to process the current entered command
volatile char*      rbBuffer[BUFFER_SIZE];
BOOL                CmdState;


// *****************************************************************************
// void SendDataBuffer(const char *buffer, UINT32 size)
// *****************************************************************************
#pragma reentrant
void SendDataBuffer(const char *buffer, UINT32 size)
{
    while(size)
    {
        while(!UARTTransmitterIsReady(UDB_UART));
        UARTSendDataByte(UDB_UART, *buffer);
        buffer++;
        size--;
    }
    while(!UARTTransmissionHasCompleted(UDB_UART));
}

// *****************************************************************************
// void SendString(char *buffer)
// *****************************************************************************
#pragma reentrant
void SendString(const char *buffer)
{
	//char* ptr = buffer;
    while(*buffer)
    {
		Sendchr (*buffer++);
    }
    while(!UARTTransmissionHasCompleted(UDB_UART));
}


// *****************************************************************************
// void Sendchr(const char character )
// *****************************************************************************
void Sendchr(UCHAR8 character )
{
    while(!UARTTransmitterIsReady(UDB_UART));
    UARTSendDataByte(UDB_UART,character);    //while(!UARTTransmissionHasCompleted(UDB_UART));
}

#pragma reentrant
// *****************************************************************************
// UCHAR8  Getchr(void) to use it independent or on interrupt
// *****************************************************************************
UCHAR8  Getchr(void){
	UCHAR8 data;
	if (UARTReceivedDataIsAvailable(UDB_UART)){
		data = UARTGetDataByte(UDB_UART);
	}
	return (data);
}

BOOL IsUARTDataReady(void)
{
	return CmdState;
}

// check if there is data ready
uint8_t* GetUARTData(void)
{
	uint8_t* ptr = Command2Process;
	CmdState = FALSE;
	prompt();
	return ptr;
}


void prompt(void)
{
	SendDataBuffer (sprompt, strlen(sprompt));
}


// *****************************************************************************
// void UART_Init(void)
// *****************************************************************************
void UART_Init(void){
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
    //UARTSetFifoMode(UART1, UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART1, GetPeripheralClock(), 115200);
    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    SendString (init_string);

    //InitUARTInterrupt (); // Init interrupt settings for UART RX
    //RingBuffer_Init(&uartrb, rbBuffer,sizeof(char), BUFFER_SIZE);	// Enable Ring Buffer
    //prompt (); // Print prompt string
}

void InitUARTInterrupt(void){
	INTDisableInterrupts();
//	// configure for single vectored mode
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
//	// Configure the interrupt priority, level 3
	INTSetVectorPriority( INT_UART_1_VECTOR, INT_PRIORITY_LEVEL_3);
//	// Configure the interrupt sub-priority, level 0
	INTSetVectorSubPriority( INT_UART_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
//	// Clear the interrupt source flag
	INTClearFlag(INT_U1RX);
//	// Enable the interrupt source
	INTEnable(INT_U1RX, INT_ENABLED);
	INTEnableInterrupts();
}

void __ISR(_UART1_VECTOR, IPL3AUTO) UARTISRHandler(void)
{
	INTClearFlag (INT_U1RX);	// Clear interrupt flag for data received
	INTDisableInterrupts();		// disable all interrupts
	uint8_t data;
	data = Getchr();

	switch (data){
		case '\b':		// Backspace, deletes a char in buffer
			Sendchr (data);
			uartrb.head--;
			break;
		case '\n':		// proceed to process the command
			data=0;	// end string in null char to prepare the string to be processed
			RingBuffer_Insert(&uartrb, &data);	// Place the null in the ring buffer
			RingBuffer_PopMult(&uartrb,Command2Process,RingBuffer_GetCount(&uartrb));	// Place the string entered in a variable to be processed by
			prompt();																	// the Command Processor
			CmdState = TRUE;
			break;
		default:
			Sendchr (data);
			RingBuffer_Insert(&uartrb, &data);
			break;
	}
	INTEnableInterrupts();
}

//****************************************************************************************
// For printf redirection
/*
   By default, MPLAB XC32's libraries use UART2 for STDOUT. This means that formatted output
 * functions such as printf() will send their output to UART2 (as opposed to the terminal
 * on a PC). In some cases, you may want to use some other peripheral or mechanism for your
 * output. To accomplish this, there are two steps that must be taken.

	You must do one of the following:
	=Disable buffered outputs to stdio with setbuf(stdout, NULL) in your initialization code
	=Call fflush(stdout) after each call to printf()
	=Insert a "\r\n" at the end of every string you wish to print
	=Write a custom replacement for _mon_putc() to write one character to the output of your choice.
 * The code below shows part of an example of how to redirect stdout from UART2 to anything you want by using your own custom function.
	_mon_putc() is defined in the PIC32 libraries with the weak attribute tag.
 * This means that if another function with the same name is found in a project without
 * the weak attribute, that one will take precedence and the weak one will be ignored.
******************************************************************************************* */
void _mon_putc(char c){
#ifdef UART_STDOUT
    Sendchr(c); // printf() passes each character to write to _mon_putc(), which in turn passes each character to a custom output function
#endif
#ifdef LCD_STDOUT
	LCDI2C_Putc( c); //printf will print on lcd
#endif
}


/*********************************************************************//**
 * @brief		Puts a decimal number to UART port just 2 digits instead of 3
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		just numbers less that 100
 **********************************************************************/
void prnPutDec99( uint8_t decnum)
{
	uint8_t c1 = decnum % 10;
	uint8_t c2 = (decnum / 10) % 10;
	Sendchr( '0' + c2);
	Sendchr( '0' + c1);
}


/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void prnPutDec( uint8_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	Sendchr( '0'+c3);
	Sendchr( '0'+c2);
	Sendchr( '0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void prnPutDec16( uint16_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	uint8_t c4=(decnum/1000)%10;
	uint8_t c5=(decnum/10000)%10;
	Sendchr( '0'+c5);
	Sendchr( '0'+c4);
	Sendchr( '0'+c3);
	Sendchr( '0'+c2);
	Sendchr( '0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void prnPutDec32( uint32_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	uint8_t c4=(decnum/1000)%10;
	uint8_t c5=(decnum/10000)%10;
	uint8_t c6=(decnum/100000)%10;
	uint8_t c7=(decnum/1000000)%10;
	uint8_t c8=(decnum/10000000)%10;
	uint8_t c9=(decnum/100000000)%10;
	uint8_t c10=(decnum/1000000000)%10;
	Sendchr( '0'+c10);
	Sendchr( '0'+c9);
	Sendchr( '0'+c8);
	Sendchr( '0'+c7);
	Sendchr( '0'+c6);
	Sendchr( '0'+c5);
	Sendchr( '0'+c4);
	Sendchr( '0'+c3);
	Sendchr( '0'+c2);
	Sendchr( '0'+c1);
}


/*********************************************************************//**
 * @brief		Puts a hex number to UART port without the 0x chars
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (8-bit long)
 * @return		None
 **********************************************************************/
void prnPutHex_( uint8_t hexnum)
{
	uint8_t nibble, i;

	//UARTPuts(UARTx, "0x");
	i = 1;
	do
	{
		nibble = (hexnum >> (4 * i)) & 0x0F;
		Sendchr((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}



/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (8-bit long)
 * @return		None
 **********************************************************************/
void prnPutHex ( uint8_t hexnum)
{
	uint8_t nibble, i;

	Sendchr('0');
	Sendchr('x');
	i = 1;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		Sendchr( (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}


/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (16-bit long)
 * @return		None
 **********************************************************************/
void prnPutHex16 ( uint16_t hexnum)
{
	uint8_t nibble, i;

	Sendchr('0');
	Sendchr('x');
	i = 3;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		Sendchr( (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}

/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (32-bit long)
 * @return		None
 **********************************************************************/
void prnPutHex32 ( uint32_t hexnum)
{
	uint8_t nibble, i;

	Sendchr('0');
	Sendchr('x');
	i = 7;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		Sendchr( (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}


int uart_printf_(const char *format, ...)
{
	unsigned int index = 0;
	va_list argptr;

	va_start(argptr, format);

	while(format[index] != '\0')
	{
		if(format[index] == '%')
		{
			index++;

			if(format[index] == '\0')
				return;

			switch(format[index])
			{
				case 'd':
					prnPutDec16 (va_arg(argptr, int));
					break;
				case 'i':
					prnPutDec16 (va_arg(argptr, int));
					break;
				case 'u':
					prnPutDec99 (va_arg(argptr, unsigned int));
					break;
				case 'c':
					Sendchr(va_arg(argptr, int));
					break;
				case 's':
					SendString (va_arg(argptr, char *));
					break;
				case 'x':
					prnPutHex (va_arg(argptr, unsigned int));
					break;
				default:
					Sendchr(format[index]);
			}
		}
		else
			Sendchr (format[index]);

		index++;
	}

    va_end(argptr);
}
