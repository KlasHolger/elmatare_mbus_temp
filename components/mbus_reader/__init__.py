import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart

DEPENDENCIES = ["uart"]

mbus_reader_ns = cg.esphome_ns.namespace("mbus_reader")
MbusReader = mbus_reader_ns.class_("MbusReader", cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MbusReader),
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    uart_component = await cg.get_variable(config["uart_id"])
    var = cg.new_Pvariable(config[cv.GenerateID()], uart_component)
    await cg.register_component(var, config)
