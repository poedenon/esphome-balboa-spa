# 🌊 Balboa Spa Monitor - ESPHome Configuration

An ESP32-based project for monitoring and controlling Balboa spa systems using ESPHome with RS485 communication. This project provides comprehensive spa monitoring and control capabilities with multi-stage pump support.

## 🛠️ Required Components

### 💻 Core Hardware
- **ESP32 Device** (Atom Lite, M5Tough, or similar) - The main ESP32 controller
- **RS485 Module** (Tail485, M5Stack RS485, or similar) - RS485 communication module

### 🔌 Cable Connection
- **4-Pin Molex Cable** for connecting the RS485 module to your Balboa spa's RS485 communication lines

⚠️ **ATTENTION**: Power the ESP32 from the Balboa spa system via the RS485 Unit using 12V and GND. All 4 wires are required (12V, GND, A+, B-).

### ➕ Additional Requirements
- **Balboa Spa System** - Compatible spa system with accessible RS485 communication port
- **Network Connection** - WiFi network for MQTT and web interface connectivity

## 📊 Current Implementation Status

### ✅ **Fully Implemented Features**
- ✅ **Real-time Spa Monitoring**: Temperature, pumps, lights, heater, and filter status
- ✅ **Multi-Stage Pump Control**: Support for pumps with OFF/LOW/HIGH states
- ✅ **Web Interface**: ESPHome web interface accessible from any device
- ✅ **RS485 Communication**: Direct communication with Balboa spa control systems
- ✅ **ESPHome Integration**: Native ESPHome platform with optimized performance
- ✅ **MQTT Integration**: Full MQTT broker connectivity with Last Will Testament (LWT) messages
- ✅ **Button-Based Controls**: Intuitive pump control via buttons instead of switches
- ✅ **Comprehensive Sensors**: Speed, running status, and interdependency monitoring

### 📋 **Future Enhancements**
- ⬜ **Touch Screen Display**: Local spa status and controls (if using M5Tough)
- ⬜ **Enhanced UI**: Custom touch interface for display-equipped devices
- ⬜ **Home Assistant Discovery**: Auto-discovery integration (currently disabled for manual MQTT setup)

## 🔍 **Protocol Discoveries & Implementation**

### **Pump Status Encoding (Status Byte 16)**
Through extensive testing and debugging, we discovered that pump status is encoded in **Status Byte 16** of the Balboa protocol, not in separate jet bits or byte 17 as originally documented.

#### **Pump 1 (Multi-Stage Pump):**
- **Bits 0-1 (0x03)**: Pump speed state
  - `00` = OFF
  - `01` = LOW speed
  - `10` = HIGH speed

#### **Pump 2 (On/Off Pump):**
- **Bits 3-4 (0x18)**: Pump state
  - `00` = OFF
  - `08` = ON (bit 3 set)

### **Pump Interdependency**
We discovered that Pump 2 has an interdependency with Pump 1:
- **When Pump 2 turns ON**: Pump 1 automatically starts at LOW speed
- **When Pump 2 turns OFF**: Pump 1 automatically turns OFF
- This appears to be a safety/circulation feature in the spa design

### **Jet vs Pump Relationship**
- **Jet 1 bit** (byte 16, bit 1): Only indicates HIGH speed for Pump 1
  - `0` = Pump 1 is OFF or LOW
  - `1` = Pump 1 is HIGH
- **Jet 2 bit** (byte 16, bit 3): Indicates Pump 2 ON state
  - `0` = Pump 2 is OFF
  - `1` = Pump 2 is ON

## 🎮 **Control Implementation**

### **Button-Based Controls**
Instead of traditional switches, we implemented button controls for better user experience:

#### **Pump 1 Cycle State Button:**
- **Press 1**: OFF → LOW
- **Press 2**: LOW → HIGH
- **Press 3**: HIGH → OFF
- **Press 4**: OFF → LOW (cycle repeats)

#### **Pump 2 Toggle Button:**
- **Press 1**: OFF → ON
- **Press 2**: ON → OFF
- **Press 3**: OFF → ON (toggle continues)

