#include "jet2_switch.h"

namespace esphome {
namespace balboa_spa {

void Jet2Switch::update(SpaState* spaState) {
    // Only update if state has changed
    if(this->state != spaState->jet2)
    {
        this->publish_state(spaState->jet2);
    }
}

void Jet2Switch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Jet2Switch::write_state(bool state) {
    // Check communication before attempting to control
    if (!spa->is_communicating()) {
        ESP_LOGW("Jet2Switch", "Cannot control jet2 - spa not communicating");
        return;
    }
    
    SpaState* spaState = spa->get_current_state();
    if(spaState->jet2 != state){
        spa->toggle_jet2();
    }
}

}  // namespace balboa_spa
}  // namespace esphome
