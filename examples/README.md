# Examples Directory

This directory contains practical examples and configurations for using the ESPHome Balboa Spa component.

## 📁 Available Examples

### 📱 Dashboards
- **`spa-dashboard-example.md`**: Detailed description of the spa dashboard interface
- **`spa-dashboard.yaml`**: Complete Home Assistant dashboard configuration

## 🎯 Dashboard Example

The dashboard example shows a beautiful, mobile-optimized interface for controlling your spa:

### Key Features
- **Temperature Control**: Circular gauge with current/target temperature display
- **Component Status**: Real-time status of pumps, lights, and filters
- **Filter Management**: Schedule display and running status
- **Quick Actions**: Easy access to common functions
- **Mobile Optimized**: Touch-friendly interface for poolside use

### Screenshot
The example is based on a real Home Assistant dashboard showing:
- Current temperature: 80°F
- Target temperature: 101°F
- Pump 1: Running at LOW speed (orange)
- Pump 2: OFF (red)
- Spa Light: OFF (grey)
- Filter 1: OFF (blue) - scheduled 12:00am - 4:00am
- Filter 2: RUNNING (green) - scheduled 12:00pm - 4:00pm
- Time sync: Spa 2:37pm | Local 02:37 PM

## 🚀 Getting Started

1. **Install Required Components**:
   ```bash
   # Install custom cards in Home Assistant
   # - mini-graph-card
   # - button-card
   # - stack-in-card
   ```

2. **Copy Dashboard Configuration**:
   ```bash
   # Copy the YAML configuration to your Home Assistant dashboard
   cp examples/dashboards/spa-dashboard.yaml /path/to/your/dashboard/
   ```
   
3. **Install Required Custom Cards**:
   - Install HACS (Home Assistant Community Store)
   - Add `piitaya/lovelace-mushroom` repository
   - Add `thomasloven/lovelace-card-mod` repository
   - Install both custom cards via HACS

3. **Customize for Your Setup**:
   - Update entity names to match your configuration
   - Adjust colors and styling as needed
   - Add or remove components based on your spa features

## 🎨 Customization

### Colors
- Primary: `#ff8c00` (Orange)
- Secondary: `#808080` (Grey)
- Background: `#f5f5f5` (Light Grey)

### Icons
- Temperature: `mdi:thermometer`
- Pumps: `mdi:pump`
- Lights: `mdi:lightbulb`
- Filters: `mdi:filter`
- Clock: `mdi:clock-edit`
- Stats: `mdi:chart-areaspline-variant`

### Layout
- Responsive design for mobile and desktop
- Card-based layout for easy organization
- Touch-friendly controls for mobile use

## 📱 Mobile Optimization

The dashboard is optimized for mobile devices with:
- Large touch targets
- Portrait orientation support
- Responsive design
- Quick access to all functions

## 🔧 Troubleshooting

### Common Issues
1. **Custom Cards Not Loading**: Ensure all required custom cards are installed
2. **Entity Names**: Verify entity names match your ESPHome configuration
3. **Styling Issues**: Check that custom CSS is properly configured

### Support
For issues with the dashboard examples:
1. Check the [main README](../README.md) for component setup
2. Verify your ESPHome configuration is working
3. Test individual entities in Home Assistant
4. Check Home Assistant logs for errors

## 📚 Additional Resources

- [Home Assistant Dashboard Documentation](https://www.home-assistant.io/dashboards/)
- [Lovelace UI Documentation](https://www.home-assistant.io/lovelace/)
- [Custom Cards Repository](https://github.com/custom-cards)
- [Material Design Icons](https://materialdesignicons.com/)

## 🤝 Contributing

Have a great dashboard example? Share it with the community:
1. Create a new file in the `dashboards/` directory
2. Include a description and screenshot
3. Provide the YAML configuration
4. Update this README with your example

Your examples help others get the most out of their Balboa spa integration! 