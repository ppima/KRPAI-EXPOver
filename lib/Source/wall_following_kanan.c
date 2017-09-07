#include "wall_following_kanan.h"

#define PI 3.1415926535897

#define MIN_DIST 12
#define MAX_DIST 30
#define MIN_DIST_FR 17
#define MIN_DIST_DG 15
#define MAX_DIST_DG 21
#define MID_DIST 15

#define MIN_ROOM 9
#define MID_ROOM 11
#define MAX_ROOM 14
#define MIN_FR_ROOM 17
#define MIN_DG_ROOM 15
#define MAX_DG_ROOM 21

#define Y_MAX (double)4.3
#define X_MAX (double)2.2

#define ANGLE_LIMIT_DOUBLE (double)60
#define ANGLE_LIMIT (double)30
#define SRF_1_TO_2_DIST (double)8

const uint8_t pingKanan[5]={0xE6, 0xE2, 0xE0, 0xE4, 0xEE};
const uint8_t pingKananMundur[3]={0xE6, 0xE8, 0xE4};
uint8_t srfFront, srfRDg, srfR1, srfR2, srfBack, srfL2, srfL1, srfLDg;
uint8_t i, j, value[4][5],g;

void kanan(uint16_t time, colorStatus color) {
	uint8_t right, left, correct = 0, colorCorrect = 0;
	char srf[4][4];
	colorMatch = 0;
	timer5Start(color);
	wf = 1;
	tpaSetServo(20);

	lcd_clear();
	if(color == BLACK)lcd_putsf(0,0,"WFKANAN BLACK");
	else if(color == GRAY) lcd_putsf(0,0,"WFKANAN GRAY");
	else if(color == WHITE) lcd_putsf(0,0,"WFKANAN HOME");
	else if(color == LINE) lcd_putsf(0,0,"WFKANAN LINE");
	else lcd_putsf(0,0,"WFKANAN NOTLINE");

	double x, y, angle;
	while(1) {
		x = 0, y = 0, angle = 0;

		if(colorMatch) {
			if(color == NOTLINE) {
				if(checkColorWF(color, -3, time)) {
					colorCorrect++;
				}
				if(colorCorrect > 3) {
					return;
				}
				else {
					colorMatch = 0;
					timer5Start(color);
					wf = 1;
				}
			}
			else {
				if(checkColorWF(color, -3, time)) {
					return;
				}
			}
		}

		if(color == HOME) {
			sonarRange(0xE4);
			delayMs(10);
			sonarRange(0xEC);
			delayMs(10);
			right = sonarGet(0xE4);
			left = sonarGet(0xEC);

			if(right < 30 && left < 30){
				correct++;
			}
			if(correct >= 4 ){
				kiri(time, WHITE);
				break;
			}
		}


		for(i = 0; i < 5; i++) {
			sonarRange(pingKanan[i]);
			delayMs(10);
		}
		srfR2 = sonarGet(pingKanan[0]);
		srfRDg = sonarGet(pingKanan[1]);
		srfFront = sonarGet(pingKanan[2]);
		srfR1 = sonarGet(pingKanan[3]);

		sprintf(srf[0], "%3i", srfFront);
		sprintf(srf[1], "%3i", srfRDg);
		sprintf(srf[2], "%3i", srfR1);
		sprintf(srf[3], "%3i", srfR2);

		lcd_putsf(0,1,srf[0]);
		lcd_putsf(4,1,srf[1]);
		lcd_putsf(8,1,srf[2]);
		lcd_putsf(12,1,srf[3]);

		srfLDg = sonarGet(pingKanan[4]);

		if(srfFront <= MIN_DIST_FR) {
			if(srfLDg <= MIN_DIST_DG && srfRDg > MIN_DIST_DG) {
				angle = 15;
			}
			else {
				angle = -ANGLE_LIMIT;
			}
		}
		else if(srfLDg <= MIN_DIST_DG) {
			y = 1;
			x = 1;
			angle = 15;
		}
		else if(srfRDg <= MIN_DIST_DG) {
			y = 2;
			x = -1;
			angle = -15;
		}
		else if(srfR1 > MAX_DIST) {
			y = 2;
			x = 1;
			angle = 17.5;
		}
		else if(srfR2 > MAX_DIST) {
			y = 3.5;
			angle = 5;
		}
		else {
			if (srfR1 < srfR2) {
				x = srfR1 - MIN_DIST;
			}
			else {
				x = srfR2 - MIN_DIST;
			}
			y = 3.5;
			angle = -(atan((srfR2 - srfR1) / SRF_1_TO_2_DIST) * 180 / PI);
			if(angle > 10) {
				angle = 10;
			}
			else if(angle < -10) {
				angle = -10;
			}
		}

		if (x > X_MAX) {
			x = X_MAX;
		}
		else if (x < -X_MAX) {
			x = -X_MAX;
		}
		if (y > Y_MAX) {
			y = Y_MAX;
		}
		else if (y < -Y_MAX) {
			y = -Y_MAX;
		}
		if(angle > ANGLE_LIMIT) {
			angle = ANGLE_LIMIT;
		}
		else if (angle < -ANGLE_LIMIT) {
			angle = -ANGLE_LIMIT;
		}

		moveRobot(x, y, angle, time);
	}
}

