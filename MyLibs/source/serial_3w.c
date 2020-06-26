/*
 * serial_3w.c
 *
 *  Created on: Dec 4, 2015
 *      Author: dsolano
 */

#include <serial_3w.h>
#include <utilities.h>
#include <define_pins.h>

#include "max7219.h"

DEFINE_PIN(CLK,D,11)
DEFINE_PIN(DOUT,D,12)
DEFINE_PIN(CS,D,13)

void serial3wire_init(void)
{
    CS_OUTPUT();
    CLK_OUTPUT();
    DOUT_OUTPUT();
    CS_ASSERT();
    CLK_TOGGLE();
}

// SPI BitBang to send data 8 bits
void serial3wire_write_byte(uint8_t data)
{
	uint8_t temp , count;

	temp=data;
	CS_DEASSERT();
	CLK_HIGH();
	for( count=0; count<8; count++)
	{
		if(temp & 0x80)
			DOUT_HIGH();
		else
			DOUT_LOW();
		temp = temp << 1;
		// pseudo clock
		CLK_LOW();
		_nop();
		CLK_HIGH();
		_nop();
		CLK_LOW();
		_nop();
	}
	CS_ASSERT();
}

// SPI BitBang to send data 16 bits
void serial3wire_write_short(uint16_t data)
{
	uint16_t temp , count;

	temp=data;
	CS_DEASSERT();
	CLK_HIGH();
	for( count=0; count<16; count++)
	{
		if(temp & 0x8000)
			DOUT_HIGH();
		else
			DOUT_LOW();
		temp = temp << 1;
		// pseudo clock
		CLK_LOW();
		_nop();
		CLK_HIGH();
		_nop();
		CLK_LOW();
		_nop();
	}
	CS_ASSERT();
}

