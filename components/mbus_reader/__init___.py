import esphome.codegen as cg
from esphome.components import uart

DEPENDENCIES = ["uart"]

mbus_reader_ns = cg.esphome_ns.namespace("mbus_reader")
MbusReader = mbus_reader_ns.class_("MbusReader", cg.Component, uart.UARTDevice)

# Expose the class for sensor.py to import
__all__ = ["mbus_reader_ns", "MbusReader"]