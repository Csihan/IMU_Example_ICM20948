/*
 *  Roverdyn UART Library
 *  Created at Mar. 6, 2020
 *  Author : Seonguk Jeong
 *  Copyright to 2020 Seonguk Jeong. All rights reserved.
 *  Contact : jswcomkr@roverdyn.com
 */

#include "ICM20948.h"

_Wire Wire;
_Serial Serial;

uint8_t _ICM20948::Init(){
	uint8_t err_code = 0;

	// TWI 통신 초기화 및 시작
	Wire.begin();

	// ICM20948 초기화
	writeData(DEVICE_ID, REG_BANK_SEL, 0x00);
	writeData(DEVICE_ID, PWR_MGMT_1, 0x80);
	nrf_delay_ms(100);
	writeData(DEVICE_ID, PWR_MGMT_1, 0x01);
	writeData(DEVICE_ID, PWR_MGMT_2, 0x00);

	// 바이패스 모드 설정
	writeData(DEVICE_ID, INT_PIN_CFG, (0x01 << 1));

	// Device ID 읽기
	// 제대로 연결이 되어 있지 않으면 0x01(에러)를 리턴
	if(getDeviceId() != 234){
		return ROVERDYN_ERROR_NO_DATA;
	}/*
	if(getMagId() != 0x09){
		return ROVERDYN_ERROR_NO_DATA;
	}*/
	char ch[40];
	sprintf(ch, "Device ID : 0x%02X(IMU), 0x%02X(MAG)", getDeviceId(), getMagId());
	Serial.println(ch);

	// 자이로스코프 설정
	setCfgGyro(DLPF_GYRO_6HZ, GYRO_RANGE_1000DPS, GYRO_AVG_1X, true);

	// 가속도 센서 설정
	setCfgAcc(DLPF_ACC_6HZ, ACCEL_RANGE_2G, ACCEL_AVG_4X, true);

	// 지자기 센서 설정
	setMagMode(MAG_CONTINUOUS_MODE2);

	// 온도 센서 설정
	setCfgTemp(TEMP_DLPF_34HZ);

	return err_code;
}

uint8_t _ICM20948::writeData(uint8_t device, uint8_t addr, uint8_t data){
	// 레지스터에 값을 쓰는 함수
	uint8_t err_code = 0;

	// 통신 시작
	Wire.beginTransmission(device);
	Wire.write(addr);
	Wire.write(data);
	Wire.endTransmission(true);

	return err_code;
}

uint8_t _ICM20948::getDeviceId(){
	// Who AM I 레지스터로부터 데이터를 읽어오는 함수
	uint8_t output = 0;

	Wire.beginTransmission(DEVICE_ID);
	Wire.write(WHO_AM_I);
	Wire.requestFrom(DEVICE_ID, 1);
	output = Wire.read();
	Wire.endTransmission(true);
	if(output != 234){
		return ROVERDYN_ERROR_NO_DATA;
	}
	return output;
}

uint8_t _ICM20948::getMagId(){
	// 지자기 센서의 WIA 레지스터로부터 데이터를 읽어오는 함수
	uint8_t output = 0;

	// Get Data
	Wire.beginTransmission(AK09916);
	Wire.write(WIA2);
	Wire.requestFrom(AK09916, 1);
	output = Wire.read();
	Wire.endTransmission(true);

	return output;
}

uint8_t _ICM20948::setCfgAcc(uint8_t ACCEL_DLPF_SEL, uint8_t ACCEL_FS_SEL, uint8_t ACCEL_AVG_SEL, bool Fchoice_b){
	// 가속도 센서의 측정 범위와 DLPF 적용 유무 설정 함수
	uint8_t err_code = 0;

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x20);

	// Set Accel Config 1
	uint8_t data = (ACCEL_DLPF_SEL << 3) | (ACCEL_FS_SEL << 1) | (Fchoice_b);
	writeData(DEVICE_ID, ACCEL_CONFIG, data);

	// Set Accel Config 2
	writeData(DEVICE_ID, ACCEL_CONFIG_2, ACCEL_FS_SEL);

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x00);

	return err_code;
}

