#include "proximity.h"

void proximityInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(PROXIMITY_CLK_LINE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PROXIMITY_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PROXIMITY_PORT, &GPIO_InitStructure);
}

uint8_t proximityCheck() {
	return !GPIO_ReadInputDataBit(PROXIMITY_PORT, PROXIMITY_PIN);
}

uint8_t dollCheck(){
	delayMs(10);
	lcd_clear();
	uint8_t i, correct = 0;

	for(i = 0; i < 10; i++){
		if(proximityCheck() == 1){
			correct++;
		}
	}
	if(correct >= 8){
		lcd_putsf(0,1, "ADA BONEKA");
		return 1;
	}
	else{
		lcd_putsf(0,1, "TIDAK ADA BONEKA");
		return 0;
	}
}
