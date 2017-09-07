#include "tombol.h"

static uint8_t fireLedBlink = 0;
static uint8_t blinkStop = 0;

void buttonInit(){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(SOUND_START_LINE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SOUND_ACT_PIN | START_BUTTON_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SOUND_START_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(BUTTON_LINE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BUTTON_1_PIN | BUTTON_2_PIN | BUTTON_3_PIN | BUTTON_4_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BUTTON_PORT, &GPIO_InitStructure);


	RCC_AHB1PeriphClockCmd(FIRE_LED_LINE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = FIRE_LED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(FIRE_LED_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(LS_LINE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LS_FR_PIN | LS_FL_PIN | LS_R_PIN | LS_L_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(LS_PORT, &GPIO_InitStructure);

	timer4InitFreq(2);
}

//uint8_t soundDetected(){
//	uint16_t i, ok = 0;
//	for (i = 0; i < 10000; i++) {
//		if (!GPIO_ReadInputDataBit(SOUND_START_PORT, SOUND_ACT_PIN)) {
//			ok++;;
//		}
//	}
//	if (ok >= 9999)
//		return 1;
//	else
//		return 0;
//}

uint8_t startButtonPressed(){
	return !GPIO_ReadInputDataBit(SOUND_START_PORT, START_BUTTON_PIN);
}

uint8_t button1Pressed(){
	return !GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_1_PIN);
}

uint8_t button2Pressed(){
	return !GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_2_PIN);
}

uint8_t button3Pressed(){
	return !GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_3_PIN);
}

uint8_t button4Pressed(){
	return !GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_4_PIN);
}

uint8_t lsLeft(){
	return GPIO_ReadInputDataBit(LS_PORT, LS_L_PIN);
}

uint8_t lsRight(){
	return GPIO_ReadInputDataBit(LS_PORT, LS_R_PIN);
}

uint8_t lsFrontRight(){
	return GPIO_ReadInputDataBit(LS_PORT, LS_FR_PIN);
}

uint8_t lsFrontLeft(){
	return GPIO_ReadInputDataBit(LS_PORT, LS_FL_PIN);
}

void fireLedToggle() {
	FIRE_LED_PORT -> ODR ^= FIRE_LED_PIN;
}

void fireLedOff() {
	GPIO_SetBits(FIRE_LED_PORT, FIRE_LED_PIN);
}

void fireLedStartBlink() {
	fireLedBlink = 1;
	blinkStop = 0;
	timer4Start();
}

void fireLedStopBlink() {
	fireLedBlink = 0;
}

void tesLimitSwitch(){
	char temp[16];
	while(1){
		lcd_clear();
		lcd_putsf(0, 0, "L FL FR R");
		sprintf(temp, "%2d %2d %2d %2d", lsLeft(), lsFrontLeft(), lsFrontRight(), lsRight());
		lcd_putsf(0, 1, temp);
		printf("\n%2d %2d %2d %2d", lsLeft(), lsFrontLeft(), lsFrontRight(), lsRight());
		delayMs(100);
	}
}

void timer4ISRCallBack() {
	if(fireLedBlink) {
		fireLedToggle();
	}
	else {
		if(blinkStop > 9) {
			blinkStop = 0;
			timer4Stop();
			fireLedOff();
		}
		fireLedToggle();
		blinkStop++;
	}
}

static int16_t mic[1024] = {0};
static int16_t micOutput[1024] = {0};

