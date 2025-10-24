#pragma once

#include <Arduino.h>
// #include <CC1101.h> // Temporarily disabled due to library issues
#include <vector>
#include <functional>
#include "core/module_manager.h"

struct RFSignal {
    uint32_t frequency;
    uint32_t duration;
    uint8_t* data;
    size_t dataLength;
    String protocol;
    String description;
    uint32_t timestamp;
    int32_t rssi;
};

struct RFAttack {
    String attackType; // "jammer", "replay", "custom"
    uint32_t frequency;
    bool isRunning;
    uint32_t packetsSent;
    uint32_t startTime;
    String targetProtocol;
};

class RFAdvancedModule : public IModule {
private:
    // CC1101 rfModule; // Temporarily disabled
    bool isInitialized;
    bool isScanning;
    bool isJamming;
    bool isReplaying;
    
    std::vector<RFSignal> capturedSignals;
    std::vector<RFSignal> recentSignals;
    std::vector<RFAttack> activeAttacks;
    
    // Spectrum analysis
    bool spectrumActive;
    std::vector<uint32_t> spectrumData;
    uint32_t spectrumStartFreq;
    uint32_t spectrumEndFreq;
    uint32_t spectrumStep;
    
    // Jammer settings
    uint32_t jammerFrequency;
    uint32_t jammerPower;
    bool jammerContinuous;
    uint32_t jammerInterval;
    
    // Replay settings
    RFSignal* replaySignal;
    uint32_t replayCount;
    uint32_t replayInterval;
    
    void initializeRFModule();
    void scanFrequency(uint32_t frequency);
    void captureSignal(uint32_t frequency);
    void analyzeSignal(RFSignal& signal);
    void identifyProtocol(RFSignal& signal);
    void startJammer();
    void stopJammer();
    void startReplay();
    void stopReplay();
    void startSpectrumScan();
    void stopSpectrumScan();
    
    // Signal processing
    String decodeSignal(RFSignal& signal);
    bool encodeSignal(String data, RFSignal& signal);
    void filterSignal(RFSignal& signal);
    void amplifySignal(RFSignal& signal);
    
    // Protocol handlers
    String decodeNEC(RFSignal& signal);
    String decodeRC5(RFSignal& signal);
    String decodeRC6(RFSignal& signal);
    String decodeSony(RFSignal& signal);
    String decodeSamsung(RFSignal& signal);
    String decodeRaw(RFSignal& signal);
    
    bool encodeNEC(String data, RFSignal& signal);
    bool encodeRC5(String data, RFSignal& signal);
    bool encodeRC6(String data, RFSignal& signal);
    bool encodeSony(String data, RFSignal& signal);
    bool encodeSamsung(String data, RFSignal& signal);
    bool encodeRaw(String data, RFSignal& signal);
    
public:
    RFAdvancedModule();
    ~RFAdvancedModule();
    
    const char* getName() const override { return "rf_advanced"; }
    void setup() override;
    void loop() override;
    
    // RF Module Control
    bool initialize();
    bool deinitialize();
    bool isModuleReady() { return isInitialized; }
    
    // Frequency Control
    bool setFrequency(uint32_t frequency);
    bool setPower(uint8_t power);
    bool setModulation(uint8_t modulation);
    uint32_t getCurrentFrequency();
    uint8_t getCurrentPower();
    
    // Scanning
    bool startScan(uint32_t startFreq, uint32_t endFreq, uint32_t step = 100000);
    bool stopScan();
    bool isScanningActive() { return isScanning; }
    std::vector<RFSignal> getCapturedSignals() { return capturedSignals; }
    void clearCapturedSignals();
    
    // Signal Capture
    bool startCapture(uint32_t frequency);
    bool stopCapture();
    bool isCapturing();
    RFSignal getLastSignal();
    std::vector<RFSignal> getRecentSignals() { return recentSignals; }
    
    // Signal Analysis
    String analyzeSignal(RFSignal& signal);
    String identifyProtocol(RFSignal& signal);
    String decodeSignal(RFSignal& signal);
    bool isSignalValid(RFSignal& signal);
    
    // Signal Replay
    bool replaySignal(RFSignal& signal, uint32_t count = 1, uint32_t interval = 1000);
    bool stopReplay();
    bool isReplaying() { return isReplaying; }
    void setReplayInterval(uint32_t interval) { replayInterval = interval; }
    
    // Jammer
    bool startJammer(uint32_t frequency, uint32_t power = 7, bool continuous = true);
    bool startIntermittentJammer(uint32_t frequency, uint32_t interval = 1000);
    bool stopJammer();
    bool isJamming() { return isJamming; }
    void setJammerPower(uint32_t power) { jammerPower = power; }
    void setJammerInterval(uint32_t interval) { jammerInterval = interval; }
    
    // Spectrum Analysis
    bool startSpectrumScan(uint32_t startFreq, uint32_t endFreq, uint32_t step = 100000);
    bool stopSpectrumScan();
    bool isSpectrumActive() { return spectrumActive; }
    std::vector<uint32_t> getSpectrumData() { return spectrumData; }
    uint32_t getPeakFrequency();
    uint32_t getFrequencyPower(uint32_t frequency);
    
    // Custom SubGHz
    bool sendCustomSignal(uint32_t frequency, uint8_t* data, size_t length);
    bool sendCustomPattern(uint32_t frequency, String pattern);
    bool setCustomProtocol(String protocol);
    
    // Signal Management
    bool saveSignal(RFSignal& signal, String filename);
    bool loadSignal(String filename, RFSignal& signal);
    bool deleteSignal(String filename);
    std::vector<String> getSavedSignals();
    
    // Attack Management
    bool startAttack(String attackType, uint32_t frequency);
    bool stopAttack(String attackType);
    bool stopAllAttacks();
    std::vector<RFAttack> getActiveAttacks() { return activeAttacks; }
    
    // Configuration
    bool setTXPin(uint8_t pin);
    bool setRXPin(uint8_t pin);
    bool setModuleType(String type);
    bool setSensitivity(uint8_t sensitivity);
    
    // Information
    String getModuleInfo();
    String getScanResults();
    String getAttackStatus();
    String getSpectrumInfo();
    
    // Callbacks
    std::function<void(RFSignal signal)> onSignalCaptured;
    std::function<void(RFAttack attack)> onAttackStarted;
    std::function<void(RFAttack attack)> onAttackStopped;
    std::function<void(uint32_t frequency, int32_t power)> onSpectrumData;
    
    // Display integration
    void drawStatus();
    void drawSignals();
    void drawSpectrum();
    void drawAttacks();
};
