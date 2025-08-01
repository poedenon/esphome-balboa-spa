#include "filter2_enable_switch.h"

namespace esphome {
namespace balboa_spa {

void Filter2EnableSwitch::update(SpaState* spaState) {
    // Update based on filter 2 enable status from filter settings
    bool filter2_enabled = spa->is_filter2_enabled();
    if(this->state != filter2_enabled) {
        this->publish_state(filter2_enabled);
    }
}

void Filter2EnableSwitch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Filter2EnableSwitch::write_state(bool state) {
    spa->set_filter2_enable(state);
}

}  // namespace balboa_spa
}  // namespace esphome 