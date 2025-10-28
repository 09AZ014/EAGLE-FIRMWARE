#ifndef IR_MODULE_H
#define IR_MODULE_H

#include "../core/module_manager.h"
#include <Arduino.h>

// Basic IR TV-B-Gone implementation
class IRModule : public IModule {
public:
    IRModule() {}
    
    const char* getName() const override { return "IR Module"; }
    void setup() override;
    void loop() override {}
    
    // TV-B-Gone functionality
    void tvbGone();
    void sendIRCode(uint16_t code, uint8_t bits);
    
private:
    const int IR_LED_PIN = 4;  // Default IR LED pin
    
    // TV power codes (NEC protocol)
    struct IRCode {
        uint16_t code;
        uint8_t bits;
    };
    
    // Common TV brands power codes
    static const IRCode TV_CODES[];
    static const int TV_CODES_COUNT;
};

#endif // IR_MODULE_H
