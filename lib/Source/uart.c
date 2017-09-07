//--------------------------------------------------------------
// File     : uart.c
// Datum    : 28.07.2015
// Version  : 1.5
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.8
// GCC      : 4.9 2015q2
// Module   : GPIO, USART, MISC
// Funktion : UART (RS232) In und OUT
//            Receive wird per Interrupt behandelt
//
// Hinweis  : moegliche Pinbelegungen
//            UART1 : TX:[PA9,PB6] RX:[PA10,PB7]
//            UART2 : TX:[PA2,PD5] RX:[PA3,PD6]
//            UART3 : TX:[PB10,PC10,PD8] RX:[PB11,PC11,PD9]
//            UART4 : TX:[PA0,PC10] RX:[PA1,PC11]
//            UART5 : TX:[PC12] RX:[PD2]
//            UART6 : TX:[PC6,PG14] RX:[PC7,PG9]
//
// Vorsicht : Als Endekennung beim Empfangen, muss der Sender
//            das Zeichen "0x0D" = Carriage-Return
//            an den String anhaengen !!
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "uart.h"

//--------------------------------------------------------------
// Definition aller UARTs
// Reihenfolge wie bei UART_NAME_t
//--------------------------------------------------------------


UART_t UART[] = {
// Name, Clock               , AF-UART      , UART , Baud , Interrupt
  {COM3,RCC_APB1Periph_USART3,GPIO_AF_USART3,USART3,115200,USART3_IRQn, // UART3 mit 115200 Baud
// PORT , PIN      , Clock              , Source
  {GPIOD,GPIO_Pin_8,RCC_AHB1Periph_GPIOD,GPIO_PinSource8},  // TX an PD8
  {GPIOD,GPIO_Pin_9,RCC_AHB1Periph_GPIOD,GPIO_PinSource9}}, // RX an PD9
  // Name, Clock               , AF-UART      , UART , Baud , Interrupt
    {COM2,RCC_APB2Periph_USART1,GPIO_AF_USART1,USART1,115200,USART1_IRQn, // UART1 mit 115200 Baud
  // PORT , PIN      , Clock              , Source
    {GPIOA,GPIO_Pin_9,RCC_AHB1Periph_GPIOA,GPIO_PinSource9},  // TX an PA9
    {GPIOA,GPIO_Pin_10,RCC_AHB1Periph_GPIOA,GPIO_PinSource10}} // RX an PA10
};

//--------------------------------------------------------------
// init aller UARTs
//--------------------------------------------------------------
void uartInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  UART_NAME_t nr;

  for(nr=0;nr<UART_ANZ;nr++) {

    // Clock enable der TX und RX Pins
    RCC_AHB1PeriphClockCmd(UART[nr].TX.CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(UART[nr].RX.CLK, ENABLE);

    // Clock enable der UART
    if((UART[nr].UART==USART1) || (UART[nr].UART==USART6)) {
      RCC_APB2PeriphClockCmd(UART[nr].CLK, ENABLE);
    }
    else {
      RCC_APB1PeriphClockCmd(UART[nr].CLK, ENABLE);
    }

    // UART Alternative-Funktions mit den IO-Pins verbinden
    GPIO_PinAFConfig(UART[nr].TX.PORT,UART[nr].TX.SOURCE,UART[nr].AF);
    GPIO_PinAFConfig(UART[nr].RX.PORT,UART[nr].RX.SOURCE,UART[nr].AF);

    // UART als Alternative-Funktion mit PushPull
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    // TX-Pin
    GPIO_InitStructure.GPIO_Pin = UART[nr].TX.PIN;
    GPIO_Init(UART[nr].TX.PORT, &GPIO_InitStructure);
    // RX-Pin
    GPIO_InitStructure.GPIO_Pin =  UART[nr].RX.PIN;
    GPIO_Init(UART[nr].RX.PORT, &GPIO_InitStructure);

    // Oversampling
    USART_OverSampling8Cmd(UART[nr].UART, ENABLE);

    // init mit Baudrate, 8Databits, 1Stopbit, keine Paritaet, kein RTS+CTS
    USART_InitStructure.USART_BaudRate = UART[nr].BAUD;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART[nr].UART, &USART_InitStructure);

    // UART enable
    USART_Cmd(UART[nr].UART, ENABLE);

    // RX-Interrupt enable
    USART_ITConfig(UART[nr].UART, USART_IT_RXNE, ENABLE);

    // enable UART Interrupt-Vector
    NVIC_InitStructure.NVIC_IRQChannel = UART[nr].INT;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // RX-Puffer vorbereiten
    UART_RX[nr].rx_buffer[0]=RX_END_CHR;
    UART_RX[nr].wr_ptr=0;
    UART_RX[nr].rd_ptr=0;
    UART_RX[nr].status=RX_EMPTY;
  }
}

//--------------------------------------------------------------
// ein Byte per UART senden
//--------------------------------------------------------------
void uartSendByte(UART_NAME_t uart, uint16_t wert)
{
  // warten bis altes Byte gesendet wurde
  while (USART_GetFlagStatus(UART[uart].UART, USART_FLAG_TXE) == RESET);
  USART_SendData(UART[uart].UART, wert);
}

