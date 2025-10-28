#include "ir_module.h"

// Common TV power codes (NEC protocol)
const IRModule::IRCode IRModule::TV_CODES[] = {
    {0x20DF10EF, 32},  // LG
    {0xE0E040BF, 32},  // Samsung
    {0xA90, 12},       // Sony
    {0x61D6, 16},      // Panasonic
    {0x02FD, 16},      // Philips
    {0x0BF4, 16},      // Sharp
    {0x1CE3, 16},      // Toshiba
    {0x08F7, 16},      // JVC
};

const int IRModule::TV_CODES_COUNT = sizeof(TV_CODES) / sizeof(TV_CODES[0]);

void IRModule::setup() {
    pinMode(IR_LED_PIN, OUTPUT);
    digitalWrite(IR_LED_PIN, LOW);
    Serial.println("[IR] Module initialized");
}

void IRModule::sendIRCode(uint16_t code, uint8_t bits) {
    // Simple NEC protocol implementation
    // 38kHz carrier frequency
    const int CARRIER_FREQ = 38000;
    const int PULSE_US = 1000000 / CARRIER_FREQ / 2;
    
    // Start burst
    for (int i = 0; i < 9000 / PULSE_US; i++) {
        digitalWrite(IR_LED_PIN, HIGH);
        delayMicroseconds(PULSE_US);
        digitalWrite(IR_LED_PIN, LOW);
        delayMicroseconds(PULSE_US);
    }
    
    // Space
    delayMicroseconds(4500);
    
    // Send data bits
    for (int i = bits - 1; i >= 0; i--) {
        // Burst
        for (int j = 0; j < 560 / PULSE_US; j++) {
            digitalWrite(IR_LED_PIN, HIGH);
            delayMicroseconds(PULSE_US);
            digitalWrite(IR_LED_PIN, LOW);
            delayMicroseconds(PULSE_US);
        }
        
        // Space (depends on bit value)
        if (code & (1 << i)) {
            delayMicroseconds(1690);  // Logical 1
        } else {
            delayMicroseconds(560);   // Logical 0
        }
    }
    
    // Final burst
    for (int i = 0; i < 560 / PULSE_US; i++) {
        digitalWrite(IR_LED_PIN, HIGH);
        delayMicroseconds(PULSE_US);
        digitalWrite(IR_LED_PIN, LOW);
        delayMicroseconds(PULSE_US);
    }
}

void IRModule::tvbGone() {
    Serial.println("[IR] Starting TV-B-Gone...");
    Serial.println("[IR] Sending power codes for common TV brands");
    
    int codesSent = 0;
    for (int i = 0; i < TV_CODES_COUNT; i++) {
        Serial.printf("[IR] Sending code %d/%d\n", i + 1, TV_CODES_COUNT);
        
        // Send each code 3 times for reliability
        for (int repeat = 0; repeat < 3; repeat++) {
            sendIRCode(TV_CODES[i].code, TV_CODES[i].bits);
            delay(50);
        }
        
        delay(200);
        codesSent++;
    }
    
    Serial.printf("[IR] TV-B-Gone complete! Sent %d codes\n", codesSent);
}
