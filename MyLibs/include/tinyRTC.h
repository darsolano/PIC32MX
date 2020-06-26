/* 
 * File:   tinyRTC.h
 * Author: dsolano
 *
 * Created on June 16, 2014, 1:05 PM
 *  TinyRTCC v1.1 by weiman
 */

#ifndef TINYRTC_H
#define	TINYRTC_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <plib.h>
#include <GenericTypeDefs.h>
//********************************************************
//Definition for DS1307Z 64 x 8, Serial, I2C Real-Time Clock + 24C32WP + 18B09 1 wire
//********************************************************
// I2C address for DS1307
#define DS1307_I2C_ADDR             0x68
#define DS1307_I2C_CLOCK_FREQ       100000      // 100KHz
#define DS1307_I2C_BUS              I2C1

#define DS1307_REG_TIME             (0x00)
#define DS1307_REG_CONTROL          (0x07)
#define DS1307_ROM_START            (0x08)
#define DS1307_ROM_END              (0x3f)

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
}RTCDateTime;

typedef enum
{
    DS1307_1HZ          = 0x00,
    DS1307_4096HZ       = 0x01,
    DS1307_8192HZ       = 0x02,
    DS1307_32768HZ      = 0x03
} DS1307_sqw_t;



BOOL DS1307_initialize(RTCDateTime *t, I2C_MODULE i2cid);

void DS1307_setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
void DS1307_setDateTime_word(uint32_t t);
void DS1307_setDateTime_array(const char* date, const char* time);
void DS1307_getDateTime(RTCDateTime*);
BOOL DS1307_isReady(void);

DS1307_sqw_t DS1307_getOutput(void);
void DS1307_setOutput(DS1307_sqw_t mode);
void DS1307_enableOutput(BOOL enabled);
BOOL DS1307_isOutput(void);
void DS1307_enable32kHz(BOOL enabled);
BOOL DS1307_is32kHz(void);

char* DS1307_dateFormat(const char* dateFormat, RTCDateTime* dt);

#ifdef	__cplusplus
}
#endif

#endif	/* TINYRTC_H */