//--------------------------------------------------------------
// einen String per UART senden
//--------------------------------------------------------------
void uartSendString(UART_NAME_t uart, char *ptr, UART_LASTBYTE_t end_cmd)
{
  // sende kompletten String
  while (*ptr != 0) {
    uartSendByte(uart,*ptr);
    ptr++;
  }
  // eventuell Endekennung senden
  if(end_cmd==LFCR) {
    uartSendByte(uart,0x0A); // LineFeed senden
    uartSendByte(uart,0x0D); // CariageReturn senden
  }
  else if(end_cmd==CRLF) {
    uartSendByte(uart,0x0D); // CariageReturn senden
    uartSendByte(uart,0x0A); // LineFeed senden
  }
  else if(end_cmd==LF) {
    uartSendByte(uart,0x0A); // LineFeed senden
  }
  else if(end_cmd==CR) {
    uartSendByte(uart,0x0D); // CariageReturn senden
  }
}

//--------------------------------------------------------------
// einen String per UART empfangen
// (der Empfang wird per Interrupt abgehandelt)
// diese Funktion muss zyklisch gepollt werden
// Return Wert :
//  -> wenn nichts empfangen = RX_EMPTY
//  -> wenn String empfangen = RX_READY -> String steht in *ptr
//  -> wenn Puffer voll      = RX_FULL
//--------------------------------------------------------------
UART_RXSTATUS_t uartReceiveString(UART_NAME_t uart, char *ptr)
{
  UART_RXSTATUS_t ret_wert=RX_EMPTY;
  uint16_t rd_pos,wr_pos,n;
  uint8_t wert,ok;

  // aktuelle Pointer auslesen
  rd_pos=UART_RX[uart].rd_ptr;
  wr_pos=UART_RX[uart].wr_ptr;
  // check ob daten im puffer
  ok=0;
  if(rd_pos!=wr_pos) {
    // Endekennung suchen
    while(rd_pos!=wr_pos) {
      wert=UART_RX[uart].rx_buffer[rd_pos];
      if(wert==RX_END_CHR) {
        // Endekennung gefunden
        ok=1;
        break;
      }
      // lese pointer weiterschieben
      rd_pos++;
      if(rd_pos>=RX_BUF_SIZE) rd_pos=0;
    }

    // falls Endekennung gefunden, String auslesen
    if(ok==1) {
      // endekennung gefunden
      ret_wert=RX_READY;
      // string komplett auslesen
      rd_pos=UART_RX[uart].rd_ptr;
      n=0;
      while(rd_pos!=wr_pos) {
        wert=UART_RX[uart].rx_buffer[rd_pos];
        // lese pointer weiterschieben
        rd_pos++;
        if(rd_pos>=RX_BUF_SIZE) rd_pos=0;
        // nur Ascii-Zeichen speichern
        if((wert>=RX_FIRST_CHR) && (wert<=RX_LAST_CHR)) {
          ptr[n]=wert;
          n++;
        }
        else if(wert==RX_END_CHR) {
          break;
        }
      }
      // Stringendekennung
      ptr[n]=0x00;
      // lese pointer aktuallisieren
      UART_RX[uart].rd_ptr=rd_pos;
    }
    else {
      // keine Endekennung gefunden
      ret_wert=RX_EMPTY;
    }
  }
  else {
    // check ob ueberlauf aufgetreten
    if(UART_RX[uart].status==RX_FULL) {
      ret_wert=RX_FULL;
      UART_RX[uart].status=RX_EMPTY;
    }
  }

  return(ret_wert);
}

UART_RXSTATUS_t uartGetByte(UART_NAME_t uart, uint8_t *ptr) {
  UART_RXSTATUS_t ret_wert=RX_EMPTY;
  uint16_t rd_pos,wr_pos,n;
  uint8_t wert;

  // aktuelle Pointer auslesen
  rd_pos=UART_RX[uart].rd_ptr;
  wr_pos=UART_RX[uart].wr_ptr;

  if(rd_pos != wr_pos) {
	ret_wert=RX_READY;
	// string komplett auslesen
	wert=UART_RX[uart].rx_buffer[rd_pos];
	*ptr = wert;
	// lese pointer weiterschieben
	rd_pos++;
	if(rd_pos>=RX_BUF_SIZE) rd_pos=0;

	// lese pointer aktuallisieren
	UART_RX[uart].rd_ptr=rd_pos;
  }
  else {
	// check ob ueberlauf aufgetreten
	if(UART_RX[uart].status==RX_FULL) {
	  ret_wert=RX_FULL;
	  UART_RX[uart].status=RX_EMPTY;
	}
  }

  return(ret_wert);
}

void uartFlush(UART_NAME_t uart){
	memset(&UART_RX[uart].rx_buffer[0], 0, sizeof(UART_RX[uart].rx_buffer));
	UART_RX[uart].rd_ptr = 0;
	UART_RX[uart].wr_ptr = 0;
}


