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
		Serial.println("[IMU] Initializing...");
		if (M5.Imu.begin()) {
			Serial.println("[IMU] OK");
		} else {
			Serial.println("[IMU] Failed (not critical)");
		}
#else
		Serial.println("[IMU] Non-M5 board - IMU disabled");
#endif
	}
	void loop() override {
		// IMU loop disabled to reduce overhead
	}
};


