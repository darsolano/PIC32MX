/*
 * dht11.c
 *
 *  Created on: Dec 20, 2018
 *      Author: dsolano
 */


#include <dht11.h>
#include <define_pins.h>
#include <string.h>
#include <utilities.h>

#if DHT11_DEBUG
#include <UART_app.h>
#include <stdio.h>
#include <peripheral/ports.h>
#endif

#define DHT11_TIMER_uS					1

#define DHT11_TIMER_INTERVAL_uS			1000000
#define DHT11_TIMER_INTERVAL_mS			1000

#define DHT11_START_SIGNAL_TIME			20000	// 20 millisenconds
#define DHT11_MIN_WAIT_TIME				20	// and datasheet says 20-40 microseconds but reads 14
#define DHT11_MAX_WAIT_TIME				40	// and datasheet says 20-40 microseconds but reads 14
#define DHT11_RESPONSE_SIGNAL_TIME_LOW	80	// microseconds start of response and datasheet says 80us
#define DHT11_RESPONSE_SIGNAL_TIME_HIGH	80	// microseconds end of response and datasheet says 80us
#define DHT11_START_TRANSMISSION_SIGNAL	54	// microseconds and datasheet says 50us
#define DHT11_BIT_HIGH_TIME				70	// microseconds and datasheet says 70
#define DHT11_BIT_LOW_TIME				24	// microseconds and datasheet says 26-28
#define DHT11_BIT_MASK					0x01
#define DHT11_TIMEOUT					100	// micro
#define DHT11_OFFSET					10
#define HIGH							1
#define LOW								0

volatile int curr_time;

DEFINE_PIN(DHT11_SIGNAL,G,1)	// GPIO SIgnal pin functions

/**************************************************
 * 
 **************************************************/
static BOOL dht11_get_checksum(DHT11_t* DHTx){
	uint16_t chksum;
	chksum = DHTx->humidity_entire 		+
			 DHTx->humidity_decimal 	+
			 DHTx->temperature_entire 	+
			 DHTx->temperature_decimal;
	chksum &= 0xff;

#if DHT11_DEBUG
	printf("Checksum : %d \n",chksum);
#endif

	if (chksum == DHTx->chksum) return TRUE;
	else return FALSE;
}

/**************************************************
 * 
 **************************************************/
static BOOL dht11_read_response(uint8_t maxtime,uint8_t offset, uint8_t xpect_value){
    DHT11_SIGNAL_OUTPUT();
    DHT11_SIGNAL_LOW();
	DHT11_SIGNAL_INPUT();
    
	curr_time = 0;
	timer_start();
    if (xpect_value == HIGH){
		do{
			if (timer_get_counter() >= maxtime+offset) break;    // This is a timeout check
		}while (DHT11_SIGNAL_READ());
	}
    
    if (xpect_value == LOW){
		do{
			if (timer_get_counter() >= maxtime+offset) break;    // This is a timeout check
		}while (!DHT11_SIGNAL_READ());
	}
    curr_time = timer_stop();
    
	if ((curr_time < (maxtime-offset)) && (curr_time > (maxtime+offset))){
#if DHT11_DEBUG
		printf("Response error...curr_time: %d maxtime: %d offset: %d @ Expected_value: %d \n",curr_time, maxtime, offset, xpect_value);
#endif
		return 0;
	}
    
	else{
		return 1;
	}
}

/**************************************************
 * 
 **************************************************/
static char dht11_read_data_bits(void){
	DHT11_SIGNAL_INPUT();

    //start transmission signal
    if (!dht11_read_response(DHT11_START_TRANSMISSION_SIGNAL,5,LOW)) return 3;

    DHT11_SIGNAL_OUTPUT();
    DHT11_SIGNAL_LOW();
	DHT11_SIGNAL_INPUT();

	// Start reading the bits 70us is a High 24-28us is a low
	curr_time = 0;
	timer_start();
	while(DHT11_SIGNAL_READ()){
		if (timer_get_counter() >= DHT11_BIT_HIGH_TIME+5) break;
	};
	curr_time = timer_stop();

	// Verify the time spam is correct for both high bit and low bit
	if ((curr_time > (DHT11_BIT_HIGH_TIME-5)) && (curr_time < (DHT11_BIT_HIGH_TIME+5))){
		return 1;
	}else
	if ((curr_time > (DHT11_BIT_LOW_TIME-5)) && (curr_time < (DHT11_BIT_LOW_TIME+5))){
		return 0;
	}else{
#if DHT11_DEBUG
		printf("Read Data Bits error...curr_time: %d \n",curr_time);
#endif
		return 2;
	}
}

