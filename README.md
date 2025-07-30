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

## 🔍 **Implementation Differences from Reference Repositories**

### **Comparison with brianfeucht/esphome-balboa-spa**

Our implementation differs significantly from the [brianfeucht/esphome-balboa-spa repository](https://github.com/brianfeucht/esphome-balboa-spa) in several key areas:

#### **Pump Status Identification**

**Original Repository (brianfeucht):**
- Uses individual jet switches (`jet1`, `jet2`, `jet3`, `jet4`) as simple on/off controls
- No multi-stage pump support
- No pump speed state monitoring
- Jet controls are treated as independent switches

**Our Implementation:**
- **Discovered actual pump status encoding** in Status Byte 16 (not documented in original)
- **Multi-stage pump support** for Pump 1 (OFF/LOW/HIGH states)
- **Pump interdependency detection** (Pump 2 auto-starts Pump 1)
- **Comprehensive speed monitoring** with numeric and binary sensors
- **Button-based controls** instead of simple switches

#### **Protocol Implementation Differences**

**Original Repository:**
```yaml
switch:
  - platform: balboa_spa
    jet1:
      name: Jet1
    jet2:
      name: Jet2
    jet3:
      name: Jet3
    jet4:
      name: Jet4
```

**Our Implementation:**
```yaml
# Hidden switches for internal control
switch:
  - platform: balboa_spa
    jet1:
      name: "Spa Pump 1 (Hidden)"
      entity_category: diagnostic
    jet2:
      name: "Spa Pump 2 (Hidden)"
      entity_category: diagnostic

# Button-based controls
button:
  - platform: template
    name: "Pump 1 Cycle State"
    # Multi-stage cycling logic
  - platform: template
    name: "Pump 2 Toggle"
    # Simple on/off toggle

# Comprehensive sensor monitoring
sensor:
  - platform: template
    name: "Spa Pump 1 Speed"
    # Shows 0, 1, or 2
  - platform: template
    name: "Spa Pump 2 Speed"
    # Shows 0 or 1
```

#### **Key Discoveries Not in Original**

1. **Status Byte 16 Encoding**: We discovered that pump status is encoded in Status Byte 16 with specific bit patterns:
   - Pump 1: Bits 0-1 (0x03) for OFF/LOW/HIGH
   - Pump 2: Bits 3-4 (0x18) for OFF/ON

2. **Jet vs Pump Relationship**: We identified that jet bits only indicate HIGH speed for multi-stage pumps, not the full pump state.

3. **Pump Interdependency**: We discovered that Pump 2 automatically starts Pump 1 at low speed when activated.

4. **Multi-Stage Control**: We implemented proper cycling through OFF→LOW→HIGH→OFF states.

#### **Sensor Implementation Differences**

**Original Repository:**
- Basic binary sensors for jet states
- No speed monitoring
- No interdependency detection

**Our Implementation:**
- **Speed sensors**: Numeric values showing actual pump speeds
- **State-specific binary sensors**: Low/High speed indicators
- **Interdependency sensors**: Detection of auto-start behavior
- **Debug sensors**: Raw protocol byte monitoring for troubleshooting

#### **Control Interface Differences**

**Original Repository:**
- Simple on/off switches
- No state feedback
- No multi-stage support

**Our Implementation:**
- **Button-based controls** with state cycling
- **Visual state feedback** with color-coded dashboard cards
- **Multi-stage support** for complex pump configurations
- **Comprehensive monitoring** of all pump states

#### **Why These Differences Matter**

1. **Accuracy**: Our implementation provides accurate pump state monitoring vs. simple on/off indicators
2. **Functionality**: Multi-stage pump support enables full spa control capabilities
3. **User Experience**: Button-based controls with visual feedback are more intuitive
4. **Troubleshooting**: Debug sensors and protocol monitoring aid in problem diagnosis
5. **Automation**: Comprehensive sensor data enables more sophisticated Home Assistant automations

These differences represent significant improvements in understanding and implementing the Balboa spa protocol, providing users with more accurate, functional, and user-friendly spa control systems.

## 🔬 **Technical Protocol Analysis: Bit/Byte Level Differences**

### **Balboa Protocol Documentation vs. Implementation Reality**

#### **Status Update Message Structure (FF AF 13)**
According to the protocol documentation:
```
 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
F0 F1 CT HH MM F2 00 00 00 F3 F4 PP 00 F5 LF F6 00 00 00 00 ST 00 00 00
```

**Protocol Claims:**
- **Byte 17 (PP)**: Pump status with 2-bit values for each pump
  - Pump 1: Bits 0-1 (0x03)
  - Pump 2: Bits 2-3 (0x0C) 
  - Pump 3: Bits 4-5 (0x30)

**Our Testing Results:**
- **Byte 17**: Always shows 0x00 (no pump status)
- **Byte 16**: Contains actual pump status information
  - Pump 1: Bits 0-1 (0x03) - Values: 0=OFF, 1=LOW, 2=HIGH
  - Pump 2: Bits 3-4 (0x18) - Values: 0=OFF, 8=ON (bit 3 set)

#### **Jet vs Pump Relationship Discovery**

**Original Repository Implementation:**
```cpp
// brianfeucht implementation - reads individual jet bits
spa_component_state = bitRead(input_queue[16], 1);  // Jet 1
spa_component_state = bitRead(input_queue[16], 3);  // Jet 2
```

**Our Implementation:**
```cpp
// Our implementation - reads actual pump status
uint8_t pump1_status = input_queue[16] & 0x03;      // Pump 1: bits 0-1
uint8_t pump2_status = (input_queue[16] & 0x18) >> 3; // Pump 2: bits 3-4
```

**Key Discovery:**
- **Jet 1 bit** (byte 16, bit 1): Only indicates HIGH speed for Pump 1
  - `0` = Pump 1 is OFF or LOW
  - `1` = Pump 1 is HIGH
- **Jet 2 bit** (byte 16, bit 3): Indicates Pump 2 ON state
  - `0` = Pump 2 is OFF
  - `1` = Pump 2 is ON

### **Bit-Level Analysis of Status Byte 16**

#### **Original Repository Interpretation:**
```cpp
// Reads individual bits as independent jet states
jet1 = bitRead(input_queue[16], 1);  // Single bit: 0 or 1
jet2 = bitRead(input_queue[16], 3);  // Single bit: 0 or 1
```

#### **Our Corrected Interpretation:**
```cpp
// Reads 2-bit values for actual pump states
pump1 = input_queue[16] & 0x03;           // Bits 0-1: 00, 01, 10
pump2 = (input_queue[16] & 0x18) >> 3;    // Bits 3-4: 00, 01
```

#### **Bit Pattern Analysis:**
```
Byte 16: [bit7][bit6][bit5][bit4][bit3][bit2][bit1][bit0]
         [  ? ][  ? ][  ? ][P2_H][P2_L][  ? ][P1_H][P1_L]

Pump 1 (bits 0-1):
- 00 = OFF
- 01 = LOW  
- 10 = HIGH
- 11 = HIGH (fallback)

Pump 2 (bits 3-4):
- 00 = OFF
- 01 = ON
- 10 = ON (alternative encoding)
- 11 = ON (alternative encoding)
```

### **Protocol Documentation Errors**

#### **Incorrect Byte Assignment:**
- **Documentation claims**: Pump status in Byte 17 (PP)
- **Reality**: Pump status in Byte 16
- **Impact**: Original implementations read wrong byte, missing multi-stage states

#### **Incorrect Bit Interpretation:**
- **Documentation suggests**: Individual jet bits represent independent states
- **Reality**: Jet bits are subset indicators of actual pump states
- **Impact**: Original implementations show incomplete state information

#### **Missing Interdependency:**
- **Documentation**: No mention of pump interdependency
- **Reality**: Pump 2 auto-starts Pump 1 at low speed
- **Impact**: Original implementations don't detect automatic pump activation

### **Implementation Differences Summary**

| Aspect | Protocol Doc | brianfeucht Repo | Our Implementation |
|--------|-------------|------------------|-------------------|
| **Pump Status Byte** | Byte 17 (PP) | Byte 16 (incorrect) | Byte 16 (correct) |
| **Pump 1 Encoding** | 2-bit (0x03) | 1-bit (bit 1) | 2-bit (0x03) |
| **Pump 2 Encoding** | 2-bit (0x0C) | 1-bit (bit 3) | 2-bit (0x18) |
| **Multi-Stage Support** | Implied | No | Yes |
| **Interdependency** | Not documented | Not detected | Detected |
| **State Accuracy** | Theoretical | Partial | Complete |

### **Why These Differences Matter**

1. **Protocol Accuracy**: Our implementation correctly reads the actual pump states
2. **Multi-Stage Support**: We can detect and control LOW/HIGH states, not just ON/OFF
3. **State Completeness**: We show the full pump state, not just jet indicators
4. **Interdependency Awareness**: We detect when pumps automatically start each other
5. **Debug Capability**: We can monitor raw protocol bytes for troubleshooting

This technical analysis demonstrates that our implementation provides the most accurate and complete interpretation of the Balboa spa protocol, correcting several errors in the original documentation and implementation.

