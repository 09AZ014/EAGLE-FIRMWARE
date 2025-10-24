#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <vector>
#include <functional>
#include "core/module_manager.h"

struct NRF24Packet {
    uint8_t* data;
    size_t length;
    uint8_t pipe;
    uint32_t timestamp;
    int32_t rssi;
    String protocol;
    String description;
};

struct NRF24Device {
    String address;
    String name;
    uint8_t channel;
    int32_t rssi;
    uint32_t packetsReceived;
    uint32_t lastSeen;
    String protocol;
    bool isActive;
};

struct NRF24Attack {
    String attackType; // "jammer", "mousejack", "spectrum"
    uint8_t channel;
    bool isRunning;
    uint32_t packetsSent;
    uint32_t startTime;
    String targetAddress;
};

class NRF24AdvancedModule : public IModule {
private:
    RF24* radio;
    bool isInitialized;
    bool isScanning;
    bool isJamming;
    bool isSpectrumActive;
    bool isMousejackActive;
    
    std::vector<NRF24Packet> capturedPackets;
    std::vector<NRF24Device> discoveredDevices;
    std::vector<NRF24Attack> activeAttacks;
    std::vector<uint32_t> spectrumData;
    
    // Pin configuration
    uint8_t cePin;
    uint8_t csnPin;
    uint8_t sckPin;
    uint8_t mosiPin;
    uint8_t misoPin;
    
    // Scanning settings
    uint8_t scanChannel;
    uint8_t scanChannels[16];
    uint32_t scanInterval;
    uint32_t scanTimeout;
    
    // Jamming settings
    uint8_t jammerChannel;
    uint8_t jammerPower;
    bool jammerContinuous;
    uint32_t jammerInterval;
    
    // Mousejack settings
    String mousejackAddress;
    uint8_t mousejackChannel;
    bool mousejackActive;
    
    // Spectrum settings
    uint8_t spectrumStartChannel;
    uint8_t spectrumEndChannel;
    uint32_t spectrumInterval;
    
    void initializeNRF24();
    void scanChannel(uint8_t channel);
    void processPacket(NRF24Packet& packet);
    void identifyDevice(NRF24Device& device);
    void startJammer();
    void stopJammer();
    void startMousejack();
    void stopMousejack();
    void startSpectrumScan();
    void stopSpectrumScan();
    
    // Protocol identification
    String identifyProtocol(NRF24Packet& packet);
    String getProtocolDescription(String protocol);
    bool isMouseDevice(NRF24Device& device);
    bool isKeyboardDevice(NRF24Device& device);
    bool isGamepadDevice(NRF24Device& device);
    
    // Packet analysis
    String analyzePacket(NRF24Packet& packet);
    String decodeMousePacket(NRF24Packet& packet);
    String decodeKeyboardPacket(NRF24Packet& packet);
    String decodeGamepadPacket(NRF24Packet& packet);
    
    // Attack implementations
    void executeMousejackAttack(NRF24Device& device);
    void executeJammingAttack(uint8_t channel);
    void executeSpectrumAttack();
    
public:
    NRF24AdvancedModule();
    ~NRF24AdvancedModule();
    
    const char* getName() const override { return "nrf24_advanced"; }
    void setup() override;
    void loop() override;
    
    // Module Control
    bool initialize(uint8_t ce, uint8_t csn);
    bool deinitialize();
    bool isModuleReady() { return isInitialized; }
    
    // Pin Configuration
    bool setPins(uint8_t ce, uint8_t csn, uint8_t sck = 0, uint8_t mosi = 0, uint8_t miso = 0);
    bool setSPIPins(uint8_t sck, uint8_t mosi, uint8_t miso);
    
    // Channel Control
    bool setChannel(uint8_t channel);
    bool setPower(uint8_t power);
    bool setDataRate(uint8_t dataRate);
    uint8_t getCurrentChannel();
    uint8_t getCurrentPower();
    uint8_t getCurrentDataRate();
    
    // Device Scanning
    bool startScan();
    bool stopScan();
    bool isScanningActive() { return isScanning; }
    std::vector<NRF24Device> getDiscoveredDevices() { return discoveredDevices; }
    void clearDiscoveredDevices();
    
    // Packet Capture
    bool startCapture();
    bool stopCapture();
    bool isCapturing();
    std::vector<NRF24Packet> getCapturedPackets() { return capturedPackets; }
    void clearCapturedPackets();
    NRF24Packet getLastPacket();
    
    // Device Analysis
    NRF24Device* getDeviceByAddress(String address);
    NRF24Device* getDeviceByName(String name);
    String analyzeDevice(NRF24Device& device);
    bool isDeviceActive(NRF24Device& device);
    
    // Jamming
    bool startJammer(uint8_t channel, uint8_t power = 7);
    bool startIntermittentJammer(uint8_t channel, uint32_t interval = 1000);
    bool stopJammer();
    bool isJamming() { return isJamming; }
    void setJammerPower(uint8_t power) { jammerPower = power; }
    void setJammerInterval(uint32_t interval) { jammerInterval = interval; }
    
    // Mousejack Attack
    bool startMousejack(String address, uint8_t channel);
    bool stopMousejack();
    bool isMousejackActive() { return isMousejackActive; }
    void setMousejackAddress(String address);
    void setMousejackChannel(uint8_t channel);
    
    // 2.4G Spectrum Scanner
    bool startSpectrumScan(uint8_t startChannel = 0, uint8_t endChannel = 125);
    bool stopSpectrumScan();
    bool isSpectrumActive() { return spectrumActive; }
    std::vector<uint32_t> getSpectrumData() { return spectrumData; }
    uint8_t getPeakChannel();
    int32_t getChannelPower(uint8_t channel);
    
    // Packet Transmission
    bool transmitPacket(uint8_t* data, size_t length, String address);
    bool transmitMouseCommand(String command);
    bool transmitKeyboardCommand(String command);
    bool transmitGamepadCommand(String command);
    
    // Device Management
    bool saveDevice(NRF24Device& device, String filename);
    bool loadDevice(String filename, NRF24Device& device);
    bool deleteDevice(String filename);
    std::vector<String> getSavedDevices();
    bool exportDevices(String filename);
    bool importDevices(String filename);
    
    // Attack Management
    bool startAttack(String attackType, String target = "");
    bool stopAttack(String attackType);
    bool stopAllAttacks();
    std::vector<NRF24Attack> getActiveAttacks() { return activeAttacks; }
    
    // Configuration
    bool setScanChannels(uint8_t* channels, size_t count);
    bool setScanInterval(uint32_t interval);
    bool setScanTimeout(uint32_t timeout);
    bool setSpectrumInterval(uint32_t interval);
    
    // Information
    String getModuleInfo();
    String getScanResults();
    String getAttackStatus();
    String getSpectrumInfo();
    String getDeviceCount();
    
    // Callbacks
    std::function<void(NRF24Device device)> onDeviceDiscovered;
    std::function<void(NRF24Packet packet)> onPacketCaptured;
    std::function<void(NRF24Attack attack)> onAttackStarted;
    std::function<void(NRF24Attack attack)> onAttackStopped;
    std::function<void(uint8_t channel, int32_t power)> onSpectrumData;
    std::function<void(String error)> onError;
    
    // Display integration
    void drawStatus();
    void drawDevices();
    void drawPackets();
    void drawAttacks();
    void drawSpectrum();
};
