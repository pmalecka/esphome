import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_STATUS,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_ARROW_EXPAND_VERTICAL,
    ICON_DATABASE,
)

from . import CONF_FULLY_JARVIS_F_ID, FullyJarvisFComponent

DEPENDENCIES = ["fully_jarvis_f"]
CONF_USER_LIMIT_SET = "user_limit_set"

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_FULLY_JARVIS_F_ID): cv.use_id(FullyJarvisFComponent),
    cv.Optional(CONF_USER_LIMIT_SET): text_sensor.text_sensor_schema(
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC, icon=ICON_ARROW_EXPAND_VERTICAL
    ),
    cv.Optional(CONF_STATUS): text_sensor.text_sensor_schema(
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC, icon=ICON_DATABASE
    ),
}


async def to_code(config):
    fully_jarvis_f_component = await cg.get_variable(config[CONF_FULLY_JARVIS_F_ID])
    if user_limit_set_config := config.get(CONF_USER_LIMIT_SET):
        sens = await text_sensor.new_text_sensor(user_limit_set_config)
        cg.add(fully_jarvis_f_component.set_user_limit_set_text_sensor(sens))
    if status_config := config.get(CONF_STATUS):
        sens = await text_sensor.new_text_sensor(status_config)
        cg.add(fully_jarvis_f_component.set_status_text_sensor(sens))
