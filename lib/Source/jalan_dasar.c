/**
 * Library Jalan Dasar SSC + IK
 *
 * @file	"jalan_dasar.c"
 * @author	Rudy Nurhadi
 * @version	09/Mei/2016
 *
 */

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

int test = 0, eins, zwei, drei, vier;

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

const double offsetY = -1.5;
const double normalZ = -1.5;
const double normalXMid = COXA_LENGTH + FEMUR_LENGTH;
const double normalXY = (sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2));
const double zUp = -2;

//--------------- Public Function ----------------
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
	left.front.coxa.initAngle = 1419;
	left.front.femur.pin = 1;
	left.front.femur.length = FEMUR_LENGTH;
	left.front.femur.minAngle = 90;
	left.front.femur.maxAngle = 90;
	left.front.femur.initAngle = 1467;
	left.front.tibia.pin = 3;
	left.front.tibia.length = TIBIA_LENGTH;
	left.front.tibia.minAngle = 90;
	left.front.tibia.maxAngle = 90;
	left.front.tibia.initAngle = 1470;

	left.mid.type = 1;
	left.mid.pos.x = 10;
	left.mid.pos.y = 0;
	left.mid.pos.z = 0;
	left.mid.coxa.pin = 4;
	left.mid.coxa.length = COXA_LENGTH;
	left.mid.coxa.minAngle = 40;
	left.mid.coxa.maxAngle = 20;
	left.mid.coxa.initAngle = 1519;
	left.mid.femur.pin = 5;
	left.mid.femur.length = FEMUR_LENGTH;
	left.mid.femur.minAngle = 90;
	left.mid.femur.maxAngle = 90;
	left.mid.femur.initAngle = 1482;
	left.mid.tibia.pin = 6;
	left.mid.tibia.length = TIBIA_LENGTH;
	left.mid.tibia.minAngle = 90;
	left.mid.tibia.maxAngle = 90;
	left.mid.tibia.initAngle = 1470;

	left.rear.type = 2;
	left.rear.pos.x = temp;
	left.rear.pos.y = -temp;
	left.rear.pos.z = 0;
	left.rear.coxa.pin = 8;
	left.rear.coxa.length = COXA_LENGTH;
	left.rear.coxa.minAngle = 45;
	left.rear.coxa.maxAngle = 45;
	left.rear.coxa.initAngle = 1529;
	left.rear.femur.pin = 9;
	left.rear.femur.length = FEMUR_LENGTH;
	left.rear.femur.minAngle = 90;
	left.rear.femur.maxAngle = 90;
	left.rear.femur.initAngle = 1497;
	left.rear.tibia.pin = 10;
	left.rear.tibia.length = TIBIA_LENGTH;
	left.rear.tibia.minAngle = 90;
	left.rear.tibia.maxAngle = 90;
	left.rear.tibia.initAngle = 1447;

	right.front.type = 0;
	right.front.pos.x = temp;
	right.front.pos.y = temp;
	right.front.pos.z = 0;
	right.front.coxa.pin = 16;
	right.front.coxa.length = COXA_LENGTH;
	right.front.coxa.minAngle = 45;
	right.front.coxa.maxAngle = 45;
	right.front.coxa.initAngle = 1506;
	right.front.femur.pin = 17;
	right.front.femur.length = FEMUR_LENGTH;
	right.front.femur.minAngle = 90;
	right.front.femur.maxAngle = 90;
	right.front.femur.initAngle = 1510;
	right.front.tibia.pin = 18;
	right.front.tibia.length = TIBIA_LENGTH;
	right.front.tibia.minAngle = 90;
	right.front.tibia.maxAngle = 90;
	right.front.tibia.initAngle = 1488;

	right.mid.type = 1;
	right.mid.pos.x = 10;
	right.mid.pos.y = 0;
	right.mid.pos.z = 0;
	right.mid.coxa.pin = 20;
	right.mid.coxa.length = COXA_LENGTH;
	right.mid.coxa.minAngle = 40;
	right.mid.coxa.maxAngle = 20;
	right.mid.coxa.initAngle = 1462;
	right.mid.femur.pin = 21;
	right.mid.femur.length = FEMUR_LENGTH;
	right.mid.femur.minAngle = 90;//130;
	right.mid.femur.maxAngle = 90;
	right.mid.femur.initAngle = 1485;
	right.mid.tibia.pin = 22;
	right.mid.tibia.length = TIBIA_LENGTH;
	right.mid.tibia.minAngle = 90;
	right.mid.tibia.maxAngle = 90;
	right.mid.tibia.initAngle = 1497;

	right.rear.type = 2;
	right.rear.pos.x = temp;
	right.rear.pos.y = -temp;
	right.rear.pos.z = 0;
	right.rear.coxa.pin = 24;
	right.rear.coxa.length = COXA_LENGTH;
	right.rear.coxa.minAngle = 45;
	right.rear.coxa.maxAngle = 45;
	right.rear.coxa.initAngle = 1544;
	right.rear.femur.pin = 25;
	right.rear.femur.length = FEMUR_LENGTH;
	right.rear.femur.minAngle = 90;//135;
	right.rear.femur.maxAngle = 90;
	right.rear.femur.initAngle = 1500;
	right.rear.tibia.pin = 26;
	right.rear.tibia.length = TIBIA_LENGTH;
	right.rear.tibia.minAngle = 90;
	right.rear.tibia.maxAngle = 90;
	right.rear.tibia.initAngle = 1500;
}

