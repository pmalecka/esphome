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
CONF_UNIT_OF_MEASUREMENT = "unit_of_measurement"


# This validation runs after all components are loaded
def final_validate(config):
    # Validate sensor dependencies
    if CONF_PRESET_1_HEIGHT in config:
        if CONF_SYS_LIMIT_MIN_HEIGHT not in config:
            raise cv.Invalid(
                f"{CONF_SYS_LIMIT_MIN_HEIGHT} is required when {CONF_PRESET_1_HEIGHT} is defined."
            )

    # Validate select dependencies
    if CONF_UNIT_OF_MEASUREMENT not in config.get("select", {}):
        raise cv.Invalid(
            f"{CONF_UNIT_OF_MEASUREMENT} is required when {CONF_PRESET_1_HEIGHT} is defined."
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
