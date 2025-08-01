#include "esphome/core/log.h"
#include "binary_sensors.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.binary_sensors";

void BalboaSpaBinarySensors::set_parent(BalboaSpa *parent) {
    this->spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void BalboaSpaBinarySensors::update(SpaState* spaState) {
    bool sensor_state_value;
    
    // Handle communication status sensor separately
    if (sensor_type == BalboaSpaBinarySensorType::CONNECTED) {
        sensor_state_value = spa->is_communicating();
        if(this->state != sensor_state_value) {
            this->publish_state(sensor_state_value);
        }
        return;
    }
    
    // Early return if spa is null or not communicating for other sensors
    if (spa == nullptr || !spa->is_communicating()) {
        return;
    }

    uint8_t state_value = 0;
    switch (sensor_type)
    {
        case BalboaSpaBinarySensorType::BLOWER:
            sensor_state_value = spaState->blower;
            break;
        case BalboaSpaBinarySensorType::HIGHRANGE:
            sensor_state_value = spaState->highrange;
            break;
        case BalboaSpaBinarySensorType::CIRCULATION:
            sensor_state_value = spaState->circulation;
            break;
        case BalboaSpaBinarySensorType::RESTMODE:
            state_value = spaState->rest_mode;
            sensor_state_value = state_value;
            if(state_value == 254) {
                // This indicate no value
                return;
            }
            break;
        case BalboaSpaBinarySensorType::HEATSTATE:
            state_value = spaState->heat_state;
            sensor_state_value = state_value;
            if(state_value == 254) {
                // no value
                return;
            }
            break;
        case BalboaSpaBinarySensorType::FILTER1_ACTIVE:
            sensor_state_value = spa->is_filter1_enabled();
            break;
        case BalboaSpaBinarySensorType::FILTER2_ACTIVE:
            sensor_state_value = spa->is_filter2_enabled();
            break;
        case BalboaSpaBinarySensorType::FILTER1_RUNNING:
            sensor_state_value = spa->is_filter1_running();
            break;
        case BalboaSpaBinarySensorType::FILTER2_RUNNING:
            sensor_state_value = spa->is_filter2_running();
            break;
        case BalboaSpaBinarySensorType::PUMP1_RUNNING:
            sensor_state_value = (spaState->pump1 > 0);  // Running if not 0 (off)
            break;
        case BalboaSpaBinarySensorType::PUMP2_RUNNING:
            sensor_state_value = (spaState->pump2 > 0);  // Running if not 0 (off)
            break;
        case BalboaSpaBinarySensorType::PUMP3_RUNNING:
            sensor_state_value = (spaState->pump3 > 0);  // Running if not 0 (off)
            break;
        default:
            ESP_LOGD(TAG, "Unknown binary sensor type: %d", sensor_type);
            return;
    }

    // Only publish if state has changed or forced update interval
    if(this->state != sensor_state_value || this->last_update_time + 300000 < millis()) {
        this->publish_state(sensor_state_value);
        last_update_time = millis();
    }
}

BalboaSpaBinarySensors::BalboaSpaBinarySensors() {
    spa = nullptr;
    sensor_type = BalboaSpaBinarySensorType::UNKNOWN;
    last_update_time = 0;
}

}}