void cobaServo() {
	int x = 0, y = 0, z = 3.5;
	moveServoIK(&left.front, x, y, z, 300);
	moveServoIK(&left.mid, x, y, z, 300);
	moveServoIK(&left.rear, x, y, z, 300);
	moveServoIK(&right.front, -x, y, z, 300);
	moveServoIK(&right.mid, -x, y, z, 300);
	moveServoIK(&right.rear, -x, y, z, 300);
	servoGroupSend();
	delayMs(1000);
	moveServoIK(&left.front, -x, -y, -z, 300);
	moveServoIK(&left.mid, -x, -y, -z, 300);
	moveServoIK(&left.rear, -x, -y, -z, 300);
	moveServoIK(&right.front, x, -y, -z, 300);
	moveServoIK(&right.mid, x, -y, -z, 300);
	moveServoIK(&right.rear, x, -y, -z, 300);
	servoGroupSend();
	delayMs(1000);

//	double angle = 20;
//	uint16_t time = 500;
//	setCoxaAngle(&left.front, angle, time);
//	setCoxaAngle(&left.mid, angle, time);
//	setCoxaAngle(&left.rear, angle, time);
//	setCoxaAngle(&right.front, -angle, time);
//	setCoxaAngle(&right.mid, -angle, time);
//	setCoxaAngle(&right.rear, -angle, time);
//	delayMs(delayTime);
//	setCoxaAngle(&left.front, -angle, time);
//	setCoxaAngle(&left.mid, -angle, time);
//	setCoxaAngle(&left.rear, -angle, time);
//	setCoxaAngle(&right.front, angle, time);
//	setCoxaAngle(&right.mid, angle, time);
//	setCoxaAngle(&right.rear, angle, time);
//	delayMs(delayTime);

}

//In reva, normalZ are assigned as positive value
//In reinhardt, nomarZ are assigned as negative value

void normalInit(uint16_t time) {
	double temp = sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2);
	double temp2 = COXA_LENGTH + FEMUR_LENGTH;
	double normalZ1 = -5;
	temp -= 2.5;
	temp2 -= 3;
	setServoIK(&left.front, temp, temp, normalZ1, time);
	setServoIK(&left.mid, temp2, 0, normalZ1, time);
	setServoIK(&left.rear, temp - 0.5, -temp, normalZ1, time);
	setServoIK(&right.front, temp, temp, normalZ1, time);
	setServoIK(&right.mid, temp2, 0, normalZ1, time);
	setServoIK(&right.rear, temp, -temp, normalZ1, time);
	servoGroupSend();
	delayMs(500);
