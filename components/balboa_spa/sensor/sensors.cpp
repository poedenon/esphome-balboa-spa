#include "esphome/core/log.h"
#include "sensors.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.sensors";

void BalboaSpaSensors::set_parent(BalboaSpa *parent) {
    this->parent = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void BalboaSpaSensors::update(SpaState* spaState) {
    uint8_t sensor_state_value;

    switch (sensor_type)
    {
        case BalboaSpaSensorType::BLOWER:
            sensor_state_value = spaState->blower;
            break;
        case BalboaSpaSensorType::HIGHRANGE:
            sensor_state_value = spaState->highrange;
            break;
        case BalboaSpaSensorType::CIRCULATION:
            sensor_state_value = spaState->circulation;
            break;
        case BalboaSpaSensorType::RESTMODE:
            sensor_state_value = spaState->rest_mode;
            if(sensor_state_value == 254)
            {
                // This indicate no value
                return;
            }
            break;
        case BalboaSpaSensorType::HEATSTATE:
            sensor_state_value = spaState->heat_state;
            if(sensor_state_value == 254)
            {
                // no value
                return;
            }
            break;
        case BalboaSpaSensorType::SPA_CLOCK_HOUR:
            sensor_state_value = spaState->hour;
            break;
        case BalboaSpaSensorType::SPA_CLOCK_MINUTE:
            sensor_state_value = spaState->minutes;
            break;
        case BalboaSpaSensorType::FILTER1_START_HOUR:
            sensor_state_value = parent->get_filter1_start_hour();
            break;
        case BalboaSpaSensorType::FILTER1_START_MINUTE:
            sensor_state_value = parent->get_filter1_start_minute();
            break;
        case BalboaSpaSensorType::FILTER1_DURATION_HOUR:
            sensor_state_value = parent->get_filter1_duration_hour();
            break;
        case BalboaSpaSensorType::FILTER1_DURATION_MINUTE:
            sensor_state_value = parent->get_filter1_duration_minute();
            break;
        case BalboaSpaSensorType::FILTER2_START_HOUR:
            sensor_state_value = parent->get_filter2_start_hour();
            break;
        case BalboaSpaSensorType::FILTER2_START_MINUTE:
            sensor_state_value = parent->get_filter2_start_minute();
            break;
        case BalboaSpaSensorType::FILTER2_DURATION_HOUR:
            sensor_state_value = parent->get_filter2_duration_hour();
            break;
        case BalboaSpaSensorType::FILTER2_DURATION_MINUTE:
            sensor_state_value = parent->get_filter2_duration_minute();
            break;
        case BalboaSpaSensorType::FILTER1_RUNTIME_HOURS:
            sensor_state_value = parent->get_filter1_runtime_hours();
            break;
        case BalboaSpaSensorType::FILTER2_RUNTIME_HOURS:
            sensor_state_value = parent->get_filter2_runtime_hours();
            break;
        case BalboaSpaSensorType::FILTER1_CYCLES_COMPLETED:
            sensor_state_value = parent->get_filter1_cycles_completed();
            break;
        case BalboaSpaSensorType::FILTER2_CYCLES_COMPLETED:
            sensor_state_value = parent->get_filter2_cycles_completed();
            break;
        case BalboaSpaSensorType::FILTER1_CURRENT_RUNTIME_MINUTES:
            sensor_state_value = parent->get_filter1_current_runtime_minutes();
            break;
        case BalboaSpaSensorType::FILTER2_CURRENT_RUNTIME_MINUTES:
            sensor_state_value = parent->get_filter2_current_runtime_minutes();
            break;
        default:
            ESP_LOGD(TAG, "Spa/Sensors/UnknownSensorType: SensorType Number: %d", sensor_type);
            // Unknown enum value. Ignore
            return;
    }

    if(this->state != sensor_state_value)
    {
        this->publish_state(sensor_state_value);
    }
}
}}