#include "color_sensor.h"

uint16_t color[6] = {0, 0, 0, 0, 0, 0};
//uint16_t blackThreshold[6] = {4000, 3900, 4000, 3500, 3000, 2600};
uint32_t blackThresholdAddress[6] = {0, 1, 2, 3, 4, 5};
uint32_t whiteThresholdAddress[6] = {13, 7, 8, 9, 11, 12};
int blackThreshold[6] = {4000, 4000, 4000, 4000, 4000, 4000};
int whiteThreshold[6] = {700, 700, 700, 700, 700, 700};

void colorSensorInit() {
	adc1Init();
//	for(i = 0; i < 6; i++) {
//		temp = eepromRead(blackThresholdAddress[i]);
//		if(temp >= 0) {
//			blackThreshold[i] = temp;
//		}
//		temp = eepromRead(whiteThresholdAddress[i]);
//		if(temp >= 0) {
//			whiteThreshold[i] = temp;
//		}
//	}
//	lcd_clear();
//	sprintf(temp1, "%4d %4d %4d", blackThreshold[0], blackThreshold[1], blackThreshold[2]);
//	sprintf(temp2, "%4d %4d %4d", blackThreshold[3], blackThreshold[4], blackThreshold[5]);
//	sprintf(temp1, "%4d %4d %4d", whiteThreshold[0], whiteThreshold[1], whiteThreshold[2]);
//	sprintf(temp2, "%4d %4d %4d", whiteThreshold[3], whiteThreshold[4], whiteThreshold[5]);
//	lcd_putsf(0, 0, temp1);
//	lcd_putsf(0, 1, temp2);
//	delayMs(1000);
}

colorStatus checkColorSensor() {
	uint8_t i, black = 0, white = 0;

	for(i = 0; i < 6; i++) {
		color[i] = adc1Read(i);
		if(color[i] >= blackThreshold[i]) black++;
		else if(color[i] <= whiteThreshold[i]) white++;
	}

	if(black >= 5) return BLACK;
	else if(white >= 4) return WHITE;
	else if(white >= 2) return LINE;
	else return GRAY;
}

void tesAdc(void){
	uint8_t i;
	normal(300);
	while(!button3Pressed()){
		lcd_clear();
		char temp1[16], temp2[16];

		for(i = 0; i < 6; i++) {
			color[i] = adc1Read(i);
		}
		sprintf(temp1, "%4d %4d %4d", color[0], color[1], color[2]);
		sprintf(temp2, "%4d %4d %4d", color[3], color[4], color[5]);
		lcd_putsf(0, 0, temp1);
		lcd_putsf(0 ,1, temp2);
		delayMs(100);
	}
	while(button3Pressed());
}

void tesColorSensor() {
	colorStatus color;
	normal(300);
	while(!button3Pressed()) {
		color = checkColorSensor();
		if(color == BLACK) {
			lcd_putsf(0, 0, "BLACK");
		}
		if(color == GRAY) {
			lcd_putsf(0, 0, "GRAY");
		}
		if(color == WHITE) {
			lcd_putsf(0, 0, "WHITE");
		}
		if(color == LINE) {
			lcd_putsf(0, 0, "LINE");
		}
		delayMs(50);
		lcd_clear();
	}
	while(button3Pressed());
}

void timer5ISRCallBack(void){
	uint8_t colorCorrect = 0, i;
	if(colorTarget == LINE) {
		for(i = 0; i < 6; i++) {
			colorStatus colorChecking = checkColorSensor();
			if(colorChecking == LINE || colorChecking == WHITE){
				colorCorrect++;
			}
		}
	}
	else if(colorTarget == NOTLINE){
		for(i = 0; i < 6; i++) {
			colorStatus colorChecking = checkColorSensor();
			if(colorChecking == GRAY || colorChecking == BLACK){
				colorCorrect++;
			}
		}
	}
	else {
		for(i = 0; i < 6; i++) {
			if(checkColorSensor() == colorTarget){
				colorCorrect++;
			}
		}
	}
	if(colorCorrect >= 3) {
		colorMatch = 1;
		timer5Stop();
	}
}

