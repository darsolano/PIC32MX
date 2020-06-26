/*
 * dht11.h
 *
 *  Created on: Dec 20, 2018
 *      Author: dsolano
 */

#ifndef INC_ELEGOO_DHT11_H_
#define INC_ELEGOO_DHT11_H_

#include <xc.h>
#include <GenericTypeDefs.h>

#define DHT11_DEBUG		0 // Be carefull with debuging to UART, adds time to the DHT11 signals and could affect the readings and timming

typedef struct{
	uint8_t humidity_entire;
	uint8_t humidity_decimal;
	uint8_t temperature_entire;
	uint8_t temperature_decimal;
	uint8_t chksum;
	BOOL valid_data;
	float rel_humidity;
	float temperature;
}DHT11_t;

BOOL 	dht11_Init		(DHT11_t* DHTx);
BOOL 	dht11_Read_Data	(DHT11_t *DHTx);


#endif /* INC_ELEGOO_DHT11_H_ */
