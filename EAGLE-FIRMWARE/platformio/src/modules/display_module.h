#pragma once

#include <Arduino.h>
#ifdef BOARD_M5STICKC_PLUS2
#include <M5Unified.h>
#endif
#include "core/module_manager.h"

class DisplayModule : public IModule {
public:
	const char* getName() const override { return "display"; }
	void setup() override {
#ifdef BOARD_M5STICKC_PLUS2
		Serial.println("[Display] Starting M5StickC Plus 2 initialization...");
		
		// Configuração básica sem complexidade
		auto cfg = M5.config();
		cfg.clear_display = true;
		cfg.output_power = true;
		cfg.internal_imu = false;  // IMU separado
		cfg.internal_rtc = false;  // RTC separado
		cfg.internal_spk = false;  // Speaker separado
		cfg.internal_mic = false;  // Mic separado
		
		Serial.println("[Display] Calling M5.begin()...");
		M5.begin(cfg);
		Serial.println("[Display] M5.begin() SUCCESS");
		
		delay(100);
		
		// Configurar display
		Serial.println("[Display] Configuring LCD...");
		M5.Display.setRotation(1);
		M5.Display.setBrightness(128);
		M5.Display.fillScreen(TFT_BLACK);
		M5.Display.setTextSize(2);
		M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
		
		// Banner EAGLE
		M5.Display.setCursor(10, 10);
		M5.Display.setTextColor(TFT_YELLOW);
		M5.Display.println("EAGLE");
		
		M5.Display.setTextSize(1);
		M5.Display.setTextColor(TFT_GREEN);
		M5.Display.setCursor(10, 35);
		M5.Display.println("FIRMWARE v1.0");
		
		M5.Display.setTextColor(TFT_WHITE);
		M5.Display.setCursor(10, 55);
		M5.Display.println("Pentesting Tool");
		
		M5.Display.setCursor(10, 75);
		M5.Display.setTextColor(TFT_CYAN);
		M5.Display.println("Ready!");
		
		Serial.println("[Display] LCD initialized successfully!");
#else
		Serial.println("[Display] Non-M5 board - display disabled");
#endif
	}
	
	void loop() override {
#ifdef BOARD_M5STICKC_PLUS2
		M5.update();
		
		// Atualizar status a cada 2 segundos
		static unsigned long lastUpdate = 0;
		if (millis() - lastUpdate > 2000) {
			lastUpdate = millis();
			
			M5.Display.fillRect(10, 95, 220, 20, TFT_BLACK);
			M5.Display.setCursor(10, 95);
			M5.Display.setTextColor(TFT_ORANGE);
			M5.Display.setTextSize(1);
			M5.Display.printf("Up: %lus Heap:%dK", millis()/1000, ESP.getFreeHeap()/1024);
		}
#endif
	}
	
	void showMessage(const char* msg, uint16_t color = TFT_WHITE) {
#ifdef BOARD_M5STICKC_PLUS2
		M5.Display.fillRect(0, 115, 240, 20, TFT_BLACK);
		M5.Display.setCursor(10, 115);
		M5.Display.setTextColor(color);
		M5.Display.setTextSize(1);
		M5.Display.println(msg);
#endif
	}
};


