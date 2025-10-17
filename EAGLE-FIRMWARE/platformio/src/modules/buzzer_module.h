#pragma once

#include <Arduino.h>
#include "core/module_manager.h"
#include "boards/m5stickc_plus2.h"

class BuzzerModule : public IModule {
public:
	const char* getName() const override { return "buzzer"; }
	void setup() override {
		pinMode(BoardM5StickC2::BUZZER_PIN, OUTPUT);
		digitalWrite(BoardM5StickC2::BUZZER_PIN, LOW);
	}
	void loop() override {}
	void beep(uint16_t ms = 100) {
		digitalWrite(BoardM5StickC2::BUZZER_PIN, HIGH);
		delay(ms);
		digitalWrite(BoardM5StickC2::BUZZER_PIN, LOW);
	}
};


