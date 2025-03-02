import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_RESTART,
    DEVICE_CLASS_UPDATE,
    ENTITY_CATEGORY_CONFIG,
    ENTITY_CATEGORY_NONE,
    ICON_NEW_BOX,
    ICON_RESTART,
    ICON_ROTATE_RIGHT,
    ICON_RULER,
)

from .. import CONF_FULLY_JARVIS_F_ID, FullyJarvisFComponent, fully_jarvis_f_ns

GoPresetButton = fully_jarvis_f_ns.class_("GoPresetButton", button.Button)

SetPresetButton = fully_jarvis_f_ns.class_("SetPresetButton", button.Button)

SetMaxHeightButton = fully_jarvis_f_ns.class_("SetMaxHeightButton", button.Button)
SetMinHeightButton = fully_jarvis_f_ns.class_("SetMinHeightButton", button.Button)

ClearMaxHeightButton = fully_jarvis_f_ns.class_("ClearMaxHeightButton", button.Button)
ClearMinHeightButton = fully_jarvis_f_ns.class_("ClearMinHeightButton", button.Button)

CONF_GO_PRESET_1 = "go_preset_1"
CONF_GO_PRESET_2 = "go_preset_2"
CONF_GO_PRESET_3 = "go_preset_3"
CONF_GO_PRESET_4 = "go_preset_4"

CONF_SET_PRESET_1 = "set_preset_1"
CONF_SET_PRESET_2 = "set_preset_2"
CONF_SET_PRESET_3 = "set_preset_3"
CONF_SET_PRESET_4 = "set_preset_4"

CONF_SET_MAX_HEIGHT = "set_max_height"
CONF_SET_MIN_HEIGHT = "set_min_height"

CONF_CLEAR_MAX_HEIGHT = "clear_max_height"
CONF_CLEAR_MIN_HEIGHT = "clear_min_height"

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_FULLY_JARVIS_F_ID): cv.use_id(FullyJarvisFComponent),
    cv.Optional(CONF_GO_PRESET_1): button.button_schema(
        GoPresetButton,
        entity_category=ENTITY_CATEGORY_NONE,
        icon=ICON_ROTATE_RIGHT,
    ),
    cv.Optional(CONF_GO_PRESET_2): button.button_schema(
        GoPresetButton,
        entity_category=ENTITY_CATEGORY_NONE,
        icon=ICON_ROTATE_RIGHT,
    ),
    cv.Optional(CONF_GO_PRESET_3): button.button_schema(
        GoPresetButton,
        entity_category=ENTITY_CATEGORY_NONE,
        icon=ICON_ROTATE_RIGHT,
    ),
    cv.Optional(CONF_GO_PRESET_4): button.button_schema(
        GoPresetButton,
        entity_category=ENTITY_CATEGORY_NONE,
        icon=ICON_ROTATE_RIGHT,
    ),
    cv.Optional(CONF_SET_PRESET_1): button.button_schema(
        SetPresetButton,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_NEW_BOX,
    ),
    cv.Optional(CONF_SET_PRESET_2): button.button_schema(
        SetPresetButton,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_NEW_BOX,
    ),
    cv.Optional(CONF_SET_PRESET_3): button.button_schema(
        SetPresetButton,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_NEW_BOX,
    ),
    cv.Optional(CONF_SET_PRESET_4): button.button_schema(
        SetPresetButton,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_NEW_BOX,
    ),
    cv.Optional(CONF_SET_MAX_HEIGHT): button.button_schema(
        SetMaxHeightButton,
        device_class=DEVICE_CLASS_UPDATE,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_RULER,
    ),
    cv.Optional(CONF_SET_MIN_HEIGHT): button.button_schema(
        SetMinHeightButton,
        device_class=DEVICE_CLASS_UPDATE,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_RULER,
    ),
    cv.Optional(CONF_CLEAR_MAX_HEIGHT): button.button_schema(
        ClearMaxHeightButton,
        device_class=DEVICE_CLASS_RESTART,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_RESTART,
    ),
    cv.Optional(CONF_CLEAR_MIN_HEIGHT): button.button_schema(
        ClearMinHeightButton,
        device_class=DEVICE_CLASS_RESTART,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_RESTART,
    ),
}


