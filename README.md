# Balboa Spa ESPHome Component

A comprehensive ESPHome component for monitoring and controlling Balboa spa systems via RS485 communication. This component provides full integration with Home Assistant for spa automation and monitoring.

## 🚀 Features

- **🌡️ Full Spa Monitoring**: Temperature, pump states, filter cycles, heat state, circulation, rest mode
- **🎛️ Climate Control**: Direct temperature control through Home Assistant
- **⚡ Pump Control**: Individual control of spa pumps with speed cycling
- **💡 Light Control**: Spa light on/off functionality
- **🔧 Filter Monitoring**: Real-time filter cycle status, runtime tracking, and schedule display
- **⏰ Time Synchronization**: Automatic spa clock synchronization
- **🔍 Comprehensive Diagnostics**: Extensive debug sensors and binary sensors
- **🔌 Hardware Agnostic**: Works with any ESP32/ESP8266 board and RS485 converter

## 📋 Requirements

### Hardware
- **ESP32 or ESP8266 board** (any compatible board)
- **RS485 converter** (MAX485, SN75176, Tail485, or similar)
- **Wiring** to connect the RS485 converter to your spa's communication port

### Software
- **ESPHome** (latest version recommended)
- **Home Assistant** (optional, for full integration)

## 🛠️ Quick Start

### 1. Install the Component

#### Option A: External Components (Recommended)
Add to your ESPHome configuration:

```yaml
external_components:
  - source: https://github.com/poedenon/esphome-balboa-spa
    components: [balboa_spa]
```

#### Option B: Manual Installation
1. Clone this repository
2. Copy the `components/balboa_spa` folder to your ESPHome configuration directory
3. Add to your ESPHome configuration:

```yaml
external_components:
  - source: ./components
    components: [balboa_spa]
```

### 2. Basic Configuration

Add this to your ESPHome configuration file:

```yaml
# Balboa Spa Component
balboa_spa:
  id: spa
  uart_id: spa_uart_bus
  spa_temp_scale: "F"  # or "C"

# UART Configuration
uart:
  id: spa_uart_bus
  tx_pin: GPIO26  # Change to your TX pin
  rx_pin: GPIO32  # Change to your RX pin
  baud_rate: 115200
  data_bits: 8
  parity: NONE
  stop_bits: 1
```

### 3. Hardware Setup

#### Wiring Diagram
```
ESP32/ESP8266    RS485 Converter    Balboa Spa
TX (GPIO26)  --> A+ (or DI)
RX (GPIO32)  --> B- (or RO)
GND         --> GND
3.3V/5V     --> VCC
```

#### Common Pin Configurations

| Board | TX Pin | RX Pin | Notes |
|-------|--------|--------|-------|
| M5Stack Atom | GPIO26 | GPIO32 | Default configuration |
| ESP32 DevKit | GPIO17 | GPIO16 | Standard ESP32 pins |
| ESP8266 NodeMCU | GPIO1 | GPIO3 | Standard ESP8266 pins |

### 4. Compile and Upload

```bash
esphome run your_config.yaml
```

## 📖 Full Configuration Example

```yaml
esphome:
  name: balboa-spa-monitor
  friendly_name: "Balboa Spa Monitor"

esp32:
  board: m5stack-atom
  framework:
    type: arduino

# UART Configuration
uart:
  id: spa_uart_bus
  tx_pin: GPIO26
  rx_pin: GPIO32
  baud_rate: 115200
  data_bits: 8
  parity: NONE
  stop_bits: 1

# Balboa Spa Component
balboa_spa:
  id: spa
  uart_id: spa_uart_bus
  spa_temp_scale: "F"

# Climate Control
climate:
  - platform: balboa_spa
    balboa_spa_id: spa
    name: "Spa Thermostat"
    id: spa_thermostat
    visual:
      min_temperature: 79 °F
      max_temperature: 104 °F  
      temperature_step: 1.0 °F

# Switch Controls
switch:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet1:
      name: "Spa Pump 1"
      id: spa_pump1_control
      icon: "mdi:pump"
      entity_category: diagnostic
    jet2:
      name: "Spa Pump 2"
      id: spa_pump2_control
      icon: "mdi:pump"
      entity_category: diagnostic
    light:
      name: "Spa Light"
      id: spa_light_control
      icon: "mdi:lightbulb"
      entity_category: diagnostic

# Sensors
sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    heatstate:
      name: "Spa Heat State"
      id: spa_heat_state
      icon: "mdi:thermometer"
    circulation:
      name: "Spa Circulation"
      id: spa_circulation
      icon: "mdi:rotate-3d-variant"
    restmode:
      name: "Spa Rest Mode"
      id: spa_rest_mode
      icon: "mdi:sleep"

# Binary Sensors
binary_sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    filter1_active:
      name: "Filter 1 Active"
      id: spa_filter1_active
      icon: "mdi:filter"
      entity_category: diagnostic
    filter2_active:
      name: "Filter 2 Active"
      id: spa_filter2_active
      icon: "mdi:filter"
      entity_category: diagnostic

# WiFi Configuration
wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

# API for Home Assistant
api:
  encryption:
    key: !secret api_encryption_key

# OTA Updates
ota:
  - platform: esphome
```

## 🎯 Available Entities

### Climate
- **Spa Thermostat**: Control spa temperature (79-104°F)

