//--------------------------------------------------------------
// File     : adc1.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_ADC1_SINGLE_H
#define __STM32F4_ADC1_SINGLE_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"


//--------------------------------------------------------------
// Liste aller ADC-Kanäle
// (keine Nummer doppelt und von 0 beginnend)
//--------------------------------------------------------------
typedef enum {
  ADC_PC0 = 0,
  ADC_PC1 = 1,
  ADC_PC2 = 2,
  ADC_PC3 = 3,
  ADC_PC4 = 4,
  ADC_PC5 = 5,
  ADC_PA1 = 6,
  ADC_PA2 = 7
} ADC1s_NAME_t;

#define  ADC1s_ANZ   8 // Anzahl von ADC1s_NAME_t


//--------------------------------------------------------------
// Anzahl der Mittelwerte
//--------------------------------------------------------------
typedef enum {
  MW_NONE =0,  // keine Mittelwerte
  MW_2,        // 2 Mittelwerte
  MW_4,        // 4 Mittelwerte
  MW_8,        // 8 Mittelwerte
  MW_16,       // 16 Mittelwerte
  MW_32,       // 32 Mittelwerte
  MW_64,       // 64 Mittelwerte
  MW_128       // 128 Mittelwerte
}ADC1s_MW_t;



//--------------------------------------------------------------
// ADC-Clock
// Max-ADC-Frq = 36MHz
// Grundfrequenz = APB2 (APB2=84MHz)
// Moegliche Vorteiler = 2,4,6,8
//--------------------------------------------------------------

//#define ADC1s_VORTEILER     ADC_Prescaler_Div2 // Frq = 42 MHz
#define ADC1s_VORTEILER     ADC_Prescaler_Div4   // Frq = 21 MHz
//#define ADC1s_VORTEILER     ADC_Prescaler_Div6 // Frq = 14 MHz
//#define ADC1s_VORTEILER     ADC_Prescaler_Div8 // Frq = 10.5 MHz



//--------------------------------------------------------------
// Struktur eines ADC Kanals
//--------------------------------------------------------------
typedef struct {
  ADC1s_NAME_t ADC_NAME;  // Name
  GPIO_TypeDef* ADC_PORT; // Port
  const uint16_t ADC_PIN; // Pin
  const uint32_t ADC_CLK; // Clock
  const uint8_t ADC_CH;   // ADC-Kanal
  ADC1s_MW_t ADC_MW;      // Mittelwerte
}ADC1s_t;



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void adc1Init(void);
uint16_t adc1Read(ADC1s_NAME_t adc_name);
uint16_t adc1ReadMean(ADC1s_NAME_t adc_name);


//--------------------------------------------------------------
#endif // __STM32F4_ADC1_SINGLE_H
