#pragma once

#include <Arduino.h>
#include "core/module_manager.h"
#include "boards/m5stickc_plus2.h"

class SafeModeModule : public IModule {
public:
	SafeModeModule() : safe(false) {}
	const char* getName() const override { return "safe_mode"; }
	void setup() override {
#ifdef BOARD_M5STICKC_PLUS2
		pinMode(BoardM5StickC2::BTN_A, INPUT);
#endif
		// read once at boot
#ifdef BOARD_M5STICKC_PLUS2
		safe = (digitalRead(BoardM5StickC2::BTN_A) == LOW);
#else
		safe = false;
#endif
	}
	void loop() override {}
	bool isSafe() const { return safe; }
private:
	bool safe;
};


