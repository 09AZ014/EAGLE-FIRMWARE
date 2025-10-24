#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "core/module_manager.h"

class WifiModule : public IModule {
public:
	const char* getName() const override { return "wifi"; }
	void setup() override {
		WiFi.mode(WIFI_STA);
	}
	void loop() override {
		// placeholder
	}
};


