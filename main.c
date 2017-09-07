#include "main.h"

uint8_t srf[8] = {};
uint8_t tpaVal, val;
double  angle, x, y;
uint8_t status1, oldStatus, count;
int tpa1, tpaFlag = 0, dirFlag;
uint16_t initPos = 1500;
uint16_t nilaitpa;
int servotpa = 2;
colorStatus StatusWarna;
int pin;
// KIPAS P5
// Proximity E5
int main(void)
{
	uint8_t time = 130;
	SystemInit();
	initAll();
	delayMs(500);
	normal(500);
	lcd_clear();
	lcd_putsf(0, 4, "REINHARDT");
//	normal(time);
//	delayMs(2000);
	uint8_t coba = 0;

	while(1){
		char input[1];

		uartReceiveArray(COM2, *input);
		//front
		if (input == "F"){
			moveRobot(0, 1, angle, time);
		}
		//back
		if (input == "B") {
			moveRobot(0 , -1, angle, time);
		}
		//left
		if (input == "L") {
			moveRobot (-1, 0, angle, time);
		}
		//right
		if (input == "R") {
			moveRobot(0, -1, angle, time);
		}
		//detect
		if (input == "D") {
			tpaScanGetPosition();
		}
		//extinguish for 5s
		if (input == "X") {
			kipasStart();
			delayMs(5000);
			kipasStop();
		}


	}
 }

void initAll(){
	uint8_t i;
	uint8_t ping[7] = {0xE0, 0xE2, 0xE4, 0xE6, 0xEA, 0xEC, 0xEE};
	delayInit();
	lcd_init();
	delayMs(500);
	i2c2Init();
	uartInit();
	exti0Init();
	colorSensorInit();
	buttonInit();
	servoInit();
	colorSensorInit();
	timer5InitFreq(80);
	kipasInit();
	tpaScanGetPosition();
	proximityInit();
	for(i = 0; i < 7; i++){
		sonarSetRange(ping[i], 800);
		sonarSetGain(ping[i], 8);
	}
}

void tesSonar(){
	lcd_clear();
	char temp[16], temp2[16];
	uint8_t i;
	uint8_t ping[7] = {0xE0, 0xE2, 0xE4, 0xE6, 0xEA, 0xEC, 0xEE};
	while(!lsFrontLeft()){
		for(i = 0; i < 8; i++){
			sonarRange(ping[i]);
			delayMs(20);
		}

		for(i = 0; i < 8; i++){
			srf[i] = sonarGet(ping[i]);
		}
		sprintf(temp, "%2d %2d %2d %2d", srf[0], srf[1], srf[2], srf[3]);
		sprintf(temp2, "%2d %2d %2d %2d", srf[4], srf[5], srf[6], srf[7]);
		printf("\n%2d %2d %2d %2d %2d %2d %2d %2d", srf[0], srf[1], srf[2], srf[3], srf[4], srf[5], srf[6], srf[7]);
		lcd_putsf(0, 0, temp);
		lcd_putsf(0, 1, temp2);
		tpaVal = getTpa81();
	}
}

void searchFireWater(uint16_t minVal){
	kanan(time, LINE);
	geserKiri(time);
	tpaSetServo(16);
	moveRobot(-1, 3, -10, time);
	moveRobot(-1, 3, -10, time);
	moveNormal(time);
	rataKiri(time);
	angle = 8;
	dirFlag = 0;
	tpaFlag = 0;
	uint8_t oldTpa1;

	oldTpa1 = tpa1;
	tpa1 = getTpa81();
	if(tpa1 < 35 && tpaFlag == 0){
		moveRobot(0, 0, angle, time);
	}
	else{
		if(!tpaFlag){
			moveRobot(0, 3, 0, time);
		}
		tpaFlag = 1;
		if(tpa1 < 140){
			if(tpa1 < oldTpa1){
				angle *= -1;
			}
			else{
				angle = angle;
			}
			moveRobot(0, 1, angle, time);
		}
		else{
			moveRobot(0, 0, -15, time);
			moveRobot(0, 0, -14, time);
			kipasStart();
			delayMs(5000);
			kipasStop();
			while(1);
		}
	}
}

void extFire(){
	moveRobot(0, 0, 9, 150);
	moveNormal(150);
	kipasStart();
	delayMs(3000);
	kipasStop();
	delayMs(1000);
	kipasStart();
	delayMs(5000);
	kipasStop();
}

void navigasiAll(uint16_t time){
	if(!robotNavigation(HALL, ROOM3, time)){
		robotNavigation(ROOM3, HALL, time);
	}
	else{
		if(!robotNavigation(ROOM3, ROOM2, time)){
			robotNavigation(ROOM2, HALL, time);
		}
		else{
			if(!robotNavigation(ROOM2, ROOM1, time)){

				robotNavigation(ROOM1, HALL, time);
			}
			else{
				if(!robotNavigation(ROOM1, ROOM4, time)){
					robotNavigation(ROOM4, HALL, time);
				}
			}
		}
	}
}

void kalibrasiSensor(){
	lcd_clear();
	initPos = 1500;
	char temp[16];
	pin = 20;
	int flag = 0, oldFlag = 0;
	while(1){
		oldFlag = flag;
		if(oldFlag){
			setServo(pin, initPos, 120, 0);
		}
		if(lsLeft())
		{
			flag = 1;
			initPos--;
			delayMs(70);
		}
		else if(lsRight())
		{
			flag = 1;
			initPos++;
			delayMs(70);
		}
		else{
			flag = 0;
		}
		if(lsFrontLeft())
		{
			pin++;
			initPos = 1500;
			delayMs(300);
		}
		else if(lsFrontRight())
		{
			pin--;
			initPos = 1500;
			delayMs(300);
		}
		if(pin>26)
		{
			pin=26;
		}
		else if(pin<0)
		{
			pin=0;
		}
		sprintf(temp, "%2d %2d", initPos, pin);
		printf("\n%2d", temp);
		lcd_putsf(0, 0, temp);
	}
}

void cobals(uint16_t time)
{
	//		printf("LS left : %d ", lsLeft());
	//		printf("LS Front left : %d ", lsFrontLeft());
	//		printf("LS Front Right : %d ", lsFrontRight());
	//		printf("\nLS Right : %d ", lsRight());
	if(lsFrontLeft() && !lsFrontRight()){
		moveRobot(3, -3, 0, time);  // 3, -3, 0
		moveRobot(3, -3, 0, time);
		moveNormal(time);
	}
	else if(lsFrontRight() && !lsFrontLeft()){
		moveRobot(-3, -3, 0, time); // -3, -3, 0
		moveRobot(-3, -3, 0, time);
		moveNormal(time);
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
	else if(lsRight())
	{
		moveRobot(-3, 0, 0, time);
		moveRobot(-3, 0, 0, time);
		moveNormal(time);
	}
	else if(lsLeft())
	{
		moveRobot(3, 0, 0, time);
		moveRobot(3, 0, 0, time);
		moveNormal(time);
	}
}
