import esphome.codegen as cg
from esphome.components import uart
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
CODEOWNERS = ["@pmalecka"]
MULTI_CONF = True

fully_jarvis_f_ns = cg.esphome_ns.namespace("fully_jarvis_f")
FullyJarvisFComponent = fully_jarvis_f_ns.class_(
    "FullyJarvisFComponent", cg.Component, uart.UARTDevice
)

CONF_FULLY_JARVIS_F_ID = "fully_jarvis_f_id"

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(FullyJarvisFComponent),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)

CONF_PRESET_1_HEIGHT = "preset_1_height"
CONF_SYS_LIMIT_MIN_HEIGHT = "sys_limit_min_height"
CONF_UNITS_SELECT_ID = "units_select"


# This validation runs after all components are loaded
def final_validate(config):
    # Check all sensor configurations
    jarvis_sensors = [
        s for s in config.get("sensor", []) if s.get("platform") == "fully_jarvis_f"
    ]

    for sensor_conf in jarvis_sensors:
        if CONF_PRESET_1_HEIGHT in sensor_conf:
            # 1. Validate sys_limit_min_height in the same sensor block
            if CONF_SYS_LIMIT_MIN_HEIGHT not in sensor_conf:
                raise cv.Invalid(
                    f"'{CONF_SYS_LIMIT_MIN_HEIGHT}' is required when using '{CONF_PRESET_1_HEIGHT}'",
                    [sensor_conf],
                )

            # 2. Validate units_select in select components
            units_select_found = any(
                sel.get("id") == CONF_UNITS_SELECT_ID
                for sel in config.get("select", [])
                if sel.get("platform") == "fully_jarvis_f"
                and "unit_of_measurement" in sel
            )

            if not units_select_found:
                raise cv.Invalid(
                    f"Select component with id '{CONF_UNITS_SELECT_ID}' is required "
                    f"when using '{CONF_PRESET_1_HEIGHT}'",
                    [sensor_conf],
                )

    return config


FINAL_VALIDATE_SCHEMA = cv.All(
    CONFIG_SCHEMA,
    uart.final_validate_device_schema(
        "fully_jarvis_f",
        baud_rate=9600,
        require_tx=True,
        require_rx=True,
        parity="NONE",
        stop_bits=1,
    ),
    final_validate,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
