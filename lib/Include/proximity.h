#ifndef __LIB_PROXIMITY_H
#define __LIB_PROXIMITY_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "delay.h"

#define PROXIMITY_CLK_LINE RCC_AHB1Periph_GPIOE
#define PROXIMITY_PORT GPIOE
#define PROXIMITY_PIN GPIO_Pin_6

void proximityInit();
uint8_t proximityCheck();
uint8_t dollCheck();

#endif
