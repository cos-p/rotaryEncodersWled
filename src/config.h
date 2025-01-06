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

namespace Buttons {
    constexpr size_t NUM_BUTTONS = 4;  // Number of buttons
    enum class Index : uint8_t {
        RED_INDEX = 0,
        GREEN_INDEX = 1,
        BLUE_INDEX = 2,
        EFFECT_INDEX = 3,
        BUTTON_COUNT = 4
    };

    enum class ID : uint8_t {
        RED_ID = 1,
        GREEN_ID = 2,
        BLUE_ID = 3,
        EFFECT_ID = 4
    };

    // Preset values for each button
    constexpr int PRESET_VALUE = 150;  // Move the preset value here
    
    // Button timing configuration
    constexpr unsigned long VERIFY_DELAY_US = 10;  // Microseconds delay for button verification
}

// Network Configuration
namespace NetworkConfig {
    constexpr char WIFI_SSID[] = "your_ssid";
    constexpr char WIFI_PASSWORD[] = "your_password";
    constexpr char STATIC_IP[] = "your_static_ip";
    constexpr char GATEWAY[] = "your_gateway";
    constexpr char SUBNET[] = "your_subnet";
    
    // WLED Configuration
    constexpr char WLED_IP[] = "your_wled_ip"; // IP of your WLED device
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
    constexpr const char* NAMES[] = {  // Added const
        "Solid", "Android", "Rainbow", "Chase", "Colorloop",
        "Dancing", "Fire", "Glitter", "Ocean", "Plasma"
    };
    constexpr int COUNT = 10;
}

namespace Timing {
    constexpr unsigned long DEBOUNCE_DELAY = 50;
    constexpr unsigned long DISPLAY_UPDATE_INTERVAL = 33;   // ~30fps
    constexpr unsigned long WLED_UPDATE_INTERVAL = 150;     // Increased to reduce flicker
    constexpr unsigned long ENCODER_PROCESS_INTERVAL = 5;   // Process encoders more frequently
}