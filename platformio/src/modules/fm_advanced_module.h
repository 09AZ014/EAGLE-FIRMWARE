#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
#include "core/module_manager.h"

struct FMSignal {
    uint32_t frequency;
    int32_t rssi;
    String stationName;
    String rdsData;
    bool isStereo;
    bool isRDS;
    uint32_t timestamp;
    String description;
};

struct FMBroadcast {
    uint32_t frequency;
    String message;
    bool isActive;
    uint32_t startTime;
    uint32_t duration;
    uint8_t power;
    bool isStereo;
    String rdsData;
};

class FMAdvancedModule : public IModule {
private:
    bool isInitialized;
    bool isScanning;
    bool isBroadcasting;
    bool isSpectrumActive;
    
    std::vector<FMSignal> discoveredStations;
    std::vector<FMBroadcast> activeBroadcasts;
    std::vector<uint32_t> spectrumData;
    
    // FM Spectrum settings
    uint32_t spectrumStartFreq;
    uint32_t spectrumEndFreq;
    uint32_t spectrumStep;
    uint32_t spectrumInterval;
    
    // Broadcasting settings
    uint32_t broadcastFrequency;
    uint8_t broadcastPower;
    bool broadcastStereo;
    String broadcastRDS;
    
    // Traffic Announcement Hijacking
    bool trafficHijackActive;
    uint32_t trafficFrequency;
    String hijackMessage;
    
    void initializeFMModule();
    void scanFrequency(uint32_t frequency);
    void analyzeSignal(FMSignal& signal);
    void startBroadcast();
    void stopBroadcast();
    void startSpectrumScan();
    void stopSpectrumScan();
    void processRDSData(FMSignal& signal);
    void startTrafficHijack();
    void stopTrafficHijack();
    
    // Signal processing
    String decodeRDS(String rdsData);
    String encodeRDS(String message);
    bool isFrequencyValid(uint32_t frequency);
    bool isPowerValid(uint8_t power);
    
    // Station identification
    String identifyStation(FMSignal& signal);
    String getStationType(FMSignal& signal);
    bool isStationActive(FMSignal& signal);
    
public:
    FMAdvancedModule();
    ~FMAdvancedModule();
    
    const char* getName() const override { return "fm_advanced"; }
    void setup() override;
    void loop() override;
    
    // Module Control
    bool initialize();
    bool deinitialize();
    bool isModuleReady() { return isInitialized; }
    
    // Frequency Control
    bool setFrequency(uint32_t frequency);
    bool setPower(uint8_t power);
    bool setStereo(bool stereo);
    uint32_t getCurrentFrequency();
    uint8_t getCurrentPower();
    
    // Station Scanning
    bool startScan(uint32_t startFreq = 87500000, uint32_t endFreq = 108000000);
    bool stopScan();
    bool isScanningActive() { return isScanning; }
    std::vector<FMSignal> getDiscoveredStations() { return discoveredStations; }
    void clearDiscoveredStations();
    
    // Station Analysis
    FMSignal* getStationByFrequency(uint32_t frequency);
    FMSignal* getStationByName(String name);
    String analyzeStation(FMSignal& signal);
    bool isStationValid(FMSignal& signal);
    
    // Broadcasting
    bool startBroadcast(String message, uint32_t frequency, uint8_t power = 7);
    bool stopBroadcast();
    bool isBroadcastingActive() { return isBroadcasting; }
    void setBroadcastMessage(String message);
    void setBroadcastFrequency(uint32_t frequency);
    void setBroadcastPower(uint8_t power);
    void setBroadcastStereo(bool stereo);
    
    // RDS Broadcasting
    bool startRDSBroadcast(String message, String rdsData, uint32_t frequency);
    bool stopRDSBroadcast();
    bool isRDSBroadcasting();
    void setRDSData(String rdsData);
    String getRDSData();
    
    // Reserved Frequency Broadcasting
    bool startReservedBroadcast(String message, uint32_t frequency);
    bool stopReservedBroadcast();
    bool isReservedBroadcasting();
    
    // Spectrum Analysis
    bool startSpectrumScan(uint32_t startFreq, uint32_t endFreq, uint32_t step = 100000);
    bool stopSpectrumScan();
    bool isSpectrumActive() { return spectrumActive; }
    std::vector<uint32_t> getSpectrumData() { return spectrumData; }
    uint32_t getPeakFrequency();
    int32_t getFrequencyPower(uint32_t frequency);
    
    // Traffic Announcement Hijacking
    bool startTrafficHijack(uint32_t frequency, String message);
    bool stopTrafficHijack();
    bool isTrafficHijackActive() { return trafficHijackActive; }
    void setHijackMessage(String message);
    void setHijackFrequency(uint32_t frequency);
    
    // Station Management
    bool saveStation(FMSignal& station, String filename);
    bool loadStation(String filename, FMSignal& station);
    bool deleteStation(String filename);
    std::vector<String> getSavedStations();
    bool exportStations(String filename);
    bool importStations(String filename);
    
    // Configuration
    bool setScanInterval(uint32_t interval);
    bool setSpectrumInterval(uint32_t interval);
    bool setBroadcastInterval(uint32_t interval);
    bool setHijackInterval(uint32_t interval);
    
    // Information
    String getModuleInfo();
    String getScanResults();
    String getBroadcastStatus();
    String getSpectrumInfo();
    String getHijackStatus();
    
    // Callbacks
    std::function<void(FMSignal station)> onStationDiscovered;
    std::function<void(FMBroadcast broadcast)> onBroadcastStarted;
    std::function<void(FMBroadcast broadcast)> onBroadcastStopped;
    std::function<void(uint32_t frequency, int32_t power)> onSpectrumData;
    std::function<void(String error)> onError;
    
    // Display integration
    void drawStatus();
    void drawStations();
    void drawBroadcasts();
    void drawSpectrum();
    void drawHijackStatus();
};