//	while(1);
	normal(time);
	delayMs(time);
}

void normal(uint16_t time) {
	if(!normalFlag) {
		normalFlag = 1;
		normalSwap0(time);
		normalSwap1(time);
		servoGroupSend();
	}
}

void normalSwap0(uint16_t time) {
	setServoIK(&right.front, normalXY, normalXY + offsetY, normalZ, time);
	setServoIK(&right.rear, normalXY, -normalXY + offsetY, normalZ, time);
	setServoIK(&left.mid, normalXMid, offsetY, normalZ, time);
}

void normalSwap1(uint16_t time) {
	setServoIK(&left.front, normalXY, normalXY + offsetY, normalZ, time);
	setServoIK(&left.rear, normalXY, -normalXY + offsetY, normalZ, time);
	setServoIK(&right.mid, normalXMid, offsetY, normalZ, time);
}

void nungging(double z, uint16_t time) {
	normalFlag = 0;

	setServoIK(&left.front, normalXY, normalXY, normalZ - 0.5, time);
	setServoIK(&left.mid, normalXMid, 0, normalZ - (z / 2), time);
	setServoIK(&left.rear, normalXY, -normalXY, normalZ - z, time);
	setServoIK(&right.front, normalXY, normalXY, normalZ + 0.5, time);
	setServoIK(&right.mid, normalXMid, 0, normalZ - 1 - (z / 2), time);
	setServoIK(&right.rear, normalXY, -normalXY, normalZ - 1 - z, time);
	servoGroupSend();
}

void miring(double z, uint16_t time) {
	normalFlag = 0;

	setServoIK(&left.front, normalXY, normalXY, normalZ - z, time);
	setServoIK(&left.mid, normalXMid, 0, normalZ - z - (z * 0.15), time);
	setServoIK(&left.rear, normalXY, -normalXY, normalZ - z, time);
	setServoIK(&right.front, normalXY, normalXY, normalZ + z, time);
	setServoIK(&right.mid, normalXMid, 0, normalZ + z + (z * 0.15), time);
	setServoIK(&right.rear, normalXY, -normalXY, normalZ + z, time);
	servoGroupSend();
}

void moveNormal1(uint16_t time){
	setServoIK(&left.front, normalXY, normalXY + offsetY, left.front.pos.z + zUp, time);
	setServoIK(&left.rear, normalXY, -normalXY + offsetY, left.rear.pos.z + zUp, time);
	setServoIK(&right.mid, normalXMid, offsetY, right.mid.pos.z + zUp, time);
	servoGroupSend();
	delayMs(time / 15);
	normalSwap0(time);
	servoGroupSend();
	delayMs(time);
	moveServoIK(&left.front, 0, 0, -zUp, time);
	moveServoIK(&left.rear, 0, 0, -zUp, time);
	moveServoIK(&right.mid, 0, 0, -zUp, time);
	servoGroupSend();
	delayMs(time);
}

void moveNormal(uint16_t time) {
	if(!normalFlag) {
		normalFlag = 1;

		if(time < 150) time = 150;
		uint16_t delayTime;
		if(wf) {
			 delayTime = time - 50;
		}
		else delayTime = time;

		if(swap == 0) {
			swap = 1;
			setServoIK(&left.front, normalXY, normalXY + offsetY, left.front.pos.z + zUp, time);
			setServoIK(&left.rear, normalXY, -normalXY + offsetY, left.rear.pos.z + zUp, time);
			setServoIK(&right.mid, normalXMid, offsetY, right.mid.pos.z + zUp, time);
			servoGroupSend();
			delayMs(time / 15);
			normalSwap0(time);
			servoGroupSend();
			delayMs(time);
			moveServoIK(&left.front, 0, 0, -zUp, time);
			moveServoIK(&left.rear, 0, 0, -zUp, time);
			moveServoIK(&right.mid, 0, 0, -zUp, time);
			servoGroupSend();
			delayMs(delayTime);
		}
		else if(swap == 1) {
			swap = 0;
			setServoIK(&right.front, normalXY, normalXY + offsetY, right.front.pos.z + zUp, time);
			setServoIK(&right.rear, normalXY, -normalXY + offsetY, right.rear.pos.z + zUp, time);
			setServoIK(&left.mid, normalXMid, offsetY, left.mid.pos.z + zUp, time);
			servoGroupSend();
			delayMs(time / 15);
			normalSwap1(time);
			servoGroupSend();
			delayMs(time);
			moveServoIK(&right.front, 0, 0, -zUp, time);
			moveServoIK(&right.rear, 0, 0, -zUp, time);
			moveServoIK(&left.mid, 0, 0, -zUp, time);
			servoGroupSend();
			delayMs(delayTime);
		}
	}
}

