/*
 * File:   utilities.h
 * Author: dsolano
 *
 * Created on June 29, 2014, 11:46 AM
 */
#include <utilities.h>
#include <proc/p32mx360f512l.h>
#include "UDBconfig.h"


#ifdef DBGPRINT
UCHAR8 buff[72];
#endif

// peripheral configurations
#define SYS_FREQ       (80000000uL)

#define FREQ_PBCLK      80000000
#define FREQ_SPICHN     10000000
#define SPI_BRG         (FREQ_PBCLK/(2 *FREQ_SPICHN)) - 1


//************************************************************
//UART printing to screen for debug porpose,
//Define globally DBGPRINT on project configuration properties
//************************************************************
#ifdef DBGPRINT

void DBUGPRINT(const char *buffstring, UCHAR8 data)
{
    sprintf(buff, buffstring, data);
    SendDataBuffer(buff, strlen(buff));
}
#endif

// Interrupt function for Core Timer (CT)
//void __ISR(_CORE_TIMER_VECTOR, IPL2SOFT) CTInterruptHandler(void)
//{
//     static variable for permanent storage duration
//    static unsigned char portValue = 0;
//     variables for Compare period
//    unsigned long ct_count = _CP0_GET_COUNT();
//    unsigned long period = CORE_TICK_RATE;
//    
//     write to port latch
//    LATA = portValue++;
//
//     update the Compare period
//    period += ct_count;
//    _CP0_SET_COMPARE(period);
//
//     clear the interrupt flag
//    IFS0CLR = _IFS0_CTIF_MASK;
//}

void start_timer(void)
{ // resets the core timer count
    WriteCoreTimer(0);
} // start_timer

double read_timer(void)
{
    unsigned int ui;

    // get the core timer count
    ui = ReadCoreTimer();

    // convert in seconds (1 core tick = 2 SYS cycles)
    return ( (ui * 2.0) / CPU_CLOCK_HZ);
} // read_timer

// ===========================================================================
// ShortDelay - Delays (blocking) for a very short period (in CoreTimer Ticks)
// ---------------------------------------------------------------------------
// The DelayCount is specified in Core-Timer Ticks.
// This function uses the CoreTimer to determine the length of the delay.
// The CoreTimer runs at half the system clock.
// If CPU_CLOCK_HZ is defined as 80000000UL, 80MHz/2 = 40MHz or 1LSB = 25nS).
// Use US_TO_CT_TICKS to convert from uS to CoreTimer Ticks.
// ---------------------------------------------------------------------------
//Then define the clock frequencies:
//And use as follows:
//  ShortDelay( 50 );   // Delay .5 sec, 100 = 1 sec.....so on

void ShortDelay(// Short Delay
                UINT32 DelayCount) // Delay Time (CoreTimer Ticks)
{
    UINT32 StartTime; // Start Time
    // get the core timer count
    StartTime = ReadCoreTimer(); // Get CoreTimer value for StartTime
    while ((UINT32) (ReadCoreTimer() - StartTime) < (DELAY_TIME * DelayCount));
}
//
void SystemClockInitialization(void)
{
//
//    // Configure the device for maximum performance but do not change the PBDIV
//    // Given the options, this function will change the flash wait states, RAM
//    // wait state and enable prefetch cache but will not change the PBDIV.
//    // The PBDIV value is already set via the pragma FPBDIV option..
    SYSTEMConfig(CPU_CLOCK_HZ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    SYSTEMConfigPerformance(CPU_CLOCK_HZ);
    SYSTEMConfigPB(CPU_CLOCK_HZ);
    OpenCoreTimer(CPU_CLOCK_HZ / 2 / 1000000);
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);
}

UCHAR8 pic32_SpiXfer(SpiChannel channel, UCHAR8 data) // Only for SPI Channel 1 & 2
{
    if (channel == SPI_CHANNEL2)
    {
        SPI2BUF = data; // write to buffer for TX
        while (!SPI2STATbits.SPIRBF); // wait transfer complete
        return SPI2BUF; // read the received value
    }
    else if (channel == SPI_CHANNEL1)
    {
        SPI1BUF = data; // write to buffer for TX
        while (!SPI1STATbits.SPIRBF); // wait transfer complete
        return SPI1BUF; // read the received value
    }
} // END SpiIO()

