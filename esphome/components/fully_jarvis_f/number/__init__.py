import esphome.codegen as cg
from esphome.components import number
import esphome.config_validation as cv
from esphome.const import (
    CONF_HEIGHT,
    CONF_OFFSET,
    DEVICE_CLASS_DISTANCE,
    ENTITY_CATEGORY_CONFIG,
    ENTITY_CATEGORY_NONE,
    ICON_GAUGE,
    ICON_RULER,
    UNIT_MILLIMETER,
)

from .. import CONF_FULLY_JARVIS_F_ID, FullyJarvisFComponent, fully_jarvis_f_ns

HeightNumber = fully_jarvis_f_ns.class_("HeightNumber", number.Number)
OffsetNumber = fully_jarvis_f_ns.class_("OffsetNumber", number.Number)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_FULLY_JARVIS_F_ID): cv.use_id(FullyJarvisFComponent),
        cv.Optional(CONF_HEIGHT): number.number_schema(
            HeightNumber,
            device_class=DEVICE_CLASS_DISTANCE,
            entity_category=ENTITY_CATEGORY_NONE,
            unit_of_measurement=UNIT_MILLIMETER,
            icon=ICON_RULER,
        ),
        cv.Optional(CONF_OFFSET): number.number_schema(
            OffsetNumber,
            device_class=DEVICE_CLASS_DISTANCE,
            entity_category=ENTITY_CATEGORY_CONFIG,
            unit_of_measurement=UNIT_MILLIMETER,
            icon=ICON_GAUGE,
        ),
    }
)


async def to_code(config):
    fully_jarvis_f_component = await cg.get_variable(config[CONF_FULLY_JARVIS_F_ID])

    if height_config := config.get(CONF_HEIGHT):
        n = await number.new_number(
            height_config, min_value=500, max_value=1500, step=10
        )
        await cg.register_parented(n, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_height_number(n))

    if offset_config := config.get(CONF_OFFSET):
        # theoretically, max value is 65535, but it doesn't make much sense.. (and I didn't test it)
        n = await number.new_number(offset_config, min_value=0, max_value=1000, step=1)
        await cg.register_parented(n, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_height_number(n))