### Switches
- **Spa Pump 1**: Control pump 1 (diagnostic category)
- **Spa Pump 2**: Control pump 2 (diagnostic category)
- **Spa Light**: Control spa light (diagnostic category)

### Sensors
- **Spa Heat State**: Current heating status
- **Spa Circulation**: Circulation pump status
- **Spa Rest Mode**: Rest mode status
- **Spa Clock Hour/Minute**: Spa's internal clock
- **Pump Speeds**: Individual pump speed indicators

### Binary Sensors
- **Filter 1/2 Active**: Filter enable/disable status
- **Filter 1/2 Running**: Real-time filter running status
- **Pump Running States**: Individual pump running indicators
- **Light Status**: Light on/off status

### Template Sensors (Optional)
The component also provides template sensors for:
- **Filter Schedules**: Display filter schedules in readable format
- **Spa Time Display**: Current spa time in 12-hour format
- **Pump Status**: Combined pump status information

## 🔧 Configuration Options

### Temperature Scale
```yaml
balboa_spa:
  spa_temp_scale: "F"  # Fahrenheit
  # or
  spa_temp_scale: "C"  # Celsius
```

### UART Configuration
```yaml
uart:
  id: spa_uart_bus
  tx_pin: GPIO26        # Transmit pin
  rx_pin: GPIO32        # Receive pin
  baud_rate: 115200     # Communication speed
  data_bits: 8          # Data bits
  parity: NONE          # Parity
  stop_bits: 1          # Stop bits
  # Optional: Direction control for 3-wire RS485
  de_pin: GPIO25        # Direction control pin
```

### Polling Intervals
The component uses optimized polling intervals for better performance:
- Temperature sensors: 5-30 seconds
- System sensors: 300 seconds
- Communication checks: 120 seconds

## 🏠 Home Assistant Integration

### Automatic Discovery
All entities are automatically discovered by Home Assistant when using the ESPHome integration.

### Dashboard Examples

Check out the [📱 Dashboard Examples](docs/examples/dashboards/) for complete dashboard configurations:

- **`spa-dashboard-example.md`**: Detailed description of a beautiful spa dashboard
- **`spa-dashboard.yaml`**: Complete YAML configuration for a mobile-optimized dashboard

#### Basic Spa Control
```yaml
type: vertical-stack
cards:
  - type: thermostat
    entity: climate.spa_thermostat
    name: "Spa Temperature"
  - type: horizontal-stack
    cards:
      - type: button
        entity: switch.spa_pump1_control
        name: "Pump 1"
        icon: mdi:pump
      - type: button
        entity: switch.spa_pump2_control
        name: "Pump 2"
        icon: mdi:pump
      - type: button
        entity: switch.spa_light_control
        name: "Light"
        icon: mdi:lightbulb
```

#### Filter Status
```yaml
type: entities
title: "Filter Status"
entities:
  - entity: binary_sensor.filter1_active
    name: "Filter 1 Active"
  - entity: binary_sensor.filter1_running
    name: "Filter 1 Running"
  - entity: binary_sensor.filter2_active
    name: "Filter 2 Active"
  - entity: binary_sensor.filter2_running
    name: "Filter 2 Running"
```

## 🐛 Troubleshooting

### Common Issues

#### No Communication with Spa
1. **Check wiring connections**
   - Verify TX/RX pins are connected correctly
   - Ensure proper power to RS485 converter
2. **Verify baud rate**
   - Most Balboa spas use 115200 baud
3. **Check pin assignments**
   - Ensure pins match your hardware configuration

#### Temperature Display Issues
1. **Check temperature scale configuration**
   - Verify `spa_temp_scale` matches your spa's setting
2. **Enable debug logging**
   - Set logger level to DEBUG for detailed information
3. **Check communication status**
   - Verify spa is responding to commands

#### Compilation Errors
1. **Verify component installation**
   - Ensure external_components is configured correctly
2. **Check board type**
   - Verify board type matches your hardware
3. **Update ESPHome**
   - Use latest ESPHome version

### Debug Information

Enable debug logging:
```yaml
logger:
  level: DEBUG
  logs:
    balboa_spa: DEBUG
```

### Support

For issues and questions:
1. Check the troubleshooting section
2. Review the debug sensors in Home Assistant
3. Open an issue on GitHub with:
   - Your ESPHome configuration
   - Hardware setup details
   - Error logs

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Original Work**: Based on [dhWasabi's M5Tough-BalboaSpa-esphome](https://github.com/dhWasabi/M5Tough-BalboaSpa-esphome) repository
- **Enhancements**: Enhanced with comprehensive filter monitoring capabilities
- **Optimizations**: Optimized for performance and reliability
- **Testing**: Tested with various ESP32 and ESP8266 boards

## 📚 Additional Resources

- **[📖 Full Documentation](docs/README.md)** - Comprehensive setup and configuration guides
- [ESPHome Documentation](https://esphome.io/)
- [Home Assistant Documentation](https://www.home-assistant.io/)
- [Balboa Spa Manuals](https://www.balboawatergroup.com/support)
- [Original Repository](https://github.com/dhWasabi/M5Tough-BalboaSpa-esphome) - dhWasabi's M5Tough-BalboaSpa-esphome

---

**Note**: This component is designed for personal use. Always follow your spa manufacturer's guidelines and safety instructions.

