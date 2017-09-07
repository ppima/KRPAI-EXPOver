#include "stdint.h"
#include "uart.h"
#include <math.h>
#include "srf_tpa.h"
#include "jalan_dasar.h"

#define PI 3.1415926535897

#define SSC_COM COM3

#define COXA_LENGTH (double)4
#define FEMUR_LENGTH (double)6
#define TIBIA_LENGTH (double)11

double test = 0, eins, zwei, drei, vier;
uint16_t coxaPos, femurPos, tibiaPos;

//--------------- Prototype Private Function ----------------
void setServo(uint8_t servoPin, uint16_t position, uint16_t time, uint8_t servoGroup);
void servoGroupSend();
void setServoIK(legPosition *legPos, double x, double y, double z, uint16_t time);
void moveServoIK(legPosition *legPos, double x, double y, double z, uint16_t time);

//--------------- Private Variable ----------------
leg left, right;
static uint8_t swap = 1;
static uint8_t normalFlag = 0;
uint8_t wf = 0;

const double offsetY = 1.5;
const double normalZ = 2;
const double normalXMid = COXA_LENGTH + FEMUR_LENGTH;
const double normalXY = (sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2));
const double zUp = -2;

//--------------- Private Function ----------------
void setServo(uint8_t servoPin, uint16_t position, uint16_t time, uint8_t servoGroup) {
	char tempChar[25];
	if (position > 2100) position = 2100;
	else if (position < 900) position = 900;
	sprintf(tempChar, "#%d P%d T%d", servoPin, position, time);
	if(servoGroup) {
		uartSendString(SSC_COM, tempChar, NONE);
	}
	else {
		uartSendString(SSC_COM, tempChar, CR);
	}
}

void servoGroupSend() {
	uartSendString(SSC_COM, "", CR);
}

double coxaAngle, femurAngle, tibiaAngle;
double sCoxa, sFemur, sTibia;
double pCoxa, pFemur, pTibia;
double L, L1, femur1, femur2, zOffset;
double tes1, tes2, tes3;
void setServoIK(legPosition *legPos, double x, double y, double z, uint16_t time) {
	legPos -> pos.x = x;
	legPos -> pos.y = y;
	legPos -> pos.z = z;

	zOffset = (double)7.3 + legPos -> pos.z;
	L1 = sqrt(pow(legPos -> pos.x, 2) + pow(legPos -> pos.y, 2));
	L = sqrt(pow(zOffset, 2) + (pow(L1 - legPos -> coxa.length, 2)));

	femur1 = acos(zOffset / L);
	femur2 = acos((pow(legPos -> tibia.length, 2) - pow(legPos->femur.length, 2) - pow(L, 2)) / (-2 * legPos -> femur.length * L));
	tes1 = legPos->coxa.length;
	tes2 = legPos->femur.length;
	tes3 = legPos->tibia.length;
	coxaAngle = atan(legPos -> pos.x / legPos -> pos.y)  * 180 / PI;;
	femurAngle = 90 - ((femur1 + femur2) * 180 / PI);
	tibiaAngle = 90 - ((acos((pow(L, 2) - pow(legPos -> tibia.length, 2) - pow(legPos -> femur.length, 2)) / (-2 * legPos -> tibia.length * legPos -> femur.length)) * 180) / PI);

	coxaPos = legPos -> coxa.initAngle + (coxaAngle * 600 / 90);
	femurPos = legPos -> femur.initAngle + (femurAngle * 600 / 90);
	tibiaPos = legPos -> tibia.initAngle + (tibiaAngle * 600 / 90);

	setServo(legPos -> coxa.pin, coxaPos, time, 1);
	setServo(legPos -> femur.pin, femurPos, time, 1);
	setServo(legPos -> tibia.pin, tibiaPos, time, 1);
}

void tesServo(uint16_t time){
	double temp = sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2);
	double temp2 = COXA_LENGTH + FEMUR_LENGTH;
	double normalZ1 = -1.5;

	double angle, x, y, z;
	x = 0;
	y = -3;
	z = 0;
	angle = 0;

	setServoIK(&right.rear, temp, 0, normalZ, 300);
}

