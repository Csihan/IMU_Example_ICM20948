#include "UART.h"
#include "ICM20948.h"

static _Serial Serial;
static _ICM20948 ICM20948;

int main(void){

	Serial.begin(115200);
	Serial.println("ICM-20948 Library Test");
	Serial.println("Initialize ICM-20948...");
	ICM20948.Init();

	char ch[60];
	while(1){
		int16_t *ACCEL, *GYRO, *MAG;
		float TEMP;
		ACCEL = ICM20948.getRawAcc();
		GYRO = ICM20948.getRawGyro();
		MAG = ICM20948.getRawMag();
		TEMP = ICM20948.getTemp(0);
		sprintf(ch, "%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d,%5d",
				ACCEL[0], ACCEL[1], ACCEL[2],
				GYRO[0], GYRO[1], GYRO[2],
				MAG[0], MAG[1], MAG[2]);
		Serial.println(ch);
	}

	return 0;
}