//--------------------------------------------------------------
// ein Daten Array per UART senden
// cnt = Anzahl der Daten die gesendet werden sollen
//--------------------------------------------------------------
void uartSendArray(UART_NAME_t uart, uint8_t *data, uint16_t cnt)
{
  uint32_t n;

  if(cnt==0) return;

  // sende alle Daten
  for(n=0;n<cnt;n++) {
    uartSendByte(uart,data[n]);
  }
}


//--------------------------------------------------------------
// ein Daten Array per UART empfangen
// Return Wert :
//   Anzahl der bis jetzt empfangenen Bytes [0...RX_BUF_SIZE]
//--------------------------------------------------------------
uint32_t uartReceiveArray(UART_NAME_t uart, uint8_t *data)
{
  uint32_t n=0;
  uint16_t rd_pos,wr_pos;
  uint8_t wert;

  // aktuelle Pointer auslesen
  rd_pos=UART_RX[uart].rd_ptr;
  wr_pos=UART_RX[uart].wr_ptr;
  // check ob daten im puffer
  n=0;
  if(rd_pos!=wr_pos) {
    // alle daten auslesen
    while(rd_pos!=wr_pos) {
      wert=UART_RX[uart].rx_buffer[rd_pos];
      // lese pointer weiterschieben
      rd_pos++;
      if(rd_pos>=RX_BUF_SIZE) rd_pos=0;
      // daten uebergeben
      data[n]=wert;
      n++;
    }
    // lese pointer aktuallisieren
    UART_RX[uart].rd_ptr=rd_pos;
  }

  return(n);
}


//--------------------------------------------------------------
// interne Funktion
// speichern des empfangenen Zeichens im Puffer
//--------------------------------------------------------------
void P_UART_Receive(UART_NAME_t uart, uint16_t wert)
{
  uint16_t wr_pos;

  // byte an schreib position speichern
  wr_pos=UART_RX[uart].wr_ptr;

  UART_RX[uart].rx_buffer[wr_pos]=wert;
  // schreib pointer weiterschieben
  wr_pos++;
  if(wr_pos>=RX_BUF_SIZE) wr_pos=0;
  UART_RX[uart].wr_ptr=wr_pos;

  // check ob Ueberlauf
  if(UART_RX[uart].wr_ptr==UART_RX[uart].rd_ptr) {
    UART_RX[uart].status=RX_FULL;
  }
}


//--------------------------------------------------------------
// interne Funktion
// UART-Interrupt-Funktion
// Interrupt-Nr muss uebergeben werden
//--------------------------------------------------------------
void P_UART_RX_INT(uint8_t int_nr, uint16_t wert)
{
  UART_NAME_t nr;

  // den passenden Eintrag suchen
  for(nr=0;nr<UART_ANZ;nr++) {
    if(UART[nr].INT==int_nr) {
      // eintrag gefunden, Byte speichern
      P_UART_Receive(nr,wert);
      break;
    }
  }
}


//--------------------------------------------------------------
// UART1-Interrupt
//--------------------------------------------------------------
void USART1_IRQHandler(void) {
  uint16_t wert;

  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
    // wenn ein Byte im Empfangspuffer steht
    wert=USART_ReceiveData(USART1);
    // Byte speichern
    P_UART_RX_INT(USART1_IRQn,wert);
  }
}

//--------------------------------------------------------------
// UART2-Interrupt
//--------------------------------------------------------------
//void USART2_IRQHandler(void) {
//  uint16_t wert;
//
//  if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
//    // wenn ein Byte im Empfangspuffer steht
//    wert=USART_ReceiveData(USART2);
//    // Byte speichern
//    P_UART_RX_INT(USART2_IRQn,wert);
//  }
//}


//--------------------------------------------------------------
// UART3-Interrupt
//--------------------------------------------------------------
void USART3_IRQHandler(void) {
  uint16_t wert;

  if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
    // wenn ein Byte im Empfangspuffer steht
    wert=USART_ReceiveData(USART3);
    // Byte speichern
    P_UART_RX_INT(USART3_IRQn,wert);
  }
}

//--------------------------------------------------------------
// UART4-Interrupt
//--------------------------------------------------------------
void UART4_IRQHandler(void) {
  uint16_t wert;

  if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET) {
    // wenn ein Byte im Empfangspuffer steht
    wert=USART_ReceiveData(UART4);
    // Byte speichern
    P_UART_RX_INT(UART4_IRQn,wert);
  }
}

//--------------------------------------------------------------
// UART5-Interrupt
//--------------------------------------------------------------
void UART5_IRQHandler(void) {
  uint16_t wert;

  if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET) {
    // wenn ein Byte im Empfangspuffer steht
    wert=USART_ReceiveData(UART5);
    // Byte speichern
    P_UART_RX_INT(UART5_IRQn,wert);
  }
}

//--------------------------------------------------------------
// UART6-Interrupt
//--------------------------------------------------------------
void USART6_IRQHandler(void) {
  uint16_t wert;

  if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET) {
    // wenn ein Byte im Empfangspuffer steht
    wert=USART_ReceiveData(USART6);
    // Byte speichern
    P_UART_RX_INT(USART6_IRQn,wert);
  }
}
