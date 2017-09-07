//--------------------------------------------------------------
// File     : timer2.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_TIM2_H
#define __STM32F4_TIM2_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

//extern uint8_t srfFront = 0, srfRDg = 0, srfR1 = 0, srfR2 = 0, srfBack = 0, srfL2 = 0, srfL1 = 0, srfLDg = 0;

//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void timer2Init(uint16_t prescaler, uint16_t periode);
void timer2InitFreq(float frq_hz);
void timer2Start(void);
void timer2Stop(void);
void timer2ISRCallBack(void);  // Diese Funktion muss extern benutzt werden !!


//--------------------------------------------------------------
#endif // __STM32F4_TIM2_H
