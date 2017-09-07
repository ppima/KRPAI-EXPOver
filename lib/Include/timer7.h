//--------------------------------------------------------------
// File     : timer7.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_TIM7_H
#define __STM32F4_TIM7_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "srf_tpa.h"

//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void timer7Init(uint16_t prescaler, uint16_t periode);
void timer7InitFreq(uint32_t frq_hz);
void timer7Start(uint16_t tpa81ValStop);
void timer7Stop(void);
void tpa81ResetValue(uint16_t tpa81ValStop);
void timer7ISRCallBack(void);  // Diese Funktion muss extern benutzt werden !!

//--------------------------------------------------------------
#endif // __STM32F4_TIM7_H
