#pragma once

#include <Arduino.h>
#include "core/module_manager.h"
#include "modules/buzzer_module.h"

class SelfTestModule : public IModule {
public:
	SelfTestModule(BuzzerModule& buzzer) : buzzer(buzzer), ran(false) {}
	const char* getName() const override { return "self_test"; }
	void setup() override {
		if (ran) return;
		// Beep short sequence
		buzzer.beep(60);
		delay(80);
		buzzer.beep(120);
		ran = true;
	}
	void loop() override {}
private:
	BuzzerModule& buzzer;
	bool ran;
};


