#include "esphome.h"
#include "esphome/core/log.h"
#include "spa_thermostat.h"
#include "esphome/components/climate/climate_mode.h"

namespace esphome {
namespace balboa_spa {

climate::ClimateTraits BalboaSpaThermostat::traits()
{
    auto traits = climate::ClimateTraits();
    traits.set_supported_modes({climate::CLIMATE_MODE_OFF, climate::ClimateMode::CLIMATE_MODE_HEAT});
    traits.set_supports_action(true);
    traits.set_supports_current_temperature(true);
    traits.set_supported_presets({climate::ClimatePreset::CLIMATE_PRESET_HOME, climate::ClimatePreset::CLIMATE_PRESET_ECO});
    traits.set_supports_two_point_target_temperature(false);
    return traits;
}

 void BalboaSpaThermostat::control(const climate::ClimateCall &call){
    // Early return if spa is not communicating
    if (!spa->is_communicating()) {
        ESP_LOGW("SpaThermostat", "Cannot control thermostat - spa not communicating");
        return;
    }
    
    bool has_temp = call.get_target_temperature().has_value();
    bool has_preset = call.get_preset().has_value();
    
    if(has_temp){
        spa->set_temp(*call.get_target_temperature());
    }
    if (has_preset){
        spa->set_highrange( *call.get_preset() == climate::ClimatePreset::CLIMATE_PRESET_HOME );
    }
 }

void BalboaSpaThermostat::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

bool inline is_diff_no_nan(float a, float b){
    return !std::isnan(a) && !std::isnan(b) && b != a;
}

void BalboaSpaThermostat::update(SpaState* spaState) {
    bool needs_update = false;

    // Check communication status first
    if (!spa->is_communicating()){
        if (!std::isnan(this->target_temperature) || !std::isnan(this->current_temperature)) {
            this->target_temperature = NAN;
            this->current_temperature = NAN;
            needs_update = true;
        }
        if (needs_update) {
            this->publish_state();
        }
        return;
    }

    // Update target temperature with better NAN handling
    float target_temp = spaState->target_temp;
    if (std::isnan(target_temp)) {
        if (!std::isnan(this->target_temperature)) {
            this->target_temperature = NAN;
            needs_update = true;
        }
    } else if (std::isnan(this->target_temperature) || target_temp != this->target_temperature) {
        this->target_temperature = target_temp;
        needs_update = true;
    }

    // Update current temperature with better NAN handling
    float current_temp = spaState->current_temp;
    if (std::isnan(current_temp)) {
        if (!std::isnan(this->current_temperature)) {
            this->current_temperature = NAN;
            needs_update = true;
        }
    } else if (std::isnan(this->current_temperature) || current_temp != this->current_temperature) {
        this->current_temperature = current_temp;
        needs_update = true;
    }

    // Update heating action
    bool is_heating = (spaState->heat_state == 1) || 
                     (!std::isnan(current_temp) && !std::isnan(target_temp) && current_temp < target_temp);
    auto new_action = is_heating ? climate::CLIMATE_ACTION_HEATING : climate::CLIMATE_ACTION_IDLE;
    
    if (new_action != this->action) {
        this->action = new_action;
        needs_update = true;
    }

    // Update mode
    auto new_mode = spaState->rest_mode == 1 ? climate::CLIMATE_MODE_OFF : climate::CLIMATE_MODE_HEAT;
    if (new_mode != this->mode) {
        this->mode = new_mode;
        needs_update = true;
    }

    // Update preset
    auto preset_mode = spaState->highrange == 1 ? climate::ClimatePreset::CLIMATE_PRESET_HOME : climate::ClimatePreset::CLIMATE_PRESET_ECO;
    if (preset_mode != this->preset) {
        this->preset = preset_mode;
        needs_update = true;
    }

    // Force update every 5 minutes to ensure Home Assistant stays in sync
    if (this->last_update_time + 300000 < millis()) {
        needs_update = true;
    }
    
    if(needs_update) {
        this->publish_state();
        this->last_update_time = millis();
    }
}

}
}