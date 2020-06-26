/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _pic32mx_rtc_H    /* Guard against multiple inclusion */
#define _pic32mx_rtc_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <xc.h>
#include <GenericTypeDefs.h>
#include <stdint.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t dayOfWeek;
    uint32_t unixtime;
}RTCCDateTime;

    
BOOL    pic32_rtcc_init(void);
void    rtcc_Get_Time_Date(RTCCDateTime *dt);
void    rtcc_Set_Time_Date(RTCCDateTime *dt);
void    rtcc_SetCLock_Alarm(RTCCDateTime *alarm);
char*   rtcc_Time_Date_Format(const char* dateFormat, RTCCDateTime* dt);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
