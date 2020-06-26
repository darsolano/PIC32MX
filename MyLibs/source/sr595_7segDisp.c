


#include "sr595_7segDisp.h"
/*
 *        a
        ------
 *      |    |b
 *     f| g  |
 *      ------
 *      |    |c
 *    e |    |
 *      ------ o h
 *         d
 */
//Number definition
const UCHAR8 display7s[] = {
//     hgfedcba
    0b00111111,             // 0
    0b00000110,             // 1
    0b01011011,             // 2
    0b01001111,             // 3
    0b01100110,             // 4
    0b01101101,             // 5
    0b01111101,             // 6
    0b00000111,             // 7
    0b01111111,             // 8
    0b01101111,             // 9
    0b01110111,             // A
    0b01111100,             // B
    0b00111001,             // C
    0b01011110,             // D
    0b01111001,             // E
    0b01110001,             // F
    0b10000000				// Dot
//     hgfedcba
};

/*
 *        a
        ------
 *      |    |b
 *     f| g  |
 *      ------
 *      |    |c
 *    e |    |
 *      ------ o h
 *         d
 */
const UCHAR8 display7sLetters[] = {
//     hgfedcba
	0b00000000,				// space
    0b01110111,             // A
    0b01111100,             // B
    0b00111001,             // C
    0b01011110,             // D
    0b01111001,             // E
    0b01110001,             // F
	0b01101111,				// G
	0b01110110,				// H
	0b00110000,				// I
	0b00011110,				// J
	0b00000000,				// K
	0b00111000,				// L
	0b00000000,				// M
	0b01010100,				// N
	0b00111111,				// O
	0b01110011,				// P
	0b10111111,				// Q
	0b01010000,				// R
	0b01101101,				// S
	0b01111000,				// T
	0b00111110,				// U
	0b00011100,				// V
	0b00000000,				// W
	0b00000000,				// X
	0b01101110,				// Y
	0b00000000				// Z
};

static UCHAR8 LastDisplayChar;

void InitShiftRegister(void){
	TRISDbits.TRISD8 = 0;	//Set pin for Shift Register Select direction
	SR_LATCH_OFF();
	SpiInit(SR_CHANNEL);
	start_timer();
}

UCHAR8 GetLastDisplayChar(){
	return (LastDisplayChar);
}

void DisplayNumber(UINT8 number, TYPEOF_t type){
	UCHAR8 datalow;
	UCHAR8 datahi;
	if (type == Letter){
		if (number == 0x20) number = 0x41;
		if (number > 0x40 && number < 0x5a){
			datalow = (number-0x40);
			datahi = 0;
			WriteShiftRegister(display7sLetters[datahi]);
			WriteShiftRegister(display7sLetters[datalow]);
		}
	}
	if (type == Number){
		datalow = 0x0f & number;
		datahi = (0xf0 & number) >> 4;
		WriteShiftRegister(display7s[datahi]);
		WriteShiftRegister(display7s[datalow]);
		}
}

void ClearDot(void){
	UCHAR8 data;
	data = LastDisplayChar & CLEAR_DOT;
	WriteShiftRegister(data);
}


void SetDot(void){
	UCHAR8 data;
	data = LastDisplayChar | SET_DOT;
	WriteShiftRegister(data);
}

void WriteShiftRegister(UCHAR8 data){
	LastDisplayChar = data;
	SR_LATCH_ON();
	ShortDelay(delay_10us);
	SR_LATCH_OFF();
	SpiIO(SR_CHANNEL,data);
	SR_LATCH_ON();
	ShortDelay(delay_10us);
	SR_LATCH_OFF();
}

void Count2Display(){
	UINT8 i = 0;
	for(i=0;i<0xff;i++){
		DisplayNumber(i,Number);
		ShortDelay(delay_qtr_sec);
	}
}

void ClearDisplays(void){
	UINT8 i;
	for(i=0;i<2;i++)
		WriteShiftRegister(0);
}

void ShowLetters(void){
	UINT8 i = 0;
	for(i='A';i<'[';i++){
		DisplayNumber(i,Letter);
		ShortDelay(delay_1sec);
	}

}