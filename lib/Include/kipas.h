#ifndef __LIB_KIPAS_H
#define __LIB_KIPAS_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define KIPAS_CLK_LINE RCC_AHB1Periph_GPIOE
#define KIPAS_PORT GPIOE
#define KIPAS_PIN GPIO_Pin_8

void kipasInit();
void kipasStart();
void kipasStop();

#endif
