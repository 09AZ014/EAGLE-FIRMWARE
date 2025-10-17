#pragma once

#ifdef ARDUINO
#include <Arduino.h>

// Configuração básica para M5StickC Plus 2
namespace BoardM5StickC2 {
	static const char* NAME = "M5StickC Plus 2";
	// ST7789 via M5 libs (LovyanGFX/M5StickCPlus2)
	static const int TFT_WIDTH = 135;
	static const int TFT_HEIGHT = 240;
	// Botões
	static const int BTN_A = 37; // GPIO37
	static const int BTN_B = 39; // GPIO39
	// Buzzer (se disponível)
	static const int BUZZER_PIN = 2; // placeholder
}

#endif


