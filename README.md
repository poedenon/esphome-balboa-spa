# Balboa Spa Monitor - ESPHome Component

A comprehensive ESPHome component for monitoring and controlling Balboa spa systems via RS485 communication. This component is designed to work with any ESP32 or ESP8266 board and RS485 converter.

## Features

- **Full Spa Monitoring**: Temperature, pump states, filter cycles, heat state, circulation, rest mode
- **Climate Control**: Direct temperature control through Home Assistant
- **Pump Control**: Individual control of spa pumps with speed cycling
- **Light Control**: Spa light on/off functionality
- **Filter Monitoring**: Real-time filter cycle status, runtime tracking, and schedule display
- **Time Synchronization**: Automatic spa clock synchronization
- **Comprehensive Diagnostics**: Extensive debug sensors and binary sensors
- **Hardware Agnostic**: Works with any ESP32/ESP8266 board and RS485 converter

## Hardware Requirements

### Required Components
- **ESP32 or ESP8266 board** (any compatible board)
- **RS485 converter** (MAX485, SN75176, Tail485, or similar)
- **Wiring** to connect the RS485 converter to your spa's communication port

### Supported Boards
- **ESP32**: esp32dev, esp32-s3-devkitc-1, m5stack-atom, esp32-c3-devkitm-1, etc.
- **ESP8266**: esp01_1m, esp01, nodemcuv2, wemos_d1_mini32, etc.

### RS485 Converters
- MAX485, SN75176, Tail485, or any RS485 converter
- Supports both 2-wire and 3-wire (with direction control) configurations

## Quick Setup

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/BalboaSpa-esphome.git
cd BalboaSpa-esphome
```

### 2. Configure Your Hardware
Copy the secrets template and configure your hardware:

```bash
cp secrets_template.txt secrets.yaml
```

Edit `secrets.yaml` with your specific hardware configuration:

```yaml
# =============================================================================
# HARDWARE CONFIGURATION
# =============================================================================

# Board Configuration
board_type: m5stack-atom  # Change to your board type
framework_type: arduino   # arduino for most boards

# RS485/UART Configuration
uart_tx_pin: GPIO26       # Change to your TX pin
uart_rx_pin: GPIO32       # Change to your RX pin
uart_baud_rate: 115200    # Usually 115200 for Balboa spas
uart_de_pin: null         # Set to pin number if using direction control

# =============================================================================
# NETWORK CONFIGURATION
# =============================================================================
wifi_ssid: "YourWiFiSSID"
wifi_password: "YourWiFiPassword"

# =============================================================================
# DEVICE CONFIGURATION
# =============================================================================
device_name: "balboa-spa-monitor"
friendly_name: "Balboa Spa Monitor"
temperature_scale: "F"    # F or C
timezone: "America/Chicago"

# =============================================================================
# SECURITY CONFIGURATION
# =============================================================================
api_encryption_key: "your_32_character_encryption_key_here"

