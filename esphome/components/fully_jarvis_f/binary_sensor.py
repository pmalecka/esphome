import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import DEVICE_CLASS_PLUG, ENTITY_CATEGORY_DIAGNOSTIC, ICON_POWER

from . import CONF_FULLY_JARVIS_F_ID, FullyJarvisFComponent

DEPENDENCIES = ["fully_jarvis_f"]
CONF_INITIALIZED = "initialized"

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_FULLY_JARVIS_F_ID): cv.use_id(FullyJarvisFComponent),
    cv.Optional(CONF_INITIALIZED): binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_PLUG,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        icon=ICON_POWER,
    ),
}


async def to_code(config):
    fully_jarvis_f_component = await cg.get_variable(config[CONF_FULLY_JARVIS_F_ID])
    if initialized_config := config.get(CONF_INITIALIZED):
        sens = await binary_sensor.new_binary_sensor(initialized_config)
        cg.add(fully_jarvis_f_component.set_initialized_binary_sensor(sens))
