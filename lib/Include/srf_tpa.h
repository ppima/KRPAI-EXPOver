//--------------------------------------------------------------
// File     : srf_tpa.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __SRF_TPA_H
#define __SRF_TPA_H

#include "stdint.h"
#include "proximity.h"

typedef struct tpaValue {
	uint16_t tpaTemp;
	uint8_t tpaPos;
} tpaValue;

extern uint8_t stopSearch;
extern uint8_t tpa81Fire;
extern uint8_t tpa81Ok;
extern uint8_t proximityDetected;
extern uint16_t tpa81Value;
extern uint16_t oldTpa81Value;
extern uint16_t tpa81ValueStop;
extern uint16_t tpa81Correct;
extern uint16_t tpa81Max;

uint8_t median(uint8_t value[], uint8_t sample);
void sonarSetRange(uint8_t addr, uint16_t jauh);
void sonarSetGain(uint8_t addr, uint16_t gain);
void sonarRange(uint8_t addr);
uint16_t sonarGet(uint8_t addr);
void srfAddressing(uint8_t oldAddr, uint8_t newAddr);
void tpaSetServo(unsigned char position);
uint16_t tpaRead(unsigned char pixelNum);
uint16_t getTpa81();
tpaValue tpaScanGetPosition();
void tpaCheckDelayMs(uint32_t time);
void tpa81ResetValue(uint16_t tpa81ValStop);

//--------------------------------------------------------------
#endif // __SRF_TPA_H
