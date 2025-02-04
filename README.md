# RGB Color Mixer with WLED Integration

An ESP32-based RGB controller that serves dual purposes: a hands-on educational toy for teaching children about color mixing, and a precise physical control interface for WLED-powered LED installations. Using rotary encoders for RGB adjustment, it provides both an engaging way for young learners to explore color theory and a tactile, responsive interface for makers and lighting enthusiasts to control their WLED setups.

## Features

- Real-time RGB color mixing using rotary encoders
- OLED display visualization of current RGB values
- WLED effects selection with dedicated encoder
- Preset color buttons for quick color selection
- Responsive interface with debounced inputs
- Integration with WLED's HTTP API
- Network configuration with static IP
- Persistent state management

## Hardware Requirements

- ESP32 development board (tested on ESP32-S2-Saola-1)
- 4× Rotary Encoder Module KY-040 [link](https://www.aliexpress.com/item/1005005559054521.html?spm)
- 0.96" SSD1306 OLED display - I2C [link](https://www.aliexpress.com/item/1005007389730469.html?spm)
- WLED device

## Pin Configuration

### Rotary Encoders
- Red: CLK (GPIO5), DT (GPIO7)
- Green: CLK (GPIO35), DT (GPIO33)
- Blue: CLK (GPIO36), DT (GPIO34)
- Effect: CLK (GPIO4), DT (GPIO6)

### Push Buttons
- Red: GPIO9
- Green: GPIO18
- Blue: GPIO21
- Effect: GPIO8

### Display
- SDA: GPIO2
- SCL: GPIO3

## Software Architecture

The project is structured into several key components:

### Core Components

1. **StateManager (StateManager.h)**
   - Handles RGB color and effect state
   - Manages state changes and updates
   - Provides preset color functionality

2. **DisplayHandler (DisplayHandler.h)**
   - Controls the OLED display
   - Renders real-time RGB value visualization
   - Shows color bars and numeric values

3. **NetworkManager (NetworkManager.h)**
   - Manages WiFi connectivity
   - Handles static IP configuration
   - Enables communication with WLED device

4. **WLEDController (WLEDController.h)**
   - Interfaces with WLED's HTTP API
   - Sends color and effect updates
   - Manages HTTP requests to WLED device

5. **ButtonEventQueue (ButtonEventQueue.h)**
   - Implements interrupt-safe button event handling
   - Provides debouncing functionality
   - Maintains event queue for button presses

### Configuration

The `config.h` file contains all configurable parameters including:
- DEBUG prints on/off setting
- Pin definitions
- Network settings
- Display parameters
- Timing constants
- Effect definitions

## Installation

1. Clone this repository
2. Update `config.h` with your network settings:
   ```cpp
   namespace NetworkConfig {
       constexpr char WIFI_SSID[] = "your_ssid";
       constexpr char WIFI_PASSWORD[] = "your_password";
       constexpr char STATIC_IP[] = "your_static_ip";
       constexpr char WLED_IP[] = "your_wled_ip";  // IP of your WLED device
   }
   ```
3. Install required libraries:
   - Adafruit_GFX
   - Adafruit_SSD1306
   - ArduinoJson
   - ESPAsyncWebServer
4. Upload to your ESP32 device

## WLED Integration

This project integrates with WLED by sending HTTP requests to WLED's API endpoint. The WLEDController class handles communication by sending JSON payloads to update colors and effects.

### Color Control
Sends color updates to WLED's `/json/state` endpoint:
```json
{
    "on": true,
    "bri": 255,
    "seg": [{
        "col": [[r, g, b]]
    }]
}
```

### Effect Control
Sends effect updates using WLED's effect indices:
```json
{
    "on": true,
    "seg": [{
        "fx": effect_index
    }]
}
```

## Supported WLED Effects

I've only included 10 effects, but they're easy enough to add, just match them to WLED's built-in effects:
1. Solid
2. Android
3. Rainbow
4. Chase
5. Colorloop
6. Dancing
7. Fire
8. Glitter
9. Ocean
10. Plasma

## Development

### Building
```bash
platformio run
```

### Uploading
```bash
platformio run --target upload
```

### Monitoring
```bash
platformio device monitor
```

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- [WLED project](https://github.com/Aircoookie/WLED) for the LED control firmware and API
- ESP32 community for various code examples and inspiration
- Contributors to the libraries used in this project