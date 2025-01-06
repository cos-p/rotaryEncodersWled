#pragma once
#include <Arduino.h>
#include "config.h"

volatile bool colorChangedFromButton = false;

struct ButtonEvent {
    Buttons::ID button;  // Changed from uint8_t to enum
    bool pressed;
    unsigned long timestamp;
    
    ButtonEvent& operator=(const volatile ButtonEvent& other) {
        button = static_cast<Buttons::ID>(other.button);  // Safe casting
        pressed = other.pressed;
        timestamp = other.timestamp;
        return *this;
    }
};
// Button event queue
class ButtonEventQueue {
public:
    // Debug info structure update
    struct DebugInfo {
        volatile uint32_t pushAttempts;
        volatile uint32_t pushSuccess;
        volatile uint32_t popAttempts;
        volatile uint32_t popSuccess;
        volatile Buttons::ID lastPushedButton;     // Changed to enum
        volatile Buttons::ID lastProcessedButton;  // Changed to enum
        volatile uint32_t interruptCalls;
        volatile uint32_t debounceChecks;
        
        DebugInfo() : 
            pushAttempts(0), 
            pushSuccess(0), 
            popAttempts(0), 
            popSuccess(0),
            lastPushedButton(Buttons::ID::RED_ID),  // Initialize with default
            lastProcessedButton(Buttons::ID::RED_ID),
            interruptCalls(0),
            debounceChecks(0) {}
    };

    // Update push method signature
    bool push(Buttons::ID button, bool pressed, unsigned long timestamp) {
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