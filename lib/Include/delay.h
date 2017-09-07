#ifndef __STM32F4_DELAY_H
#define __STM32F4_DELAY_H

/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"

static __IO uint32_t timingDelay;

/**
 * This variable can be used in main
 * It is automatically increased every time systick make an interrupt
 */
static __IO uint32_t time;
static __IO uint32_t time2;

/**
 *
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in microseconds.
  * @retval None
  */
void delayUs(__IO uint32_t nTime);

/**
 *
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
void delayMs(__IO uint32_t nTime);

/**
 *
 *
 * Initialize timer settings for delay
 */
void delayInit(void);

/**
 * Get the time variable value
 */
uint32_t delayTime(void);

/**
 * Set value for time variable
 */
void delaySetTime(uint32_t time);

/**
 * Get the time variable value
 */
uint32_t delayTime2(void);

/**
 * Set value for time variable
 */
void delaySetTime2(uint32_t time);

/**
 * Re-enable Systick. It has to be configured before with TM_DELAY_Init();
 */
void delayEnableSystick(void);

/**
 * Disable Systick. Usef when going into sleep mode, so systick will not
 */
void delayDisableSystick(void);

#endif
