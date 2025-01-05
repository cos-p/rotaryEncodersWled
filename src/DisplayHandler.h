#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

class DisplayHandler {
public:
    DisplayHandler();
    bool begin();
    void updateDisplay(int redValue, int greenValue, int blueValue);

private:
    Adafruit_SSD1306 display;
    void drawTestPattern();
    
    // Track last values to prevent unnecessary updates
    int lastRed = -1;
    int lastGreen = -1;
    int lastBlue = -1;
    unsigned long updateCount = 0;
};

DisplayHandler::DisplayHandler() 
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
    Serial.println("Display Handler constructor called");
}

bool DisplayHandler::begin() {
    Serial.println("Starting display initialization...");
    
    Wire.begin(I2C::SDA_PIN, I2C::SCL_PIN);
    Wire.setClock(I2C::FREQUENCY);
    Serial.printf("I2C Initialized - SDA: %d, SCL: %d\n", I2C::SDA_PIN, I2C::SCL_PIN);

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.printf("SSD1306 allocation failed! Check if display is connected to pins SDA:%d SCL:%d\n", 
                     I2C::SDA_PIN, I2C::SCL_PIN);
        return false;
    }
    
    Serial.println("Display allocated successfully");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    drawTestPattern();
    
    return true;
}

void DisplayHandler::drawTestPattern() {
    Serial.println("Drawing test pattern...");
    display.clearDisplay();
    
    display.drawPixel(0, 0, SSD1306_WHITE);
    display.drawPixel(SCREEN_WIDTH-1, 0, SSD1306_WHITE);
    display.drawPixel(0, SCREEN_HEIGHT-1, SSD1306_WHITE);
    display.drawPixel(SCREEN_WIDTH-1, SCREEN_HEIGHT-1, SSD1306_WHITE);
    
    display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
    
    display.setCursor(10, 20);
    display.print("Display Test");
    display.display();
    
    Serial.println("Test pattern sent to display");
    delay(2000);
    
    display.clearDisplay();
    display.display();
}

void DisplayHandler::updateDisplay(int redValue, int greenValue, int blueValue) {
    // Only update if values have changed
    if (redValue == lastRed && greenValue == lastGreen && blueValue == lastBlue) {
        return;
    }
    
    // Store new values
    lastRed = redValue;
    lastGreen = greenValue;
    lastBlue = blueValue;
    
    // Debug print every update
    Serial.printf("Display Update #%d - R:%d G:%d B:%d\n", 
                 ++updateCount, redValue, greenValue, blueValue);
    
    display.clearDisplay();

    const int usableWidth = SCREEN_WIDTH - (2 * DisplayConfig::MARGIN);
    const int barSpacing = (usableWidth - (3 * DisplayConfig::BAR_WIDTH)) / 2;
    const int baseX = DisplayConfig::MARGIN;
    
    const int redHeight = (redValue * DisplayConfig::MAX_HEIGHT) / 255;
    const int greenHeight = (greenValue * DisplayConfig::MAX_HEIGHT) / 255;
    const int blueHeight = (blueValue * DisplayConfig::MAX_HEIGHT) / 255;
    
    const int greenX = baseX + DisplayConfig::BAR_WIDTH + barSpacing;
    const int blueX = greenX + DisplayConfig::BAR_WIDTH + barSpacing;
    
    // Draw bars and labels
    display.fillRect(baseX, DisplayConfig::BASE_Y - redHeight, 
                    DisplayConfig::BAR_WIDTH, redHeight, SSD1306_WHITE);
    display.fillRect(greenX, DisplayConfig::BASE_Y - greenHeight, 
                    DisplayConfig::BAR_WIDTH, greenHeight, SSD1306_WHITE);
    display.fillRect(blueX, DisplayConfig::BASE_Y - blueHeight, 
                    DisplayConfig::BAR_WIDTH, blueHeight, SSD1306_WHITE);
    
    // Draw labels
    display.setCursor(baseX + 5, DisplayConfig::BASE_Y + 2);
    display.print("R");
    display.setCursor(greenX + 5, DisplayConfig::BASE_Y + 2);
    display.print("G");
    display.setCursor(blueX + 5, DisplayConfig::BASE_Y + 2);
    display.print("B");
    
    // Draw values
    display.setCursor(baseX, 2);
    display.print("R-");
    display.print(redValue);
    display.setCursor(greenX, 2);
    display.print("G-");
    display.print(greenValue);
    display.setCursor(blueX, 2);
    display.print("B-");
    display.print(blueValue);
    
    display.display();
}