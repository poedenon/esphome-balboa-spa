# M5Tough Balboa Spa Monitor

An ESP32-based project for monitoring Balboa spa systems using the M5Tough device with RS485 communication. This project is built on ESPHome and provides real-time spa monitoring capabilities.

## Baseline and Current Status

This project is based on the excellent [ESPHome Balboa Component](https://github.com/mstaiger/esphome-balboa and https://github.com/mhetzi/esphome-balboa-spa) implementation, adapted specifically for the M5Tough ESP32 platform.

### Currently Implemented
- **Real-time Spa Monitoring**: Temperature, jets, lights, heater, and filter status
- **Web Interface**: ESPHome web interface accessible from any device
- **RS485 Communication**: Direct communication with Balboa spa control systems via Grove Port A
- **ESPHome Integration**: Native ESPHome platform with optimized performance
- **Basic Controls**: Set temperature, control jets, lights, and heating modes via web interface

### Todo List
- **Touch Screen Display**: M5Tough TFT display output for local spa status and controls
- **MQTT Integration**: MQTT broker connectivity for enhanced home automation integration
- **Enhanced UI**: Custom touch interface for the M5Tough display

## Hardware Requirements

- **M5Tough ESP32 Device**: ESP32-based development board with integrated TFT display and touch screen
- **RS485 Unit**: Connected to Grove Port A for spa communication
- **Balboa Spa System**: Compatible spa system with RS485 communication support

## Hardware Configuration

### M5Tough ESP32 Platform
- **Base**: ESP32 microcontroller with WiFi capability
- **Display**: 2.4" TFT LCD with touch screen (currently unused - on todo list)
- **Connectivity**: Grove ports for easy sensor/module connection

### RS485 Connection via Grove Port A
- **Grove Port A**: Used for RS485 Unit connection
- **TX Pin**: GPIO 32 (connected to RS485 Unit TX)
- **RX Pin**: GPIO 33 (connected to RS485 Unit RX)
- **RS485 Lines**: Connect A/B differential pair to your Balboa spa system

## Protocol Implementation

This project uses the ESPHome Balboa component for communication protocol handling:

- Status message parsing and control commands
- Temperature monitoring and thermostat control
- Pump, jet, and accessory status and control
- Connection monitoring and error handling
- Optimized UART communication to reduce CRC errors

## Project Structure

```
├── esphome-m5tough-balboa-spa.yaml    # Main ESPHome configuration
├── secrets.yaml                       # WiFi and sensitive configuration  
├── secrets_template.yaml              # Template for new installations
└── README.md                          # This documentation
```

## Getting Started

### Prerequisites
- ESPHome installed (`pip install esphome`)
- M5Tough device with RS485 Unit on Grove Port A
- Access to Balboa spa RS485 communication lines

### Initial Setup

1. **Configure Secrets**: Copy `secrets_template.yaml` to `secrets.yaml` and configure your WiFi settings
2. **Compile**: `esphome compile esphome-m5tough-balboa-spa.yaml`
3. **First Upload**: Connect M5Tough via USB and upload initial firmware
4. **OTA Updates**: After initial setup, use OTA for subsequent updates

## Building and Uploading

### Using ESPHome

```bash
# Compile the configuration
esphome compile esphome-m5tough-balboa-spa.yaml

# Upload to M5Tough device via USB (first time)
esphome upload esphome-m5tough-balboa-spa.yaml

# Upload via OTA (after initial setup - replace with your device's IP)
esphome upload esphome-m5tough-balboa-spa.yaml --device 192.168.1.xxx

# Monitor logs
esphome logs esphome-m5tough-balboa-spa.yaml --device 192.168.1.xxx
```

## Configuration

The main configuration is in `esphome-m5tough-balboa-spa.yaml`. Key settings:

- **WiFi**: Configure in `secrets.yaml`
- **RS485**: Uses UART with TX=GPIO32, RX=GPIO33 (Grove Port A)
- **Display**: M5Tough TFT display (todo - not yet implemented)
- **Sensors**: Temperature sensors with ESPHome Balboa component
- **Climate**: Thermostat control for spa temperature

## Current Functionality

### Web Interface Access
Once uploaded, access the spa controls via:
1. **ESPHome Web Interface**: Navigate to your device's IP address in a browser
2. **Home Assistant**: Auto-discovered if you have Home Assistant on your network
3. **API**: ESPHome provides a native API for integration

### Available Controls
The system currently monitors and controls:
- **Temperature**: Current and target temperature readings
- **Climate Control**: Heat mode (Off/Heat/Rest/Ready)
- **Jets**: Jet 1 and Jet 2 on/off control  
- **Light**: Spa light on/off control
- **Heater**: Heater operation status monitoring
- **Filter**: Filter pump operation status
- **Connection**: Real-time RS485 connection status

## Future Enhancements (Todo)

### M5Tough Display Integration
- Custom ESPHome display component for the M5Tough TFT screen
- Touch screen interface for local spa control
- Real-time status display on device
- Temperature, jets, and light status visualization

### MQTT Integration  
- MQTT broker connectivity for enhanced automation
- Integration with non-Home Assistant systems
- Custom MQTT topics for spa status and control
- Retained messages for status persistence

## Troubleshooting

### RS485 Communication Issues
- **Check Physical Connections**: Ensure RS485 Unit is properly connected to Grove Port A
  - TX: GPIO 32
  - RX: GPIO 33  
  - A/B lines connected to spa system
- **Verify Baud Rate**: Confirm 115200 baud rate in ESPHome configuration
- **Check Polarity**: Ensure proper A/B line polarity (swap if no communication)
- **Monitor Logs**: Look for CRC errors or connection timeouts

### Web Interface Issues
- **Network Connectivity**: Verify M5Tough has WiFi connection
- **IP Address**: Check device logs for assigned IP address
- **Firewall**: Ensure no firewall blocking ESPHome web interface
- **Browser Cache**: Clear cache if interface appears stale

### ESPHome Logs and Monitoring

Monitor device operation with:
```bash
esphome logs esphome-m5tough-balboa-spa.yaml --device 192.168.1.xxx
```

Watch for:
- RS485 connection status updates  
- Temperature reading success/failures
- CRC error messages
- Status change notifications
- WiFi connection status

## Technical Details

### GPIO Pin Assignment (M5Tough)
- **Grove Port A TX**: GPIO 32 (connected to RS485 Unit)
- **Grove Port A RX**: GPIO 33 (connected to RS485 Unit)
- **Display Interface**: Internal TFT connection (not yet utilized)
- **Touch Screen**: Internal touch controller (not yet utilized)

### ESPHome Configuration Highlights
- **Platform**: ESP32 with M5Tough board definition
- **UART Configuration**: Hardware serial for reliable RS485 communication
- **Balboa Component**: Official ESPHome Balboa integration
- **OTA Updates**: Over-the-air firmware updates enabled

## Credits and References

This project builds upon excellent work from the ESPHome community:
- **Base Implementation**: [ESPHome Balboa Component](https://github.com/mstaiger/esphome-balboa)
- **Protocol Documentation**: ESPHome Balboa component documentation
- **Hardware Platform**: M5Stack M5Tough ESP32 development board
- **Framework**: ESPHome home automation platform

## Contributing

Future contributions welcome for:
- M5Tough display implementation
- MQTT integration features  
- Touch screen interface development
- Protocol optimization and error handling improvements
