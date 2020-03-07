/*
 *  Roverdyn UART Library
 *  Created at Jan. 14, 2020
 *  Author : Seonguk Jeong
 *  Copyright to 2019 Seonguk Jeong. All rights reserved.
 *  Contact : jswcomkr@roverdyn.com
 */

#include <stdio.h>
#include <nrf_uart.h>
#include <string.h>

#define PIN_UART_TXD 6
#define PIN_UART_RXD 8

#ifndef UART_H_
#define UART_H_

class _Serial{
private:
public:
	void begin(uint32_t baudrate);
	void end();
	uint16_t print(const char *message);
	uint16_t println(const char *message);
	uint16_t available();
	uint8_t read();
};


#endif
