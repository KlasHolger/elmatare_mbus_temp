#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include <cstring>

namespace esphome {
namespace mbus_reader {

// OBIS Code Mapping - Kamstrup Omnipower Meter
// 1.1.1.7.0.255  - Active Power (Output)         [kW]      * 0.001
// 1.1.31.7.0.255 - Current Phase L1              [A]       * 0.01
// 1.1.51.7.0.255 - Current Phase L2              [A]       * 0.01
// 1.1.71.7.0.255 - Current Phase L3              [A]       * 0.01
// 1.1.32.7.0.255 - Voltage Phase L1              [V]       * 1.0
// 1.1.52.7.0.255 - Voltage Phase L2              [V]       * 1.0
// 1.1.72.7.0.255 - Voltage Phase L3              [V]       * 1.0
// 1.1.1.8.0.255  - Active Energy Total           [kWh]     * 0.001
// 1.1.4.7.0.255  - Reactive Power                [VAr]     * 1.0
// 1.1.4.8.0.255  - Reactive Energy Cumulative    [kVArh]   * 0.01

class MbusReader : public Component, public uart::UARTDevice {
 public:
  MbusReader(uart::UARTComponent *parent) : uart::UARTDevice(parent) {}

  void set_wattage_sensor(sensor::Sensor *sensor) { wattage_sensor_ = sensor; }
  void set_reactive_power_sensor(sensor::Sensor *sensor) { reactive_power_sensor_ = sensor; }
  void set_amperage_l1_sensor(sensor::Sensor *sensor) { amperage_l1_sensor_ = sensor; }
  void set_amperage_l2_sensor(sensor::Sensor *sensor) { amperage_l2_sensor_ = sensor; }
  void set_amperage_l3_sensor(sensor::Sensor *sensor) { amperage_l3_sensor_ = sensor; }
  void set_voltage_l1_sensor(sensor::Sensor *sensor) { voltage_l1_sensor_ = sensor; }
  void set_voltage_l2_sensor(sensor::Sensor *sensor) { voltage_l2_sensor_ = sensor; }
  void set_voltage_l3_sensor(sensor::Sensor *sensor) { voltage_l3_sensor_ = sensor; }
  void set_energy_sensor(sensor::Sensor *sensor) { energy_sensor_ = sensor; }
  void set_reactive_energy_sensor(sensor::Sensor *sensor) { reactive_energy_sensor_ = sensor; }

  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::LATE; }

 private:
  sensor::Sensor *wattage_sensor_ = nullptr;
  sensor::Sensor *reactive_power_sensor_ = nullptr;
  sensor::Sensor *amperage_l1_sensor_ = nullptr;
  sensor::Sensor *amperage_l2_sensor_ = nullptr;
  sensor::Sensor *amperage_l3_sensor_ = nullptr;
  sensor::Sensor *voltage_l1_sensor_ = nullptr;
  sensor::Sensor *voltage_l2_sensor_ = nullptr;
  sensor::Sensor *voltage_l3_sensor_ = nullptr;
  sensor::Sensor *energy_sensor_ = nullptr;
  sensor::Sensor *reactive_energy_sensor_ = nullptr;

  uint8_t temp_byte_ = 0;
  uint8_t uart_buffer_[512]{0};
  uint16_t uart_counter_ = 0;
  char obis_code_[32];
  uint32_t obis_value_ = 0;

  bool read_message_();
  void parse_obis_data_(uint16_t index);
  void publish_obis_value_(const char *obis_code, uint32_t value);
};

}  // namespace mbus_reader
}  // namespace esphome
