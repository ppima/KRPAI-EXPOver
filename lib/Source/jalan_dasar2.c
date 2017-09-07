/**
 * Library Jalan Dasar SSC + IK
 *
 * @file		"jalan_dasar2.c"
 * @author		Rudy Nurhadi
 * @co-author	Nicholas Nainggolan
 * @version		17/03/2017
 *
 */

/* This library implement inverse kinematics algorithm with 3 degree of freedom (dof).
 * You can use this libary for Hexapod robot that have servos as it actuators.
 * This library will support translation (x, y plane horizontally) and Yaw (rotate) movement.
 * All movements are refer to cartesian coordinate system.
 *
 * !!!PLEASE TAKE CARE OF THIS CONVENTION BEFORE MAKE ANY MOVEMENT OR CALL FUNCTION TO THE ROBOT!!!
 * DIRECTIONS AND VALUES:
 * X positive will move right, as the opposed X negative will move left.
 * Y positive will move forward, as the opposed Y negative will move backward.
 * Angle positive will move CW (Clock Wise), as the opposed Angle negative will move CCW (Counter Clock Wise).
 * Z positive will move downward, as the opposed Z negative will move upward.
 * Time increase will move slower, as the opposed Time decrease will move faster.
 * Minimum value of Time is 60, means the robot move the legs each 60 ms.
 *
 */

//These are the include libraries that needed to support this library.
//Math.h is used to provide trigonometry and other mathematics function.
#include "stdint.h"
#include "uart.h"
#include <math.h>
#include "srf_tpa.h"
#include "stdio.h"
#include "jalan_dasar.h"

#define PI 3.1415926535897

#define SSC_COM COM3

//This directive define the coxa, femur, and tibia length.
//Please make sure your coxa, femur, and tibia actual size on the robot match with these values.
//Change these values based on your robot actual size
#define COXA_LENGTH (double)4
#define FEMUR_LENGTH (double)6
#define TIBIA_LENGTH (double)11

double test = 0, eins, zwei, drei, vier;

//--------------- Prototype Private Function ----------------
void setServo(uint8_t servoPin, uint16_t position, uint16_t time, uint8_t servoGroup);
void servoGroupSend();
void setServoIK(legPosition *legPos, double x, double y, double z, uint16_t time);
void moveServoIK(legPosition *legPos, double x, double y, double z, uint16_t time);

//--------------- Private Variable ----------------
//This variable diferrentiate left servos and right servos.
//All left servos (coxa, femur, tibia in left leg) are configured with CW
//All right servos (coxa, femur, tibia in right leg) are configured with CCW
leg left, right;

//This library grouped the legs into two group and showed by swap's value.
//If swap = 1, then the group contain left.front, right.mid, and left.rear
//If swap = 0, then the group contain right.front, left.mid, and right.rear
static uint8_t swap = 1;
static uint8_t normalFlag = 0;

//Variable wf show if the robot is doing wall following or not while call moveRobot() function
//If wf = 1, than it show the robot that it is doing wall following.
//Variable wf is needed in order to perform ultrasonic ranging while the legs are moving.
uint8_t wf = 0;

const double offsetY = -2;
const double normalZ = 2.5;
const double normalXMid = COXA_LENGTH + FEMUR_LENGTH;
const double normalXY = (sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2));
const double zUp = 1.5;

//--------------- Public Function ----------------
//This function will give initial value for every leg and it's part.
//e.g. x, y, z position, coxa, femur, tibia length, servos' pin, and angle for perpendicular.
//To calibrate each servo, adjust the initAngle value. Just find the proper value.

