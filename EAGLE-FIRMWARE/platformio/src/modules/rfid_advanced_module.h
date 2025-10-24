#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <vector>
#include <functional>
#include "core/module_manager.h"

// RFID Module types
#define RFID_PN532 1
#define RFID_PN532KILLER 2
#define RFID_MFRC522 3
#define RFID_CHAMELEON 4
#define RFID_AMIIBOLINK 5

struct RFIDTag {
    String uid;
    String type;
    String manufacturer;
    uint8_t* data;
    size_t dataLength;
    bool isWritable;
    bool isEmulatable;
    String protocol;
    uint32_t timestamp;
};

struct NDEFRecord {
    String type;
    String payload;
    String id;
    bool isUri;
    bool isText;
    bool isMime;
};

class RFIDAdvancedModule : public IModule {
private:
    uint8_t moduleType;
    bool isInitialized;
    bool isScanning;
    bool isEmulating;
    bool isWriting;
    
    std::vector<RFIDTag> discoveredTags;
    std::vector<RFIDTag> savedTags;
    RFIDTag* currentTag;
    RFIDTag* emulatingTag;
    
    // PN532 specific
    uint8_t pn532SDA;
    uint8_t pn532SCL;
    uint8_t pn532IRQ;
    uint8_t pn532RST;
    
    // MFRC522 specific
    uint8_t mfrc522SS;
    uint8_t mfrc522RST;
    
    // Chameleon specific
    uint8_t chameleonTX;
    uint8_t chameleonRX;
    
    // Amiibolink specific
    uint8_t amiibolinkTX;
    uint8_t amiibolinkRX;
    
    void initializePN532();
    void initializeMFRC522();
    void initializeChameleon();
    void initializeAmiibolink();
    
    bool readPN532Tag();
    bool readMFRC522Tag();
    bool readChameleonTag();
    bool readAmiibolinkTag();
    
    bool writePN532Tag(RFIDTag& tag);
    bool writeMFRC522Tag(RFIDTag& tag);
    bool writeChameleonTag(RFIDTag& tag);
    bool writeAmiibolinkTag(RFIDTag& tag);
    
    bool emulatePN532Tag(RFIDTag& tag);
    bool emulateChameleonTag(RFIDTag& tag);
    bool emulateAmiibolinkTag(RFIDTag& tag);
    
    void processTag(RFIDTag& tag);
    String identifyTagType(RFIDTag& tag);
    String getManufacturer(RFIDTag& tag);
    bool isTagWritable(RFIDTag& tag);
    bool isTagEmulatable(RFIDTag& tag);
    
    // NDEF handling
    std::vector<NDEFRecord> parseNDEF(RFIDTag& tag);
    bool writeNDEF(RFIDTag& tag, std::vector<NDEFRecord>& records);
    String createNDEFUri(String uri);
    String createNDEFText(String text, String language = "en");
    String createNDEFMime(String mimeType, String data);
    
    // Data management
    bool saveTagToFile(RFIDTag& tag, String filename);
    bool loadTagFromFile(String filename, RFIDTag& tag);
    bool deleteTagFile(String filename);
    std::vector<String> getTagFiles();
    
public:
    RFIDAdvancedModule();
    ~RFIDAdvancedModule();
    
    const char* getName() const override { return "rfid_advanced"; }
    void setup() override;
    void loop() override;
    
    // Module Control
    bool initialize(uint8_t type = RFID_PN532);
    bool deinitialize();
    bool isModuleReady() { return isInitialized; }
    void setModuleType(uint8_t type) { moduleType = type; }
    
    // Pin Configuration
    bool setPN532Pins(uint8_t sda, uint8_t scl, uint8_t irq = 0, uint8_t rst = 0);
    bool setMFRC522Pins(uint8_t ss, uint8_t rst);
    bool setChameleonPins(uint8_t tx, uint8_t rx);
    bool setAmiibolinkPins(uint8_t tx, uint8_t rx);
    
    // Tag Reading
    bool startScan();
    bool stopScan();
    bool isScanningActive() { return isScanning; }
    std::vector<RFIDTag> getDiscoveredTags() { return discoveredTags; }
    void clearDiscoveredTags();
    
    bool readTag();
    RFIDTag* getCurrentTag() { return currentTag; }
    String getTagUID();
    String getTagType();
    String getTagManufacturer();
    bool isTagPresent();
    
    // Tag Writing
    bool writeTag(RFIDTag& tag);
    bool writeTagData(uint8_t* data, size_t length);
    bool writeTagString(String data);
    bool isWriting() { return isWriting; }
    
    // Tag Emulation
    bool startEmulation(RFIDTag& tag);
    bool stopEmulation();
    bool isEmulating() { return isEmulating; }
    RFIDTag* getEmulatingTag() { return emulatingTag; }
    
    // Tag Cloning
    bool cloneTag(RFIDTag& sourceTag, RFIDTag& targetTag);
    bool cloneTagToFile(RFIDTag& tag, String filename);
    bool cloneTagFromFile(String filename, RFIDTag& tag);
    
    // NDEF Operations
    std::vector<NDEFRecord> readNDEFRecords();
    bool writeNDEFRecords(std::vector<NDEFRecord>& records);
    bool writeNDEFUri(String uri);
    bool writeNDEFText(String text, String language = "en");
    bool writeNDEFMime(String mimeType, String data);
    
    // Data Management
    bool saveTag(RFIDTag& tag, String filename);
    bool loadTag(String filename, RFIDTag& tag);
    bool deleteTag(String filename);
    std::vector<String> getSavedTags();
    bool exportTags(String filename);
    bool importTags(String filename);
    
    // Tag Analysis
    String analyzeTag(RFIDTag& tag);
    String getTagInfo(RFIDTag& tag);
    bool isTagValid(RFIDTag& tag);
    bool isTagSupported(RFIDTag& tag);
    
    // 125kHz Support
    bool read125kHzTag();
    bool write125kHzTag(RFIDTag& tag);
    bool emulate125kHzTag(RFIDTag& tag);
    
    // Amiibolink/Chameleon Specific
    bool setAmiibolinkMode(String mode);
    bool setChameleonMode(String mode);
    bool loadAmiiboFile(String filename);
    bool saveAmiiboFile(String filename);
    
    // Configuration
    bool setReadPower(uint8_t power);
    bool setWritePower(uint8_t power);
    bool setAntennaGain(uint8_t gain);
    bool setScanInterval(uint32_t interval);
    
    // Information
    String getModuleInfo();
    String getScanResults();
    String getTagInfo();
    String getEmulationStatus();
    
    // Callbacks
    std::function<void(RFIDTag tag)> onTagDiscovered;
    std::function<void(RFIDTag tag)> onTagRead;
    std::function<void(RFIDTag tag)> onTagWritten;
    std::function<void(RFIDTag tag)> onTagEmulated;
    std::function<void(String error)> onError;
    
    // Display integration
    void drawStatus();
    void drawTags();
    void drawCurrentTag();
    void drawEmulationStatus();
};
