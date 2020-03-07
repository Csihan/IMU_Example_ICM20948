/*
 *  Roverdyn I2C(TWI) Library
 *  Created at Jan. 14, 2020
 *  Author : Seonguk Jeong
 *  Copyright to 2019 Seonguk Jeong. All rights reserved.
 *  Contact : jswcomkr@roverdyn.com
 */

#ifndef WIRE_H_
#define WIRE_H_

#include <nrf_twi.h>
#include <stdio.h>
#include <string.h>

#define pinSDA 11
#define pinSCL 12

class _Wire{
public:
	void begin();
	uint8_t rxBuffer[256] = {0}, rCount = 0;
	uint8_t requestFrom(uint8_t address, uint8_t bytes);
	void beginTransmission(uint8_t address);
	uint8_t endTransmission(bool toggle);
	uint8_t write(uint8_t data);
	uint8_t available();
	uint8_t read();
	void setClock(uint32_t frequency);
};

#endif
