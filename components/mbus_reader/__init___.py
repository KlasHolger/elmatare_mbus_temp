import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

mbus_reader_ns = cg.esphome_ns.namespace("mbus_reader")
MbusReader = mbus_reader_ns.class_("MbusReader", cg.Component, uart.UARTDevice)