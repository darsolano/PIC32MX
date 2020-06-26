/*
 * tinyrtc.c
 *
 *  Created on: Aug 4, 2014
 *      Author: dsolano
 *	TinyRTCC v.1.1 by weiman
 */

#include "tinyRTC.h"
#include <utilities.h>
#include "mypic32_i2c.h"
#include <string.h>
#include <stdio.h>

/*
 * PRIVATE FUNCTION AND VARIABLES
 */

const uint8_t daysArray[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
const uint8_t dowArray[]  = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
I2C_MODULE i2cx;
char buffer[255];


static char 	*strDayOfWeek(uint8_t dayOfWeek);
static char 	*strMonth(uint8_t month);
static char 	*strAmPm(uint8_t hour, BOOL uppercase);
static char 	*strDaySufix(uint8_t day);

static uint8_t 	hour12(uint8_t hour24);
static uint8_t 	bcd2dec(uint8_t bcd);
static uint8_t 	dec2bcd(uint8_t dec);

static long 	time2long(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
static uint16_t date2days(uint16_t year, uint8_t month, uint8_t day);
static uint8_t 	daysInMonth(uint16_t year, uint8_t month);
static uint16_t dayInYear(uint16_t year, uint8_t month, uint8_t day);
static BOOL 	isLeapYear(uint16_t year);
static uint8_t 	dow(uint16_t y, uint8_t m, uint8_t d);

static uint32_t unixtime(RTCDateTime *t);
static uint8_t 	conv2d(const char* p);

static void 	writeRegister8(uint8_t reg, uint8_t value);
static uint8_t 	readRegister8(uint8_t reg);

static uint8_t bcd2dec(uint8_t bcd)
{
    return ((bcd / 16) * 10) + (bcd % 16);
}

static uint8_t dec2bcd(uint8_t dec)
{
    return ((dec / 10) * 16) + (dec % 10);
}

static char *strDayOfWeek(uint8_t dayOfWeek)
{
    switch (dayOfWeek) {
        case 1:
            return "Monday";
            break;
        case 2:
            return "Tuesday";
            break;
        case 3:
            return "Wednesday";
            break;
        case 4:
            return "Thursday";
            break;
        case 5:
            return "Friday";
            break;
        case 6:
            return "Saturday";
            break;
        case 7:
            return "Sunday";
            break;
        default:
            return "Unknown";
    }
}

static char *strMonth(uint8_t month)
{
    switch (month) {
        case 1:
            return "January";
            break;
        case 2:
            return "February";
            break;
        case 3:
            return "March";
            break;
        case 4:
            return "April";
            break;
        case 5:
            return "May";
            break;
        case 6:
            return "June";
            break;
        case 7:
            return "July";
            break;
        case 8:
            return "August";
            break;
        case 9:
            return "September";
            break;
        case 10:
            return "October";
            break;
        case 11:
            return "November";
            break;
        case 12:
            return "December";
            break;
        default:
            return "Unknown";
    }
}

static char *strAmPm(uint8_t hour, BOOL uppercase)
{
    if (hour < 12)
    {
        if (uppercase)
        {
            return "AM";
        } else
        {
            return "am";
        }
    } else
    {
        if (uppercase)
        {
            return "PM";
        } else
        {
            return "pm";
        }
    }
}

static char *strDaySufix(uint8_t day)
{
    if (day % 10 == 1)
    {
        return "st";
    } else
    if (day % 10 == 2)
    {
        return "nd";
    }
    if (day % 10 == 3)
    {
        return "rd";
    }

    return "th";
}

static uint8_t hour12(uint8_t hour24)
{
    if (hour24 == 0)
    {
        return 12;
    }

    if (hour24 > 12)
    {
       return (hour24 - 12);
    }

    return hour24;
}

static long time2long(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    return ((days * 24L + hours) * 60 + minutes) * 60 + seconds;
}

static uint16_t dayInYear(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t fromDate;
    uint16_t toDate;

    fromDate = date2days(year, 1, 1);
    toDate = date2days(year, month, day);

    return (toDate - fromDate);
}

static BOOL isLeapYear(uint16_t year)
{
    return (year % 4 == 0);
}

static uint8_t daysInMonth(uint16_t year, uint8_t month)
{
    uint8_t days;

    days = *(daysArray + month - 1);

    if ((month == 2) && isLeapYear(year))
    {
        ++days;
    }

    return days;
}

static uint16_t date2days(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t i;
    year = year - 2000;

    uint16_t days16 = day;

    for ( i = 1; i < month; ++i)
    {
        days16 += *(daysArray + i - 1);
    }

    if ((month == 2) && isLeapYear(year))
    {
        ++days16;
    }

    return days16 + 365 * year + (year + 3) / 4 - 1;
}

static uint32_t unixtime(RTCDateTime *t)
{
    uint32_t u;

    u = time2long(date2days(t->year, t->month, t->day), t->hour, t->minute, t->second);
    u += 946681200;

    return u;
}

static uint8_t conv2d(const char* p)
{
    uint8_t v = 0;

    if ('0' <= *p && *p <= '9')
    {
        v = *p - '0';
    }

    return 10 * v + *++p - '0';
}

static uint8_t dow(uint16_t y, uint8_t m, uint8_t d)
{
    uint8_t dow;

    y -= m < 3;
    dow = ((y + y/4 - y/100 + y/400 + *(dowArray+(m-1)) + d) % 7);

    if (dow == 0)
    {
        return 7;
    }

    return dow;
}

static void writeRegister8(uint8_t reg, uint8_t value)
{
	uint8_t buff[2] = {0};
	buff[0] = reg;
	buff[1] = value;
	i2c_master_send(i2cx, DS1307_I2C_ADDR, buff, 2);
}

static uint8_t readRegister8(uint8_t reg)
{
    uint8_t value;

	i2c_master_cmd_read(i2cx, DS1307_I2C_ADDR, reg, &value, 1);

    return value;
}

/*
 * END OF PRIVATE FUNCTIONS
 */

/*
 * START OF PUBLIC FUNCTIONS
 */

BOOL DS1307_initialize(RTCDateTime *t, I2C_MODULE i2cid)
{
    I2CInitDevice(i2cid, DS1307_I2C_CLOCK_FREQ);
    i2cx = i2cid;


    t->year = 2019;
    t->month = 1;
    t->day = 1;
    t->hour = 0;
    t->minute = 0;
    t->second = 0;
    t->dayOfWeek = 6;
    t->unixtime = 946681200;
    
    
    writeRegister8(DS1307_REG_TIME,0);
    return TRUE;
}

void DS1307_setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
	uint8_t buf[10] = {0};
	I2C_MData_Cfg_sType xfer = {0};

	buf[0] = DS1307_REG_TIME;
	buf[1] = (dec2bcd(second)) & 0x7f;
    buf[2] = (dec2bcd(minute));
    buf[3] = (dec2bcd(hour));
    buf[4] = (dec2bcd(dow(year, month, day)));
    buf[5] = (dec2bcd(day));
    buf[6] = (dec2bcd(month));
    buf[7] = (dec2bcd(year-2000));

    xfer.sl_addr_7bit = DS1307_I2C_ADDR;
    xfer.ptxBuff = buf;
    xfer.txDataLen = 8;
	I2C_MasterDataTransfer(i2cx, &xfer);
}

void DS1307_setDateTime_word(uint32_t t)
{
    t -= 946681200;

    uint8_t daysPerMonth;

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    second = t % 60;
    t /= 60;

    minute = t % 60;
    t /= 60;

    hour = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;

    for (year = 0; ; ++year)
    {
        leap = year % 4 == 0;
        if (days < 365 + leap)
        {
            break;
        }
        days -= 365 + leap;
    }

    for (month = 1; ; ++month)
    {
        daysPerMonth = daysArray[month - 1];

        if (leap && month == 2)
        {
            ++daysPerMonth;
        }

        if (days < daysPerMonth)
        {
            break;
        }
        days -= daysPerMonth;
    }

    day = days + 1;

    DS1307_setDateTime(year+2000, month, day, hour, minute, second);
}

void DS1307_setDateTime_array(const char* date, const char* time)
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    year = conv2d(date+9);

    switch (date[0])
    {
        case 'J': month = (date[1] == 'a' ? 1 : ((date[2] == 'n' ? 6 : 7))); break;
        case 'F': month = 2; break;
        case 'A': month = (date[2] == 'r' ? 4 : 8); break;
        case 'M': month = (date[2] == 'r' ? 3 : 5); break;
        case 'S': month = 9; break;
        case 'O': month = 10; break;
        case 'N': month = 11; break;
        case 'D': month = 12; break;
    }

    day 	= conv2d(date+4);
    hour 	= conv2d(time);
    minute 	= conv2d(time+3);
    second 	= conv2d(time+6);

    DS1307_setDateTime(year+2000, month, day, hour, minute, second);
}

