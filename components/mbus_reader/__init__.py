import esphome.codegen as cg
from esphome.components import uart

DEPENDENCIES = ["uart"]

mbus_reader_ns = cg.esphome_ns.namespace("mbus_reader")
MbusReader = mbus_reader_ns.class_("MbusReader", cg.Component, uart.UARTDevice)

async def to_code(config):
    uart_component = await cg.get_variable(config["uart_id"])
    var = cg.new_Pvariable(config[cg.cv.GenerateID()], uart_component)
    await cg.register_component(var, config)
