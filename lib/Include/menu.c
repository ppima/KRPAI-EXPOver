#include "menu.h"
#define MENU_LIMIT 35
char menu[MENU_LIMIT][16] = {"MAIN", "SONAR", "ADC", "COLOR", "TPA",
							"UVTRON", "PROXIMITY", "WF KIRI", "WF KANAN", "RATA KIRI",
							"RATA KANAN", "GESER KIRI", "GESER KANAN", "JALAN", "APPR FIRE", "SEARCH FIRE",
							"KILL FIRE", "TES KIPAS", "TES SOUNDACT", "FIRE LED", "WHT THRES",
							"BLCK THRES", "CEK THRES", "PTRKN90", "PTRKR90", "PTR BALIK",
							"CHECK ROOMIN", "CHECK ROOMOUT", "ROOM START", "KANAN MUNDUR", "KIRI MUNDUR",
							"APPR KILL", "TEZT", "KANAN ROOM", "ROBOT NAV"};

void menuLCD(void){
	uint8_t i = 0, j = 1, option = 0, iPrev = 1;
	uint8_t ok;
	char lcdNow[20], lcdNext[20];
	tpaSetServo(26);
	uint16_t time = 120;

	while(1) {
		ok = 0;
		if(startButtonPressed()) {
			if(startButtonPressed()) {
//				tes(time);
//				navigasiAllArbitrary(time);
//				while(1);
//			}
//			else if(soundDetected1(10)) {
//				if(soundDetected1(10)) {
//					navigasiAllArbitrary(time);
//					while(1);
//				}
			}
		}
		else if(lsLeft()){
			delayMs(200);
			if(i > 0) {
				i--;
				j = i + 1;
			}
			else {
				i = MENU_LIMIT - 1;
				j = 0;
			}
		}
		if(lsRight()){
			delayMs(200);
			if(i < MENU_LIMIT - 2) {
				i++;
				j = i + 1;
			}
			else if(i == MENU_LIMIT - 1) {
				i = 0;
				j = 1;
			}
			else {
				i = MENU_LIMIT - 1;
				j = 0;
			}
		}
		else if(lsFrontRight()){
			ok = 1;
		}
		sprintf(lcdNow,">%2d %s", i, menu[i]);
		sprintf(lcdNext,"%2d %s", j, menu[j]);
		if(iPrev != i) {
			lcd_clear();
			lcd_putsf(0,0, lcdNow);
			lcd_putsf(0,1, lcdNext);
			iPrev = i;
		}
		option = i;
		if(ok) {
			delayMs(200);
			iPrev = i - 1;
			switch(option){
				case 0:
//					navigasiAll(time);
//					while(1);
					break;
				case 1:
					tesSonar();
					break;
				case 2:
					tesAdc();
					break;
				case 3:
					tesColorSensor();
					break;
				case 4:
					while(!lsFrontLeft()) {
//						nungging(2, 100);
						tpaValue tpaValues = tpaScanGetPosition();
//						normal(100);
						char temp[16];
						sprintf(temp, "Val=%3d Pos=%2d", tpaValues.tpaTemp, tpaValues.tpaPos);
						lcd_clear();
						lcd_putsf(0, 0, temp);
						while(!button4Pressed()) {
							if(button3Pressed()) break;
						}
					}
					while(lsFrontLeft());
					tpaSetServo(26);
					break;
				case 5:
					lcd_clear();
					while(!lsFrontLeft()) {
						char temp[16];
						uint8_t photon = uvtronCheck(1000);
						sprintf(temp, "PHOTON = %3d", photon);
						lcd_putsf(0, 1, temp);
					}
					while(lsFrontLeft());
					break;
				case 6:
					while(!lsFrontLeft()) {
						char temp[16];
						uint8_t proximity = proximityCheck();
						uint8_t doll = dollCheck();
						sprintf(temp, "PROX=%1d  DOLL=%1d", proximity, doll);
						lcd_putsf(0, 0, temp);
						delayMs(25);
					}
					while(lsFrontLeft());
					break;
				case 7:
					kiri(time, LINE);
					break;
				case 8:
					kanan(time, LINE);
					break;
				case 9:
					rataKiri(time);
					break;
				case 10:
					rataKanan(time);
					break;
				case 11:
					geserKiri(time);
					break;
				case 12:
					geserKanan(time);
					break;
				case 13:
					while(!lsFrontLeft()) {
						tpaSetServo(26);
						moveRobot(0, 1, 0, 150);
					}
					while(lsFrontLeft());
					break;
				case 14:
					approachFire(50);
					break;
				case 15:
					searchFire();
					break;
				case 16:
					killFire();
					break;
				case 17:
					kipasStart();
					while(button4Pressed());
					kipasStop();
					break;
				case 18:
					lcd_clear();
					while(!lsFrontLeft()) {
//						tesSoundAct(10);
					}
					while(lsFrontLeft());
					lcd_clear();
					break;
				case 19:
					fireLedStartBlink();
					lcd_clear();
					lcd_putsf(0, 0, "BLINK");
					while(!lsFrontLeft());
					fireLedStopBlink();
					break;
				case 20:
					getWhiteThreshold();
					break;
				case 21:
					getBlackThreshold();
					break;
				case 22:
					checkThreshold();
					break;
				case 23:
					putarKanan90(time);
					break;
				case 24:
					putarKiri90(time);
					break;
				case 25:
					putarBalik(time);
					break;
				case 26:
					while(!lsFrontLeft()) {
						checkRoomIn();
						while(!button4Pressed() && !lsFrontLeft());
					}
					while(lsFrontLeft());
					break;
				case 27:
					while(!lsFrontLeft()) {
						checkRoomOut();
						while(!button4Pressed() && !lsFrontLeft());
					}
					while(lsFrontLeft());
					break;
				case 28:
					roomStart(time);
					checkRoomIn();
					break;
				case 29:
					kananMundur(time, LINE);
					break;
				case 30:
					kiriMundur(time, LINE);
					break;
				case 31:
					approachKillFire(150);
					break;
				case 32:
					tes(time);
					break;
				case 33:
					kananRoom(150, LINE);
					break;
				case 34:
					robotNav(time);
					break;
				default : break;
			}
			iPrev = 0;
		}
	}
}


