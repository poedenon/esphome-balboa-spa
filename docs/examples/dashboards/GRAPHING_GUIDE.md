# Spa Graphing Guide for Home Assistant

This guide will help you set up beautiful graphs and analytics for your ESPHome Balboa Spa system in Home Assistant.

## 📊 Available Dashboard Configurations

We provide two dashboard configurations for different needs:

### 1. Simple Graphs Dashboard (`spa-dashboard-simple-graphs.yaml`)
- **Best for**: Quick setup, minimal dependencies
- **Uses**: Built-in Home Assistant `history-graph` cards
- **Custom Cards Required**: Only `mushroom-template-card` and `card-mod`
- **Features**: Basic historical data visualization

### 2. Advanced Graphs Dashboard (`spa-dashboard-with-graphs.yaml`)
- **Best for**: Professional analytics, detailed insights
- **Uses**: `mini-graph-card` for advanced charting
- **Custom Cards Required**: `mushroom-template-card`, `card-mod`, and `mini-graph-card`
- **Features**: Interactive charts, custom formatting, advanced analytics

## 🚀 Quick Start Guide

### Step 1: Install Required Custom Cards

1. **Install HACS** (Home Assistant Community Store) if you haven't already
2. **Add Custom Repositories** in HACS:
   - Go to HACS → Frontend → Add Repository
   - Add: `piitaya/lovelace-mushroom`
   - Add: `thomasloven/lovelace-card-mod`
   - For advanced graphs, also add: `kalkih/mini-graph-card`
3. **Install the Custom Cards** via HACS

### Step 2: Add Required Template Sensors

Add these template sensors to your `configuration.yaml`:

```yaml
sensor:
  - platform: time_date
    display_options:
      - "time"
  - platform: template
    sensors:
      spa_pump_1_running_numeric:
        friendly_name: "Spa Pump 1 Running Numeric"
        unique_id: spa_pump_1_running_numeric_sensor
        unit_of_measurement: ""
        value_template: >-
          {% if is_state('binary_sensor.esphome_web_bb4e14_spa_pump_1_running', 'on') %}
            1
          {% else %}
            0
          {% endif %}
      
      spa_pump_2_speed_adjusted:
        friendly_name: "Spa Pump 2 Speed Adjusted"
        unique_id: spa_pump_2_speed_adjusted
        unit_of_measurement: ""
        value_template: >-
          {% set pump2_state = states('sensor.esphome_web_bb4e14_spa_pump_2_speed') | int(0) %}
          {% if pump2_state > 0 %}
            2
          {% else %}
            0
          {% endif %}
```

**Important**: Replace `esphome_web_bb4e14` with your actual device name.

### Step 3: Choose Your Dashboard

#### Option A: Simple Graphs (Recommended for Beginners)
```yaml
# Copy the contents of spa-dashboard-simple-graphs.yaml
# This uses built-in Home Assistant history graphs
```

#### Option B: Advanced Graphs (For Power Users)
```yaml
# Copy the contents of spa-dashboard-with-graphs.yaml
# This provides interactive charts with custom formatting
```

### Step 4: Apply the Dashboard

1. **Create a New Dashboard** in Home Assistant:
   - Go to Settings → Dashboards → Add Dashboard
   - Choose "YAML" mode
   - Paste the configuration from your chosen file

2. **Update Entity Names** (if needed):
   - Replace `esphome_web_bb4e14` with your actual device name
   - Check your entities in Developer Tools → States

## 📈 Available Graphs and Metrics

### Temperature Analytics
- **Current vs Target Temperature**: See how well your spa maintains temperature
- **Heating Cycles**: Visualize when the heater is active
- **Temperature Stability**: Monitor temperature fluctuations

### Pump Usage Analytics
- **Pump 1 Speed Patterns**: Track low/high speed usage (0=OFF, 1=LOW, 2=HIGH)
- **Pump 2 Activity**: Monitor secondary pump usage (0=OFF, 2=ON/HIGH)
- **Usage Patterns**: Identify peak usage times

### Filter Cycle Analytics
- **Filter 1 Schedule**: Visualize primary filter cycles
- **Filter 2 Schedule**: Monitor secondary filter activity
- **Filter Efficiency**: Track filter runtime vs schedule

### System Health Analytics
- **WiFi Signal Strength**: Monitor connection stability
- **Device Uptime**: Track system reliability
- **Connection Quality**: Identify connectivity issues

## 🎯 Key Insights You Can Gain

### Temperature Management
- **Heating Efficiency**: How quickly does your spa reach target temperature?
- **Energy Usage Patterns**: When does the heater run most?
- **Temperature Stability**: How well does your spa maintain temperature?

