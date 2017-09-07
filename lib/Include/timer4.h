//--------------------------------------------------------------
// File     : timer4.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_TIM4_H
#define __STM32F4_TIM4_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void timer4Init(uint16_t prescaler, uint16_t periode);
void timer4InitFreq(uint32_t frq_hz);
void timer4Start(void);
void timer4Stop(void);
void timer4ISRCallBack(void);  // Diese Funktion muss extern benutzt werden !!


//--------------------------------------------------------------
#endif // __STM32F4_TIM4_H
