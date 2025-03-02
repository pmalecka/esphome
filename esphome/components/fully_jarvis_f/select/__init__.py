import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import (
    CONF_SENSITIVITY,
    CONF_UNIT_OF_MEASUREMENT,
    ENTITY_CATEGORY_CONFIG,
    ICON_FINGERPRINT,
    ICON_RADIOACTIVE,
    ICON_RULER,
    ICON_VIBRATE,
)

from .. import CONF_FULLY_JARVIS_F_ID, FullyJarvisFComponent, fully_jarvis_f_ns

UnitsSelect = fully_jarvis_f_ns.class_("UnitsSelect", select.Select)
TouchModeSelect = fully_jarvis_f_ns.class_("TouchModeSelect", select.Select)
KillModeSelect = fully_jarvis_f_ns.class_("KillModeSelect", select.Select)
SensitivitySelect = fully_jarvis_f_ns.class_("SensitivitySelect", select.Select)

CONF_TOUCH_MODE = "touch_mode"
CONF_KILL_MODE = "kill_mode"

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_FULLY_JARVIS_F_ID): cv.use_id(FullyJarvisFComponent),
    cv.Optional(CONF_UNIT_OF_MEASUREMENT): select.select_schema(
        UnitsSelect,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_RULER,
    ),
    cv.Optional(CONF_TOUCH_MODE): select.select_schema(
        TouchModeSelect,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_FINGERPRINT,
    ),
    cv.Optional(CONF_KILL_MODE): select.select_schema(
        KillModeSelect,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_RADIOACTIVE,
    ),
    cv.Optional(CONF_SENSITIVITY): select.select_schema(
        SensitivitySelect,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_VIBRATE,
    ),
}


async def to_code(config):
    fully_jarvis_f_component = await cg.get_variable(config[CONF_FULLY_JARVIS_F_ID])
    if units_config := config.get(CONF_UNIT_OF_MEASUREMENT):
        s = await select.new_select(units_config, options=["mm", "in"])
        await cg.register_parented(s, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_units_select(s))
    if touch_mode_config := config.get(CONF_TOUCH_MODE):
        s = await select.new_select(touch_mode_config, options=["single", "continuous"])
        await cg.register_parented(s, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_touch_mode_select(s))
    if kill_mode_config := config.get(CONF_KILL_MODE):
        s = await select.new_select(kill_mode_config, options=["kill", "letlive"])
        await cg.register_parented(s, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_kill_mode_select(s))
    if sensitivity_config := config.get(CONF_SENSITIVITY):
        s = await select.new_select(
            sensitivity_config,
            options=["high", "medium", "low"],
        )
        await cg.register_parented(s, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_sensitivity_select(s))