void moveNormal2(uint16_t time) {
	if(!normalFlag) {
		normalFlag = 1;

		if(time < 150) time = 150;
		uint16_t delayTime;
		if(wf) {
			 delayTime = time - 50;
		}
		else delayTime = time;

		setServoIK(&left.front, normalXY, normalXY + offsetY, left.front.pos.z + zUp, time);
		setServoIK(&left.rear, normalXY, -normalXY + offsetY, left.rear.pos.z + zUp, time);
		setServoIK(&right.mid, normalXMid, offsetY, right.mid.pos.z + zUp, time);
		servoGroupSend();
		moveServoIK(&left.front, 0, 0, -zUp, time);
		moveServoIK(&left.rear, 0, 0, -zUp, time);
		moveServoIK(&right.mid, 0, 0, -zUp, time);
		servoGroupSend();
		delayMs(time);
		setServoIK(&right.front, normalXY, normalXY + offsetY, right.front.pos.z + zUp, time);
		setServoIK(&right.rear, normalXY, -normalXY + offsetY, right.rear.pos.z + zUp, time);
		setServoIK(&left.mid, normalXMid, offsetY, left.mid.pos.z + zUp, time);
		servoGroupSend();
		delayMs(time);
		moveServoIK(&right.front, 0, 0, -zUp, time);
		moveServoIK(&right.rear, 0, 0, -zUp, time);
		moveServoIK(&left.mid, 0, 0, -zUp, time);
		servoGroupSend();
		delayMs(delayTime);
	}
}