void servoInit() {
	double temp = sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2);

	left.front.type = 0;
	left.front.pos.x = temp;
	left.front.pos.y = temp;
	left.front.pos.z = 0;
	left.front.coxa.pin = 0;
	left.front.coxa.length = COXA_LENGTH;
	left.front.coxa.minAngle = 45;
	left.front.coxa.maxAngle = 45;
	left.front.femur.pin = 1;
	left.front.femur.length = FEMUR_LENGTH;
	left.front.femur.minAngle = 90;
	left.front.femur.maxAngle = 90;
	left.front.tibia.pin = 3;
	left.front.tibia.length = TIBIA_LENGTH;
	left.front.tibia.minAngle = 90;
	left.front.tibia.maxAngle = 90;

	left.mid.type = 1;
	left.mid.pos.x = 10;
	left.mid.pos.y = 0;
	left.mid.pos.z = 0;
	left.mid.coxa.pin = 4;
	left.mid.coxa.length = COXA_LENGTH;
	left.mid.coxa.minAngle = 40;
	left.mid.coxa.maxAngle = 20;
	left.mid.femur.pin = 5;
	left.mid.femur.length = FEMUR_LENGTH;
	left.mid.femur.minAngle = 90;
	left.mid.femur.maxAngle = 90;
	left.mid.tibia.pin = 6;
	left.mid.tibia.length = TIBIA_LENGTH;
	left.mid.tibia.minAngle = 90;
	left.mid.tibia.maxAngle = 90;

	left.rear.type = 2;
	left.rear.pos.x = temp;
	left.rear.pos.y = -temp;
	left.rear.pos.z = 0;
	left.rear.coxa.pin = 8;
	left.rear.coxa.length = COXA_LENGTH;
	left.rear.coxa.minAngle = 45;
	left.rear.coxa.maxAngle = 45;
	left.rear.femur.pin = 9;
	left.rear.femur.length = FEMUR_LENGTH;
	left.rear.femur.minAngle = 90;
	left.rear.femur.maxAngle = 90;
	left.rear.tibia.pin = 10;
	left.rear.tibia.length = TIBIA_LENGTH;
	left.rear.tibia.minAngle = 90;
	left.rear.tibia.maxAngle = 90;

	right.front.type = 0;
	right.front.pos.x = temp;
	right.front.pos.y = temp;
	right.front.pos.z = 0;
	right.front.coxa.pin = 16;
	right.front.coxa.length = COXA_LENGTH;
	right.front.coxa.minAngle = 45;
	right.front.coxa.maxAngle = 45;
	right.front.femur.pin = 17;
	right.front.femur.length = FEMUR_LENGTH;
	right.front.femur.minAngle = 90;
	right.front.femur.maxAngle = 90;
	right.front.tibia.pin = 18;
	right.front.tibia.length = TIBIA_LENGTH;
	right.front.tibia.minAngle = 90;
	right.front.tibia.maxAngle = 90;

	right.mid.type = 1;
	right.mid.pos.x = 10;
	right.mid.pos.y = 0;
	right.mid.pos.z = 0;
	right.mid.coxa.pin = 20;
	right.mid.coxa.length = COXA_LENGTH;
	right.mid.coxa.minAngle = 40;
	right.mid.coxa.maxAngle = 20;
	right.mid.femur.pin = 21;
	right.mid.femur.length = FEMUR_LENGTH;
	right.mid.femur.minAngle = 90;//130;
	right.mid.femur.maxAngle = 90;
	right.mid.tibia.pin = 22;
	right.mid.tibia.length = TIBIA_LENGTH;
	right.mid.tibia.minAngle = 90;
	right.mid.tibia.maxAngle = 90;

	right.rear.type = 2;
	right.rear.pos.x = temp;
	right.rear.pos.y = -temp;
	right.rear.pos.z = 0;
	right.rear.coxa.pin = 24;
	right.rear.coxa.length = COXA_LENGTH;
	right.rear.coxa.minAngle = 45;
	right.rear.coxa.maxAngle = 45;
	right.rear.femur.pin = 25;
	right.rear.femur.length = FEMUR_LENGTH;
	right.rear.femur.minAngle = 90;//135;
	right.rear.femur.maxAngle = 90;
	right.rear.tibia.pin = 26;
	right.rear.tibia.length = TIBIA_LENGTH;
	right.rear.tibia.minAngle = 90;
	right.rear.tibia.maxAngle = 90;
}