//uint8_t soundDetected1(uint8_t sample) {
//	uint32_t fftSize = 1024;
//	uint32_t ifftFlag = 0;
//	uint32_t doBitReverse = 1;
//	uint32_t maxValueIndex;
//	int16_t maxValue;
//	uint32_t mid = 0;
//	uint16_t i = 0;
//	uint8_t count = 0, correct = 0;
//
//	for(i = 0; i < 5000; i++) {
//		mid += adc1Read(ADC_MIC);
//	}
//	mid /= 5000;
//	i = 0;
//	while(count < sample) {
//		mic[i] = (adc1Read(ADC_MIC) - mid);
//		if(mic[i] >= 0) {
//			mic[i] = 20000;
//		}
//		else {
//			mic[i] = -20000;
//		}
//		mic[i+1] = 0;
//		i = i + 2;
//		delayUs(3);
//		if(i >= 1024) {
//			i = 0;
//			count++;
//
//			arm_status status;
//			arm_cfft_radix4_instance_q15 S;
//
//			status = ARM_MATH_SUCCESS;
//
//			/* Initialize the CFFT/CIFFT module */
//			status = arm_cfft_radix4_init_q15(&S, fftSize, ifftFlag, doBitReverse);
//
//			/* Process the data through the CFFT/CIFFT module */
//			arm_cfft_radix4_q15(&S, mic);
//
//			/* Process the data through the Complex Magnitude Module for
//			calculating the magnitude at each bin */
//			arm_cmplx_mag_q15(mic, micOutput, fftSize);
//
//			/* Calculates maxValue and returns corresponding BIN value */
//			arm_max_q15(micOutput, fftSize, &maxValue, &maxValueIndex);
//
//			uint8_t indexMin1 = 22, indexMax1 = 24, indexMin2 = 992, indexMax2 = 994, j;
//			if((maxValueIndex >= indexMin1 && maxValueIndex <= indexMax1) || (maxValueIndex >= indexMin2 && maxValueIndex <= indexMax2)) {
//				correct++;
//			}
//			else {
//				uint8_t search = 0;
//				for(j = indexMin1; j <= indexMax1; j++) {
//					if(micOutput[j] > 530) {
//						correct++;
//						search = 1;
//						break;
//					}
//				}
//				if(search = 0) {
//					correct = 0;
//					return 0;
//				}
//			}
//			if(correct >= sample) {
//				return 1;
//			}
//		}
//	}
//	return 0;
//}
//
//void tesSoundAct(uint8_t sample) {
//	uint32_t fftSize = 1024;
//	uint32_t ifftFlag = 0;
//	uint32_t doBitReverse = 1;
//	uint32_t maxValueIndex;
//	int16_t maxValue;
//	uint32_t mid = 0;
//	uint16_t i = 0;
//	uint8_t count = 0, correct = 0;
//
//	for(i = 0; i < 5000; i++) {
//		mid += adc1Read(ADC_MIC);
//	}
//	mid /= 5000;
//	i = 0;
//	sample = sample * 2;
//	while(count < sample) {
//		mic[i] = (adc1Read(ADC_MIC) - mid);
//		if(mic[i] >= 0) {
//			mic[i] = 20000;
//		}
//		else {
//			mic[i] = -20000;
//		}
//		mic[i+1] = 0;
//		i = i + 2;
//		delayUs(3);
//		if(i >= 1024) {
//			char temp[16];
//			sprintf(temp, "%6d", mid);
//			lcd_putsf(0, 0, temp);
//
//			i = 0;
//			count++;
//
//			arm_status status;
//			arm_cfft_radix4_instance_q15 S;
//
//			status = ARM_MATH_SUCCESS;
//
//			/* Initialize the CFFT/CIFFT module */
//			status = arm_cfft_radix4_init_q15(&S, fftSize, ifftFlag, doBitReverse);
//
//			/* Process the data through the CFFT/CIFFT module */
//			arm_cfft_radix4_q15(&S, mic);
//
//			/* Process the data through the Complex Magnitude Module for
//			calculating the magnitude at each bin */
//			arm_cmplx_mag_q15(mic, micOutput, fftSize);
//
//			/* Calculates maxValue and returns corresponding BIN value */
//			arm_max_q15(micOutput, fftSize, &maxValue, &maxValueIndex);
//
//			sprintf(temp, "%6d", maxValueIndex);
//			lcd_putsf(0, 1, temp);
//
//			uint8_t indexMin1 = 22, indexMax1 = 24, indexMin2 = 992, indexMax2 = 994, j;
//			if((maxValueIndex >= indexMin1 && maxValueIndex <= indexMax1) || (maxValueIndex >= indexMin2 && maxValueIndex <= indexMax2)) {
//				correct++;
//			}
//			else {
//				uint8_t search = 0;
//				for(j = indexMin1; j <= indexMax1; j++) {
//					if(micOutput[j] > 530) {
//						correct++;
//						search = 1;
//						break;
//					}
//				}
//				if(search = 0) {
//					correct = 0;
//					lcd_putsf(12, 1, "NO");
//					return;
//				}
//			}
//			if(correct >= sample) {
//				lcd_putsf(12, 1, "OK");
//				return;
//			}
//		}
//	}
//	lcd_putsf(12, 1, "NO");
//	return;
//}