void moveRobot(double x, double y, double angle, uint16_t time) {
	double temp = sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2);
	double temp2 = COXA_LENGTH + FEMUR_LENGTH;

	if(angle > 35) angle = 35;
	uint16_t delayTime;
	if(wf) {
		 delayTime = time - 50;
	}
	else {
		delayTime = time;
	}
	if(x != 0 || y != 0 || angle != 0) {
		normalFlag = 0;
		angle = angle / 2;
		double LFPos;
		double angleDeg = angle * PI / 180;
		double angleDeg1 = (angle + 45) * PI / 180;
		double angleDeg2 = (-angle + 45) * PI / 180;

		if(swap == 0) {
			swap = 1;

			double xLF = temp2 * cos(angleDeg1) - (x / 2);
			double yLF = temp2 * sin(angleDeg1) + offsetY + (y / 2);
			double zLF = left.front.pos.z;
			double xLR = temp2 * cos(angleDeg2) - (x / 2);
			double yLR = -temp2 * sin(angleDeg2) + offsetY + (y / 2);
			double zLR = left.rear.pos.z;
			double xRM = temp2 * cos(angleDeg) + (x / 2);
			double yRM = -temp2 * sin(angleDeg) + offsetY + (y / 2);
			double zRM = right.mid.pos.z;

			angleDeg = -angle * PI / 180;
			angleDeg1 = (-angle + 45) * PI / 180;
			angleDeg2 = (angle + 45) * PI / 180;

			double xRF = temp2 * cos(angleDeg2) - (x / 2);
			double yRF = temp2 * sin(angleDeg2) + offsetY - (y / 2);
			double zRF = right.front.pos.z;
			double xRR = temp2 * cos(angleDeg1) - (x / 2);
			double yRR = -temp2 * sin(angleDeg1) + offsetY - (y / 2);
			double zRR = right.rear.pos.z;
			double xLM = temp2 * cos(angleDeg) + (x / 2);
			double yLM = temp2 * sin(angleDeg) + offsetY - (y / 2);
			double zLM = left.mid.pos.z;

			setServoIK(&left.front, xLF, yLF, zLF + zUp, time);
			setServoIK(&left.rear, xLR, yLR, zLR + zUp, time);
			setServoIK(&right.mid, xRM, yRM, zRM + zUp, time);
			servoGroupSend();
			delayMs(time / 15);
			normalSwap0(time);
			servoGroupSend();
			delayMs(time);
			setServoIK(&right.front, xRF, yRF, zRF, time);
			setServoIK(&right.rear, xRR, yRR, zRR, time);
			setServoIK(&left.mid, xLM, yLM, zLM, time);
			servoGroupSend();
			delayMs(time / 15);
			moveServoIK(&left.front, 0, 0, -zUp, time);
			moveServoIK(&left.rear, 0, 0, -zUp, time);
			moveServoIK(&right.mid, 0, 0, -zUp, time);
			servoGroupSend();
			delayMs(delayTime);
		}
		else if(swap == 1) {
			swap = 0;

			double xRF = temp2 * cos(angleDeg2) + (x / 2);
			double yRF = temp2 * sin(angleDeg2) + offsetY + (y / 2);
			double zRF = right.front.pos.z;
			double xRR = temp2 * cos(angleDeg1) + (x / 2);
			double yRR = -temp2 * sin(angleDeg1) + offsetY + (y / 2);
			double zRR = right.rear.pos.z;
			double xLM = temp2 * cos(angleDeg) - (x / 2);
			double yLM = temp2 * sin(angleDeg) + offsetY + (y / 2);
			double zLM = left.mid.pos.z;

			angleDeg = -angle * PI / 180;
			angleDeg1 = (-angle + 45) * PI / 180;
			angleDeg2 = (angle + 45) * PI / 180;

			double xLF = temp2 * cos(angleDeg1) + (x / 2);
			double yLF = temp2 * sin(angleDeg1) + offsetY - (y / 2);
			double zLF = left.front.pos.z;
			double xLR = temp2 * cos(angleDeg2) + (x / 2);
			double yLR = -temp2 * sin(angleDeg2) + offsetY - (y / 2);
			double zLR = left.rear.pos.z;
			double xRM = temp2 * cos(angleDeg) - (x / 2);
			double yRM = -temp2 * sin(angleDeg) + offsetY - (y / 2);
			double zRM = right.mid.pos.z;

			setServoIK(&right.front, xRF, yRF, zRF + zUp, time);
			setServoIK(&right.rear, xRR, yRR, zRR + zUp, time);
			setServoIK(&left.mid, xLM, yLM, zLM + zUp, time);
			servoGroupSend();
			delayMs(time / 15);
			normalSwap1(time);
			servoGroupSend();
			delayMs(time);
			setServoIK(&left.front, xLF, yLF, zLF, time);
			setServoIK(&left.rear, xLR, yLR, zLR, time);
			setServoIK(&right.mid, xRM, yRM, zRM, time);
			servoGroupSend();
			delayMs(time / 15);
			moveServoIK(&right.front, 0, 0, -zUp, time);
			moveServoIK(&right.rear, 0, 0, -zUp, time);
			moveServoIK(&left.mid, 0, 0, -zUp, time);
			servoGroupSend();
			delayMs(delayTime);
		}
	}
}

