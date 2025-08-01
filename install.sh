#!/bin/bash

# Balboa Spa ESPHome Component Installation Script
# This script helps users install the component in their ESPHome configuration

set -e

echo "🚀 Balboa Spa ESPHome Component Installer"
echo "=========================================="

# Check if ESPHome is installed
if ! command -v esphome &> /dev/null; then
    echo "❌ ESPHome is not installed or not in PATH"
    echo "Please install ESPHome first: https://esphome.io/guide/installation.html"
    exit 1
fi

echo "✅ ESPHome found: $(esphome version)"

# Get ESPHome configuration directory
echo ""
echo "📁 ESPHome Configuration Directory"
echo "Please enter your ESPHome configuration directory path:"
echo "(Default: ~/.esphome)"
read -p "Path: " ESPHOME_CONFIG_DIR

if [ -z "$ESPHOME_CONFIG_DIR" ]; then
    ESPHOME_CONFIG_DIR="$HOME/.esphome"
fi

# Check if directory exists
if [ ! -d "$ESPHOME_CONFIG_DIR" ]; then
    echo "❌ Directory does not exist: $ESPHOME_CONFIG_DIR"
    echo "Please create the directory or provide the correct path"
    exit 1
fi

echo "✅ Using configuration directory: $ESPHOME_CONFIG_DIR"

# Create components directory if it doesn't exist
COMPONENTS_DIR="$ESPHOME_CONFIG_DIR/components"
if [ ! -d "$COMPONENTS_DIR" ]; then
    echo "📁 Creating components directory..."
    mkdir -p "$COMPONENTS_DIR"
fi

# Copy component files
echo "📋 Copying component files..."
if [ -d "components/balboa_spa" ]; then
    cp -r components/balboa_spa "$COMPONENTS_DIR/"
    echo "✅ Component files copied successfully"
else
    echo "❌ Component files not found in current directory"
    echo "Please run this script from the repository root"
    exit 1
fi

# Create example configuration
EXAMPLE_CONFIG="$ESPHOME_CONFIG_DIR/balboa_spa_example.yaml"
echo "📝 Creating example configuration..."

cat > "$EXAMPLE_CONFIG" << 'EOF'
# Balboa Spa Example Configuration
# Copy this to your ESPHome configuration and customize as needed

esphome:
  name: balboa-spa-monitor
  friendly_name: "Balboa Spa Monitor"

esp32:
  board: m5stack-atom  # Change to your board type
  framework:
    type: arduino

# UART Configuration
uart:
  id: spa_uart_bus
  tx_pin: GPIO26  # Change to your TX pin
  rx_pin: GPIO32  # Change to your RX pin
  baud_rate: 115200
  data_bits: 8
  parity: NONE
  stop_bits: 1

# External Components
external_components:
  - source: ./components
    components: [balboa_spa]

# Balboa Spa Component
balboa_spa:
  id: spa
  uart_id: spa_uart_bus
  spa_temp_scale: "F"  # or "C"

# Climate Control
climate:
  - platform: balboa_spa
    balboa_spa_id: spa
    name: "Spa Thermostat"
    id: spa_thermostat
    visual:
      min_temperature: 79 °F
      max_temperature: 104 °F  
      temperature_step: 1.0 °F

# Switch Controls
switch:
  - platform: balboa_spa
    balboa_spa_id: spa
    jet1:
      name: "Spa Pump 1"
      id: spa_pump1_control
      icon: "mdi:pump"
      entity_category: diagnostic
    jet2:
      name: "Spa Pump 2"
      id: spa_pump2_control
      icon: "mdi:pump"
      entity_category: diagnostic
    light:
      name: "Spa Light"
      id: spa_light_control
      icon: "mdi:lightbulb"
      entity_category: diagnostic

# Sensors
sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    heatstate:
      name: "Spa Heat State"
      id: spa_heat_state
      icon: "mdi:thermometer"
    circulation:
      name: "Spa Circulation"
      id: spa_circulation
      icon: "mdi:rotate-3d-variant"
    restmode:
      name: "Spa Rest Mode"
      id: spa_rest_mode
      icon: "mdi:sleep"

# Binary Sensors
binary_sensor:
  - platform: balboa_spa
    balboa_spa_id: spa
    filter1_active:
      name: "Filter 1 Active"
      id: spa_filter1_active
      icon: "mdi:filter"
      entity_category: diagnostic
    filter2_active:
      name: "Filter 2 Active"
      id: spa_filter2_active
      icon: "mdi:filter"
      entity_category: diagnostic

# WiFi Configuration (add your credentials)
wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

# API for Home Assistant
api:
  encryption:
    key: !secret api_encryption_key

# OTA Updates
ota:
  - platform: esphome
EOF

echo "✅ Example configuration created: $EXAMPLE_CONFIG"

# Create secrets template
SECRETS_TEMPLATE="$ESPHOME_CONFIG_DIR/secrets_template.yaml"
echo "🔐 Creating secrets template..."

cat > "$SECRETS_TEMPLATE" << 'EOF'
# ESPHome Secrets Template for Balboa Spa Monitor
# Copy this file to 'secrets.yaml' and fill in your values

# WiFi Configuration
wifi_ssid: "YourWiFiSSID"
wifi_password: "YourWiFiPassword"

# API encryption key for ESPHome dashboard (32 characters)
# Generate a random key: openssl rand -base64 24
api_encryption_key: "your_32_character_encryption_key_here"

# Optional: MQTT Configuration
# mqtt_broker: "your_mqtt_broker_ip"
# mqtt_port: 1883
# mqtt_username: "your_mqtt_username"
# mqtt_password: "your_mqtt_password"
EOF

echo "✅ Secrets template created: $SECRETS_TEMPLATE"

echo ""
echo "🎉 Installation Complete!"
echo "========================"
echo ""
echo "📋 Next Steps:"
echo "1. Copy the example configuration to your ESPHome config:"
echo "   cp $EXAMPLE_CONFIG your_config.yaml"
echo ""
echo "2. Create your secrets file:"
echo "   cp $SECRETS_TEMPLATE secrets.yaml"
echo "   # Edit secrets.yaml with your WiFi credentials"
echo ""
echo "3. Customize the configuration for your hardware:"
echo "   - Update board type"
echo "   - Set correct TX/RX pins"
echo "   - Configure temperature scale"
echo ""
echo "4. Compile and upload:"
echo "   esphome run your_config.yaml"
echo ""
echo "📚 Documentation:"
echo "- README.md: Full documentation and examples"
echo "- docs/configuration/home_assistant_entity_organization.yaml: Home Assistant integration guide"
echo "- docs/examples/: Dashboard examples and configurations"
echo ""
echo "🐛 Need Help?"
echo "- Check the troubleshooting section in README.md"
echo "- Open an issue on GitHub"
echo ""
echo "Happy spa monitoring! 🏊‍♂️" 