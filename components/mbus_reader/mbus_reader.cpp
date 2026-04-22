#include "mbus_reader.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mbus_reader {

static const char *const TAG = "mbus_reader";

void MbusReader::setup() {
  ESP_LOGI(TAG, "M-Bus Reader initialized for Kamstrup Omnipower");
}

void MbusReader::loop() {
  read_message_();
}

void MbusReader::dump_config() {
  ESP_LOGCONFIG(TAG, "M-Bus Reader (Kamstrup Omnipower):");
  LOG_SENSOR("  ", "Wattage Sensor", this->wattage_sensor_);
  LOG_SENSOR("  ", "Reactive Power Sensor", this->reactive_power_sensor_);
  LOG_SENSOR("  ", "Amperage L1 Sensor", this->amperage_l1_sensor_);
  LOG_SENSOR("  ", "Amperage L2 Sensor", this->amperage_l2_sensor_);
  LOG_SENSOR("  ", "Amperage L3 Sensor", this->amperage_l3_sensor_);
  LOG_SENSOR("  ", "Voltage L1 Sensor", this->voltage_l1_sensor_);
  LOG_SENSOR("  ", "Voltage L2 Sensor", this->voltage_l2_sensor_);
  LOG_SENSOR("  ", "Voltage L3 Sensor", this->voltage_l3_sensor_);
  LOG_SENSOR("  ", "Energy Sensor", this->energy_sensor_);
  LOG_SENSOR("  ", "Reactive Energy Sensor", this->reactive_energy_sensor_);
}

bool MbusReader::read_message_() {
  while (available() >= 1) {
    read_byte(&temp_byte_);
    
    if (temp_byte_ == 126) {  // Frame end marker (0x7E)
      if (uart_counter_ > 2) {
        uart_buffer_[uart_counter_] = temp_byte_;
        uart_counter_++;

        // Parse OBIS codes from buffer
        for (uint16_t i = 0; i < uart_counter_ && i < 256; i++) {
          if (uart_buffer_[i - 1] == 9 && uart_buffer_[i] == 6) {
            parse_obis_data_(i);
          }
        }

        uart_counter_ = 0;
      } else {
        uart_counter_ = 0;
      }
    }
    
    uart_buffer_[uart_counter_] = temp_byte_;
    uart_counter_++;
  }

  return false;
}

void MbusReader::parse_obis_data_(uint16_t index) {
  obis_code_[0] = '\0';
  char temp_obis[10];

  // Construct OBIS code from buffer bytes
  for (uint16_t y = 1; y < 6; y++) {
    snprintf(temp_obis, sizeof(temp_obis), "%d.", uart_buffer_[index + y]);
    strncat(obis_code_, temp_obis, sizeof(obis_code_) - strlen(obis_code_) - 1);
  }
  snprintf(temp_obis, sizeof(temp_obis), "%d", uart_buffer_[index + 6]);
  strncat(obis_code_, temp_obis, sizeof(obis_code_) - strlen(obis_code_) - 1);

  ESP_LOGD(TAG, "OBIS code found: %s, data type: %d", obis_code_, uart_buffer_[index + 7]);

  obis_value_ = 0;

  // Parse value based on data type
  // Type 6 = 4-byte unsigned integer
  if (uart_buffer_[index + 7] == 6) {
    for (uint8_t y = 0; y < 4; y++) {
      obis_value_ += (uint32_t)uart_buffer_[index + 8 + y] << ((3 - y) * 8);
    }
  }
  // Type 18 = 2-byte unsigned integer
  else if (uart_buffer_[index + 7] == 18) {
    for (uint8_t y = 0; y < 2; y++) {
      obis_value_ += (uint32_t)uart_buffer_[index + 8 + y] << ((1 - y) * 8);
    }
  }

  publish_obis_value_(obis_code_, obis_value_);
}

void MbusReader::publish_obis_value_(const char *obis_code, uint32_t value) {
  // Active Power (kW) - OBIS 1.1.1.7.0.255
  if (strcmp(obis_code, "1.1.1.7.0.255") == 0) {
    ESP_LOGD(TAG, "Wattage: %lu W", value);
    if (wattage_sensor_) {
      wattage_sensor_->publish_state(value * 0.001f);
    }
  }
  // Current L1 (A) - OBIS 1.1.31.7.0.255
  else if (strcmp(obis_code, "1.1.31.7.0.255") == 0) {
    ESP_LOGD(TAG, "Current L1: %lu (raw)", value);
    if (amperage_l1_sensor_) {
      amperage_l1_sensor_->publish_state(value * 0.01f);
    }
  }
  // Current L2 (A) - OBIS 1.1.51.7.0.255
  else if (strcmp(obis_code, "1.1.51.7.0.255") == 0) {
    ESP_LOGD(TAG, "Current L2: %lu (raw)", value);
    if (amperage_l2_sensor_) {
      amperage_l2_sensor_->publish_state(value * 0.01f);
    }
  }
  // Current L3 (A) - OBIS 1.1.71.7.0.255
  else if (strcmp(obis_code, "1.1.71.7.0.255") == 0) {
    ESP_LOGD(TAG, "Current L3: %lu (raw)", value);
    if (amperage_l3_sensor_) {
      amperage_l3_sensor_->publish_state(value * 0.01f);
    }
  }
  // Voltage L1 (V) - OBIS 1.1.32.7.0.255
  else if (strcmp(obis_code, "1.1.32.7.0.255") == 0) {
    ESP_LOGD(TAG, "Voltage L1: %lu V", value);
    if (voltage_l1_sensor_) {
      voltage_l1_sensor_->publish_state(value);
    }
  }
  // Voltage L2 (V) - OBIS 1.1.52.7.0.255
  else if (strcmp(obis_code, "1.1.52.7.0.255") == 0) {
    ESP_LOGD(TAG, "Voltage L2: %lu V", value);
    if (voltage_l2_sensor_) {
      voltage_l2_sensor_->publish_state(value);
    }
  }
  // Voltage L3 (V) - OBIS 1.1.72.7.0.255
  else if (strcmp(obis_code, "1.1.72.7.0.255") == 0) {
    ESP_LOGD(TAG, "Voltage L3: %lu V", value);
    if (voltage_l3_sensor_) {
      voltage_l3_sensor_->publish_state(value);
    }
  }
  // Active Energy Total (kWh) - OBIS 1.1.1.8.0.255
  else if (strcmp(obis_code, "1.1.1.8.0.255") == 0) {
    ESP_LOGD(TAG, "Energy: %lu (raw)", value);
    if (energy_sensor_) {
      energy_sensor_->publish_state(value * 0.001f);
    }
  }
  // Reactive Power (VAr) - OBIS 1.1.4.7.0.255
  else if (strcmp(obis_code, "1.1.4.7.0.255") == 0) {
    ESP_LOGD(TAG, "Reactive Power: %lu VAr", value);
    if (reactive_power_sensor_) {
      reactive_power_sensor_->publish_state(value);
    }
  }
  // Reactive Energy (kVArh) - OBIS 1.1.4.8.0.255
  else if (strcmp(obis_code, "1.1.4.8.0.255") == 0) {
    ESP_LOGD(TAG, "Reactive Energy: %lu (raw)", value);
    if (reactive_energy_sensor_) {
      reactive_energy_sensor_->publish_state(value * 0.01f);
    }
  }
  else {
    ESP_LOGW(TAG, "Unknown OBIS code: %s, value: %lu", obis_code, value);
  }
}

}  // namespace mbus_reader
}  // namespace esphome