void getBlackThreshold() {
	uint8_t i;
	char temp1[16], temp2[16];
	uint16_t colorMax[6] = {0, 0, 0, 0, 0, 0};
	normal(300);
	lcd_clear();
	lcd_putsf(0, 0, "BLACK THRESHOLD");
	lcd_putsf(0, 1, "Press OK!");
	while(!button4Pressed()) {
		if(button3Pressed()) {
			while(button3Pressed());
			delayMs(200);
			return;
		}
	}
	while(button4Pressed());
	delayMs(200);
	while(1) {
		for(i = 0; i < 6; i++) {
			color[i] = adc1ReadMean(i);
			if(color[i] > colorMax[i]) {
				colorMax[i] = color[i];
			}
		}
		sprintf(temp1, "%4d %4d %4d", colorMax[0], colorMax[1], colorMax[2]);
		sprintf(temp2, "%4d %4d %4d", colorMax[3], colorMax[4], colorMax[5]);
		lcd_clear();
		lcd_putsf(0, 0, temp1);
		lcd_putsf(0, 1, temp2);
		if(button4Pressed()) {
			while(button4Pressed());
			lcd_clear();
			lcd_putsf(0, 0, "GRAY THRESHOLD");
			lcd_putsf(0, 1, "Press OK!");
			while(!button4Pressed()) {
				if(button3Pressed()) {
					while(button3Pressed());
					delayMs(200);
					return;
				}
			}
			while(button4Pressed());
			delayMs(200);
			while(1) {
				for(i = 0; i < 6; i++) {
					color[i] = adc1ReadMean(i);
				}
				sprintf(temp1, "%4d %4d %4d", color[0], color[1], color[2]);
				sprintf(temp2, "%4d %4d %4d", color[3], color[4], color[5]);
				lcd_clear();
				lcd_putsf(0, 0, temp1);
				lcd_putsf(0, 1, temp2);
				if(button4Pressed()) {
					while(button4Pressed());
					for(i = 0; i < 6; i++) {
//						eepromWrite(blackThresholdAddress[i], (colorMax[i] - ((colorMax[i] - color[i]) / 4)));
						blackThreshold[i] = (colorMax[i] - ((colorMax[i] - color[i]) / 6));
					}
					delayMs(200);
					break;
				}
				if(button3Pressed()) {
					while(button3Pressed());
					delayMs(200);
					break;
				}
			}
			delayMs(200);
			break;
		}
		if(button3Pressed()) {
			while(button3Pressed());
			delayMs(200);
			break;
		}
	}
}

void getWhiteThreshold() {
	uint8_t i;
	char temp1[16], temp2[16];
	uint16_t colorMin[6] = {4095, 4095, 4095, 4095, 4095, 4095};
	normal(300);
	lcd_clear();
	lcd_putsf(0, 0, "WHITE THRESHOLD");
	lcd_putsf(0, 1, "Press OK!");
	while(!button4Pressed()) {
		if(button3Pressed()) {
			while(button3Pressed());
			delayMs(200);
			return;
		}
	}
	while(button4Pressed());
	delayMs(200);
	while(1) {
		for(i = 0; i < 6; i++) {
			color[i] = adc1ReadMean(i);
			if(color[i] < colorMin[i]) {
				colorMin[i] = color[i];
			}
		}
		sprintf(temp1, "%4d %4d %4d", colorMin[0], colorMin[1], colorMin[2]);
		sprintf(temp2, "%4d %4d %4d", colorMin[3], colorMin[4], colorMin[5]);
		lcd_clear();
		lcd_putsf(0, 0, temp1);
		lcd_putsf(0, 1, temp2);
		if(button4Pressed()) {
			while(button4Pressed());
			for(i = 0; i < 6; i++) {
//				eepromWrite(whiteThresholdAddress[i], colorMin[i] + 500);
				whiteThreshold[i] = colorMin[i] + 500;
			}
			delayMs(200);
			return;
		}
		if(button3Pressed()) {
			while(button3Pressed());
			delayMs(200);
			return;
		}
	}
}

