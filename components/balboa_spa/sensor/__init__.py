import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

DEPENDENCIES = ["balboa_spa"]

SpaSensor = balboa_spa_ns.class_("BalboaSpaSensors", sensor.Sensor)
SpaSensorTypeEnum = SpaSensor.enum("BalboaSpaSensorType", True)

CONF_BLOWER = "blower"
CONF_HIGHRANGE = "highrange"
CONF_CIRCULATION = "circulation"
CONF_RESTMODE = "restmode"
CONF_HEATSTATE = "heatstate"
CONF_CLOCK_HOUR = "spa_clock_hour"
CONF_CLOCK_MINUTE = "spa_clock_minute"
CONF_FILTER1_START_HOUR = "filter1_start_hour"
CONF_FILTER1_START_MINUTE = "filter1_start_minute"
CONF_FILTER1_DURATION_HOUR = "filter1_duration_hour"
CONF_FILTER1_DURATION_MINUTE = "filter1_duration_minute"
CONF_FILTER2_START_HOUR = "filter2_start_hour"
CONF_FILTER2_START_MINUTE = "filter2_start_minute"
CONF_FILTER2_DURATION_HOUR = "filter2_duration_hour"
CONF_FILTER2_DURATION_MINUTE = "filter2_duration_minute"
CONF_FILTER1_RUNTIME_HOURS = "filter1_runtime_hours"
CONF_FILTER2_RUNTIME_HOURS = "filter2_runtime_hours"
CONF_FILTER1_CYCLES_COMPLETED = "filter1_cycles_completed"
CONF_FILTER2_CYCLES_COMPLETED = "filter2_cycles_completed"
CONF_FILTER1_CURRENT_RUNTIME_MINUTES = "filter1_current_runtime_minutes"
CONF_FILTER2_CURRENT_RUNTIME_MINUTES = "filter2_current_runtime_minutes"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_BLOWER): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_HIGHRANGE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_CIRCULATION): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_RESTMODE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_HEATSTATE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_CLOCK_HOUR): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_CLOCK_MINUTE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_FILTER1_START_HOUR): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_FILTER1_START_MINUTE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_FILTER1_DURATION_HOUR): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_FILTER1_DURATION_MINUTE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_FILTER2_START_HOUR): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_FILTER2_START_MINUTE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_FILTER2_DURATION_HOUR): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_FILTER2_DURATION_MINUTE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_FILTER1_RUNTIME_HOURS): sensor.sensor_schema(
            SpaSensor,
            unit_of_measurement="h",
            icon="mdi:clock-outline",
        ),
        cv.Optional(CONF_FILTER2_RUNTIME_HOURS): sensor.sensor_schema(
            SpaSensor,
            unit_of_measurement="h",
            icon="mdi:clock-outline",
        ),
        cv.Optional(CONF_FILTER1_CYCLES_COMPLETED): sensor.sensor_schema(
            SpaSensor,
            icon="mdi:counter",
        ),
        cv.Optional(CONF_FILTER2_CYCLES_COMPLETED): sensor.sensor_schema(
            SpaSensor,
            icon="mdi:counter",
        ),
        cv.Optional(CONF_FILTER1_CURRENT_RUNTIME_MINUTES): sensor.sensor_schema(
            SpaSensor,
            unit_of_measurement="min",
            icon="mdi:timer",
        ),
        cv.Optional(CONF_FILTER2_CURRENT_RUNTIME_MINUTES): sensor.sensor_schema(
            SpaSensor,
            unit_of_measurement="min",
            icon="mdi:timer",
        ),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for sensor_type in [CONF_BLOWER, CONF_HIGHRANGE, CONF_CIRCULATION, CONF_RESTMODE, CONF_HEATSTATE, CONF_CLOCK_HOUR, CONF_CLOCK_MINUTE, CONF_FILTER1_START_HOUR, CONF_FILTER1_START_MINUTE, CONF_FILTER1_DURATION_HOUR, CONF_FILTER1_DURATION_MINUTE, CONF_FILTER2_START_HOUR, CONF_FILTER2_START_MINUTE, CONF_FILTER2_DURATION_HOUR, CONF_FILTER2_DURATION_MINUTE, CONF_FILTER1_RUNTIME_HOURS, CONF_FILTER2_RUNTIME_HOURS, CONF_FILTER1_CYCLES_COMPLETED, CONF_FILTER2_CYCLES_COMPLETED, CONF_FILTER1_CURRENT_RUNTIME_MINUTES, CONF_FILTER2_CURRENT_RUNTIME_MINUTES]:
        if conf := config.get(sensor_type):
            var = await sensor.new_sensor(conf)
            cg.add(var.set_parent(parent))
            sensor_type_value = getattr(SpaSensorTypeEnum, sensor_type.upper())
            cg.add(var.set_sensor_type(sensor_type_value))
