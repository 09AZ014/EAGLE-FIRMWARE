#pragma once

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <vector>
#include <functional>
#include "core/module_manager.h"

struct ESPNowPeer {
    uint8_t mac[6];
    String name;
    bool isConnected;
    uint32_t lastSeen;
    int32_t rssi;
    uint32_t packetsReceived;
    uint32_t packetsSent;
    String status;
};

struct ESPNowMessage {
    uint8_t* data;
    size_t length;
    uint8_t sender[6];
    uint8_t receiver[6];
    uint32_t timestamp;
    String type;
    String content;
    bool isEncrypted;
};

struct ESPNowFile {
    String filename;
    size_t size;
    uint8_t* data;
    uint32_t chunks;
    uint32_t currentChunk;
    bool isComplete;
    uint32_t timestamp;
    String sender;
};

class ESPNowModule : public IModule {
private:
    bool isInitialized;
    bool isScanning;
    bool isBroadcasting;
    bool isFileTransferActive;
    
    std::vector<ESPNowPeer> discoveredPeers;
    std::vector<ESPNowPeer> connectedPeers;
    std::vector<ESPNowMessage> receivedMessages;
    std::vector<ESPNowFile> receivedFiles;
    
    // File transfer
    ESPNowFile* currentFile;
    uint32_t fileTransferTimeout;
    uint32_t chunkSize;
    uint32_t maxFileSize;
    
    // Broadcasting
    String broadcastMessage;
    uint32_t broadcastInterval;
    uint32_t lastBroadcast;
    
    // Encryption
    bool encryptionEnabled;
    uint8_t encryptionKey[16];
    
    void initializeESPNow();
    void deinitializeESPNow();
    void processReceivedData(const uint8_t* mac, const uint8_t* data, int len);
    void processFileChunk(ESPNowMessage& message);
    void processCommand(ESPNowMessage& message);
    void processFileTransfer(ESPNowMessage& message);
    
    // Peer management
    void addPeer(uint8_t* mac, String name = "");
    void removePeer(uint8_t* mac);
    ESPNowPeer* getPeerByMAC(uint8_t* mac);
    ESPNowPeer* getPeerByName(String name);
    void updatePeerStatus(ESPNowPeer& peer);
    
    // Message handling
    String encodeMessage(ESPNowMessage& message);
    bool decodeMessage(uint8_t* data, size_t length, ESPNowMessage& message);
    String getMessageType(ESPNowMessage& message);
    String getMessageContent(ESPNowMessage& message);
    
    // File transfer
    bool startFileTransfer(String filename, size_t size, uint8_t* data);
    bool sendFileChunk(ESPNowFile& file, uint32_t chunk);
    bool receiveFileChunk(ESPNowMessage& message);
    bool completeFileTransfer(ESPNowFile& file);
    bool abortFileTransfer(ESPNowFile& file);
    
    // Encryption
    bool encryptData(uint8_t* data, size_t length);
    bool decryptData(uint8_t* data, size_t length);
    void generateEncryptionKey();
    bool setEncryptionKey(uint8_t* key);
    
public:
    ESPNowModule();
    ~ESPNowModule();
    
    const char* getName() const override { return "espnow"; }
    void setup() override;
    void loop() override;
    
    // Module Control
    bool initialize();
    bool deinitialize();
    bool isModuleReady() { return isInitialized; }
    
    // Peer Discovery
    bool startScan();
    bool stopScan();
    bool isScanningActive() { return isScanning; }
    std::vector<ESPNowPeer> getDiscoveredPeers() { return discoveredPeers; }
    void clearDiscoveredPeers();
    
    // Peer Management
    bool addPeer(String mac, String name = "");
    bool removePeer(String mac);
    bool connectToPeer(String mac);
    bool disconnectFromPeer(String mac);
    std::vector<ESPNowPeer> getConnectedPeers() { return connectedPeers; }
    ESPNowPeer* getPeerByMAC(String mac);
    ESPNowPeer* getPeerByName(String name);
    
    // Message Sending
    bool sendMessage(String mac, String message, String type = "text");
    bool sendData(String mac, uint8_t* data, size_t length);
    bool sendCommand(String mac, String command, String parameters = "");
    bool broadcastMessage(String message, String type = "broadcast");
    bool isBroadcastingActive() { return isBroadcasting; }
    
    // Message Receiving
    std::vector<ESPNowMessage> getReceivedMessages() { return receivedMessages; }
    void clearReceivedMessages();
    ESPNowMessage getLastMessage();
    String getMessageContent(ESPNowMessage& message);
    String getMessageType(ESPNowMessage& message);
    
    // File Transfer
    bool sendFile(String mac, String filename, uint8_t* data, size_t size);
    bool receiveFile(String filename);
    bool isFileTransferActive() { return isFileTransferActive; }
    std::vector<ESPNowFile> getReceivedFiles() { return receivedFiles; }
    ESPNowFile* getCurrentFile() { return currentFile; }
    
    // File Management
    bool saveFile(ESPNowFile& file, String path);
    bool loadFile(String path, ESPNowFile& file);
    bool deleteFile(String filename);
    std::vector<String> getFileList();
    
    // Command System
    bool sendCommand(String mac, String command, String parameters = "");
    bool executeCommand(String command, String parameters = "");
    String getCommandResponse(String command);
    bool isCommandSupported(String command);
    
    // Encryption
    bool enableEncryption();
    bool disableEncryption();
    bool isEncryptionEnabled() { return encryptionEnabled; }
    bool setEncryptionKey(String key);
    String getEncryptionKey();
    
    // Configuration
    bool setChannel(uint8_t channel);
    bool setScanInterval(uint32_t interval);
    bool setBroadcastInterval(uint32_t interval);
    bool setFileTransferTimeout(uint32_t timeout);
    bool setChunkSize(uint32_t size);
    bool setMaxFileSize(uint32_t size);
    
    // Information
    String getModuleInfo();
    String getPeerCount();
    String getMessageCount();
    String getFileCount();
    String getTransferStatus();
    
    // Callbacks
    std::function<void(ESPNowPeer peer)> onPeerDiscovered;
    std::function<void(ESPNowPeer peer)> onPeerConnected;
    std::function<void(ESPNowPeer peer)> onPeerDisconnected;
    std::function<void(ESPNowMessage message)> onMessageReceived;
    std::function<void(ESPNowFile file)> onFileReceived;
    std::function<void(String command, String parameters)> onCommandReceived;
    std::function<void(String error)> onError;
    
    // Display integration
    void drawStatus();
    void drawPeers();
    void drawMessages();
    void drawFiles();
    void drawTransferStatus();
};
