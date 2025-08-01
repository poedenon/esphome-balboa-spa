import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

from esphome.const import (
    DEVICE_CLASS_CONNECTIVITY,
    ENTITY_CATEGORY_DIAGNOSTIC,
    DEVICE_CLASS_POWER
)

DEPENDENCIES = ["balboa_spa"]

SpaSensor = balboa_spa_ns.class_("BalboaSpaBinarySensors", binary_sensor.BinarySensor)
SpaSensorTypeEnum = SpaSensor.enum("BalboaSpaBinarySensorType", True)

CONF_BLOWER = "blower"
CONF_HIGHRANGE = "highrange"
CONF_CIRCULATION = "circulation"
CONF_RESTMODE = "restmode"
CONF_HEATSTATE = "heatstate"
CONF_CONNECTED = "connected"
CONF_FILTER1_ACTIVE = "filter1_active"
CONF_FILTER2_ACTIVE = "filter2_active"
CONF_FILTER1_RUNNING = "filter1_running"
CONF_FILTER2_RUNNING = "filter2_running"
CONF_PUMP1_RUNNING = "pump1_running"
CONF_PUMP2_RUNNING = "pump2_running"
CONF_PUMP3_RUNNING = "pump3_running"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_BLOWER): binary_sensor.binary_sensor_schema(
            SpaSensor,
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_HIGHRANGE): binary_sensor.binary_sensor_schema(
            SpaSensor,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_CIRCULATION): binary_sensor.binary_sensor_schema(
            SpaSensor,
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_RESTMODE): binary_sensor.binary_sensor_schema(
            SpaSensor,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_HEATSTATE): binary_sensor.binary_sensor_schema(
            SpaSensor,
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_CONNECTED): binary_sensor.binary_sensor_schema(
            SpaSensor,
            icon="mdi:lan-connect",
            device_class=DEVICE_CLASS_CONNECTIVITY,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_FILTER1_ACTIVE): binary_sensor.binary_sensor_schema(
            SpaSensor,
            icon="mdi:filter",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_FILTER2_ACTIVE): binary_sensor.binary_sensor_schema(
            SpaSensor,
            icon="mdi:filter",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_FILTER1_RUNNING): binary_sensor.binary_sensor_schema(
            SpaSensor,
            icon="mdi:filter-variant",
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_FILTER2_RUNNING): binary_sensor.binary_sensor_schema(
            SpaSensor,
            icon="mdi:filter-variant",
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_PUMP1_RUNNING): binary_sensor.binary_sensor_schema(
            SpaSensor,
            icon="mdi:pump",
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_PUMP2_RUNNING): binary_sensor.binary_sensor_schema(
            SpaSensor,
            icon="mdi:pump",
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_PUMP3_RUNNING): binary_sensor.binary_sensor_schema(
            SpaSensor,
            icon="mdi:pump",
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for sensor_type in [CONF_BLOWER, CONF_HIGHRANGE, CONF_CIRCULATION, CONF_RESTMODE, CONF_HEATSTATE, CONF_CONNECTED, CONF_FILTER1_ACTIVE, CONF_FILTER2_ACTIVE, CONF_FILTER1_RUNNING, CONF_FILTER2_RUNNING, CONF_PUMP1_RUNNING, CONF_PUMP2_RUNNING, CONF_PUMP3_RUNNING]:
        if conf := config.get(sensor_type):
            var = await binary_sensor.new_binary_sensor(conf)
            cg.add(var.set_parent(parent))
            sensor_type_value = getattr(SpaSensorTypeEnum, sensor_type.upper()) if hasattr(SpaSensorTypeEnum, sensor_type.upper()) else sensor_type
            cg.add(var.set_sensor_type(sensor_type_value))
