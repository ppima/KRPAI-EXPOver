#ifndef __WALL_FOLLOWING_KANAN_H
#define __WALL_FOLLOWING_KANAN_H

#include "srf_tpa.h"
#include "jalan_dasar.h"
#include "delay.h"
#include "color_sensor.h"
#include <math.h>
#include "lcd.h"
#include "timer5.h"
#include "proximity.h"
#include "wall_following_kiri.h"

extern const uint8_t pingKanan[5];

void kanan(uint16_t time, colorStatus color);
void kananRoom(uint16_t time, colorStatus color);
uint8_t kananProx(uint16_t time, colorStatus color);
void kananMundur(uint16_t time, colorStatus color);
void rataKanan(uint16_t time);
void putarBalik(uint16_t time);
void geserKanan(uint16_t time);
void geserKananRoom(uint16_t time);
void putarKanan90(uint16_t time);

#endif