void kananRoom(uint16_t time, colorStatus color){
	uint8_t right, left, correct = 0, colorCorrect = 0;
	char srf[4][4];
	colorMatch = 0;
	timer5Start(color);
	wf = 1;
	tpaSetServo(20);

	lcd_clear();
	if(color == BLACK)lcd_putsf(0,0,"WFKAN BLK RM");
	else if(color == GRAY) lcd_putsf(0,0,"WFKAN GRY RM");
	else if(color == WHITE) lcd_putsf(0,0,"WFKAN HOM RM");
	else if(color == LINE) lcd_putsf(0,0,"WFKAN LNE RM");
	else lcd_putsf(0,0,"WFKAN ~LNE RM");

	double x, y, angle;
	while(1) {
		x = 0, y = 0, angle = 0;

		if(colorMatch) {
			if(color == NOTLINE) {
				if(checkColorWF(color, -3, time)) {
					colorCorrect++;
				}
				if(colorCorrect > 3) {
					return;
				}
				else {
					colorMatch = 0;
					timer5Start(color);
					wf = 1;
				}
			}
			else {
				if(checkColorWF(color, -3, time)) {
					return;
				}
			}
		}

		for(i = 0; i < 5; i++) {
			sonarRange(pingKanan[i]);
			delayMs(15);
		}
		srfR2 = sonarGet(pingKanan[0]);
		srfRDg = sonarGet(pingKanan[1]);
		srfFront = sonarGet(pingKanan[2]);
		srfR1 = sonarGet(pingKanan[3]);

		sprintf(srf[0], "%3i", srfFront);
		sprintf(srf[1], "%3i", srfRDg);
		sprintf(srf[2], "%3i", srfR1);
		sprintf(srf[3], "%3i", srfR2);

		lcd_putsf(0,1,srf[0]);
		lcd_putsf(4,1,srf[1]);
		lcd_putsf(8,1,srf[2]);
		lcd_putsf(12,1,srf[3]);

		srfLDg = sonarGet(pingKanan[4]);

		if(srfFront <= MIN_FR_ROOM) {
			if(srfLDg <= MIN_DG_ROOM && srfRDg > MIN_DG_ROOM) {
				angle = 15;
			}
			else {
				angle = -ANGLE_LIMIT;
			}
		}
		else if(srfLDg <= MIN_DG_ROOM) {
			y = 1;
			x = 1;
			angle = 15;
		}
		else if(srfRDg <= MIN_DG_ROOM) {
			y = 2;
			x = -1;
			angle = -15;
		}
		else if(srfR1 > MAX_ROOM) {
			y = 2;
			x = 1;
			angle = 17.5;
		}
		else if(srfR2 > MAX_ROOM) {
			y = 3.5;
			angle = 10;
		}
		else {
			if (srfR1 < srfR2) {
				x = srfR1 - MIN_ROOM;
			}
			else {
				x = srfR2 - MIN_ROOM;
			}
			y = 3.5;
			angle = -(atan((srfR2 - srfR1) / SRF_1_TO_2_DIST) * 180 / PI);
			if(angle > 10) {
				angle = 10;
			}
			else if(angle < -10) {
				angle = -10;
			}
		}

		if (x > X_MAX) {
			x = X_MAX;
		}
		else if (x < -X_MAX) {
			x = -X_MAX;
		}
		if (y > Y_MAX) {
			y = Y_MAX;
		}
		else if (y < -Y_MAX) {
			y = -Y_MAX;
		}
		if(angle > ANGLE_LIMIT) {
			angle = ANGLE_LIMIT;
		}
		else if (angle < -ANGLE_LIMIT) {
			angle = -ANGLE_LIMIT;
		}

		moveRobot(x, y, angle, time);
	}
}

