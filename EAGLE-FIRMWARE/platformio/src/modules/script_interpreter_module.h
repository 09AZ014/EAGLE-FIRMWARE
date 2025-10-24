#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
#include <map>
#include "core/module_manager.h"

struct Script {
    String name;
    String content;
    String language;
    bool isRunning;
    uint32_t startTime;
    uint32_t duration;
    String status;
    String output;
    String error;
};

struct BadUSBScript {
    String name;
    String content;
    bool isRunning;
    uint32_t startTime;
    uint32_t duration;
    String status;
    String output;
    String error;
};

class ScriptInterpreterModule : public IModule {
private:
    bool isInitialized;
    bool isJavaScriptEnabled;
    bool isBadUSBEnabled;
    
    std::vector<Script> loadedScripts;
    std::vector<BadUSBScript> badUSBScripts;
    Script* currentScript;
    BadUSBScript* currentBadUSBScript;
    
    // JavaScript interpreter
    void* jsContext;
    void* jsGlobal;
    bool jsInitialized;
    
    // BadUSB functionality
    bool badUSBActive;
    uint32_t badUSBDelay;
    String badUSBOutput;
    
    // Script management
    void initializeJavaScript();
    void deinitializeJavaScript();
    void initializeBadUSB();
    void deinitializeBadUSB();
    
    // JavaScript execution
    bool executeJavaScript(String code);
    String getJavaScriptOutput();
    String getJavaScriptError();
    bool isJavaScriptRunning();
    
    // BadUSB execution
    bool executeBadUSB(String script);
    String getBadUSBOutput();
    String getBadUSBError();
    bool isBadUSBRunning();
    
    // Script parsing
    String parseScript(String content);
    String validateScript(String content, String language);
    bool isScriptValid(String content, String language);
    
    // BadUSB parsing
    String parseBadUSBScript(String content);
    String validateBadUSBScript(String content);
    bool isBadUSBScriptValid(String content);
    
    // File management
    bool saveScript(Script& script, String filename);
    bool loadScript(String filename, Script& script);
    bool deleteScript(String filename);
    std::vector<String> getScriptFiles();
    
    bool saveBadUSBScript(BadUSBScript& script, String filename);
    bool loadBadUSBScript(String filename, BadUSBScript& script);
    bool deleteBadUSBScript(String filename);
    std::vector<String> getBadUSBScriptFiles();
    
    // BadUSB commands
    void executeBadUSBCommand(String command);
    void delayCommand(uint32_t delay);
    void keyPressCommand(String key);
    void keyReleaseCommand(String key);
    void keyTypeCommand(String text);
    void mouseMoveCommand(int x, int y);
    void mouseClickCommand(String button);
    void mousePressCommand(String button);
    void mouseReleaseCommand(String button);
    void scrollCommand(int x, int y);
    
public:
    ScriptInterpreterModule();
    ~ScriptInterpreterModule();
    
    const char* getName() const override { return "script_interpreter"; }
    void setup() override;
    void loop() override;
    
    // Module Control
    bool initialize();
    bool deinitialize();
    bool isModuleReady() { return isInitialized; }
    
    // JavaScript Interpreter
    bool enableJavaScript();
    bool disableJavaScript();
    bool isJavaScriptEnabled() { return isJavaScriptEnabled; }
    
    bool executeScript(String name, String content);
    bool stopScript(String name);
    bool stopAllScripts();
    std::vector<Script> getLoadedScripts() { return loadedScripts; }
    Script* getCurrentScript() { return currentScript; }
    
    // BadUSB
    bool enableBadUSB();
    bool disableBadUSB();
    bool isBadUSBEnabled() { return isBadUSBEnabled; }
    
    bool executeBadUSB(String name, String content);
    bool stopBadUSB(String name);
    bool stopAllBadUSB();
    std::vector<BadUSBScript> getBadUSBScripts() { return badUSBScripts; }
    BadUSBScript* getCurrentBadUSB() { return currentBadUSBScript; }
    
    // Script Management
    bool loadScript(String filename);
    bool saveScript(String filename, String content, String language = "javascript");
    bool deleteScript(String filename);
    std::vector<String> getAvailableScripts();
    Script* getScriptByName(String name);
    
    // BadUSB Management
    bool loadBadUSBScript(String filename);
    bool saveBadUSBScript(String filename, String content);
    bool deleteBadUSBScript(String filename);
    std::vector<String> getAvailableBadUSBScripts();
    BadUSBScript* getBadUSBScriptByName(String name);
    
    // Script Execution
    bool runScript(String name);
    bool pauseScript(String name);
    bool resumeScript(String name);
    bool restartScript(String name);
    
    // BadUSB Execution
    bool runBadUSB(String name);
    bool pauseBadUSB(String name);
    bool resumeBadUSB(String name);
    bool restartBadUSB(String name);
    
    // Script Information
    String getScriptStatus(String name);
    String getScriptOutput(String name);
    String getScriptError(String name);
    uint32_t getScriptRuntime(String name);
    
    // BadUSB Information
    String getBadUSBStatus(String name);
    String getBadUSBOutput(String name);
    String getBadUSBError(String name);
    uint32_t getBadUSBRuntime(String name);
    
    // Script Validation
    bool validateScript(String content, String language);
    String getValidationError(String content, String language);
    bool isScriptSafe(String content, String language);
    
    // BadUSB Validation
    bool validateBadUSBScript(String content);
    String getBadUSBValidationError(String content);
    bool isBadUSBScriptSafe(String content);
    
    // Configuration
    bool setBadUSBDelay(uint32_t delay);
    bool setScriptTimeout(uint32_t timeout);
    bool setMaxScripts(uint32_t max);
    bool setMaxBadUSBScripts(uint32_t max);
    
    // Information
    String getModuleInfo();
    String getScriptCount();
    String getBadUSBCount();
    String getActiveScripts();
    String getActiveBadUSB();
    
    // Callbacks
    std::function<void(Script script)> onScriptStarted;
    std::function<void(Script script)> onScriptStopped;
    std::function<void(Script script)> onScriptError;
    std::function<void(BadUSBScript script)> onBadUSBStarted;
    std::function<void(BadUSBScript script)> onBadUSBStopped;
    std::function<void(BadUSBScript script)> onBadUSBError;
    std::function<void(String output)> onScriptOutput;
    std::function<void(String output)> onBadUSBOutput;
    
    // Display integration
    void drawStatus();
    void drawScripts();
    void drawBadUSB();
    void drawOutput();
};