void moveRobotTPA(double x, double y, double angle, uint32_t time) {
	double temp = sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2);
	double temp2 = COXA_LENGTH + FEMUR_LENGTH;

	if(angle > 35) angle = 35;

	uint16_t delayTime;
	if(wf) {
		 delayTime = (time - 50);
	}
	else {
		delayTime = time;
	}
	if(x != 0 || y != 0 || angle != 0) {
		normalFlag = 0;
		angle = angle / 2;
		double LFPos;
		double angleDeg = angle * PI / 180;
		double angleDeg1 = (angle + 45) * PI / 180;
		double angleDeg2 = (-angle + 45) * PI / 180;

		if(swap == 0) {
			swap = 1;

			double xLF = temp2 * cos(angleDeg1) - (x / 2);
			double yLF = temp2 * sin(angleDeg1) + offsetY + (y / 2);
			double zLF = left.front.pos.z;
			double xLR = temp2 * cos(angleDeg2) - (x / 2);
			double yLR = -temp2 * sin(angleDeg2) + offsetY + (y / 2);
			double zLR = left.rear.pos.z;
			double xRM = temp2 * cos(angleDeg) + (x / 2);
			double yRM = -temp2 * sin(angleDeg) + offsetY + (y / 2);
			double zRM = right.mid.pos.z;

			angleDeg = -angle * PI / 180;
			angleDeg1 = (-angle + 45) * PI / 180;
			angleDeg2 = (angle + 45) * PI / 180;

			double xRF = temp2 * cos(angleDeg2) - (x / 2);
			double yRF = temp2 * sin(angleDeg2) + offsetY - (y / 2);
			double zRF = right.front.pos.z;
			double xRR = temp2 * cos(angleDeg1) - (x / 2);
			double yRR = -temp2 * sin(angleDeg1) + offsetY - (y / 2);
			double zRR = right.rear.pos.z;
			double xLM = temp2 * cos(angleDeg) + (x / 2);
			double yLM = temp2 * sin(angleDeg) + offsetY - (y / 2);
			double zLM = left.mid.pos.z;

			setServoIK(&left.front, xLF, yLF, zLF + zUp, time);
			setServoIK(&left.rear, xLR, yLR, zLR + zUp, time);
			setServoIK(&right.mid, xRM, yRM, zRM + zUp, time);
			servoGroupSend();
			tpaCheckDelayMs(time / 15);
			normalSwap0(time);
			servoGroupSend();
			tpaCheckDelayMs(time);
			setServoIK(&right.front, xRF, yRF, zRF, time);
			setServoIK(&right.rear, xRR, yRR, zRR, time);
			setServoIK(&left.mid, xLM, yLM, zLM, time);
			servoGroupSend();
			tpaCheckDelayMs(time / 15);
			moveServoIK(&left.front, 0, 0, -zUp, time);
			moveServoIK(&left.rear, 0, 0, -zUp, time);
			moveServoIK(&right.mid, 0, 0, -zUp, time);
			servoGroupSend();
			tpaCheckDelayMs(delayTime);
		}
		else if(swap == 1) {
			swap = 0;

			double xRF = temp2 * cos(angleDeg2) + (x / 2);
			double yRF = temp2 * sin(angleDeg2) + offsetY + (y / 2);
			double zRF = right.front.pos.z;
			double xRR = temp2 * cos(angleDeg1) + (x / 2);
			double yRR = -temp2 * sin(angleDeg1) + offsetY + (y / 2);
			double zRR = right.rear.pos.z;
			double xLM = temp2 * cos(angleDeg) - (x / 2);
			double yLM = temp2 * sin(angleDeg) + offsetY + (y / 2);
			double zLM = left.mid.pos.z;

			angleDeg = -angle * PI / 180;
			angleDeg1 = (-angle + 45) * PI / 180;
			angleDeg2 = (angle + 45) * PI / 180;

			double xLF = temp2 * cos(angleDeg1) + (x / 2);
			double yLF = temp2 * sin(angleDeg1) + offsetY - (y / 2);
			double zLF = left.front.pos.z;
			double xLR = temp2 * cos(angleDeg2) + (x / 2);
			double yLR = -temp2 * sin(angleDeg2) + offsetY - (y / 2);
			double zLR = left.rear.pos.z;
			double xRM = temp2 * cos(angleDeg) - (x / 2);
			double yRM = -temp2 * sin(angleDeg) + offsetY - (y / 2);
			double zRM = right.mid.pos.z;

			setServoIK(&right.front, xRF, yRF, zRF + zUp, time);
			setServoIK(&right.rear, xRR, yRR, zRR + zUp, time);
			setServoIK(&left.mid, xLM, yLM, zLM + zUp, time);
			servoGroupSend();
			tpaCheckDelayMs(time / 15);
			normalSwap1(time);
			servoGroupSend();
			tpaCheckDelayMs(time);
			setServoIK(&left.front, xLF, yLF, zLF, time);
			setServoIK(&left.rear, xLR, yLR, zLR, time);
			setServoIK(&right.mid, xRM, yRM, zRM, time);
			servoGroupSend();
			tpaCheckDelayMs(time / 15);
			moveServoIK(&right.front, 0, 0, -zUp, time);
			moveServoIK(&right.rear, 0, 0, -zUp, time);
			moveServoIK(&left.mid, 0, 0, -zUp, time);
			servoGroupSend();
			tpaCheckDelayMs(delayTime);
		}
	}
}

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
uint16_t coxaPos, femurPos, tibiaPos;

