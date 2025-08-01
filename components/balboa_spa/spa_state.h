#include <stdint.h>

#ifndef SPA_STATE_H
#define SPA_STATE_H

namespace esphome {
namespace balboa_spa {

class SpaState {
    public:
        SpaState() {
            rest_mode = 254;
            heat_state = 254;
            target_temp = NAN;
            current_temp = NAN;
            filter1_runtime_hours = 0;
            filter2_runtime_hours = 0;
            filter1_cycles_completed = 0;
            filter2_cycles_completed = 0;
        }
        uint8_t jet1 :2;
        uint8_t jet2 :2;
        uint8_t jet3 :2;
        uint8_t jet4 :2;
        uint8_t pump1 :2;  // Pump 1 status (0=off, 1=low, 2=high)
        uint8_t pump2 :2;  // Pump 2 status (0=off, 1=low, 2=high)
        uint8_t pump3 :2;  // Pump 3 status (0=off, 1=low, 2=high)
        uint8_t blower :1;
        uint8_t light :1;
        uint8_t highrange:1;        
        uint8_t circulation:1;
        uint8_t hour:5;
        uint8_t minutes:6;
        uint8_t rest_mode;
        uint8_t heat_state;
        float target_temp;
        float current_temp;
        
        // Filter status tracking
        uint16_t filter1_runtime_hours;      // Total runtime hours for filter 1
        uint16_t filter2_runtime_hours;      // Total runtime hours for filter 2
        uint16_t filter1_cycles_completed;   // Number of completed filter cycles
        uint16_t filter2_cycles_completed;   // Number of completed filter cycles
        bool filter1_running;                // Current filter 1 running status
        bool filter2_running;                // Current filter 2 running status
        uint32_t filter1_last_start_time;    // Last start time for filter 1 (millis)
        uint32_t filter2_last_start_time;    // Last start time for filter 2 (millis)
};
}  // namespace balboa_spa
}  // namespace esphome

#endif