void servoInit() {
	double temp = sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2);

	left.front.type = 0;
	left.front.pos.x = temp;
	left.front.pos.y = temp;
	left.front.pos.z = -1;
	left.front.coxa.pin = 0;
	left.front.coxa.length = COXA_LENGTH;
	left.front.coxa.minAngle = 45;
	left.front.coxa.maxAngle = 45;
	left.front.coxa.initAngle = 1419;
	left.front.femur.pin = 1;
	left.front.femur.length = FEMUR_LENGTH;
	left.front.femur.minAngle = 90;
	left.front.femur.maxAngle = 90;
	left.front.femur.initAngle = 1469;
	left.front.tibia.pin = 3;
	left.front.tibia.length = TIBIA_LENGTH;
	left.front.tibia.minAngle = 90;
	left.front.tibia.maxAngle = 90;
	left.front.tibia.initAngle = 1469;

	left.mid.type = 1;
	left.mid.pos.x = 10;
	left.mid.pos.y = -0.2;
	left.mid.pos.z = -0.5;
	left.mid.coxa.pin = 4;
	left.mid.coxa.length = COXA_LENGTH;
	left.mid.coxa.minAngle = 40;
	left.mid.coxa.maxAngle = 20;
	left.mid.coxa.initAngle = 1519;
	left.mid.femur.pin = 5;
	left.mid.femur.length = FEMUR_LENGTH;
	left.mid.femur.minAngle = 90;
	left.mid.femur.maxAngle = 90;
	left.mid.femur.initAngle = 1470;
	left.mid.tibia.pin = 6;
	left.mid.tibia.length = TIBIA_LENGTH;
	left.mid.tibia.minAngle = 90;
	left.mid.tibia.maxAngle = 90;
	left.mid.tibia.initAngle = 1469;

	left.rear.type = 2;
	left.rear.pos.x = temp;
	left.rear.pos.y = -temp;
	left.rear.pos.z = -0.2;
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
	right.front.femur.initAngle = 1500;
	right.front.tibia.pin = 18;
	right.front.tibia.length = TIBIA_LENGTH;
	right.front.tibia.minAngle = 90;
	right.front.tibia.maxAngle = 90;
	right.front.tibia.initAngle = 1488;

	right.mid.type = 1;
	right.mid.pos.x = 10;
	right.mid.pos.y = 0;
	right.mid.pos.z = -0.2;
	right.mid.coxa.pin = 20;
	right.mid.coxa.length = COXA_LENGTH;
	right.mid.coxa.minAngle = 40;
	right.mid.coxa.maxAngle = 20;
	right.mid.coxa.initAngle = 1317; //1500
	right.mid.femur.pin = 21;
	right.mid.femur.length = FEMUR_LENGTH;
	right.mid.femur.minAngle = 90;//130;
	right.mid.femur.maxAngle = 90;
	right.mid.femur.initAngle = 1481; //1483
	right.mid.tibia.pin = 22;
	right.mid.tibia.length = TIBIA_LENGTH;
	right.mid.tibia.minAngle = 90;
	right.mid.tibia.maxAngle = 90;
	right.mid.tibia.initAngle = 1472; //1460

	right.rear.type = 2;
	right.rear.pos.x = temp;
	right.rear.pos.y = -temp;
	right.rear.pos.z = 0;
	right.rear.coxa.pin = 24;
	right.rear.coxa.length = COXA_LENGTH;
	right.rear.coxa.minAngle = 45;
	right.rear.coxa.maxAngle = 45;
	right.rear.coxa.initAngle = 1544;  //1544
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

//In reva, normalZ are assigned as positive value
//In reinhardt, nomalZ are assigned as negative value
//Z-positive will move the robot upward, means all servos move downward
void normalInit(uint16_t time) {
	double temp = sqrt(((COXA_LENGTH + FEMUR_LENGTH) * (COXA_LENGTH + FEMUR_LENGTH)) / 2);
	double temp2 = COXA_LENGTH + FEMUR_LENGTH;
	double normalZ = 5;

	setServoIK(&left.front, temp, temp, normalZ, time);
	setServoIK(&left.mid, temp2, 0, normalZ, time);
	setServoIK(&left.rear, temp, -temp, normalZ, time);
	setServoIK(&right.front, temp, temp, normalZ, time);
	setServoIK(&right.mid, temp2, 0, normalZ, time);
	setServoIK(&right.rear, temp, -temp, normalZ, time);
	servoGroupSend();
	delayMs(500);
	normal(time);
	delayMs(time);
}

//This function is normal position for right.front, left.mid, right.rear
void normalSwap0(uint16_t time) {
	setServoIK(&right.front, normalXY, normalXY + offsetY, normalZ, time);
	setServoIK(&right.rear, normalXY, -normalXY + offsetY, normalZ , time);
	setServoIK(&left.mid, normalXMid, offsetY, normalZ - 0.5, time);
}

//This function is normal position for left.front, right.mid, left.rear
void normalSwap1(uint16_t time) {
	setServoIK(&left.front, normalXY, normalXY + offsetY, normalZ - 1, time);
	setServoIK(&left.rear, normalXY, -normalXY + offsetY, normalZ - 0.2, time);
	setServoIK(&right.mid, normalXMid, offsetY, normalZ - 0.2, time);
}

//This function is to move the robot for it's initial position.
//Call this function before you make any movement with the robot.
//Call it after all initialization is very recommended, so the robot can move properly.
//If you forget to call this function and move the robot, please shut down the robot
void normal(uint16_t time) {
	if(!normalFlag) {
		normalFlag = 1;
		normalSwap0(time);
		normalSwap1(time);
		servoGroupSend();
	}
}