void checkThreshold() {
	char temp1[16], temp2[16];
	lcd_clear();
	lcd_putsf(0, 0, "BLACK THRESHOLD");
	lcd_putsf(0, 1, "Press OK!");
	while(!button4Pressed()) {
		if(button3Pressed()) {
			while(button3Pressed());
			delayMs(200);
			return;
		}
	}
	while(button4Pressed());
	delayMs(200);

	lcd_clear();
	sprintf(temp1, "%4d %4d %4d", blackThreshold[0], blackThreshold[1], blackThreshold[2]);
	sprintf(temp2, "%4d %4d %4d", blackThreshold[3], blackThreshold[4], blackThreshold[5]);
	lcd_putsf(0, 0, temp1);
	lcd_putsf(0, 1, temp2);
	while(!button4Pressed()) {
		if(button3Pressed()) {
			while(button3Pressed());
			delayMs(200);
			return;
		}
	}
	while(button4Pressed());
	delayMs(200);

	lcd_clear();
	lcd_putsf(0, 0, "WHITE THRESHOLD");
	lcd_putsf(0, 1, "Press OK!");
	while(!button4Pressed()) {
		if(button3Pressed()) {
			while(button3Pressed());
			delayMs(200);
			return;
		}
	}
	while(button4Pressed());
	delayMs(200);

	lcd_clear();
	sprintf(temp1, "%4d %4d %4d", whiteThreshold[0], whiteThreshold[1], whiteThreshold[2]);
	sprintf(temp2, "%4d %4d %4d", whiteThreshold[3], whiteThreshold[4], whiteThreshold[5]);
	lcd_putsf(0, 0, temp1);
	lcd_putsf(0, 1, temp2);
	while(!button4Pressed()) {
		if(button3Pressed()) {
			while(button3Pressed());
			delayMs(200);
			return;
		}
	}
	while(button4Pressed());
	delayMs(200);
}

uint8_t checkColorWF(colorStatus color, double y, uint16_t time) {
	colorStatus colorChecking;
	uint8_t colorCorrect = 0;
	uint8_t i = 0;
	if(color == LINE) {
		for(i = 0; i < 6; i++) {
			colorChecking = checkColorSensor();
			if(colorChecking == LINE || colorChecking == WHITE){
				colorCorrect++;
			}

//			delayMs(50);
//			wf = 0;
//			return 1;

		}
	}
	else if(color == NOTLINE){
		for(i = 0; i < 6; i++) {
			colorChecking = checkColorSensor();
			if(colorChecking == GRAY || colorChecking == BLACK){
				colorCorrect++;
			}
		}
	}
	else {
		for(i = 0; i < 6; i++) {
			if(checkColorSensor() == color){
				colorCorrect++;
			}
		}
	}
	if(colorCorrect >= 4) {
//		delayMs(50);
		wf = 0;
		return 1;
	}
	else {
		uint8_t j;
		for(j = 0; j < 6; j++) {
			colorCorrect = 0;
			if(color == LINE) {
				delayMs(55);
				moveRobot(0, y, 0, time);
				for(i = 0; i < 6; i++) {
					colorChecking = checkColorSensor();
					if(colorChecking == LINE || colorChecking == WHITE){
						colorCorrect++;
					}
				}
			}
			else if(color == NOTLINE){
				for(i = 0; i < 6; i++) {
					colorChecking = checkColorSensor();
					if(colorChecking == GRAY || colorChecking == BLACK){
						colorCorrect++;
					}
				}
			}
			else {
				for(i = 0; i < 6; i++) {
					if(checkColorSensor() == color){
						colorCorrect++;
					}
				}
			}
			if(colorCorrect >= 4) {
//				delayMs(50);
				wf = 0;
				return 1;
			}
		}
	}
	colorMatch = 0;
	timer5Start(color);
	wf = 1;
	return 0;
}
