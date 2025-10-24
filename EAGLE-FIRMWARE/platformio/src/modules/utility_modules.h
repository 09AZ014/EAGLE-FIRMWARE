#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
#include "core/module_manager.h"

// Microphone Spectrum Module
class MicrophoneSpectrumModule : public IModule {
private:
    bool isInitialized;
    bool isRecording;
    uint8_t micPin;
    uint32_t sampleRate;
    uint32_t sampleCount;
    std::vector<int16_t> audioBuffer;
    std::vector<float> spectrumData;
    uint32_t lastUpdate;
    
public:
    MicrophoneSpectrumModule();
    ~MicrophoneSpectrumModule();
    
    const char* getName() const override { return "mic_spectrum"; }
    void setup() override;
    void loop() override;
    
    bool initialize(uint8_t pin);
    bool startRecording();
    bool stopRecording();
    bool isRecordingActive() { return isRecording; }
    std::vector<float> getSpectrumData() { return spectrumData; }
    void clearSpectrumData();
    String getSpectrumInfo();
};

// QR Code Module
class QRCodeModule : public IModule {
private:
    bool isInitialized;
    String lastQRCode;
    uint32_t lastScanTime;
    
public:
    QRCodeModule();
    ~QRCodeModule();
    
    const char* getName() const override { return "qrcode"; }
    void setup() override;
    void loop() override;
    
    bool initialize();
    String generateQRCode(String data);
    String scanQRCode();
    bool isQRCodeDetected();
    String getLastQRCode() { return lastQRCode; }
    void clearQRCode();
};

// SD Card Manager Module
class SDCardManagerModule : public IModule {
private:
    bool isInitialized;
    bool isMounted;
    String mountPath;
    uint32_t totalSize;
    uint32_t usedSize;
    uint32_t freeSize;
    
public:
    SDCardManagerModule();
    ~SDCardManagerModule();
    
    const char* getName() const override { return "sd_manager"; }
    void setup() override;
    void loop() override;
    
    bool initialize();
    bool mount(String path = "/sd");
    bool unmount();
    bool isMountedActive() { return isMounted; }
    bool fileExists(String filename);
    bool createFile(String filename);
    bool deleteFile(String filename);
    bool writeFile(String filename, String content);
    String readFile(String filename);
    std::vector<String> listFiles(String path = "/");
    uint32_t getFileSize(String filename);
    String getStorageInfo();
};

// LittleFS Manager Module
class LittleFSManagerModule : public IModule {
private:
    bool isInitialized;
    bool isMounted;
    String mountPath;
    uint32_t totalSize;
    uint32_t usedSize;
    uint32_t freeSize;
    
public:
    LittleFSManagerModule();
    ~LittleFSManagerModule();
    
    const char* getName() const override { return "littlefs_manager"; }
    void setup() override;
    void loop() override;
    
    bool initialize();
    bool mount(String path = "/littlefs");
    bool unmount();
    bool isMountedActive() { return isMounted; }
    bool fileExists(String filename);
    bool createFile(String filename);
    bool deleteFile(String filename);
    bool writeFile(String filename, String content);
    String readFile(String filename);
    std::vector<String> listFiles(String path = "/");
    uint32_t getFileSize(String filename);
    String getStorageInfo();
};

// RTC Module
class RTCModule : public IModule {
private:
    bool isInitialized;
    bool isNTPEnabled;
    String ntpServer;
    int32_t timezoneOffset;
    uint32_t lastSync;
    uint32_t syncInterval;
    
public:
    RTCModule();
    ~RTCModule();
    
    const char* getName() const override { return "rtc"; }
    void setup() override;
    void loop() override;
    
    bool initialize();
    bool enableNTP(String server = "pool.ntp.org");
    bool disableNTP();
    bool isNTPEnabled() { return isNTPEnabled; }
    String getCurrentTime();
    String getCurrentDate();
    uint32_t getUnixTime();
    bool setTime(uint32_t unixTime);
    bool setTimezone(int32_t offset);
    String getTimezone();
    bool syncWithNTP();
    String getTimeInfo();
};

// Web UI Module
class WebUIModule : public IModule {
private:
    bool isInitialized;
    bool isServerActive;
    String serverIP;
    uint16_t serverPort;
    String serverPath;
    
public:
    WebUIModule();
    ~WebUIModule();
    
    const char* getName() const override { return "webui"; }
    void setup() override;
    void loop() override;
    
    bool initialize();
    bool startServer(uint16_t port = 80);
    bool stopServer();
    bool isServerActive() { return isServerActive; }
    String getServerURL();
    String getServerIP() { return serverIP; }
    uint16_t getServerPort() { return serverPort; }
    String getServerInfo();
};

// PIX System Module (Brazilian payment system)
class PIXModule : public IModule {
private:
    bool isInitialized;
    String pixKey;
    String pixData;
    String qrCodeData;
    
public:
    PIXModule();
    ~PIXModule();
    
    const char* getName() const override { return "pix"; }
    void setup() override;
    void loop() override;
    
    bool initialize();
    bool generatePIX(String key, float amount, String description = "");
    String getPIXQRCode();
    String getPIXData() { return pixData; }
    bool validatePIXKey(String key);
    String getPIXInfo();
};
