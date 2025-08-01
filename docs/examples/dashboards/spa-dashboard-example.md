# Spa Dashboard Example

This example shows a beautiful Home Assistant dashboard for controlling and monitoring a Balboa spa system using the ESPHome Balboa Spa component.

## 📱 Dashboard Overview

The dashboard provides a comprehensive interface for spa control with the following key features:

### 🎯 Temperature Control Section
- **Thermostat Card**: Native Home Assistant thermostat control with preset modes
- **Current Status**: Shows "Idle" when spa is not actively heating
- **Current Temperature**: Large display of actual water temperature (80°F)
- **Target Temperature**: Set point display (101°F)
- **Temperature Controls**: Plus/minus buttons for easy adjustment
- **Preset Modes**: Home and Eco mode selection with icons

### 🔧 Component Status Cards
Each spa component is displayed using `mushroom-template-card` with dynamic status:

#### Pump Controls
- **Pump 1**: 
  - Status: LOW (orange), HIGH (green), or OFF (red)
  - Tap to cycle through states
  - Dynamic color coding based on speed
- **Pump 2**: 
  - Status: ON (green) or OFF (red)
  - Tap to toggle on/off
  - Simple binary state with color feedback

#### Lighting
- **Spa Light**: 
  - Status: ON (yellow) or OFF (grey)
  - Tap to toggle
  - Light bulb icon with appropriate colors

#### System Functions
- **Spa Time Sync**: 
  - Displays spa time vs local time
  - Format: "Spa: 2:37pm | Local: 02:37 PM"
  - Tap to sync spa time with local time
  - 12-hour format display

#### Filter Management
- **Filter 1**: 
  - Status: RUNNING (green) or OFF (blue)
  - Schedule display: "12:00am - 4:00am"
  - Read-only status display
- **Filter 2**: 
  - Status: RUNNING (green), OFF (blue), or DISABLED (grey)
  - Schedule display: "12:00pm - 4:00pm"
  - Enhanced status with enabled/disabled states

## 🎨 Design Features

### Visual Design
- **Mushroom Template Cards**: Modern, clean card design
- **Dynamic Color Coding**: 
  - Red: OFF/Disabled states
  - Orange: Low speed/Intermediate states
  - Green: Running/Active states
  - Blue: Scheduled/Standby states
  - Grey: Inactive/Unknown states
- **Grid Layout**: Responsive 4-column grid system
- **Fill Container**: Cards expand to fill available space

### User Experience
- **Touch-Friendly**: Large touch targets for mobile use
- **Real-Time Updates**: Live status updates from spa system
- **Intuitive Controls**: Familiar interface patterns
- **Comprehensive Monitoring**: All spa functions visible at a glance
- **Visual Feedback**: Immediate color changes on state changes

## 📊 Dashboard Configuration

This dashboard uses the following Home Assistant components:

### Required Custom Cards
1. **mushroom-template-card**: Modern card design with templates
   - Repository: https://github.com/piitaya/lovelace-mushroom
2. **card-mod**: Custom CSS styling
   - Repository: https://github.com/thomasloven/lovelace-card-mod

### Installation Steps
1. **Install HACS** (Home Assistant Community Store)
2. **Add Custom Repositories**:
   - `piitaya/lovelace-mushroom`
   - `thomasloven/lovelace-card-mod`
3. **Install Custom Cards** via HACS
4. **Copy Configuration** from `spa-dashboard.yaml`

### Key Features
- **Template Logic**: Advanced Jinja2 templates for dynamic content
- **Conditional Styling**: Colors change based on state
- **Time Formatting**: 12-hour format with AM/PM
- **Grid Layout**: Responsive design with proper spacing
- **Service Calls**: Direct button integration for controls

## 🔧 Entity Mapping

The dashboard uses the following entity naming convention:
- `climate.esphome_web_bb4e14_spa_thermostat` - Temperature control
- `sensor.esphome_web_bb4e14_spa_pump_1_speed` - Pump 1 speed sensor
- `sensor.esphome_web_bb4e14_spa_pump_2_speed` - Pump 2 speed sensor
- `binary_sensor.esphome_web_bb4e14_spa_light_running` - Light status
- `sensor.esphome_web_bb4e14_spa_clock_hour` - Spa clock hour
- `sensor.esphome_web_bb4e14_spa_clock_minute` - Spa clock minute
- `binary_sensor.esphome_web_bb4e14_filter_1_running` - Filter 1 status
- `binary_sensor.esphome_web_bb4e14_filter_2_running` - Filter 2 status
- `binary_sensor.esphome_web_bb4e14_filter_2_enabled` - Filter 2 enabled
- `sensor.esphome_web_bb4e14_filter_1_schedule` - Filter 1 schedule
- `sensor.esphome_web_bb4e14_filter_2_schedule` - Filter 2 schedule

## 📱 Mobile Optimization

The dashboard is optimized for mobile devices with:
- **Responsive Grid**: Adapts to different screen sizes
- **Touch Controls**: Large, easy-to-tap buttons
- **Portrait Orientation**: Optimized for phone use
- **Quick Access**: All controls accessible without scrolling
- **Visual Feedback**: Clear status indicators

## 🎯 Key Benefits

1. **Centralized Control**: All spa functions in one interface
2. **Visual Feedback**: Clear status indicators for all components
3. **Easy Temperature Control**: Native thermostat interface
4. **Filter Management**: Clear scheduling and status display
5. **Time Synchronization**: Keeps spa clock accurate
6. **Mobile Friendly**: Perfect for poolside control
7. **Real-Time Updates**: Live status changes
8. **Professional Design**: Modern, clean interface

## 🔄 Real-Time Updates

The dashboard provides real-time updates for:
- Temperature changes and heating cycles
- Pump speed and state changes
- Filter cycle status and scheduling
- Light state changes
- Time synchronization
- System connectivity status

## 🎨 Customization Options

### Colors
- **Primary**: Orange (#ff8c00) for active states
- **Success**: Green for running/active states
- **Warning**: Orange for intermediate states
- **Error**: Red for off/disabled states
- **Info**: Blue for scheduled/standby states
- **Neutral**: Grey for inactive states

### Layout
- **Grid System**: 4-column responsive layout
- **Card Sizing**: Fill container for optimal space usage
- **Spacing**: Proper margins and padding
- **Typography**: Clear, readable text

### Templates
- **Conditional Logic**: Advanced Jinja2 templates
- **State Detection**: Dynamic content based on entity states
- **Time Formatting**: 12-hour format with proper AM/PM
- **Color Coding**: Automatic color changes based on status

This example demonstrates the full potential of the ESPHome Balboa Spa component when integrated with Home Assistant's powerful dashboard system and modern custom cards. 