# =============================================================================
# MQTT CONFIGURATION
# =============================================================================
mqtt_broker: "your_mqtt_broker_ip"
mqtt_port: 1883
mqtt_username: "your_mqtt_username"
mqtt_password: "your_mqtt_password"
```

### 3. Common Hardware Configurations

#### M5Stack Atom Lite (Default)
```yaml
board_type: m5stack-atom
framework_type: arduino
uart_tx_pin: GPIO26
uart_rx_pin: GPIO32
uart_baud_rate: 115200
uart_de_pin: null
```

#### ESP32 DevKit
```yaml
board_type: esp32dev
framework_type: arduino
uart_tx_pin: GPIO17
uart_rx_pin: GPIO16
uart_baud_rate: 115200
uart_de_pin: null
```

#### ESP8266 NodeMCU
```yaml
board_type: nodemcuv2
framework_type: arduino
uart_tx_pin: GPIO1
uart_rx_pin: GPIO3
uart_baud_rate: 115200
uart_de_pin: null
```

#### ESP32 with Direction Control
```yaml
board_type: esp32dev
framework_type: arduino
uart_tx_pin: GPIO17
uart_rx_pin: GPIO16
uart_baud_rate: 115200
uart_de_pin: GPIO25  # Direction control pin
```

### 4. Wiring Instructions

#### Basic 2-Wire RS485 Setup
```
ESP32/ESP8266    RS485 Converter    Balboa Spa
TX (GPIO26)  --> A+ (or DI)
RX (GPIO32)  --> B- (or RO)
GND         --> GND
3.3V/5V     --> VCC
```

#### 3-Wire RS485 with Direction Control
```
ESP32/ESP8266    RS485 Converter    Balboa Spa
TX (GPIO26)  --> DI
RX (GPIO32)  --> RO
DE (GPIO25)  --> DE/RE
GND         --> GND
3.3V/5V     --> VCC
```

### 5. Compile and Upload
```bash
esphome run esphome-atomlite-balboa-spa.yaml
```

## Configuration Options

### Board Types
Common ESP32 board types:
- `esp32dev` - Generic ESP32 DevKit
- `esp32-s3-devkitc-1` - ESP32-S3 DevKit
- `m5stack-atom` - M5Stack Atom Lite
- `esp32-c3-devkitm-1` - ESP32-C3 DevKit

Common ESP8266 board types:
- `esp01_1m` - ESP-01 with 1MB flash
- `esp01` - ESP-01
- `nodemcuv2` - NodeMCU v2
- `wemos_d1_mini32` - WeMos D1 Mini

### Pin Configuration
- **TX Pin**: GPIO pin connected to RS485 converter's transmit input
- **RX Pin**: GPIO pin connected to RS485 converter's receive output
- **DE Pin**: GPIO pin for direction control (optional, set to `null` if not used)

### Baud Rate
Most Balboa spas use 115200 baud rate. If your spa uses a different rate, change `uart_baud_rate` accordingly.

## Available Entities

### Climate
- **Spa Thermostat**: Control spa temperature (79-110°F)

### Switches
- **Spa Pump 1**: Control pump 1 (hidden, diagnostic)
- **Spa Pump 2**: Control pump 2 (hidden, diagnostic)
- **Spa Light**: Control spa light (hidden, diagnostic)

### Sensors
- **Spa Heat State**: Current heating status
- **Spa Circulation**: Circulation pump status
- **Spa Rest Mode**: Rest mode status
- **Spa Clock Hour/Minute**: Spa's internal clock
- **Pump Speeds**: Individual pump speed indicators
- **WiFi Signal**: WiFi signal strength
- **Uptime**: Device uptime

### Filter Sensors
- **Filter 1/2 Schedule**: Display filter schedules in 12-hour format (e.g., "12:00pm - 4:00pm")
- **Filter 1/2 Runtime Hours**: Total accumulated runtime hours
- **Filter 1/2 Cycles Completed**: Number of completed filter cycles
- **Filter 1/2 Current Runtime**: Current active runtime in minutes

### Binary Sensors
- **Device Status**: Online/offline status
- **Filter 1 Running**: Real-time filter 1 running status
- **Filter 2 Running**: Real-time filter 2 running status
- **Filter 1/2 Active**: Filter enable/disable status
- **Pump Running States**: Individual pump running indicators
- **Light Status**: Light on/off status

### Buttons
- **Restart**: Restart the device
- **Sync Spa Time**: Synchronize spa clock with device time
- **Pump Controls**: Cycle pump states and toggle controls
- **Light Toggle**: Toggle spa light
- **Test Filter Settings**: Log current filter status and schedules
- **Request Filter Settings**: Send filter configuration request to spa

## Troubleshooting

### Common Issues

#### No Communication with Spa
1. Check wiring connections
2. Verify baud rate (usually 115200)
3. Check RS485 converter power
4. Verify pin assignments in `secrets.yaml`

#### Compilation Errors
1. Ensure board type is correct
2. Check framework type (usually `arduino`)
3. Verify all required secrets are set

#### WiFi Connection Issues
1. Check WiFi credentials
2. Verify WiFi signal strength
3. Check for special characters in passwords

### Debug Sensors
The configuration includes extensive debug sensors that can help troubleshoot issues:
- Status byte sensors (16-19)
- Pump interdependency sensor
- Raw jet state sensors

### Logs
Enable debug logging by changing the logger level in the configuration:
```yaml
logger:
  level: DEBUG  # Change from INFO to DEBUG
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Home Assistant Dashboard Examples

### Filter Status Cards
For displaying filter status in your Home Assistant dashboard, you can use simple markdown cards:

#### Filter 1 Status Card
```yaml
type: markdown
content: >
  **Filter 1 Status**
  
  {% set filter_running = states('binary_sensor.esphome_web_bb4e14_filter_1_running') %}
  {% set schedule = states('sensor.esphome_web_bb4e14_filter_1_schedule') %}
  
  {% if filter_running == 'on' %}
  🟢 **RUNNING** | {{ schedule }}
  {% else %}
  🔵 **INACTIVE** | {{ schedule }}
  {% endif %}
```

#### Filter 2 Status Card
```yaml
type: markdown
content: >
  **Filter 2 Status**
  
  {% set filter_running = states('binary_sensor.esphome_web_bb4e14_filter_2_running') %}
  {% set filter_enabled = states('binary_sensor.esphome_web_bb4e14_filter2_active') %}
  {% set schedule = states('sensor.esphome_web_bb4e14_filter_2_schedule') %}
  
  {% if filter_running == 'on' %}
  🟢 **RUNNING** | {{ schedule }}
  {% elif filter_enabled == 'on' %}
  🔵 **INACTIVE** | {{ schedule }}
  {% else %}
  ⚫ **DISABLED** | {{ schedule }}
  {% endif %}
```

**Note**: Replace `esphome_web_bb4e14` with your actual device name from the ESPHome configuration.

## Support

For issues and questions:
1. Check the troubleshooting section
2. Review the debug sensors
3. Open an issue on GitHub with detailed information about your setup

## Acknowledgments

- Based on the original Balboa spa ESPHome component
- Adapted for hardware flexibility and ease of use
- Tested with various ESP32 and ESP8266 boards
- Enhanced with comprehensive filter monitoring capabilities