double posCoxa, posFemur, posTibia;
double sudutCoxa, sudutFemur, sudutTibia;

void setServoIK(legPosition *legPos, double x, double y, double z, uint16_t time) {
	legPos -> pos.x = x;
	legPos -> pos.y = y;
	legPos -> pos.z = z;

	double zOffset = legPos -> tibia.length - legPos -> pos.z;
	double L1 = sqrt(pow(legPos -> pos.x, 2) + pow(legPos -> pos.y, 2));
	double L = sqrt(pow(zOffset, 2) + pow((L1 - legPos -> coxa.length), 2));
	coxaAngle = atan(legPos -> pos.y / legPos -> pos.x) * 180 / PI;
	if(legPos -> type == 0) {
		coxaAngle -= 45;
	}
	else if(legPos -> type == 2 ) {
		coxaAngle += 45;
	}
	double femur1 = atan((L1 - legPos -> coxa.length) / zOffset);
	double femur2 = acos((pow(legPos -> femur.length, 2) + pow(L, 2) - pow(legPos -> tibia.length, 2)) / (2 * legPos -> femur.length * L));
	femurAngle = -((femur1 + femur2) * 180 / PI) + 90;
	tibiaAngle = -(acos((pow(legPos -> tibia.length, 2) + pow(legPos -> femur.length, 2) - pow(L, 2)) / (2 * legPos -> tibia.length * legPos -> femur.length)) * 180 / PI) + 90;

//	if(coxaAngle < legPos -> coxa.minAngle){
//		coxaAngle = legPos -> coxa.minAngle;
//	}
//	else if(coxaAngle > legPos -> coxa.maxAngle){
//		coxaAngle = legPos -> coxa.maxAngle;
//	}
//
//	if(femurAngle < legPos -> femur.minAngle){
//		femurAngle = legPos -> femur.minAngle;
//	}
//	else if(femurAngle > legPos -> femur.maxAngle){
//		femurAngle = legPos -> femur.maxAngle;
//	}
//
//	if(tibiaAngle < legPos -> tibia.minAngle){
//		tibiaAngle = legPos -> tibia.minAngle;
//	}
//	else if(tibiaAngle > legPos -> tibia.maxAngle){
//		tibiaAngle = legPos -> tibia.maxAngle;
//	}

	sudutCoxa = coxaAngle;
	sudutFemur = femurAngle;
	sudutTibia= tibiaAngle;

	//servo kanan => Jika sudut bertambah, bergerak CCW
	//Servo kiri  => Jika sudut bertambah, bergerak CW
	coxaPos = legPos -> coxa.initAngle + (coxaAngle * 600 / 90);
	femurPos = legPos -> femur.initAngle + (femurAngle * 600 / 90);
	tibiaPos = legPos -> tibia.initAngle + (tibiaAngle * 600 / 90);

	posCoxa = coxaPos;
	posFemur = femurPos;
	posTibia = tibiaPos;
	setServo(legPos -> coxa.pin, coxaPos, time, 1);
	setServo(legPos -> femur.pin, femurPos, time, 1);
	setServo(legPos -> tibia.pin, tibiaPos, time, 1);
}

