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
		Serial.println("[Display] Initializing M5StickC Plus 2...");
		auto cfg = M5.config();
		M5.begin(cfg);
		Serial.println("[Display] M5 initialized");
		
		M5.Display.fillScreen(BLACK);
		M5.Display.setRotation(1);
		M5.Display.setTextColor(WHITE);
		M5.Display.setTextSize(1);
		M5.Display.setCursor(10, 10);
		M5.Display.println("EAGLE-FIRMWARE");
		M5.Display.setCursor(10, 30);
		M5.Display.setTextColor(GREEN);
		M5.Display.println("Ready!");
		Serial.println("[Display] LCD initialized");
#else
		Serial.println("[Display] Non-M5 board - display disabled");
#endif
	}
	void loop() override {
#ifdef BOARD_M5STICKC_PLUS2
		M5.update();
#endif
	}
};


