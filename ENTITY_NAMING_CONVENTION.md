# ESPHome & Home Assistant Entity Naming Convention

## Overview
This document defines a consistent naming convention for ESPHome Balboa Spa entities that works within ESPHome limitations while providing clear, descriptive names for Home Assistant integration.

## Core Naming Structure

### Format
```
{domain}.{device_type}_{location}_{function}_{instance}
```

### Components
- **domain**: Entity type (sensor, switch, binary_sensor, climate, button, text_sensor)
- **device_type**: Device category (spa, pump, filter, light, temp, etc.)
- **location**: Physical location (backyard, spa, etc.)
- **function**: Specific function or measurement
- **instance**: Number or identifier for multiple instances

## ESPHome Configuration Guidelines

### 1. Entity ID Naming
Use descriptive, lowercase names with underscores:
```yaml
# Good
id: spa_pump1_speed
id: spa_temp_current
id: spa_filter1_status

# Avoid
id: pump1
id: temp
id: filter1
```

### 2. Friendly Name Naming
Use clear, human-readable names:
```yaml
# Good
name: "Spa Pump 1 Speed"
name: "Spa Temperature"
name: "Filter 1 Status"

# Avoid
name: "Pump 1"
name: "Temperature"
name: "Filter"
```

### 3. Entity Categories
Use appropriate entity categories for organization:
- `diagnostic`: System status, debug info, technical data
- `config`: Configuration entities
- `primary`: Main user-facing controls and sensors

## Specific Entity Type Conventions

### Climate Controls
```yaml
climate:
  - platform: balboa_spa
    name: "Spa Thermostat"
    id: spa_thermostat
    # Entity ID: climate.spa_thermostat
```

### Switches
```yaml
switch:
  - platform: balboa_spa
    jet1:
      name: "Spa Pump 1"
      id: spa_pump1_control
      # Entity ID: switch.spa_pump1_control
    jet2:
      name: "Spa Pump 2"
      id: spa_pump2_control
      # Entity ID: switch.spa_pump2_control
    light:
      name: "Spa Light"
      id: spa_light_control
      # Entity ID: switch.spa_light_control
```

### Sensors
```yaml
sensor:
  - platform: balboa_spa
    heatstate:
      name: "Spa Heat State"
      id: spa_heat_state
      # Entity ID: sensor.spa_heat_state
    circulation:
      name: "Spa Circulation"
      id: spa_circulation
      # Entity ID: sensor.spa_circulation
```

### Binary Sensors
```yaml
binary_sensor:
  - platform: balboa_spa
    filter1_active:
      name: "Filter 1 Active"
      id: spa_filter1_active
      # Entity ID: binary_sensor.spa_filter1_active
    filter2_active:
      name: "Filter 2 Active"
      id: spa_filter2_active
      # Entity ID: binary_sensor.spa_filter2_active
```

### Buttons
```yaml
button:
  - platform: template
    name: "Pump 1 Cycle"
    id: spa_pump1_cycle
    # Entity ID: button.spa_pump1_cycle
  - platform: template
    name: "Sync Spa Time"
    id: spa_time_sync
    # Entity ID: button.spa_time_sync
```

### Text Sensors
```yaml
text_sensor:
  - platform: template
    name: "Spa Time"
    id: spa_time_display
    # Entity ID: text_sensor.spa_time_display
  - platform: template
    name: "Filter 1 Schedule"
    id: spa_filter1_schedule
    # Entity ID: text_sensor.spa_filter1_schedule
```

## Template Sensors & Binary Sensors

### Speed/State Sensors
```yaml
sensor:
  - platform: template
    name: "Spa Pump 1 Speed"
    id: spa_pump1_speed
    # Entity ID: sensor.spa_pump1_speed
  - platform: template
    name: "Spa Pump 2 Speed"
    id: spa_pump2_speed
    # Entity ID: sensor.spa_pump2_speed
```

### Status Binary Sensors
```yaml
binary_sensor:
  - platform: template
    name: "Spa Pump 1 Running"
    id: spa_pump1_running
    # Entity ID: binary_sensor.spa_pump1_running
  - platform: template
    name: "Spa Pump 2 Running"
    id: spa_pump2_running
    # Entity ID: binary_sensor.spa_pump2_running
```

## Debug/Diagnostic Entities

### Debug Sensors
```yaml
sensor:
  - platform: template
    name: "Debug - Jet 1 Raw"
    id: debug_jet1_raw
    entity_category: diagnostic
    # Entity ID: sensor.debug_jet1_raw
  - platform: template
    name: "Debug - Status Byte 16"
    id: debug_status_byte_16
    entity_category: diagnostic
    # Entity ID: sensor.debug_status_byte_16
```

### System Status
```yaml
binary_sensor:
  - platform: status
    name: "Device Status"
    entity_category: diagnostic
    # Entity ID: binary_sensor.device_status

sensor:
  - platform: wifi_signal
    name: "WiFi Signal"
    entity_category: diagnostic
    # Entity ID: sensor.wifi_signal
  - platform: uptime
    name: "Uptime"
    entity_category: diagnostic
    # Entity ID: sensor.uptime
```

## Home Assistant Integration

### Area Assignment
Assign all spa entities to a "Backyard" or "Spa" area in Home Assistant for better organization.

### Device Grouping
Group related entities under a single "Balboa Spa Monitor" device in Home Assistant.

### Friendly Names in Home Assistant
Use the friendly names defined in ESPHome, but consider adding location context:
- "Spa Pump 1" → "Backyard Spa Pump 1"
- "Spa Temperature" → "Backyard Spa Temperature"

## Example Entity ID Mapping

| ESPHome ID | Entity ID | Friendly Name | Category |
|------------|-----------|---------------|----------|
| `spa_thermostat` | `climate.spa_thermostat` | "Spa Thermostat" | primary |
| `spa_pump1_control` | `switch.spa_pump1_control` | "Spa Pump 1" | primary |
| `spa_pump1_speed` | `sensor.spa_pump1_speed` | "Spa Pump 1 Speed" | diagnostic |
| `spa_pump1_running` | `binary_sensor.spa_pump1_running` | "Spa Pump 1 Running" | diagnostic |
| `spa_filter1_active` | `binary_sensor.spa_filter1_active` | "Filter 1 Active" | diagnostic |
| `spa_time_sync` | `button.spa_time_sync` | "Sync Spa Time" | config |
| `spa_time_display` | `text_sensor.spa_time_display` | "Spa Time" | diagnostic |
| `debug_jet1_raw` | `sensor.debug_jet1_raw` | "Debug - Jet 1 Raw" | diagnostic |

## Implementation Checklist

- [ ] Update all entity IDs to follow the convention
- [ ] Ensure all entities have descriptive friendly names
- [ ] Assign appropriate entity categories
- [ ] Group entities in Home Assistant by area
- [ ] Create device grouping in Home Assistant
- [ ] Update any automations to use new entity IDs
- [ ] Test all entity functionality after renaming

## Benefits

1. **Consistency**: All entities follow the same naming pattern
2. **Clarity**: Entity IDs are self-documenting
3. **Organization**: Easy to group and filter entities
4. **Maintainability**: Clear structure for future additions
5. **Home Assistant Integration**: Works well with HA's entity management
6. **Debugging**: Easy to identify entities in logs and external tools 