char* DS1307_dateFormat(const char* dateFormat, RTCDateTime* dt)
{

    buffer[0] = 0;

    char helper[11];

    while (*dateFormat != '\0')
    {
        switch (dateFormat[0])
        {
            // Day decoder
            case 'd':
                sprintf(helper, "%02d", dt->day);
                strcat(buffer, (const char *)helper);
                break;
            case 'j':
                sprintf(helper, "%d", dt->day);
                strcat(buffer, (const char *)helper);
                break;
            case 'l':
                strcat(buffer, (const char *)strDayOfWeek(dt->dayOfWeek));
                break;
            case 'D':
                strncat(buffer, strDayOfWeek(dt->dayOfWeek), 3);
                break;
            case 'N':
                sprintf(helper, "%d", dt->dayOfWeek);
                strcat(buffer, (const char *)helper);
                break;
            case 'w':
                sprintf(helper, "%d", (dt->dayOfWeek + 7) % 7);
                strcat(buffer, (const char *)helper);
                break;
            case 'z':
                sprintf(helper, "%d", dayInYear(dt->year, dt->month, dt->day));
                strcat(buffer, (const char *)helper);
                break;
            case 'S':
                strcat(buffer, (const char *)strDaySufix(dt->day));
                break;

            // Month decoder
            case 'm':
                sprintf(helper, "%02d", dt->month);
                strcat(buffer, (const char *)helper);
                break;
            case 'n':
                sprintf(helper, "%d", dt->month);
                strcat(buffer, (const char *)helper);
                break;
            case 'F':
                strcat(buffer, (const char *)strMonth(dt->month));
                break;
            case 'M':
                strncat(buffer, (const char *)strMonth(dt->month), 3);
                break;
            case 't':
                sprintf(helper, "%d", daysInMonth(dt->year, dt->month));
                strcat(buffer, (const char *)helper);
                break;

            // Year decoder
            case 'Y':
                sprintf(helper, "%d", dt->year);
                strcat(buffer, (const char *)helper);
                break;
            case 'y':
            	sprintf(helper, "%02d", dt->year-2000);
                strcat(buffer, (const char *)helper);
                break;
            case 'L':
                sprintf(helper, "%d", isLeapYear(dt->year));
                strcat(buffer, (const char *)helper);
                break;

            // Hour decoder
            case 'H':
                sprintf(helper, "%02d", dt->hour);
                strcat(buffer, (const char *)helper);
                break;
            case 'G':
                sprintf(helper, "%d", dt->hour);
                strcat(buffer, (const char *)helper);
                break;
            case 'h':
                sprintf(helper, "%02d", hour12(dt->hour));
                strcat(buffer, (const char *)helper);
                break;
            case 'g':
                sprintf(helper, "%d", hour12(dt->hour));
                strcat(buffer, (const char *)helper);
                break;
            case 'A':
                strcat(buffer, (const char *)strAmPm(dt->hour, TRUE));
                break;
            case 'a':
                strcat(buffer, (const char *)strAmPm(dt->hour, FALSE));
                break;

            // Minute decoder
            case 'i':
                sprintf(helper, "%02d", dt->minute);
                strcat(buffer, (const char *)helper);
                break;

            // Second decoder
            case 's':
                sprintf(helper, "%02d", dt->second);
                strcat(buffer, (const char *)helper);
                break;

            // Misc decoder
            case 'U':
                sprintf(helper, "%lu", dt->unixtime);
                strcat(buffer, (const char *)helper);
                break;

            default:
                strncat(buffer, dateFormat, 1);
                break;
        }
        dateFormat++;
    }

    return buffer;
}

