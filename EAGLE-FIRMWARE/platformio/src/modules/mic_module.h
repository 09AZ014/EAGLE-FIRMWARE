#pragma once

#include <Arduino.h>
#include "core/module_manager.h"

class MicModule : public IModule {
public:
	const char* getName() const override { return "mic"; }
	void setup() override {
		// Placeholder: configure ADC/INMP441 I2S if available
	}
	void loop() override {
		// Placeholder: compute simple level and log/bargraph serial
	}
};