### Pump Performance
- **Usage Patterns**: When are pumps most active?
- **Speed Preferences**: Do you use low or high speed more?
- **Maintenance Insights**: Track pump runtime for maintenance scheduling

### Filter Health
- **Schedule Compliance**: Are filters running as scheduled?
- **Cycle Duration**: How long do filter cycles typically run?
- **Filter Efficiency**: Track filter performance over time

### System Reliability
- **Connection Stability**: Monitor WiFi signal quality
- **Uptime Tracking**: Track system reliability
- **Performance Trends**: Identify degradation over time

## 🔧 Customization Options

### Time Ranges
You can customize the time ranges for different graphs:

```yaml
# For 24-hour view
hours_to_show: 24

# For 7-day view
hours_to_show: 168

# For 30-day view
hours_to_show: 720
```

### Graph Colors
Customize colors using Home Assistant CSS variables:

```yaml
color: var(--accent-color)      # Orange
color: var(--primary-color)     # Blue
color: var(--success-color)     # Green
color: var(--warning-color)     # Yellow
color: var(--error-color)       # Red
```

### Chart Heights
Adjust chart heights for better visibility:

```yaml
# For history-graph cards
height: 200

# For mini-graph-card
height: 200
```

### Data Aggregation
Control how data is aggregated over time:

```yaml
# Show last value in each time period
aggregate_func: last

# Show maximum value in each time period
aggregate_func: max

# Show average value in each time period
aggregate_func: avg
```

## 📱 Mobile Optimization

Both dashboards are optimized for mobile use:
- **Responsive Design**: Adapts to different screen sizes
- **Touch-Friendly**: Large touch targets for easy interaction
- **Portrait Orientation**: Optimized for phone use
- **Quick Access**: All controls accessible without scrolling

## 🔍 Troubleshooting

### Common Issues

1. **Graphs Not Showing Data**:
   - Check that your entities are reporting data
   - Verify entity names match your configuration
   - Ensure Home Assistant is recording history

2. **Custom Cards Not Loading**:
   - Verify HACS installation
   - Check custom card installation in HACS
   - Clear browser cache and reload

3. **Entity Names Don't Match**:
   - Check your actual entity names in Developer Tools → States
   - Update the dashboard configuration with correct names
   - Your device name will be different from `esphome_web_bb4e14`

4. **Pump Speed Values Showing Incorrectly**:
   - Ensure template sensors are properly configured
   - Check that `spa_pump_2_speed_adjusted` is converting values correctly
   - Verify the `state_map` configuration in mini-graph-cards

### Finding Your Entity Names

1. Go to **Developer Tools → States** in Home Assistant
2. Search for your spa device name
3. Look for entities like:
   - `climate.your_device_name_spa_thermostat`
   - `sensor.your_device_name_spa_pump_1_speed`
   - `binary_sensor.your_device_name_filter_1_running`

## 📊 Advanced Analytics Tips

### Energy Usage Analysis
- Monitor temperature vs heating cycles
- Track pump usage patterns
- Identify peak energy consumption times

### Maintenance Scheduling
- Use pump runtime data for maintenance planning
- Track filter cycle compliance
- Monitor system uptime for reliability

### Performance Optimization
- Identify optimal temperature settings
- Track filter efficiency
- Monitor system health trends

## 🎨 Dashboard Themes

Both dashboards work with Home Assistant themes:
- **Light Theme**: Clean, professional appearance
- **Dark Theme**: Easy on the eyes, modern look
- **Custom Themes**: Fully compatible with custom themes

## 📈 Future Enhancements

Consider these additional analytics:
- **Energy Cost Tracking**: Integrate with energy monitoring
- **Usage Statistics**: Daily/weekly/monthly summaries
- **Predictive Maintenance**: Alert based on usage patterns
- **Weather Integration**: Correlate with weather data

## 🔗 Related Resources

- [ESPHome Balboa Spa Component Documentation](../README.md)
- [Home Assistant History Documentation](https://www.home-assistant.io/integrations/history/)
- [Mini Graph Card Documentation](https://github.com/kalkih/mini-graph-card)
- [Mushroom Template Card Documentation](https://github.com/piitaya/lovelace-mushroom)

## 💡 Pro Tips

1. **Start Simple**: Begin with the simple graphs dashboard
2. **Monitor Regularly**: Check graphs daily for the first week
3. **Customize Gradually**: Add advanced features as you become comfortable
4. **Backup Configurations**: Save your dashboard configurations
5. **Share Insights**: Use graphs to optimize your spa usage
6. **Use Template Sensors**: They help transform data for better visualization
7. **Test Entity Names**: Always verify your actual entity names before copying configurations

---

*This guide will help you get the most out of your spa monitoring system with beautiful, informative graphs and analytics.* 