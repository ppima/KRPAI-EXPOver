#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_gpio.h"

#include "color_sensor.h"
#include "delay.h"
#include "i2c2.h"
#include "jalan_dasar.h"
#include "navigasi.h"
#include "uart.h"
#include "srf_tpa.h"
#include "timer7.h"
#include "timer4.h"
#include "tombol.h"
#include "kipas.h"

void trial(void);
void navigasiAll(uint16_t time);
void initAll(void);
void searchFireWater(uint16_t minVal);
void extFire(void);
void cobals(uint16_t time);