void DS1307_getDateTime(RTCDateTime* t)
{
    uint8_t values[7];

    i2c_master_cmd_read(i2cx, DS1307_I2C_ADDR, DS1307_REG_TIME, values, 7);

    t->second 		= bcd2dec(values[0]);
    t->minute 		= bcd2dec(values[1]);
    t->hour 		= bcd2dec(values[2]);
    t->dayOfWeek 	= bcd2dec(values[3]);
    t->day 			= bcd2dec(values[4]);
    t->month 		= bcd2dec(values[5]);
    t->year 		= bcd2dec(values[6]) + 2000;
    t->unixtime 	= unixtime(t);

}

BOOL DS1307_isReady(void)
{
    uint8_t control = 0;
    control = readRegister8(DS1307_REG_CONTROL);
    return (control && 0x80);
}

void DS1307_enableOutput(BOOL enabled)
{
    uint8_t value;

    value = readRegister8(DS1307_REG_CONTROL);

    if (!enabled) value &= 0b01101111;
    if (enabled)  value |= (9 << 4);

    writeRegister8(DS1307_REG_CONTROL, value);
}

BOOL DS1307_isOutput(void)
{
    uint8_t value;

    value = readRegister8(DS1307_REG_CONTROL);

    return (value && 0b10010000);
}