void rataKanan(uint16_t time) {
	int vs = 0;
	tpaSetServo(27);
	lcd_clear();
	lcd_putsf(0,0,"RATA KANAN");
	wf = 1;
	double angle;
	while(1) {
		angle = 0;

		delayMs(30);
		sonarRange(pingKanan[0]);
		delayMs(10);
		sonarRange(pingKanan[3]);
		delayMs(10);
		srfR2 = sonarGet(pingKanan[0]);
		srfR1 = sonarGet(pingKanan[3]);

		vs = srfR1 - srfR2;

		if(vs < -1 || vs > 1) {
			angle = (atan((srfR1 - srfR2) / SRF_1_TO_2_DIST) * 180 / PI);
		}
		else {
			wf = 0;
			moveNormal(time);
			break;
		}
		if(angle > ANGLE_LIMIT) {
			angle = ANGLE_LIMIT;
		}
		else if (angle < -ANGLE_LIMIT) {
			angle = -ANGLE_LIMIT;
		}
		moveRobot(0, 0, angle, time);
	}
}

void geserKanan(uint16_t time) {
	const uint8_t ping[2] = {0xE4, 0xE6};
	char temp[16];
	int vs = 0, dist;
	double x = 0, y = 0, angle = 0;
	tpaSetServo(27);
	wf = 1;
	lcd_clear();
	lcd_putsf(0,0, "GESER KANAN");
	while(1){
		y = 0;

		delayMs(30);
		for(i = 0; i < 2; i++) {
			sonarRange(ping[i]);
			delayMs(10);
		}
		srfR1 = sonarGet(ping[0]);
		srfR2 = sonarGet(ping[1]);
		sprintf(temp, "R1=%3d R2=%3d", srfR1, srfR2);
		lcd_putsf(0, 1, temp);

		vs = srfR1 - srfR2;
		dist = srfR1 - (MID_DIST);
		if(dist > 2 || dist < -2){
			if(vs > 1 || vs < -1 ){
				angle = (atan((srfR1 - srfR2) / SRF_1_TO_2_DIST) * 180 / PI);
				if(angle > 10) {
					angle = 10;
				}
				else if(angle < -10) {
					angle = -10;
				}
			}
			if(srfR1 > MID_DIST){
				x = 3;
			}
			else{
				x = (srfR1 - MID_DIST);
			}
		}
		else{
			wf = 0;
			if(vs > 1 || vs < -1 ){
				rataKanan(time);
			}
			else {
				moveNormal(time);
			}
			break;
		}
		moveRobot(x, y, angle, time);
	}
}

void geserKananRoom(uint16_t time){
	const uint8_t ping[2] = {0xE4, 0xE6};
	char temp[16];
	int vs = 0, dist;
	double x = 0, y = 0, angle = 0;
	tpaSetServo(27);
	wf = 1;
	lcd_clear();
	lcd_putsf(0,0, "GESER KANAN RM");
	while(1){
		y = 0;

		delayMs(30);
		for(i = 0; i < 2; i++) {
			sonarRange(ping[i]);
			delayMs(15);
		}
		srfR1 = sonarGet(ping[0]);
		srfR2 = sonarGet(ping[1]);
		sprintf(temp, "R1=%3d R2=%3d", srfR1, srfR2);
		lcd_putsf(0, 1, temp);

		vs = srfR1 - srfR2;
		dist = srfR1 - (MIN_ROOM + 1);
		if(dist > 2 || dist < -2){
			if(vs > 1 || vs < -1 ){
				angle = (atan((srfR1 - srfR2) / SRF_1_TO_2_DIST) * 180 / PI);
				if(angle > 10) {
					angle = 10;
				}
				else if(angle < -10) {
					angle = -10;
				}
			}
			if(srfR1 > MIN_ROOM + 1){
				x = 2;
			}
			else{
				x = (srfR1 - MIN_ROOM);
			}
		}
		else{
			wf = 0;
			if(vs > 1 || vs < -1 ){
				rataKanan(time);
			}
			else {
				moveNormal(time);
			}
			break;
		}
		moveRobot(x, y, angle, time);
	}

}

