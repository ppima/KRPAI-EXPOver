#include "srf_tpa.h"
#include "navigasi.h"

#define ROOM1_LENGTH 118
#define ROOM2_LENGTH 103
#define ROOM3_LENGTH 87
#define ROOM4_LENGTH 70

#define ROOM1_WIDTH 91
#define ROOM2_WIDTH 72
#define ROOM3_WIDTH 72
#define ROOM4_WIDTH 51

#define ROOM1_WIDE 10400
#define ROOM2_WIDE 7100
#define ROOM3_WIDE 5900
#define ROOM4_WIDE 3200

roomStatus room1 = UNCHECKED;
roomStatus room2 = UNCHECKED;
roomStatus room3 = UNCHECKED;
roomStatus room4 = UNCHECKED;

dollPosition dollPos = UNKNOWN;
roomType robotPos = HALL;
static uint8_t fireStatus = 1;
room4Type room4Model;
room1Type room1Model;
room1DoorType room1Door;
roomType origin, current;

uint8_t searchFireCounter = 0;
uint8_t moveFireCounter = 0;
double searchAngle = 0;


//=====================================================================
//--------------------Private Functions Prototype----------------------
//=====================================================================

void roomStop(uint16_t time);
void room4Start(uint16_t time);
void navigationRoom4(dollPosition dollPos, uint16_t time);
void room1toRoom2(uint16_t time);
void room1toRoom3(uint16_t time);
void room1toRoom4Unchecked(uint16_t time);
void room1toRoom4Checked(uint16_t time);
void room2toRoom1(uint16_t time);
void room2toRoom3(uint16_t time);
void room2toRoom4(uint16_t time);
void room3toRoom1(uint16_t time);
void room3toRoom2(uint16_t time);
void room3toRoom4(uint16_t time);
void room4toRoom1(uint16_t time);
void room4toRoom2(uint16_t time);
void room4toRoom3(uint16_t time);

//=====================================================================
//------------------End of Private Functions Prototype-----------------
//=====================================================================