void DS1307_setOutput(DS1307_sqw_t mode)
{
    uint8_t value;

    value = readRegister8(DS1307_REG_CONTROL);

    value &= 0b11111100;
    value |= (mode << 0);

    writeRegister8(DS1307_REG_CONTROL, value);
}

DS1307_sqw_t DS1307_getOutput(void)
{
    uint8_t value;

    value = readRegister8(DS1307_REG_CONTROL);

    value &= 0b00000011;

    return (DS1307_sqw_t)value;
}

void DS1307_enable32kHz(BOOL enabled)
{
    uint8_t value;

    value = readRegister8(DS1307_REG_CONTROL);

    value &= 0b11111100;
    value |= (DS1307_32768HZ << 0);

    writeRegister8(DS1307_REG_CONTROL, value);
}

BOOL DS1307_is32kHz(void)
{
    uint8_t value;

    value = readRegister8(DS1307_REG_CONTROL);

    value &= 0b00000011;

    return (value && DS1307_32768HZ);
}

//Save a Byte of data to DS1307 pointed by the address to be written
UCHAR8 DS1307SaveRAMByte ( UCHAR8 address, UCHAR8 data )
{
    if (address < 0x08) return 1;
    buffer[0] = address;
    buffer[1] = data;
    i2c_master_send(i2cx, DS1307_I2C_ADDR,buffer,2);
    return 0;
}

UCHAR8 DS1307ReadRAMByte ( UCHAR8 address )
{
    if (address < 0x08) return 1;
    i2c_master_cmd_read(i2cx, DS1307_I2C_ADDR, address, buffer, 1);
    return buffer[0];
}

void DS1307CleanRam ( void )
{
    UCHAR8 addr;
    for (addr = 8; addr < 64; addr++) {
        DS1307SaveRAMByte ( addr, 0 );
    }
}

void DS1307ReadFullRam ( void )
{
    UCHAR8 addr;
    UCHAR8 *ptr;

    i2c_master_cmd_read(i2cx,DS1307_I2C_ADDR,DS1307_ROM_START,buffer, 56);
    ptr = buffer;
    printf ( "\nDS1307 Full RAM Read 56 Bytes" );
    printf ( "\n----------------------------------------------------" );
    printf ( "\n     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" );
    printf ( "\n====================================================" );

    for (addr = 0; addr < 64; addr++) {
        if (!(addr & 0x0F)) printf ( "\n%02X:  ", addr >> 4 );
        printf ( "%02X ", *ptr );
        ptr++;
        ShortDelay ( delay_100us );
    }
    printf ( "\n" );

}