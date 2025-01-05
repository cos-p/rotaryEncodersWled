#pragma once

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

class WLEDController {
public:
    void updateColor(int red, int green, int blue);
    void updateEffect(int effectIndex);

private:
    HTTPClient http;
    
    void sendRequest(const String& jsonString);
};

void WLEDController::updateColor(int red, int green, int blue) {
    if (WiFi.status() != WL_CONNECTED) return;

    String url = String("http://") + NetworkConfig::WLED_IP + "/json/state";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    JsonDocument doc;  // Changed from StaticJsonDocument
    doc["on"] = true;
    doc["bri"] = 255;  // Full brightness
    
    // New syntax for nested arrays and objects
    JsonArray seg = doc["seg"].to<JsonArray>();
    JsonObject segment = seg.add<JsonObject>();
    JsonArray col = segment["col"].to<JsonArray>();
    JsonArray color = col.add<JsonArray>();
    
    color.add(red);
    color.add(green);
    color.add(blue);
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    sendRequest(jsonString);
}

void WLEDController::updateEffect(int effectIndex) {
    if (WiFi.status() != WL_CONNECTED) return;

    String url = String("http://") + NetworkConfig::WLED_IP + "/json/state";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    JsonDocument doc;  
    JsonArray seg = doc["seg"].to<JsonArray>();
    JsonObject segment = seg.add<JsonObject>();
    segment["fx"] = effectIndex;
    doc["on"] = true;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    sendRequest(jsonString);
}

void WLEDController::sendRequest(const String& jsonString) {
    unsigned long startTime = millis();
    
    Serial.println("WLED Request Start --------");
    Serial.println("Sending: " + jsonString);
    
    int httpResponseCode = http.POST(jsonString);
    unsigned long duration = millis() - startTime;
    
    Serial.printf("Request took: %lu ms\n", duration);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Response: " + response);
    } else {
        Serial.printf("Error %d: %s\n", httpResponseCode, http.errorToString(httpResponseCode).c_str());
    }
    Serial.println("WLED Request End ----------\n");
    
    http.end();
}