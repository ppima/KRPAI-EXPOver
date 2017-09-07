#ifndef __TOMBOL_H
#define __TOMBOL_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "adc1.h"
//#include "arm_math.h"
#include "timer4.h"

#define SAMPLE_LENGTH 8192

#define SOUND_START_LINE RCC_AHB1Periph_GPIOC
#define SOUND_START_PORT GPIOA
#define SOUND_ACT_PIN GPIO_Pin_10
#define START_BUTTON_PIN GPIO_Pin_8
#define ADC_MIC ADC_PA2

#define BUTTON_LINE RCC_AHB1Periph_GPIOB
#define BUTTON_PORT GPIOE
#define BUTTON_1_PIN GPIO_Pin_4
#define BUTTON_2_PIN GPIO_Pin_3
#define BUTTON_3_PIN GPIO_Pin_2
#define BUTTON_4_PIN GPIO_Pin_1

#define FIRE_LED_LINE RCC_AHB1Periph_GPIOC
#define FIRE_LED_PORT GPIOC
#define FIRE_LED_PIN GPIO_Pin_7

#define LS_LINE RCC_AHB1Periph_GPIOB
#define LS_PORT GPIOB
#define LS_L_PIN GPIO_Pin_2
#define LS_FL_PIN GPIO_Pin_3
#define LS_FR_PIN GPIO_Pin_4
#define LS_R_PIN GPIO_Pin_5

void buttonInit();
//uint8_t soundDetected();
uint8_t startButtonPressed();
uint8_t button1Pressed();
uint8_t button2Pressed();
uint8_t button3Pressed();
uint8_t button4Pressed();
uint8_t lsFrontRight();
uint8_t lsFrontLeft();
void fireLedStartBlink();
void fireLedStopBlink();
void tesLimitSwitch();
//uint8_t soundDetected1(uint8_t sample);
//void tesSoundAct(uint8_t sample);

#endif