void testing() {
	while(!button4Pressed()){
		moveRobot(0, 4, 0, time);
	}
}

void robotNav(uint16_t time){
	char menu1[15][16] = {"ROOM START", "ROOM4START", "ROOM1to2", "ROOM1to3", "ROOM1to4 UNC",
						  "ROOM1to4 C", "ROOM2to1", "ROOM2to3", "ROOM2to4", "ROOM3to1",
						  "ROOM3to2", "ROOM13o4", "ROOM4to1", "ROOM4to2", "ROOM4to3"};
	char lcd_now[20], lcd_next[20];
	int a = 0, b = 1 , pilih = 0;
	int aPrev;
	while(!lsFrontLeft()){
		pilih = 0;
		if(lsLeft()){
			delayMs(50);
			if(a > 0) {
				a--;
				b = a + 1;
			}
			else {
				a = 15 - 1;
				b = 0;
			}
		}
		else if(lsRight()){
			delayMs(50);
			if(a < 15 - 2) {
				a++;
				b = a + 1;
			}
			else if(a == 15 - 1) {
				a = 0;
				b = 1;
			}
			else {
				a = 0;
				b = 1;
			}
		}
		else if(lsFrontRight()){
			pilih = 1;
		}

		sprintf(lcd_now,">%2d %s", a, menu1[a]);
		sprintf(lcd_next,"%2d %s", b, menu1[b]);
		if(aPrev != a) {
			lcd_clear();
			lcd_putsf(0,0, lcd_now);
			lcd_putsf(0,1, lcd_next);
			aPrev = a;
		}
		delayMs(50);
		if(pilih){
			delayMs(200);
			switch (a){
				case 0 : roomStart(time); break;
				case 1 : room4Start(time); break;
				case 2 : room1toRoom2(time); break;
				case 3 : room1toRoom3(time); break;
				case 4 : room1toRoom4Unchecked(time); break;
				case 5 : room1toRoom4Checked(time); break;
				case 6 : room2toRoom1(time); break;
				case 7 : room2toRoom3(time); break;
				case 8 : room2toRoom4(time); break;
				case 9 : room3toRoom1(time); break;
				case 10 : room3toRoom2(time); break;
				case 11 : room3toRoom4(time); break;
				case 12 : room4toRoom1(time); break;
				case 13 : room4toRoom2(time); break;
				case 14 : room4toRoom3(time); break;
			}
			aPrev = 0;
		}
	}
}
