#pragma once

#include <Arduino.h>

// Debug information structure
struct DebugInfo {
    volatile uint32_t pushAttempts;
    volatile uint32_t pushSuccess;
    volatile uint32_t popAttempts;
    volatile uint32_t popSuccess;
    volatile uint32_t lastPushedButton;
    volatile uint32_t lastProcessedButton;
    volatile uint32_t interruptCalls;
    volatile uint32_t debounceChecks;
    
    DebugInfo() : 
        pushAttempts(0), 
        pushSuccess(0), 
        popAttempts(0), 
        popSuccess(0),
        lastPushedButton(0),
        lastProcessedButton(0),
        interruptCalls(0),
        debounceChecks(0) {}
};
// // Add a flag to track color changes from buttons
volatile bool colorChangedFromButton = false;
// Button event structure
struct ButtonEvent {
    uint8_t button;      // RED = 1, GREEN = 2, BLUE = 3, EFFECT = 4
    bool pressed;        // true for press, false for release
    unsigned long timestamp;
    
    // Copy constructor for volatile to non-volatile conversion
    ButtonEvent& operator=(const volatile ButtonEvent& other) {
        button = other.button;
        pressed = other.pressed;
        timestamp = other.timestamp;
        return *this;
    }
};

class ButtonEventQueue {
public:
    ButtonEventQueue() : head(0), tail(0) {}
    
    // Push a new button event to the queue
    bool push(uint8_t button, bool pressed, unsigned long timestamp) {
        debugInfo.pushAttempts++;
        int nextHead = (head + 1) % SIZE;
        
        if (nextHead != tail) {
            events[head].button = button;
            events[head].pressed = pressed;
            events[head].timestamp = timestamp;
            head = nextHead;
            debugInfo.pushSuccess++;
            debugInfo.lastPushedButton = button;
            return true;
        }
        return false;
    }
    
    // Pop the next button event from the queue
    bool pop(ButtonEvent* event) {
        debugInfo.popAttempts++;
        
        if (tail != head) {
            // Properly handle volatile to non-volatile assignment
            event->button = events[tail].button;
            event->pressed = events[tail].pressed;
            event->timestamp = events[tail].timestamp;
            
            tail = (tail + 1) % SIZE;
            debugInfo.popSuccess++;
            debugInfo.lastProcessedButton = event->button;
            return true;
        }
        return false;
    }
    
    // Get current size of queue
    int getSize() const {
        return (head >= tail) ? (head - tail) : (SIZE - (tail - head));
    }
    
    // Get debug information
    const DebugInfo& getDebugInfo() const {
        return debugInfo;
    }

private:
    static const int SIZE = 20;
    volatile ButtonEvent events[SIZE];
    volatile int head;
    volatile int tail;
    DebugInfo debugInfo;
};