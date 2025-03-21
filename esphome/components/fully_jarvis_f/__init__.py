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


# This validation runs after all components are loaded
def final_validate(config):
    # Check if any sensor has preset_1_height ID
    has_preset = any(s.get("id") == "preset_1_height" for s in config.get("sensor", []))

    if has_preset:
        errors = []

        # Check for sys_limit_min_height sensor
        if not any(
            s.get("id") == "sys_limit_min_height" for s in config.get("sensor", [])
        ):
            errors.append("sys_limit_min_height sensor")

        # Check for units_select in select components
        if not any(s.get("id") == "units_select" for s in config.get("select", [])):
            errors.append("units_select component")

        if errors:
            raise cv.Invalid(
                f"Required when using preset_x_height: {', '.join(errors)}"
            )
    return config


FINAL_VALIDATE_SCHEMA = cv.All(
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
