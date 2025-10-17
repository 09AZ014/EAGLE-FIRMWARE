#pragma once

#include <Arduino.h>
#ifdef BOARD_M5STICKC_PLUS2
#include <M5StickCPlus2.h>
#endif
#include "core/module_manager.h"

class ImuModule : public IModule {
public:
	const char* getName() const override { return "imu"; }
	void setup() override {
#ifdef BOARD_M5STICKC_PLUS2
		M5.Imu.begin();
#endif
	}
	void loop() override {
#ifdef BOARD_M5STICKC_PLUS2
		float ax, ay, az, gx, gy, gz;
		M5.Imu.getAccelData(&ax, &ay, &az);
		M5.Imu.getGyroData(&gx, &gy, &gz);
		// Simple periodic log
		static unsigned long last = 0;
		if (millis() - last > 500) {
			last = millis();
			Serial.printf("IMU A:%.2f,%.2f,%.2f G:%.2f,%.2f,%.2f\n", ax, ay, az, gx, gy, gz);
		}
#endif
	}
};