void pic32_Spi_Init(SpiChannel channel, int rate)
{
    // Set IOs directions for SPI
    SpiChnEnable(channel,false);
    SpiChnOpen(channel,SPI_OPEN_MSTEN | SPI_OPEN_MODE8 , FREQ_PBCLK/rate);
    SpiChnEnable(channel,true);
}



//--------------------------------------------------------------
// This function converts an 8 bit BCD value to
// an 8 bit binary value.
// The input range must be from 00 to 99.

uint8_t bcd2bin(uint8_t bcd_value)
{
    uint8_t temp;
    temp = bcd_value;
    // Shifting upper digit right by 1 is same as multiplying by 8.
    temp >>= 1;
    // Isolate the bits for the upper digit.
    temp &= 0x78;
    // Now return: (Tens * 8) + (Tens * 2) + Ones
    return (temp + (temp >> 2) + (bcd_value & 0x0f));
}

//-------------------------------------------------------------
// This function converts an 8 bit binary value
// to an 8 bit BCD value.
// The input range must be from 0 to 99.

UCHAR8 bin2bcd(UCHAR8 binary_value)
{
    UCHAR8 temp;
    UCHAR8 retval;

    temp = binary_value;
    retval = 0;

    while (1)
    {
        // Get the tens digit by doing multiple subtraction
        // of 10 from the binary value.
        if (temp >= 10)
        {
            temp -= 10;
            retval += 0x10;
        }
        else // Get the ones digit by adding the remainder.
        {
            retval += temp;
            break;
        }
    }
    return (retval);
}

uint32_t app_rate_hz;
/* 
 * 32 bit timer combining timer2 and timer3
 */
void timer_init(uint32_t rate){
    app_rate_hz = rate;
//    INTDisableInterrupts();
//    OpenTimer2(T2_OFF|T2_PS_1_1|T2_SOURCE_INT, (uint16_t)((PB_BUS_MAX_FREQ_HZ) / rate)-1);
//    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_3 | T2_INT_SUB_PRIOR_0);
    T2CONbits.T32 = 1;  // 32 bit counter timer combined with timer3
    T2CONbits.ON = 0;   // timer2_3 disable
    T2CONbits.TCS = 0;  // Clock source internal
    T2CONbits.TCKPS = 0;// PBCKL 1 a 1
    PR2 = 0xffffffff;   // free running counter comparator when reach this value will return to 0
//    INTEnableSystemMultiVectoredInt();
}

void timer_start(void){
    T2CONbits.ON = 0;   // disable timer2
    TMR2 = 0;           // clear timer counter
//	INTEnableInterrupts();
    T2CONbits.ON = 1;   // enable timer2_3
}

volatile int timer_get_counter(void){
    return TMR2/(PB_BUS_MAX_FREQ_HZ/app_rate_hz);    // time 2_3  full 32 bits
}

volatile int timer_stop(void){
    T2CONbits.ON = 0;   // disable timer2 and timer3
//    INTDisableInterrupts();
    return TMR2/(PB_BUS_MAX_FREQ_HZ/app_rate_hz);  // return the 32 bit value counter register for T2 and T3
}

/*
This code example demonstrates a simple interrupt service routine for Timer
interrupts. The user?s code at this ISR handler should perform any application
specific operations and must clear the corresponding Timer interrupt status flag
before exiting.
*/

//void __ISR(_TIMER_2_VECTOR,IPL3AUTO)Timer2_ISR_Handler(void)
//{
////... perform application specific operations in response to the interrupt
//    //INTDisableInterrupts();
//    //running_timer++;
//    mT2ClearIntFlag(); // Be sure to clear the Timer2 interrupt status
//	//INTEnableInterrupts();
//    
//}