#pragma once

#include <string>

#include "esphome/components/sensor/sensor.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class BalboaSpaSensors : public sensor::Sensor {
public:
  enum class BalboaSpaSensorType : uint8_t{
    BLOWER = 1,
    HIGHRANGE = 2,
    CIRCULATION = 3,
    RESTMODE = 4,
    HEATSTATE = 5,
    SPA_CLOCK_HOUR = 6,
    SPA_CLOCK_MINUTE = 7,
    FILTER1_START_HOUR = 8,
    FILTER1_START_MINUTE = 9,
    FILTER1_DURATION_HOUR = 10,
    FILTER1_DURATION_MINUTE = 11,
    FILTER2_START_HOUR = 12,
    FILTER2_START_MINUTE = 13,
    FILTER2_DURATION_HOUR = 14,
    FILTER2_DURATION_MINUTE = 15,
  };

public:
  BalboaSpaSensors() {};
  void update(SpaState* spaState);

  void set_parent(BalboaSpa *parent);
  void set_sensor_type(BalboaSpaSensorType _type) { sensor_type = _type;  }

  private:
    BalboaSpaSensorType sensor_type;
    BalboaSpa *parent;
};

}  // namespace balboa_spa
}  // namespace esphome
