#pragma once

#include <Arduino.h>
#ifdef BOARD_M5STICKC_PLUS2
#include <M5StickCPlus2.h>
#endif
#include "core/module_manager.h"

class DisplayModule : public IModule {
public:
	const char* getName() const override { return "display"; }
	void setup() override {
#ifdef BOARD_M5STICKC_PLUS2
		M5.begin();
		M5.Lcd.fillScreen(BLACK);
		M5.Lcd.setRotation(1);
		M5.Lcd.setTextColor(WHITE);
		M5.Lcd.setTextSize(2);
		M5.Lcd.setCursor(0,0);
		M5.Lcd.println("EAGLE");
#endif
	}
	void loop() override {
#ifdef BOARD_M5STICKC_PLUS2
		M5.update();
#endif
	}
};


