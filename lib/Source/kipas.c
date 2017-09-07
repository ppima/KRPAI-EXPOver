#include "kipas.h"

void kipasInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(KIPAS_CLK_LINE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KIPAS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KIPAS_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(KIPAS_PORT, KIPAS_PIN);
}

void kipasStart() {
	GPIO_SetBits(KIPAS_PORT, KIPAS_PIN);
}

void kipasStop() {
	GPIO_ResetBits(KIPAS_PORT, KIPAS_PIN);
}
