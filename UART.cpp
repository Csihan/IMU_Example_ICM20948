#include "UART.h"

/*
 *  Roverdyn UART Library
 *  Created at Jan. 14, 2020
 *  Author : Seonguk Jeong
 *  Copyright to 2019 Seonguk Jeong. All rights reserved.
 *  Contact : jswcomkr@roverdyn.com
 */


void _Serial::begin(uint32_t baudrate){
	// UART 통신 초기화 함수
	NRF_UART0->TASKS_STOPTX = 1;
	NRF_UART0->TASKS_STOPRX = 1;
	NRF_UART0->ENABLE = 0;
	NRF_UART0->PSELTXD = PIN_UART_TXD;
	NRF_UART0->PSELRXD = PIN_UART_RXD;
	NRF_UART0->CONFIG = 0x00;

	// Buadrate 설정
	switch(baudrate){
	case 4800:
		NRF_UART0->BAUDRATE = 0x0013B000;
		break;
	case 9600:
		NRF_UART0->BAUDRATE = 0x00275000;
		break;
	case 19200:
		NRF_UART0->BAUDRATE = 0x004EA000;
		break;
	case 38400:
		NRF_UART0->BAUDRATE = 0x009D5000;
		break;
	case 57600:
		NRF_UART0->BAUDRATE = 0x00EBF000;
		break;
	case 115200:
		NRF_UART0->BAUDRATE = 0x01D7E000;
		break;
	case 230400:
		NRF_UART0->BAUDRATE = 0x03AFB000;
		break;
	case 460800:
		NRF_UART0->BAUDRATE = 0x075F7000;
		break;
	default:
		NRF_UART0->BAUDRATE = 0x01D7E000;
		break;
	}
	NRF_UART0->INTENCLR = 0x01 | (0x01 << 1) | (0x01 << 2) | (0x01 << 7) | (0x01 << 9) | (0x01 << 17);
	NRF_UART0->ENABLE = 4;
	NRF_UART0->TASKS_STARTTX = 1;
	NRF_UART0->TASKS_STARTRX = 1;
}

void _Serial::end(){
	// UART 통신 종료 함수
	NRF_UART0->ENABLE = 0;
}

uint16_t _Serial::print(const char *message){
	// UART TXD 함수(줄바꿈 없음)
	uint16_t len = strlen(message);
	for(uint8_t i=0;i<len;i++){
		NRF_UART0->TXD = message[i];
		while(!NRF_UART0->EVENTS_TXDRDY){}
		NRF_UART0->EVENTS_TXDRDY = 0;
	}

	return len;
}

uint16_t _Serial::println(const char *message){
	// UART TXD 함수(줄바꿈 있음)
	uint16_t len = strlen(message);

	for(uint8_t i=0;i<len;i++){
		NRF_UART0->TXD = message[i];
		while(!NRF_UART0->EVENTS_TXDRDY){}
		NRF_UART0->EVENTS_TXDRDY = 0;
	}
	NRF_UART0->TXD = 10;
	while(!NRF_UART0->EVENTS_TXDRDY){}
	NRF_UART0->EVENTS_TXDRDY = 0;
	NRF_UART0->TXD = 13;
	while(!NRF_UART0->EVENTS_TXDRDY){}
	NRF_UART0->EVENTS_TXDRDY = 0;

	return len;
}

uint16_t _Serial::available(){
	uint16_t output = 0;

	return output;

}

uint8_t _Serial::read(){
	return NRF_UART0->RXD;
}
