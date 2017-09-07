/**
 * Library Jalan Dasar SSC + IK
 *
 * @file	"jalan_dasar.h"
 * @author	Rudy Nurhadi
 * @version	09/Mei/2016
 *
 */

#ifndef __JALAN_DASAR_H
#define __JALAN_DASAR_H

typedef struct position {
	double x;
	double y;
	double z;
} position;

typedef struct legPart {
	uint8_t pin;
	double length;
	double minAngle;
	double maxAngle;
	double initAngle;
	uint16_t posNow;
} legPart;

typedef struct legPosition {
	uint8_t type; //0 = front, 1 = mid, 2 = rear
	position pos;
	legPart coxa;
	legPart femur;
	legPart tibia;
} legPosition;

typedef struct leg {
	legPosition front;
	legPosition mid;
	legPosition rear;
} leg;

extern uint8_t wf;
extern leg left, right;

void servoInit();
void normalInit(uint16_t time);
void normal(uint16_t time);
void nungging(double z, uint16_t time);
void miring(double z, uint16_t time);
void moveNormal(uint16_t time);
void moveNormal2(uint16_t time);
void cobaServo();
void moveRobot(double x, double y, double angle, uint16_t time);
void moveRobotTPA(double x, double y, double angle, uint32_t time);

#endif
