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
		int16_t *ACCEL, *GYRO;
		float TEMP;
		ACCEL = ICM20948.getRawAcc();
		GYRO = ICM20948.getRawGyro();
		TEMP = ICM20948.getTemp(0);
		sprintf(ch, "Accel : %5d %5d %5d  Gyro : %4d %4d %4d  Temp : %3d",
				ACCEL[0], ACCEL[1], ACCEL[2],
				GYRO[0], GYRO[1], GYRO[2],
				(int16_t)TEMP);
		Serial.println(ch);
	}

	return 0;
}