### **Sensor Implementation**
Comprehensive sensor suite for monitoring:

#### **Speed Sensors:**
- **Spa Pump 1 Speed**: Shows 0, 1, or 2
- **Spa Pump 2 Speed**: Shows 0 or 1

#### **Binary Sensors:**
- **Spa Pump 1 Low Speed**: ON when speed = 1
- **Spa Pump 1 High Speed**: ON when speed = 2
- **Spa Pump 1 Running**: ON when speed > 0
- **Spa Pump 2 Running**: ON when speed > 0
- **Pump 2 Auto-Started Pump 1**: ON when interdependency active

## 🔧 **Hardware Configuration**

### **ESP32 Platform**
- **Board**: ESP32-based device (Atom Lite, M5Tough, etc.)
- **Connectivity**: WiFi for MQTT and web interface

### **RS485 Connection**
- **TX Pin**: GPIO 26 (Tail485) or GPIO 32 (M5Tough)
- **RX Pin**: GPIO 32 (Tail485) or GPIO 33 (M5Tough)
- **RS485 Lines**: Connect A/B differential pair to your Balboa spa system

## 📡 **Protocol Implementation**

### **Status Message Decoding**
The component correctly decodes the Balboa protocol status messages:
- **Message Structure**: 0x7E delimited with CRC-8 validation
- **Status Updates**: Received every second from the spa
- **Pump Status**: Decoded from Status Byte 16 with proper bit masking

### **Command Structure**
Commands follow the Balboa protocol format:
- **Toggle Items**: 0A BF 11 with item ID
- **Set Temperature**: 0A BF 20 with temperature value
- **Set Time**: 0A BF 21 with hour/minute

## 📁 **Project Structure**

```
├── esphome-atomlite-balboa-spa.yaml    # Main ESPHome configuration
├── components/balboa_spa/              # Custom Balboa spa component
│   ├── balboaspa.cpp                   # Main component implementation
│   ├── balboaspa.h                     # Component header
│   ├── spa_state.h                     # State structure definitions
│   └── binary_sensor/                  # Binary sensor implementations
├── secrets.yaml                        # WiFi and sensitive configuration  
├── secrets_template.yaml               # Template for new installations
└── README.md                           # This documentation
```

## 🚀 **Getting Started**

### 📋 **Prerequisites**
- ESPHome installed (`pip install esphome`)
- ESP32 device with RS485 module
- Access to Balboa spa RS485 communication lines

### 🔧 **Initial Setup**

1. **Configure Secrets**: Copy `secrets_template.yaml` to `secrets.yaml` and configure your WiFi and MQTT settings
2. **Compile**: `esphome compile esphome-atomlite-balboa-spa.yaml`
3. **First Upload**: Connect ESP32 via USB and upload initial firmware
4. **OTA Updates**: After initial setup, use OTA for subsequent updates

## 🔨 **Building and Uploading**

### **Using ESPHome**

```bash
# Compile the configuration
esphome compile esphome-atomlite-balboa-spa.yaml

# Upload to device (first time)
esphome upload esphome-atomlite-balboa-spa.yaml

# Upload over-the-air (subsequent updates)
esphome run esphome-atomlite-balboa-spa.yaml
```

## 🎯 **Usage Examples**

### **Home Assistant Automations**
```yaml
# Turn on Pump 1 when temperature drops
automation:
  - alias: "Turn on Pump 1 when cold"
    trigger:
      platform: numeric_state
      entity_id: sensor.spa_current_temperature
      below: 95
    action:
      - service: button.press
        target:
          entity_id: button.pump_1_cycle_state

# Notify when Pump 2 auto-starts Pump 1
automation:
  - alias: "Pump interdependency notification"
    trigger:
      platform: state
      entity_id: binary_sensor.pump_2_auto_started_pump1
      to: "on"
    action:
      - service: notify.mobile_app
        data:
          message: "Pump 2 automatically started Pump 1"
```

### **Dashboard Configuration**

#### **Mushroom Template Cards (Recommended)**
For the best visual experience with state-based colors:

