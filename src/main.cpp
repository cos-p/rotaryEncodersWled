#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "DisplayHandler.h"
#include "WLEDController.h"
#include "NetworkManager.h"
#include "ButtonEventQueue.h"
#include "StateManager.h"


// Global instances
ButtonEventQueue buttonQueue;
StateManager stateManager;
DisplayHandler display;
WLEDController wled;
NetworkManager network;

// State tracking
volatile bool encoderEvent = false;
volatile int8_t encoderChanges[4] = {0};  // RED, GREEN, BLUE, EFFECT
volatile uint8_t prevEncoderStates[4] = {0};  // RED, GREEN, BLUE, EFFECT
volatile bool buttonStates[4] = {HIGH, HIGH, HIGH, HIGH};  // RED, GREEN, BLUE, EFFECT
unsigned long lastButtonPress[4] = {0};  // Timestamps for button debouncing

// Encoder interrupt handler
void IRAM_ATTR handleEncoder(uint8_t pinA, uint8_t pinB, volatile uint8_t& prevState, int encoderIndex) {
    static const int8_t encoder_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    
    prevState = (prevState << 2) & 0x0F;
    prevState |= (digitalRead(pinA) << 1) | digitalRead(pinB);
    
    int8_t change = encoder_states[prevState];
    if (change != 0) {
        encoderChanges[encoderIndex] += change;
        encoderEvent = true;
    }
}

// Individual encoder interrupt handlers
void IRAM_ATTR handleRedEncoder() { handleEncoder(Pins::RED_A, Pins::RED_B, prevEncoderStates[0], 0); }
void IRAM_ATTR handleGreenEncoder() { handleEncoder(Pins::GREEN_A, Pins::GREEN_B, prevEncoderStates[1], 1); }
void IRAM_ATTR handleBlueEncoder() { handleEncoder(Pins::BLUE_A, Pins::BLUE_B, prevEncoderStates[2], 2); }
void IRAM_ATTR handleEffectEncoder() { handleEncoder(Pins::EFFECT_A, Pins::EFFECT_B, prevEncoderStates[3], 3); }

// Generic button handler
void IRAM_ATTR handleButton(uint8_t pin, uint8_t buttonIndex, uint8_t buttonId) {
    bool currentState = digitalRead(pin);
    unsigned long currentTime = millis();
    
    if (currentTime - lastButtonPress[buttonIndex] > Timing::DEBOUNCE_DELAY) {
        if (currentState == LOW && buttonStates[buttonIndex] == HIGH) {
            delayMicroseconds(10);  // Short verification delay
            if (digitalRead(pin) == LOW) {
                buttonQueue.push(buttonId, true, currentTime);
                lastButtonPress[buttonIndex] = currentTime;
            }
        }
        buttonStates[buttonIndex] = currentState;
    }
}

// Individual button interrupt handlers
void IRAM_ATTR handleRedButton() { handleButton(Pins::RED_BUTTON, 0, 1); }
void IRAM_ATTR handleGreenButton() { handleButton(Pins::GREEN_BUTTON, 1, 2); }
void IRAM_ATTR handleBlueButton() { handleButton(Pins::BLUE_BUTTON, 2, 3); }
void IRAM_ATTR handleEffectButton() { 
    handleButton(Pins::EFFECT_BUTTON, 3, 4);
}

void setupPins() {
    // Configure encoder pins
    const uint8_t encoderPins[] = {
        Pins::RED_A, Pins::RED_B, Pins::GREEN_A, Pins::GREEN_B,
        Pins::BLUE_A, Pins::BLUE_B, Pins::EFFECT_A, Pins::EFFECT_B
    };
    
    for (uint8_t pin : encoderPins) {
        pinMode(pin, INPUT_PULLUP);
    }

    // Configure button pins
    const uint8_t buttonPins[] = {
        Pins::RED_BUTTON, Pins::GREEN_BUTTON, 
        Pins::BLUE_BUTTON, Pins::EFFECT_BUTTON
    };
    
    for (uint8_t pin : buttonPins) {
        pinMode(pin, INPUT_PULLUP);
    }

    // Attach encoder interrupts
    attachInterrupt(digitalPinToInterrupt(Pins::RED_A), handleRedEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::RED_B), handleRedEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::GREEN_A), handleGreenEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::GREEN_B), handleGreenEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::BLUE_A), handleBlueEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::BLUE_B), handleBlueEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::EFFECT_A), handleEffectEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::EFFECT_B), handleEffectEncoder, CHANGE);

    // Attach button interrupts
    attachInterrupt(digitalPinToInterrupt(Pins::RED_BUTTON), handleRedButton, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::GREEN_BUTTON), handleGreenButton, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::BLUE_BUTTON), handleBlueButton, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Pins::EFFECT_BUTTON), handleEffectButton, CHANGE);
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    DEBUG_PRINTLN("\n\nStarting RGB Controller...");
    DEBUG_PRINTLN("Version: " __DATE__ " " __TIME__);

    if (!display.begin()) {
        DEBUG_PRINTLN("Display initialization failed!");
        while (1) delay(100);
    }
    DEBUG_PRINTLN("Display initialized successfully");

    setupPins();
    DEBUG_PRINTLN("Pins configured successfully");

    if (!network.begin()) {
        DEBUG_PRINTLN("Network initialization failed! Continuing with local display only.");
    }
   
    network.setupWebServer(stateManager);
    DEBUG_PRINTLN("Initialization complete!");
    DEBUG_PRINTF("Current WLED IP: %s\n", NetworkConfig::WLED_IP);
}