uint8_t robotNavigation(roomType source, roomType dest, uint16_t time) {
	const uint8_t ping[3] = {0xE4, 0xEC, 0xE0};
	uint8_t left, right, front, i;
	lcd_clear();
	if(source == HALL) {
		for(i = 0; i < 3; i++) {
			sonarRange(ping[i]);
			delayMs(10);
		}
		right = sonarGet(ping[0]);
		left = sonarGet(ping[1]);
		front = sonarGet(ping[2]);
		if(dest == ROOM1) {

		}
		else if(dest == ROOM2) {

		}
		else if(dest == ROOM3) {
			if(front < 27){
				if(left < right){
					putarBalik(time);
					rataKanan(time);
				}
				else{
					putarKiri90(time);
					rataKanan(time);
				}
			}
			else{
				if(left < right) {
					putarKanan90(time);
					moveNormal(time);
					rataKanan(time);
				}
				else{
					rataKanan(time);
				}
			}
			kanan(time, BLACK);
			kanan(time, LINE);
			rataKiri(time);
			normal(300);
			if(fireStatus){
				if(checkingFire(time, 3) == 1) {
					room3 = ON_FIRE;
					do {
						killFire();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
	//				kanan(time, LINE);
					room3 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return 0;
			}
		}
		else if(dest == ROOM4) {

		}
	}
	else if(source == ROOM1) {
		if(dest == HALL) {
			putarBalik(time);
			if(room1Model == TYPE1) {
				kiri(time, LINE);
				delayMs(50);
				wf = 1;
				moveRobot(0, 2, 0, time);
				kanan(time, BLACK);
			}
			else {
				kanan(time, LINE);
				delayMs(50);
				wf = 1;
				moveRobot(0, 2, 0, time);
			}
			kanan(time, GRAY);
			kiri(time, BLACK);
			delayMs(50);
			moveRobot(0, -4, 0, time);
			moveRobot(0, -4, 0, time);
			moveRobot(0, -4, 0, time);
			moveRobot(0, -4, 0, time);
			putarBalik(time);
			kiri(time, WHITE);
		}
		else if(dest == ROOM2) {
			room1toRoom2(time);
		}
		else if(dest == ROOM3) {
			room1toRoom3(time);
		}
		else if(dest == ROOM4) {
			if(origin == ROOM4){
				room1toRoom4Checked(time);
			}
			else{
				room1toRoom4Unchecked(time);
			}
		}
	}
	else if(source == ROOM2) {
		if(dest == HALL) {
//			for(i = 0; i < 2; i++) {
//				sonarRange(ping[i]);
//				delayMs(10);
//			}
//			right = sonarGet(ping[0]);
//			left = sonarGet(ping[1]);
//
//			putarBalik(time);
//			if(right > left) {
//				kiri(time, LINE);
//			}
//			else {
//				kanan(time, LINE);
//			}
			putarBalik(time);
//			kanan(time, LINE);
			kanan(time, BLACK);
			kiri(time, GRAY);
			kiri(time, BLACK);
			delayMs(50);
			moveRobot(0, -4, 0, time);
			moveRobot(0, -4, 0, time);
			moveRobot(0, -4, 0, time);
			moveRobot(0, -4, 0, time);
			putarBalik(time);
			kiri(time, WHITE);
		}
		else if(dest == ROOM1) {
			room2toRoom1(time);
		}
		else if(dest == ROOM3) {
			room2toRoom3(time);
		}
		else if(dest == ROOM4) {
			room2toRoom4(time);
		}
	}
	else if(source == ROOM3) {
		if(dest == HALL) {
			putarBalik(time);
			kanan(time, LINE);
			delayMs(50);
			moveRobot(0, 5, 0, time);
			wf = 1;
			moveRobot(0, 5, 0, time);
			kanan(time, GRAY);
			kiri(time, WHITE);
		}
		else if(dest == ROOM1) {
			room3toRoom1(time);
		}
		else if(dest == ROOM2) {
			room3toRoom2(time);
		}
		else if(dest == ROOM4) {
			room3toRoom4(time);
		}
	}
	else if(source == ROOM4) {
		if(dest == HALL) {
			if(room4Model == MODEL1) {
				if(dollPos == POS1) {
					putarBalik(time);
					rataKanan(time);
					kanan(time, LINE);
					kanan(time, BLACK);
					kanan(time, GRAY);
					kiri(time, BLACK);
					kanan(time, GRAY);
					kanan(time, BLACK);
					kanan(time, GRAY);
					kanan(time, HOME);
				}
				else {
//					const uint8_t ping[3] = {0xE4, 0xEC};
//					uint8_t left, right, i;
//					putarBalik(time);
//					for(i = 0; i < 2; i++) {
//						sonarRange(ping[i]);
//						delayMs(10);
//					}
//					right = sonarGet(ping[0]);
//					left = sonarGet(ping[1]);
//
//					if(left > right) {
//	//					rataKanan(time);
//	//					geserKanan(time);
//						kanan(time, LINE);
//					}
//					else {
//	//					rataKiri(time);
//	//					geserKiri(time);
//						kiri(time, LINE);
//					}
					putarBalik(time);
					rataKanan(time);
					kanan(time, LINE);
					kiri(time, BLACK);
					kiri(time, GRAY);
					kanan(time, WHITE);
				}
			}
			else {
				const uint8_t ping[3] = {0xE4, 0xEC};
				uint8_t left, right, i;
				putarBalik(time);
				for(i = 0; i < 2; i++) {
					sonarRange(ping[i]);
					delayMs(10);
				}
				right = sonarGet(ping[0]);
				left = sonarGet(ping[1]);

				if(left > right) {
//					rataKanan(time);
//					geserKanan(time);
					rataKanan(time);
					kanan(time, LINE);
				}
				else {
//					rataKiri(time);
//					geserKiri(time);
					rataKiri(time);
					kiri(time, LINE);
				}
				kanan(time, BLACK);
				delayMs(50);
				wf = 1;
				moveRobot(0, 3, 0, time);
				kanan(time, GRAY);
				kanan(time, HOME);
			}
		}
		else if(dest == ROOM1) {
			room4toRoom1(time);
		}
		else if(dest == ROOM2) {
			room4toRoom2(time);
		}
		else if(dest == ROOM3) {
			room4toRoom3(time);
		}
	}

	return fireStatus;
}

roomType roomStart(uint16_t time) {
	uint16_t front, left, right, back;
	uint8_t i;
	const uint8_t ping[6] = {0xE0, 0xE4, 0xEC, 0xE8, 0xE6, 0xEA};
	roomType robotPost;
	uint8_t room1DoorACorrect = 0;
	uint8_t room1DoorBCorrect = 0;
	uint8_t room2Correct = 0;
	uint8_t room3Correct = 0;
	uint8_t room4Correct = 0;

	tpaSetServo(27);
	sonarRange(ping[0]);
	front = sonarGet(ping[0]);
	sonarRange(ping[1]);
	right = sonarGet(ping[1]);
	sonarRange(ping[2]);
	left = sonarGet(ping[2]);
	sonarRange(ping[3]);
	back = sonarGet(ping[3]);

	if(right <= 30 && left <= 30) {
		if(front < back) {
			rataKiri(time);
		}
		else {
			rataKanan(time);
		}
	}
	else if(right > left) {
		rataKiri(time);
	}
	else {
		rataKanan(time);
	}

	sonarRange(ping[0]);
	front = sonarGet(ping[0]);
	sonarRange(ping[1]);
	right = sonarGet(ping[1]);
	sonarRange(ping[2]);
	left = sonarGet(ping[2]);
	sonarRange(ping[3]);
	back = sonarGet(ping[3]);


	if(front > 30) {
		if(right > left){
			kiriRoom(time, NOTLINE);
			kiriRoom(time, LINE);
			delayMs(50);
			moveRobot(0, -3, 0, time);
			wf = 1;
			moveRobot(0, -3, 0, time);
			kiriMundur(time, NOTLINE);
		}
		else{
			kananRoom(time, NOTLINE);
			kananRoom(time, LINE);
			delayMs(50);
			moveRobot(0, -3, 0, time);
			wf = 1;
			moveRobot(0, -3, 0, time);
			kananMundur(time, NOTLINE);
		}
	}
	else {
		if(right <= 30 && left <= 30 && back > front) {
			putarBalik(time);
			geserKananRoom(time);
			kananRoom(time, NOTLINE);
			kananRoom(time, LINE);
			delayMs(50);
			moveRobot(0, -3, 0, time);
			wf = 1;
			moveRobot(0, -3, 0, time);
			kananMundur(time, NOTLINE);
		}
		else if(right < left){
			putarKiri90(time);
			kananRoom(time, NOTLINE);
			kananRoom(time, LINE);
			delayMs(50);
			moveRobot(0, -3, 0, time);
			wf = 1;
			moveRobot(0, -3, 0, time);
			kananMundur(time, NOTLINE);
		}
		else{
			putarKanan90(time);
			kiriRoom(time, NOTLINE);
			kiriRoom(time, LINE);
			delayMs(50);
			moveRobot(0, -3, 0, time);
			wf = 1;
			moveRobot(0, -3, 0, time);
			kiriMundur(time, NOTLINE);
		}
	}
	delayMs(50);
	moveRobot(0, -3, 0, time);
	wf = 1;
	moveNormal(time);

	sonarRange(ping[1]);
	right = sonarGet(ping[1]);
	sonarRange(ping[2]);
	left = sonarGet(ping[2]);
	sonarRange(ping[4]);
	right += sonarGet(ping[4]);
	sonarRange(ping[5]);
	left += sonarGet(ping[5]);

	if(right > left){
		rataKiri(time);
		geserKiri(time);
	}
	else{
		rataKanan(time);
		geserKanan(time);
	}
	for(i = 0; i < 6; i++){
		robotPost = checkRoomIn();
		if(robotPost == ROOM1) {
			if(room1Door == DOORA) {
				room1DoorACorrect++;
			}
			else if(room1Door == DOORB){
				room1DoorBCorrect++;
			}
		}
		else if(robotPost == ROOM2) {
			room2Correct++;
		}
		else if(robotPost == ROOM3) {
			room3Correct++;
		}
		else if(robotPost == ROOM4) {
			room4Correct++;
		}
		if(room1DoorACorrect > 4) {
			robotPos = ROOM1;
			room1Door = DOORA;
			room1 = CHECKED;
			break;
		}
		else if(room1DoorBCorrect > 4) {
			robotPos = ROOM1;
			room1Door = DOORB;
			room1 = CHECKED;
			break;
		}
		else if(room2Correct > 4) {
			robotPos = ROOM2;
			room2 = CHECKED;
			break;
		}
		else if(room3Correct > 4) {
			robotPos = ROOM3;
			room3 = CHECKED;
			break;
		}
		else if(room4Correct > 4) {
			robotPos = ROOM4;
			room4 = CHECKED;
			break;
		}
	}
	return robotPos;
}

roomType checkRoomIn(){
	uint16_t front, left, right, back, length, width, i;
	const uint8_t ping[4] = {0xE0, 0xE6, 0xE8, 0xEA};
	char temp1[16], temp2[16], temp3[16];

	for (i = 0; i < 4; i++) {
		sonarSetGain(ping[i], 1);
		sonarSetRange(ping[i], 1500);
	}

	for (i = 0; i < 4; i++) {
		sonarRange(ping[i]);
		delayMs(13);
	}

	front = sonarGet(ping[0]);
	right = sonarGet(ping[1]);
	back = sonarGet(ping[2]);
	left = sonarGet(ping[3]);

	length = front + back + 20;
	width = left + right + 15;
	lcd_clear();
	sprintf(temp1, "l=%d w=%d", length, width);
	sprintf(temp2, "l=%2d r=%2d", left, right);
	sprintf(temp3, "F%d L%d R%d B%d", front, left, right, back);
	lcd_putsf(0, 0, temp3);

	if(checkColorSensor() == GRAY){
		robotPos = ROOM2;
		lcd_putsf(0, 1, "ROOM2");
	}
	else{
		if(width > ROOM1_WIDTH - 8){
			if(back < ROOM1_WIDTH - 5){
				robotPos = ROOM1;
				room1Door = DOORB;
				lcd_putsf(0, 1, "ROOM1B");
			}
			else{
				robotPos = ROOM1;
				room1Door = DOORA;
				lcd_putsf(0, 1, "ROOM1A");
			}
		}
		else if((width > ROOM3_WIDTH - 15) && (width < ROOM1_WIDTH - 10)){
			if(back > ROOM4_WIDTH + 5){
				robotPos = ROOM3;
				lcd_putsf(0, 1, "ROOM3");
			}
			else{
				robotPos = ROOM4;
				lcd_putsf(0, 1, "ROOM4");
			}
		}
//		else if((width > ROOM4_LENGTH) - 10 && (width < ROOM3_LENGTH - 10)){
//			if(back < ROOM4_WIDTH + 10){
//				robotPos = ROOM4;
//				lcd_putsf(0, 1, "ROOM4");
//			}
//		}
		else{
			lcd_putsf(0, 1, "NOT ROOM");
		}
	}

	for (i = 0; i < 4; i++) {
		sonarSetGain(ping[i], 8);
		sonarSetRange(ping[i], 800);
	}

	return robotPos;
}

roomType checkRoomOut(){
	uint16_t front, left, right, back, length, width, i;
	const uint8_t ping[4] = {0xE0, 0xE4, 0xE8, 0xEC};
	char temp1[16], temp2[16], temp3[16];

	lcd_clear();
	front = 0;
	left = 0;
	right = 0;
	back = 0;

	for (i = 0; i < 4; i++) {
		sonarSetGain(ping[i], 1);
		sonarSetRange(ping[i], 1500);
	}
	for (i = 0; i < 4; i++) {
		sonarRange(ping[i]);
		delayMs(13);
	}

	front = sonarGet(ping[0]);
	right = sonarGet(ping[1]);
	back = sonarGet(ping[2]);
	left = sonarGet(ping[3]);

	length = front + back + 20;
	width = left + right + 15;

	sprintf(temp1, "l=%d w=%d", length, width);
	sprintf(temp2, "l=%2d r=%2d", left, right);
	sprintf(temp3, "F%d L%d R%d B%d", front, left, right, back);
	lcd_putsf(0, 0, temp3);

	if(checkColorSensor() == GRAY){
		robotPos = ROOM2;
		lcd_putsf(0, 1, "ROOM2");
	}
	else{
		if(width > ROOM1_WIDTH - 8){
			if(front < ROOM1_WIDTH - 5){
				robotPos = ROOM1;
				room1Door = DOORB;
				lcd_putsf(0, 1, "ROOM1B");
			}
			else{
				robotPos = ROOM1;
				room1Door = DOORA;
				lcd_putsf(0, 1, "ROOM1A");
			}
		}
		else if((width > ROOM3_WIDTH - 15) && (width < ROOM1_WIDTH - 10)){
			if(front > ROOM4_WIDTH + 5){
				robotPos = ROOM3;
				lcd_putsf(0, 1, "ROOM3");
			}
			else{
				robotPos = ROOM4;
				lcd_putsf(0, 1, "ROOM4");
			}
		}
//		else if((width > ROOM4_LENGTH) - 10 && (width < ROOM3_LENGTH - 10)){
//			if(back < ROOM4_WIDTH + 10){
//				robotPos = ROOM4;
//				lcd_putsf(0, 1, "ROOM4");
//			}
//		}
		else{
			lcd_putsf(0, 1, "NOT ROOM");
		}
	}
	delayMs(50);

	for (i = 0; i < 4; i++) {
		sonarSetGain(ping[i], 5);
		sonarSetRange(ping[i], 800);
	}

	return robotPos;
}
uint8_t checkFire() {
	uint8_t photon = uvtronCheck(1000);
	photon = 5;
	if(photon > 1) {
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t checkingFire(uint16_t time, uint8_t totalMove) {
	uint8_t photon = uvtronChecking(time, totalMove);
	if(photon > 1) {
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t searchFire() {
	lcd_clear();
	lcd_putsf(0,1,"FIRE SEARCH");
	uint8_t left, right;
	const uint8_t ping[2] = {0xE4, 0xEC};
	uint16_t time = 70;

	sonarRange(ping[0]);
	right = sonarGet(ping[0]);
	sonarRange(ping[1]);
	left = sonarGet(ping[1]);
	sonarRange(ping[0]);
	right += sonarGet(ping[0]);
	sonarRange(ping[1]);
	left += sonarGet(ping[1]);

	if(left > right) {
//		rataKanan(200);
		searchAngle = -30;
	}
	else {
//		rataKiri(200);
		searchAngle = 30;
	}
	moveRobot(0, 0, -searchAngle, time);
	moveNormal(time);
	searchFireCounter = 0;
	char temp[10];

	while(1) {
//		nungging(1, 100);
		tpaValue tpaVal = tpaScanGetPosition();
//		noarmal(100);
		if(tpaVal.tpaPos != 0 && tpaVal.tpaTemp >= 36) {
			return tpaVal.tpaPos;
		}
		else {
			searchFireCounter++;
			sprintf(temp, "%d", tpaVal.tpaTemp);
			lcd_putsf(0,0,temp);
			moveRobot(0, 0, searchAngle, time);
			moveRobot(0, 0, searchAngle, time);
			moveRobot(0, 0, searchAngle, time);
			moveNormal(time);
			lcd_clear();
		}
	}
}

void approachFire(uint16_t tpa81Val) {
	lcd_clear();
	tpaSetServo(15);
	lcd_putsf(0,1,"APPROACH FIRE");
	uint8_t left, right, sFlag = 0;
	double x, y;
	const uint8_t ping[2] = {0xE4, 0xEC};
	uint16_t time = 150;

	sonarRange(ping[0]);
	right = sonarGet(ping[0]);
	sonarRange(ping[1]);
	left = sonarGet(ping[1]);
	sonarRange(ping[0]);
	right += sonarGet(ping[0]);
	sonarRange(ping[1]);
	left += sonarGet(ping[1]);
	searchFireCounter = 0;

	if(left > right) {
		searchAngle = -18;
		moveRobot(0, 0, -searchAngle, time);
		moveRobot(0, 0, -searchAngle, time);
		moveNormal(time);
	}
	else {
		searchAngle = 18;
		moveRobot(0, 0, -searchAngle, time);
		moveRobot(0, 0, -searchAngle, time);
		moveNormal(time);
	}
	tpa81ResetValue(0);
	while(1) {
		if(tpa81Fire) {
			if(searchFireCounter > 2) {
				searchFireCounter -= 3;
			}
			else {
				searchFireCounter = 0;
			}
			break;
		}
		moveRobotTPA(0, 0, searchAngle, time);
		searchFireCounter++;
	}
	moveRobot(0, 0, -searchAngle, time);
	delayMs(1000);
	double moveSearchAngle = searchAngle / 1.8;
	tpa81ResetValue(tpa81Val);
	moveFireCounter = 0;
	while(1) {
		if(moveSearchAngle < 0){
			sFlag = 0;
		}
		else{
			sFlag = 1;
		}
		if(lsFrontLeft() && !lsFrontRight()){
			moveRobot(3, -3, 0, time);
		}
		else if(lsFrontRight() && !lsFrontLeft()){
			moveRobot(-3, -3, 0, time);
		}
//		else if(lsFrontRight() && lsFrontLeft()){
//			moveRobot(0, -2, 0, time);
//			sonarRange(ping[0]);
//			right = sonarGet(ping[0]);
//			sonarRange(ping[1]);
//			left = sonarGet(ping[1]);
//			sonarRange(ping[0]);
//			right += sonarGet(ping[0]);
//			sonarRange(ping[1]);
//			left += sonarGet(ping[1]);
//			searchFireCounter = 0;
//
//			if(left > right) {
//				moveRobot(-2, 0, 0, time);
//				moveRobot(-2, 0, 0, time);
//				moveNormal(time);
//			}
//			else {
//				moveRobot(2, 0, 0, time);
//				moveRobot(2, 0, 0, time);
//				moveNormal(time);
//			}
//		}

		if(stopSearch) {
			moveRobot(0, -2, 0, time);
			moveRobot(0, -2, 0, time);
			moveNormal(time);
			break;
		}
//		else if(proximityDetected) {
//			moveRobot(0, -1, 0, time);
//			moveRobot(0, -1, 0, time);
//			moveNormal(time);
//			break;
//		}
		else if(tpa81Fire) {
			moveRobot(0, 3, -moveSearchAngle * 1.8, time);
			if(sFlag){
				moveSearchAngle = -(moveSearchAngle - 1);
			}
			else{
				moveSearchAngle = -(moveSearchAngle + 1);
			}
//			moveSearchAngle = -moveSearchAngle;
			tpa81Value = 0;
			oldTpa81Value = 0;
			tpa81Fire = 0;
			tpa81Ok = 0;
			moveFireCounter++;
		}
		moveRobotTPA(0, 2, -moveSearchAngle, time);
		moveFireCounter++;
	}
}

void approachKillFire(uint16_t tpa81Val) {
	uint8_t a, b, tpaPos, min = 3, plus = 3;
	uint16_t time = 70;

	lcd_clear();
	lcd_putsf(0,0,"MATIIN API");
	fireLedStartBlink();

	wf = 0;
	approachFire(tpa81Val);
	tpaPos = tpaScanGetPosition().tpaPos + 2;
	do {
		nungging(3.8, 700);
		lcd_putsf(0,1,"KIPAS ON");
		kipasStart();
		if(tpaPos < 4 + min) {
			min = tpaPos - 4;
		}
		else if(tpaPos > 27 - plus) {
			plus = 27 - tpaPos;
		}
		for(b = 0; b < 5; b++) {
			for(a = tpaPos - min; a <= tpaPos + plus; a++){
				tpaSetServo(a);
				delayMs(60);
			}
		}
		tpaSetServo(tpaPos);
		normal(1000);
	}
	while(checkFire());
	delayMs(500);
	kipasStop();
	lcd_putsf(0,1,"KIPAS OFF");
	for(a = 0; a < moveFireCounter; a++) {
		moveRobot(0, -5, 0, time);
	}
	for(a = 0; a < searchFireCounter; a++) {
		moveRobot(0, 0, -searchAngle, time);
	}

	if(searchAngle < 0) {
		rataKanan(time);
		geserKanan(time);
	}
	else {
		rataKiri(time);
		geserKiri(time);
	}

	fireStatus = 0;
	fireLedStopBlink();
}

void killFire() {
	uint8_t a, b, tpaPos, min = 3, plus = 3;
	uint16_t time = 70;

	lcd_clear();
	lcd_putsf(0,0,"MATIIN API");
	fireLedStartBlink();

	wf = 0;
	tpaPos = searchFire();
	tpaPos = tpaPos + 2;
	do {
		nungging(3.8, 700);
		lcd_putsf(0,1,"KIPAS ON");
		kipasStart();
		if(tpaPos < 4 + min) {
			min = tpaPos - 4;
		}
		else if(tpaPos > 27 - plus) {
			plus = 27 - tpaPos;
		}
		for(b = 0; b < 5; b++) {
			for(a = tpaPos - min; a <= tpaPos + plus; a++) {
				tpaSetServo(a);
				delayMs(60);
			}
		}
		tpaSetServo(tpaPos);
		normal(1000);
	}
	while(checkFire());
	delayMs(500);
	kipasStop();
	lcd_putsf(0,1,"KIPAS OFF");
	moveRobot(0, 0, searchAngle, time);
	for(a = 0; a < searchFireCounter; a++) {
		moveRobot(0, 0, -searchAngle, time);
		moveRobot(0, 0, -searchAngle, time);
		moveRobot(0, 0, -searchAngle, time);
	}

	if(searchAngle < 0) {
		rataKanan(time);
		geserKanan(time);
	}
	else {
		rataKiri(time);
		geserKiri(time);
	}

	fireStatus = 0;
	fireLedStopBlink();
}

void killFireWater(){
	moveRobot(0, -2, -13, 150);
	moveRobot(0, -2, -14, 150);
	moveNormal(150);
	kipasStart();
	delayMs(300);
	kipasStop();
	delayMs(1000);
	kipasStart();
	delayMs(300);
	kipasStop();
}

void navigationRoom4(dollPosition dollPos, uint16_t time) {
	lcd_clear();
	const uint8_t ping[3] = {0xE4, 0xEC, 0xE0};
	uint8_t left, right, i;
	if(dollPos == POS2) {
		kiri(time, BLACK);
		delayMs(50);
		wf = 1;
		moveRobot(0, 3, 0, time);
//		moveRobot(0, 4, 0, time);
		rataKiri(time);
		for(i = 0; i < 2; i++) {
			sonarRange(ping[i]);
			delayMs(10);
		}
		right = sonarGet(ping[0]);
		left = sonarGet(ping[1]);

		//Cek apakah room 4 model 2
		if(right > 33) {
			room4Model = MODEL2;
			wf = 1;
			moveRobot(0, -4, 0, time);
			kanan(time, BLACK);
			kanan(time, LINE);
			kiri(time, BLACK);
			if(fireStatus){
				if(checkingFire(time, 2)) {
					room4 = ON_FIRE;
					do {
						killFire();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
	//				kanan(time, LINE);
					room4 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
		//room 4 adalah model 1
		else {
			room4Model = MODEL1;
			kanan(time, GRAY);
			kanan(time, BLACK);
			delayMs(50);
			wf = 1;
			moveRobot(0, 4, 0, time);
			rataKiri(time);
			uint8_t i;
			for(i = 0; i < 2; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
			right = sonarGet(ping[0]);
			left = sonarGet(ping[1]);

			if(right > 30) {
				wf = 1;
				moveRobot(0, -2, 0, time);
				kanan(time, BLACK);
				kanan(time, LINE);
				kanan(time, LINE);
				kiri(time, BLACK);
					if(fireStatus){
						if(checkingFire(time, 2)) {
							room4 = ON_FIRE;
							do {
								killFire();
							}
							while(checkFire());
							putarBalik(time);
							fireStatus = 0;
						}
						else {
							putarKanan90(time);
//							kanan(time, LINE);
							room4 = CHECKED;
						}
					}
					else{
						roomStop(time);
						return;
					}
			}
		}
	}
}

void roomStop(uint16_t time){
	delayMs(50);
	moveRobot(0, 5, 0, time);
	moveRobot(0, 5, 0, time);
	moveRobot(0, 5, 0, time);
	moveNormal(time);
}

void room4Start(uint16_t time){
	const uint8_t ping[3] = {0xE4, 0xEC, 0xE0};
	uint8_t left, right, i;
	roomType robotPost;
	uint8_t room1DoorACorrect = 0;
	uint8_t room1DoorBCorrect = 0;
	uint8_t room2Correct = 0;
	uint8_t room3Correct = 0;
	uint8_t room4Correct = 0;

	kanan(time, LINE);
	delayMs(50);
	putarKiri90(time);
	geserKanan(time);
	rataKanan(time);
	if(kananProx(time, GRAY) == 1){
		delayMs(50);
		moveRobot(0, -4, 0, time);
		moveRobot(0, -4, 0, time);
		moveRobot(0, -4, 0, time);
		putarBalik(time);
		kiri(time, GRAY);
		kiri(time, BLACK);
		kiri(time, LINE);
		delayMs(50);
		wf = 1;
		moveRobot(0, 4, 0, time);
		kiri(time, BLACK);
		rataKiri(time);

		for(i = 0; i < 6; i++){
			robotPost = checkRoomOut();
			if(robotPost == ROOM1) {
				if(room1Door == DOORA) {
					room1DoorACorrect++;
				}
				else if(room1Door == DOORB){
					room1DoorBCorrect++;
				}
			}
//			else if(robotPost == ROOM2) {
//				room2Correct++;
//			}
			else if(robotPost == ROOM3) {
				room3Correct++;
			}
//			else if(robotPost == ROOM4) {
//				room4Correct++;
//			}
		}
		if(room1DoorACorrect > 4) {
			robotPos = ROOM1;
			room1Door = DOORA;
		}
		else if(room1DoorBCorrect > 4) {
			robotPos = ROOM1;
			room1Door = DOORB;
		}
		else if(room2Correct > 4) {
			robotPos = ROOM2;
		}
		else if(room3Correct > 4) {
			robotPos = ROOM3;
		}
		else if(room4Correct > 4) {
			robotPos = ROOM4;
		}

		if(room1Door == DOORA){
			room4Model = MODEL2;
			robotPos = ROOM1;
			photon = 0;
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			wf = 1;
			moveNormal(time);

			for(i = 0; i < 2; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
			right = sonarGet(ping[0]);
			left = sonarGet(ping[1]);

		//	putarBalik(time);
			if(right > left) {
				room1Model = TYPE1;
				rataKiri(time);
			}
			else {
				room1Model = TYPE2;
				rataKanan(time);
			}
			if(fireStatus){
				if(photon > 1) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else if(checkingFire(time, 0)) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
					room1 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
		else if(room1Door == DOORB){
			room4Model = MODEL1;
			robotPos = ROOM1;
			dollPos = POS1;
			photon = 0;
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveNormal(time);

			if(fireStatus){
				if(photon > 1) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else if(checkingFire(time, 0)) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKiri90(time);
					room1 = CHECKED;
				}
			}
			else {
				roomStop(time);
				return;
			}
		}
	}
	else {
		delayMs(1000);
		kanan(time, BLACK);
		delayMs(1000);
		kanan(time, LINE);
		delayMs(1000);
		kiri(time, BLACK);
		geserKiri(time);
		rataKiri(time);

		for(i = 0; i < 6; i++){
			robotPost = checkRoomOut();
			if(robotPost == ROOM1) {
				if(room1Door == DOORA) {
					room1DoorACorrect++;
				}
				else if(room1Door == DOORB){
					room1DoorBCorrect++;
				}
			}
//			else if(robotPost == ROOM2) {
//				room2Correct++;
//			}
			else if(robotPost == ROOM3) {
				room3Correct++;
			}
//			else if(robotPost == ROOM4) {
//				room4Correct++;
//			}
		}
		if(room1DoorACorrect > 4) {
			robotPos = ROOM1;
			room1Door = DOORA;
		}
		else if(room1DoorBCorrect > 4) {
			robotPos = ROOM1;
			room1Door = DOORB;
		}
		else if(room2Correct > 4) {
			robotPos = ROOM2;
		}
		else if(room3Correct > 4) {
			robotPos = ROOM3;
		}
		else if(room4Correct > 4) {
			robotPos = ROOM4;
		}

		if(robotPos == ROOM3){
			room4Model = MODEL1;
			if(fireStatus){
				if(checkingFire(time, 3) == 1) {
					room3 = ON_FIRE;
					do {
						approachFire(150);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
					room3 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
		else if(robotPos == ROOM1){
			room4Model = MODEL2;
			room1Door = DOORB;
			robotPos = ROOM1;
			photon = 0;
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveNormal(time);

			if(fireStatus){
				if(photon > 1) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else if(checkingFire(time, 0)) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					room1 = CHECKED;
					putarKiri90(time);
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
	}
}
void room1toRoom2(uint16_t time){
	const uint8_t ping[3] = {0xE6, 0xEA, 0xE0};
	uint8_t left, right, front, i;
	sonarRange(ping[0]);
	right = sonarGet(ping[0]);
	sonarRange(ping[1]);
	left = sonarGet(ping[1]);
	sonarRange(ping[0]);
	right += sonarGet(ping[0]);
	sonarRange(ping[1]);
	left += sonarGet(ping[1]);

	if(right > left){
		kiri(time, LINE);
		delayMs(50);
		wf = 1;
		moveRobot(-1, 2, 0, time);
		kiri(time, BLACK);
		delayMs(50);
		wf = 1;
		moveRobot(-1, 2, 0, time);
		kiri(time, LINE);
	}
	else{
		kanan(time, LINE);
		delayMs(50);
		putarKiri90(time);
		geserKanan(time);
		kanan(time, BLACK);
		kanan(time, LINE);
	}

	geserKiri(time);
//	rataKiri(time);
	if(fireStatus){
		if(checkingFire(time, 4)) {
			room2 = ON_FIRE;
			do {
				approachFire(150);
				killFireWater();
			}
			while(checkFire());
			putarBalik(time);
			fireStatus = 0;
		}
		else {
			putarKanan90(time);
//			kanan(time, LINE);
			room2 = CHECKED;
		}
	}
	else{
		roomStop(time);
		return;
	}
}

void room1toRoom3(uint16_t time){
	const uint8_t ping[2] = {0xE6, 0xEA};
	uint8_t i;
	uint8_t left, right;

	for(i = 0; i < 2; i++) {
		sonarRange(ping[i]);
		delayMs(10);
	}
	right = sonarGet(ping[0]);
	left = sonarGet(ping[1]);

	for(i = 0; i < 2; i++) {
		sonarRange(ping[i]);
		delayMs(10);
	}
	right += sonarGet(ping[0]);
	left += sonarGet(ping[1]);

	if(right > left){
		kiri(time, LINE);
	}
	else{
		kanan(time, LINE);
	}

	if(room1Model == TYPE1){
		delayMs(50);
		putarKanan90(time);
		geserKiri(time);
		kiri(time, GRAY);
	}
	else if(room1Model == TYPE2){
		kanan(time, BLACK);
		kanan(time, GRAY);
		kiriMundur(time, BLACK);
		geserKiri(time);
		kiri(time, GRAY);
	}
	kiri(time, BLACK);
	kiri(time, LINE);
	kiri(time, BLACK);
	if(fireStatus){
		if(checkingFire(time, 4)) {
			room3 = ON_FIRE;
			do {
				approachFire(150);
				killFireWater();
			}
			while(checkFire());
			putarBalik(time);
			fireStatus = 0;
		}
		else {
			putarKanan90(time);
//			kanan(time, LINE);
			room3 = CHECKED;
		}
	}
	else{
		roomStop(time);
		return;
	}
}

void room1toRoom4Unchecked(uint16_t time){
	const uint8_t ping[3] = {0xE4, 0xEC, 0xE0};
	uint8_t left, right, front, i;

	if(room1Model == TYPE1) {
		kiri(time, BLACK);
		kiri(time, LINE);
	}
	else {
		kanan(time, BLACK);
		kanan(time, LINE);
	}

	rataKanan(time);
	kanan(time, BLACK);
	if(kananProx(time, GRAY) == 1) {
		dollPos = POS2;
		kananMundur(time, LINE);
		delayMs(50);
		moveRobot(-2, 4, 0, time);
		wf = 1;
		moveRobot(-2, 4, 0, time);
		kiri(time, BLACK);
		kiri(time, GRAY);
		navigationRoom4(POS2, time);
	}
	else {
		if(kananProx(time, BLACK) == 2) {
			for(i = 0; i < 2; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
			right = sonarGet(ping[0]);
			left = sonarGet(ping[1]);
			if(left > 40) {
				delayMs(30);
				putarKiri90(time);
				geserKanan(time);
				if(kananProx(time, BLACK) ==  2) {
					lcd_clear();
					dollPos == POS1;
					moveRobot(0, -2, 0, time);
					putarKiri90(time);
					kiri(time, BLACK);
					kanan(time, GRAY);
					navigationRoom4(POS2, time);
				}
				else {
					moveRobot(0, 4, 0, time);
					moveRobot(0, 4, 0, time);
					rataKanan(time);
					left = 0;
					right = 0;
					for(i = 0; i < 3; i++) {
						sonarRange(ping[i]);
						delayMs(10);
					}
					right = sonarGet(ping[0]);
					left = sonarGet(ping[1]);
					if(kananProx(time, GRAY) == 1) {
						dollPos = POS1;
						kananMundur(time, GRAY);
						if(left > 30) {
							room4Model = MODEL1;
							delayMs(50);
							wf = 1;
							moveRobot(0, 4, 0, time);
							kiri(time, BLACK);
							kiri(time, LINE);
							kiri(time, BLACK);
							if(fireStatus){
								if(checkingFire(time, 2)) {
									room4 = ON_FIRE;
									do {
										approachFire(150);
										killFireWater();
									}
									while(checkFire());
									putarBalik(time);
									fireStatus = 0;
								}
								else {
									putarKanan90(time);
					//				kanan(time, LINE);
									room4 = CHECKED;
								}
							}
							else{
								roomStop(time);
								return;
							}
						}
						else {
							room4Model = MODEL2;
							delayMs(50);
							moveRobot(0, -4, 0, time);
							putarBalik(time);
							kiri(time, GRAY);
							kiri(time, BLACK);
							kanan(time, GRAY);
							kanan(time, BLACK);
							kanan(time, LINE);
							kiri(time, BLACK);
							if(fireStatus){
								if(checkingFire(time, 2)) {
									room4 = ON_FIRE;
									do {
										approachFire(150);
										killFireWater();
									}
									while(checkFire());
									putarBalik(time);
									fireStatus = 0;
								}
								else {
									putarKanan90(time);
					//				kanan(time, LINE);
									room4 = CHECKED;
								}
							}
							else{
								roomStop(time);
								return;
							}
						}
					}
					else {
						if(left > 30) {
							room4Model = MODEL1;
							delayMs(50);
							putarBalik(time);
							kiri(time, BLACK);
							delayMs(50);
							moveRobot(0, 4, 0, time);
							wf = 1;
							moveRobot(0, 4, 0, time);
							kanan(time, LINE);
							kiri(time, BLACK);
							if(fireStatus){
								if(checkingFire(time, 2)) {
									room4 = ON_FIRE;
									do {
										approachFire(150);
										killFireWater();
									}
									while(checkFire());
									putarBalik(time);
									fireStatus = 0;
								}
								else {
									putarKanan90(time);
					//				kanan(time, LINE);
									room4 = CHECKED;
								}
							}
							else{
								roomStop(time);
								return;
							}
						}
						else {
							room4Model = MODEL2;
							kiri(time, BLACK);
							kiri(time, LINE);
							kiri(time, BLACK);
							if(fireStatus){
								if(checkingFire(time, 2)) {
									room4 = ON_FIRE;
									do {
										approachFire(150);
										killFireWater();
									}
									while(checkFire());
									putarBalik(time);
									fireStatus = 0;
								}
								else {
									putarKanan90(time);
					//				kanan(time, LINE);
									room4 = CHECKED;
								}
							}
							else{
								roomStop(time);
								return;
							}
						}
					}
				}
			}
			else {
				putarBalik(time);
				dollPos = POS2;
				kanan(time, BLACK);
				kanan(time, GRAY);
				navigationRoom4(POS2, time);
			}
		}
		else {
			delayMs(50);
			moveRobot(0, 4, 0, time);
			wf = 1;
			moveRobot(0, 4, 0, time);
			rataKanan(time);
			left = 0;
			right = 0;
			for(i = 0; i < 2; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
			right = sonarGet(ping[0]);
			left = sonarGet(ping[1]);
			if(kananProx(time, GRAY) == 1) {
				dollPos = POS1;
				kananMundur(time, GRAY);
				if(left > 30) {
					room4Model = MODEL1;
					delayMs(50);
					wf = 1;
					moveRobot(0, -4, 0, time);
					kiri(time, LINE);
					kiri(time, BLACK);
					if(fireStatus){
						if(checkingFire(time, 2)) {
							room4 = ON_FIRE;
							do {
								approachFire(150);
								killFireWater();
							}
							while(checkFire());
							putarBalik(time);
							fireStatus = 0;
						}
						else {
							putarKanan90(time);
			//				kanan(time, LINE);
							room4 = CHECKED;
						}
					}
					else{
						roomStop(time);
						return;
					}
				}
				else {
					room4Model = MODEL2;
					delayMs(50);
					moveRobot(0, -4, 0, time);
					putarBalik(time);
					kiri(time, GRAY);
					kiri(time, BLACK);
					kanan(time, GRAY);
					kanan(time, BLACK);
					kanan(time, LINE);
					kiri(time, BLACK);
					if(fireStatus){
						if(checkingFire(time, 2)) {
							room4 = ON_FIRE;
							do {
								approachFire(150);
								killFireWater();
							}
							while(checkFire());
							putarBalik(time);
							fireStatus = 0;
						}
						else {
							putarKanan90(time);
			//				kanan(time, LINE);
							room4 = CHECKED;
						}
					}
					else{
						roomStop(time);
						return;
					}
				}
			}
			else {
				putarBalik(time);
				room4Model = MODEL1;
				kanan(time, BLACK);
				kanan(time, LINE);
				kiri(time, BLACK);
				if(fireStatus){
					if(checkingFire(time, 2)) {
						room4 = ON_FIRE;
						do {
							approachFire(150);
							killFireWater();
						}
						while(checkFire());
						putarBalik(time);
						fireStatus = 0;
					}
					else {
						putarKanan90(time);
		//				kanan(time, LINE);
						room4 = CHECKED;
					}
				}
				else{
					roomStop(time);
					return;
				}
			}
		}
	}
}

void room1toRoom4Checked(uint16_t time){
	uint16_t left, right;
	uint8_t i, correct;
	const uint8_t ping[2] = {0xE4, 0xEC};
	roomType robotPost;
	uint8_t room1DoorACorrect = 0;
	uint8_t room1DoorBCorrect = 0;

	if(origin == ROOM4){
		for(i = 0; i < 2; i++) {
			sonarRange(ping[i]);
			delayMs(10);
		}
		right = sonarGet(ping[0]);
		left = sonarGet(ping[1]);

		for(i = 0; i < 2; i++) {
			sonarRange(ping[i]);
			delayMs(10);
		}
		right += sonarGet(ping[0]);
		left += sonarGet(ping[1]);

		if(right > left){
			kiri(time, LINE);
			kiriMundur(time, BLACK);
		}
		else{
			kanan(time, LINE);
			kananMundur(time, BLACK);
		}
		delayMs(50);
		moveRobot(0, -4, 0, time);
		moveRobot(0, -4, 0, time);
		moveRobot(0, -4, 0, time);
		wf = 1;
		moveNormal(time);

		sonarRange(ping[0]);
		right = sonarGet(ping[0]);
		sonarRange(ping[1]);
		left = sonarGet(ping[1]);

		if(right > left){
			geserKiri(time);
		}
		else{
			geserKanan(time);
		}
		for(i = 0; i < 6; i++){
			robotPost = checkRoomIn();
			if(robotPost == ROOM1) {
				if(room1Door == DOORA) {
					room1DoorACorrect++;
				}
				else if(room1Door == DOORB){
					room1DoorBCorrect++;
				}
			}
			else{

			}
		}

		if(room1DoorACorrect > 4){
			room1Door = DOORA;
		}
		if(room1DoorBCorrect > 4){
			room1Door = DOORB;
		}

		if(room1Door == DOORA){
			for(i = 0; i < 2; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
			right = sonarGet(ping[0]);
			left = sonarGet(ping[1]);

			for(i = 0; i < 2; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
			right += sonarGet(ping[0]);
			left += sonarGet(ping[1]);

			if(right > left){
				kiri(time, LINE);
			}
			else{
				kanan(time, LINE);
			}

			if(room4Model == MODEL2){
				kanan(time, BLACK);
				kanan(time, GRAY);
				kanan(time, BLACK);
				kiri(time, LINE);
				kiri(time, BLACK);
				roomStop(time);
				return;
			}
			else{
				if(dollPos == POS1){
					kanan(time, BLACK);
					kanan(time, GRAY);
					kanan(time, BLACK);
					kanan(time, GRAY);
					kanan(time, BLACK);
					kiri(time, GRAY);
					kanan(time, BLACK);
					delayMs(50);
					wf = 1;
					moveRobot(0, 4, 0, time);
					kiri(time, LINE);
					kiri(time, BLACK);
					roomStop(time);
					return;
				}
				else{
					if(room1Model == TYPE1){
						putarKanan90(time);
						geserKiri(time);
						kiri(time, GRAY);
					}
					else{
						kanan(time, BLACK);
						kanan(time, GRAY);
						kiriMundur(time, BLACK);
						geserKiri(time);
						kiri(time, GRAY);
					}
					kiri(time, BLACK);
					delayMs(50);
					moveRobot(0, 4, 0, time);
					moveRobot(0, 4, 0, time);
					geserKiri(time);
					putarBalik(time);
					kanan(time, GRAY);
					kiri(time, BLACK);
					delayMs(50);
					wf = 1;
					moveRobot(0, 4, 0, time);
					kanan(time, LINE);
					kiri(time, BLACK);
					return;
				}
			}
		}
		// Pintu di room 1 adalah DOOR B
		else{
			if(room4Model == MODEL1){
				if(dollPos == POS1){
					kanan(time, LINE);
					kanan(time, BLACK);
					kanan(time, GRAY);
					kanan(time, BLACK);
					delayMs(50);
					wf = 1;
					moveRobot(0, 4, 0, time);
					kiri(time, LINE);
					kiri(time, BLACK);
					roomStop(time);
					return;
				}
				else{
					putarBalik(time);
					kiri(time, BLACK);
					kiri(time, LINE);

					if(room1Model == TYPE1){
						delayMs(50);
						putarKanan90(time);
						geserKiri(time);
						kiri(time, GRAY);
					}
					else{
						kanan(time, BLACK);
						kanan(time, GRAY);
						kiriMundur(time, BLACK);
						geserKiri(time);
						kiri(time, GRAY);
					}

					kiri(time, BLACK);
					delayMs(50);
					moveRobot(0, 4, 0, time);
					moveRobot(0, 4, 0, time);
					putarBalik(time);
					kanan(time, GRAY);
					kiri(time, BLACK);
					delayMs(50);
					wf = 1;
					moveRobot(0, 4, 0, time);
					kanan(time, LINE);
					kiri(time, BLACK);
					return;
				}
			}
			//Model di room 4 adalah MODEL 2, DOOR B
			else{
				if(dollPos == POS3){
					kanan(time, LINE);
					kanan(time, BLACK);
					kanan(time, GRAY);
					kanan(time, BLACK);
					kiri(time, GRAY);
					kiri(time, BLACK);
					delayMs(50);
					wf = 1;
					moveRobot(0, 4, 0, time);
					kiri(time, LINE);
					kiri(time, BLACK);
					roomStop(time);
					return;
				}
				else{
					kanan(time, LINE);
					kiri(time, BLACK);
					delayMs(50);
					wf = 1;
					moveRobot(-3, 4, 0, time);
					kiri(time, GRAY);
					kiri(time, BLACK);
					delayMs(50);
					wf = 1;
					moveRobot(0, 4, 0, time);
					kanan(time, LINE);
					kiri(time, BLACK);
					roomStop(time);
					return;
				}
			}
		}
	}
	else{
		room1toRoom4Unchecked(time);
	}
}

void room2toRoom1(uint16_t time){
	const uint8_t ping[3] = {0xE6, 0xEA, 0xE0};
	uint8_t left, right, front, i;

	kanan(time, LINE);
	kanan(time, BLACK);
	kanan(time, LINE);
	photon = 0;
	delayMs(50);
	moveRobot(0, 5, 0, time);
	moveRobot(0, 5, 0, time);
	moveRobot(0, 5, 0, time);
	moveRobot(0, 5, 0, time);
	wf = 1;
	moveNormal(time);

	for(i = 0; i < 2; i++) {
		sonarRange(ping[i]);
		delayMs(10);
	}
	right = sonarGet(ping[0]);
	left = sonarGet(ping[1]);

//	putarBalik(time);
	if(right > left) {
		room1Model = TYPE1;
		rataKiri(time);
	}
	else {
		room1Model = TYPE2;
		rataKanan(time);
	}
	if(fireStatus){
		if(photon > 1) {
			room1 = ON_FIRE;
			do {
				approachFire(50);
				killFireWater();
			}
			while(checkFire());
			if(origin == ROOM4 && (room1Door == DOORA || room1Door == NOTYPE)) {

			}
			else {
				putarBalik(time);
			}
			fireStatus = 0;
		}
		else if(checkingFire(time, 0)) {
			room1 = ON_FIRE;
			do {
				approachFire(50);
				killFireWater();
			}
			while(checkFire());
			if(origin == ROOM4 && (room1Door == DOORA || room1Door == NOTYPE)) {

			}
			else {
				putarBalik(time);
			}
			fireStatus = 0;
		}
		else {
			if(room1Model == TYPE1) {
				putarKiri90(time);

			}
			else if(room1Model == TYPE2) {
				putarKanan90(time);
			}
			room1 = CHECKED;
		}
	}
	else{
		roomStop(time);
		return;
	}
}

void room2toRoom3(uint16_t time){
	kanan(time, LINE);
	delayMs(50);
//	moveRobot(-3, 4, 0, time);			uncomment jika pake sepatu tpe
	wf = 1;
	moveRobot(-4, 3, 0, time);
	kiri(time, BLACK);
	kiri(time, GRAY);
	kiri(time, BLACK);
	kiri(time, LINE);
	if(fireStatus){
		if(checkingFire(time, 4)) {
			room3 = ON_FIRE;
			do {
				approachFire(140);
				killFireWater();
			}
			while(checkFire());
			putarBalik(time);
			fireStatus = 0;
		}
		else {
			putarKanan90(time);
//			kanan(time, LINE);
			room3 = CHECKED;
		}
	}
	else{
		roomStop(time);
		return;
	}
}

void room2toRoom4(uint16_t time) {
	kanan(time, LINE);
	delayMs(50);
//	moveRobot(-3, 4, 0, time);
	wf = 1;
	moveRobot(-4, 3, 0, time);
	kiri(time, BLACK);
	kiri(time, GRAY);
	if(room4Model == MODEL1) {
		if(dollPos == POS1) {
			kanan(time, BLACK);
			kiri(time, GRAY);
			kiri(time, BLACK);
			kiri(time, GRAY);
			kiri(time, BLACK);
			kiri(time, LINE);
		}
		else {
			kiri(time, BLACK);
			delayMs(50);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			geserKiri(time);
			putarBalik(time);
			kanan(time, GRAY);
			kiri(time, BLACK);
			kanan(time, LINE);
		}
	}
	else if(room4Model == MODEL2) {
		delayMs(50);
		wf = 1;
		moveRobot(3, 3, 0, time);
		kanan(time, BLACK);
//		delayMs(50);
//		wf = 1;
//		moveRobot(2, 4, 0, time);		//uncomment jika pake sepatu tpe
		kiri(time, LINE);
	}
	kiri(time, BLACK);

	if(fireStatus){
		if(checkingFire(time, 2)) {
			room4 = ON_FIRE;
			do {
				approachFire(150);
				killFireWater();
			}
			while(checkFire());
			putarBalik(time);
			fireStatus = 0;
		}
		else {
			putarKanan90(time);
//			kanan(time, LINE);
			room4 = CHECKED;
		}
	}
	else{
		roomStop(time);
		return;
	}
}

void room3toRoom1(uint16_t time){
	const uint8_t ping[3] = {0xE6, 0xEA, 0xE0};
	uint8_t left, right, i;
	kanan(time, LINE);
	kanan(time, BLACK);
	kanan(time, GRAY);
	kanan(time, BLACK);
	kiri(time, LINE);
	kiri(time, BLACK);

	for(i = 0; i < 2; i++) {
		sonarRange(ping[i]);
		delayMs(10);
	}
	right = sonarGet(ping[0]);
	left = sonarGet(ping[1]);

	if(right < left) {
		room1Model = TYPE1;
	}
	else {
		room1Model = TYPE2;
	}

	if(fireStatus){
		if(checkingFire(time, 3) == 1) {
			room1 = ON_FIRE;
			do {
				approachFire(50);
				killFireWater();
			}
			while(checkFire());
			putarBalik(time);
			fireStatus = 0;
		}
		else {
			if(room1Model == TYPE1) {
				putarKanan90(time);

			}
			else if(room1Model == TYPE2) {
				putarKiri90(time);
			}
			room1 = CHECKED;
		}
	}
	else{
		roomStop(time);
		return;
	}
}
void room3toRoom2(uint16_t time){
	kanan(time, LINE);
	kanan(time, BLACK);
	kanan(time, GRAY);
	kanan(time, BLACK);
	kanan(time, LINE);
	geserKiri(time);
	if(fireStatus){
		if(checkingFire(time, 3) == 1) {
			room2 = ON_FIRE;
			do {
				approachFire(140);
				killFireWater();
			}
			while(checkFire());
			putarBalik(time);
			fireStatus = 0;
		}
		else {
			putarKanan90(time);
			room2 = CHECKED;
		}
	}
	else{
		roomStop(time);
		return;
	}
}

void room3toRoom4(uint16_t time){
	const uint8_t ping[3] = {0xE4, 0xEC, 0xE0};
	uint8_t left, right, front, i;

	kanan(time, LINE);
	kanan(time, BLACK);
	kanan(time, GRAY);
	if(room4 == UNCHECKED){
		do {
			kiriProx(time, BLACK);
			for(i = 0; i < 3; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
			right = sonarGet(ping[0]);
			left = sonarGet(ping[1]);
			front = sonarGet(ping[2]);
		}
		while (front >= 40);
		delayMs(20);
		putarKanan90(time);
		geserKiri(time);

		if(kiriProx(time, BLACK) == 2) {
			dollPos = POS1;
			moveRobot(0, -5, 0, time);
			moveRobot(0, -5, 0, time);
//			moveRobot(0, -5, 0, time);
			putarKanan90(time);
			geserKanan(time);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			wf = 1;
			moveRobot(0, 5, 0, time);
			kiri(time, BLACK);
			delayMs(50);
			moveRobot(0, 4, 0, time);
			moveRobot(0, 4, 0, time);
			moveNormal(time);
			for(i = 0; i < 2; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
//			moveRobot(0, -4, 0, time);
			wf = 1;
			moveRobot(0, -4, 0, time);
			right = sonarGet(ping[0]);
			left = sonarGet(ping[1]);

			if(left > 30) {
				room4Model = MODEL2;
				kiri(time, LINE);
				kiri(time, BLACK);
				if(fireStatus){
					if(checkingFire(time, 2)) {
						room4 = ON_FIRE;
						do {
							approachFire(150);
							killFireWater();
						}
						while(checkFire());
						putarBalik(time);
						fireStatus = 0;
					}
					else {
						putarKanan90(time);
			//			kanan(time, LINE);
						room4 = CHECKED;
					}
				}
				else{
					roomStop(time);
					return;
				}
			}
			else {
				room4Model = MODEL1;
				kiri(time, GRAY);
				kiri(time, BLACK);
				kiri(time, GRAY);
				kanan(time, BLACK);
				delayMs(50);
				wf = 1;
				moveRobot(0, 4, 0, time);
				kiri(time, LINE);
				kiri(time, BLACK);
				if(fireStatus){
					if(checkingFire(time, 2)) {
						room4 = ON_FIRE;
						do {
							approachFire(150);
							killFireWater();
						}
						while(checkFire());
						putarBalik(time);
						fireStatus = 0;
					}
					else {
						putarKanan90(time);
			//			kanan(time, LINE);
						room4 = CHECKED;
					}
				}
				else{
					roomStop(time);
					return;
				}
			}
		}
		else {
			delayMs(50);
			wf = 1;
			moveRobot(0, 3, 0, time);
//			moveRobot(0, 4, 0, time);
			rataKiri(time);
//			moveNormal(time);
			for(i = 0; i < 2; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
			right = sonarGet(ping[0]);
			left = sonarGet(ping[1]);

			if(right > 33) {
				wf = 1;
				moveRobot(0, -4, 0, time);
//				moveRobot(0, -4, 0, time);
//				moveNormal(time);
				room4Model = MODEL1;
				kanan(time, LINE);
				kiri(time, BLACK);
				if(fireStatus){
					if(checkingFire(time, 2)) {
						room4 = ON_FIRE;
						do {
							approachFire(150);
							killFireWater();
						}
						while(checkFire());
						putarBalik(time);
						fireStatus = 0;
					}
					else {
						putarKanan90(time);
			//			kanan(time, LINE);
						room4 = CHECKED;
					}
				}
				else{
					roomStop(time);
					return;
				}
			}
			else {
				room4Model = MODEL2;
				putarBalik(time);
				kiri(time, GRAY);
				kiri(time, BLACK);
				delayMs(50);
				wf = 1;
				moveRobot(-2, 4, 0, time);
				kiri(time, LINE);
				kiri(time, BLACK);
				if(fireStatus){
					if(checkingFire(time, 3) == 1) {
						room4 = ON_FIRE;
						do {
							approachFire(150);
							killFireWater();
						}
						while(checkFire());
						putarBalik(time);
						fireStatus = 0;
					}
					else {
						putarKanan90(time);
						room4 = CHECKED;
					}
				}
				else{
					roomStop(time);
					return;
				}
			}
		}
	}
//Dari room 3 ke room 4 return HOME
	else if(room4Model == MODEL2) {
		kanan(time, BLACK);
		delayMs(50);
		putarBalik(time);
		moveRobot(2, 4, 0, time);
		wf = 1;
		moveRobot(2, 4, 0, time);
		kanan(time, GRAY);
		kanan(time, BLACK);
		delayMs(50);
		wf = 1;
		moveRobot(-2, 4, 0, time);
		kiri(time, LINE);
		kiri(time, BLACK);
		if(fireStatus){
			if(checkingFire(time, 2)) {
				room4 = ON_FIRE;
				do {
					approachFire(150);
					killFireWater();
				}
				while(checkFire());
				putarBalik(time);
				fireStatus = 0;
			}
			else {
				putarKanan90(time);
//					kanan(time, LINE);
				room4 = CHECKED;
			}
		}
		else{
			roomStop(time);
			return;
		}
	}
	else if (room4Model == MODEL1){
		if(dollPos == POS1){
			kanan(time, BLACK);
			delayMs(50);
			putarBalik(time);
			wf = 1;
			moveRobot(0, 4, 0, time);
			kanan(time, GRAY);
			kanan(time, BLACK);
			kiri(time, GRAY);
			kiri(time, BLACK);
			kiri(time, GRAY);
			kanan(time, BLACK);
			delayMs(50);
			wf = 1;
			moveRobot(0, 4, 0, time);
			kiri(time, LINE);
			kiri(time, BLACK);
			if(fireStatus){
				if(checkingFire(time, 2)) {
					room4 = ON_FIRE;
					do {
						approachFire(150);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
//					kanan(time, LINE);
					room4 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
		else{
			kiri(time, BLACK);
			delayMs(50);
			wf = 1;
			moveRobot(0, 3, 0, time);
			kanan(time, LINE);
			kiri(time, BLACK);
			if(fireStatus){
				if(checkingFire(time, 2)) {
					room4 = ON_FIRE;
					do {
						approachFire(150);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
//					kanan(time, LINE);
					room4 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
	}
}

void room4toRoom1(uint16_t time){
	const uint8_t ping[3] = {0xE4, 0xEC, 0xE0};
	uint8_t left, right, i;

	kanan(time, LINE);
	if(room4Model == MODEL1){
		if(dollPos == POS1 || dollPos == POS3){
			room1Door = DOORB;
			kanan(time, BLACK);
			kanan(time, GRAY);
			kiri(time, BLACK);
			kiri(time, LINE);
			photon = 0;
			delayMs(50);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			wf = 1;
			moveNormal(time);

			if(fireStatus){
				for(i = 0; i < 2; i++) {
					sonarRange(ping[i]);
					delayMs(10);
				}
				right = sonarGet(ping[0]);
				left = sonarGet(ping[1]);

//				putarBalik(time);
				if(right < left) {
					rataKanan(time);
				}
				else {
					rataKiri(time);
				}
				if(photon > 1) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else if(checkingFire(time, 0)) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					room1 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
		else{
			room1Door = DOORA;
			delayMs(50);
			putarKiri90(time);
			geserKanan(time);
			kanan(time, GRAY);
			kanan(time, BLACK);
			geserKiri(time);
			delayMs(50);
			putarBalik(time);
			kanan(time, GRAY);
			kanan(time, BLACK);
			kiri(time, LINE);
			if(fireStatus){
				photon = 0;
				delayMs(50);
				moveRobot(0, 5, 0, time);
				moveRobot(0, 5, 0, time);
				moveRobot(0, 5, 0, time);
				moveRobot(0, 5, 0, time);
				wf = 1;
				moveNormal(time);

				for(i = 0; i < 2; i++) {
					sonarRange(ping[i]);
					delayMs(10);
				}
				right = sonarGet(ping[0]);
				left = sonarGet(ping[1]);

				if(right > left) {
					room1Model = TYPE1;
					rataKiri(time);
				}
				else {
					room1Model = TYPE2;
					rataKanan(time);
				}
				if(photon > 1) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else if(checkingFire(time, 0)) {
					room1 = ON_FIRE;
					do {
						approachFire(50);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					if(room1Model == TYPE1) {
						putarKanan90(time);
					}
					else if(room1Model == TYPE2) {
						putarKiri90(time);
					}
					room1 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
	}
	else{
		delayMs(50);
		putarKanan90(time);
		geserKiri(time);
		kiri(time, GRAY);
		kiri(time, BLACK);
		kiri(time, LINE);
		room1Door = DOORA;
		if(fireStatus){
			photon = 0;
			delayMs(50);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveRobot(0, 5, 0, time);
			moveNormal(time);

			for(i = 0; i < 2; i++) {
				sonarRange(ping[i]);
				delayMs(10);
			}
			right = sonarGet(ping[0]);
			left = sonarGet(ping[1]);

			if(right > left) {
				room1Model = TYPE1;
				rataKiri(time);
			}
			else {
				room1Model = TYPE2;
				rataKanan(time);
			}

			if(photon > 1) {
				room1 = ON_FIRE;
				do {
					approachFire(50);
					killFireWater();
				}
				while(checkFire());
				putarBalik(time);
				fireStatus = 0;
			}
			if(checkingFire(time, 0)) {
				room1 = ON_FIRE;
				do {
					approachFire(50);
					killFireWater();
				}
				while(checkFire());
				putarBalik(time);
				fireStatus = 0;
			}
			else {
				if(room1Model == TYPE1) {
					putarKanan90(time);
				}
				else if(room1Model == TYPE2) {
					putarKiri90(time);
				}
				room1 = CHECKED;
			}
		}
		else{
			roomStop(time);
			return;
		}
	}
}

void room4toRoom2(uint16_t time){
	kanan(time, LINE);
	if(room4Model == MODEL1){
		if(dollPos == POS1){
			delayMs(50);
			putarKanan90(time);
			geserKiri(time);
			kiri(time, GRAY);
			kiri(time, BLACK);
			kanan(time, GRAY);
			kanan(time, BLACK);
			kanan(time, GRAY);
			delayMs(50);
			moveRobot(0, 4, 0, time);
			wf = 1;
			moveRobot(0, 4, 0, time);
			kiri(time, BLACK);
			kanan(time, LINE);
			if(fireStatus){
				if(checkingFire(time, 3) == 1) {
					room2 = ON_FIRE;
					do {
						approachFire(140);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
	//				kanan(time, LINE);
					room2 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
		else{
			delayMs(50);
			putarKiri90(time);
			geserKanan(time);
			kanan(time, GRAY);
			kanan(time, BLACK);
			geserKiri(time);
			delayMs(50);
			putarBalik(time);
			kanan(time, GRAY);
			kanan(time, BLACK);
			kanan(time, LINE);
			if(fireStatus){
				if(checkingFire(time, 4)) {
					room2 = ON_FIRE;
					do {
						approachFire(140);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
	//				kanan(time, LINE);
					room2 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
	}
	else{
		delayMs(50);
		putarKanan90(time);
		geserKiri(time);
		kiri(time, GRAY);
		kiri(time, BLACK);
		kanan(time, LINE);
		if(fireStatus){
			if(checkingFire(time, 3) == 1) {
				room2 = ON_FIRE;
				do {
					approachFire(150);
					killFireWater();
				}
				while(checkFire());
				putarBalik(time);
				fireStatus = 0;
			}
			else {
				putarKanan90(time);
//				kanan(time, LINE);
				room2 = CHECKED;
			}
		}
		else{
			roomStop(time);
			return;
		}
	}
}

void room4toRoom3(uint16_t time){
	//Diasumsikan robot berada di dalam room 4. Melakukan wf kanan sampai ketemu garis room.
	kanan(time, LINE);
	//Jika mode room 4 diketahui merupakan model 1, maka bisa langsung ke room 3.
	if(room4Model == MODEL1){
		if(dollPos == POS1){
			kanan(time, BLACK);
			kanan(time, GRAY);
			kiri(time, BLACK);
			kanan(time, GRAY);
			kanan(time, BLACK);
			kanan(time, GRAY);
			kiri(time, BLACK);
			delayMs(50);
			putarBalik(time);
			kiri(time, GRAY);
			kiri(time, BLACK);
			kiri(time, LINE);
			if(fireStatus){
				if(checkingFire(time, 4)) {
					room3 = ON_FIRE;
					do {
						approachFire(140);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
	//				kanan(time, LINE);
					room3 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
		else{
			delayMs(50);
			putarKiri90(time);
			geserKanan(time);
			kanan(time, GRAY);
			kanan(time, BLACK);
			kiri(time, LINE);
			if(fireStatus){
				if(checkingFire(time, 4)) {
					room3 = ON_FIRE;
					do {
						approachFire(140);
						killFireWater();
					}
					while(checkFire());
					putarBalik(time);
					fireStatus = 0;
				}
				else {
					putarKanan90(time);
//					kanan(time, LINE);
					room3 = CHECKED;
				}
			}
			else{
				roomStop(time);
				return;
			}
		}
	}
	else{
		delayMs(50);
		putarKanan90(time);
		geserKiri(time);
		kiri(time, GRAY);
		kiri(time, BLACK);
		delayMs(50);
		putarBalik(time);
		geserKiri(time);
		kiri(time, GRAY);
		kiri(time, BLACK);
		kiri(time, LINE);
		if(fireStatus){
			if(checkingFire(time, 4)) {
				room3 = ON_FIRE;
				do {
					approachFire(140);
					killFireWater();
				}
				while(checkFire());
				putarBalik(time);
				fireStatus = 0;
			}
			else {
				putarKanan90(time);
//				kanan(time, LINE);
				room3 = CHECKED;
			}
		}
		else{
			roomStop(time);
			return;
		}
	}
}

void navigasiAllArbitrary(uint16_t time){
	room1 = UNCHECKED;
	room2 = UNCHECKED;
	room3 = UNCHECKED;
	room4 = UNCHECKED;

	roomStart(time);
	origin = robotPos;
	if(origin == ROOM4){
		room4Start(time);
		room4 = CHECKED;
		if(robotPos == ROOM1){
			if(!fireStatus){
				robotNavigation(ROOM1, origin, time);
			}
			else{
				if(!robotNavigation(ROOM1, ROOM2, time)){
					robotNavigation(ROOM2, origin, time);
				}
				else{
					if(!robotNavigation(ROOM2, ROOM3, time)){
						robotNavigation(ROOM3, origin, time);
					}
				}
			}
		}
		else if(robotPos == ROOM3){
			if(!fireStatus){
				robotNavigation(ROOM3, origin, time);
			}
			else{
				if(!robotNavigation(ROOM3, ROOM2, time)){
					robotNavigation(ROOM2, origin, time);
				}
				else{
					if(!robotNavigation(ROOM2, ROOM1, time)){
						robotNavigation(ROOM1, origin, time);
					}
				}
			}
		}
	}
	else{
		if(origin == ROOM1){
			room1Model = NOTYPE;
			if(room1Door == DOORB){
				if(!robotNavigation(ROOM1, ROOM4, time)){
					robotNavigation(ROOM4, origin, time);
				}
				else{
					if(!robotNavigation(ROOM4, ROOM3, time)){
						robotNavigation(ROOM3, origin, time);
					}
					else{
						if(!robotNavigation(ROOM3, ROOM2, time)){
							robotNavigation(ROOM2, origin, time);
						}
					}
				}
			}
			else{
				if(!robotNavigation(ROOM1, ROOM2, time)){
					robotNavigation(ROOM2, origin, time);
				}
				else{
					if(!robotNavigation(ROOM2, ROOM3, time)){
						robotNavigation(ROOM3, origin, time);
					}
					else{
						if(!robotNavigation(ROOM3, ROOM4, time)){
							robotNavigation(ROOM4, origin, time);
						}
					}
				}
			}
		}
		else if(origin == ROOM2){
			if(!robotNavigation(ROOM2, ROOM1, time)){
				robotNavigation(ROOM1, origin, time);
			}
			else{
				if(!robotNavigation(ROOM1, ROOM4, time)){
					robotNavigation(ROOM4, origin, time);
				}
				else{
					if(!robotNavigation(ROOM4, ROOM3, time)){
						robotNavigation(ROOM3, origin, time);
					}
				}
			}
		}
		else if(origin == ROOM3){
			if(!robotNavigation(ROOM3, ROOM2, time)){
				robotNavigation(ROOM2, origin, time);
			}
			else{
				if(!robotNavigation(ROOM2, ROOM1, time)){
					robotNavigation(ROOM1, origin, time);
				}
				else{
					if(!robotNavigation(ROOM1, ROOM4, time)){
						robotNavigation(ROOM4, origin, time);
					}
				}
			}
		}
	}
	delayMs(50);
	moveRobot(0, 5, 0, time);
	moveRobot(0, 5, 0, time);
	moveNormal(time);
}

void tes(uint16_t time) {
//	origin = room4;
//	room4 = CHECKED;
	room4Model = MODEL2;
	dollPos = POS3;
//	room4toRoom1(time);
//	room3toRoom4(time);
//	while(!button4Pressed());
	room2toRoom4(time);
}
