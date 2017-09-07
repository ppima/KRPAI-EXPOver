#ifndef __NAVIGASI_H
#define __NAVIGASI_H

#include "wall_following_kanan.h"
#include "wall_following_kiri.h"
#include "color_sensor.h"
#include "proximity.h"
#include "lcd.h"
#include "jalan_dasar.h"
#include "exti0.h"
#include "stdio.h"
#include "tombol.h"
#include "kipas.h"

typedef enum {
	NOROOM,
	ROOM1,
	ROOM2,
	ROOM3,
	ROOM4,
	HALL,
} roomType;

typedef enum {
	UNCHECKED,
	UNCHECKED_INV,
	CHECKED,
	ON_FIRE,
} roomStatus;

/*
 *   pos1__
 *  |         | pos2
 *  |         |
 *  |         |
 *  |_________|
 *   pos3
 */

/*	Model room 4
 *  	  ____ 1
 *  HALL |		 |
 *  	 |		 |
 *  	 |		 |
 *   	  2  ____|
 */

/*  Model room 1
 * 	  __________
 * 	 |			Type 2
 * 	 |			|	DOOR A
 * 	 |DOOR B____Type 1
 */
typedef enum {
	UNKNOWN,
	POS1,
	POS2,
	POS3
} dollPosition;

typedef enum{
	OFF,
	ON,
} status;

typedef enum{
	MODEL1,
	MODEL2,
} room4Type;

typedef enum{
	NOTYPE,
	TYPE1,
	TYPE2,
} room1Type;

typedef enum{
	DOORA,
	DOORB,
} room1DoorType;


status robotNavigation(roomType source, roomType dest, uint16_t time);
status hallDollCheck(uint16_t time);
roomType checkRoomIn();
roomType checkRoomOut();
uint8_t checkFire();
uint8_t searchFire();
void approachFire(uint16_t tpa81Val);
void approachKillFire(uint16_t tpa81Val);
void killFire();
void killFireWater();
uint8_t checkFire();
uint8_t checkingFire(uint16_t time, uint8_t totalMove);
roomType roomStart(uint16_t time);
void navigasiAllArbitrary(uint16_t time);
void tes(uint16_t time);

#endif

