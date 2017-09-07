//--------------------------------------------------------------
// File     : exti0.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_EXT_INT0_H
#define __STM32F4_UB_EXT_INT0_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "jalan_dasar.h"
#include "srf_tpa.h"
#include "lcd.h"
#include "wall_following_kanan.h"
#include "wall_following_kiri.h"
#include "stdio.h"

extern uint8_t photon;

//--------------------------------------------------------------
// Struktur eines EXT_INT
//--------------------------------------------------------------
typedef struct {
  GPIO_TypeDef* INT_PORT; // Port
  const uint16_t INT_PIN; // Pin
  const uint32_t INT_CLK; // Clock
  const uint8_t INT_SOURCE; // Source
  GPIOPuPd_TypeDef INT_R; // Widerstand
  EXTITrigger_TypeDef INT_TRG; // Trigger
}EXT_INT0_t;


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void exti0Init(void);
uint8_t uvtronCheck(uint16_t delayms);
uint8_t uvtronChecking(uint16_t time, uint8_t totalMove);

//--------------------------------------------------------------
#endif // __STM32F4_UB_EXT_INT0_H