/**************************************************
 * 
 **************************************************/
static void dht11_read_data_frame(DHT11_t* DHTx){
	char d_bit;
	int byte;
	int bit;
	uint8_t data;
	uint8_t data_all[5];

    // SEND TO DHT11 START UP SIGNAL 18ms
	DHT11_SIGNAL_OUTPUT();
    timer_start();
    do{
        DHT11_SIGNAL_LOW();
    }while (timer_get_counter() <= DHT11_START_SIGNAL_TIME);
    curr_time = timer_stop();
#if DHT11_DEBUG
    printf("Step 1 Start Signal OK...curr_time: %d \n",curr_time);
#endif

    //ShortDelay(DHT11_START_SIGNAL_TIME);
    
	// Wait for 40us as input the pullup will high the pin -- Step 2
	DHT11_SIGNAL_HIGH();
	curr_time = 0;
    DHT11_SIGNAL_INPUT();
    timer_start();
    
	do{
		if (timer_get_counter() >= DHT11_MAX_WAIT_TIME) break;
	}while (DHT11_SIGNAL_READ());
    
	curr_time = timer_stop();
    if ((curr_time < DHT11_MIN_WAIT_TIME-DHT11_OFFSET) || (curr_time > DHT11_MAX_WAIT_TIME+DHT11_OFFSET)){
#if DHT11_DEBUG
        printf("Step 2 Wait Time error...curr_time: %d \n",curr_time);
#endif
        return;
    }
#if DHT11_DEBUG
    printf("Step 2 Wait Time OK...curr_time: %d \n",curr_time);
#endif
    // DETECT DHT11 PRESENCE
	// wait from response 80us low and 80us high from DHT11 -- Step 3 --

	if (!dht11_read_response(DHT11_RESPONSE_SIGNAL_TIME_LOW , 5, LOW)) return;
	if (!dht11_read_response(DHT11_RESPONSE_SIGNAL_TIME_HIGH, 5, HIGH)) return;

	// Get all data bit from stream
	for (byte=0;byte<5;byte++){
		for (bit=0;bit<8;bit++){
			d_bit = dht11_read_data_bits();
			if (d_bit==0)
				data &= ~(DHT11_BIT_MASK<<(7-bit));	// Remember **Most** significant bit First
			else
			if (d_bit==1)
				data |=  (DHT11_BIT_MASK<<(7-bit));	// Remember **Most** significant bit First
			else {
				DHTx->valid_data = FALSE;
#if DHT11_DEBUG
				printf("Read Data frame error...Byte count: %d Bit count: %d Data: %d Bit rcvd: %d\n",byte, bit, data, d_bit);
#endif
				return;
			}
		}
		data_all[byte] = data;
		data = 0;
	}
    int16_t value;
	DHTx->humidity_entire 		= data_all[0];
	DHTx->humidity_decimal 		= data_all[1];
	DHTx->temperature_entire 	= data_all[2];
	DHTx->temperature_decimal 	= data_all[3];
	DHTx->chksum 				= data_all[4];
	DHTx->rel_humidity          = (float) DHTx->humidity_entire;
    DHTx->temperature           = (float)(DHTx->temperature_entire + (DHTx->temperature_decimal/10));

#if DHT11_DEBUG
	printf("Final ..HEnt%d HDec%d TEnt%d TDec%d CS%d  \n",data_all[0], data_all[1],data_all[2],data_all[3],data_all[4]);
#endif

	// Lets validate Checksum
	if (dht11_get_checksum(DHTx)){
		DHTx->valid_data = TRUE;
		return;
	}else {
		DHTx->valid_data = FALSE;
		return;
	}
}


/**************************************************
 * 
 **************************************************/
BOOL dht11_Init(DHT11_t* DHTx){
#if DHT11_DEBUG
	printf("PIC32MX360F512L DIGILENT - UNIVERSAL DEVELOPMENT BOARD - DHT11 test app...\n");
    printf("At (" __DATE__ "," __TIME__ ") \n");
#endif
	timer_init(DHT11_TIMER_INTERVAL_uS);
	dht11_read_data_frame(DHTx);
	if (DHTx->valid_data) return TRUE;
	else return FALSE;
}

/**************************************************
 * 
 **************************************************/
BOOL dht11_Read_Data(DHT11_t* DHTx){
	DHTx->valid_data = FALSE;
	dht11_read_data_frame(DHTx);
	if (DHTx->valid_data) return TRUE;
	else return FALSE;
}
