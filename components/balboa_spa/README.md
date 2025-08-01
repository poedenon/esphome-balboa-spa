# Balboa Spa Component

ESPHome component for monitoring and controlling Balboa spa systems via RS485 communication.

## Features

- Full spa monitoring (temperature, pumps, filters, heat state)
- Climate control integration
- Pump and light control
- Filter cycle monitoring
- Time synchronization
- Comprehensive diagnostics

## Installation

### External Components (Recommended)
```yaml
external_components:
  - source: https://github.com/poedenon/esphome-balboa-spa
    components: [balboa_spa]
```

### Manual Installation
1. Copy the `balboa_spa` folder to your ESPHome configuration directory
2. Add to your configuration:
```yaml
external_components:
  - source: ./components
    components: [balboa_spa]
```

## Configuration

### Basic Configuration
```yaml
# Balboa Spa Component
balboa_spa:
  id: spa
  uart_id: spa_uart_bus
  spa_temp_scale: "F"  # or "C"

# UART Configuration
uart:
  id: spa_uart_bus
  tx_pin: GPIO26
  rx_pin: GPIO32
  baud_rate: 115200
```

### Climate Control
```yaml
climate:
  - platform: balboa_spa
    balboa_spa_id: spa
    name: "Spa Thermostat"
    id: spa_thermostat
```

### Switches
```yaml
switch:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet1:
      name: "Spa Pump 1"
      id: spa_pump1_control
    jet2:
      name: "Spa Pump 2"
      id: spa_pump2_control
    light:
      name: "Spa Light"
      id: spa_light_control
```

### Sensors
```yaml
sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    heatstate:
      name: "Spa Heat State"
      id: spa_heat_state
    circulation:
      name: "Spa Circulation"
      id: spa_circulation
    restmode:
      name: "Spa Rest Mode"
      id: spa_rest_mode
```

### Binary Sensors
```yaml
binary_sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    filter1_active:
      name: "Filter 1 Active"
      id: spa_filter1_active
    filter2_active:
      name: "Filter 2 Active"
      id: spa_filter2_active
```

## Hardware Setup

### Wiring
```
ESP32/ESP8266    RS485 Converter    Balboa Spa
TX (GPIO26)  --> A+ (or DI)
RX (GPIO32)  --> B- (or RO)
GND         --> GND
3.3V/5V     --> VCC
```

### Common Pin Configurations
- **M5Stack Atom**: TX=GPIO26, RX=GPIO32
- **ESP32 DevKit**: TX=GPIO17, RX=GPIO16
- **ESP8266 NodeMCU**: TX=GPIO1, RX=GPIO3

## Troubleshooting

### No Communication
1. Check wiring connections
2. Verify baud rate (usually 115200)
3. Check RS485 converter power
4. Verify pin assignments

### Temperature Issues
1. Check temperature scale configuration
2. Enable debug logging
3. Verify communication status

### Debug Logging
```yaml
logger:
  level: DEBUG
  logs:
    balboa_spa: DEBUG
```

## Support

For issues and questions:
1. Check the troubleshooting section
2. Review debug sensors in Home Assistant
3. Open an issue on GitHub

## Acknowledgments

- **Original Work**: Based on [dhWasabi's M5Tough-BalboaSpa-esphome](https://github.com/dhWasabi/M5Tough-BalboaSpa-esphome) repository
- **Enhancements**: Enhanced with comprehensive monitoring capabilities
- **Optimizations**: Optimized for performance and reliability

## License

MIT License - see LICENSE file for details. 