async def to_code(config):
    fully_jarvis_f_component = await cg.get_variable(config[CONF_FULLY_JARVIS_F_ID])
    for x in range(4):
        preset_num = x + 1
        if go_preset_n_config := config.get(f"go_preset_{preset_num}"):
            b = cg.new_Pvariable(go_preset_n_config[CONF_ID], preset_num)
            await button.register_button(b, go_preset_n_config)
            await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
            cg.add(fully_jarvis_f_component.set_go_preset_button(preset_num, b))

        if set_preset_n_config := config.get(f"set_preset_{preset_num}"):
            b = cg.new_Pvariable(set_preset_n_config[CONF_ID], preset_num)
            await button.register_button(b, set_preset_n_config)
            await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
            cg.add(fully_jarvis_f_component.set_set_preset_button(preset_num, b))

    # if go_preset_1_config := config.get(CONF_GO_PRESET_1):
    #     preset_num = 1
    #     b = await button.new_button(go_preset_1_config)
    #     await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
    #     cg.add(fully_jarvis_f_component.set_go_preset_button(preset_num, b))
    # if go_preset_2_config := config.get(CONF_GO_PRESET_2):
    #     preset_num = 2
    #     b = await button.new_button(go_preset_2_config)
    #     await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
    #     cg.add(fully_jarvis_f_component.set_go_preset_button(preset_num, b))
    # if go_preset_3_config := config.get(CONF_GO_PRESET_3):
    #     preset_num = 3
    #     b = await button.new_button(go_preset_3_config)
    #     await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
    #     cg.add(fully_jarvis_f_component.set_go_preset_button(preset_num, b))
    # if go_preset_4_config := config.get(CONF_GO_PRESET_4):
    #     preset_num = 4
    #     b = await button.new_button(go_preset_4_config)
    #     await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
    #     cg.add(fully_jarvis_f_component.set_go_preset_button(preset_num, b))

    # if set_preset_1_config := config.get(CONF_SET_PRESET_1):
    #     preset_num = 1
    #     b = await button.new_button(set_preset_1_config)
    #     await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
    #     cg.add(fully_jarvis_f_component.set_set_preset_button(preset_num, b))
    # if set_preset_2_config := config.get(CONF_SET_PRESET_2):
    #     preset_num = 2
    #     b = await button.new_button(set_preset_2_config)
    #     await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
    #     cg.add(fully_jarvis_f_component.set_set_preset_button(preset_num, b))
    # if set_preset_3_config := config.get(CONF_SET_PRESET_3):
    #     preset_num = 3
    #     b = await button.new_button(set_preset_3_config)
    #     await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
    #     cg.add(fully_jarvis_f_component.set_set_preset_button(preset_num, b))
    # if set_preset_4_config := config.get(CONF_SET_PRESET_4):
    #     preset_num = 4
    #     b = await button.new_button(set_preset_4_config)
    #     await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
    #     cg.add(fully_jarvis_f_component.set_set_preset_button(preset_num, b))

    if set_max_height_config := config.get(CONF_SET_MAX_HEIGHT):
        b = await button.new_button(set_max_height_config)
        await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_set_max_height_button(b))
    if set_min_height_config := config.get(CONF_SET_MIN_HEIGHT):
        b = await button.new_button(set_min_height_config)
        await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_set_min_height_button(b))

    if clear_max_height_config := config.get(CONF_CLEAR_MAX_HEIGHT):
        b = await button.new_button(clear_max_height_config)
        await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_clear_max_height_button(b))
    if clear_min_height_config := config.get(CONF_CLEAR_MIN_HEIGHT):
        b = await button.new_button(clear_min_height_config)
        await cg.register_parented(b, config[CONF_FULLY_JARVIS_F_ID])
        cg.add(fully_jarvis_f_component.set_clear_min_height_button(b))