//This function will move all servos into it's initial Angle.
//Use this function to do finishing touch for servo calibration.
void normalInitAngle(uint16_t time){
	setServo(left.front.coxa.pin, left.front.coxa.initAngle, time, 0);
	setServo(left.front.femur.pin, left.front.femur.initAngle, time, 0);
	setServo(left.front.tibia.pin, left.front.tibia.initAngle, time, 0);

	setServo(left.mid.coxa.pin, left.mid.coxa.initAngle, time, 0);
	setServo(left.mid.femur.pin, left.mid.femur.initAngle, time, 0);
	setServo(left.mid.tibia.pin, left.mid.tibia.initAngle, time, 0);

	setServo(left.rear.coxa.pin, left.rear.coxa.initAngle, time, 0);
	setServo(left.rear.femur.pin, left.rear.femur.initAngle, time, 0);
	setServo(left.rear.tibia.pin, left.rear.tibia.initAngle, time, 0);

	setServo(right.front.coxa.pin, right.front.coxa.initAngle, time, 0);
	setServo(right.front.femur.pin, right.front.femur.initAngle, time, 0);
	setServo(right.front.tibia.pin, right.front.tibia.initAngle, time, 0);

	setServo(right.mid.coxa.pin, right.mid.coxa.initAngle, time, 0);
	setServo(right.mid.femur.pin, right.mid.femur.initAngle, time, 0);
	setServo(right.mid.tibia.pin, right.mid.tibia.initAngle, time, 0);

	setServo(right.rear.coxa.pin, right.rear.coxa.initAngle, time, 0);
	setServo(right.rear.femur.pin, right.rear.femur.initAngle, time, 0);
	setServo(right.rear.tibia.pin, right.rear.tibia.initAngle, time, 0);
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

//This function is to move the legs to normal position.
//If you want all the servos move to normal position,
//use this function after move the robot.
//Z positive move the leg downward, z negative move the leg upward
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

//This function will move the robot according to it's argument/paramter values.
//There are 4 parameters, that is x, y, angle, and time.
//x, y, and angle parameters indicate which movement (translation or rotate or both) the robot does.
//Time indicate the movement speed of the robot.
//PLEASE TAKE CARE OF THE CONVENTION that we used about direction and values in the beginning of this library.
//JUST MAKE SURE YOU KNOW WHAT YOU DO WITH THIS FUNCTION.
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


uint16_t coxaPos, femurPos, tibiaPos;
double coxaAngle, femurAngle, tibiaAngle;

void setServoIK(legPosition *legPos, double x, double y, double z, uint16_t time) {
	legPos -> pos.x = x;
	legPos -> pos.y = y;
	legPos -> pos.z = z;

	double zOffset = legPos -> tibia.length - legPos -> pos.z;
	double R1 = sqrt(pow(legPos -> pos.x, 2) + pow(legPos -> pos.y, 2));
	double R = sqrt(pow(zOffset, 2) + pow((R1 - legPos -> coxa.length), 2));
	coxaAngle = atan(legPos -> pos.y / legPos -> pos.x) * 180 / PI;

	if(legPos -> type == 0) {
		coxaAngle -= 45;
	}
	else if(legPos -> type == 2 ) {
		coxaAngle += 45;
	}
	double femur1 = atan((R1 - legPos -> coxa.length) / zOffset);
	double femur2 = acos((pow(legPos -> femur.length, 2) + pow(R, 2) - pow(legPos -> tibia.length, 2)) / (2 * legPos -> femur.length * R));
	femurAngle = -((femur1 + femur2) * 180 / PI) + 90;
	tibiaAngle = -(acos((pow(legPos -> tibia.length, 2) + pow(legPos -> femur.length, 2) - pow(R, 2)) / (2 * legPos -> tibia.length * legPos -> femur.length)) * 180 / PI) + 90;


	if(coxaAngle < -(legPos -> coxa.minAngle)){
		coxaAngle = -(legPos -> coxa.minAngle);
	}
	else if(coxaAngle > legPos -> coxa.maxAngle){
		coxaAngle = legPos -> coxa.maxAngle;
	}

	if(femurAngle < -(legPos -> femur.minAngle)){
		femurAngle = -(legPos -> femur.minAngle);
	}
	else if(femurAngle > legPos -> femur.maxAngle){
		femurAngle = legPos -> femur.maxAngle;
	}

	if(tibiaAngle < -(legPos -> tibia.minAngle)){
		tibiaAngle = -(legPos -> tibia.minAngle);
	}
	else if(tibiaAngle > legPos -> tibia.maxAngle){
		tibiaAngle = legPos -> tibia.maxAngle;
	}

	legPos -> coxa.posNow = legPos -> coxa.initAngle + (coxaAngle * 600 / 90);
	legPos -> femur.posNow = legPos -> femur.initAngle + (-femurAngle * 600 / 90);
	legPos -> tibia.posNow = legPos -> tibia.initAngle + (-tibiaAngle * 600 / 90);

	setServo(legPos -> coxa.pin, legPos -> coxa.posNow, time, 1);
	setServo(legPos -> femur.pin, legPos -> femur.posNow, time, 1);
	setServo(legPos -> tibia.pin, legPos -> tibia.posNow, time, 1);
}

//if z increase, the leg moves down

void moveServoIK(legPosition *legPos, double x, double y, double z, uint16_t time) {
	x += legPos -> pos.x;
	y += legPos -> pos.y;
	z += legPos -> pos.z;
	setServoIK(legPos, x, y, z, time);
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

void moveServoRR(double x, double y, double z, uint8_t time)
{
	setServoIK(&right.rear, x, y, z, time);
}

void tesServo(uint16_t time){
	setServo(left.front.coxa.pin, left.front.coxa.initAngle, time, 0);
	setServo(left.front.femur.pin, left.front.femur.initAngle, time, 0);
	setServo(left.front.tibia.pin, left.front.tibia.initAngle, time, 0);
}
