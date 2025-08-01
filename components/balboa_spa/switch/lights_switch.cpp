#include "lights_switch.h"

namespace esphome {
namespace balboa_spa {
void LightsSwitch::update(SpaState* spaState) {
    // Only update if state has changed
    if(this->state != spaState->light)
    {
        this->publish_state(spaState->light);
    }
}

void LightsSwitch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void LightsSwitch::write_state(bool state) {
    // Check communication before attempting to control
    if (!spa->is_communicating()) {
        ESP_LOGW("LightsSwitch", "Cannot control light - spa not communicating");
        return;
    }
    
    SpaState* spaState = spa->get_current_state();
    if(spaState->light != state){
        spa->toggle_light();
    }
}

}  // namespace balboa_spa
}  // namespace esphome
