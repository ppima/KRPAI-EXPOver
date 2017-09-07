/* 8/10/2014
 * Adika Bintang Sulaeman, Beroda 2012
 * This library is used for SRF08 or SRF10 and TPA81
 * It uses i2c2, with available SCL on PB6, SDA on PB7.
 * Make sure SDA and SCL have pull-up resistors, which the value can be set
 * from 1k8 to 4k7, but 1k8 to 2k is preferable.
 *
 * If there is something weird when reading shorter or longer distance, try
 * to change the value of i2c2_TIMEOUT. The smaller it is, the shorter minimum value
 * it can read, and vice versa.
 *
 */

#include "srf_tpa.h"
#include "i2c2.h"
#include "delay.h"

uint16_t maxRange = 70;
uint8_t position = 27;
uint8_t stopSearch = 0;
uint8_t tpa81Fire = 0;
uint8_t tpa81Ok = 0;
uint8_t proximityDetected = 0;
uint16_t tpa81Value = 0, oldTpa81Value = 0, tpa81Correct = 0, tpa81ValueStop = 0;
uint16_t tpa81Max = 190;	//TPA Threshold for optimum distance to extinguish

uint8_t median(uint8_t value[], uint8_t sample) {
	uint8_t i, j, temp;
	for(i = 0; i < sample-1; i++) {
		for(j = i + 1; j < sample; j++) {
			if(value[j] < value[i]) {
				// swap elements
				temp = value[i];
				value[i] = value[j];
				value[j] = temp;
			}
		}
	}

	if(sample%2==0) {
		// if there is an even number of elements, return mean of the two elements in the middle
		return((value[sample/2] + value[sample/2 - 1]) / 2);
	} else {
		// else return the element in the middle
		return value[sample/2];
	}
}

/* sonarSetRange() is used to set the maximum range
 * e.g: sonarSetRange(0xE0, 500) will set the maximum range to be 500 mm (50 cm)
 */
void sonarSetRange(uint8_t addr, uint16_t jauh) {
	uint8_t r;

	maxRange = jauh/10;

	r = (uint8_t)((jauh - 43)/43) + 1;
	i2c2WriteByte(addr, 2, r);
}

/* sonarSetGain(addr, gain) is used to set the analogue gain
 * gain values : 0x00 to 0x10 or 0 to 16 (default is 16)
 * e.g: sonarSetRange(0xE0, 0x03)
 */
void sonarSetGain(uint8_t addr, uint16_t gain) {
	if (gain<=16 && gain>=0)
		i2c2WriteByte(addr, 1, gain);
	else
		i2c2WriteByte(addr, 1, 0x10); //default gain
}

/*
To start the SRF08 ranging you would write 0x51 to the command register at 0x00 like this:
1. Send a start sequence
2. Send 0xE0 ( I2C address of the SRF08 with the R/W bit low (even address)
3. Send 0x00 (Internal address of the command register)
4. Send 0x51 (The command to start the SRF08 ranging, in cm)
5. Send the stop sequence.
*/
void sonarRange(uint8_t addr) {
	i2c2WriteByte(addr,0x00,0x51);
}

uint16_t sonarGet(uint8_t addr) {
	uint16_t r;
//	int16_t timeOut = 100000;
	do {
		r = i2c2ReadByte(addr, 0); //harusnya register 0
//		if(timeOut <= 0) {
//			return maxRange;
//		}
//		else {
//			timeOut = timeOut - 1;
//		}
	}
	while(r == 0xFF); //will be set high if there is no input //harusnya 0xFF

	r = 0;
	r = i2c2ReadByte(addr, 2) << 8;
	r += i2c2ReadByte(addr, 3);

	if (r >= maxRange || r <= 0)
		r = maxRange;

	return (uint16_t)r;
}

void srfAddressing(uint8_t oldAddr, uint8_t newAddr) {
	i2c2WriteByte(oldAddr, 0x00, 0xA0);
	i2c2WriteByte(oldAddr, 0x00, 0xAA);
	i2c2WriteByte(oldAddr, 0x00, 0xA5);
	i2c2WriteByte(oldAddr, 0x00, newAddr);
}

void tpaSetServo(unsigned char pos) {
	position = pos;
	i2c2WriteByte(0xD0, 0x00, pos);
}

uint16_t tpaRead(unsigned char pixelNum) {     /* QC */
    uint16_t data = 0;

    data = i2c2ReadByte(0xD0, pixelNum);  // Internal register address will increment automatically if we pass(1), not otherwise

    return data;
}

uint16_t getTpa81() {
    short pixel = 0;
    uint16_t max = 0;
    uint16_t temp = max;
    for (pixel = 0; pixel <= 7; pixel++) {
        temp = tpaRead(pixel + 2);
        if (temp > max) {
          	max = temp;
        }
    }
    return max;
}

tpaValue tpaScanGetPosition() {
	uint16_t tpaTemp = 0, tpaMaxTemp = 0;
	uint8_t tpaMaxPosition = 0;
	tpaValue tpaValues = {0, 0};

	if(position != 27) {
		tpaSetServo(27);
		if(position <= 26 && position >= 4) {
			delayMs((27 - position) * 8);
		}
		else {
			delayMs(200);
		}
	}
	for(position = 27; position >= 4; position--) {
		tpaSetServo(position);
		delaySetTime2(40000);
		while(delayTime2() != 0) {
			tpaTemp = getTpa81();
			if(tpaTemp > tpaMaxTemp) {
				tpaMaxTemp = tpaTemp;
				tpaMaxPosition = position;
			}
		}
	}
	tpaSetServo(tpaMaxPosition);
	tpaValues.tpaTemp = tpaMaxTemp;
	tpaValues.tpaPos = tpaMaxPosition;

	return tpaValues;
}

void tpaCheckDelayMs(uint32_t time) {
	delaySetTime2(time * 1000);
	uint8_t tpaThres = 34;
	while(delayTime2() != 0) {
		oldTpa81Value = tpa81Value;
		tpa81Value = getTpa81();
		if(tpa81ValueStop != 0) {
			if(!stopSearch) {
				if(tpa81Value >= tpa81ValueStop) {
					stopSearch = 1;
				}
			}
		}
		if(!tpa81Fire) {
			if(!tpa81Ok) {
				if(tpa81Value > tpaThres) {
					tpa81Ok = 1;
				}
			}
			else {
				if(tpa81Value < oldTpa81Value || tpa81Value <= tpaThres + 2) {
					tpa81Correct++;
				}
				else if(tpa81Value == oldTpa81Value) {

				}
				else {
					tpa81Correct = 0;
				}
				if(tpa81Correct > 3) {
					tpa81Fire = 1;
				}
			}
		}
		if(!proximityDetected) {
			if(proximityCheck()) {
				proximityDetected = 1;
			}
		}
	}
}

void tpa81ResetValue(uint16_t tpa81ValStop)
{
	tpa81ValueStop = tpa81ValStop;
	stopSearch = 0;
	tpa81Fire = 0;
	tpa81Ok = 0;
	proximityDetected = 0;
	tpa81Value = 0;
	oldTpa81Value = 0;
}




