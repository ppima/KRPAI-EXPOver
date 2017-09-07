//--------------------------------------------------------------
// File     : exti0.c
// Datum    : 01.04.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, EXTI, SYSCFG, MISC
// Funktion : External Interrupt0
//
// Hinweis  : mögliche Pinbelegungen
//            EXT_INT0 :
//               [PA0, PB0, PC0, PD0, PE0, PF0, PG0, PH0, PI0]
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "exti0.h"
// ab hier eigene includes



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_EXT_INT0_LoFlanke(void);
void P_EXT_INT0_HiFlanke(void);

uint8_t photon = 0;
//--------------------------------------------------------------
// Definition vom EXT_INT0 Pin
//
// Widerstand : [GPIO_PuPd_NOPULL,GPIO_PuPd_UP,GPIO_PuPd_DOWN]
// Trigger    : [EXTI_Trigger_Rising,EXTI_Trigger_Falling,EXTI_Trigger_Rising_Falling]
//--------------------------------------------------------------
EXT_INT0_t EXT_INT0 = {
  // PORT , PIN       , CLOCK          ,Source,             ,Widerstand      , Trigger
  GPIOD,GPIO_Pin_0,RCC_AHB1Periph_GPIOD,EXTI_PortSourceGPIOD,GPIO_PuPd_NOPULL,EXTI_Trigger_Rising_Falling   // EXT_Int-Pin = PA0
};



//--------------------------------------------------------------
// Init vom external Interrupt 0
//--------------------------------------------------------------
void exti0Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  // Clock enable (GPIO)
  RCC_AHB1PeriphClockCmd(EXT_INT0.INT_CLK, ENABLE);

  // Config als Digital-Eingang
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = EXT_INT0.INT_R;
  GPIO_InitStructure.GPIO_Pin = EXT_INT0.INT_PIN;
  GPIO_Init(EXT_INT0.INT_PORT, &GPIO_InitStructure);

  // Clock enable (SYSCONFIG)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // EXT_INT0 mit Pin verbinden
  SYSCFG_EXTILineConfig(EXT_INT0.INT_SOURCE, EXTI_PinSource0);

  // EXT_INT0 config
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXT_INT0.INT_TRG;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  // NVIC config
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//--------------------------------------------------------------
// diese Funktion wird aufgerufen,
// bei einer Lo-Flanke am EXT-INT0
// wenn [EXTI_Trigger_Falling oder EXTI_Trigger_Rising_Falling]
//--------------------------------------------------------------
void P_EXT_INT0_LoFlanke(void)
{
  // hier eigenen Code eintragen

}


//--------------------------------------------------------------
// diese Funktion wird aufgerufen,
// bei einer Hi-Flanke am EXT-INT0
// wenn [EXTI_Trigger_Rising oder EXTI_Trigger_Rising_Falling]
//--------------------------------------------------------------
void P_EXT_INT0_HiFlanke(void)
{
  // hier eigenen Code eintragen
	photon++;
}

uint8_t uvtronCheck(uint16_t delayms) {
	photon = 0;
	normal(delayms);
	delayMs(delayms);
	return photon;
}

uint8_t uvtronChecking(uint16_t time, uint8_t totalMove) {
	photon = 0;
	lcd_clear();
	char temp[16];
	const uint8_t ping[3] = {0xE4, 0xEC};
	uint8_t left, right, i;

	if(totalMove > 0) {
		for(i = 0; i < totalMove; i++) {
			moveRobot(0, 4, 0, time);
		}
	}

	if(photon == 0){
		for(i = 0; i < 2; i++) {
			sonarRange(ping[i]);
			delayMs(10);
		}
		right = sonarGet(ping[0]);
		left = sonarGet(ping[1]);

		if(left > right){
			geserKanan(time);
			putarKiri90(time);
		}
		else{
			geserKiri(time);
			putarKanan90(time);
		}
		if(photon != 0){
			if(left > right){
				putarKanan90(time);
			}
			else{
				putarKiri90(time);
			}
		}
	}
	else {
		for(i = 0; i < 2; i++) {
			sonarRange(ping[i]);
			delayMs(10);
		}
		right = sonarGet(ping[0]);
		left = sonarGet(ping[1]);

		if(left > right){
			geserKanan(time);
		}
		else{
			geserKiri(time);
		}
	}

	sprintf(temp, "PHOTON = %d", photon);
	lcd_putsf(0,0, temp);
	return photon;
}

//--------------------------------------------------------------
// external Interrupt-0
// wird je nach einstellung der Triggerflanke
// entweder bei Hi- oder Lo- oder beiden Flanken ausgelöst
//--------------------------------------------------------------
void EXTI0_IRQHandler(void)
{
  uint32_t wert;

  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    // wenn Interrupt aufgetreten
    EXTI_ClearITPendingBit(EXTI_Line0);

    wert=GPIO_ReadInputDataBit(EXT_INT0.INT_PORT, EXT_INT0.INT_PIN);
    if(wert==Bit_RESET) {
      P_EXT_INT0_LoFlanke();
    }
    else {
      P_EXT_INT0_HiFlanke();
    }
  }
}

