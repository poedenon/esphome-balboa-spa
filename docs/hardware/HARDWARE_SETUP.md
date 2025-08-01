# Hardware Setup Guide

## Quick Configuration Reference

### Popular Board Configurations

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

#### ESP32-S3 DevKit
```yaml
board_type: esp32-s3-devkitc-1
framework_type: arduino
uart_tx_pin: GPIO17
uart_rx_pin: GPIO16
uart_baud_rate: 115200
uart_de_pin: null
```

#### ESP32-C3 DevKit
```yaml
board_type: esp32-c3-devkitm-1
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

#### ESP8266 WeMos D1 Mini
```yaml
board_type: wemos_d1_mini32
framework_type: arduino
uart_tx_pin: GPIO1
uart_rx_pin: GPIO3
uart_baud_rate: 115200
uart_de_pin: null
```

#### ESP-01
```yaml
board_type: esp01_1m
framework_type: arduino
uart_tx_pin: GPIO1
uart_rx_pin: GPIO3
uart_baud_rate: 115200
uart_de_pin: null
```

## Wiring Diagrams

### Basic 2-Wire RS485 Setup

```
ESP32/ESP8266    RS485 Converter    Balboa Spa
TX (GPIO26)  --> A+ (or DI)
RX (GPIO32)  --> B- (or RO)
GND         --> GND
3.3V/5V     --> VCC
```

### 3-Wire RS485 with Direction Control

```
ESP32/ESP8266    RS485 Converter    Balboa Spa
TX (GPIO26)  --> DI
RX (GPIO32)  --> RO
DE (GPIO25)  --> DE/RE
GND         --> GND
3.3V/5V     --> VCC
```

## Common Pin Assignments

### ESP32 Pin Reference
- **GPIO26**: UART TX (default)
- **GPIO32**: UART RX (default)
- **GPIO17**: Alternative UART TX
- **GPIO16**: Alternative UART RX
- **GPIO25**: Direction control (optional)
- **GPIO27**: Alternative direction control
- **GPIO14**: Alternative direction control

### ESP8266 Pin Reference
- **GPIO1**: UART TX (default)
- **GPIO3**: UART RX (default)
- **GPIO13**: Alternative UART TX
- **GPIO15**: Alternative UART RX

## RS485 Converter Types

### MAX485
- **A+**: Connect to spa A+ or DI
- **B-**: Connect to spa B- or RO
- **DE/RE**: Direction control (optional)
- **VCC**: 3.3V or 5V from ESP
- **GND**: Ground

### SN75176
- **A**: Connect to spa A+ or DI
- **B**: Connect to spa B- or RO
- **DE**: Direction control (optional)
- **VCC**: 3.3V or 5V from ESP
- **GND**: Ground

### Tail485
- **A+**: Connect to spa A+ or DI
- **B-**: Connect to spa B- or RO
- **DE**: Direction control (optional)
- **VCC**: 3.3V or 5V from ESP
- **GND**: Ground

## Troubleshooting

### No Communication
1. **Check Wiring**: Verify all connections are secure
2. **Power**: Ensure RS485 converter has proper power
3. **Baud Rate**: Confirm spa uses 115200 baud (most common)
4. **Pins**: Double-check TX/RX pin assignments
5. **Ground**: Ensure proper ground connection

### Compilation Errors
1. **Board Type**: Verify board type is correct
2. **Framework**: Use `arduino` for most boards
3. **Secrets**: Ensure all required secrets are set

### Common Issues
- **Wrong TX/RX**: Swap TX and RX pins if no communication
- **Wrong Baud Rate**: Try 9600 or 19200 if 115200 doesn't work
- **Power Issues**: Use 3.3V for ESP32, 3.3V or 5V for ESP8266
- **Direction Control**: Set `uart_de_pin: null` if not using direction control

## Quick Test

After setup, check these debug sensors:
- **Debug - Status Byte 16**: Should show changing values
- **Spa Heat State**: Should show heating status
- **Device Status**: Should show "ON"

If these sensors show data, your hardware is working correctly! 