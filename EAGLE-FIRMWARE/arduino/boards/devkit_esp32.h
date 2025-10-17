#pragma once

#ifdef ARDUINO
#include <Arduino.h>

// GPIO defaults for generic ESP32 DevKit
namespace Board {
	static const char* NAME = "ESP32 DevKit";
	static const int LED_PIN = 2;
}

#endif


