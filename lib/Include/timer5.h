//--------------------------------------------------------------
// File     : timer5.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_TIM5_H
#define __STM32F4_TIM5_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "color_sensor.h"

extern colorStatus colorTarget;
extern uint8_t colorMatch;

//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void timer5Init(uint16_t prescaler, uint16_t periode);
void timer5InitFreq(uint32_t frq_hz);
void timer5Start(colorStatus color);
void timer5Stop(void);
void timer5ISRCallBack(void);  // Diese Funktion muss extern benutzt werden !!


//--------------------------------------------------------------
#endif // __STM32F4_TIM5_H