uint8_t _ICM20948::setCfgGyro(uint8_t GYRO_DLPF_SEL, uint8_t GYRO_FS_SEL, uint8_t GYRO_AVG_SEL, bool Fchoice_b){
	// 자이로스코프의 측정 범위와 DLPF 적용 유무 설정 함수
	uint8_t err_code = 0;

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x20);

	// Set Gyro Config 1
	uint8_t data = (GYRO_DLPF_SEL << 3) | (GYRO_FS_SEL << 1) | (Fchoice_b);
	writeData(DEVICE_ID, GYRO_CONFIG_1, data);

	// Set Gyro Config 2
	writeData(DEVICE_ID, GYRO_CONFIG_2, GYRO_AVG_SEL);

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x00);

	return err_code;
}

uint8_t _ICM20948::setCfgTemp(uint8_t TEMP_DLPF_SET){
	// 온도 센서의 DLPF 설정
	uint8_t err_code = 0;

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x20);

	// Set DLPF
	writeData(DEVICE_ID, TEMP_CONFIG, TEMP_DLPF_SET);

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x00);

	return err_code;
}

uint8_t _ICM20948::setMagMode(uint8_t MODE){
	// 지자기 센서 설정 함수
	uint8_t err_code = 0;

	// Mode 설정
	writeData(AK09916, CNTL2, MODE);
	return err_code;
}

int16_t* _ICM20948::getMagASA(){
	// 지자기 센서 ASA 데이터 읽기 함수
	static int16_t output[3];

	return output;
}

int16_t* _ICM20948::getRawAcc(){
	// 가속도 센서 Raw 데이터 읽기 함수
	static int16_t output[3] = {0};

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x00);

	// Get Data
	Wire.beginTransmission(DEVICE_ID);
	Wire.write(ACCEL_XOUT_H);
	Wire.requestFrom(DEVICE_ID, 6);
	for(uint8_t i=0;i<3;i++){
		output[i] = Wire.read() << 8 | Wire.read();
	}
	Wire.endTransmission(true);
	return output;
}

int16_t* _ICM20948::getRawGyro(){
	// 자이로스코프 Raw 데이터 읽기 함수
	static int16_t output[3] = {0};

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x00);

	// Get Data
	Wire.beginTransmission(DEVICE_ID);
	Wire.write(GYRO_XOUT_H);
	Wire.requestFrom(DEVICE_ID, 6);
	for(uint8_t i=0;i<3;i++){
		output[i] = Wire.read() << 8 | Wire.read();
	}
	Wire.endTransmission(true);

	return output;
}

int16_t* _ICM20948::getRawMag(){
	// 지자기 센서 Raw 데이터 취득 함수
	static int16_t output[3] = {0};

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x00);

	// Get Data
	Wire.beginTransmission(AK09916);
	Wire.write(ST1);
	Wire.requestFrom(AK09916, 1);
	bool st1 = Wire.read() & 0x01;
	Wire.endTransmission(true);
	if(st1){
		// Read Mag Data
		Wire.beginTransmission(AK09916);
		Wire.write(HXL);
		Wire.requestFrom(AK09916, 6);
		for(uint8_t i=0;i<3;i++){
			output[i] = Wire.read();
			output[i] = (Wire.read() << 8) | output[i];
		}
		Wire.endTransmission(true);

		// Read ST2 register
		Wire.beginTransmission(AK09916);
		Wire.write(ST2);
		Wire.requestFrom(AK09916, 1);
		bool st2 = (Wire.read() >> 3) & 0x01;
		if(st2){
			return 0;
		}
		Wire.endTransmission(true);
	}
	return output;
}

float _ICM20948::getTemp(float TEMP_OFFSET){
	// 온도 데이터 수신 함수
	float output = 0;

	// Select Bank
	writeData(DEVICE_ID, REG_BANK_SEL, 0x00);

	// Get Data
	Wire.beginTransmission(DEVICE_ID);
	Wire.write(TEMP_OUT_H);
	Wire.requestFrom(DEVICE_ID, 2);
	output = Wire.read() << 8 | Wire.read();
	Wire.endTransmission(true);

	// 측정한 데이터를 Degree 단위로 변환
	output = ((output - TEMP_OFFSET)/TEMP_SENSITIVITY) + 21.0f;

	return output;
}
