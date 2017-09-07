#include "delay.h"

static __IO uint32_t timingDelay = 0;
static __IO uint32_t time = 0;
uint8_t delayInitialized = 0;

void delayUs(__IO uint32_t nTime) {
	timingDelay = nTime;

	while (timingDelay != 0);
}

void delayMs(__IO uint32_t nTime) {
	timingDelay = 1000 * nTime;

	while (timingDelay != 0);
}

void SysTick_Handler(void) {
	time++;
	if (time2 != 0x00) {
		time2--;
	}
	if (timingDelay != 0x00) {
		timingDelay--;
	}
}

void delayInit(void) {
	if (delayInitialized) {
		return;
	}
	RCC_HSEConfig(RCC_HSE_ON);
	while (!RCC_WaitForHSEStartUp());
	/* Set Systick interrupt every 1us */
	if (SysTick_Config(SystemCoreClock / 1000000)) {
		/* Capture error */
		while (1);
	}
	delayInitialized = 1;
}

uint32_t delayTime(void) {
	return time;
}

uint32_t delayTime2(void) {
	return time2;
}

void delaySetTime(uint32_t time) {
	time = time;
}

void delaySetTime2(uint32_t time) {
	time2 = time;
}

void delayEnableSystick(void) {
	if (!delayInitialized) {
		return;
	}
	
	/* Enable systick interrupts, useful when you wakeup from sleep mode */  
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void delayDisableSystick(void) {
	/* Disable systick, useful when you go to sleep mode */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

