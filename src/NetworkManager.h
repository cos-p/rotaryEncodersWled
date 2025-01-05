#pragma once

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#include "StateManager.h"

class NetworkManager {
public:
    NetworkManager();
    bool begin();
    void setupWebServer(StateManager& stateManager);  

private:
    AsyncWebServer server;
    StateManager* stateManagerPtr; 
    bool setupWiFi();
};

NetworkManager::NetworkManager() : server(80), stateManagerPtr(nullptr) {}

bool NetworkManager::begin() {
    return setupWiFi();
}

bool NetworkManager::setupWiFi() {
    Serial.println("\nConnecting to WiFi...");
    Serial.printf("SSID: %s\n", NetworkConfig::WIFI_SSID);
    Serial.printf("Static IP: %s\n", NetworkConfig::STATIC_IP);
    
    WiFi.mode(WIFI_STA);
    
    // Configure static IP
    IPAddress local_ip;
    IPAddress gateway_ip;
    IPAddress subnet_mask;
    local_ip.fromString(NetworkConfig::STATIC_IP);
    gateway_ip.fromString(NetworkConfig::GATEWAY);
    subnet_mask.fromString(NetworkConfig::SUBNET);
    
    if (!WiFi.config(local_ip, gateway_ip, subnet_mask)) {
        Serial.println("Static IP Configuration Failed");
        return false;
    }
    
    WiFi.begin(NetworkConfig::WIFI_SSID, NetworkConfig::WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected successfully!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    
    Serial.println("\nWiFi connection failed!");
    return false;
}

void NetworkManager::setupWebServer(StateManager& stateManager) {
    stateManagerPtr = &stateManager;  // Store the reference
    server.on("/api/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
        JsonDocument doc; 
        const auto& color = stateManagerPtr->getColorState();
        doc["red"] = color.red;
        doc["green"] = color.green;
        doc["blue"] = color.blue;
        doc["effect"] = Effects::NAMES[stateManagerPtr->getEffectIndex()];
        doc["effect_index"] = stateManagerPtr->getEffectIndex();
        
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });

    server.begin();
}