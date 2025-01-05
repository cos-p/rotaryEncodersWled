// #pragma once

// #include <Arduino.h>
// #include "config.h"  // For Effects::COUNT

// // Structure to hold RGB color values
// struct ColorState {
//     int red;
//     int green;
//     int blue;
// };

// class StateManager {
// public:
//     StateManager() : 
//         colorState{0, 0, 0},
//         effectIndex(0),
//         colorChangedFromButton(false),
//         effectChanged(false) {}

//     // Color methods
//     void setColor(int red, int green, int blue) {
//         colorState.red = constrain(red, 0, 255);
//         colorState.green = constrain(green, 0, 255);
//         colorState.blue = constrain(blue, 0, 255);
//     }

//     void adjustColor(int redDelta, int greenDelta, int blueDelta) {
//         colorState.red = constrain(colorState.red + redDelta, 0, 255);
//         colorState.green = constrain(colorState.green + greenDelta, 0, 255);
//         colorState.blue = constrain(colorState.blue + blueDelta, 0, 255);
//     }

//     // Preset color methods
//     void setRedPreset() {
//         setColor(150, 0, 0);
//         colorChangedFromButton = true;
//     }

//     void setGreenPreset() {
//         setColor(0, 150, 0);
//         colorChangedFromButton = true;
//     }

//     void setBluePreset() {
//         setColor(0, 0, 150);
//         colorChangedFromButton = true;
//     }

//     // Effect methods
//     void setEffect(int newIndex) {
//         effectIndex = constrain(newIndex, 0, Effects::COUNT - 1);
//         effectChanged = true;
//     }

//     void adjustEffect(int delta) {
//         effectIndex = (effectIndex + delta + Effects::COUNT) % Effects::COUNT;
//         effectChanged = true;
//     }

//     void resetEffect() {
//         effectIndex = 0;
//         effectChanged = true;
//     }

//     // Getters
//     const ColorState& getColorState() const { return colorState; }
//     int getEffectIndex() const { return effectIndex; }
//     bool hasColorChanged() const { return colorChangedFromButton; }
//     bool hasEffectChanged() const { return effectChanged; }
    
//     // Individual color getters
//     int getRed() const { return colorState.red; }
//     int getGreen() const { return colorState.green; }
//     int getBlue() const { return colorState.blue; }

//     // Flag management
//     void clearColorChanged() { colorChangedFromButton = false; }
//     void clearEffectChanged() { effectChanged = false; }

// private:
//     ColorState colorState;
//     int effectIndex;
//     volatile bool colorChangedFromButton;
//     volatile bool effectChanged;
// };




#pragma once

#include <Arduino.h>
#include "config.h"  // For Effects::COUNT

// Structure to hold RGB color values
struct ColorState {
    int red;
    int green;
    int blue;
};

class StateManager {
public:
    StateManager() : 
        colorState{0, 0, 0},
        effectIndex(0),
        colorChangedFromButton(false),
        effectChanged(false) {}

    // Color methods
    void setColor(int red, int green, int blue) {
        colorState.red = constrain(red, 0, 255);
        colorState.green = constrain(green, 0, 255);
        colorState.blue = constrain(blue, 0, 255);
    }

    // void adjustColor(int redDelta, int greenDelta, int blueDelta) {
    //     colorState.red = constrain(colorState.red + redDelta, 0, 255);
    //     colorState.green = constrain(colorState.green + greenDelta, 0, 255);
    //     colorState.blue = constrain(colorState.blue + blueDelta, 0, 255);
    // }
    void adjustColor(int redDelta, int greenDelta, int blueDelta) {
        int newRed = constrain(colorState.red + redDelta, 0, 255);
        int newGreen = constrain(colorState.green + greenDelta, 0, 255);
        int newBlue = constrain(colorState.blue + blueDelta, 0, 255);
        
        // Only set changed flag if values actually changed
        if (newRed != colorState.red || newGreen != colorState.green || newBlue != colorState.blue) {
            colorState.red = newRed;
            colorState.green = newGreen;
            colorState.blue = newBlue;
            colorChangedFromButton = true;  // Set flag for any color change
    }
}

    // Preset color methods
    void setRedPreset() {
        setColor(150, 0, 0);
        colorChangedFromButton = true;
    }

    void setGreenPreset() {
        setColor(0, 150, 0);
        colorChangedFromButton = true;
    }

    void setBluePreset() {
        setColor(0, 0, 150);
        colorChangedFromButton = true;
    }

    // Effect methods
    void setEffect(int newIndex) {
        effectIndex = constrain(newIndex, 0, Effects::COUNT - 1);
        effectChanged = true;
    }

    void adjustEffect(int delta) {
        effectIndex = (effectIndex + delta + Effects::COUNT) % Effects::COUNT;
        effectChanged = true;
    }

    void resetEffect() {
        effectIndex = 0;
        effectChanged = true;
    }

    // Getters
    const ColorState& getColorState() const { return colorState; }
    int getEffectIndex() const { return effectIndex; }
    bool hasColorChanged() const { return colorChangedFromButton; }
    bool hasEffectChanged() const { return effectChanged; }
    
    // Individual color getters
    int getRed() const { return colorState.red; }
    int getGreen() const { return colorState.green; }
    int getBlue() const { return colorState.blue; }

    // Flag management
    void clearColorChanged() { colorChangedFromButton = false; }
    void clearEffectChanged() { effectChanged = false; }

private:
    ColorState colorState;
    int effectIndex;
    volatile bool colorChangedFromButton;
    volatile bool effectChanged;
};