void putarKanan90(uint16_t time){
	uint8_t temp = wf;
	wf = 0;
	lcd_clear();
	uint8_t a;
	lcd_putsf(0,0,"PUTAR KANAN 90");
	//Untuk sepatu tpe pake 35
	for(a = 0; a < 2; a++) {
		moveRobot(0, 0, 31, time);
		moveRobot(0, 0, 31, time);
	}
	moveRobot(0, 0, 31, time);
	moveNormal(time);
	wf = temp;
}

void putarBalik(uint16_t time){
	uint8_t temp = wf;
	wf = 0;
	lcd_clear();
	uint8_t a;
	lcd_putsf(0,0,"PUTAR BALIK");
	//Untuk sepatu tpe looping 5 kali, angle = 30
	for(a = 0; a < 5; a++){
		moveRobot(0, 0, 31, time);
		moveRobot(0, 0, 30, time);
	}
	wf = 1;
	moveNormal(time);
	wf = temp;
}

uint8_t kananProx(uint16_t time, colorStatus color) {
	char srf[16][4];
	colorMatch = 0;
	timer5Start(color);
	wf = 1;
	tpaSetServo(27);

	lcd_clear();
	if(color == BLACK)lcd_putsf(0,0,"WFKANAN BLACK");
	else if(color == GRAY) lcd_putsf(0,0,"WFKANAN GRAY");
	else if(color == WHITE) lcd_putsf(0,0,"WFKIRI WHITE");
	else if(color == LINE) lcd_putsf(0,0,"WFKIRI LINE");
	else lcd_putsf(0,0,"WFKIRI NOTLINE");

	double x, y, angle;
	while(1) {
		x = 0, y = 0, angle = 0;

		if(colorMatch) {
			if(checkColorWF(color, -3, time)) {
				return 0;
			}
		}

		for(i = 0; i < 5; i++) {
			sonarRange(pingKanan[i]);
			delayMs(10);
		}
		srfR2 = sonarGet(pingKanan[0]);
		srfRDg = sonarGet(pingKanan[1]);
		srfFront = sonarGet(pingKanan[2]);
		srfR1 = sonarGet(pingKanan[3]);

		sprintf(srf[0], "%3i", srfFront);
		sprintf(srf[1], "%3i", srfRDg);
		sprintf(srf[2], "%3i", srfR1);
		sprintf(srf[3], "%3i", srfR2);

		lcd_putsf(0,1,srf[0]);
		lcd_putsf(4,1,srf[1]);
		lcd_putsf(8,1,srf[2]);
		lcd_putsf(12,1,srf[3]);

		srfLDg = sonarGet(pingKanan[4]);

		if(dollCheck() == 1 && srfR1 <= 36) {
			wf = 0;
			moveRobot(0, -3, 0, time);
			rataKanan(time);
			if(dollCheck() == 1) {
				colorStatus colorCheck;
				uint8_t blackCorrect = 0, grayCorrect = 0;
				for(i = 0; i < 11; i++) {
					colorCheck = checkColorSensor();
					if(colorCheck == BLACK){
						blackCorrect++;
					}
					else if(colorCheck == GRAY) {
						grayCorrect++;
					}
				}
				sonarRange(pingKiri[3]);
				srfL1 = sonarGet(pingKiri[3]);
				if(srfL1 <= 40) {
					moveRobot(0, -5, 0, time);
					moveRobot(0, -5, 0, time);
					moveRobot(0, -5, 0, time);
				}
				if(blackCorrect > grayCorrect) {
					return 1;
				}
				else {
					return 2;
				}
			}
			wf = 1;
		}

		if(srfFront <= MIN_DIST_FR) {
			if(srfLDg <= MIN_DIST_DG && srfRDg > MIN_DIST_DG) {
				angle = 15;
			}
			else {
				angle = -ANGLE_LIMIT;
			}
		}
		else if(srfLDg <= MIN_DIST_DG) {
			y = 1;
			x = 1;
			angle = 15;
		}
		else if(srfRDg <= MIN_DIST_DG) {
			y = 3;
			x = -1;
			angle = -15;
		}
		else if(srfR1 > MAX_DIST) {
			y = 3;
			x = 1;
			angle = 19;
		}
		else if(srfR2 > MAX_DIST) {
			y = 4;
		}
		else {
			if (srfR1 < srfR2) {
				x = srfR1 - MIN_DIST;
			}
			else {
				x = srfR2 - MIN_DIST;
			}
			y = 4;
			angle = -(atan((srfR2 - srfR1) / SRF_1_TO_2_DIST) * 180 / PI);
			if(angle > 10) {
				angle = 10;
			}
			else if(angle < -10) {
				angle = -10;
			}
		}

		if (x > X_MAX) {
			x = X_MAX;
		}
		else if (x < -X_MAX) {
			x = -X_MAX;
		}
		if (y > Y_MAX) {
			y = Y_MAX;
		}
		else if (y < -Y_MAX) {
			y = -Y_MAX;
		}
		if(angle > ANGLE_LIMIT) {
			angle = ANGLE_LIMIT;
		}
		else if (angle < -ANGLE_LIMIT) {
			angle = -ANGLE_LIMIT;
		}
		moveRobot(x, y, angle, time);
	}
}

