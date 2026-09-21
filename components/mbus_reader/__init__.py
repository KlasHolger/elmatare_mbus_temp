import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    CONF_ID,
    UNIT_AMPERE,
    UNIT_VOLT,
    UNIT_KILOWATT_HOURS,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
)

DEPENDENCIES = ["uart"]

mbus_reader_ns = cg.esphome_ns.namespace("mbus_reader")
MbusReader = mbus_reader_ns.class_("MbusReader", cg.Component, uart.UARTDevice)

CONF_WATTAGE_SENSOR = "wattage_sensor"
CONF_REACTIVE_POWER_SENSOR = "reactive_power_sensor"
CONF_AMPERAGE_L1_SENSOR = "amperage_l1_sensor"
CONF_AMPERAGE_L2_SENSOR = "amperage_l2_sensor"
CONF_AMPERAGE_L3_SENSOR = "amperage_l3_sensor"
CONF_VOLTAGE_L1_SENSOR = "voltage_l1_sensor"
CONF_VOLTAGE_L2_SENSOR = "voltage_l2_sensor"
CONF_VOLTAGE_L3_SENSOR = "voltage_l3_sensor"
CONF_ENERGY_SENSOR = "energy_sensor"
CONF_REACTIVE_ENERGY_SENSOR = "reactive_energy_sensor"


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MbusReader),
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
        cv.Optional(CONF_WATTAGE_SENSOR): sensor.sensor_schema(
            unit_of_measurement="kW",
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_REACTIVE_POWER_SENSOR): sensor.sensor_schema(
            unit_of_measurement="VAr",
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_AMPERAGE_L1_SENSOR): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_AMPERAGE_L2_SENSOR): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_AMPERAGE_L3_SENSOR): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_VOLTAGE_L1_SENSOR): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_VOLTAGE_L2_SENSOR): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_VOLTAGE_L3_SENSOR): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_ENERGY_SENSOR): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT_HOURS,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_REACTIVE_ENERGY_SENSOR): sensor.sensor_schema(
            unit_of_measurement="kVArh",
            accuracy_decimals=2,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    uart_component = await cg.get_variable(config["uart_id"])
    var = cg.new_Pvariable(config[CONF_ID], uart_component)
    await cg.register_component(var, config)

    sensor_setters = {
        CONF_WATTAGE_SENSOR: "set_wattage_sensor",
        CONF_REACTIVE_POWER_SENSOR: "set_reactive_power_sensor",
        CONF_AMPERAGE_L1_SENSOR: "set_amperage_l1_sensor",
        CONF_AMPERAGE_L2_SENSOR: "set_amperage_l2_sensor",
        CONF_AMPERAGE_L3_SENSOR: "set_amperage_l3_sensor",
        CONF_VOLTAGE_L1_SENSOR: "set_voltage_l1_sensor",
        CONF_VOLTAGE_L2_SENSOR: "set_voltage_l2_sensor",
        CONF_VOLTAGE_L3_SENSOR: "set_voltage_l3_sensor",
        CONF_ENERGY_SENSOR: "set_energy_sensor",
        CONF_REACTIVE_ENERGY_SENSOR: "set_reactive_energy_sensor",
    }

    for config_key, setter_name in sensor_setters.items():
        if config_key in config:
            sens = await sensor.new_sensor(config[config_key])
            cg.add(getattr(var, setter_name)(sens))