void tesServo(uint16_t time){
	double temp = sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2);
	double temp2 = COXA_LENGTH + FEMUR_LENGTH;
	double normalZ1 = -1.5;
	double angle1 = (45) * PI / 180;
	double angle2 = (45) * PI / 180;
	eins = (temp2 * cos(angle2)) + 0;
	zwei = (temp2 * sin(angle2)) + 0;
	drei = normalZ;
	setServoIK(&left.mid, temp2, 3, normalZ1, time);
	servoGroupSend();
	delayMs(time);
	delayMs(1000);
	setServoIK(&left.mid, temp2, 0, normalZ1, time);
	servoGroupSend();
	delayMs(time);
	delayMs(1000);

/*	udah bisa posisi normal()
	setServoIK(&left.rear, eins, zwei, normalZ1, time);
	setServoIK(&left.rear, temp, temp, normalZ1, time);
*/
}
/*
void setServoIK(legPosition *legPos, double x, double y, double z, uint16_t time) {
	legPos -> pos.x = x;
	legPos -> pos.y = y;
	legPos -> pos.z = z;

	double zOffset = legPos -> tibia.length - legPos -> pos.z;
	double L1 = sqrt(pow(legPos -> pos.x, 2) + pow(legPos -> pos.y, 2));
	double L = sqrt(pow(zOffset, 2) + pow((L1 - legPos -> coxa.length), 2));
	coxaAngle = atan(legPos -> pos.y / legPos -> pos.x) * 180 / PI;
	if(legPos -> type == 0) {
		coxaAngle -= 45;
	}
	else if(legPos -> type == 2 ) {
		coxaAngle += 45;
	}
	double femur1 = atan((L1 - legPos -> coxa.length) / zOffset);
	double femur2 = acos((pow(legPos -> femur.length, 2) + pow(L, 2) - pow(legPos -> tibia.length, 2)) / (2 * legPos -> femur.length * L));
	femurAngle = -((femur1 + femur2) * 180 / PI) + 90;
	tibiaAngle = -(acos((pow(legPos -> tibia.length, 2) + pow(legPos -> femur.length, 2) - pow(L, 2)) / (2 * legPos -> tibia.length * legPos -> femur.length)) * 180 / PI) + 90;
	if(coxaAngle < 0) {
		coxaPos = 1500 + (coxaAngle * 600 / legPos -> coxa.minAngle);
	}
	else {
		coxaPos = 1500 + (coxaAngle * 600 / legPos -> coxa.maxAngle);
	}
	if(femurAngle < 0) {
		femurPos = 1500 + (-femurAngle * 600 / legPos -> femur.minAngle);
	}
	else {
		femurPos = 1500 + (-femurAngle * 600 / legPos -> femur.maxAngle);
	}
	if(tibiaAngle < 0) {
		tibiaPos = 1500 + (-tibiaAngle * 600 / legPos -> tibia.minAngle);
	}
	else {
		tibiaPos = 1500 + (-tibiaAngle * 600 / legPos -> tibia.maxAngle);
	}

	setServo(legPos -> coxa.pin, coxaPos, time, 1);
	setServo(legPos -> femur.pin, femurPos, time, 1);
	setServo(legPos -> tibia.pin, tibiaPos, time, 1);
}
*/
void moveServoIK(legPosition *legPos, double x, double y, double z, uint16_t time) {
	x += legPos -> pos.x;
	y += legPos -> pos.y;
	z += legPos -> pos.z;
	setServoIK(legPos, x, y, z, time);
}
