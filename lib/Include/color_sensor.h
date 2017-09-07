#ifndef _COLORSENSOR_H_
#define _COLORSENSOR_H_

typedef enum {
	BLACK = 0,
	GRAY = 1,
	LINE = 2,
	WHITE = 3,
	HOME = 4,
	NOTLINE = 5,
	NOCOLOR = 6,
} colorStatus;

#include "adc1.h"
#include "timer5.h"
#include "lcd.h"
#include "jalan_dasar.h"
#include "tombol.h"
#include "stdio.h"

void colorSensorInit();
colorStatus checkColorSensor();
void tesAdc();
void getBlackThreshold();
void getWhiteThreshold();
uint8_t checkColorWF(colorStatus color, double y, uint16_t time);

#endif
