import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    DEVICE_CLASS_DISTANCE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_ACCOUNT,
    ICON_COUNTER,
    ICON_MEMORY,
    UNIT_MILLIMETER,
)

from . import CONF_FULLY_JARVIS_F_ID, FullyJarvisFComponent

DEPENDENCIES = ["fully_jarvis_f"]
# TODO: add validation to ensur sys_limit_min (sensor) and units (select) is also defined, if these preset_x_height
#       sensors are defined, as the calculation of the preset_x_height value depends on these two
CONF_PRESET_1_HEIGHT = "preset_1_height"
CONF_PRESET_2_HEIGHT = "preset_2_height"
CONF_PRESET_3_HEIGHT = "preset_3_height"
CONF_PRESET_4_HEIGHT = "preset_4_height"

CONF_USER_LIMIT_MIN_HEIGHT = "user_limit_min_height"
CONF_USER_LIMIT_MAX_HEIGHT = "user_limit_max_height"

CONF_SYS_LIMIT_MIN_HEIGHT = "sys_limit_min_height"
CONF_SYS_LIMIT_MAX_HEIGHT = "sys_limit_max_height"


def validate_preset_dependencies(config):
    if (
        "preset_1_height" in config
        or "preset_2_height" in config
        or "preset_3_height" in config
        or "preset_4_height" in config
    ):
        missing = []
        if "sys_limit_min_height" not in config:
            missing.append("sys_limit_min_height")
        if not any(s.get("id") == "units_select" for s in config.get("select", [])):
            missing.append("units_select")

        if missing:
            raise cv.Invalid(
                f"Required components when using preset_1_height: {', '.join(missing)}"
            )
    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_FULLY_JARVIS_F_ID): cv.use_id(FullyJarvisFComponent),
            cv.Optional(CONF_PRESET_1_HEIGHT): sensor.sensor_schema(
                device_class=DEVICE_CLASS_DISTANCE,
                unit_of_measurement=UNIT_MILLIMETER,
                accuracy_decimals=0,
                icon=ICON_COUNTER,
            ),
            cv.Optional(CONF_PRESET_2_HEIGHT): sensor.sensor_schema(
                device_class=DEVICE_CLASS_DISTANCE,
                unit_of_measurement=UNIT_MILLIMETER,
                accuracy_decimals=0,
                icon=ICON_COUNTER,
            ),
            cv.Optional(CONF_PRESET_3_HEIGHT): sensor.sensor_schema(
                device_class=DEVICE_CLASS_DISTANCE,
                unit_of_measurement=UNIT_MILLIMETER,
                accuracy_decimals=0,
                icon=ICON_COUNTER,
            ),
            cv.Optional(CONF_PRESET_4_HEIGHT): sensor.sensor_schema(
                device_class=DEVICE_CLASS_DISTANCE,
                unit_of_measurement=UNIT_MILLIMETER,
                accuracy_decimals=0,
                icon=ICON_COUNTER,
            ),
            cv.Optional(CONF_USER_LIMIT_MIN_HEIGHT): sensor.sensor_schema(
                device_class=DEVICE_CLASS_DISTANCE,
                unit_of_measurement=UNIT_MILLIMETER,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                accuracy_decimals=0,
                icon=ICON_ACCOUNT,
            ),
            cv.Optional(CONF_USER_LIMIT_MAX_HEIGHT): sensor.sensor_schema(
                device_class=DEVICE_CLASS_DISTANCE,
                unit_of_measurement=UNIT_MILLIMETER,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                accuracy_decimals=0,
                icon=ICON_ACCOUNT,
            ),
            cv.Optional(CONF_SYS_LIMIT_MIN_HEIGHT): sensor.sensor_schema(
                device_class=DEVICE_CLASS_DISTANCE,
                unit_of_measurement=UNIT_MILLIMETER,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                accuracy_decimals=0,
                icon=ICON_MEMORY,
            ),
            cv.Optional(CONF_SYS_LIMIT_MAX_HEIGHT): sensor.sensor_schema(
                device_class=DEVICE_CLASS_DISTANCE,
                unit_of_measurement=UNIT_MILLIMETER,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                accuracy_decimals=0,
                icon=ICON_MEMORY,
            ),
        }
    ),
    validate_preset_dependencies,
)


async def to_code(config):
    fully_jarvis_f_component = await cg.get_variable(config[CONF_FULLY_JARVIS_F_ID])
    if preset_1_height_config := config.get(CONF_PRESET_1_HEIGHT):
        sens = await sensor.new_sensor(preset_1_height_config)
        cg.add(fully_jarvis_f_component.set_preset_1_height_sensor(sens))
    if preset_2_height_config := config.get(CONF_PRESET_2_HEIGHT):
        sens = await sensor.new_sensor(preset_2_height_config)
        cg.add(fully_jarvis_f_component.set_preset_2_height_sensor(sens))
    if preset_3_height_config := config.get(CONF_PRESET_3_HEIGHT):
        sens = await sensor.new_sensor(preset_3_height_config)
        cg.add(fully_jarvis_f_component.set_preset_3_height_sensor(sens))
    if preset_4_height_config := config.get(CONF_PRESET_4_HEIGHT):
        sens = await sensor.new_sensor(preset_4_height_config)
        cg.add(fully_jarvis_f_component.set_preset_4_height_sensor(sens))

    if user_limit_min_height_config := config.get(CONF_USER_LIMIT_MIN_HEIGHT):
        sens = await sensor.new_sensor(user_limit_min_height_config)
        cg.add(fully_jarvis_f_component.set_user_limit_min_height_sensor(sens))
    if user_limit_max_height_config := config.get(CONF_USER_LIMIT_MAX_HEIGHT):
        sens = await sensor.new_sensor(user_limit_max_height_config)
        cg.add(fully_jarvis_f_component.set_user_limit_max_height_sensor(sens))

    if sys_limit_min_height_config := config.get(CONF_SYS_LIMIT_MIN_HEIGHT):
        sens = await sensor.new_sensor(sys_limit_min_height_config)
        cg.add(fully_jarvis_f_component.set_sys_limit_min_height_sensor(sens))
    if sys_limit_max_height_config := config.get(CONF_SYS_LIMIT_MAX_HEIGHT):
        sens = await sensor.new_sensor(sys_limit_max_height_config)
        cg.add(fully_jarvis_f_component.set_sys_limit_max_height_sensor(sens))
