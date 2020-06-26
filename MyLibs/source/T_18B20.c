/*
 * pic32mx360f512l
   T_18B20.c - library for 18B20 Thermometer
 * Created on June 26, 2014, 12:36 AM
 */

#include <stdbool.h>

#include "T_18B20.h"
#include "utilities.h"
#include "_1wire.h"

PRIVATE UCHAR8 printbuff[20];

BOOL DS18B20Init(DS18b20_t* temp)
{
    if (!OW_reset_pulse()) return FALSE;
    temp->Temperature.Temp_Type = CELSIUS;
    if (DS18B20CmdGetROMIDCode(temp) && DS18B20GetScrachtPadReg(temp) && DS18B20GetTemperature(temp))
    {
        temp->Temperature.Lowest_Temp = temp->Temperature.Temp_Whole;
        return TRUE;
    }
    return FALSE;
}

BOOL DS18B20CmdGetROMIDCode(DS18b20_t* temp)
{
    UINT8 loop;
    if (!OW_reset_pulse()) return FALSE;
    OW_write_byte(READ_ROM);
    for (loop = 0; loop < 8; loop++)
    {
        temp->ROM.code[loop] = OW_read_byte();
    }
    return TRUE;
}

BOOL DS18B20ConfirmIDofDevice(DS18b20_t *temp)
{
    UCHAR8 i;
    if (!OW_reset_pulse()) return FALSE;
    OW_write_byte(MATCH_ROM); // match ROM
    for (i = 0; i < 8; i++)
    {
        OW_write_byte(temp->ROM.code[i]); //send ROM code
        ShortDelay(delay_10us);
    }
    return TRUE;
}

BOOL DS18B20WriteAlarm_CfgReg(UINT16 Alarm, UCHAR8 Config)
{

}

BOOL DS18B20GetTemperature(DS18b20_t* temp)
{
    int whole = 0, decimal = 0;
    int c16, c2, f10;

    if (!OW_reset_pulse()) return FALSE;

    OW_write_byte(SKIP_ROM); //Skip ROM
    OW_write_byte(CONVERT_T); // Start Conversion
    ShortDelay(5);
    if (!OW_reset_pulse()) return FALSE;

    OW_write_byte(SKIP_ROM); // Skip ROM
    OW_write_byte(READ_SCRATCHPAD); // Read Scratch Pad

    //from scratchpad read, the first 2 byte are from temp register, others are dropped
    temp->ScratchPad.TempLow = OW_read_byte();
    temp->ScratchPad.TempHi = OW_read_byte();

    if (!OW_reset_pulse()) return FALSE;

    /* Be sure to start the temperature sensor with a temp_type defined */
    /* Start Celsius Convertion*/
    if (temp->Temperature.Temp_Type == CELSIUS)
    {
        // Celsius calculation
        temp->Temperature.Temp_Whole = (temp->ScratchPad.TempHi & 0x07) << 4; // grab lower 3 bits of t1
        temp->Temperature.Temp_Whole |= temp->ScratchPad.TempLow >> 4; // and upper 4 bits of t0
        temp->Temperature.Temp_Decimal = temp->ScratchPad.TempLow & 0x0F; // decimals in lower 4 bits of t0
        temp->Temperature.Temp_Decimal *= 625; // conversion factor for 12-bit resolution
    }
    /* Start Farenheit convertion */
    if (temp->Temperature.Temp_Type == FARENHEIT)
    {
        // Farenheit convertion
        c16 = (temp->ScratchPad.TempHi << 8) + temp->ScratchPad.TempLow; // result is temp*16, in celcius
        c2 = c16 / 8; // get t*2, with fractional part lost
        f10 = c16 + c2 + 320; // F=1.8C+32, so 10F = 18C+320 = 16C + 2C + 320
        temp->Temperature.Temp_Whole = f10 / 10; // get whole part
        temp->Temperature.Temp_Decimal = f10 % 10; // get fractional part
    }
    /* Set de high and low temperature reading */
    if (temp->Temperature.Temp_Whole >= temp->Temperature.Highest_Temp)
        temp->Temperature.Highest_Temp = temp->Temperature.Temp_Whole;
    if (temp->Temperature.Temp_Whole <= temp->Temperature.Lowest_Temp)
        temp->Temperature.Lowest_Temp = temp->Temperature.Temp_Whole;

    return TRUE;
}

void DS18B20RestoreEEPROMValues(void)
{

}

void DS18B20SaveSRAMValues(void)
{

}

POWERTYPE_t DS18B20GetPWRSupplyStatus(void)
{

}

UINT8 DS18B20CalcAndCompareCRC(UCHAR8 _crc)
{

}


BOOL DS18B20GetScrachtPadReg(DS18b20_t* temp)
{
    int j;
    if (!OW_reset_pulse()) return FALSE;
    OW_write_byte(SKIP_ROM);        // Skip ROM
    OW_write_byte(READ_SCRATCHPAD); // Read Scratch Pad
    temp->ScratchPad.TempLow    = OW_read_byte();
    temp->ScratchPad.TempHi     = OW_read_byte();
    temp->ScratchPad.UserByte1  = OW_read_byte();
    temp->ScratchPad.UserByte2  = OW_read_byte();
    temp->ScratchPad.Config     = OW_read_byte();
    temp->ScratchPad.reserved0  = OW_read_byte();
    temp->ScratchPad.reserved1  = OW_read_byte();
    temp->ScratchPad.reserved2  = OW_read_byte();
    temp->ScratchPad.CRC        = OW_read_byte();
    return TRUE;
}

#define DS_MAXIM_POLY   0x31    // Polynom x^8+x^5+x^4+x^0 = x^8+x^5+x^4+1 = 100110001 = 0x131 msb discarded = 0x31
uint8_t DS18B20VerifyCRC(uint8_t* data, uint8_t crc){
    
}