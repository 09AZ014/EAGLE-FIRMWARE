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
		M5.Lcd.println("EAGLE BOOT");
		M5.Lcd.setCursor(0,30);
		M5.Lcd.setTextSize(1);
		M5.Lcd.println("Display OK!");
		M5.Lcd.setCursor(0,50);
		M5.Lcd.setTextColor(YELLOW);
		M5.Lcd.println("If you see this, LCD works!");
		// Teste: troca de cor após 1 segundo
		for(int i=0;i<255;i+=50) {
			M5.Lcd.fillScreen(i<<16); // Incrementa vermelho (visual debug)
			delay(100);
		}
		M5.Lcd.fillScreen(BLACK);
		M5.Lcd.setCursor(0,0);
		M5.Lcd.setTextColor(WHITE);
		M5.Lcd.setTextSize(2);
		M5.Lcd.println("EAGLE RUN");
		// Backlight (se aplicável, para modelos com função)
		#ifdef M5STICKC_PLUS2_HAS_BACKLIGHT
		M5.Axp.ScreenBreath(10);
		#endif
#endif
	}
	void loop() override {
#ifdef BOARD_M5STICKC_PLUS2
		M5.update();
		static uint32_t last = 0;
		if(millis()-last > 2000) {
			last = millis();
			M5.Lcd.setCursor(0,80);
			M5.Lcd.setTextColor(GREEN,BLACK);
			M5.Lcd.printf("Uptime: %lus    ", millis()/1000);
		}
#endif
	}
};


