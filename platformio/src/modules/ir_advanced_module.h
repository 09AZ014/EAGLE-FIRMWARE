#pragma once

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <vector>
#include <functional>
#include "core/module_manager.h"

struct IRSignal {
    uint32_t frequency;
    uint32_t* rawData;
    size_t rawDataLength;
    String protocol;
    uint32_t value;
    uint8_t bits;
    bool isRepeat;
    uint32_t timestamp;
    String description;
};

struct IRProtocol {
    String name;
    uint32_t frequency;
    uint8_t bits;
    bool hasRepeat;
    String description;
};

class IRAdvancedModule : public IModule {
private:
    IRsend* irSend;
    IRrecv* irRecv;
    bool isInitialized;
    bool isReceiving;
    bool isTransmitting;
    
    std::vector<IRSignal> capturedSignals;
    std::vector<IRSignal> savedSignals;
    std::vector<IRProtocol> supportedProtocols;
    
    // TV-B-Gone functionality
    bool tvbGoneActive;
    std::vector<IRSignal> tvbGoneSignals;
    uint32_t tvbGoneIndex;
    uint32_t tvbGoneInterval;
    
    // Protocol handlers
    void initializeProtocols();
    void processReceivedSignal(decode_results* results);
    String identifyProtocol(decode_results* results);
    String getProtocolDescription(String protocol);
    
    // TV-B-Gone implementation
    void loadTVBGoneSignals();
    void startTVBGone();
    void stopTVBGone();
    void nextTVBGoneSignal();
    
    // Signal management
    bool saveSignal(IRSignal& signal, String filename);
    bool loadSignal(String filename, IRSignal& signal);
    bool deleteSignal(String filename);
    std::vector<String> getSavedSignals();
    
    // Protocol encoding/decoding
    String encodeSignal(IRSignal& signal);
    bool decodeSignal(String data, IRSignal& signal);
    
public:
    IRAdvancedModule();
    ~IRAdvancedModule();
    
    const char* getName() const override { return "ir_advanced"; }
    void setup() override;
    void loop() override;
    
    // Module Control
    bool initialize(uint8_t txPin, uint8_t rxPin);
    bool deinitialize();
    bool isModuleReady() { return isInitialized; }
    
    // Pin Configuration
    bool setTXPin(uint8_t pin);
    bool setRXPin(uint8_t pin);
    bool setFrequency(uint32_t frequency);
    
    // Signal Reception
    bool startReceiving();
    bool stopReceiving();
    bool isReceivingActive() { return isReceiving; }
    std::vector<IRSignal> getCapturedSignals() { return capturedSignals; }
    void clearCapturedSignals();
    IRSignal getLastSignal();
    
    // Signal Transmission
    bool transmitSignal(IRSignal& signal);
    bool transmitRaw(uint32_t* data, size_t length, uint32_t frequency = 38000);
    bool transmitProtocol(String protocol, uint32_t value, uint8_t bits = 32);
    bool isTransmittingActive() { return isTransmitting; }
    
    // TV-B-Gone
    bool startTVBGone();
    bool stopTVBGone();
    bool isTVBGoneActive() { return tvbGoneActive; }
    void setTVBGoneInterval(uint32_t interval) { tvbGoneInterval = interval; }
    uint32_t getTVBGoneSignalCount() { return tvbGoneSignals.size(); }
    
    // Protocol Support
    std::vector<IRProtocol> getSupportedProtocols() { return supportedProtocols; }
    bool isProtocolSupported(String protocol);
    String getProtocolInfo(String protocol);
    
    // Signal Analysis
    String analyzeSignal(IRSignal& signal);
    String getSignalInfo(IRSignal& signal);
    bool isSignalValid(IRSignal& signal);
    String getSignalDescription(IRSignal& signal);
    
    // Signal Management
    bool saveSignal(IRSignal& signal, String filename);
    bool loadSignal(String filename, IRSignal& signal);
    bool deleteSignal(String filename);
    std::vector<String> getSavedSignals();
    bool exportSignals(String filename);
    bool importSignals(String filename);
    
    // Custom Protocols
    bool addCustomProtocol(IRProtocol protocol);
    bool removeCustomProtocol(String name);
    bool updateCustomProtocol(String name, IRProtocol protocol);
    
    // Signal Processing
    bool filterSignal(IRSignal& signal, String filter);
    bool amplifySignal(IRSignal& signal, float factor);
    bool normalizeSignal(IRSignal& signal);
    bool compressSignal(IRSignal& signal);
    
    // Batch Operations
    bool transmitSignals(std::vector<IRSignal>& signals, uint32_t interval = 1000);
    bool captureMultipleSignals(uint32_t count, uint32_t timeout = 10000);
    bool replaySignals(std::vector<IRSignal>& signals, uint32_t interval = 1000);
    
    // Configuration
    bool setReceiveTimeout(uint32_t timeout);
    bool setTransmitPower(uint8_t power);
    bool setReceiveSensitivity(uint8_t sensitivity);
    bool setProtocolTimeout(uint32_t timeout);
    
    // Information
    String getModuleInfo();
    String getSignalCount();
    String getProtocolCount();
    String getTVBGoneStatus();
    
    // Callbacks
    std::function<void(IRSignal signal)> onSignalReceived;
    std::function<void(IRSignal signal)> onSignalTransmitted;
    std::function<void(String protocol)> onProtocolDetected;
    std::function<void(String error)> onError;
    
    // Display integration
    void drawStatus();
    void drawSignals();
    void drawProtocols();
    void drawTVBGoneStatus();
};