void processEncoders() {
    if (!encoderEvent) return;
    
    noInterrupts();
    bool colorChanged = false;
    
    // Process RGB encoders
    if (encoderChanges[0] != 0 || encoderChanges[1] != 0 || encoderChanges[2] != 0) {
        stateManager.adjustColor(
            encoderChanges[0] * 5,
            encoderChanges[1] * 5,
            encoderChanges[2] * 5
        );
        encoderChanges[0] = encoderChanges[1] = encoderChanges[2] = 0;
        colorChanged = true;
    }
    
    // Process effect encoder
    if (encoderChanges[3] != 0) {
        stateManager.adjustEffect(encoderChanges[3]);
        encoderChanges[3] = 0;
    }
    
    encoderEvent = false;
    interrupts();
    
    if (colorChanged) {
        const auto& color = stateManager.getColorState();
        DEBUG_PRINTF("Color Update - R:%d G:%d B:%d\n", 
            color.red, color.green, color.blue);
    }
}

void processButtons() {
    ButtonEvent event;
    while (buttonQueue.pop(&event)) {
        if (!event.pressed) continue;
        
        DEBUG_PRINTF("Processing button %d press\n", event.button);
        
        switch (event.button) {
            case 1:  // RED
                stateManager.setRedPreset();
                break;
            case 2:  // GREEN
                stateManager.setGreenPreset();
                break;
            case 3:  // BLUE
                stateManager.setBluePreset();
                break;
            case 4:  // EFFECT
                stateManager.resetEffect();
                break;
        }
    }
}

void printDebugInfo() {
    static unsigned long lastDebugPrint = 0;
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastDebugPrint >= 5000) {
        const auto& debugInfo = buttonQueue.getDebugInfo();
        const auto& color = stateManager.getColorState();
        
        DEBUG_PRINTLN("\n----- DEBUG INFO -----");
        DEBUG_PRINTF("Queue - Push attempts: %lu, Success: %lu\n", 
                     debugInfo.pushAttempts, debugInfo.pushSuccess);
        DEBUG_PRINTF("Queue - Pop attempts: %lu, Success: %lu\n", 
                     debugInfo.popAttempts, debugInfo.popSuccess);
        DEBUG_PRINTF("Last button - Pushed: %lu, Processed: %lu\n", 
                     debugInfo.lastPushedButton, debugInfo.lastProcessedButton);
        DEBUG_PRINTF("Interrupts: %lu, Debounce checks: %lu\n", 
                     debugInfo.interruptCalls, debugInfo.debounceChecks);
        DEBUG_PRINTF("Current Values - R:%d G:%d B:%d Effect:%d\n", 
                     color.red, color.green, color.blue, stateManager.getEffectIndex());
        DEBUG_PRINTLN("--------------------\n");
        
        lastDebugPrint = currentMillis;
    }
}

void loop() {
    static unsigned long lastDisplayUpdate = 0;
    static unsigned long lastWLEDUpdate = 0;
    unsigned long currentMillis = millis();
    
    processEncoders();
    processButtons();
    printDebugInfo();
    
    // Update display
    if (currentMillis - lastDisplayUpdate >= Timing::DISPLAY_UPDATE_INTERVAL) {
        const auto& color = stateManager.getColorState();
        display.updateDisplay(color.red, color.green, color.blue);
        lastDisplayUpdate = currentMillis;
    }
    
    // Update WLED
    if (currentMillis - lastWLEDUpdate >= Timing::WLED_UPDATE_INTERVAL) {
        const auto& color = stateManager.getColorState();
        if (stateManager.hasColorChanged()) {  // Simplified check
            DEBUG_PRINTF("Sending WLED update - R:%d G:%d B:%d\n", 
                color.red, color.green, color.blue);
            wled.updateColor(color.red, color.green, color.blue);
            stateManager.clearColorChanged();
        } else if (stateManager.hasEffectChanged()) {
            DEBUG_PRINTF("Sending WLED effect update: %s\n", 
                Effects::NAMES[stateManager.getEffectIndex()]);
            wled.updateEffect(stateManager.getEffectIndex());
            stateManager.clearEffectChanged();
        }
        lastWLEDUpdate = currentMillis;
    }
}