#pragma once

#define DEBUG true

#if DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTF(x, ...) Serial.printf(x, __VA_ARGS__)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x, ...)
#endif

// Display Constants
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// I2C Pins
namespace I2C {
    constexpr int SDA_PIN = 2;
    constexpr int SCL_PIN = 3;
    constexpr int FREQUENCY = 400000;  // 400kHz
}

// Pin Definitions
namespace Pins {
    // Encoders
    constexpr int RED_A = 36;
    constexpr int RED_B = 34;
    constexpr int GREEN_A = 35;
    constexpr int GREEN_B = 33;
    constexpr int BLUE_A = 5;
    constexpr int BLUE_B = 7;
    constexpr int EFFECT_A = 4;
    constexpr int EFFECT_B = 6;

    // Buttons
    constexpr int RED_BUTTON = 21;
    constexpr int GREEN_BUTTON = 18;
    constexpr int BLUE_BUTTON = 9;
    constexpr int EFFECT_BUTTON = 8;
}

// Network Configuration
namespace NetworkConfig {
    constexpr char WIFI_SSID[] = "xxxxx";
    constexpr char WIFI_PASSWORD[] = "xxxxx";
    constexpr char STATIC_IP[] = "192.168.x.x";
    constexpr char GATEWAY[] = "192.168.x.x";
    constexpr char SUBNET[] = "255.255.255.0";
    
    // WLED Configuration
    constexpr char WLED_IP[] = "192.168.x.x";
    constexpr int WLED_PORT = 80;
}

// Display settings
namespace DisplayConfig {
    constexpr int BAR_WIDTH = 20;
    constexpr int MARGIN = 10;
    constexpr int MAX_HEIGHT = 48;
    constexpr int BASE_Y = 60;
}

// Effect Names
namespace Effects {
    constexpr char* NAMES[] = {
        "Solid", "Android", "Rainbow", "Chase", "Colorloop",
        "Dancing", "Fire", "Glitter", "Ocean", "Plasma"
    };
    constexpr int COUNT = 10;
}

// Timing Constants
//pre splitting the encoder.
// namespace Timing {
//     constexpr unsigned long DEBOUNCE_DELAY = 50;
//     constexpr unsigned long DISPLAY_UPDATE_INTERVAL = 16;  // ~60fps
//     constexpr unsigned long WLED_UPDATE_INTERVAL = 20;     // 50fps - much more responsive
// }

// namespace Timing {
//     constexpr unsigned long DEBOUNCE_DELAY = 50;
//     constexpr unsigned long DISPLAY_UPDATE_INTERVAL = 33;  // ~30fps - more reasonable
//     constexpr unsigned long WLED_UPDATE_INTERVAL = 100;    // Less frequent updates, more stable
// }
namespace Timing {
    constexpr unsigned long DEBOUNCE_DELAY = 50;
    constexpr unsigned long DISPLAY_UPDATE_INTERVAL = 33;   // ~30fps
    constexpr unsigned long WLED_UPDATE_INTERVAL = 150;     // Increased to reduce flicker
    constexpr unsigned long ENCODER_PROCESS_INTERVAL = 5;   // Process encoders more frequently
}