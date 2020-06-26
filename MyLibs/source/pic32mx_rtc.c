

#include <pic32mx_rtc.h>
#include <plib.h>
#include <stdio.h>
#include <string.h>
#include <proc/p32mx360f512l.h>

const uint8_t daysArray[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
const uint8_t dowArray[]  = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
char buffer[255];
#define ENABLE  1
#define DISABLE 0

uint32_t time = 0x16353000;
uint32_t date = 0x16050403;

static uint8_t 	bcd2dec(uint8_t bcd);
static uint8_t 	dec2bcd(uint8_t dec);
static uint32_t unixtime(RTCCDateTime *t);
static long 	time2long(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
static uint16_t date2days(uint16_t year, uint8_t month, uint8_t day);
static BOOL 	isLeapYear(uint16_t year);
static char 	*strDayOfWeek(uint8_t dayOfWeek);
static char 	*strMonth(uint8_t month);
static char 	*strAmPm(uint8_t hour, BOOL uppercase);
static char 	*strDaySufix(uint8_t day);
static uint8_t 	hour12(uint8_t hour24);
static uint8_t 	daysInMonth(uint16_t year, uint8_t month);
static uint16_t dayInYear(uint16_t year, uint8_t month, uint8_t day);

/*****************************************************
 PRIVATE FUNCTIONS
 *****************************************************/

static uint8_t bcd2dec(uint8_t bcd)
{
    return ((bcd / 16) * 10) + (bcd % 16);
}

static uint8_t dec2bcd(uint8_t dec)
{
    return ((dec / 10) * 16) + (dec % 10);
}

static long time2long(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    return ((days * 24L + hours) * 60 + minutes) * 60 + seconds;
}

static BOOL isLeapYear(uint16_t year)
{
    return (year % 4 == 0);
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

static uint32_t unixtime(RTCCDateTime *t)
{
    uint32_t u;

    u = time2long(date2days(t->year, t->month, t->day), t->hour, t->minute, t->second);
    u += 946681200;

    return u;
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

static uint16_t dayInYear(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t fromDate;
    uint16_t toDate;

    fromDate = date2days(year, 1, 1);
    toDate = date2days(year, month, day);

    return (toDate - fromDate);
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

/*****************************************************
 PUBLIC FUNCTIONS
 *****************************************************/
BOOL pic32_rtcc_init(void)
{
    mOSCEnableSOSC();
    RtccInit();
    while(RtccGetClkStat()!=RTCC_CLK_ON);	// wait for the SOSC to be actually running and RTCC to have its clock source
							// could wait here at most 32ms
    RtccEnable();

}

void rtcc_Get_Time_Date(RTCCDateTime *dt)
{
    rtccTime t;
    rtccDate d;
    
    RtccGetTimeDate(&t, &d);

    dt->second       = bcd2dec((uint8_t) (t.sec ));
    dt->minute       = bcd2dec((uint8_t) (t.min ));
    dt->hour         = bcd2dec((uint8_t) (t.hour));
    
    dt->day          = bcd2dec((uint8_t) (d.mday));
    dt->month        = bcd2dec((uint8_t) (d.mon ));
    dt->year         = bcd2dec((uint8_t) (d.year))+2000;
    
    dt->dayOfWeek    = RtccWeekDay((int)dt->year, (int)dt->month, (int)dt->day);
    
    dt->unixtime     = unixtime(dt);
}


void rtcc_Set_Time_Date(RTCCDateTime *dt)
{
    uint32_t date;
    uint32_t time;

    dt->dayOfWeek = RtccWeekDay((int)dt->year, (int)dt->month, (int)dt->day);

    time =      ((dec2bcd(dt->hour)   << 24) & 0xff000000) | 
                ((dec2bcd(dt->minute) << 16) & 0x00ff0000) | 
                ((dec2bcd(dt->second) << 8)  & 0x0000ff00);
        
    date =      (((dec2bcd(dt->year-2000)) << 24) & 0xff000000) | 
                (((dec2bcd(dt->month))     << 16) & 0x00ff0000) | 
                (((dec2bcd(dt->day))       << 8)  & 0x0000ff00) |
                (((dec2bcd(dt->dayOfWeek)) << 0)  & 0x000000ff);
    
    //RtccOpen(time, date,0);
    RtccSetTimeDate(time, date);
}

char* rtcc_Time_Date_Format(const char* dateFormat, RTCCDateTime* dt)
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

void rtcc_SetCLock_Alarm(RTCCDateTime *alarm)
{
// TODO
}