```yaml
# Pump 1 - Multi-stage with color coding
type: custom:mushroom-template-card
primary: Pump 1
secondary: >
  {% set speed = states('sensor.esphome_web_bb4e14_spa_pump_1_speed') | int(0) %}
  {% if speed == 0 %}
    OFF
  {% elif speed == 1 %}
    LOW
  {% elif speed == 2 %}
    HIGH
  {% else %}
    Unknown
  {% endif %}
icon: mdi:pump
icon_color: >
  {% set speed = states('sensor.esphome_web_bb4e14_spa_pump_1_speed') | int(0) %}
  {% if speed == 0 %}
    red
  {% elif speed == 1 %}
    orange
  {% elif speed == 2 %}
    green
  {% else %}
    grey
  {% endif %}
fill_container: true
tap_action:
  action: call-service
  service: button.press
  target:
    entity_id: button.esphome_web_bb4e14_pump_1_cycle_state

# Pump 2 - On/Off with color coding
type: custom:mushroom-template-card
primary: Pump 2
secondary: >
  {% set speed = states('sensor.esphome_web_bb4e14_spa_pump_2_speed') | int(0) %}
  {% if speed == 0 %}
    OFF
  {% else %}
    ON
  {% endif %}
icon: mdi:pump
icon_color: >
  {% set speed = states('sensor.esphome_web_bb4e14_spa_pump_2_speed') | int(0) %}
  {% if speed == 0 %}
    red
  {% else %}
    green
  {% endif %}
fill_container: true
tap_action:
  action: call-service
  service: button.press
  target:
    entity_id: button.esphome_web_bb4e14_pump_2_toggle
```

#### **Alternative Dashboard Layouts**

**Basic Button Cards:**
```yaml
type: vertical-stack
cards:
  - type: button
    entity: button.pump_1_cycle_state
    name: "Pump 1"
    icon: mdi:pump
  - type: button
    entity: button.pump_2_toggle
    name: "Pump 2"
    icon: mdi:pump
  - type: entities
    entities:
      - entity: sensor.spa_pump_1_speed
      - entity: sensor.spa_pump_2_speed
      - entity: sensor.spa_current_temperature
```

**Status Monitoring Cards:**
```yaml
type: entities
title: Spa Status
entities:
  - entity: sensor.spa_current_temperature
    name: Current Temperature
  - entity: sensor.spa_target_temperature
    name: Target Temperature
  - entity: sensor.spa_pump_1_speed
    name: Pump 1 Speed
  - entity: sensor.spa_pump_2_speed
    name: Pump 2 Speed
  - entity: binary_sensor.spa_pump_1_running
    name: Pump 1 Running
  - entity: binary_sensor.spa_pump_2_running
    name: Pump 2 Running
state_color: true
```

## 🔍 **Troubleshooting**

### **Common Issues**

1. **Pump states not updating**: Check RS485 connection and spa communication
2. **Button not responding**: Verify the pump switch IDs are correctly referenced
3. **MQTT disconnections**: Check network stability and MQTT broker settings

### **Debug Sensors**
The configuration includes comprehensive debug sensors:
- **Debug - Status Byte 16-19**: Raw protocol bytes
- **Debug - Pump Interdependency**: Combined pump states
- **Debug - Jet Raw**: Individual jet bit states

## 📚 **References**

This project builds upon excellent reference implementations:
- **[ESPHome Balboa Spa (Dakoriki)](https://github.com/Dakoriki/ESPHome-Balboa-Spa)** - Original ESPHome Balboa spa integration
- **[ESPHome Balboa Spa (brianfeucht)](https://github.com/brianfeucht/ESPHome-Balboa-Spa)** - Enhanced implementation
- **[ESPHome Balboa Spa (mhetzi)](https://github.com/mhetzi/esphome-balboa-spa)** - Most current version

## 🤝 **Contributing**

This project represents significant protocol discoveries and implementation improvements. Contributions are welcome, especially for:
- Additional spa model support
- Enhanced UI implementations
- Protocol documentation improvements
- Bug fixes and optimizations

