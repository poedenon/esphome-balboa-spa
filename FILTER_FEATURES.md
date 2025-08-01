# Balboa Spa Filter Status and Updating Features

This document describes the enhanced filter functionality added to the Balboa Spa ESPHome component.

## Overview

The Balboa Spa component now includes comprehensive filter status tracking and control capabilities, allowing you to:

- Monitor filter runtime and cycle completion
- Track current filter running status
- Monitor filter schedules
- Enable/disable filter 2
- Reset filter runtime counters
- Automate filter maintenance alerts

## New Components

### Binary Sensors

#### Filter Status Sensors
- `filter1_active` - Shows if Filter 1 is enabled (always true)
- `filter2_active` - Shows if Filter 2 is enabled
- `filter1_running` - Shows if Filter 1 is currently running
- `filter2_running` - Shows if Filter 2 is currently running

### Sensors

#### Filter Schedule Sensors
- `filter1_start_hour` - Filter 1 start hour (0-23)
- `filter1_start_minute` - Filter 1 start minute (0-59)
- `filter1_duration_hour` - Filter 1 duration hours (0-23)
- `filter1_duration_minute` - Filter 1 duration minutes (0-59)
- `filter2_start_hour` - Filter 2 start hour (0-23)
- `filter2_start_minute` - Filter 2 start minute (0-59)
- `filter2_duration_hour` - Filter 2 duration hours (0-23)
- `filter2_duration_minute` - Filter 2 duration minutes (0-59)

#### Filter Runtime and Cycle Tracking
- `filter1_runtime_hours` - Total runtime hours for Filter 1
- `filter2_runtime_hours` - Total runtime hours for Filter 2
- `filter1_cycles_completed` - Number of completed Filter 1 cycles
- `filter2_cycles_completed` - Number of completed Filter 2 cycles
- `filter1_current_runtime_minutes` - Current Filter 1 runtime in minutes
- `filter2_current_runtime_minutes` - Current Filter 2 runtime in minutes

### Switches

#### Filter Control
- `filter2_enable` - Enable/disable Filter 2



## Configuration Example

```yaml
# Binary Sensors for Filter Status
binary_sensor:
  - platform: balboa_spa
    balboa_spa_id: balboa_spa
    filter1_active:
      name: "Filter 1 Active"
    filter2_active:
      name: "Filter 2 Active"
    filter1_running:
      name: "Filter 1 Running"
    filter2_running:
      name: "Filter 2 Running"

# Sensors for Filter Timing and Status
sensor:
  - platform: balboa_spa
    balboa_spa_id: balboa_spa
    filter1_runtime_hours:
      name: "Filter 1 Total Runtime"
    filter2_runtime_hours:
      name: "Filter 2 Total Runtime"
    filter1_cycles_completed:
      name: "Filter 1 Cycles Completed"
    filter2_cycles_completed:
      name: "Filter 2 Cycles Completed"
    filter1_current_runtime_minutes:
      name: "Filter 1 Current Runtime"
    filter2_current_runtime_minutes:
      name: "Filter 2 Current Runtime"

# Switches for Control
switch:
  - platform: balboa_spa
    balboa_spa_id: balboa_spa
    filter2_enable:
      name: "Filter 2 Enable"

# Number Components for Filter Schedule Control
number:
  - platform: balboa_spa
    balboa_spa_id: balboa_spa
    name: "Filter 1 Start Hour"
    type: filter1_start_hour
    min_value: 0
    max_value: 23
    step: 1
```

## API Methods

The BalboaSpa class now includes these filter-related methods:

### Filter Status Methods
```cpp
bool is_filter1_running() const;
bool is_filter2_running() const;
uint16_t get_filter1_runtime_hours() const;
uint16_t get_filter2_runtime_hours() const;
uint16_t get_filter1_cycles_completed() const;
uint16_t get_filter2_cycles_completed() const;
uint32_t get_filter1_current_runtime_minutes() const;
uint32_t get_filter2_current_runtime_minutes() const;
```

### Filter Update Methods
```cpp
void set_filter1_schedule(uint8_t start_hour, uint8_t start_minute, 
                         uint8_t duration_hour, uint8_t duration_minute);
void set_filter2_schedule(uint8_t start_hour, uint8_t start_minute, 
                         uint8_t duration_hour, uint8_t duration_minute);
void set_filter2_enable(bool enable);
void reset_filter_runtime(uint8_t filter_number);
void reset_filter_cycles(uint8_t filter_number);
```

## Filter Status Tracking Logic

The component automatically tracks filter status by:

1. **Schedule Analysis**: Comparing current time against filter schedules
2. **Runtime Calculation**: Tracking when filters start/stop running
3. **Cycle Counting**: Incrementing cycle counters when filters complete
4. **State Updates**: Updating running status based on schedule

### Schedule Logic
- Filters run when current time is within their scheduled window
- Handles midnight wrap-around (e.g., 23:00 to 01:00)
- Filter 1 is always enabled, Filter 2 can be disabled
- Duration can be 0 hours and 0 minutes (filter disabled)

### Runtime Tracking
- Runtime accumulates in hours for total tracking
- Current runtime shows minutes for active filters
- Runtime resets when manually called
- Cycle counters increment when filters complete

## Automation Examples

### Filter Maintenance Alert
```yaml
automation:
  - alias: "Filter Maintenance Alert"
    trigger:
      platform: numeric_state
      entity_id: sensor.filter_1_total_runtime
      above: 1000
    action:
      - service: notify.mobile_app
        data:
          message: "Filter 1 has exceeded 1000 hours runtime - consider maintenance"
```

### Reset Runtime After Cycle
```yaml
automation:
  - alias: "Reset Filter Runtime After Cycle"
    trigger:
      platform: numeric_state
      entity_id: sensor.filter_1_cycles_completed
      above: 0
    action:
      - service: number.set_value
        target:
          entity_id: number.filter_1_runtime_reset
        data:
          value: 0
```

### Filter Schedule Optimization
```yaml
automation:
  - alias: "Optimize Filter Schedule for Winter"
    trigger:
      platform: time
      at: "00:00:00"
    condition:
      - platform: numeric_state
        entity_id: sensor.outdoor_temperature
        below: 32
    action:
      - service: number.set_value
        target:
          entity_id: number.filter_1_duration_hours
        data:
          value: 8  # Increase filter time in cold weather
```

## Troubleshooting

### Filter Not Running
1. Check if filter is enabled (`filter2_active` for Filter 2)
2. Verify schedule settings (start time and duration)
3. Check spa time synchronization
4. Review debug logs for filter status updates

### Runtime Not Updating
1. Ensure filter is actually running (`filter1_running` or `filter2_running`)
2. Check that schedule is properly configured
3. Verify spa communication is working
4. Check for any automation that might be resetting counters

### Schedule Issues
1. Verify start time is within 0-23 hours and 0-59 minutes
2. Check duration is not 0 hours and 0 minutes
3. Ensure time zone is correctly configured
4. Test with simple schedules first

## Debug Information

Enable debug logging to see filter status updates:

```yaml
logger:
  level: DEBUG
  logs:
    balboa_spa: DEBUG
```

This will show:
- Filter start/stop events
- Runtime calculations
- Schedule analysis
- Command sending/receiving

## Limitations

- Filter runtime tracking is based on schedule analysis, not direct pump feedback
- Runtime counters are stored in memory and reset on device restart
- Schedule changes require spa communication to be working
- Filter 1 cannot be disabled (spa limitation)
- Maximum duration is 23 hours and 59 minutes per filter 