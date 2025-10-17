#ifdef ARDUINO
#include <Arduino.h>

#include "config/settings.h"
#include "boards/devkit_esp32.h"
#include "boards/m5stickc_plus2.h"

void setup() {
	Serial.begin(115200);
	delay(200);
	Serial.println("EAGLE-FIRMWARE (Arduino) starting...");
}

void loop() {
	// Placeholder loop
	delay(1000);
}
#endif