void kananMundur(uint16_t time, colorStatus color) {
	char srf[16][4];
	colorMatch = 0;
	timer5Start(color);
	wf = 1;
	tpaSetServo(27);

	lcd_clear();
	if(color == BLACK)lcd_putsf(0,0,"WFKANAN BLACK");
	else if(color == GRAY) lcd_putsf(0,0,"WFKANAN GRAY");
	else if(color == NOTLINE) lcd_putsf(0,0,"WFKANAN NOTLINE");
	else lcd_putsf(0,0,"WFKANAN LINE");

	double x, y, angle;
	while(1) {
		x = 0, y = 0, angle = 0;

		if(colorMatch) {
			if(checkColorWF(color, 3, time)) {
				return;
			}
		}

		for(i = 0; i < 3; i++) {
			sonarRange(pingKananMundur[i]);
			delayMs(15);
		}
		delayMs(5);
		srfR2 = sonarGet(pingKananMundur[0]);
		srfBack = sonarGet(pingKananMundur[1]);
		srfR1 = sonarGet(pingKananMundur[2]);

		sprintf(srf[0], "%3i", srfBack);
		sprintf(srf[1], "%3i", srfR1);
		sprintf(srf[2], "%3i", srfR2);

		lcd_putsf(0,1,srf[0]);
		lcd_putsf(4,1,srf[1]);
		lcd_putsf(8,1,srf[2]);


		if (srfR1 < srfR2) {
			x = (srfR1 - MIN_DIST);
		}
		else {
			x = (srfR2 - MIN_DIST);
		}
		y = -6;
		angle = -(atan((srfR2 - srfR1) / SRF_1_TO_2_DIST) * 180 / PI);
		if(angle > 10) {
			angle = 10;
		}
		else if(angle < -10) {
			angle = -10;
		}

		if (x > X_MAX) {
			x = X_MAX;
		}
		else if (x < -X_MAX) {
			x = -X_MAX;
		}
		if (y > Y_MAX) {
			y = Y_MAX;
		}
		else if (y < -Y_MAX) {
			y = -Y_MAX;
		}
		if(angle > ANGLE_LIMIT) {
			angle = ANGLE_LIMIT;
		}
		else if (angle < -ANGLE_LIMIT) {
			angle = -ANGLE_LIMIT;
		}

		moveRobot(x, y, angle, time);
	}
}
